#ifndef WPP_CODEGEN_IA32_H
#define WPP_CODEGEN_IA32_H

#include "vm.hpp"
#include "nativeasm_ia32.hpp"

namespace vm
{

class ICodeGen
{
public:
	virtual void genMov(unsigned char * & pc, int opcount) = 0;
	virtual void genOp(unsigned char  op,unsigned char * & pc, int opcount)=0;
	virtual void genJump(unsigned char op,unsigned char * & pc, int opcount)=0;
	virtual void genCall(unsigned char * & pc, int opcount)=0;
	virtual void genRet(unsigned char * & pc, int opcount)=0;
	virtual void genPush(unsigned char * & pc, int opcount)=0;
	virtual void genPop(unsigned char * & pc, int opcount)=0;
public:
	virtual int genFn(int fn, unsigned char *  pc,unsigned char *  end)=0;
};

class CodeGen
{
public:
	Class * class_;
	Method * method_;
public:
	//virtual register
	int regsType[255];
	int	regsMap[255];
	
	//link to virtual
	//index when > regRealCount for memory else for native register
	int regsUsed[255];
	int fregsUsed[255];
	//register used
	int regUsed_;
	int fregUsed_;
	//native register resource
	int regRealCount;
	int fregRealCount;
	int regs[16];
	int fregs[16];
private:
	IVMTable * vmtable_;
private:

public:
	int  exchToNReg(int nIndex,int noReg = 0);
	int  askReg(int notReg  = 0);
	int  askFReg();
	void freeReg(int nIndex);
	void freeNReg(int nIndex);
	void usedReg(int vreg,int reg);
public:
	const Var*   getVarItem(int nVar) const;
	const Param* getParamItem(int nParam) const;
	const Field* getFieldItem(int nField) const;

public:
	CodeGen(IVMTable * metaTable);
public:
	virtual void genMov(unsigned char * & pc, int opcount);
	virtual void genOp(unsigned char  op,unsigned char * & pc, int opcount);
	virtual void genOp1(unsigned char  op,unsigned char * & pc);
	virtual void genOp2(unsigned char  op,unsigned char * & pc);
	virtual void genOp3(unsigned char  op,unsigned char * & pc);
	virtual void genJump(unsigned char op,unsigned char * & pc, int opcount);
	virtual void genCall(unsigned char * & pc, int opcount);
	virtual void genRet(unsigned char * & pc, int opcount);
	virtual void genPush(unsigned char * & pc, int opcount);
	virtual void genPop(unsigned char * & pc, int opcount);
public:
	virtual int genFn(int fn, unsigned char *  pc,unsigned char *  end);
};

} //namespace vm
#endif //WPP_CODEGEN_IA32_H