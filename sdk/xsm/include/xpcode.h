#pragma once
#include <windows.h>

namespace xcode
{
	enum  {
		XC_PUSH			= 0x01, //S(type|indent| string |double| int)
		XC_POP			= 0x02, //empty | $v0..4
		XC_F			= 0x03, //opeartor
		XC_ARRAY		= 0x04, //int | $v0..4<-dim tuple
		XC_MAKE			= 0x05, //S(0:#tuple|1:#ruler|2:#selection|3:#desc|4:#step|5:#combo) + S($v0..4|int)
		XC_DCLV			= 0x07, //indent | $v0..4<-ref 
		XC_DCLT			= 0x08, //indent | $v1..4<-ref 
		XC_DCLF			= 0x09, //indent | $v1..4<-ref 
		XC_WITH			= 0x0A, //indent | $v0..4<-ref
		XC_ELEMENT		= 0x0B, //indent | $v0..4<-ref
		XC_USING		= 0x10, //indent | $v0..4<-ref
		XC_EXEC			= 0x13, //stmt:indent |$v0..4<-ref
		XC_NAME			= 0x17, //indent
		XC_REF			= 0x18, //S($v0..4|int)

		XC_CALL			= 0x20, //S($v0..4|int)
		XC_RET			= 0x21, //stack:S(empty | int) + ret:$v0..4 

		XC_JMP			= 0x30,//S($$"+"int | $$"-"int ||addr ||$v0..4)
		XC_JMPF			= 0x31,//S($$"+"int | $$"-"int ||addr ||$v0..4)
		XC_JMPT			= 0x32,//S($$"+"int | $$"-"int ||addr ||$v0..4)
		XC_LABEL		= 0x33,//indent || $v0..4
		XC_NOP			= 0x40,

		XC_INC			= 0x50, //$v0..v4
		XC_DEC			= 0x51, //$v0..v4

		//debug
		XC_QUERY		= 0x80, //indent
		XC_BREAK		= 0x81,

		XC_EXIT			= 0x82 //exit
	};

	enum {
		XC_BLKSTART		= 0x90, //indent | $v0..4<-ref
		XC_BLKEND		= 0x91,
		XC_FCTITEM		= 0x92, //S(0:#desc|1:#code|2:#fctret|3:#fctarg)
		XC_INTERFACE	= 0x93, 
		XC_CLASS		= 0x94, 
		XC_CLASSITEM	= 0x95,  //S(0:#decl|1:#code|2:#derived|3:#prop|4:#interface|5:#include|6:#method|7:impl|8:#event|9:#field)
		XC_INCLUDEITEM	= 0x96,  //S(0:#class|1:#interface|2:#prop|3:#include|4:#method|5:impl|6:#event)
		XC_UNION		= 0xA0,
		XC_STRUCT		= 0xA1,
		XC_ENUM			= 0xA2 
	};

	struct code
	{
		unsigned char order;
		unsigned char v;
		union
		{
			unsigned char  x[2];
			short h;
		};
		union
		{
			double d;
			float  f;
			int	   n;
			void* p[2];
		};
	};
}
