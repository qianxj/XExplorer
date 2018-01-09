#pragma once

#include "xvmcore.h"

namespace xcode
{
	bool f_sym(xvmcore &v,int op,int ext=1);
	bool f_append(xvmcore &v,int op,int ext=1);
	bool f_assignment(xvmcore &v,int op,int ext=1);
	bool f_op(xvmcore &v,int op,int ext=1);
	bool f_opcalc(xvmcore &v,int op,int ext=1);
	bool f_opbit(xvmcore &v,int op,int ext=1);
	bool f_oplogic(xvmcore &v,int op,int ext=1);
	bool f_opcompare(xvmcore &v,int op,int ext=1);
	bool f_opincordec(xvmcore &v,int op,int ext=1);
	bool f_opdim(xvmcore &v,int op,int cnt=1);
	bool f_opref(xvmcore &v,lptstr indent);
	bool f_optuple(xvmcore &v,xtuple * dest,xtuple * source); 
	bool f_optuple(xvmcore &v,xtuple * dest,xtuple * source,int op,int ext=1); 
	bool f_optupletoarray(xvmcore &v,xcolinfo &info ,xardata ** ppardata,xtuple * source,int op,int ext=1); 
	bool f_opstdfct(xvmcore &v,tchar * pfct,int nargs);

	namespace xo
	{
		enum
		{
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

			adds		= 0x0110,	//	'++'
			subs		= 0x0111,	//	'--'
			clons		= 0x0012,	//	'::'
			pointer		= 0x0013,	//	'->'
			_ref		= 0x0014,	//	#ref
			_val		= 0x0015,	//	#val

			addeq		= 0x0120,	//	'+='
			subeq		= 0x0121,	//	'-='
			muleq		= 0x0122,	//	'*='
			diveq		= 0x0123,	//	'/='
			modeq		= 0x0124,	//	'%='
			powereq		= 0x0125,	//	'**='
			shiftrighteq	= 0x0126,	//	'>>='
			shiftlefteq	= 0x0127,	//	'<<='
			bitandeq	= 0x0128,	//	'&='
			bitoreq		= 0x0129,	//	'|='
			bitxoreq	= 0x012A,	//	'^='
			bitnoteq	= 0x012B,	//	'~='

			equ			= 0x0130,	//	'=='
			gtr			= 0x0132,	//	'>'
			lss			= 0x0133,	//	'<'
			geq			= 0x0134,	//	'>='
			leq			= 0x0135,	//	'<='
			neq			= 0x0136,	//	'!='

			eq			= 0x0140,	//	'='
			_copyof		= 0x0141,	//	#copyof 
			or			= 0x0142,	//	||
			and			= 0x0143,	//	&&
			not			= 0x0144,	//	!

			nsub		= 0x0145,	//	-

			dim			= 0x0150,	//[]

			_unoin		= 0x0180,	//	#unoin
			_inter		= 0x0181,	//	#inter
			_in			= 0x0182,	//	#in
			_heade		= 0x0183,	//	#heade
			_trail		= 0x0184,	//	#trail
			_first		= 0x0185,	//	#first
			_last		= 0x0186,	//	#last 
			_piror		= 0x0187,	//	#piror
			_next		= 0x0188,	//	#next
			_flat		= 0x0189,	//	#flat
			_empty		= 0x018A,	//	#empty
			_pos		= 0x018B,	//	#pos
			_oneof		= 0x018C,	//	#oneof
			_revert		= 0x018D,	//	#revert
			_split		= 0x018E,	//	#split
			_merge		= 0x018F,	//	#merge

			_sum		= 0x0200,	//	#sum
			_avg		= 0x0201,	//	#avg
			_count		= 0x0202,	//	#count

			_instanceof	= 0x0210,	//	#instanceof 
			_typeof		= 0x0211,	//	#typeof
			_valueof	= 0x0212,	//	#valueof

			_matchruler	= 0x0240,	//	#matchruler
			_matchstart	= 0x0241,	//	#matchstart

			_declarevar	= 0x0251,	//	#declarevar
			_declaretype = 0x0252,	//	#declaretype
			_declarestmt = 0x0253,	//	#declarestmt

			_exec		= 0x0300,	//	#exec
			_append		= 0x0301,	//	#append
			_erase		= 0x0302,	//	#erase
			_new		= 0x0303,	//	#new

			_sym		= 0x0304,	//	#sym
			
			colon		= 0x0501,	//:
			becomes		= 0x0502,	//:=
			period		= 0x0503,	//.
			nest		= 0x0504,	//::
			dot3		= 0x0505,	//...
			hash		= 0x0506    //#
		};
	}
}
