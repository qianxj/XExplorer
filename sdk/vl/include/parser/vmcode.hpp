#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"
#include <stack>

namespace vl
{
	//flag  bit[0-3]
	#define _item_null		0x0
	#define _item_var		0x1
	#define _item_arg		0x2
	#define _item_citem		0x3
	#define _item_this		0x4
	#define _item_oitem		0x5
	#define _item_pitem		0x6
	#define _item_call		0x7
	#define _item_ptcall	0x8
	#define _item_const		0x9
	#define _item_op		0xA
	#define _item_refcall	0xB

	#define itemFlag(x)     (0x1<<(x - 1))

	struct vm
	{
		//������Ĵ��������趨
		enum CReg
		{
			//ͨ�üĴ���������
			GeneralRegisters	= 16,
			//�����Ĵ���������
			IntRegisters		= 32,
			//�������Ĵ���������
			DoubleRegisters		= 32,
			//�����Ĵ���������
			ParamRegisters		= 16,
			//�����Ĵ���������
			VarRegisters		= 80,
			//ָ��Ĵ���������
			PointerRegisters	= 4,
			//����Ĵ���������
			ArrayRegisters		= 4,
			//�ֶμĴ���������
			FieldRegisters		= 4,
			//ֱ�������Ĵ���������
			IntDirectRegisters	= 8,
			//ֱ�Ӹ���Ĵ���������
			DoubleDirectRegisters = 4,
			//ֱ���ַ����Ĵ�������
			StringDirectRegisters = 2,
			//ָ��Ĵ�������
			InsRegisters = 8,	//2bit for param count 1bit for extern
			//�����ƼĴ���
			ControlRegisters = 36,
			//���ű�ָ��
			MetaRegisters = 2,
			//�ۼƼĴ���������
			CountRegisters      = GeneralRegisters + IntRegisters + DoubleRegisters +
				ParamRegisters + VarRegisters + PointerRegisters	+ ArrayRegisters +
				FieldRegisters + IntDirectRegisters + DoubleDirectRegisters + StringDirectRegisters+
				InsRegisters + ControlRegisters + MetaRegisters + 4
		};

		enum Reg : unsigned char
		{
			//�Ĵ�����ʼ����
			RegStart = 1,

			//ͨ�üĴ���
			E0	= RegStart,
			ECP = E0,
			ESP,
			EBP, 
			This,
			Ee	= E0 + GeneralRegisters - 1,
			//�����Ĵ���
			R0,
			EAX = R0,
			EBX,
			ECX,
			EDX,
			ESI,
			EDI,
			Re  = R0 + IntRegisters - 1,
			//�������Ĵ���
			D0	,
			De  = D0 + DoubleRegisters - 1,
			//�����Ĵ��� 
			P0,
			Pe  = P0 + ParamRegisters - 1,
			//�����Ĵ���
			V0	,	
			Ve  = V0 + VarRegisters - 1,
			//ָ��Ĵ���
			T0 ,
			Te = T0 + PointerRegisters - 1,
			//����Ĵ���
			A0 ,	//one dim
			Ae	= A0  + ArrayRegisters - 1,	//if dim>3 then next byte is count of dims
			//�ֶμĴ���
			S0 ,
			Se  = S0 + FieldRegisters - 1,
			//ֱ�����Ĵ���
			I0,
			Ie = I0 + IntDirectRegisters - 1,
			//����ֱ�����Ĵ���
			F0,
			Fe = F0 + DoubleDirectRegisters - 1,
			//�ַ���ֱ�ӼĴ���
			L0,
			Le = L0 + StringDirectRegisters - 1,
			//ָ��Ĵ���
			N0,
			Ne = N0 + InsRegisters - 1,
			//��ָ��Ĵ���
			BL0,
			BL_IF = BL0,	
			BL_IFCondition,
			BL_IFTrueBody,	
			BL_IFFalseBody,
			
			BL_FOR,
			BL_FORInitial,
			BL_FORCondition,
			BL_FORLoop,
			BL_FORBody,

			BL_WHILE,
			BL_WHILECondition,
			BL_WHILEBody,

			BL_DO,
			BL_DOBody,
			BL_DOCondition,

			BL_SWITCH,
			BL_SWITCHCondition,
			BL_SWITCHCaseCondition,
			BL_SWITCHDefault,

			BL_TRY,
			BL_TRYBody,
			BL_TRYCatchCondition,
			BL_TRYCatchBody,
			BL_TRYFinally,

			BL_LABEL,

			BL_BREAK,
			BL_GOTO,
			BL_CONTINUE,
			BL_THROW,
			BL_RETURN,

			BL_BLOCKStart,
			BL_BLOCKEnd,

			BL_NULL,
			BL_DEBUG,

			BLe = BL0 + ControlRegisters - 1,
			//Ԫ���ݼĴ���
			M0,
			Me = M0 + MetaRegisters,
			//����
			SKIP,
			//��ֵ
			NIL,
			//�Ĵ�����������
			RegEnd
		};

		//������������
		enum eIntType
		{
			//�޷�������
			U8		= ((0x1<<2)|0x00),
			U16		= ((0x1<<2)|0x01),
			U32		= ((0x1<<2)|0x02),
			U64		= ((0x1<<2)|0x03),
			//����
			I8		= 0x00,
			I16		= 0x01,
			I32		= 0x02,
			I64		= 0x03
		};

		//Ԫ����ָ��
		enum mcode : unsigned char
		{
			Make	= 0x20,	//����
			Set		= 0x21,	//����Ԫ������
			Enter	= 0x22,	//����
			Leave	= 0x23, //�뿪
			Use		= 0x24,	//�ɷ���
			Get		= 0x25,
			Find	= 0x26
		};

		//Ԫ������
		enum mtype : unsigned char
		{
			Module		= 0x20,	//ģ��
			Unit		= 0x21,	//��Ԫ
			NameSpace	= 0x22,	//�����ռ�

			Class		= 0x30,	//��
			Struct		= 0x31,	//�ṹ
			Enum		= 0x32,	//ö��
			Union		= 0x33,	//����
			Interface	= 0x34,	//�ӿ�

			Function	= 0x40,	//����
			Ramda		= 0x41,	//ramda����

			DclType		= 0x50,	//��������
			DclVar		= 0x51,	//��������
			DclParam	= 0x52,	//��������

			Super		= 0x53
		};

		//Ԫ������
		enum mItem : unsigned char
		{
			ItemSpec	= 0x21,	//���
			ItemType	= 0x22,	//����
			ItemName	= 0x23,	//����
			ItemNote	= 0x24,	//��ע
			ItemParam	= 0x25,	//����
			ItemVar		= 0x26,	//����
			ItemField	= 0x27,	//�ֶ�
			ItemMethod	= 0x28,	//����
			ItemBody	= 0x29	//����
		};

		//�����ָ��
		enum pcode : unsigned char
		{
			LAD		= 0x20 ,
			STO		= 0x21 ,

			MOV		= 0x22 ,
			CST		= 0x23 ,	//����ת��

			ADD		= 0x40 ,
			SUB		= 0x41 ,
			MUL		= 0x42 ,
			DIV		= 0x43 ,

			SHR		= 0x44 ,
			SHL		= 0x45 ,
			BITAND	= 0x46 ,
			BITOR	= 0x47 ,
			BITXOR	= 0x48 ,
			BITNOT	= 0x49 ,
			MODE	= 0x4B ,	//
			POWER	= 0x4C ,	//
			LT		= 0x4D ,
			GT		= 0x4E ,
			EQ		= 0x4F ,
			LTE		= 0x50 ,
			GTE		= 0x51 ,
			NEQ		= 0x52 ,
			OR		= 0x53 ,
			AND		= 0x54 ,
			NOT		= 0x55 ,
			NEG		= 0x56 ,

			FADD	= 0x60 ,
			FSUB	= 0x61 ,
			FMUL	= 0x62 ,
			FDIV	= 0x63 ,
			FCT		= 0x64 ,  //�����׼����

			INC		= 0x70 , //
			DEC		= 0x71 , //
			NEW		= 0x72 ,
			DEL		= 0x73 ,

			NOP		= 0x80
		};

		static bool  EnterCode(std::stack<unsigned char> &codepool,unsigned int code);
		static bool  EnterCode(std::stack<unsigned char> &codepool,unsigned int code,int ext);
		static bool  EnterCode(std::stack<unsigned char> &codepool,unsigned int code,double ext);
		static bool  EnterCode(std::stack<unsigned char> &codepool,unsigned int code,__int64 ext);
	};

	namespace vp
	{
		enum pcode {
			UNVALID			= 0x00,
			PUSH			= 0x01, //
			POP				= 0x02, //
			NOP				= 0x03, //
			ARRAY			= 0x04, //
			MAKE			= 0x05, //
			DCLV			= 0x07, //
			DCLP			= 0x08, //
			DCLF			= 0x09, //
			LOAD			= 0x0A, //
			LOADA			= 0x0B,	//
			STORE			= 0x0C, //
			VREF			= 0x0D, //
			AREF			= 0x0E, //
			XREF			= 0x0F, //
			NAME			= 0x17, //
			ADDR			= 0x19, //
			VAL				= 0x1A, //
			CALL			= 0x20, //
			RET				= 0x21, //

			APPEND			= 0x22, //
			ELEMENT			= 0x23, //
			JMP				= 0x30, //
			JMPF			= 0x31, //
			JMPT			= 0x32, //
			LABEL			= 0x33, //
			EXP				= 0x34, //
			
			ADD				= 0x41, //
			SUB				= 0x42, //
			MUL				= 0x43, //
			DIV				= 0x44, //
			SHL				= 0x45, //
			SHR				= 0x46, //
			BITAND			= 0x47, //
			BITOR			= 0x48, //
			BITXOR			= 0x49, //
			BITNOT			= 0x4A,
			MODE			= 0x4B,	//
			POWER			= 0x4C,	//
			LT				= 0x4D,
			GT				= 0x4E,
			EQ				= 0x4F,
			LTE				= 0x50,
			GTE				= 0x51,
			NEQ				= 0x52,
			OR				= 0x53,
			AND				= 0x54,
			NOT				= 0x55,
			NEG				= 0x56,

			ASN				= 0x60,
			ADDASN			= 0x61, //
			SUBASN			= 0x62, //
			MULASN			= 0x63, //
			DIVASN			= 0x64, //
			SHLASN			= 0x65, //
			SHRASN			= 0x66, //
			BITANDASN		= 0x67, //
			BITORASN		= 0x68, //
			BITXORASN		= 0x69, //
			BITNOTASN		= 0x6A,

			INC				= 0x70, //
			DEC				= 0x71, //
			NEW				= 0x72,
			DESTORY			= 0x73,				

			TRACE			= 0x81,	//
			EXIT			= 0x82,  //
			POPS			= 0x83,//
			DEBUGBREAK		= 0x84
		};

		const int FSmallInt	= 0x01;
		const int FHaveArg	= 0x02;
	}

	namespace vk
	{
		enum kcode
		{
			UNVALID			= 0x00,
			USE				= 0x01, //
			UNIT			= 0x02, //
			STMT			= 0x03, //
			SUBJECT			= 0x05, //
			PRED			= 0x06, //
			OBJECT			= 0x07, //
			ITEM			= 0x08, //
			BLOCK			= 0x09, //
			PARENT			= 0x0A,	//
			POP				= 0x0B
		};
	};

	namespace ast
	{
		enum nodetype
		{
			astExp = 0x01,
			astInt,
			astDouble,
			astString,
			astBool,
			astChar,
			astOp,
			astRef,
			astCall,
			astField,
			astItem,
			astEle
		};

		const	int flagconst=	0x01;
		const	int flaglv	=	0x02;

		struct astnode
		{
			int child;
			int slibe;
			int node;
			int val;
			int val1;
			int flag;
		};
	}

	namespace vt
	{
		enum tcode
		{
			UNVALID			= 0x00,
			NS				= 0x01, //
			OBJECT			= 0x02, //
			FIELD			= 0x03, //
			METHOD			= 0x04, //
			PARAM			= 0x05, //
			VAR				= 0x07, //
			PROP			= 0x08, //
			TYPE			= 0x09, //
			USE				= 0x0A,	//
			SPEC			= 0x0B, //
			CLASS			= 0x0C,
			EXTEND			= 0x0D,
			POP				= 0x0E,

			//for stylang
			STYPACKAGE		= 0x10,
			STYITEM			= 0x11,
			STYSTEP			= 0x12,
			STYDATA			= 0x13
		};
	};

	#define MakeCode(code,ntyp,flag) (((code)<<24)|((ntyp)<<16)|((flag)<<22))
	#define MakeCode0(code) ((code)<<24)
	#define MakeCode1(code,ntyp,flag,d) (((code)<<24)|((ntyp)<<16)|((flag)<<22)|(d & 0xffff))

	class vmcode
	{
	public:
		static vp::pcode getPCode(const tchar * p);
		static vp::pcode getOpCode(int op);
		static const tchar * getPCodeString(int cd);
	public:
		static int  GetOpString(int op,tchar buf[]);
	public:
		static vt::tcode getTCode(const tchar * p);
		static const tchar * getTCodeString(int cd);
	public:
		static vk::kcode getKCode(const tchar * p);
		static const tchar * getKCodeString(int cd);
	};
}