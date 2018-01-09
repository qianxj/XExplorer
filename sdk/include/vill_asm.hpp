#ifndef WPP_VILL_ASM_H
#define WPP_VILL_ASM_H

#include "vlang3.h"
#include "type.hpp"
#include <string>
#include "vill_code.hpp"

namespace mill{

struct vill_label
{
	int dest;
	vector<int> links;
	vill_label():dest(0){};
};

class VLANG3_API vill_asm
{
public:
	typedef  unsigned char reg_t;
public:
	unsigned char *  code;
	unsigned char *  pc;
	int codeLen;
public:
	vill_asm();
	~vill_asm();
public:
	void ensureFree();
public:
	void emit_code(unsigned char buf[],int len);
	unsigned char get_call_opt(int fn);
public:
	void nop();
	void exit(); 
	void br();
	//上一代码长度可用于反向查找
	void ulen(unsigned char size);
public:
	void neg(vill_oprand_type_t opt, reg_t r0);
	void neg(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void bnot(vill_oprand_type_t opt,reg_t r0);
	void bnot(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void add(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void add(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void sub(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void sub(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void mul(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void mul(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void div(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void div(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void mod(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void mod(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void xor(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void xor(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void band(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void band(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void bor(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void bor(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void shr(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void shr(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void shl(vill_oprand_type_t opt, reg_t r0,reg_t r1);
	void shl(vill_oprand_type_t opt, reg_t r0,reg_t r1,reg_t r2);
	void sadd(reg_t r0,reg_t r1);
	void sadd(reg_t r0,reg_t r1,reg_t r2);
	void tadd(reg_t r0,reg_t r1);
	void tadd(reg_t r0,reg_t r1,reg_t r2);
	void tsub(reg_t r0,reg_t r1);
	void tsub(reg_t r0,reg_t r1,reg_t r2);

	void or(reg_t r0,reg_t r1);
	void or(reg_t r0,reg_t r1,reg_t r2);
	void and(reg_t r0,reg_t r1);
	void and(reg_t r0,reg_t r1,reg_t r2);
	void not(reg_t r0);
	void not(reg_t r0,reg_t r1);

	void binary(unsigned char cd, vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void binary(unsigned char cd, vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void unary( unsigned char cd, vill_oprand_type_t opt,reg_t r0);
	void unary( unsigned char cd, vill_oprand_type_t opt,reg_t r0,reg_t r1);

	void cast(vill_oprand_type_t opt_dest,vill_oprand_type_t opt_src,reg_t r0);
	void cast(vill_oprand_type_t opt_dest,vill_oprand_type_t opt_src,reg_t  r0,reg_t r1);

	void mov(vill_oprand_type_t,reg_t r0,reg_t r1);
	void mov(reg_t r0,	imm::u8  v);
	void mov(reg_t r0,	imm::u16 v);
	void mov(reg_t r0,	imm::u32 v);
	void mov(reg_t r0,	imm::u64 v);
	void mov(reg_t r0,	imm::i8  v);
	void mov(reg_t r0,	imm::i16 v);
	void mov(reg_t r0,	imm::i32 v);
	void mov(reg_t r0,	imm::i64 v);
	void mov(reg_t r0,	imm::f32 v);
	void mov(reg_t r0,	imm::f64 v);
	void mov(reg_t r0,	imm::f80 v);
	void mov(reg_t r0,	imm::str v);
	void mov(reg_t r0,	imm::ptr v);
	void movsp(reg_t r0,imm::i16 inc);
	void movbp(reg_t r0,imm::i16 inc);

	void dec(vill_oprand_type_t opt,reg_t r0);
	void dec(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void inc(vill_oprand_type_t opt,reg_t r0);
	void inc(vill_oprand_type_t opt, reg_t r0,reg_t r1);

	void gtr(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void gtr(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void gte(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void gte(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void ltr(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void ltr(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void lte(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void lte(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void nil(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void nil(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void eq(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void eq(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);
	void neq(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void neq(vill_oprand_type_t opt,reg_t r0,reg_t r1,reg_t r2);

	//call ret
	void call(unsigned int fn,reg_t r0,unsigned short argsize,vill_ocall_type_t otyp = 0);
	void ret(unsigned short psize);
	//push pop
	void push(vill_oprand_type_t opt,reg_t r0);
	void pop(vill_oprand_type_t opt,reg_t r0);

	//load 、stroe
	void load(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	void store(vill_oprand_type_t opt,reg_t r0,reg_t r1);
	
	//alloc 、free
	void alloc(int wc,reg_t r0);
	void free(reg_t r0);

	//jmp
	void jmp(short rel);
	void jz (short rel);
	void jnz(short rel);
	void jgt(short rel);
	void jge(short rel);
	void jlt(short rel);
	void jle(short rel);
	void jeq(short rel);
	void jne(short rel);
	void jzero(short rel);

	//label
	void jmp(vill_label& l);
	void jz(vill_label& l);
	void jnz(vill_label& l);
	void jgt(vill_label& l);
	void jge(vill_label& l);
	void jlt(vill_label& l);
	void jle(vill_label& l);
	void jeq(vill_label& l);
	void jne(vill_label& l);
	void jzero(vill_label& l);

	void jmp(unsigned char cd, vill_label& l);
	void jmp(unsigned char cd,short rel);
	
	void test(vill_oprand_type_t opt,reg_t r0);
	void label(vill_label& l);
	void labelas(vill_label& l,vill_label& sl);

	//support ole
	void opush(vill_oprand_type_t opt,unsigned short vt, reg_t r0);

	void trace(reg_t r0);

	//pred support
	void setpred(vill_oprand_type_t opt,reg_t r0,int pd, reg_t r1);
	void getpred(vill_oprand_type_t opt,reg_t r0,int pd,reg_t r1);
	void appendpred(vill_oprand_type_t opt,reg_t r0,int pd, reg_t r1);

};

} //namespace mill

#endif //WPP_VILL_ASM_H