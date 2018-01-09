#ifndef WPP_VMCODE_H
#define WPP_VMCODE_H

#include <vector>
using namespace std;

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
struct ParamOprand : public Oprand
{
	int index;
	ParamOprand():Oprand(OprandType::param_){}
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


//define object of code
struct DVMCode
{
};
struct  IVMCode
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
class VMCode : public IVMCode
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

} //namespace vm

#endif //WPP_XVM_H