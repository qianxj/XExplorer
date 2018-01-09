#ifndef WPP_MILL_CODE_H
#define WPP_MILL_CODE_H

#include "vlang3.h"
#include "mill_const.hpp"

#include <vector>
using namespace std;

/*
1��Mill�Ĳ�������:
	a��Mill���������Ķ�������Ĵ������������������Լ�ֱ���������еĲ�����������Ĵ���
	   ���������͡�
	b���Ĵ��������������ƣ������ͱ�����ԭ�����ͣ�ֻ����ȷ�����ַ���͵ļĴ�������
	   ���ܽ��ж��ڴ�Ĵ�ȡ������
	c�������Ͳ����ȿ�����Դ��ַ��Ҳ������Ŀ���ַ����ΪĿ���ַ����ֱ���޸����ݡ�
	d�������ʾʱһ��ʹ�����µ���дϰ��:
		register <- r0..rn	
		v0		 <- this
		v1..vn   <- variant
		x1..xn	 <- param
		c1..cn   <- comp
		liter    <- direct
		link     <- jmp link

2��Mill�Ĵ����ʽ:
	a��Mill�Ĵ����ɲ�����(opcode)�Ͳ��������,���в�����
	b��������Ͳ������Ĺ���
	opcode  ooooooxa
	opcode1 aabbbccc
	oprand1 rrrrrrrr
	oprand2 rrrrrrrr
	oprand3 rrrrrrrr

	c��λ��־:
	   x:   1 ����������
	        0 ������
	   aaa: ָ��������1�Ĳ�������
		    ��������������Ŀ�������ֻ����register|variant|param|comp��
		    ���������������c�ĸ�λ��Ϊa�ĸ�λ��
	   bbb: ������2��Ŀ�ꡣ
	   ccc: ������3��Ŀ�ꡣ
	   
	   ��ע:������Щָ�λ��־�в�ͬ�Ľ��ͣ���������������ָ��ʱ������
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

//ע�����
struct RegOprand : public Oprand
{
	int index;
	RegOprand():Oprand(OprandType::register_){}
	int get_index(){return index;}
};
//������
struct VarOprand : public Oprand
{
	int index;
	VarOprand():Oprand(OprandType::var_){}
	int get_index(){return index;}
};
//������
struct ArgOprand : public Oprand
{
	int index;
	ArgOprand():Oprand(OprandType::arg_){}
	int get_index(){return index;}
};
//������
struct CompOprand : public Oprand
{
	CompOprand():Oprand(OprandType::comp_){}
};
//ֱ����
struct NumOprand : public Oprand
{
	NumOprand():Oprand(OprandType::number_){}
};

struct Label
{
	int dest;
	vector<int> links;
};

//����
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
