#pragma once
#include "gen.hxx"


namespace TAsm
{
	namespace XRegisters
	{
		struct RRegisterID
		{
			int v;
			RRegisterID(int r):v(r+4){};
		};

		struct VRegisterID
		{
			int v;
			VRegisterID(int r):v(r){};
		};
		struct PRegisterID
		{
			int v;
			PRegisterID(int r):v(r){};
		};
		struct  DRegisterID
		{
			int v;
			DRegisterID(int r):v(r){};
		};
		struct MRegisterID 
		{
			int v;
			MRegisterID(int r):v(r){};
		};
		struct TRegisterID
		{
			int v;
			TRegisterID(int r):v(r){};
		};
	}

	typedef XRegisters::RRegisterID RRegisterID;
	typedef XRegisters::VRegisterID VRegisterID;
	typedef XRegisters::PRegisterID PRegisterID;
	typedef XRegisters::DRegisterID DRegisterID;
	typedef XRegisters::MRegisterID MRegisterID;
	typedef XRegisters::TRegisterID TRegisterID;

	static RRegisterID rcx(-1);
	static RRegisterID r0(0);
	static RRegisterID r1(1);

	namespace tp
	{
		enum op
		{
			//group 0x00
			move	= 0x00,
			xchg	= 0x01,
			lea		= 0x02,
			zero	= 0x03,

			//group 0x01
			add 	= 0x10,
			sub		= 0x11,
			mul		= 0x12,
			div		= 0x13,
			inc		= 0x14,
			dec		= 0x15,

			//group 0x02
			bitand	=	0x20,
			bitor	=	0x21,
			bitxor	=	0x22,
			neg		=	0x23,
			bitnot	=	0x24,
			sftleft	=	0x25,
			sftright=	0x26,

			//group 0x03
			or		=	0x30,
			and		=	0x31,
			not		=	0x32,
			cmp		=	0x33,
			test	=	0x34,
			jmp		=	0x35,
			brearpoint	=	0x36,
		
			//group 0x04
			push	=	0x40,
			pop		=	0x41,
			call	=	0x42,
			ret		=	0x43,
			nop		=	0x44,
		
			//group 0x05
			_abs		=	0x50,
			_cos		=	0x51,
			_sin		=	0x52,
			_fcos	=	0x53,
			_fsin	=	0x54,
			_sqrt	=	0x55,
			_max		=	0x56,
			_min		=	0x57,
			
			//group 0x06
			_addr	=	0x60,
			_val		=	0x61,
			_cast	=	0x62,
			_new		=	0x63,
			_free	=	0x64,
			_sizeof	=	0x65,
			_type	=	0x66,
		
			//group 0x07
			_ref		=	0x70,
			_offset		=	0x71,
			_ele		=	0x72,
			_addref		=	0x73,
			_release	=	0x74,
		};
	}
	
	typedef int	ACTy;
	enum AC
	{
		ac_u = 0x00, //未使用
		ac_r = 0x01, //寄存器
		ac_d = 0x02, //直接数
		ac_v = 0x03, //变量
		ac_p = 0x04, //参数
		ac_m = 0x05, //暂存
		ac_t = 0x06  //临时
	};
	#define ref_d	0x02
	#define ref_s	0x01
	#define AC2(d,s,r) (((((r&ref_d)<<3)|d)<<4)|(((r&ref_s)<<3)|s))

	struct XSM_API VItem
	{
		ACTy nType;
		 union 
		{
			int iVal;
			LPTSTR sVal;
			double dVal;
			void* pVal;
			__int64 iVal2;
		};
	};

	enum XCondition
	{
		Zero	= 0x01,
		NonZero = 0x02
	};

	struct XLabel
	{
		int Addr;
	};
	struct XJump
	{
		int addr;
	};

	class XSM_API tcode
	{
	private:
		BYTE*	m_pCode;
		BYTE*	m_pData;
		
		int nCodeUsed;
		int nCodeAlloced;

		int nDataUsed;
		int nDataAlloced;
	private:
		void EnsureCode(int cx);
	public:
		VItem m_Item;
	public:
		tcode();
		~tcode();
	public:
		int  EnterData(ACTy ntype, BYTE* pData,int nLen);
		void Enter(tp::op m);
		void Enter(tp::op m,XJump &jump);
		void Enter(tp::op m,XCondition cd,XJump &jump);
		void Enter(tp::op m,BYTE rt,int r);
		void Enter(tp::op m,BYTE rt,int dest,int src);
	private:
		VItem R[32];
		VItem V[96];
		VItem A[32];
		VItem T[32];
	private:
		VItem*  GetData(int nData);
		bool IsDoubleType(ACTy ntype);
		bool IsIntType(ACTy ntype);
	public:
		void eval(VItem &item);
	public:
		virtual XLabel label();
		virtual XJump  jump();
		virtual void  link(XJump &jump,XLabel &label);
	public:
		virtual void move(RRegisterID  rd, RRegisterID rs , BYTE ref2 = 0);
		virtual void move(RRegisterID  rd, DRegisterID rs,  BYTE ref2 = 0);
		virtual void move(VRegisterID  rd, DRegisterID rs,  BYTE ref2 = 0);
		virtual void move(PRegisterID  rd, DRegisterID rs,  BYTE ref2 = 0);
		virtual void move(VRegisterID  rd, RRegisterID rs,  BYTE ref2 = 0);
		virtual void move(PRegisterID  rd, RRegisterID rs,  BYTE ref2 = 0);
		virtual void move(RRegisterID  rd, VRegisterID rs,  BYTE ref2 = 0);
		virtual void move(RRegisterID  rd, PRegisterID rs,  BYTE ref2 = 0);
		virtual void move(RRegisterID  rd, MRegisterID rs,  BYTE ref2 = 0);
		virtual void move(RRegisterID  rd, TRegisterID rs,  BYTE ref2 = 0);
		virtual void move(MRegisterID  rd, RRegisterID rs,  BYTE ref2 = 0);
		virtual void move(TRegisterID  rd, RRegisterID rs,  BYTE ref2 = 0);
	public:
		virtual void xchg(RRegisterID  rd,  RRegisterID  rs);
		virtual void xchg(VRegisterID rd,  RRegisterID  rs);
		virtual void xchg(RRegisterID  rd,  VRegisterID rs);
		virtual void xchg(PRegisterID rd,  RRegisterID  rs);
		virtual void xchg(RRegisterID  rd,  PRegisterID rs);
		virtual void xchg(VRegisterID rd,  PRegisterID rs);
		virtual void xchg(PRegisterID rd,  VRegisterID rs);
	public:
		virtual void add(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void add(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void sub(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void sub(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void mul(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void mul(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void div(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void div(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
	public:
		virtual void inc(RRegisterID  rd, BYTE ref2 = 0);
		virtual void inc(PRegisterID rd, BYTE ref2 = 0);
		virtual void inc(VRegisterID rd, BYTE ref2 = 0);
		virtual void dec(RRegisterID  rd, BYTE ref2 = 0);
		virtual void dec(PRegisterID rd, BYTE ref2 = 0);
		virtual void dec(VRegisterID rd, BYTE ref2 = 0);
	public:
		virtual void bitand  (RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void bitand  (RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void bitor   (RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void bitor   (RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void bitxor  (RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void bitxor  (RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void sftleft (RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void sftleft (RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void sftright(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void sftright(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
	public:
		virtual void neg(RRegisterID  rd, BYTE ref2 = 0);
		virtual void neg(PRegisterID rd, BYTE ref2 = 0);
		virtual void neg(VRegisterID rd, BYTE ref2 = 0);
		virtual void bitnot(RRegisterID  rd, BYTE ref2 = 0);
		virtual void bitnot(PRegisterID rd, BYTE ref2 = 0);
		virtual void bitnot(VRegisterID rd, BYTE ref2 = 0);
	public:
		virtual void and(RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void and(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void or (RRegisterID  rd, RRegisterID   rs , BYTE ref2 = 0);
		virtual void or (RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void not(RRegisterID  rd, BYTE ref2 = 0);
		virtual void not(PRegisterID rd, BYTE ref2 = 0);
		virtual void not(VRegisterID rd, BYTE ref2 = 0);
	public:
		virtual void cmp(RRegisterID  rd, RRegisterID  rs , BYTE ref2 = 0);
		virtual void cmp(RRegisterID  rd, DRegisterID  rs , BYTE ref2 = 0);
		virtual void test(XCondition  cd, RRegisterID  rd , XJump & jump);
		virtual void test(XCondition  cd, XJump  &jump);
		virtual void jmp(XJump  &jump);
		virtual void brearpoint();
	public:
		virtual void push(RRegisterID  rd,  BYTE ref2 = 0);
		virtual void push(PRegisterID rd,  BYTE ref2 = 0);
		virtual void push(VRegisterID rd,  BYTE ref2 = 0);
		virtual void push(DRegisterID rd,  BYTE ref2 = 0);
		virtual void push(TRegisterID rd,  BYTE ref2 = 0);
		virtual void push(MRegisterID rd,  BYTE ref2 = 0);
		virtual void pop (RRegisterID  rd,  BYTE ref2 = 0);
		virtual void pop (PRegisterID rd,  BYTE ref2 = 0);
		virtual void pop (VRegisterID rd,  BYTE ref2 = 0);
		virtual void pop (DRegisterID rd,  BYTE ref2 = 0);
		virtual void pop (TRegisterID rd,  BYTE ref2 = 0);
		virtual void pop (MRegisterID rd,  BYTE ref2 = 0);
		virtual void call(RRegisterID rd,  BYTE ref2 = 0);
		virtual void call(PRegisterID rd,  BYTE ref2 = 0);
		virtual void call(VRegisterID rd,  BYTE ref2 = 0);
		virtual void call(DRegisterID rd,  BYTE ref2 = 0);
		virtual void ret(RRegisterID rd,  BYTE ref2 = 0);
		virtual void ret(PRegisterID rd,  BYTE ref2 = 0);
		virtual void ret(VRegisterID rd,  BYTE ref2 = 0);
		virtual void ret(DRegisterID rd,  BYTE ref2 = 0);
		virtual void ret();
		virtual void nop();
	public:
		virtual void _abs (RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _cos (RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _sin (RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _fcos(RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _fsin(RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _sqrt(RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _max (RRegisterID  rd,  BYTE ref2 = 0);
		virtual void _min (RRegisterID  rd,  BYTE ref2 = 0);
	};
}