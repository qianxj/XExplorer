#ifndef WPP_TYPE_H
#define WPP_TYPE_H

#include "xsdpred.hpp"

namespace wpp {

#define null 0
typedef int type_category_t;
struct type_category
{
	static const int undef_		= 0x00;
	static const int int_	 	= 0x01;
	static const int double_ 	= 0x02;
	static const int bool_	 	= 0x04;
	static const int string_	= 0x10;
	static const int class_		= 0x20;
	static const int pointer_	= 0x40;
	static const int void_		= 0x80;
	static const int bit_		= 0x100;
	static const int date_		= 0x200;
	static const int ole_		= 0x400;
	static const int tuple_		= 0x1000;
	static const int any_		= 0x2000;
	static const int blob_		= 0x4000;
	static const int type_		= 0x8000;
	static const int guid_		= 0x10000;
	static const int handle_	= 0x20000;
	static const int array_		= 0x40000;
	static const int term_		= 0x80000;
	static const int function_	= 0x100000;
	static const int list_		= 0x200000;
	static const int oleobject_	= 0x400000;

	static const int number_    = int_ | double_;
};

struct type_info
{
	int type;
	int size;
	int category;
	bool constructor;
	bool valtype;
};

typedef int type_spec_t;
namespace type_spec
{
	const type_spec_t 
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

typedef int type_t;
struct type
{
	static const unsigned int type_mask = ((0x1<< 5) - 1);
	static const int type_extbits		= 3;
	static const int type_classshift	= 8;

	static const type_t undef_	= 0x0;
	static const type_t unvalid_ = 0x1;
	static const type_t int8_	= 0x2;
	static const type_t int16_   = 0x3;
	static const type_t int32_   = 0x4;
	static const type_t int64_   = 0x5;
	static const type_t uint8_   = 0x6;
	static const type_t uint16_  = 0x7;
	static const type_t uint32_  = 0x8;
	static const type_t uint64_  = 0x9;
	static const type_t char_	= 0xA;
	static const type_t byte_	= 0xB ;
	static const type_t bool_	= 0xC;
	static const type_t real32_  = 0xD ;
	static const type_t real64_  = 0xE;
	static const type_t real80_  = 0xF;
	static const type_t string_  = 0x10;
	static const type_t ptr_	 = 0x11;
	static const type_t void_	 = 0x12;

	static const type_t int_	= int32_;
	static const type_t uint_	= uint32_;
	static const type_t long_	= uint32_;
	static const type_t double_	= real64_;
	static const type_t float_	= real32_;

	static const type_t astring_    = 0x13;
	static const type_t class_		= 0x14;
	static const type_t struct_		= 0x15;
	static const type_t interface_	= 0x16;
	static const type_t union_		= 0x17;
	static const type_t enum_		= 0x18;
	static const type_t array_		= 0x19;
	static const type_t function_	= 0x1A;
	static const type_t list_		= 0x1B;
	static const type_t voc_		= 0x1C;
	//static const type_t pred_		= 0x1D;

	//ole support
	static const type_t oleobject_	= 0x20;
	static const type_t oleitem_	= 0x21;
	static const type_t variant_	= 0x22;
	static const type_t blob_		= 0x23;
	static const type_t guid_		= 0x24;

	//逻辑类型
	static const type_t t_term		= 0x40; 
	static const type_t t_prop		= 0x41;
	static const type_t t_element	= 0x42;
	static const type_t t_relation	= 0x43;
	static const type_t t_axiom		= 0x44;
	static const type_t t_equation	= 0x45;
	static const type_t t_word		= 0x46;
	static const type_t t_stmt		= 0x47;
	static const type_t t_lang		= 0x48;

	static const wchar_t* get_type_string(type_t ty);
};

struct type_broader
{
	int type1;
	int type2;
};

struct type_narrow
{
	int type1;
	int type2;
};

struct type_binary_operate
{
	int type1;
	int type2;
	int dest_type;
	int op_type1;
	int op_type2;
};

class  wpp_type
{
public:
	 static unsigned int get_type_spec(const wchar_t* spec);
public:
	 static const type_info* get_type_info();
	 static const type_info* get_type_info(type_t ty);

public:
	static const type_broader * get_type_broader();
	static const type_broader * get_type_broader(type_t ty);
public:
	//宽化
	 static bool broader(type_t t1,type_t t2);
	 //窄化
	 static bool narrow(type_t t1,type_t t2);
	  //最小上界
	static type_t  lup(type_t t1,type_t t2);
	 //显式转换
	inline static bool exp_conv(type_t t1,type_t t2){ return broader(t1,t2); }
	  //隐式转换
	 inline static bool imp_conv(type_t t1,type_t t2){ return broader(t1,t2)||narrow(t1,t2); }
	 //匹配
	 static bool match(type_t t1,type_t t2);
	 //置换
	 static void substitution(type_t t1,type_t t2);
	 //转换
	 static void convert(type_t t1,type_t t2);		
	 //合一算法
	 static void unify(type_t t1,type_t t2);
};

} //namespace wpp

#endif //WPP_TYPE_H


