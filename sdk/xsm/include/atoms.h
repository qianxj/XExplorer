#pragma once

namespace xcode
{
	enum
	{
		xa_undef	= 0x0000,	//#undef
		xa_type		= 0x0010,	//#type
		xa_var		= 0x0011,	//#var
		xa_atom		= 0x0012,	//#atom
		xa_int		= 0x0021,	//#int
		xa_float	= 0x0022,	//#float
		xa_double	= 0x0023,	//#double
		xa_string	= 0x0024,	//#string
		xa_indent	= 0x0025,	//#indent
		xa_class	= 0x0026,	//#class
		xa_struct	= 0x0027,	//#struct
		xa_enum		= 0x0028,	//#enum
		xa_union	= 0x0029,	//#union
		xa_function	= 0x0030,	//#function
		xa_tuple	= 0x0031,	//#tuple
		xa_stmt		= 0x0032,	//#stmt
		xa_pointer	= 0x0033,	//#pointer
		xa_long		= 0x0034,	//long
		xa_byte		= 0x0035,	//byte
		xa_word		= 0x0036,	//word
		xa_dword	= 0x0037,	//dword
		xa_iconst	= 0x0038,	//int const
		xa_dconst	= 0x0039,	//double const
		xa_sconst	= 0x0040,	//string const
		xa_bool		= 0x0041,	//bool
		xa_interface= 0x0042,	//interface
		xa_uint		= 0x0043,	//uint
		xa_void		= 0x0044,	//void
		xa_nclass	= 0x0045,	//#class
		xa_char 	= 0x0046,	//#char
		xa_nstruct 	= 0x0048,	//nstruct
		xa_oleobject= 0x0049,	//oleobject
		xa_oleobjectex = 0x004A,//oleobjectex
		xa_any	 	= 0x0047,	//any
		xa_nany		= 0x004B,	//native any
		xa_astring	= 0x0049	//astring
	};
}
