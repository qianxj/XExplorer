#ifndef  XMACHINE_H
#define  XMACHINE_H

#define SHOWMacro(x)						SHOWIMacro(x);
#define SHOWIMacro(x)					printf("%s",#x);
	
enum TBit
{
	B8 = 8,
	B16 = 16,
	B32 = 32,
	Bit64 = 64,
	Bit128 = 128
};

enum TCond
{
	cOverflow     	=  0, 
	cNoOverflow  	=  1, 
	cBelow        	=  2, 
	cAboveEqual	=  3, 
	cEqual        	=  4, 
	cNotEqual     	=  5, 
	cBelowEqual 	=  6,
	cAbove        	=  7,
	cNegative     	=  8,
	cPositive     	=  9,
	cParityEven   	= 10,
	cParityOdd    	= 11,
	cLess         	= 12,
	cGreaterEqual	= 13,
	cLessEqual   	= 14,
	cGreater      	= 15,

	cCarry        	= cBelow,
	cNotCarry    	= cAboveEqual,
	cZero         	= cEqual,
	cNotZero     	= cNotEqual,
	cSign         	= cNegative,
	cNotSign     	= cPositive
};
	
class Term
{
	public:
		enum Kind : unsigned char
		{
			kTerm = 0,
			kRegister = 1,
			kAddress =2,
			kFRegister = 3,
			kImm = 4,
			kFImm = 5,
			kDisp = 6
		};
	public:
		Term(unsigned char kind_,unsigned char bits_):
			kind(kind_), bits(bits_){}
	public:
		unsigned char kind;
		unsigned char bits;
};

template<TBit Bits>
class XTerm  : public Term
{
	public:
		XTerm(unsigned char kind) :Term(kind,Bits){}
};

template<TBit Bits>
class Register : public XTerm<Bits>
{
	public:
		Register(int reg) : value(reg), XTerm<Bits>(Term::kRegister){}
	public:
		unsigned char value; 
};

template<TBit Bits>
class FRegister : public XTerm<Bits>
{
	public:
		FRegister(int reg) : value(reg),XTerm<Bits>(Term::kFRegister){}
	public:
		unsigned char value; 
};

template<TBit Bits>
class Disp : public XTerm<Bits>
{
	public:
		Disp(int val) :  value(val), XTerm<Bits>(Term::kDisp){}
	public:
		int value;
};


template<TBit Bits>
class Address : public XTerm<Bits>
{
	public:
		Address() : XTerm<Bits>(Term::kAddress){}
};

template<TBit Bits>
class Imm : public XTerm<Bits>
{
	public:
		Imm(int val): value(val), XTerm<Bits>(Term::kImm){}
	int value;
};

template<TBit Bits>
class FImm: public XTerm<Bits>
{
	public:
		FImm(): XTerm<Bits>(Term::kFImm){}
};

class Machine
{
};

namespace X86
{

	
	static const Register<B8> al(0);
	static const Register<B8> cl(1);
	static const Register<B8> dl(2);
	static const Register<B8> bl(3);
	static const Register<B8> ah(4);
	static const Register<B8> ch(5);
	static const Register<B8> dh(7);
	static const Register<B8> bh(8);
	
	static const Register<B16> ax(0);
	static const Register<B16> cx(1);
	static const Register<B16> dx(2);
	static const Register<B16> bx(3);
	static const Register<B16> sp(4);
	static const Register<B16> bp(5);
	static const Register<B16> si(6);
	static const Register<B16> di(7);
	
	static const Register<B32> eax(0);
	static const Register<B32> ecx(1);
	static const Register<B32> edx(2);
	static const Register<B32> ebx(3);
	static const Register<B32> esp(4);
	static const Register<B32> ebp(5);
	static const Register<B32> esi(6);
	static const Register<B32> edi(7);
	
	#define MODRM(m, r1, r2) ((m<<6)|r1<<3|r2)
	#define SIB(s, i, b) ((s<<6)|i<<3|b)
	#define WARPWORD()	0x66
	#define EMPTY 
	#define WARP *pc++=0x66;
	#define OPCODE(cd,d,w)	(cd|(d)<<1|w)
	#define OP_addr(lhs, r) \
		*pc++ = lhs.modrm | (r <<3); 	\
		if(lhs.modrm >> 6 != 0x4 && (lhs.modrm & 0x7)==0x4) *pc++ = lhs.sib; \
		if(lhs.modrm >>6 == 0x0 && (lhs.modrm &0x7) == 0x5) \
		{	\
			*(int*)pc = lhs.value;	\
			pc+=4;	\
		}	\
		else if(lhs.modrm >>6 == 0x1)	\
		{	\
			*(unsigned char*)pc++ = (unsigned char)lhs.value;	\
		}	\
		else if(lhs.modrm >>6 == 0x2)	\
		{	\
			*(int*)pc = lhs.value;	\
			pc+=4;	\
		}
		
	template<TBit Bits = B32>
	class AddressX86 : public Address<Bits>
	{
		public:
			AddressX86(const Register<B32> & reg)
			{
				sib = reg.value==4 ? sib=SIB(0,4,4) :  0; //esp 
				modrm = reg.value !=5 ? MODRM(0,0,reg.value) : MODRM(0x1,0,reg.value);
				value = 0;
			}
			
			AddressX86(const Disp<B32> & disp)
			{
				modrm = MODRM(0,0,5);
				sib = 0;
				value=disp.value;
			}
			AddressX86(const Register<B32> & reg, const Disp<B8> & disp)
			{
				sib = reg.value==4 ? SIB(0,4,4) : 0; //esp
				modrm = MODRM(0x1,0,reg.value);
				value = disp.value;
			}
			AddressX86(const Register<B32> & reg, const Disp<B32> & disp)
			{
				sib = reg.value==4 ? SIB(0,4,4) : 0; //esp
				modrm = MODRM(0x2,0,reg.value);
				value = disp.value;
			}
		public:
			unsigned char modrm;
			unsigned char sib;
			int value; //disp value
	};

	//fix http://bbs.csdn.net/topics/390675157
	#define OWNER(x)	x
	#define ENCODE_c0() 						
	#define ENCODE_c1(c1) 					*pc++ = c1;
	#define ENCODE_c2(c1,c2) 				ENCODE_c1(c1) *pc++=c2;
	#define ENCODE_c3(c1,c2,c3) 			ENCODE_c2(c1,c2) *pc++=c3;
	#define ENCODE_c4(c1,c2,c3,c4) 		ENCODE_c3(c1,c2,c3) *pc++=c4;
	
	#define ENCODE_i8(x)						*pc++ = (char) (x); 
	#define ENCODE_i16(x)						*(unsigned short*)pc = (unsigned short) (x); pc+=2; 
	#define ENCODE_i32(x)						*(unsigned int*)pc = (unsigned int) (x); pc+=4; 
	
	#define CONCAT(x,y)	CONCAT_(x,y)
	#define CONCAT_(x,y)	x##y

	//to register
	#define D(t)			CONCAT(D,t)
	#define D_(t,x,...)	D_##t				
	#define D_RR			1	
	#define D_RM			1	
	#define D_MR			0	
	//word
	#define W(t)			CONCAT(W,t)
	#define W_(t,x,...)	W_##x				
	#define W_8			0	
	#define W_16			1	
	#define W_32			1	
	#define W_64			1	
	//sign
	#define S(t)			CONCAT(S,t)
	#define S_(t,x,y)		S_##x##_##y		
	#define S_16_8		1	
	#define S_32_8		1		
	#define S_32_16		1		
	#define S_64_8		1
	#define S_64_32		1
	#define S_8_8			0	
	#define S_16_16		0		
	#define S_32_32		0		
	#define S_64_64		0
	//warp
	#define WP(t)			CONCAT(WP,t)
	#define WP_(t,x,...)	WP_##x				
	#define WP_8				
	#define WP_16		*pc++ = 0x66;		
	#define WP_32				
	#define WP_64
	
	//imm
	#define IMM(t,i)			CONCAT(IMM,t)(i)
	#define IMM_(t,x,y)		ENCODE_i##y
	#define IMM1(t,i)			CONCAT(IMM1,t)(i)
	#define IMM1_(t,x,...)	ENCODE_i##x
	
	#define RR(x,y)			_(RR,x,y)
	#define MR(x,y)			_(MR,x,y)
	#define RI(x,y)			_(RI,x,y)
	#define MI(x,y)			_(MI,x,y)
	#define RM(x,y)			_(RM,x,y)
	#define I(x)				_(I,x)
	#define R(x)				_(R,x)
	#define M(x)				_(M,x)
	
	#define OPR(t)			CONCAT(OPR,t)
	#define OPR_(t,x,y)		OPR_##t(x,y)
	#define OPR_RR(x,y)		const Register<B##x> &lhs , const Register<B##y> &rhs
	#define OPR_MR(x,y)	const AddressX86<B##x> &lhs , const Register<B##y> &rhs
	#define OPR_RM(x,y)	const Register<B##x> &lhs , const AddressX86<B##y> &rhs
	#define OPR_RI(x,y)		const Register<B##x> &lhs , const Imm<B##y> &rhs
	#define OPR_MI(x,y)		const AddressX86<B##x> &lhs , const Imm<B##y> &rhs
	
	#define OPR1(t)			CONCAT(OPR1,t)
	#define OPR1_(t,x)		OPR1_##t(x)
	#define OPR1_I(x)		const Imm<B##x> &lhs	
	#define OPR1_M(x)		const AddressX86<B##x> &lhs	
	#define OPR1_R(x)		const Register<B##x> &lhs	
	
	#define ENCODE_RR(op,cd,t)			\
	static unsigned char *  op(unsigned char * pc,OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | (D(t)<<1)|W(t);\
		*pc++ = MODRM(0x3,lhs.value,rhs.value);	\
		return pc;	\
	}
	#define ENCODE_RM(op,cd,t)			\
	static unsigned char *  op(unsigned char * pc,OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | (D(t)<<1)|W(t); \
		OP_addr(rhs,lhs.value)	\
		return pc;	\
	}
	#define ENCODE_XM(op,cd,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,OPR(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		OP_addr(rhs,lhs.value)	\
		return pc;	\
	}
	#define ENCODE_XR(op,cd,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,OPR(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		*pc++ = MODRM(0x3,lhs.value,rhs.value);	\
		return pc;	\
	}
	
	#define ENCODE_MR(op,cd,t)			\
	static unsigned char *  op(unsigned char * pc, OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | (D(t)<<1)|W(t); \
		OP_addr(lhs,rhs.value)	\
		return pc;	\
	}
	//sign extent
	#define ENCODE_SRI(op,cd,rx,t)			\
	static unsigned char *  op(unsigned char * pc, OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | (S(t)<<1)|W(t); \
		*pc++ = MODRM(0x3,rx,lhs.value);	\
		IMM(t,rhs.value) \
		return pc;	\
	}
	#define ENCODE_RI(op,cd,rx,t)			\
	static unsigned char *  op(unsigned char * pc, OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | W(t); \
		*pc++ = MODRM(0x3,rx,lhs.value);	\
		IMM(t,rhs.value) \
		return pc;	\
	}
	#define ENCODE_SMI(op,cd,rx,t)			\
	static unsigned char *  op(unsigned char * pc, OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | (S(t)<<1)|W(t); \
		OP_addr(lhs,rx)	\
		IMM(t,rhs.value) \
		return pc;	\
	}
	#define ENCODE_MI(op,cd,rx,t)			\
	static unsigned char *  op(unsigned char * pc, OPR(t)) \
	{	\
		WP(t) \
		*pc++ = cd | W(t); \
		OP_addr(lhs,rx)	\
		IMM(t,rhs.value) \
		return pc;	\
	}

	#define ENCODE_I(op,cd,t)			\
	static unsigned char *  op(unsigned char * pc, OPR1(t)) \
	{	\
		WP(t) \
		*pc++ = cd|W(t); \
		IMM1(t,lhs.value) \
		return pc;	\
	}
	
	#define ENCODE_NI(op,cd,t)			\
	static unsigned char *  op(unsigned char * pc, OPR1(t)) \
	{	\
		WP(t) \
		*pc++ = cd; \
		IMM1(t,lhs.value) \
		return pc;	\
	}

	#define ENCODE_C(op,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,OPR1(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		return pc;	\
	}
	
	#define ENCODE_C0(op,n,...)			\
	static unsigned char *  op(unsigned char * pc) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		return pc;	\
	}
	
	#define ENCODE_CI(op,t,n,...)			\
	static unsigned char *  op(unsigned char * pc, OPR1(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		IMM1(t,lhs.value) \
		return pc;	\
	}

	#define ENCODE_CR(op,rx,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,OPR1(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		*pc++ = MODRM(0x3,rx,lhs.value);	\
		return pc;	\
	}
	
	#define ENCODE_CM(op,rx,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,OPR1(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		OP_addr(lhs,rx)	\
		return pc;	\
	}
	
	#define ENCODE_JC(op,cd,t,n,...)			\
	static unsigned char *  op(unsigned char * pc,TCond cnd, OPR1(t)) \
	{	\
		OWNER(ENCODE_c##n( __VA_ARGS__ ))	\
		*pc++ = cd|cnd;		\
		IMM1(t,lhs.value) \
		return pc;	\
	}
	
	//mov 
	//register2 to register1 1000 101w : 11 reg1 reg2
	//register1 to register2 1000 100w : 11 reg1 reg2
	ENCODE_RR(MOV, 0x88,RR(32,32))
	ENCODE_RR(MOV, 0x88,RR(16,16))
	ENCODE_RR(MOV, 0x88,RR(8,8))
	ENCODE_RM(MOV, 0x88,RM(32,32))
	ENCODE_RM(MOV, 0x88,RM(16,16))
	ENCODE_RM(MOV, 0x88,RM(8,8))
	ENCODE_MR(MOV, 0x88,MR(32,32))
	ENCODE_MR(MOV, 0x88,MR(16,16))
	ENCODE_MR(MOV, 0x88,MR(8,8))
	
	//immediate to register 1100 011w : 11 000 reg : immediate data 
	ENCODE_RI(MOV, 0xC6,0x0,RI(32,32))
	ENCODE_RI(MOV, 0xC6,0x0,RI(16,16))
	ENCODE_RI(MOV, 0xC6,0x0,RI(8,8))
	
	ENCODE_XM(MOVSX, 0xBC,RM(32,8),2,0x0f,OPCODE(0xBC,1,0)) 
	ENCODE_XM(MOVSX, 0xBC,RM(32,16),2,0x0f,OPCODE(0xBC,1,1)) 
	ENCODE_XM(MOVSX, 0xBC,RM(16,8),3,0x66,0x0f,OPCODE(0xBC,1,0)) 
	
	ENCODE_XM(MOVZX, 0xB4,RM(32,8),2,0x0f,OPCODE(0xB4,1,0)) 
	ENCODE_XM(MOVZX, 0xB4,RM(32,16),2,0x0f,OPCODE(0xB4,1,1)) 
	ENCODE_XM(MOVZX, 0xB4,RM(16,8),3,0x66,0x0f,OPCODE(0xB4,1,0)) 
	ENCODE_XR(MOVZX, 0xB4,RR(32,8),2,0x0f,OPCODE(0xB4,1,0)) 
	ENCODE_XR(MOVZX, 0xB4,RR(32,16),2,0x0f,OPCODE(0xB4,1,1)) 
	ENCODE_XR(MOVZX, 0xB4,RR(16,8),3,0x66,0x0f,OPCODE(0xB4,1,0)) 
	
	#define XADD(op,cd) \
	ENCODE_RR(op,cd,RR(32,32))	\
	ENCODE_RR(op,cd,RR(16,16))	\
	ENCODE_RR(op,cd,RR(8,8))	\
		\
	ENCODE_RM(op,cd,RM(32,32))	\
	ENCODE_RM(op,cd,RM(16,32))	\
	ENCODE_RM(op,cd,RM(8,32))	\
		\
	ENCODE_MR(op,cd,MR(32,32))	\
	ENCODE_MR(op,cd,MR(16,32))	\
	ENCODE_MR(op,cd,MR(8,32))	\
		\
	ENCODE_SRI(op,cd,0x0,RI(32,32))	\
	ENCODE_SRI(op,cd,0x0,RI(16,16))	\
	ENCODE_SRI(op,cd,0x0,RI(8,8))	\
	ENCODE_SRI(op,cd,0x0,RI(16,8))	\
	ENCODE_SRI(op,cd,0x0,RI(32,8))	\
		\
	ENCODE_SMI(op,cd,0x0,MI(32,32))	\
	ENCODE_SMI(op,cd,0x0,MI(32,16))	\
	ENCODE_SMI(op,cd,0x0,MI(32,8))	\
		\
	ENCODE_I(op,cd|0x4,I(32))	\
	ENCODE_I(op,cd|0x4,I(16))	\
	ENCODE_I(op,cd|0x4,I(8))	
	
	//add 0x00
	XADD(ADD,0x00)
	XADD(ADC,0x10)
	XADD(AND,0x20)
	XADD(OR, 0x30)
	XADD(XOR,0x08)
	XADD(SBB,0x18)
	XADD(SUB,0x28)
	XADD(CMP,0x38)
	
	#undef XADD
	
	//shift
	//shift by 1
	ENCODE_CR(SHL1,0x4,R(32),1,OPCODE(0xD0,0,1)) 
	ENCODE_CR(SHL1,0x4,R(16),2,0x66,OPCODE(0xD0,0,1)) 
	ENCODE_CR(SHL1,0x4,R(8),1,OPCODE(0xD0,0,0)) 
	ENCODE_CM(SHL1,0x4,M(32),1,OPCODE(0xD0,0,1)) 
	ENCODE_CM(SHL1,0x4,M(16),2,0x66,OPCODE(0xD0,0,1)) 
	ENCODE_CM(SHL1,0x4,M(8),1,OPCODE(0xD0,0,0)) 
	//shift by cl
	ENCODE_CR(SHL,0x4,R(32),1,OPCODE(0xD2,0,1)) 
	ENCODE_CR(SHL,0x4,R(16),2,0x66,OPCODE(0xD2,0,1)) 
	ENCODE_CR(SHL,0x4,R(8),1,OPCODE(0xD2,0,0)) 
	ENCODE_CM(SHL,0x4,M(32),1,OPCODE(0xD2,0,1)) 
	ENCODE_CM(SHL,0x4,M(16),2,0x66,OPCODE(0xD2,0,1)) 
	ENCODE_CM(SHL,0x4,M(8),1,OPCODE(0xD2,0,0)) 
	//shift by imm8
	ENCODE_RI(SHL,0xC0,0x4,RI(32,8))
	ENCODE_RI(SHL,0xC0,0x4,RI(16,8))
	ENCODE_RI(SHL,0xC0,0x4,RI(8,8))
	ENCODE_MI(SHL,0xC0,0x4,MI(32,8))
	ENCODE_MI(SHL,0xC0,0x4,MI(16,8))
	ENCODE_MI(SHL,0xC0,0x4,MI(8,8))
	
	/*
	JMP  Unconditional Jump (to same segment)
		short	1110 1011 : 8-bit
		displacement direct 1110 1001 : full displacement
		register indirect 1111 1111 : 11 100 reg 
		memory indirect 1111 1111 : mod 100 r/m 
	JMP Unconditional Jump (to other segment)
		direct intersegment 1110 1010 : unsigned full offset, selector 
		indirect intersegment1111 1111 : mod 101 r/m
	*/
	ENCODE_I(JMP,0xEB,I(8))
	ENCODE_I(JMP,0xE9,I(16))
	ENCODE_I(JMP,0xE9,I(32))
	ENCODE_CR(JMP,0x4,R(32),1,0xff)
	ENCODE_CM(JMP,0x4,M(32),1,0xff)
	
	/*
	Jcc 每 Jump if Condition is Met
		8-bit displacement 0111 tttn : 8-bit displacement
		full displacement 0000 1111 : 1000 tttn : full displacement
	*/
	ENCODE_JC(JCC,0x70,I(8),0)
	ENCODE_JC(JCC,0x80,I(16),2,0x66,0x0f)
	ENCODE_JC(JCC,0x80,I(32),1,0x0f)
	
	/*
	PUSH 每 Push Operand onto the Stack
		register 1111 1111 : 11 110 reg
		register (alternate encoding) 0101 0 reg
		memory 1111 1111 : mod 110 r/m
		immediate 0110 10s0 : immediate data
	*/
	ENCODE_NI(PUSH,0x68|0x2,I(8))
	ENCODE_NI(PUSH,0x68,I(16))
	ENCODE_NI(PUSH,0x68,I(32))
	//ENCODE_R(PUSH,0xff,0x6,R(32),1,0xff)
	ENCODE_C(PUSH,R(32),1,MODRM(0x1,0x2,lhs.value))
	ENCODE_CM(PUSH,0x6, M(32),1,0xff)
	
	/*
	POP 每 Pop a Word from the Stack
		register 1000 1111 : 11 000 reg
		register (alternate encoding) 0101 1 reg
		memory 1000 1111 : mod 000 r/m
	*/
	//ENCODE_R(POP,0x8f,0x0,R(32),1,0xff)
	ENCODE_C(POP,R(32),1,MODRM(0x1,0x3,lhs.value))
	ENCODE_CM(POP,0x0, M(32),1,0x8f)
	
	/*RET 每 Return from Procedure (to same segment)
		no argument 1100 0011
		adding immediate to SP 1100 0010 : 16-bit displacement
	*/
	ENCODE_C0(RET,1,0xC3)
	ENCODE_CI(RET,I(16),1,0xC2)
	
	/*	CALL 每 Call Procedure (in same segment)
		direct 1110 1000 : full displacement
		register indirect 1111 1111 : 11 010 reg
		memory indirect 1111 1111 : mod 010 r/m
	*/
	ENCODE_CI(CALL,I(32),1,0xE8)
	ENCODE_CR(CALL,0x2,R(32),1,0xFF)
	ENCODE_CM(CALL,0x2,M(32),1,0xFF)
	
	#undef OWNER
	#undef ENCODEChar
	#undef ENCODE_c_
	#undef ENCODE_c1
	#undef ENCODE_c2
	#undef ENCODE_c3
	#undef ENCODE_c4
	
	#undef ENCODEAddr
	#undef ENCODE_adr
	#undef ENCODE_adr_
	
	#undef ENCODEImm
	#undef ENCODE_i
	#undef ENCODE_i_
	#undef ENCODE_i8
	#undef ENCODE_i16
	#undef ENCODE_i32
	
	#undef ENCODE
	
	#undef D				
	#undef D_				
	#undef D_RR				
	#undef D_RM			
	#undef D_MR				
	#undef D_RI			
	#undef D_IR			
	#undef D_MI			
	#undef D_IR			
	#undef W			
	#undef W_			
	#undef W_8				
	#undef W_16				
	#undef W_32				
	#undef W_64				
	#undef S			
	#undef S_			
	#undef S_16_8			
	#undef S_32_8				
	#undef S_32_16				
	#undef S_64_8		
	#undef S_64_32		
	#undef S_8_8				
	#undef S_16_16				
	#undef S_32_32				
	#undef S_64_64		
	#undef WP
	#undef WP_
	#undef WP_8				
	#undef WP_16			
	#undef WP_32				
	#undef WP_64		
	
	#undef MR
	#undef RR
	#undef RM
	#undef OPR
	#undef OPR_
	#undef OPR_MR
	#undef OPR_RR
	#undef OPR_RM
	
template<TBit Bits>
	struct Add : public XTerm<Bits>
	{
	};
	
	template<>
	struct Add<B32> : public XTerm<B32>
	{
		Term * lhs;
		Term * rhs;
		
		int GetCodeLength()
		{
			return 0;
		}
		
		/*
		int Print(osstrem os)
		{
			return 1;
		}*/
	};
}

#endif 