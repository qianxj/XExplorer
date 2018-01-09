#ifndef WPP_MILL_CODE_H
#define WPP_MILL_CODE_H

#include "vlang3.h"
#include "mill_const.hpp"

#include <vector>
using namespace std;

/*
1、Mill的操作对象:
	a、Mill机器操作的对象包括寄存器、变量、参数、以及直接数，所有的操作对象包括寄存器
	   都还有类型。
	b、寄存器数量不受限制，但类型必须是原子类型，只有明确标出地址类型的寄存器才能
	   才能进行对内存的存取操作。
	c、变量和参数既可以是源地址，也可以是目标地址，作为目标地址可以直接修改数据。
	d、代码表示时一般使用如下的书写习惯:
		register <- r0..rn	
		v0		 <- this
		v1..vn   <- variant
		x1..xn	 <- param
		c1..cn   <- comp
		liter    <- direct
		link     <- jmp link

2、Mill的代码格式:
	a、Mill的代码由操作码(opcode)和操作数组成,其中操作数
	b、操作码和操作数的构成
	opcode  ooooooxa
	opcode1 aabbbccc
	oprand1 rrrrrrrr
	oprand2 rrrrrrrr
	oprand3 rrrrrrrr

	c、位标志:
	   x:   1 三操作数。
	        0 其他。
	   aaa: 指定操作数1的操作对象。
		    对于三操作数，目标操作数只能是register|variant|param|comp。
		    对于其他，则借用c的高位作为a的高位。
	   bbb: 操作数2的目标。
	   ccc: 操作数3的目标。
	   
	   备注:对于有些指令，位标志有不同的解释，具体在描述具体指令时给出。
*/
namespace mill
{

struct opcode
{
	//nop
	static const unsigned char nop   = 0x00;

	//mov
	static const unsigned char mov   = 0x01;

	//math 
	static const unsigned char add   = 0x02;
	static const unsigned char sub   = 0x03;
	static const unsigned char mul   = 0x04;
	static const unsigned char div   = 0x05;

	//logic
	static const unsigned char or    = 0x08;
	static const unsigned char and   = 0x09;

	//bit operate
	static const unsigned char xor   = 0x0A;
	static const unsigned char not   = 0x0B;
	static const unsigned char neg   = 0x0C;
	static const unsigned char shl   = 0x0D;
	static const unsigned char shr   = 0x0E;

	//call
	static const unsigned char ret   = 0x0F;
	static const unsigned char call  = 0x10;
	static const unsigned char push  = 0x11;
	static const unsigned char pop   = 0x12;

	//inc, dec
	static const unsigned char inc   = 0x13;
	static const unsigned char dec   = 0x14;

	//comp jmp
	static const unsigned char jmp   = 0x15;
	static const unsigned char jz	 = 0x16;
	static const unsigned char jnz   = 0x17;
	static const unsigned char jgt	 = 0x18;
	static const unsigned char jge	 = 0x19;
	static const unsigned char jlt   = 0x1A;
	static const unsigned char jle   = 0x1B;
	static const unsigned char jeq   = 0x1C;
	static const unsigned char jne   = 0x1D;
	static const unsigned char zero  = 0x20;
	
	//comp oprate
	static const unsigned char nil   = 0x21;
	static const unsigned char gtr   = 0x22;
	static const unsigned char gte   = 0x23;
	static const unsigned char ltr   = 0x24;
	static const unsigned char lte   = 0x25;
	static const unsigned char eq    = 0x26;
	static const unsigned char neq   = 0x27;

	//debug
	static const unsigned char br	 = 0x30;

	//xform
	static const unsigned char yadd   = 0x40;
	static const unsigned char ysub   = 0x41;
	static const unsigned char ymul   = 0x42;
	static const unsigned char ydiv   = 0x43;
	static const unsigned char yor    = 0x44;
	static const unsigned char yand   = 0x45;
	static const unsigned char yeq	  = 0x46;
	static const unsigned char yseq	  = 0x47;
};

struct OprandType
{
	static const unsigned char register_	= 0x00;
	static const unsigned char var_			= 0x01;
	static const unsigned char arg_			= 0x02;
	static const unsigned char comp_		= 0x03;
	static const unsigned char number_		= 0x04;
	static const unsigned char link_		= 0x05;
};

//define vm item
struct Oprand
{
	unsigned char what_;
	Oprand(int what):what_(what){};
	virtual int get_index() = 0;
};

//注册表项
struct RegOprand : public Oprand
{
	int index;
	RegOprand():Oprand(OprandType::register_){}
	int get_index(){return index;}
};
//变量项
struct VarOprand : public Oprand
{
	int index;
	VarOprand():Oprand(OprandType::var_){}
	int get_index(){return index;}
};
//参数项
struct ArgOprand : public Oprand
{
	int index;
	ArgOprand():Oprand(OprandType::arg_){}
	int get_index(){return index;}
};
//复合项
struct CompOprand : public Oprand
{
	CompOprand():Oprand(OprandType::comp_){}
};
//直接数
struct NumOprand : public Oprand
{
	NumOprand():Oprand(OprandType::number_){}
};

struct Label
{
	int dest;
	vector<int> links;
};

//链接
struct LinkOprand : public Oprand
{
	int addr;
	LinkOprand():Oprand(OprandType::link_){}
	void linkTo(Label & label)
	{
		label.links.push_back(addr);
	}
};

class VLANG3_API mill_code
{
public:
	unsigned char *  code;
	unsigned char *  pc;
	int codeLen;
public:
	mill_code();
	~mill_code();
public:
	void ensureFree();
public:
	virtual void enterCode(unsigned char code,Oprand & op0,Oprand & op1,Oprand & op2);
	virtual void enterCode(unsigned char code,Oprand & op0,Oprand & op1);
	virtual void enterCode(unsigned char code,Oprand & op0);
	virtual void enterCode(unsigned char code);
public:
	virtual void label(Label &l);
public:
	static unsigned char encode(unsigned char code, int opcount);
	static void decode(unsigned char data, unsigned char & code, int & opcount);
public:
	static void deoprand(unsigned char data,unsigned char woprand[3]);
	static int deindex(unsigned char * & pc);
public:
	template<class T>
	unsigned char * pushNumber(T d)
	{
		*(T *)pc = d;
		return pc + sizeof(T);
	}
	unsigned char * pushOprand(Oprand & op);
};


}//namespace opcode

#endif
