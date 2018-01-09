#ifndef WPP_XVM_H
#define WPP_XVM_H

#include "word.hpp"
#include <deque>
#include <vector>

using namespace wpp::how;
using namespace std;

#define __IClass(x) public x

namespace vm
{

namespace OprandType
{
	const unsigned char 
	register_ = 0x00,
	var_	  = 0x01,
	param_	  = 0x02,
	comp_	  = 0x03,
	number_	  = 0x04,
	link_	  = 0x05;
};

//define vm item
struct Oprand
{
	unsigned char what_;
	Oprand(int what):what_(what){};
	virtual int getIndex() = 0;
};

//注册表项
struct RegOprand : public Oprand
{
	int index;
	RegOprand():Oprand(OprandType::register_){}
	int getIndex(){return index;}
};
//变量项
struct VarOprand : public Oprand
{
	int index;
	VarOprand():Oprand(OprandType::var_){}
	int getIndex(){return index;}
};
//参数项
struct ParamOprand : public Oprand
{
	int index;
	ParamOprand():Oprand(OprandType::param_){}
	int getIndex(){return index;}
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





//define object of code
struct DVMCode
{
};
struct __declspec(novtable) IVMCode
{
public:
	virtual void enterCode(unsigned char code,Oprand & op0,Oprand & op1,Oprand & op2)=0;
	virtual void enterCode(unsigned char code,Oprand & op0,Oprand & op1)=0;
	virtual void enterCode(unsigned char code,Oprand & op0)=0;
	virtual void enterCode(unsigned char code)=0;
public:
	virtual void label(Label &l)=0;
};

//interpretation code
class VMCode : __IClass(IVMCode)
{
public:
	VMCode();
	~VMCode();
public:
	unsigned char *  code;
	unsigned char *  pc;
	int codeLen;
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
	unsigned char encode(unsigned char code, int opcount);
	void decode(unsigned char data, unsigned char & code, int & opcount);
public:
	void deoprand(unsigned char data,unsigned char woprand[3]);
public:
	int toCode(unsigned char * start, unsigned char * end);
public:
	template<class T>
	unsigned char * pushNumber(unsigned char * data, T d)
	{
		*(T *)data = d;
		return data + sizeof(T);
	}
	unsigned char * pushOprand(unsigned char * data,Oprand & op);
};

//虚拟机
class VMRavine
{
public:
	struct RegItem
	{
		int type;
		CWordValue val;
	};
public:
	VMRavine();
	~VMRavine();
public:
	//registers
	vector<RegItem>* registers;
	//stack
	unsigned char* stacks;
	unsigned char* stacksType;
public:
	VMCode* codePtr;
public:
	void procMove(int opcount);
	void procOperate(int opcount);
	void procCall(int opcount);
	void procRet(int opcount);
public:
	int call(int pc);
	int eval(int fn);
};

} //namespace vm

#endif //WPP_XVM_H