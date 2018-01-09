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
		//虚拟机寄存器数量设定
		enum CReg
		{
			//通用寄存器的数量
			GeneralRegisters	= 16,
			//整数寄存器的数量
			IntRegisters		= 32,
			//浮点数寄存器的数量
			DoubleRegisters		= 32,
			//参数寄存器的数量
			ParamRegisters		= 16,
			//变量寄存器的数量
			VarRegisters		= 80,
			//指针寄存器的数量
			PointerRegisters	= 4,
			//数组寄存器的数量
			ArrayRegisters		= 4,
			//字段寄存器的数量
			FieldRegisters		= 4,
			//直接整数寄存器的数量
			IntDirectRegisters	= 8,
			//直接浮点寄存器的数量
			DoubleDirectRegisters = 4,
			//直接字符串寄存器数量
			StringDirectRegisters = 2,
			//指令寄存器数量
			InsRegisters = 8,	//2bit for param count 1bit for extern
			//流控制寄存器
			ControlRegisters = 36,
			//符号表指令
			MetaRegisters = 2,
			//累计寄存器的数量
			CountRegisters      = GeneralRegisters + IntRegisters + DoubleRegisters +
				ParamRegisters + VarRegisters + PointerRegisters	+ ArrayRegisters +
				FieldRegisters + IntDirectRegisters + DoubleDirectRegisters + StringDirectRegisters+
				InsRegisters + ControlRegisters + MetaRegisters + 4
		};

		enum Reg : unsigned char
		{
			//寄存器开始常数
			RegStart = 1,

			//通用寄存器
			E0	= RegStart,
			ECP = E0,
			ESP,
			EBP, 
			This,
			Ee	= E0 + GeneralRegisters - 1,
			//整数寄存器
			R0,
			EAX = R0,
			EBX,
			ECX,
			EDX,
			ESI,
			EDI,
			Re  = R0 + IntRegisters - 1,
			//浮点数寄存器
			D0	,
			De  = D0 + DoubleRegisters - 1,
			//参数寄存器 
			P0,
			Pe  = P0 + ParamRegisters - 1,
			//变量寄存器
			V0	,	
			Ve  = V0 + VarRegisters - 1,
			//指针寄存器
			T0 ,
			Te = T0 + PointerRegisters - 1,
			//数组寄存器
			A0 ,	//one dim
			Ae	= A0  + ArrayRegisters - 1,	//if dim>3 then next byte is count of dims
			//字段寄存器
			S0 ,
			Se  = S0 + FieldRegisters - 1,
			//直接数寄存器
			I0,
			Ie = I0 + IntDirectRegisters - 1,
			//浮点直接数寄存器
			F0,
			Fe = F0 + DoubleDirectRegisters - 1,
			//字符串直接寄存器
			L0,
			Le = L0 + StringDirectRegisters - 1,
			//指令寄存器
			N0,
			Ne = N0 + InsRegisters - 1,
			//块指令寄存器
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
			//元数据寄存器
			M0,
			Me = M0 + MetaRegisters,
			//忽略
			SKIP,
			//空值
			NIL,
			//寄存器结束常数
			RegEnd
		};

		//定义整数类型
		enum eIntType
		{
			//无符号整数
			U8		= ((0x1<<2)|0x00),
			U16		= ((0x1<<2)|0x01),
			U32		= ((0x1<<2)|0x02),
			U64		= ((0x1<<2)|0x03),
			//整数
			I8		= 0x00,
			I16		= 0x01,
			I32		= 0x02,
			I64		= 0x03
		};

		//元数据指令
		enum mcode : unsigned char
		{
			Make	= 0x20,	//创建
			Set		= 0x21,	//设置元数据项
			Enter	= 0x22,	//进入
			Leave	= 0x23, //离开
			Use		= 0x24,	//可访问
			Get		= 0x25,
			Find	= 0x26
		};

		//元数据类
		enum mtype : unsigned char
		{
			Module		= 0x20,	//模块
			Unit		= 0x21,	//单元
			NameSpace	= 0x22,	//命名空间

			Class		= 0x30,	//类
			Struct		= 0x31,	//结构
			Enum		= 0x32,	//枚举
			Union		= 0x33,	//联合
			Interface	= 0x34,	//接口

			Function	= 0x40,	//函数
			Ramda		= 0x41,	//ramda函数

			DclType		= 0x50,	//声明类型
			DclVar		= 0x51,	//声明变量
			DclParam	= 0x52,	//声明变量

			Super		= 0x53
		};

		//元数据项
		enum mItem : unsigned char
		{
			ItemSpec	= 0x21,	//规格
			ItemType	= 0x22,	//类型
			ItemName	= 0x23,	//名称
			ItemNote	= 0x24,	//备注
			ItemParam	= 0x25,	//参数
			ItemVar		= 0x26,	//参数
			ItemField	= 0x27,	//字段
			ItemMethod	= 0x28,	//方法
			ItemBody	= 0x29	//内容
		};

		//虚拟机指令
		enum pcode : unsigned char
		{
			LAD		= 0x20 ,
			STO		= 0x21 ,

			MOV		= 0x22 ,
			CST		= 0x23 ,	//类型转化

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
			FCT		= 0x64 ,  //浮点标准函数

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