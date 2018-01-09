#pragma once
#include "../vl.h"
#include "vldef.hpp"

namespace vl
{
	#define	_type_flag_native			1
	#define	_type_flag_array			1
	
	//type use six bit
	#define	_type_undef				0x0
	#define	_type_int				0x1
	#define	_type_double			0x2
	#define	_type_string			0x3
 	#define	_type_void				0x4
	#define	_type_pvoid				0x5
    #define	_type_char				0x6
	#define	_type_bool				0x7
	#define	_type_float				0x8
	#define	_type_uint				0x9
	#define	_type_byte				0xA
	#define	_type_bit				0xB
	#define	_type_tuple				0xC
	#define	_type_dtuple			0xD
	#define	_type_atom				0xE
	#define	_type_any				0xF
	#define	_type_astring			0x10
	#define	_type_blob				0x11
	#define	_type_function			0x12
	#define	_type_array				0x13
	#define	_type_class				0x14
	#define	_type_struct			0x15
	#define	_type_interface			0x16
	#define	_type_enum				0x17
    #define	_type_type				0x18
	#define	_type_aint				0x19
	#define	_type_union				0x1A
	#define	_type_long				0x1B
	#define	_type_unit				0x1C
	#define	_type_guid				0x1D
	#define	_type_oleobject			0x1E
	#define	_type_nclass			0x1F
	
	#define	_type_int_int8	        0x20
	#define	_type_int_int16	        0x21
	#define	_type_int_int32	        0x22
	#define	_type_int_int64	        0x23
	#define	_type_int_int128	    0x24

	#define	_type_uint_uint8	    0x25
	#define	_type_uint_uint16	    0x26
	#define	_type_uint_uint32	    0x27
	#define	_type_uint_uint64	    0x28
	#define	_type_uint_uint128	    0x29

	#define	_type_double_double32   0x08
	#define	_type_double_double64   0x02
	#define	_type_double_double80	0x2A
	#define	_type_double_double128  0x2B
	
	#define	_type_oleobjectitem		0x2C
	#define	_type_variant			0x2D
	#define	_type_param				0x2E
	
	#define	_type_category_undef		0x00
	#define	_type_category_int	 		0x01
	#define	_type_category_double	 	0x02
	#define	_type_category_bool	 		0x04
	#define	_type_category_string		0x10
	#define	_type_category_class		0x20
	#define	_type_category_pointer		0x40
	#define	_type_category_void			0x80
	#define	_type_category_bit			0x100
	#define	_type_category_date			0x200
	#define	_type_category_ole			0x400
	#define	_type_category_tuple		0x1000
	#define	_type_category_any			0x2000
	#define	_type_category_blob			0x4000
	#define	_type_category_type			0x8000
	#define	_type_category_guid			0x10000
	#define	_type_category_handle		0x20000
	#define	_type_category_array		0x40000
	
	#define	_type_category_number		_type_category_int|_type_category_double

	 //token sym type
	#define	_sym_typeref	0x1024
	#define	_sym_type	0x1025
	#define	_sym_class	0x1026
	#define	_sym_field	0x1027
	#define	_sym_method	0x1028
	#define	_sym_memberref	0x1029
	#define	_sym_param	0x102a
	#define	_sym_var	0x102b
	#define	_sym_classextend	0x102c
	#define	_sym_namespace	0x102d
	#define	_sym_pcode	0x102e
	#define	_sym_ncode	0x102f
	#define	_sym_expression	0x1030
	#define	_sym_nativemethod	0x1031
	#define	_sym_typearray	0x1032
	#define	_sym_typearrayarea	0x1033
	
	#define	_sym_class_none	 			0x00
	#define	_sym_class_class	 		0x01
	#define	_sym_class_struct	 		0x02
	#define	_sym_class_union	 		0x03
	#define	_sym_class_unit		 		0x04
	#define	_sym_class_interface		0x05
	#define	_sym_class_enum				0x06
 //token metatable class
	#define	_sym_meta_typeref			0x01
	#define	_sym_meta_typedef			0x02
	#define	_sym_meta_fieldptr			0x03
	#define	_sym_meta_field				0x04
	#define	_sym_meta_methodptr			0x05
	#define	_sym_meta_method			0x06
	#define	_sym_meta_paramptr			0x07
	#define	_sym_meta_param				0x08
	#define	_sym_meta_interfaceimpl		0x09
	#define	_sym_meta_memberref			0x0A
	#define	_sym_meta_constant			0x0B
	
	#define	_sym_meta_pcode				0x70
	#define	_sym_meta_ncode				0x71
	#define	_sym_meta_expression		0x72
	#define	_sym_meta_nativemethod		0x73
	#define	_sym_meta_typearray			0x74
	#define	_sym_meta_typearrayarea		0x74
    
	#define _type_unsafe				(0x1<<7)
	#define _type_nativee				(0x1<<7)
	#define _type_ref					(0x1<<6)
	#define _type_const					(0x1<<5)
	#define _type_bstr					(0x1<<4)

	//type spec
	#define	_cp_spec_stdcall	          0x00000001
	#define	_cp_spec_pascal	          0x00000002
	#define	_cp_spec_pure		        0x00000004
	#define	_cp_spec_const	           0x00000008
	#define	_cp_spec_control	         0x00000010
	#define	_cp_spec_export	         0x00000020
	#define	_cp_spec_unsigned	     0x00000040
	#define	_cp_spec_signed	         0x00000080
	#define	_cp_spec_protected	     0x00000100
	#define	_cp_spec_public	          0x00000200
	#define	_cp_spec_private	        0x00000400
	#define	_cp_spec_native	         0x00000800
	#define	_cp_spec_inline		       0x00001000
	#define	_cp_spec_virtual		       0x00002000
	#define	_cp_spec_explicit		       0x00004000
	#define	_cp_spec_event	          0x00008000
	#define	_cp_spec_static	          0x00010000
	#define	_cp_spec_auto	            0x00020000
	#define	_cp_spec_register	       0x00040000
	#define	_cp_spec_extern	         0x00080000
	#define	_cp_spec_mutable	       0x00100000
	#define	_cp_spec_volatile	        0x00200000
	#define	_cp_spec_friend		       0x00400000
	#define	_cp_spec_typedef	       0x00800000
	#define	_cp_spec_ansi		      0x01000000
	#define	_cp_spec_byref		      0x02000000
	#define	_cp_spec_byval		      0x04000000
	#define	_cp_spec_out		      0x08000000
	#define	_cp_spec_unnative	     0x10000000
	#define	_cp_spec_thisex	        0x20000000
	#define	_cp_spec_bstr	           0x40000000

	//control code
	#define	_code_if	 				0x80
	#define	_code_if_condition			0x8001
	#define	_code_if_true_body			0x8002
	#define	_code_if_false_body			0x8003
	
	#define	_code_for					0x81
	#define	_code_for_initial			0x8101
	#define	_code_for_condition			0x8102
	#define	_code_for_loop				0x8103
	#define	_code_for_body				0x8104
	
	#define	_code_while					0x82
	#define	_code_while_condition		0x8201
	#define	_code_while_body			0x8202
	
	#define	_code_do					0x83
	#define	_code_do_body				0x8301
	#define	_code_do_condition			0x8302
	
	#define	_code_switch				0x85
	#define	_code_switch_condition		0x8501
	#define	_code_switch_case_condition		0x8502
	//#define	_code_switch_case_body		0x8503
	#define	_code_switch_default			0x8504
	//#define	_code_switch_default_body		0x8505
	
	#define	_code_try					0x86
	#define	_code_try_body				0x8601
	#define	_code_try_catch_condition	0x8602
	#define	_code_try_catch_body		0x8603
	#define	_code_try_finally			0x8604
	
	#define	_code_label				0x87
	
	#define	_code_break				0x90
	#define	_code_goto				0x91
	#define	_code_continue			0x92
	#define	_code_throw				0x93
	#define	_code_return			0x94
	#define	_code_block_start		0x95
	#define	_code_block_end			0x96

	#define	_code_let_pool			0x98
	#define	_code_exp_pool			0x99
	#define	_code_let				0x9A
	#define	_code_exp				0x9B
	#define	_code_dclv				0x9C
	#define	_code_dclt				0x9D
	#define	_code_nop				0x9E
	#define	_code_debugbreak		0x9F
	
	#define	_code_version			0xA0

	struct  typetoken
	{
		tchar*			name;
		int			val;
		int				size;
		unsigned int	typecat;
	};

	class vmtype
	{
	public:
		vmtype();
		~vmtype();
	public:
		const static int undefT		= 0;
		const static int intT		= _type_int;
		const static int uintT		= _type_uint;
		const static int doubleT	= _type_double;
		const static int boolT		= _type_bool;
		const static int byteT		= _type_byte;
		const static int charT		= _type_char;
		const static int stringT	= _type_string;
		const static int rangeT		= 0;
		const static int arrayT		= _type_array;
		const static int tupleT		= _type_tuple;
		const static int structT	= _type_struct;
		const static int classT		= _type_class;
	public:
		static const tchar * getTypeString(int ntype);
		static int getType(const tchar * pt);
	public:
		static int getSize(int ntype);
		static int isMatch(int ty1 ,int ty2);
		static int isCanMatch(int ty1 ,int ty2);
	public:
		static int  type_result(int t1, int t2,int op,int &error, bool convert=false,int t3=0);
		static bool type_check(int from , int to);
		static bool TypeCanImpConv(int From, int To);
		static bool TypeCanExpConv(int From, int To);
		static int  GetTypeCat(int ty);
	};
}