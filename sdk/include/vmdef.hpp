#ifndef WPP_VMDEF_H
#define WPP_VMDEF_H

#include "pred.hpp"

using namespace std;
using namespace wpp::how;

namespace vm
{
	namespace spec
	{
		const int 
		stdcall_	= 0x00000001,
		pascal_		= 0x00000002,
		pure_		= 0x00000004,
		const_		= 0x00000008,
		control_	= 0x00000010,
		export_		= 0x00000020,
		unsigned_	= 0x00000040,
		signed_		= 0x00000080,
		protected_	= 0x00000100,
		public_		= 0x00000200,
		private_	= 0x00000400,
		native_		= 0x00000800,
		inline_		= 0x00001000,
		virtual_	= 0x00002000,
		explicit_	= 0x00004000,
		event_		= 0x00008000,
		static_		= 0x00010000,
		auto_		= 0x00020000,
		register_	= 0x00040000,
		extern_		= 0x00080000,
		mutable_	= 0x00100000,
		volatile_	= 0x00200000,
		friend_		= 0x00400000,
		typedef_	= 0x00800000,
		ansi_		= 0x01000000,
		byref_		= 0x02000000,
		byval_		= 0x04000000,
		out_		= 0x08000000,
		manage_		= 0x10000000,
		thisex_		= 0x20000000,
		bstr_		= 0x40000000,
		constex_	= 0x80000000;
	}
	
	namespace opcode
	{
		const unsigned char 
		nop   = 0x00,
		mov   = 0x01,
		add   = 0x02,
		sub   = 0x03,
		mul   = 0x04,
		div   = 0x05,
		or    = 0x08,
		and   = 0x09,
		xor   = 0x0A,
		not   = 0x0B,
		neg   = 0x0C,
		shl   = 0x0D,
		shr   = 0x0E,
		ret   = 0x0F,
		call  = 0x10,
		push  = 0x11,
		pop   = 0x12,
		inc   = 0x13,
		dec   = 0x14,
		jmp   = 0x15,
		jz	  = 0x16,
		jnz   = 0x17,
		jgt	  = 0x18,
		jge	  = 0x19,
		jlt   = 0x1A,
		jle   = 0x1B,
		jeq   = 0x1C,
		jne   = 0x1D,
		zero  = 0x20,
		nil   = 0x21,
		gtr   = 0x22,
		gte   = 0x23,
		ltr   = 0x24,
		lte   = 0x25,
		eq    = 0x26,
		neq   = 0x27,
		br	  = 0x30,

		yadd   = 0x40,
		ysub   = 0x41,
		ymul   = 0x42,
		ydiv   = 0x43,
		yor    = 0x44,
		yand   = 0x45,
		yeq	   = 0x46,
		yseq   = 0x47;
	}

	namespace type
	{
		const unsigned char
		//值类型
		undef_		= 0x00,
		uint8_		= 0x01,
		uint16_		= 0x02,
		uint32_		= 0x03,
		int8_		= 0x04,
		int16_		= 0x05,
		int32_		= 0x06,
		float_		= 0x07,
		double_		= 0x08,
		string_		= 0x09,
		pointer_	= 0x0A,
		array_		= 0x0B,
		class_		= 0x0C,
		struct_		= 0x0D,
		interface_	= 0x0F,

		//别名类别
		int_		= int32_,
		uint_		= uint32_,
		char_		= int16_,
		uchar_		= uint16_,
		bool_		= uint8_,

		//逻辑类型
		term_		= 0x20, 
		prop_		= 0x21,
		element_	= 0x22,
		relation_	= 0x23,
		axiom_		= 0x24,
		equation_	= 0x25,
		word_		= 0x26,
		stmt_		= 0x27,
		lang_		= 0x28;
	}

	//atom type
	namespace atype 
	{
		decl_pred_(Undef,	"vm-atype:undef");
		decl_pred_(UInt8,	"vm-atype:uint8");
		decl_pred_(UInt16,	"vm-atype:uint16");
		decl_pred_(UInt32,	"vm-atype:uint32");
		decl_pred_(Int8,	"vm-atype:int8");
		decl_pred_(Int16,	"vm-atype:int16");
		decl_pred_(Int32,	"vm-atype:int32");
		decl_pred_(Float,	"vm-atype:float");
		decl_pred_(Double,	"vm-atype:double");
		decl_pred_(String,	"vm-atype:string");
		decl_pred_(Pointer,	"vm-atype:pointer");
		decl_pred_(Array,	"vm-atype:array");
		decl_pred_(Class,	"vm-atype:class");
		decl_pred_(Struct,	"vm-atype:struct");
		decl_pred_(Interface,"vm-atype:interface");

		//别名类别
		decl_pred_(Int,		"vm-atype:int");
		decl_pred_(UInt,		"vm-atype:uint");
		decl_pred_(Char,		"vm-atype:char");
		decl_pred_(UChar,	"vm-atype:uchar");
		decl_pred_(Bool,		"vm-atype:bool");

		//逻辑类型
		decl_pred_(term,		"vm-atype:term"); 
		decl_pred_(prop,		"vm-atype:prop");
		decl_pred_(element,		"vm-atype:element");
		decl_pred_(relation,	"vm-atype:relation");
		decl_pred_(axiom,		"vm-atype:axiom");
		decl_pred_(equation,	"vm-atype:equation");
		decl_pred_(word,		"vm-atype:word");
		decl_pred_(stmt,		"vm-atype:stmt");
		decl_pred_(lang,		"vm-atype:lang");
	}
}

#endif //WPP_VMDEF_H