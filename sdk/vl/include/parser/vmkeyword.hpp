#pragma once
#include "../vl.h"

namespace vl
{
	#define	_cp_token_key_def_start	0x0
	#define	_cp_token_key_any		0x1
	#define	_cp_token_key_as		0x2
	#define	_cp_token_key_asm		0x3
	#define	_cp_token_key_auto	0x4
	#define	_cp_token_key_blob	0x5
	#define	_cp_token_key_bool	0x6
	#define	_cp_token_key_break	0x7
	#define	_cp_token_key_byref	0x8
	#define	_cp_token_key_byte	0x9
	#define	_cp_token_key_byval	0xa
	#define	_cp_token_key_case	0xb
	#define	_cp_token_key_catch	0xc
	#define	_cp_token_key_char	0xd
	#define	_cp_token_key_class	0xe
	#define	_cp_token_key_const	0xf
	#define	_cp_token_key_const_cast	0x10
	#define	_cp_token_key_continue	0x11
	#define	_cp_token_key_default	0x12
	#define	_cp_token_key_delete	0x13
	#define	_cp_token_key_do	0x14
	#define	_cp_token_key_double	0x15
	#define	_cp_token_key_dynamic_cast	0x16
	#define	_cp_token_key_else	0x17
	#define	_cp_token_key_enum	0x18
	#define	_cp_token_key_explicit	0x19
	#define	_cp_token_key_export	0x1a
	#define	_cp_token_key_extern	0x1b
	#define	_cp_token_key_false	0x1c
	#define	_cp_token_key_float	0x1d
	#define	_cp_token_key_for	0x1e
	#define	_cp_token_key_friend	0x1f
	#define	_cp_token_key_function	0x20
	#define	_cp_token_key_goto	0x21
	#define	_cp_token_key_if	0x22
	#define	_cp_token_key_inline	0x23
	#define	_cp_token_key_int	0x24
	#define	_cp_token_key_interface	0x25
	#define	_cp_token_key_long	0x26
	#define	_cp_token_key_mutable	0x27
	#define	_cp_token_key_namespace	0x28
	#define	_cp_token_key_native	0x29
	#define	_cp_token_key_new	0x2a
	#define	_cp_token_key_nil	0x2b
	#define	_cp_token_key_oleobject	0x2c
	#define	_cp_token_key_operator	0x2d
	#define	_cp_token_key_out	0x2e
	#define	_cp_token_key_param	0x2f
	#define	_cp_token_key_private	0x30
	#define	_cp_token_key_protected	0x31
	#define	_cp_token_key_public	0x32
	#define	_cp_token_key_readonly	0x33
	#define	_cp_token_key_ref	0x34
	#define	_cp_token_key_register	0x35
	#define	_cp_token_key_reinterpret_cast	0x36
	#define	_cp_token_key_return	0x37
	#define	_cp_token_key_short	0x38
	#define	_cp_token_key_signed	0x39
	#define	_cp_token_key_sizeof	0x3a
	#define	_cp_token_key_static	0x3b
	#define	_cp_token_key_static_cast	0x3c
	#define	_cp_token_key_stdcall	0x3d
	#define	_cp_token_key_string	0x3e
	#define	_cp_token_key_struct	0x3f
	#define	_cp_token_key_switch	0x40
	#define	_cp_token_key_template	0x41
	#define	_cp_token_key_this	0x42
	#define	_cp_token_key_throw	0x43
	#define	_cp_token_key_true	0x44
	#define	_cp_token_key_try	0x45
	#define	_cp_token_key_type	0x46
	#define	_cp_token_key_typedef	0x47
	#define	_cp_token_key_typeid	0x48
	#define	_cp_token_key_typename	0x49
	#define	_cp_token_key_uint	0x4a
	#define	_cp_token_key_union	0x4b
	#define	_cp_token_key_unit	0x4c
	#define	_cp_token_key_unnative	0x4d
	#define	_cp_token_key_unsigned	0x4e
	#define	_cp_token_key_using	0x4f
	#define	_cp_token_key_var	0x50
	#define	_cp_token_key_virtual	0x51
	#define	_cp_token_key_void	0x52
	#define	_cp_token_key_volatile	0x53
	#define	_cp_token_key_wchar_t	0x54
	#define	_cp_token_key_while	0x55
	#define	_cp_token_key_def_end	0x56
		//keyword  type
	#define	_cp_key_type_stmt				0x0001
	#define	_cp_key_type_spec	       		0x0002
	#define	_cp_key_type_cast	       		0x0004
	#define	_cp_key_type_expr	      		0x0008
	#define	_cp_key_type_type	       		0x0010
	#define	_cp_key_type_class	      	0x0020
	#define	_cp_key_type_stype	       	0x0040
	#define	_cp_key_type_other	       	0x0080

	#define	_cp_key_type_stmt_selection		0x0100
	#define	_cp_key_type_stmt_iteration		0x0200
	#define	_cp_key_type_stmt_jump	 		0x0400
	#define	_cp_key_type_stmt_label			0x0800
	#define	_cp_key_type_stmt_exception		0x1000
		//type spec
	#define	_cp_spec_stdcall				0x00000001
	#define	_cp_spec_pascal					0x00000002
	#define	_cp_spec_pure					0x00000004
	#define	_cp_spec_const					0x00000008
	#define	_cp_spec_control				0x00000010
	#define	_cp_spec_export					0x00000020
	#define	_cp_spec_unsigned				0x00000040
	#define	_cp_spec_signed					0x00000080
	#define	_cp_spec_protected				0x00000100
	#define	_cp_spec_public					0x00000200
	#define	_cp_spec_private				0x00000400
	#define	_cp_spec_native					0x00000800
	#define	_cp_spec_inline					0x00001000
	#define	_cp_spec_virtual				0x00002000
	#define	_cp_spec_explicit				0x00004000
	#define	_cp_spec_event	     		    0x00008000
	#define	_cp_spec_static	     		    0x00010000
	#define	_cp_spec_auto	     		    0x00020000
	#define	_cp_spec_register	 		    0x00040000
	#define	_cp_spec_extern	     		    0x00080000
	#define	_cp_spec_mutable	 		    0x00100000
	#define	_cp_spec_volatile	 		    0x00200000
	#define	_cp_spec_friend		 		    0x00400000
	#define	_cp_spec_typedef	 		    0x00800000
	#define	_cp_spec_ansi		 		    0x01000000
	#define	_cp_spec_byref		 		    0x02000000
	#define	_cp_spec_byval		 		    0x04000000
	#define	_cp_spec_out		 		    0x08000000
	#define	_cp_spec_unnative	 		    0x10000000
	#define	_cp_spec_thisex	     		    0x20000000
	#define	_cp_spec_bstr	     		    0x40000000
	#define	_cp_spec_constex     		    0x80000000

	struct keytoken
	{	
		tchar* name;
		int key;
		int typ;
		unsigned int val;
	};

	extern keytoken  keywords[];
}