#ifndef  WPP_VORDDEF_H
#define  WPP_VORDDEF_H

#include "vlang4.h"
#include "global.hpp"

namespace vord {

//vord data
struct vitem_t
{
	int id_;  //vord id, index to info_t
	int gid_; //group id;
};

//vord category 
struct info_t
{
	int_t first;
	int_t last;
	int_t rcount; //ref count
	int_t reference;
	int_t next;  //use for free list
	int_t piror;
	int_t parent;
	int_t child;
};

class vord_t;
struct value_t
{
	union
	{
		uint_t		uval;
		int_t		val;
		double_t	dval;
		ptr_t		ptr;
		wstring_t	sval;
		int64_t		val64;
		uint64_t	uval64;
		struct
		{
			int_t	hval;
			int_t	lval;
		};
		vitem_t		wi;
	};

	value_t():hval(0),lval(0){}
	value_t(int_t v):val(v){}
	value_t(uint_t v):uval(v){}
	value_t(int64_t v):val64(v){}
	value_t(uint64_t v):uval64(v){}
	value_t(double_t v):dval(v){}
	value_t(float_t v):dval(v){}
	value_t(ptr_t v):ptr(v){}
	value_t(wchar_t* v):sval(v){}
	value_t(const wchar_t* v):sval((wstring_t)v){}
	value_t(vord_t&  v);

	VLANG4_API static const value_t * nilValue();
	bool isnil(value_t&  v){ return &v ==nilValue()? true:false;}
	bool isnil(){return this == nilValue()? true:false;}
};

//vord detail 
struct item_t
{
	uint_t		next;
	uint_t		pred;
	value_t		val;
};

typedef int vordtype_t;
struct vordtype_
{
	static const int undef_	= 0x0;
	static const int unvalid_ = 0x1;
	static const int int8_	= 0x2;
	static const int int16_   = 0x3;
	static const int int32_   = 0x4;
	static const int int64_   = 0x5;
	static const int uint8_   = 0x6;
	static const int uint16_  = 0x7;
	static const int uint32_  = 0x8;
	static const int uint64_  = 0x9;
	static const int char_	= 0xA;
	static const int byte_	= 0xB ;
	static const int bool_	= 0xC;
	static const int real32_  = 0xD ;
	static const int real64_  = 0xE;
	static const int real80_  = 0xF;
	static const int string_  = 0x10;
	static const int ptr_	 = 0x11;
	static const int void_	 = 0x12;

	static const int int_	= int32_;
	static const int uint_	= uint32_;
	static const int long_	= uint32_;
	static const int double_	= real64_;
	static const int float_	= real32_;

	static const int astring_    = 0x13;
	static const int class_		= 0x14;
	static const int struct_		= 0x15;
	static const int interface_	= 0x16;
	static const int union_		= 0x17;
	static const int enum_		= 0x18;
	static const int array_		= 0x19;
	static const int function_	= 0x1A;
	static const int list_		= 0x1B;
	static const int voc_		= 0x1C;
	//static const int pred_		= 0x1D;

	//ole support
	static const int oleobject_	= 0x20;
	static const int oleitem_	= 0x21;
	static const int variant_	= 0x22;
	static const int blob_		= 0x23;
	static const int guid_		= 0x24;

	//Âß¼­ÀàÐÍ
	static const int t_term		= 0x40; 
	static const int t_prop		= 0x41;
	static const int t_element	= 0x42;
	static const int t_relation	= 0x43;
	static const int t_axiom	= 0x44;
	static const int t_equation	= 0x45;
	static const int t_word		= 0x46;
	static const int t_stmt		= 0x47;
	static const int t_lang		= 0x48;

	static const int dim_		=  0x100;
};

} //namespace vord

#endif
