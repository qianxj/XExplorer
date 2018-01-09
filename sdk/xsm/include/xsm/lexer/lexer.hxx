	#pragma once
	
	#include "windows.h"
	#include "tchar.h"
	#include <vector>
	#include 	"keyword.hxx"
   //token
	#define	__atom_cp_token_none		        0
	
	//#cp syn
	#define	__atom_cp_syn_class	0x1024           //class
	#define	__atom_cp_syn_interface	0x1025	//interface
	#define	__atom_cp_syn_struct	0x1026		//struct
	#define	__atom_cp_syn_enum	0x1027		//
	
	//cp token
	#define	__atom_cp_token_exclam	 	'!'	
	#define	__atom_cp_token_quotes	 	'"'	
	#define	__atom_cp_token_hash	 		'#'	
	#define	__atom_cp_token_dollar	 		'$'	
	#define	__atom_cp_token_percent	 	'%'	
	#define	__atom_cp_token_ampersand	 	'&'	
	#define	__atom_cp_token_apostrophe	 	'\''	
	#define	__atom_cp_token_lparen	 	'('	
	#define	__atom_cp_token_rparen	 	')'
	#define	__atom_cp_token_lbrace			'{'
	#define	__atom_cp_token_rbrace			'}'
	#define	__atom_cp_token_lbracket		'['
	#define	__atom_cp_token_rbracket		']'
	#define	__atom_cp_token_bar			'|'
	#define	__atom_cp_token_tlide			'~'
	#define	__atom_cp_token_mul			'*'
	#define	__atom_cp_token_plus	 		'+'
	#define	__atom_cp_token_comma	 	','
	#define	__atom_cp_token_minus	 	'-'
	#define	__atom_cp_token_dot	 		'.'
	#define	__atom_cp_token_period	         '.'
	#define	__atom_cp_token_ref	 		'.'
	#define	__atom_cp_token_div			'/'
	#define	__atom_cp_token_colon	 		':'
	#define	__atom_cp_token_semicolon	 	';'
	#define	__atom_cp_token_lt			'<'
	#define	__atom_cp_token_eq			'='
	#define	__atom_cp_token_gt			'>'
	#define	__atom_cp_token_query			'?'
	#define	__atom_cp_token_sign	            '@'
	#define	__atom_cp_token_backslash	    '\\'
	#define	__atom_cp_token_caret			'^'
	#define	__atom_cp_token_underscore		'_'
	#define	__atom_cp_token_reversequote	 '^'
	 
	#define	__atom_cp_token_becomes		0XA8 	//':='  or '..'
	#define	__atom_cp_token_nest			0XA9		//::
	#define	__atom_cp_token_dot3			0XAA		//...
	#define	__atom_cp_token_pointer		0XAB		//->
	
	#define	__atom_cp_token_digits	0x1028		// [0-9]
	#define	__atom_cp_token_letter	0x1029		//[a-zA-Z]
	#define	__atom_cp_token_uletter	0x102a	//[A-Z]
	#define	__atom_cp_token_lletter	0x102b         //[a-z]

	#define	__atom_cp_token_eof	0x102c	        //process finished
	#define	__atom_cp_token_undef	0x102d	        //lexer initial
	#define	__atom_cp_token_blank	0x102e          //blank string
	#define	__atom_cp_token_ln	0x102f               //ln char
	#define	__atom_cp_token_comment	0x1030     // /**/ style comment
	#define	__atom_cp_token_commentln	0x1031   // line style comment
	
	#define	__atom_cp_token_indent	0x1032		//indent
	#define	__atom_cp_token_bool	0x1033		//bool
	#define	__atom_cp_token_int	0x1034		//int
	#define	__atom_cp_token_double	0x1035	//double
	#define	__atom_cp_token_string	0x1036		//string
	#define	__atom_cp_token_char	0x1037            //char

	//cp state
	#define	__atom_cp_state_blank			0x0001    //read blank token
	#define	__atom_cp_state_ln			0x0002    //read line token
	#define	__atom_cp_state_dotindent	     0x0004    //indent can include dot char
	#define	__atom_cp_state_comment		0x0008    //read comment token
	#define	__atom_cp_state_escape		0x0010    //escape string 
     //error
 	 #define	__atom_cp_error_scannumber	0x1024          //number scannumber is zero
	 #define	__atom_cp_error_doublescale	0x1025          //number scale not isvalid 
	 #define	__atom_cp_error_comment_trial	0x1026      //need */ but meet eof
	 #define	__atom_cp_error_string_trial	0x1027           //string or char unfinished but meet eof
     //operator
 	 //operator
	#define	__atom_cp_op_mul			'*'	//*	
	#define	__atom_cp_op_div			'/'	///	
	#define	__atom_cp_op_mod		'%'	//%	
 	#define	__atom_cp_op_add			'+'	//+
	#define	__atom_cp_op_sub			'-'	//-
	#define	__atom_cp_op_ltr			'<'	//ltr
	#define	__atom_cp_op_gtr			'>'	//gtr
	#define	__atom_cp_op_bitand		'&'	//&
	#define	__atom_cp_op_bitxor		'^'	//^
	#define	__atom_cp_op_bitor	        '|'	//|
	#define	__atom_cp_op_query		'?'	//?:
	
	#define	__atom_cp_op_leq					0x80	//leq   <=
	#define	__atom_cp_op_geq					0x81	//geq  >=
	#define	__atom_cp_op_eq					0x82	//==
	#define	__atom_cp_op_neq					0x83	//!=
	#define	__atom_cp_op_and					0x84	//&&
	#define	__atom_cp_op_or					0x85	//||
 	#define	__atom_cp_op_shiftleft				0x86	//<<
	#define	__atom_cp_op_shiftright				0x87	//>>

	
	 #define	__atom_cp_op_assign	      		0x90	//=
	 #define	__atom_cp_op_assign_mul	     		0x91 	//*=
	 #define	__atom_cp_op_assign_div			0x92	///=
	 #define	__atom_cp_op_assign_mod			0x93	//%=
	 #define	__atom_cp_op_assign_add			0x94	//+=
	 #define	__atom_cp_op_assign_sub			0x95	//-=
	 #define	__atom_cp_op_assign_shiftleft	 	0x96	//<<=
	 #define	__atom_cp_op_assign_shiftright	 	0x97 //>>=
	 #define	__atom_cp_op_assign_bitand		   	0x98	//&=
	 #define	__atom_cp_op_assign_bitxor		   	0x99	//^=
	 #define	__atom_cp_op_assign_bitor		   	0x9A	//|=
	 #define	__atom_cp_op_assign_bitnot		   	0x9B	//~=
	 
	//unary 
	#define	__atom_cp_op_unary_value			0xA0	//*
	#define	__atom_cp_op_unary_ref				0xA1	//&
	#define	__atom_cp_op_unary_bitnot			0xA2	//~
	#define	__atom_cp_op_unary_not				0xA3	//!
	#define	__atom_cp_op_unary_neg			0xA4	//-
	#define	__atom_cp_op_unary_plus			0xA5	//+
	
	#define	__atom_cp_op_inc					0xB0	//++
	#define	__atom_cp_op_dec					0xB1	//--
	#define	__atom_cp_op_val					0xB2	//val
	#define	__atom_cp_op_new				0xB3	//val
	#define	__atom_cp_op_delete				0xB4	//val
	
	#define	__atom_cp_op_pcall				0xB5	//function pointer call
	#define	__atom_cp_op_element				0xB6	//a[i,j]
	#define	__atom_cp_op_ref					0xB7	//a.b
	#define	__atom_cp_op_cast				0xB8	//cast
 	#define	__atom_cp_op_typeof				0xB9	//type of
	#define	__atom_cp_op_sizeof				0xBA	//sizeof of
 	#define	__atom_cp_op_offset				0xBB	//offset of
	#define	__atom_cp_op_debugbreak			0xBC	//debug break;
	#define	__atom_cp_op_ret					0xBD	//ret;
 struct XSM_API lexertoken
 {	int cc;	//position of code source
	 	tchar ch;  	//next char
	 
	 	atom sy;   //token type
	 
	 	int pcc;
	 	int slen;
	 	union    //value of current token
	{
		double	rnum;    //double valude
		int		inum;     //int value
	 };
	 	tchar 	snum[4096];   //string value
	 	int  sindex;		   //string sindex  
	 
	 	int row;    //pointe to code source
	 	int col;     //point to code column
	 
	 	int prow;    //pointe to code source
	 	int pcol;     //point to code column
	 	int plen;
	 
	 
	 lexertoken();
	 };

/* keyword:
	typedef-name:
	 * identifier
	 * namespace-name:
	 * original-namespace-name
	 * namespace-alias
	 * original-namespace-name:
	 * identifier
	 * namespace-alias:
	 * identifier
	 * class-name:
	 * identifier
	 * template-id
	 * enum-name:
	 * identifier
	 * template-name:
	 * identifier
 */
 
/*  Lexical conventions:
	 *  hex-quad:
	 * hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit
	 * universal-character-name:
	 * \u hex-quad
	 * \U hex-quad hex-quad
	 *  preprocessing-token:
	 * header-name
	 * identifier
	 * pp-number
	 * character-literal
	 * string-literal
	 * preprocessing-op-or-punc
	 * each non-white-space character that cannot be one of the above
	 * token:
	 * identifier
	 * keyword
	 * literal
	 * operator
	 * punctuator
	 * header-name:
	 * <h-char-sequence>
	 * "q-char-sequence"
	 * h-char-sequence:
	 * h-char
	 * h-char-sequence h-char
	 * h-char:
	 * any member of the source character set except
	 * new-line and >
	 * q-char-sequence:
	 * q-char
	 * q-char-sequence q-char
	 * q-char:
	 * any member of the source character set except
	 * new-line and "
	 * pp-number:
	 * digit
	 * . digit
	 * pp-number digit
	 * pp-number nondigit
	 * pp-number e sign
	 * pp-number E sign
	 * pp-number .
	 * identifier:
	 * nondigit
	 * identifier nondigit
	 * identifier digit
	 * nondigit: one of
	 * universal-character-name
	 * _ a b c d e f g h i j k l m
	 * n o p q r s t u v w x y z
	 * A B C D E F G H I J K L M
	 * N O P Q R S T U V W X Y Z
	 * digit: one of
	 * 0 1 2 3 4 5 6 7 8 9
	 * preprocessing-op-or-punc: one of
	 * { } [ ] # ## ( )
	 * <: :> <% %> %: %:%: ; : ...
	 * new delete ? :: . .*
	 * + - * / % ? & | ?
	 * ! = < > += -= *= /= %=
	 * ?= &= |= << >> >>= <<= == !=
	 * <= >= && || ++ -- , ->* ->
	 * and and_eq bitand bitor compl not not_eq
	 * or or_eq xor xor_eq
	 * literal:
	 * integer-literal
	 * character-literal
	 * floating-literal
	 * string-literal
	 * boolean-literal
	 * integer-literal:
	 * decimal-literal integer-suffixopt
	 * octal-literal integer-suffixopt
	 * hexadecimal-literal integer-suffixopt
	 * decimal-literal:
	 * nonzero-digit
	 * decimal-literal digit
	 * octal-literal:
	 * 0
	 * octal-literal octal-digit
	 * hexadecimal-literal:
	 * 0x hexadecimal-digit
	 * 0X hexadecimal-digit
	 * hexadecimal-literal hexadecimal-digit
	 * nonzero-digit: one of
	 * 1 2 3 4 5 6 7 8 9
	 * octal-digit: one of
	 * 0 1 2 3 4 5 6 7
	 * hexadecimal-digit: one of
	 * 0 1 2 3 4 5 6 7 8 9
	 * a b c d e f
	 * A B C D E F
	 * integer-suffix:
	 * unsigned-suffix long-suffixopt
	 * long-suffix unsigned-suffixopt
	 * unsigned-suffix: one of
	 * u U
	 * long-suffix: one of
	 * l L
	 * character-literal:
	 * ¡¯c-char-sequence¡¯
	 * L¡¯c-char-sequence¡¯
	 * c-char-sequence:
	 * c-char
	 * c-char-sequence c-char
	 * c-char:
	 * any member of the source character set except
	 * the single-quote ¡¯, backslash \, or new-line character
	 * escape-sequence
	 * universal-character-name
	 * escape-sequence:
	 * simple-escape-sequence
	 * octal-escape-sequence
	 * hexadecimal-escape-sequence
	 * simple-escape-sequence: one of
	 * \¡¯ \" \? \\
	 * \a \b \f \n \r \t \v
	 * octal-escape-sequence:
	 * \ octal-digit
	 * \ octal-digit octal-digit
	 * \ octal-digit octal-digit octal-digit
	 * hexadecimal-escape-sequence:
	 * \x hexadecimal-digit
	 * hexadecimal-escape-sequence hexadecimal-digit
	 * floating-literal:
	 * fractional-constant exponent-partopt floating-suffixopt
	 * digit-sequence exponent-part floating-suffixopt
	 * fractional-constant:
	 * digit-sequenceopt . digit-sequence
	 * digit-sequence .
	 * exponent-part:
	 * e signopt digit-sequence
	 * E signopt digit-sequence
	 * sign: one of
	 * + -
	 * digit-sequence:
	 * digit
	 * digit-sequence digit
	 * floating-suffix: one of
	 * f l F L
	 * string-literal:
	 * "s-char-sequenceopt"
	 * L"s-char-sequenceopt"
	 * s-char-sequence:
	 * s-char
	 * s-char-sequence s-char
	 * s-char:
	 * any member of the source character set except
	 * the double-quote ", backslash \, or new-line character
	 * escape-sequence
	 * universal-character-name
	 * boolean-literal:
	 * false
	 * true
 */

 class XSM_API lexer
 {
	 
public:
	 	tchar*  source;      //code souce
	 	int	len;             //code source len;
	
	 	tchar*  url;      //code souce

	//current token
	 	lexertoken token;  

	//lexer  state   see #cp.state.*
         	dword	lexerstate;

		int startIndex;
	 
	 //construct
	 
	 lexer();
	 ~lexer();
	 int error(dword errcode);
	 int setsource(tchar* psrc,tchar* purl);
	 int insymbol(dword state = 0);
	 void skipsym(int n, dword state = 0);
	 tchar nextch();
	 tchar getchar(int pos);std::vector<lexertoken> tokenstack	;

	 //save state
	 
	 int save();
	 int restore();
	 int push();
	 int push(lexertoken & tk);
	 int pop();
	 lexertoken & item(int index);
	 int find(atom tk);};  