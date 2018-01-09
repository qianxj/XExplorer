#ifndef WPP_CODE_H
#define WPP_CODE_H

#include "vlang3.h"
#include "type.hpp"
#include <string>


namespace mill{

#define code_t static const unsigned char 
struct vill
{
	//nop
	code_t nop  = 0x08;
	code_t exit = 0x01;
	code_t ulen = 0x02;

	//math
	code_t add  = 0x10;
	code_t sub  = 0x11;
	code_t mul  = 0x12;
	code_t div  = 0x13;
	code_t mod  = 0x14;
	code_t xor  = 0x15;
	code_t band = 0x16;
	code_t bor  = 0x17;
	code_t shl  = 0x18;
	code_t shr  = 0x19;
	code_t bnot = 0x1A;
	code_t neg  = 0x1B;
	code_t cast = 0x1F;
	
	code_t dec  = 0x20;
	code_t inc  = 0x21;

	code_t or   = 0x22;
	code_t and  = 0x23;
	code_t not  = 0x24;
	code_t test = 0x25;
	code_t sadd	= 0x28;
	code_t tadd	= 0x29;
	code_t tsub	= 0x2A;

	//comp
	code_t gtr  = 0x30;
	code_t gte  = 0x31;
	code_t ltr  = 0x32;
	code_t lte  = 0x33;
	code_t nil  = 0x34;
	code_t eq   = 0x35;
	code_t neq  = 0x36;

	//jmp
	code_t jmp   = 0x40;
	code_t jz	 = 0x41;
	code_t jnz   = 0x42;
	code_t jgt	 = 0x43;
	code_t jge	 = 0x44;
	code_t jlt   = 0x45;
	code_t jle   = 0x46;
	code_t jeq   = 0x47;
	code_t jne   = 0x48;
	code_t jzero = 0x49;

	//mov
	code_t mov   = 0x51;
	code_t mova  = 0x52;
	code_t load  = 0x53;
	code_t store = 0x54;	
	code_t push  = 0x55;
	code_t pop   = 0x56;

	code_t opush = 0x57;

	//call ret
	code_t call  = 0x5A;
	code_t ret   = 0x5B;

	//new 、delete
	code_t alloc		= 0x60;
	code_t free			= 0x61;
	//code_t element	= 0x64;
	//code_t field		= 0x65;

	//debug
	code_t br			= 0xC3;
	code_t trace		= 0xC4;

	//pred
	code_t setp			= 0x70;	//setpred
	code_t getp			= 0x71;	//getpred
	code_t appendp		= 0x72; //appendpred

	static const wchar_t * code_string(const unsigned char cd);
};
#undef code_t

struct imm
{
	struct u8 
	{
		unsigned char val_;
		u8(unsigned char val):val_(val){}
	};
	struct u16 
	{
		unsigned short val_;
		u16(unsigned short val):val_(val){}
	};
	struct u32
	{
		unsigned int val_;
		u32(unsigned int val):val_(val){}
	};
	struct u64 
	{
		unsigned __int64 val_;
		u64(unsigned __int64 val):val_(val){}
	};
	struct i8 
	{
		char val_;
		i8(char val):val_(val){}
	};
	struct i16 
	{
		short val_;
		i16(short val):val_(val){}
	};
	struct i32 
	{
		int val_;
		i32(int val):val_(val){}
	};
	struct i64
	{
		__int64 val_;
		i64(__int64 val):val_(val){}
	};
	struct f32 
	{
		float val_;
		f32(float val):val_(val){}
	};
	struct f64 
	{
		double val_;
		f64(double val):val_(val){}
	};
	struct f80 
	{
		long double val_;
		f80(long double val):val_(val){}
	};
	struct str 
	{
		wchar_t*  val_;
		str(wchar_t* val):val_(val){}
	};
	struct ptr
	{
		void* val_;
		ptr(void* val):val_(val){}
	};
};
typedef unsigned char vill_oprand_type_t;
struct vill_oprand_type
{
	static const unsigned char u8  = 0;
	static const unsigned char u16 = 1;
	static const unsigned char u32 = 2;
	static const unsigned char u64 = 3;
	static const unsigned char i8  = 4;
	static const unsigned char i16 = 5;
	static const unsigned char i32 = 6;
	static const unsigned char i64 = 7;
	static const unsigned char f32 = 8;
	static const unsigned char f64 = 9;
	static const unsigned char f80 = 0xa;
	static const unsigned char str = 0xb;
	static const unsigned char ptr = 0xc;
	static const unsigned char void_= 0xd;
	static const unsigned char ebp = 0xe;
	static const unsigned char esp = 0xf;

	static const unsigned char imm  = 0x10; //直接数
	static const unsigned char oprl	= 0x20; //长操作数

	static vill_oprand_type_t get_type(wpp::type_t ty);
	static const wchar_t * opt_string(vill_oprand_type_t opt);
};

typedef unsigned char vill_call_type_t;
struct vill_call_type
{
	static const unsigned char call		= 0x01;	//call
	static const unsigned char vcall	= 0x02;	//virtual call
	static const unsigned char ncall	= 0x03; //native call
	static const unsigned char ocall	= 0x04; //ole call

	static const unsigned char cdcall	= 0x00; //C调用
	static const unsigned char stdcall	= 0x10; //标准调用
	static const unsigned char thiscall	= 0x20; //this调用
	static const unsigned char theadcall= 0x40; //线程调用
	static const unsigned char addr     = 0x80; //直接地址
};

typedef unsigned char vill_ocall_type_t;
struct vill_ocall_type
{
	static const unsigned char propget = 0x01;
	static const unsigned char propput = 0x02;
	static const unsigned char method  = 0x03;
};

} //namespace mill

#endif //WPP_CODE_H