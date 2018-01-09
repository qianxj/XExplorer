#pragma once

#include "windows.h"
#include "tchar.h"

#define ARRAYINC 255
#define ARRAYMANINC 255
#define STRINC 255
#define REALCONSTINIT 8192
#define SCCONSTINIT	32768
#define CODESINIT 8192
#define TABLESINIT 8192
#define STACKMAX 4096//32768
#define DEFSTRSIZE 255

#include "GridFrameSvr.hpp"
#include "DwTable.hpp"
#include "IDwTable.hpp"
using namespace Hxsoft::XFrame::XOffice::XCell;

class CHxComplier
{
public:
	enum    keyw{undef,plus,minus,intcon,realcon,charcon,indent,arraysy,
			recordsy,constsy,typesy,varsy,procsy,funcsy,
			lparent,notsy,strcon,beginsy,ifsy,whilesy,repeatsy,forsy,casesy,notyp,ints,reals,chars,strs,bools,arrays,recorders,
			stringcon,
			times,idiv,imod,rdiv,
			andsy,orsy,eql,neq,gtr,lss,leq,geq,
			rparent,lbrack,rbrack,comma,semicolon,period,colon,becomes,
			programsy,
			endsy,elsesy,untilsy,ofsy,dosy,tosy,downtosy,thensy,
			functionsy,divsy,modsy,proceduresy,konstant,vvariable,typel,procedure,function,cnv,fct};

protected:
	#define CODECOUNT 75
	enum CODE{LDA,LOD,LDI,DIS, RV1  , RV2  ,RV3   , RV4  ,FCT,CINT,JMP,JPC,SWT,CAS,
				 F1U,F2U,F1D,F2D,MKS,CCAL,IDX,IXX,LDB,CPB,LDC,LDR,FLT,RED,
				 WRS,WRW,WRU,HLT,EXP,EXF,LDT,NOT,MUS,WRR,STO,EQR,NER,LSR,
				 LER,GTR,GER,EQL,NEQ,LSS,LEQ,GRT,GEQ,ORR,ADD,SUB,ADR,SUR,
				 AND,MUL,DIV,MOD,MUR,DIR,RDL,WRL,EQC,NEC,LSC,LEC,GRC,GEC,
				 LDS,ADC,FLC,VAL,ROW};

	static LPTSTR mne[];

	static keyw	constbegsys[] ;
	static keyw	typebegsys[] ;
	static keyw	blockbegsys[];
	static keyw	facbegsys[] ;
	static keyw	statbegsys[];
	static keyw	stantyps[] ;
	static keyw  symbol[];
	static LPTSTR key[];
	static keyw  ksy[];
	static keyw objs[];
	


	keyw sps[255];


	//int CHxComplier::types[]={notyp,ints,reals,bools,chars,arrays,records};
	//int	CHxComplier::typsy[]={notyp,ints,reals,chars,bools,array,record};
	static keyw	codesy[];

	struct item
	{
		keyw		typ;
		int		refe;
	};

	struct tagtab
	{
		LPTSTR		name;
		short		link;
		keyw		obj;
		keyw		typ;
		short		refe;
		bool		normal;
		short		lev;
		short		adr;
	};

		struct order
	{
		int		f;
		int		x;
		int		y;
	};

	order* codes;
	int codesln;

public:
	struct var
	{
		union
		{
			int i;
			bool b;
			TCHAR ch;
		};
		float f;
		LPTSTR m_pStr;
		int StrSize;
		keyw sv_type;
		bool bStrNeedDeleted;

		float toFloat()
		{
			if(sv_type==ints) f=(float)i;
			else if( sv_type == chars) f = ch;
			else if(sv_type==bools) f = b;
			else if(sv_type==strs)
			{
				if(m_pStr)
				{
					i  = ::_tstoi(m_pStr);
					if(bStrNeedDeleted)	delete m_pStr;
				}
				else 
					i=0;
			}
			this->sv_type=reals;
			return f;
		}
		int toInt()
		{
			if(sv_type==reals) i = (int)f;
			else if(sv_type==strs)
			{
				if(m_pStr)
				{
					i  = ::_tstoi(m_pStr);
					if(bStrNeedDeleted) delete m_pStr;
				}
				else 
					i=0;
			}
			this->sv_type=ints;
			return i;
		}
		int toChar()
		{
			this->sv_type=chars;
			this->ch = toInt()  % 65536;
			return this->ch;
		}

		LPTSTR toString()
		{
			if(sv_type==strs)return this->m_pStr;

			m_pStr =  new TCHAR[DEFSTRSIZE];
			StrSize = DEFSTRSIZE;
			bStrNeedDeleted = true;

			if(sv_type==reals)
			{
				::_stprintf_s(m_pStr,60,_T("%f"),f);
			}
			else if(sv_type==ints)
			{
				::_stprintf_s(m_pStr,60,_T("%i"),i);
			}
			else if(sv_type==chars)
			{
				::_stprintf_s(m_pStr,60,_T("%c"),ch);
			}
			else if(sv_type==bools)
			{
				if(b)
					::_tcscpy_s(m_pStr,60,_T("true"));
				else
					::_tcscpy_s(m_pStr,60,_T("false"));
			}
			this->sv_type=strs;
			return m_pStr;
		}	
	};

public:
	CHxComplier();           
	virtual ~CHxComplier();

	class CGridFrameSvr*	m_pGrid;
	int FRow;
	int FCol;

protected:
	LPTSTR	source;
	int		sourcelen;

	int 	cc;
	TCHAR	ch;
	int 	irow;

	keyw	sy;
	float	rnum;
	int	inum;
	TCHAR	snum[256];
	int	isnum;
	LPTSTR	snumex;

	//字符串常数
	LPTSTR psconst;
	int isconstpos;
	LPTSTR*	sconst;
	int	sc1,sc2;

	//real const
	float*	rconst;
	int	c2,c1;

	tagtab*	tab;
	int tabln;
	int tablni;

public:
	// 设置源代码
	int SetSource(LPTSTR szSource);
public:
	//get next char
	int nextch(void);
public:
	//read float scale
	int readscale(void);
public:
	//error information
	int error(LPTSTR);
public:
	//adjust real scale
	int adjustscale(int e);
	int adjustscale(float &rnum,int e);
public:
	// word symbol
	int insymbol(void);
public:
	int expression(keyw* & fsys, item & x);
	int simplesexpression(keyw* & fsys, item & x);
	int term(keyw* & fsys, item & x);
	int factor(keyw* & fsys, item & x);
	int standfct(keyw* & fsys, int n, int i,item &x);
public:
	int in(keyw*  fsys, int count, keyw val);
public:
	int emit(int fct);
public:
	int emit1(int fct, int b);
public:
	int emit2(int fct, int a, int b);
public:
	keyw resulttype(keyw l1, keyw l2);
public:
	int loc(LPTSTR szName);
public:
	int enterstr(LPTSTR x);
public:
	int enterreal(float x);
public:
	int construct(void);
public:
	int enter(LPTSTR x0, keyw x1, keyw x2, short x3);
public:
	int of_calcexpression(LPTSTR as_source,var &val);
public:
	int interpret(var & value);
public:
	// parse excell cell name
	bool ParseCellName(LPTSTR szName, int & col, int & row, bool & acol, bool & arow);
public:
	bool isNumber(LPTSTR x0);
	bool isNumber(LPTSTR x0,keyw &typ,int & inum,float &rnum);
public:
	keyw GetCellType(LPTSTR szCell);
public:
	// 得到关联的单元格
	int GetRCells(LPTSTR szFormula, RECT pRect[] ,DWORD pAbs[]);
	int GetRCells(LPTSTR szFormula, RECT pRect[] ,DWORD pAbs[],int sIndex);
public:
	bool GetRCellFormula(LPTSTR  pFormula, int ACol , int ARow , LPTSTR  & pRFormula);
public:
	int getdsindex (LPTSTR as_name, int &al_index, int &al_colid, LPTSTR &as_type);
};
