	#pragma once
	#include 	"xsm.h"
 //type base
	#define	__atom_type_flag_native		1
	#define	__atom_type_flag_array			1
	
	//type use six bit
	#define	__atom_type_undef				0x0
	#define	__atom_type_int					0x1
	#define	__atom_type_double				0x2
	#define	__atom_type_string				0x3
 	#define	__atom_type_void					0x4
	#define	__atom_type_pvoid				0x5
        #define	__atom_type_char					0x6
	#define	__atom_type_bool					0x7
	#define	__atom_type_float					0x8
	#define	__atom_type_uint					0x9
	#define	__atom_type_byte					0xA
	#define	__atom_type_bit					0xB
	#define	__atom_type_stuple				0xC
	#define	__atom_type_dtuple				0xD
	#define	__atom_type_atom				0xE
	#define	__atom_type_any					0xF
	#define	__atom_type_astring				0x10
	#define	__atom_type_blob					0x11
	#define	__atom_type_function				0x12
	#define	__atom_type_array				0x13
	#define	__atom_type_class				0x14
	#define	__atom_type_struct				0x15
	#define	__atom_type_interface				0x16
	#define	__atom_type_enum				0x17
        #define	__atom_type_type					0x18
	#define	__atom_type_aint					0x19
	#define	__atom_type_union				0x1A
	#define	__atom_type_long					0x1B
	#define	__atom_type_unit					0x1C
	#define	__atom_type_guid					0x1D
	#define	__atom_type_oleobject				0x1E
	#define	__atom_type_nclass				0x1F
	
	#define	__atom_type_int_int8	               	0x20
	#define	__atom_type_int_int16	             	0x21
	#define	__atom_type_int_int32	             	0x22
	#define	__atom_type_int_int64	             	0x23
	#define	__atom_type_int_int128	            	0x24

	#define	__atom_type_uint_uint8	            	0x25
	#define	__atom_type_uint_uint16	          	0x26
	#define	__atom_type_uint_uint32	          	0x27
	#define	__atom_type_uint_uint64	          	0x28
	#define	__atom_type_uint_uint128	        	0x29

	#define	__atom_type_double_double32	        0x08
	#define	__atom_type_double_double64	        0x02
	#define	__atom_type_double_double80		0x2A
	#define	__atom_type_double_double128	      0x2B
	
	#define	__atom_type_oleobjectitem			0x2C
	#define	__atom_type_variant				0x2D
	#define	__atom_type_param				0x2E
	
	#define	__atom_type_extent0			0x3D    //扩展标志   
	#define	__atom_type_extent1			0x3E    //扩展标志   
	#define	__atom_type_extent2			0x3F    //扩展标志   
 //type extent 0
	//define extent int
	#define	__atom_type_int_int256	           0x15
	#define	__atom_type_int_int512	           0x16
	#define	__atom_type_int_int1024	         0x17
	#define	__atom_type_int_int2048	         0x18
	#define	__atom_type_int_int4096	         0x19
	#define	__atom_type_int_int8192	         0x1A
	#define	__atom_type_int_int16384	       0x1B
	#define	__atom_type_int_int32768	       0x1C

	#define	__atom_type_int_int8_1	              0x20
	#define	__atom_type_int_int16_1	             0x21
	#define	__atom_type_int_int32_1	            0x22
	#define	__atom_type_int_int64_1	             0x23
	#define	__atom_type_int_int128_1	           0x24
	#define	__atom_type_int_int256_1	           0x25
	#define	__atom_type_int_int512_1	           0x26
	#define	__atom_type_int_int1024_1	         0x27
	#define	__atom_type_int_int2048_1	         0x28
	#define	__atom_type_int_int4096_1	         0x29
	#define	__atom_type_int_int8192_1	         0x2A
	#define	__atom_type_int_int16384_1	       0x2B
	#define	__atom_type_int_int32768_1	       0x2C

	#define	__atom_type_int_int8_2	              0x30
	#define	__atom_type_int_int16_2	             0x31
	#define	__atom_type_int_int32_2	            0x32
	#define	__atom_type_int_int64_2	             0x33
	#define	__atom_type_int_int128_2	           0x34
	#define	__atom_type_int_int256_2	           0x35
	#define	__atom_type_int_int512_2	           0x36
	#define	__atom_type_int_int1024_2	         0x37
	#define	__atom_type_int_int2048_2	         0x38
	#define	__atom_type_int_int4096_2	         0x39
	#define	__atom_type_int_int8192_2	         0x3A
	#define	__atom_type_int_int16384_2	       0x3B
	#define	__atom_type_int_int32768_2	       0x3C

	#define	__atom_type_int_int8_4	              0x40
	#define	__atom_type_int_int16_4	             0x41
	#define	__atom_type_int_int32_4	            0x42
	#define	__atom_type_int_int64_4	             0x43
	#define	__atom_type_int_int128_4	           0x44
	#define	__atom_type_int_int256_4	           0x45
	#define	__atom_type_int_int512_4	           0x46
	#define	__atom_type_int_int1024_4	         0x47
	#define	__atom_type_int_int2048_4	         0x48
	#define	__atom_type_int_int4096_4	         0x49
	#define	__atom_type_int_int8192_4	         0x4A
	#define	__atom_type_int_int16384_4	       0x4B
	#define	__atom_type_int_int32768_4	       0x4C
 
	#define	__atom_type_int_int8_8	               0x50
	#define	__atom_type_int_int16_8	             0x51
	#define	__atom_type_int_int32_8	             0x52
	#define	__atom_type_int_int64_8	             0x53
	#define	__atom_type_int_int128_8	           0x54
	#define	__atom_type_int_int256_8	           0x55
	#define	__atom_type_int_int512_8	           0x56
	#define	__atom_type_int_int1024_8	         0x57
	#define	__atom_type_int_int2048_8	         0x58
	#define	__atom_type_int_int4096_8	         0x59
	#define	__atom_type_int_int8192_8	         0x5A
	#define	__atom_type_int_int16384_8	       0x5B
	#define	__atom_type_int_int32768_8	       0x5C

	//define extent uint      
	#define	__atom_type_uint_uint256	           	0x65
	#define	__atom_type_uint_uint512	           	0x66
	#define	__atom_type_uint_uint1024	        	0x67
	#define	__atom_type_uint_uint2048	         	0x68
	#define	__atom_type_uint_uint4096	         	0x69
	#define	__atom_type_uint_uint8192	         	0x6A
	#define	__atom_type_uint_uint16384	       	0x6B
	#define	__atom_type_uint_uint32768	       	0x6C

	#define	__atom_type_uint_uint8_1	              0x70
	#define	__atom_type_uint_uint16_1	             0x71
	#define	__atom_type_uint_uint32_1	            0x72
	#define	__atom_type_uint_uint64_1	             0x73
	#define	__atom_type_uint_uint128_1	           0x74
	#define	__atom_type_uint_uint256_1	           0x75
	#define	__atom_type_uint_uint512_1	           0x76
	#define	__atom_type_uint_uint1024_1	         0x77
	#define	__atom_type_uint_uint2048_1	         0x78
	#define	__atom_type_uint_uint4096_1	         0x79
	#define	__atom_type_uint_uint8192_1	         0x7A
	#define	__atom_type_uint_uint16384_1	       0x7B
	#define	__atom_type_uint_uint32768_1	       0x7C

	#define	__atom_type_uint_uint8_2	              0x80
	#define	__atom_type_uint_uint16_2	             0x81
	#define	__atom_type_uint_uint32_2	            0x82
	#define	__atom_type_uint_uint64_2	             0x83
	#define	__atom_type_uint_uint128_2	           0x84
	#define	__atom_type_uint_uint256_2	           0x85
	#define	__atom_type_uint_uint512_2	           0x86
	#define	__atom_type_uint_uint1024_2	         0x87
	#define	__atom_type_uint_uint2048_2	         0x88
	#define	__atom_type_uint_uint4096_2	         0x89
	#define	__atom_type_uint_uint8192_2	         0x8A
	#define	__atom_type_uint_uint16384_2	       0x8B
	#define	__atom_type_uint_uint32768_2	       0x8C

	#define	__atom_type_uint_uint8_4	              0x90
	#define	__atom_type_uint_uint16_4	             0x91
	#define	__atom_type_uint_uint32_4	            0x92
	#define	__atom_type_uint_uint64_4	             0x93
	#define	__atom_type_uint_uint128_4	           0x94
	#define	__atom_type_uint_uint256_4	           0x95
	#define	__atom_type_uint_uint512_4	           0x96
	#define	__atom_type_uint_uint1024_4	         0x97
	#define	__atom_type_uint_uint2048_4	         0x98
	#define	__atom_type_uint_uint4096_4	         0x99
	#define	__atom_type_uint_uint8192_4	         0x9A
	#define	__atom_type_uint_uint16384_4	       0x9B
	#define	__atom_type_uint_uint32768_4	       0x9C
 
	#define	__atom_type_uint_uint8_8	               0xA0
	#define	__atom_type_uint_uint16_8	             0xA1
	#define	__atom_type_uint_uint32_8	             0xA2
	#define	__atom_type_uint_uint64_8	             0xA3
	#define	__atom_type_uint_uint128_8	           0xA4
	#define	__atom_type_uint_uint256_8	           0xA5
	#define	__atom_type_uint_uint512_8	           0xA6
	#define	__atom_type_uint_uint1024_8	         0xA7
	#define	__atom_type_uint_uint2048_8	         0xA8
	#define	__atom_type_uint_uint4096_8	         0xA9
	#define	__atom_type_uint_uint8192_8	         0xAA
	#define	__atom_type_uint_uint16384_8	       0xAB
	#define	__atom_type_uint_uint32768_8	       0xAC

	//definde extent double
	#define	__atom_type_double_double256	           0xB3
	#define	__atom_type_double_double512	           0xB4
	#define	__atom_type_double_double1024	         0xB5
	#define	__atom_type_double_double2048	         0xB6
	#define	__atom_type_double_double4096	         0xB7
	#define	__atom_type_double_double8192	         0xB8
	#define	__atom_type_double_double16384	       0xB9
	#define	__atom_type_double_double32768	       0xBA

	#define	__atom_type_double_double8_4	              0xC0
	#define	__atom_type_double_double16_4	             0xC1
	#define	__atom_type_double_double32_4	            0xC2
	#define	__atom_type_double_double64_4	             0xC3
	#define	__atom_type_double_double128_4	           0xC4
	#define	__atom_type_double_double256_4	           0xC5
	#define	__atom_type_double_double512_4	           0xC6
	#define	__atom_type_double_double1024_4	         0xC7
	#define	__atom_type_double_double2048_4	         0xC8
	#define	__atom_type_double_double4096_4	         0xC9
	#define	__atom_type_double_double8192_4	         0xCA
	#define	__atom_type_double_double16384_4	       0xCB
	#define	__atom_type_double_double32768_4	       0xCC
 
	#define	__atom_type_double_double8_8	               0xD0
	#define	__atom_type_double_double16_8	             0xD1
	#define	__atom_type_double_double32_8	             0xD2
	#define	__atom_type_double_double64_8	             0xD3
	#define	__atom_type_double_double128_8	           0xD4
	#define	__atom_type_double_double256_8	           0xD5
	#define	__atom_type_double_double512_8	           0xD6
	#define	__atom_type_double_double1024_8	         0xD7
	#define	__atom_type_double_double2048_8	         0xD8
	#define	__atom_type_double_double4096_8	         0xD9
	#define	__atom_type_double_double8192_8	         0xDA
	#define	__atom_type_double_double16384_8	       0xDB
	#define	__atom_type_double_double32768_8	       0xDC
 //type extent 1
	#define	__atom_type_rect_int32					 0x20
	#define	__atom_type_rect_int64					 0x32
	#define	__atom_type_rect_double32				 0x33
	#define	__atom_type_rect_double64				 0x34

	#define	__atom_type_size_int32					 0x35
	#define	__atom_type_size_int64					 0x36
	#define	__atom_type_size_double32				 0x37
	#define	__atom_type_size_double64				 0x38
	
	#define	__atom_type_point_int32				0x39
	#define	__atom_type_point_int64				0x3A
	#define	__atom_type_point_double32				0x3B
	#define	__atom_type_point_double64				0x3C
	
	//use type   0x3e80 - 0x3fff
	
	//window type  0x80 - 0x9f
	#define	__atom_type_hwnd					0x80
	#define	__atom_type_hinstance					0x81
	
	//gdi type
	#define	__atom_type_gdi_hdc					0x82
	#define	__atom_type_gdi_object					0x83
	#define	__atom_type_gdi_hfont					0x84
	#define	__atom_type_gdi_hpen					0x85
	#define	__atom_type_gdi_hbitmap				0x86
	#define	__atom_type_gdi_hbrush					0x87
	#define	__atom_type_gdi_hrgn					0x88
	#define	__atom_type_gdi_hpath					0x89
   //general   type
	#define	__atom_int					__atom_type_int
	#define	__atom_string				__atom_type_string
	#define	__atom_double				__atom_type_double
	#define	__atom_bool				__atom_type_bool
	#define	__atom_char				__atom_type_char
	#define	__atom_byte				__atom_type_byte
	#define	__atom_bit					__atom_type_bit
	#define	__atom_void				__atom_type_void
	#define	__atom_pvoid				__atom_type_pvoid
	
	#define	__atom_class				__atom_type_class
	#define	__atom_interface				__atom_type_interface
	#define	__atom_struct				__atom_type_struct
	#define	__atom_enum				__atom_type_enum
	 
	#define	__atom_int_int8				__atom_type_int_int8
	#define	__atom_int_int16				__atom_type_int_int16
	#define	__atom_int_int32				__atom_type_int_int32
	#define	__atom_int_int64				__atom_type_int_int64
	#define	__atom_int_int128				__atom_type_int_int128
	
	#define	__atom_int_uint8				__atom_type_uint_uint8
	#define	__atom_int_uint16				__atom_type_uint_uint16
	#define	__atom_int_uint32				__atom_type_uint_uint32
	#define	__atom_int_uint64				__atom_type_uint_uint64
	#define	__atom_int_uint128			__atom_type_uint_uint128

	#define	__atom_int_int				__atom_type_int
	#define	__atom_int_uint				__atom_type_uint

	#define	__atom_int_short				__atom_type_int_int16
	#define	__atom_int_long				__atom_type_int_int32
	#define	__atom_int_longlong			__atom_type_int_int64
	#define	__atom_int_ushort				__atom_type_uint_uint16
	#define	__atom_int_ulong				__atom_type_uint_uint32
	#define	__atom_int_ulonglong			__atom_type_uint_uint64

	#define	__atom_double_double			__atom_type_double
	#define	__atom_double_float			__atom_type_real
	#define	__atom_double_double80		__atom_type_double80
	#define	__atom_double_double128		__atom_type_double_double128

	#define	__atom_type_category_undef			0x00
	#define	__atom_type_category_int	 		0x01
	#define	__atom_type_category_double	 	0x02
	#define	__atom_type_category_bool	 		0x04
	#define	__atom_type_category_string		 	0x10
	#define	__atom_type_category_class			0x20
	#define	__atom_type_category_pointer			0x40
	#define	__atom_type_category_void			0x80
	#define	__atom_type_category_bit			0x100
	#define	__atom_type_category_date			0x200
	#define	__atom_type_category_ole			0x400
	#define	__atom_type_category_tuple			0x1000
	#define	__atom_type_category_any			0x2000
	#define	__atom_type_category_blob			0x4000
	#define	__atom_type_category_type			0x8000
	#define	__atom_type_category_guid			0x10000
	#define	__atom_type_category_handle			0x20000
	#define	__atom_type_category_array			0x40000
	
	#define	__atom_type_category_number		__atom_type_category_int|__atom_type_category_double
struct  typetoken
 {	tchar* name;
		atom   val;
		int      size;
		dword	typecat;
		__int64  impconv;
		__int64  expconv;
 		__int64  warnconv;
 
	};

/*
pieces
{
    piece $BS($x)  (0x1>>$x)
    piece $BS_INT $BS(#type.int)|$BS(#type.uint)|$BS(#type.aint)
    piece $BS_DOUBLE  $BS(#type.double)|$BS(#type.double80)|$BS(#type.double128)|$BS(#ype.double32)|$BS(#type.float)
    piece $BS_CINT $BS(#type.char)|$BS(#type.bool) 
}
 */
