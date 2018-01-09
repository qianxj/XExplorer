#ifndef WPP_NATIVEASM32_H
#define WPP_NATIVEASM32_H

#include "globals.hpp"
#include "nativeasm.hpp"
using namespace wpp::how;

namespace vm { 

struct Register
{
	int r_;
	bool is(Register r){return r.r_==r_;}
	operator const unsigned char () const {return (const unsigned char)r_;}
	operator const int () const {return r_;}
};

const Register 
	no_reg = { -1 },
	eax = { 0 },
	ecx = { 1 },
	edx = { 2 },
	ebx = { 3 },
	esp = { 4 },
	ebp = { 5 },
	esi = { 6 },
	edi = { 7 };

struct XMMRegister 
{
	int r_;
	bool is(XMMRegister r){return r.r_==r_;}
	operator const unsigned char () const {return (const unsigned char)r_;}
	operator const int () const {return r_;}
};

const XMMRegister
	xmm0 = {0},
	xmm1 = {0},
	xmm2 = {0},
	xmm3 = {0},
	xmm4 = {0},
	xmm5 = {0},
	xmm6 = {0},
	xmm7 = {0};

struct ModRM
{
	unsigned char rm_:3;
	unsigned char reg_:3;
	unsigned char mod_:2;

	ModRM():rm_(0),reg_(0),mod_(0){}
	ModRM(const unsigned char mod,const unsigned char reg,const unsigned char rm):
		rm_(rm),reg_(reg),mod_(mod){}
};

struct Scaled
{
	const static unsigned char time_1 = 0;
	const static unsigned char time_2 = 1;
	const static unsigned char time_4 = 2;
	const static unsigned char time_8 = 3;
};

struct SIB
{
	unsigned char base_:3;
	unsigned char index_:3;
	unsigned char scale_:2;

	SIB():base_(0),index_(0),scale_(0){}
	SIB(const unsigned char scale,const unsigned char index,const unsigned char base):
		base_(base),index_(index),scale_(scale){}
};

struct Immediate
{
	int v_;

	inline bool is_zero() const { return v_ == 0; }
	inline bool is_int8() const { return -128 <= v_ && v_ < 128; }
	inline bool is_int16()const { return -32768 <= v_ && v_ < 32768;}
	operator int () {return v_;}
	operator char () {return (char)v_;}
	operator short () {return (short)v_;}
	Immediate():v_(0){}
	Immediate(int v):v_(v){}
};

struct Displacement
{
	union 
	{
		char	d8_;
		short	d16_;
		int		d32_;
	};
};



struct Addr
{
public:
	inline explicit Addr(Register r)
	{
		this->len_ = 1;
		modrm_ = ModRM(3,0,r);
	}
	inline explicit Addr(XMMRegister x)
	{
		this->len_ = 1;
		modrm_ = ModRM(3,0,x);
	}
	inline explicit Addr(int disp)
	{
		this->len_ = 5;
		modrm_ = ModRM(0,0,ebp);
		disp_.d32_ = disp;
	}
	explicit Addr(Register r,Immediate disp);
	explicit  Addr(SIB sib,Immediate disp);
	explicit Addr(Register index,const unsigned char scale,Immediate disp);
public:
	inline void encoder(Register r){modrm_.reg_ = r;}
	inline unsigned char mod(){return buf_[0];}
	inline unsigned char rm(){return buf_[1];}

	union
	{
		unsigned char buf_[6];
		struct
		{
			ModRM modrm_;
			SIB	  sib_;
			Displacement disp_;
		};
	};

	int len_;
};



class DAssembler
{
	unsigned char* buffer_;
	int buffer_size_;

	unsigned char* current_; 

public:
	DAssembler(unsigned char* buffer, int buffer_size):
		buffer_(buffer),buffer_size_(buffer_size),current_(buffer){}
	inline unsigned char* buffer(){return buffer_;}
	inline int bufferLength(){ return buffer_size_;}
	inline unsigned char* & current(){return current_;}
	inline int bufferUsed() {return (int)(current_ - buffer_);}
	inline int bufferFree() {return (int)(buffer_size_ - (current_ - buffer_));}
	inline void emit_b(unsigned char cd){	*current_++ = cd;}
	inline void emit(unsigned int imm)
	{
		*(unsigned int *)current_ = imm;
		current_ += sizeof(unsigned int);
	}
	inline void emit(Addr & addr)
	{

	}
	void growBuffer();
};

class IAssembler
{
public:
	static const int ctOneCodeSize = 32;
	//void ensurceSpace();
};

class Assembler : public IAssembler, public DAssembler, public AssemblerBase
{
public:
  void ensurceSpace()
  {
	  if(bufferFree() > IAssembler::ctOneCodeSize) return ;
  }
  //operator
  void adc(Register rd, Register rs);
  void adc();

  // Stack
  void pushad();
  void popad();

  void pushfd();
  void popfd();
};

}//namespace vm

#endif //WPP_ASSEMBLER_IA32_H