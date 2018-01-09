#pragma once
#include "../vl.h"
namespace vl
{
	enum
	{
		xa_undef	= 0x0000,	//#undef
		xa_ln		= 0x0001,	//line
		xa_type		= 0x0010,	//#type
		xa_var		= 0x0011,	//#var
		xa_atom		= 0x0012,	//#int
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
		xa_astring	= 0x0049,	//astring

		xa_space		= 0x0080,	//space
		xa_commentln	= 0x0081,	//comment line
		xa_comment		= 0x0082,	//comment
		xa_number		= 0x0083,	//number
		xa_operator		= 0x0084,	//operator
		xa_eol			= 0x0085,	//eol
		xa_blank		= 0x0086,

	};

	#define _parse_state_unaryinc 0x01	//for unaryinc and unarydec

	//cp token
	#define	token_exclam	 	'!'	
	#define	token_quotes	 	'"'	
	#define	token_hash	 		'#'	
	#define	token_dollar	 	'$'	
	#define	token_percent	 	'%'	
	#define	token_ampersand	 	'&'	
	#define	token_apostrophe	'\''	
	#define	token_lparen	 	'('	
	#define	token_rparen	 	')'
	#define	token_lbrace		'{'
	#define	token_rbrace		'}'
	#define	token_lbracket		'['
	#define	token_rbracket		']'
	#define	token_bar			'|'
	#define	token_tlide			'~'
	#define	token_mul			'*'
	#define	token_plus	 		'+'
	#define	token_comma	 		','
	#define	token_minus	 		'-'
	#define	token_dot	 		'.'
	#define	token_period	    '.'
	#define	token_ref	 		'.'
	#define	token_div			'/'
	#define	token_colon	 		':'
	#define	token_semicolon	 	';'
	#define	token_lt			'<'
	#define	token_eq			'='
	#define	token_gt			'>'
	#define	token_query			'?'
	#define	token_sign	        '@'
	#define	token_backslash	    '\\'
	#define	token_caret			'^'
	#define	token_underscore	'_'
	#define	token_reversequote	 '^'

	struct xo
	{
		enum
		{
			opstart		= 0x099,
			add			= 0x0100, //'+'
			sub			= 0x0101, //'-'
			mul			= 0x0102, //'*'
			div			= 0x0103, //'/'
			mod			= 0x0104, //'%'
			power		= 0x0105, //'**'
			shiftright	= 0x0106, //'>>'
			shiftleft	= 0x0107, //'>>'
			bitand		= 0x0108, //'&'
			bitor		= 0x0109, //'|'
			bitxor		= 0x010A, //'^'
			bitnot		= 0x010B, //'~'

			inc			= 0x0110,	//	'++'
			dec			= 0x0111,	//	'--'
			pointer		= 0x0112,	//  ->

			unaryinc	= 0x0114,	//'++'
			unarydec	= 0x0115,	//'--'
			unaryval	= 0x0116,
			unaryref	= 0x0117,
			unaryadd	= 0x0118,
			unarysub	= 0x0119,
			unarynot	= 0x011A,
			unarybitnot = 0x011B,

			assign 					= 0x0140,	//	'='
			addassign				= 0x0120,	//	'+='
			subassign				= 0x0121,	//	'-='
			mulassign				= 0x0122,	//	'*='
			divassign				= 0x0123,	//	'/='
			modassign				= 0x0124,	//	'%='
			powerassign				= 0x0125,	//	'**='
			shiftrightassign		= 0x0126,	//	'>>='
			shiftleftassign			= 0x0127,	//	'<<='
			bitandassign			= 0x0128,	//	'&='
			bitorassign				= 0x0129,	//	'|='
			bitxorassign			= 0x012A,	//	'^='
			bitnotassign			= 0x012B,	//	'~='

			eq			= 0x0130,	//	'=='
			gtr			= 0x0132,	//	'>'
			ltr			= 0x0133,	//	'<'
			geq			= 0x0134,	//	'>='
			leq			= 0x0135,	//	'<='
			neq			= 0x0136,	//	'!='

			or			= 0x0142,	//	||
			and			= 0x0143,	//	&&
			not			= 0x0144,	//	!

			nsub		= 0x0145,	//	-
			dim			= 0x0150,	//[]
			
			opsizeof	= 0x0160,
			opnew		= 0x0161,
			opdelete    = 0x0162,
			pointerm	= 0x0163,	//成员指针
			refm		= 0x0164,	//成员引用
			opcast		= 0x0165,	//转化
			optypeid	= 0x0166,
			opdynamic_cast		= 0x0167,
			opstatic_cast		= 0x0168,
			opreinterpret_cast	= 0x0169,
			opconst_cast		= 0x016A,
			optypename			= 0x016B,
			opoperator			= 0x017C,
			opliteral			= 0x017D,
			opthis				= 0x017E,
			opid				= 0x017F,
			opitem				= 0x0180,
			opcaset				= 0x0181,
			optypeof			= 0x0182,

			colon		= 0x0501,	//:
			becomes		= 0x0502,	//:=
			period		= 0x0503,	//.
			nest		= 0x0504,	//::
			dot3		= 0x0505,	//...
			hash		= 0x0506,   //#

			comma		= 0x601,	//,
			slash		= 0x602,	//
			tilde		= 0x603,	//~
			query		= 0x604,	//?
			sign		= 0x605,	//@
			exclam		= not,		//!
			opend		= 0x800
		};
	};

	#define AssignmentOperatorL	xo::addassign
	#define AssignmentOperatorH	xo::bitnotassign
	
	#define OperatorPrimary(o) (o==xo::opoperator||o==xo::opliteral||o==xo::opthis \
				||o==xo::opid||o==xo::opitem)
	#define OperatorPostfix(o) (o==xo::optypename || o==xo::opdynamic_cast || o==xo::opstatic_cast || \
				o==xo::opreinterpret_cast || o==xo::opconst_cast || o==xo::optypeid)
	#define OperatorUnary(o)  (o==xo::unaryinc || o==xo::unarydec || o==xo::unaryval || \
			o==xo::unaryref || o==xo::unaryadd || o==xo::unarysub ||o==xo::unarynot ||o==xo::unarybitnot || \
			o==xo::opsizeof || o==xo::opnew || o==xo::opdelete)
	#define OperatorCast(o) (o==xo::opcast)
	#define OperatorPm(o) (o==xo::refm || o==xo::pointerm)
	#define OperatorMultiplicative(o) (o==xo::mul || o==xo::div)
	#define OperatorAdditive(o) (o==xo::add || o==xo::sub)
	#define OperatorShift(o) (o==xo::shiftleft || o==xo::shiftright)
	#define OperatorRelational(o) (o==xo::gtr || o==xo::ltr || o==xo::geq || o==xo::leq )
	#define OperatorEquality(o) (o==xo::eq || o==xo::neq )
	#define OperatorBit(o) (o==xo::bitand || o==xo::bitor || o==xo::bitxor)
	#define OperatorLogical(o) (o==xo::and || o==xo::or )
	#define OperatorConditional(o) (o==xo::query)
	#define OperatorAssignment(o) (o== xo::assign||(o>= AssignmentOperatorL && o<= AssignmentOperatorH))

	#define OperatorLevel(o) (  OperatorAssignment(o)?		1 : \
								OperatorConditional(o)?		2 : \
								OperatorLogical(o)?			3 : \
								OperatorBit(o)?				4 : \
								OperatorEquality(o)?		5 : \
								OperatorRelational(o)?		6 : \
								OperatorShift(o)?			7 : \
								OperatorAdditive(o)?		8 : \
								OperatorMultiplicative(o)?	9 : 10 )

	#define OperatorLevelPm			10
	#define OperatorLevelCast		11
	#define OperatorLevelUnary		12
	#define OperatorLevelPostfix	13
	#define OperatorLevelPrimary	14
	#define OperatorLevelUnvalid	15

	#define CompOperator(x,y) (OperatorLevel(x) - OperatorLevel(y))

	#define OperatorPmLevel(o) (OperatorPm(o)?		OperatorLevelPm : \
								OperatorCast(o)?	OperatorLevelCast : \
								OperatorUnary(o)?	OperatorLevelUnary : \
								OperatorPostfix(o)?	OperatorLevelPostfix : \
								OperatorPrimary(o)?	OperatorLevelPrimary : OperatorLevelUnvalid)

	#define TranOpUnary(o,s,cp) (o==xo::inc		? xo::unaryinc : \
							o==xo::dec		? xo::unarydec : \
							o==xo::mul		? xo::unaryval : \
							o==xo::bitand	? xo::unaryref : \
							o==xo::add		? xo::unaryadd : \
							o==xo::sub		? xo::unarysub : \
							o==xo::not		? xo::unarynot : \
							o==xo::bitnot	? xo::unarybitnot : \
							cp(s,L"sizeof")==0	? xo::opsizeof : \
							cp(s,L"new")==0? xo::opnew : \
							cp(s,L"delete")==0	? xo::opdelete : 0)

	#define TranOpPostfix(o,s,cp) (o!=xa_indent				? 0 : \
								cp(s,L"typename")==0		? xo::optypename : \
								cp(s,L"dynamic_cast")==0	? xo::opdynamic_cast : \
								cp(s,L"static_cast")==0		? xo::opstatic_cast : \
								cp(s,L"reinterpret_cast")==0? xo::opreinterpret_cast : \
								cp(s,L"const_cast")==0		? xo::opconst_cast : \
								cp(s,L"typeid")==0			? xo::optypeid : 0)
	#define TranOpPrimary(o,s,cp) (o==xa_indent && cp(s,L"this")==0	? xo::opthis: \
							   o==xa_indent && cp(s,L"operator")==0	? xo::opoperator: \
							   o==xa_int||o==xa_float||o==xa_double || o==xa_bool || o==xa_char || \
							   o==xa_string || o==xa_uint || o==xa_tuple ? xo::opliteral : \
							   o==xa_indent ? xo::opid : 0)
	
	#define TranPm(o,s,cp) (TranOpUnary(o,s,cp)!= 0? TranOpUnary(o,s,cp) : TranOpPostfix(o,s,cp)!=0?  TranOpPostfix(o,s,cp) : TranOpPrimary(o,s,cp))

	inline const tchar * getxostring(int op)
	{
		switch(op)
		{
			case xo::add:
				return 			L"+";
			case xo::sub:
				return 			L"-";
			case xo::mul:
				return 			L"*";
			case xo::div:
				return 			L"/";
			case xo::mod:
				return 			L"%";
			case xo::power:
				return 			L"**";
			case xo::shiftright:
				return			L">>";
			case xo::shiftleft:
				return 			L"<<";
			case xo::bitand:
				return 			L"&";
			case xo::bitor:
				return 			L"|";
			case xo::bitxor:
				return 			L"^";
			case xo::bitnot:
				return 			L"~";
			case xo::inc:
				return 			L"++";
			case xo::dec:
				return 			L"--";
			case xo::pointer:
				return 			L"->";

			case xo::assign:
				return  		L"=";
			case xo::addassign:
				return 			L"+=";
			case xo::subassign:
				return 			L"-=";
			case xo::mulassign:
				return 			L"*=";
			case xo::divassign:
				return 			L"/=";
			case xo::modassign:
				return 			L"%=";
			case xo::powerassign:
				return 			L"**=";
			case xo::shiftrightassign:
				return 			L">>=";
			case xo::shiftleftassign:
				return 			L"<<=";
			case xo::bitandassign:
				return 			L"&=";
			case xo::bitorassign: 
				return 			L"|=";
			case xo::bitxorassign: 
				return 			L"^=";
			case xo::bitnotassign: 
				return 			L"~=";

			case xo::eq: 
				return 			L"==";
			case xo::gtr: 
				return 			L">";
			case xo::ltr: 
				return 			L"<";
			case xo::geq: 
				return 			L">=";
			case xo::leq: 
				return 			L"<=";
			case xo::neq: 
				return 			L"!=";

			case xo::or: 
				return 			L"||";
			case xo::and: 
				return 			L"&&";
			case xo::not: 
				return 			L"!";

			case xo::nsub: 
				return 			L"-";
			case xo::dim: 
				return 			L"[]";
			
			case xo::colon: 
				return 			L":";
			case xo::becomes: 
				return 			L":=";
			case xo::period: 
				return 			L".";
			case xo::nest: 
				return 			L"::";
			case xo::dot3: 
				return 			L"...";
			case xo::hash: 
				return 			L"#";

			case xo::comma: 
				return 			L",";
			case xo::slash: 
				return 			L"\\";
			case xo::tilde: 
				return 			L"~";
			case xo::query: 
				return 			L"?";
			case xo::sign: 
				return 			L"@";
			default:
				return			null;
		}
	}
}
