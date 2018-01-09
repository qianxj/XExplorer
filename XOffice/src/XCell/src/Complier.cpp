#include "StdAfx.h"
#include "Complier.h"
#include "math.h"
#include <strsafe.h>
#include "stringgrid.h"

LPTSTR	constbegsys[]={_T("plus"),_T("minus"),_T("intcon"),_T("realcon"),_T("charcon"),_T("indent")};
LPTSTR	typebegsys[] ={_T("indent"),_T("arraysy"),_T("recordsy")};
LPTSTR	blockbegsys[]={_T("constsy"),_T("typesy"),_T("varsy"),_T("procsy"),_T("funcsy"),_T("beginsy")};
LPTSTR	facbegsys[]={_T("intcon"),_T("realcon"),_T("charcon"),_T("indent"),_T("lparent"),_T("notsy"),_T("strcon")};
LPTSTR	statbegsys[]={_T("beginsy"),_T("ifsy"),_T("whilesy"),_T("repeatsy"),_T("forsy"),_T("casesy")};
LPTSTR	stantyps[] ={_T("notyp"),_T("ints"),_T("reals"),_T("chars"),_T("strs")};

LPTSTR symbol[]={_T("intcon"),_T("realcon"),_T("charcon"),_T("stringcon"),_T("notsy"),
		_T("plus"),_T("minus"),_T("times"),_T("idiv"),_T("idmod"),
		_T("andsy"),_T("orsy"),_T("eql"),_T("neq"),_T("gtr"),_T("lss"),_T("leq"),
		_T("lparent"),_T("rparent"),_T("lbrack"),_T("rbrack"),_T("comma"),_T("semicolon"),_T("period"),_T("colon"),_T("becomes"),
		_T("constsy"),_T("typesy"),_T("varsy"),_T("funcsy"),
		_T("procsy"),_T("arraysy"),_T("recordsy"),_T("programsy"),_T("ident"),
		_T("beginsy"),_T("ifsy"),_T("casesy"),_T("repeatsy"),_T("whilesy"),_T("forsy"),
		_T("endsy"),_T("elsesy"),_T("untilsy"),_T("ofsy"),_T("dosy"),_T("tosy"),_T("downtosy"),_T("thensy")};

LPTSTR key[]={_T("and"),_T("array"),_T("begin"),_T("case"),_T("const"),_T("div"),_T("do"),_T("downto"),_T("else"),_T("end"),_T("for"),_T("function"),
		_T("if"),_T("mod"),_T("not"),_T("of"),_T("or"),_T("procedure"),_T("program"),_T("record"),_T("repeat"),_T("then"),_T("to"),_T("type"),
		_T("until"),_T("var"),_T("while")};
LPTSTR ksy[]={_T("andsy"),_T("arraysy"),_T("beginsy"),_T("casesy"),_T("constsy"),_T("divsy"),_T("dosy"),_T("downtosy"),_T("elsesy"),_T("endsy"),_T("forsy"),_T("functionsy"),
		_T("ifsy"),_T("modsy"),_T("notsy"),_T("ofsy"),_T("orsy"),_T("proceduresy"),_T("programsy"),_T("recordsy"),_T("repeatsy"),_T("thensy"),_T("tosy"),_T("typesy"),
		_T("untilsy"),_T("varsy"),_T("whilesy")};

LPTSTR objs[]={_T("konstant"),_T("vvariable"),_T("typel"),_T("procedure"),_T("function")};
LPTSTR types[]={_T("notyp"),_T("ints"),_T("reals"),_T("bools"),_T("chars"),_T("arrays"),_T("records")};
LPTSTR	typsy[]={_T("notyp"),_T("ints"),_T("reals"),_T("chars"),_T("bools"),_T("array"),_T("record")};
LPTSTR	codesy[]={_T("cnv"),_T("fct"),_T("eql"),_T("neq"),_T("gtr"),_T("lss"),_T("leq"),_T("neq")};

#define CODECOUNT 75
LPTSTR  mne[] = {_T("LDA"),_T("LOD"),_T("LDI"),_T("DIS"),_T("   "),_T("   "),_T("   "),_T("   "),_T("FCT"),_T("INT"),_T("JMP"),_T("JPC"),_T("SWT"),_T("CAS"),
			 _T("F1U"),_T("F2U"),_T("F1D"),_T("F2D"),_T("MKS"),_T("CAL"),_T("IDX"),_T("IXX"),_T("LDB"),_T("CPB"),_T("LDC"),_T("LDR"),_T("FLT"),_T("RED"),
			 _T("WRS"),_T("WRW"),_T("WRU"),_T("HLT"),_T("EXP"),_T("EXF"),_T("LDT"),_T("NOT"),_T("MUS"),_T("WRR"),_T("STO"),_T("EQR"),_T("NER"),_T("LSR"),
			 _T("LER"),_T("GTR"),_T("GER"),_T("EQL"),_T("NEQ"),_T("LSS"),_T("LEQ"),_T("GRT"),_T("GEQ"),_T("ORR"),_T("ADD"),_T("SUB"),_T("ADR"),_T("SUR"),
			 _T("AND"),_T("MUL"),_T("DIV"),_T("MOD"),_T("MUR"),_T("DIR"),_T("RDL"),_T("WRL"),_T("EQC"),_T("NEC"),_T("LSC"),_T("LEC"),_T("GRC"),_T("GEC"),
			 _T("LDS"),_T("ADC"),_T("FLC"),_T("VAL"),_T("ROW")};


CComplier::CComplier(void):irow(1),lc(1),it(-1),ia(-1),ib(-1),is(-1)
{


	m_pArrayMan = new CArrayMan();


	m_pArrayMan->AddItem((LPVOID &)display,sizeof(int));

	m_pArrayMan->AddItem((LPVOID &)tab,sizeof(tagtab));
	m_pArrayMan->AddItem((LPVOID &)atab,sizeof(tagatab));
	m_pArrayMan->AddItem((LPVOID &)btab,sizeof(tagbtab));
	m_pArrayMan->AddItem((LPVOID &)stab,sizeof(LPTSTR));

	m_pArrayMan->AddItem((LPVOID &)code,sizeof(order));
	m_pArrayMan->AddItem((LPVOID &)rconst,sizeof(float));

	m_pArrayMan->AddItem((LPVOID &)cols,sizeof(tagcols));

	c2=0;
	sc2 = -1;

	m_pGrid = NULL;

	psconst = new TCHAR[SCCONSTINIT];
	sconst = new LPTSTR[255];
	isconstpos = 0;

	int index = m_pArrayMan->AddItemData((LPVOID &)display);
	display[index] = 1;
	construct();
}

CComplier::~CComplier(void)
{
	/*
	delete symbol;
	delete key;
	delete ksy;
	delete sps;
	delete objs;
	delete types;
	*/
	delete display;
	delete tab;
	delete atab;
	delete btab;
	delete code;
	delete rconst;
	delete cols;
	delete m_pArrayMan;
	delete psconst;
	delete[] sconst;
}

int  CComplier::of_setsource (LPTSTR as_source)
{
	source =  as_source;
	sourcelen = (int)_tcslen(as_source);

	cc =-1; 

	snum= NULL;
	rnum=0;
	inum=0;

	sy= NULL;

	return 1;
};
int CComplier::readscale ()
{
	int	sign;
	int	s;

	nextch();
	sign = 1;
	s=0;

	if(ch=='+')
		nextch();
	else if(ch=='-')
	{
		sign = -1;
		nextch();
	}
	if(ch<'0' || ch>'9')
		error("readscale01");
	else
		do
		{
			s= 10*s + ch - '0';
			nextch();
		}while(ch>='0' && ch<='9');

		return sign*s;
}

void CComplier::nextch ()
{
	if(cc == sourcelen)
		ch='\0';
	else
	{
		cc++;
		ch=source[cc];
		if(ch=='\r')irow++;
	}
}

void CComplier::error (LPTSTR as_errcode)
{
	//MessageBox(LPTSTR(irow)+' ' +LPTSTR(cc),as_errcode)
}

void CComplier::adjustscale (int e)
{
	int	s;
	int	i;

	s=e>0?e: - e;
	if(e >0)
	{
		for(i=1;i<=s;i++)
		{
			rnum *= 10;
		}
	}else
	{
		for(i=1;i<=s;i++)
		{
			rnum /= 10;
		}
	}
}


void CComplier::enter (LPTSTR x0, LPTSTR x1, LPTSTR x2, short x3)
{
	int	t;
	t = this->m_pArrayMan->AddItemData((LPVOID &)tab);

	tab[t].name = x0;
	tab[t].link = t - 1;
	tab[t].obj  = x1;
	tab[t].typ  = x2;
	tab[t].refe = 0;
	tab[t].normal = true;
	tab[t].lev = 0;
	tab[t].adr = x3;
}

short CComplier::insymbol()
{
	int		i,j,k,e;
	CStr	ls_str;

	sy = NULL;
	snum = NULL;
	rnum = 0;
	inum = 0;

L1:
	while(ch==' ' || ch=='\r' || ch=='\n' || ch=='\t')
	{
		nextch();
	}

	if(ch=='\0')
	{
		sy= NULL;
		return 0;
	}
	if((ch>='a' && ch<='z') ||(ch>='A' && ch<='Z'))
	{
		snum= NULL;
		do
		{
			snum += ch;
			nextch();
		}while(ch =='_' || ch > 255  || (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9'));
		snum.makeLower();
		i=1;
		j=sizeof(key)/sizeof(key[0]);
		do
		{
			k=(i+j)/2;
			if(snum <= key[k])j=k - 1;
			if(snum >= key[k]) i=k + 1;
		}while( i<=j);
		if(i - 1> j)
			sy = ksy[k];
		else
			sy=_T("indent");

	}else if(ch >'0' && ch<='9')
	{
		k=0;
		inum = 0;
		sy=_T("intcon");
		do
		{
			inum = 10*inum + ch - '0';
			k++;
			nextch();
		}while(ch>='0' && ch<='9');
		if(ch=='.')
		{
			nextch();
			if(ch=='.')
				ch=':';
			else
			{
				sy =_T("realcon");
				rnum = inum;
				e=0;
				while (ch>='0' && ch<='9')
				{
					e= e - 1;
					rnum = rnum*10 + ch - '0';
					nextch();
				}
				if(e==0) error("decimal");
				if(ch=='e' ||ch=='E')e= readscale();
				if(e !=0)adjustscale(e);
			}
		}else if( ch=='e' ||ch=='E')
		{
			sy=_T("realcon");
			rnum = inum;
			e = 0;
			e = readscale();
			if(e != 0)adjustscale(e);
		}
	}else if(ch==':')
	{
		nextch();
		if(ch='=')
		{
			sy = _T("becomes");
			nextch();
		}else
			sy=_T("colon");
	}else if(ch=='<')
	{
		nextch();
		if(ch=='=')
		{
			sy=_T("leq");
			nextch();
		}else if(ch=='>')
		{
			sy=_T("neq");
			nextch();
		}else
			sy=_T("lss");
	}else if( ch=='>')
	{
		nextch();
		if(ch=='=')
		{
			sy=_T("geq"); 
			nextch();
		}else
			sy=_T("gtr");
	}else if(ch=='.')
	{
		nextch();
		if(ch=='.')
		{
			sy==_T("colon");
			nextch();
		}else
			sy=_T("period");
	}else if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='(' || ch==')' || ch=='[' || ch==']'
		|| ch=='=' || ch==';' || ch==',')  
	{
		sy = sps[ch];
		nextch();
	}else if(ch=='"')
	{
		sy =_T("strcon");
		snum=NULL;
L2:	nextch();
		while(!(ch=='\\' || ch=='\0' || ch=='"'))
		{
			snum = snum + ch;
			nextch();
		}
		if(ch=='\0')
		{
			error("string");
			nextch();
		}else if(ch=='\\')
		{
			nextch();
			if(ch=='\'')
				snum = snum +'\'';
			else if( ch=='"')
				snum = snum +'\"';
			else if(ch=='r')
				snum = snum +'\r';
			else if(ch=='n')
				snum = snum +'\n';
			else if(ch=='t')
				snum = snum +'\t';
			else
				snum = snum +'\\';
			goto L2;
		}
		nextch();
	}else if(ch= '\'')
	{
		sy = _T("strcon");
		snum=NULL;
L3:	nextch();
		while(!(ch=='\\' || ch=='\0' || ch=='\''))
		{
			snum = snum + ch;
			nextch();
		}
		if(ch=='\0')
		{
			error("string");
			nextch();
		}else if(ch='\\')
		{
			nextch();
			if(ch=='\'') 
				snum = snum +'\'';
			else if(ch=='"')
				snum = snum +'"';
			else if(ch=='r')
				snum = snum +'\r';
			else if(ch=='n')
				snum = snum +'\n';
			else if(ch=='t')
				snum = snum +'\t';
			else
				snum = snum +'\\';
			goto L3;
		}
		nextch();
	}else
	{
		error(_T("24"));
		nextch();
		goto L1;
	}
	return 1;
}

int	CComplier::expression (LPTSTR * &fsys, item &x)
{
	LPTSTR	ls_header[]={_T("eql"),_T("neq"),_T("gtr"),_T("lss"),_T("leq"),_T("neq")};
	item		y;
	CStr ls_sy,typ;
	
	simplesexpression(fsys,x);
	ls_sy = sy;
	if(!sy.m_pData || in(ls_header,sizeof(ls_header)/sizeof(ls_header[0]),(LPTSTR)sy)<0 ) return 1;

	insymbol();
	simplesexpression(fsys,y);

	if( x.typ.m_pData && ::_tcsstr("notyp/ints/bools/chars",x.typ) && x.typ == y.typ)
	{
		if(ls_sy==_T("eql"))
			emit(45);
		else if(ls_sy==_T("neq"))
			exit(46);
		else if(ls_sy==_T("lss"))
			exit(47);
		else if(ls_sy==_T("leq"))
			exit(48);
		else if(ls_sy==_T("gtr"))
			exit(49);
		else if(ls_sy==_T("geq"))
			exit(50);
	}else if(x.typ==_T("ints"))
	{
		x.typ=_T("reals");
		emit1(26,1);
	}else if(y.typ==_T("ints"))
	{
		y.typ=_T("reals");
		emit1(26,0);
	}
	if(x.typ==_T("reals") && y.typ==_T("reals"))
	{
		if(ls_sy==_T("eql"))
			emit(45);
		else if(ls_sy==_T("neq"))
			exit(46);
		else if(ls_sy==_T("lss"))
			exit(47);
		else if(ls_sy==_T("leq"))
			exit(48);
		else if(ls_sy==_T("gtr"))
			exit(49);
		else if(ls_sy==_T("geq"))
			exit(50);
	}
	x.typ = _T("bools");

	return 1;
}

int CComplier::standfct (LPTSTR * &fsys, int n, int i)
{
	LPTSTR *	ts=NULL;
	item		x,y[10];
	int count =0;

	if( sy==_T("lparent") )
		insymbol();
	else
		error(_T("standfct lparent"));

	if( n < 17 )
	{
		//int index = this->m_pArrayMan->AddItemData((LPVOID &)fsys);
		//fsys[index]=_T("rparent");
		expression(fsys,x);
		switch(n)
		{
		case 0: //abs
		case 2: //sqr
			{
				count = 2;
				ts = new LPTSTR[2];
				ts[0] = _T("ints");
				ts[1] = _T("reals");
				tab[i].typ = x.typ;
				if(x.typ==_T("reals"))n++;
			}
			break;
		case 4: //odd
		case 5: //char
			{
				count = 1;
				ts = new LPTSTR[1];
				ts[0] = _T("ints");
			}
			break;
		case 6: //odr
			{
				count = 3;
				ts = new LPTSTR[3];
				ts[0] = _T("ints");
				ts[1] = _T("boolea");
				ts[2] = _T("chars");
			}
			break;
		case 7: //succ
		case 8:
			{
				count = 3;
				ts = new LPTSTR[3];
				ts[0] = _T("ints");
				ts[1] = _T("boolea");
				ts[2] = _T("chars");
				tab[i].typ = x.typ;
			}
			break;
		case 9: //round,trunc,sin,cos..
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			{
				count = 2;
				ts = new LPTSTR[2];
				ts[0] = _T("ints");
				ts[1] = _T("reals");
				if(x.typ==_T("ints"))
				{
					emit1(26,x.refe);
					x.refe = this->m_pArrayMan->GetUpperBound((LPVOID &)rconst) + 1;  //??
				}


			}
			break;
		}
		if(x.typ.m_pData && in(ts,count,x.typ) >= 0)
			emit1(8,n);
		else
			if( x.typ != _T("notyp"))error(_T("48"));
	}else if (n==17 ||n==18) //read ,write
	{
		if( sy != _T("indent"))
			error(_T("standfct indent"));
		else
			insymbol();
		emit1(8,n);
	}else if( n>=1000)
	{
		expression(fsys,x);
		switch(n)
		{
		case 1002: //Char,space
		case 1015:
			{
				if( x.typ != _T("ints"))
					error(_T("args should interger"));
				else
					emit1(8,n);
			}
			break;
		case 1001: //Asc,len,lower,reverse,space,trim,upper,lefttrim,righttrim
		case 1006:
		case 1007:
		case 1012:
		case 1016:
		case 1017:
		case 1005:
		case 1014:
			{
				if( x.typ != _T("strs"))
					error(_T("args should string"));
				else
					emit1(8,n);
			}
			break;
		case 1003: //fill,left,right
		case 1004:
		case 1013:
			{
				if( x.typ != _T("strs") && x.typ !=_T("notyp"))
				{
					error("args1 should string");
					break;
				}
				if(x.typ ==_T("notyp"))
				{
					emit1(72,0);
				}

				insymbol();
				expression(fsys,y[0]);
				if( y[0].typ !=_T("ints") && y[0].typ !=_T("notyp") )
					error(_T("args2 should integer"));
				if(y[0].typ ==_T("notyp"))
				{
					emit1(8,1002); //toint
				}
				emit1(8,n);
			}
			break;
		case 1008: //match
			{
				if( x.typ ==_T("strs"))
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ !=_T("strs") )
						error(_T("arg2 should string"));
					else
						emit1(8,n);
				}
				else
					error(_T("arg1 should string"));
			}
			break;
		case 1009: //mid
			{
				if( x.typ == _T("strs"))
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ !=_T("ints") )
						error(_T("arg2 should integer"));
					else
					{
						if( sy==_T("comma" ))
						{
							insymbol();	//drop comma
							expression(fsys,y[1]);
							if( y[1].typ !=_T("ints"))
								error(_T("arg3 should integer"));
							else
								emit1(8,n);
						}
						else
						{
							emit1(24,0);
							emit1(8,n);
						}
					}
				}
				else
					error(_T("arg1 should string"));
			}
			break;
		case 1010: //pos
			{
				if( x.typ == _T("strs"))
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ !=_T("strs"))
						error(_T("arg2( should string"));
					else
					{
						if( sy=_T("comma"))
						{
							insymbol();	//drop comma
							expression(fsys,y[1]);
							if(y[1].typ !=_T("ints"))
								error(_T("arg3 should integer"));
							else
								emit1(8,n);
						}
						else
						{
							emit1(24,1);
							emit1(8,n);
						}
					}
				}
				else
					error(_T("arg1 should string"));
			}
			break;
		case 1011: //replace
			{
				if( x.typ == _T("strs"))
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( x.typ !=_T("ints"))
						error(_T("arg2 should integer"));
					else
					{
						insymbol();	//drop comma
						expression(fsys,y[1]);
						if( y[1].typ !=_T("ints"))
							error(_T("arg3 should integer"));
						else
						{
							insymbol();	//drop comma
							expression(fsys,y[2]);
							if( y[2].typ !=_T("strs"))
								error(_T("arg4 should string"));
							else
								emit1(8,n);
						}
					}
				}
				else
					error(_T("arg1 should string"));
			}
			break;
		}
	}

	if(ts) delete[] ts;
	x.typ = tab[i].typ;
	if( sy == _T("rparent"))
		insymbol();
	else
	{
		error(_T("standfct rparent"));
		return 0;
	}

	return 1;
}

int CComplier::in (LPTSTR aa_args[], int count , LPTSTR value)
{

	int	ll_i;
	CStr str = value;
	for( ll_i = 0;ll_i<count;ll_i++)
	{
		if(str == aa_args[ll_i])return ll_i;
	}
	return -1;
}

int  CComplier::variableeddeclaration()
{
	return 1;
}


int CComplier::loc(LPTSTR as_id)
{
	int	ll_i;
	int upper = this->m_pArrayMan->GetUpperBound((LPVOID &)tab);
	for(ll_i=0;ll_i<=upper;ll_i++)
	{
		if((CStr)as_id == tab[ll_i].name) return ll_i;
	}
	return -1;
}

int  CComplier::factor (LPTSTR * &fsys, item &x)
{
	int i,f;

	x.typ =_T("notyp");
	x.refe = 0; 

	while(sy.m_pData && in(facbegsys,sizeof(facbegsys)/sizeof(facbegsys[0]),sy) >=0)
	{
		if(sy==_T("indent"))
		{
			i=loc(snum);
			if(i< 0)
			{
				emit1(80,this->enterstr(snum));
				insymbol();
				return 1;

				/*
				int ll_index,ll_colid;
				LPSTR ls_type;
				i = getdsindex(snum,ll_index,ll_colid,ls_type);
				if(i < 0)
				{
					error(_T("factor indent"));
					insymbol();
					return 0;
				}else
				{
					emit1(74,ll_index);	//getRow
					emit2(73,ll_index,ll_colid);	//getValue
					x.typ = ls_type;

					insymbol();
					return 1;
				}
				*/
			}
			insymbol();
			if(tab[i].obj== _T("konstant"))
			{
				x.typ = tab[i].typ;
				x.refe = -1;
				if(x.typ ==_T("reals"))
					emit1(25,tab[i].adr);
				else
					emit1(24,tab[i].adr);
			}else if(tab[i].obj== _T("vvariable"))
			{
				x.typ = tab[i].typ;
				x.refe = tab[i].refe;
				if(sy==_T("lbrack") || sy==_T("lparent") ||sy==_T("period"))
				{
					if(tab[i].normal)
						f = 0;
					else
						f=1;
					emit2(f, tab[i].lev, tab[i].adr);
					//!			selector(fsys,x)
					if(x.typ.m_pData && in(stantyps,sizeof(stantyps)/sizeof(stantyps),x.typ) >=0)
					{
						if(tab[i].normal)
							f=1;
						else
							f=2;
					}else
					{
						if(tab[i].normal)
							f=0;
						else
							f=1;
					}
				}
				emit2(f,tab[i].lev,tab[i].adr);
			}else if(tab[i].obj== _T("typel") || tab[i].obj== _T("prozedure"))
			{
				error(_T("factor 3"));
			}else if(tab[i].obj== _T("function"))
			{
				x.typ = tab[i].typ;
				if(tab[i].lev != 0)
				{
					//!			call(fsys,i)
				}
				else
					standfct(fsys,tab[i].adr,i);
			}
		}else if(sy.m_pData && ::_tcsstr(_T("charcon/intcon/realcon/strcon"),sy))
		{
			if(sy==_T("realcon"))
			{
				x.typ=_T("reals");
				enterreal(rnum);
				emit1(25,c1);
			}else if(sy==_T("strcon"))
			{ 
				x.typ = _T("strs");
				enterstr(snum);
				emit1(70,sc1);
			}else
			{
				if(sy==_T("charco"))
					x.typ = _T("chars");
				else
					x.typ = _T("ints");
				emit1(24,inum); //LDC
			}
			x.refe = -1;
			insymbol();
		}else if(sy==_T("lparent"))
		{
			insymbol();
			expression(fsys,x);
			if(sy==_T("rparent"))
				insymbol();
			else
				error(_T("factor 4"));
		}else if(sy=_T("notsy"))
		{
			insymbol();
			factor(fsys,x);
			if (x.typ==_T("bools"))
				emit(35);
			else
			{
				if(x.typ == _T("nottyp"))
					error(_T("factor 5"));
			}
			//!	test(fsys,facbegsys,6)
		}
	}
	return  1;
}

int CComplier::simplesexpression(LPTSTR * &fsys, item &x)
{
	CStr	op;
	item 		y;

	if(sy==_T("plus") || sy==_T("minus"))
	{
		op = sy;
		insymbol();
		term(fsys,x);
		if(x.typ > _T("reals"))              //??
			error(_T("simplessExpression 1"));
		else
		{
			if(op ==_T("minus"))
				emit(36);
		}
	}else
		term(fsys,x);

	while(sy==_T("plus") || sy==_T("minus") || sy==_T("orsy"))
	{
		op = sy;
		insymbol();
		term(fsys,y);
		if(op==_T("orsy"))
		{
			if(x.typ==_T("bools") &&  y.typ==_T("bools"))
				emit(51);
			else
			{
				if (x.typ !=_T("notyp") && x.typ !=_T("notyp"))
				{
					error(_T("simplessExpression 2"));
					x.typ =_T("notyp");
				}
			}
		}else
		{
			x.typ = resulttype(x.typ,y.typ);
			if(x.typ==_T("strs"))
				emit(71);
			else if(x.typ==_T("ints"))
			{
				if(op ==_T("plus"))
					emit(52); //ADD
				else
					emit(53); //PLUS
			}else if(x.typ==_T("reals"))
			{
				if(op ==_T("plus"))
					emit(54);
				else
					emit(55);
			}else
			{
				emit(71);
			}

		}
	}
	return 1;
}


LPTSTR CComplier::resulttype (LPTSTR l1, LPTSTR l2)
{
	CStr t1,t2;
	t1 = l1;
	t2 = l2;

	if(t1==t2) return l1;
	if((t1==_T("arrays") && t2 !=_T("arrays")) ||(t1!=_T("arrays") && t2 ==_T("arrays"))) return _T("");
	if((t1==_T("recorders") && t2 !=_T("recorders")) ||(t1!=_T("recorders") && t2 ==_T("recorders"))) return _T("");

	if(t1==_T("notyp")) return l2;
	if(t2==_T("notyp")) return l1;

	if(t2==_T("reals") &&  t1==_T("ints"))
	{
		emit1(26,1);
		return l2;
	}
	if(t1==_T("reals") &&  t2==_T("ints"))
	{
		emit1(26,0);
		return l1;
	}
	if(t1==_T("strs") &&  t2!=_T("strs"))
	{
		emit1(72,0);
		return l1;
	}
	if(t1!=_T("strs") &&  t2==_T("strs"))
	{
		emit1(72,1);
		return l2;
	}


	return _T("chars");
}

void CComplier::emit1 (int  fct, int b)
{
	lc = this->m_pArrayMan->AddItemData((LPVOID &)code);
	code[lc].f = fct;
	code[lc].y = b;
}

void CComplier::emit (int  fct)
{
	lc = this->m_pArrayMan->AddItemData((LPVOID &)code);
	code[lc].f = fct;
}

void CComplier::emit2 (int  fct, int a,int b)
{
	lc = this->m_pArrayMan->AddItemData((LPVOID &)code);
	code[lc].f = fct;
	code[lc].x = a;
	code[lc].y = b;
}

int  CComplier::term(LPTSTR * &fsys, item &x)
{
	CStr	op;
	item	y;

	factor(fsys,x);
	while(sy.m_pData && ::_tcsstr(_T("times/rdiv/idiv/imod/andsy"),sy))
	{
		op = sy;
		insymbol();
		factor(fsys,y);

		if(op == _T("rdiv") && x.typ == _T("ints") && y.typ==_T("ints"))op == _T("idiv");

		if(op ==_T("times"))
		{
			x.typ =resulttype(x.typ,y.typ);
			if(x.typ==_T("ints")) 
				emit(57); //MUL
			else if(x.typ==_T("reals"))
				emit(60); //MUR
		}else if(op=_T("rdiv"))
		{
			if(x.typ=_T("ints"))
			{
				emit1(26,1);
				x.typ=_T("reals");
			}
			if(y.typ=_T("ints"))
			{
				emit1(26,0);
				y.typ=_T("reals");
			}
			if(x.typ==_T("reals") && y.typ==_T("reals"))
				emit(61);
			else
			{
				if(x.typ !=_T("notyp") && y.typ !=_T("notyp"))
				{
					error(_T("term 1"));
					x.typ=_T("notyp");
				}
			}
		}else if(op==_T("andsy"))
		{
			if(x.typ==_T("bools") && y.typ ==_T("bools")) 
				emit(56);
			else
			{
				if(x.typ !=_T("notyp") &&  y.typ !=_T("notyp"))
				{
					error(_T("term 2"));
					x.typ = _T("notyp");
				}
			}
		}else
		{
			if(x.typ ==_T("ints") && y.typ==_T("ints"))
			{
				if(op=="idiv")
					emit(58);
				else
					emit(59);
			}else
			{
				if(x.typ !=_T("notyp") &&  y.typ !=_T("notyp"))
				{
					error(_T("term 3"));
					x.typ = _T("notyp");
				}
			}
		}
	}
	return 1;
}

int CComplier::enterreal (float x)
{
	rconst[c2+1] = x;          //??
	c1 = 0;

	while( rconst[c1] != x )
		c1++;

	if(c1>c2 ) c2= c1;
	return c2;
}

int CComplier::interpret (var & value)
{
	order	ir;
	int	pc,t,b,h1,h2,h3,h4;
	int	lncnt,ocnt,blkcnt,chrcnt;

	CStr	ps;
	int		fld[100];	//fld[]
	int		disp[100];  //disp[]

	stack	s; //s[]


	s[1] = 0;
	s[2] = 0;
	s[3] = 0;
	//s[4] = btab[1].lastadr
	s[4] = 0;

	disp[0] = 0;
	b = 0;

	//t = btab[2].vsize - 1
	//pc=btab[1].lastAdr
	t = 4;
	pc = 0;

	lncnt = 0;
	ocnt = 0;
	chrcnt = 0;
	ps=_T("run");

	fld[1] = 10;
	fld[2] = 22;
	fld[3] = 10;
	fld[4] = 1;

	do
	{
		ir = code[pc];
		pc = pc+1;
		ocnt = ocnt + 1;

		switch(ir.f)
		{
		case 0:	//load address
			t++;
			s[t] = disp[ir.x] + ir.y;
			break;
		case 1:	//load value
			t++;
			s[t] = s[disp[ir.x] + ir.y];
			break;
		case 2:	//load indirect
			t++;
			s[t] = s[s[disp[ir.x] + ir.y].i];
			break;
		case 3:	//update display
			h1 = ir.y;
			h2 = ir.x;
			h3 = b;
			do
			{
				disp[h1] = h3;
				h1 = h1 - 1;
				h3 = s[h3+2];
			}while(h2!=h1);
			break;
		case 8: //fucntion
			switch(ir.y)
			{
			case 0: 
				s[t] = abs(s[t].i);
				break;
			case 1: 
				s[t] = abs(s[t].f);
				break;
			case 2:
				s[t] = (int)sqrt((float)s[t].i);
				break;
			case 3:
				s[t].f = sqrt(s[t].f);
				break;
			case 4:
				//s[t] = odd(s[t].i);
				break;
			case 5:
				s[t] = s[t].ch;   //chr
				break;
			case 6:
				//s[t] = ord(s[t].i);
				break;
			case 7:
				//s[t] = succ(s[t].i);
				break;
			case 8:
				//s[t] = pred(s[t].i);
				break;
			case 9:
				//s[t] = round(s[t].f);
				break;
			case 10:
				//s[t] = trunc(s[t].f);
				break;
			case 11:
				s[t].f = sin(s[t].f);
				break;
			case 12:
				s[t].f = cos(s[t].f);
				break;
			case 13:
				s[t].f = exp(s[t].f);
				break;
			case 14:
				//s[t] = ln(s[t].f);
				break;
			case 15:
				s[t].f = sqrt(s[t].f);
				break;
			case 16:
				//s[t] = arcTan(s[t].f);
				break;
			case 17:
				break;
			case 18:
				break;
			case 1001:
				s[t].toString(); //asc
				break;
			case 1002:
				s[t].toInt(); //char,int
				break;
			case 1003: //fill
				_tcsset_s(s[t].m_pStr->m_pData,s[t].m_pStr->m_iStrlen,s[t+1].ch);
				break;
			case 1004:  //left
				t = t - 1;
				s[t].m_pStr->m_pData[s[t+1].i> s[t].m_pStr->m_iStrlen?s[t].m_pStr->m_iStrlen:s[t+1].i]='\0';
				s[t].m_pStr->m_iStrlen = s[t+1].i> s[t].m_pStr->m_iStrlen?s[t].m_pStr->m_iStrlen:s[t+1].i;
				break;
			case 1005: //trimleft
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.TrimLeft();
					s[t] = (const LPTSTR)cs.GetBuffer();
				}
				break;

			case 1006: //len
				s[t] = s[t].m_pStr->m_iStrlen;
				break;

			case 1007:  //lower
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.MakeLower();
					s[t] = (const LPTSTR)cs.GetBuffer();
				}
				break;

			case 1008: //match
				//s[t] = match(s[t],s[t+1]);
				s[t] = false; //未实现
				break;
			case 1009: //mid
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					t = t - 2;
					if(s[t+2].i == 0)
						s[t] = cs.Mid(s[t+1].i).GetBuffer();
					else
						s[t] = cs.Mid(s[t+1].i,s[t+2].i).GetBuffer();
				}
				break;
			case 1010:	//pos
				{
					t = t - 2;
					CString cs;
					cs = s[t].m_pStr->m_pData;
					s[t] = cs.Find(s[t+1].m_pStr->m_pData,s[t+2].i);
				}
				break;
			case 1011: //replace
				{
					t = t - 3;
					//s[t] = replace(s[t],s[t+1],s[t+2],s[t+3]); //未实现
				}
				break;
			case 1012: //reverse
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.MakeReverse();
					s[t] = cs;
					break;
				}
			case 1013: //right
				{
					t = t -1;
					CString cs;
					cs = s[t].m_pStr->m_pData;
					s[t]  = cs.Right(s[t+1]);
				}
				break;
			case 1014: //righttrim
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.TrimRight();
					s[t]  = cs;
				}
				break; 
			case 1015: //space
				{
					TCHAR*  pBuf = new TCHAR[s[t].i];
					*(pBuf + s[t].i) ='\0';
					_tcsset_s(pBuf,s[t].i,' ');
					s[t]=pBuf;
					delete pBuf;
				}
				break;
			case 1016: //trim
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.Trim();
					s[t]  = cs;
				}
				break;
			case 1017: //upper
				{
					CString cs;
					cs = s[t].m_pStr->m_pData;
					cs.MakeUpper();
					s[t]  = cs;
				}
				break;
			}
			break;
		case 9:	//offest
			s[t].i = s[t].i + ir.y;
			break;
		case 10:	//jump
			pc = ir.y;
			break;
		case 11:	//Condition Jump
			if(s[t].i == 1) pc = ir.y;
			break;
		case 12:	//switch
			h1 = s[t];
			h2 = ir.y;
			t = t -1;
			h3 =0;
			do
			{
				if(code[h2].f != 13)
				{
					h3 = 1;
					ps =_T("caschk");
				}else
				{
					if(code[h2].y == h1)
					{
						h3 = 1;
						pc = code[h2+1].y;
					}else
						h2 = h2+2;
				}
			}while(h3 == 0);
			break;
		case 14:	//for1up
			h1 = s[t - 1];
			if(h1 < s[t].i)
				s[s[t - 2]] = h1;
			else
			{
				t = t - 3;
				pc = ir.y;
			}
			break;
		case 15: //for2up
			h2 = s[t - 2];
			h1 = s[h2].i + 1;
			if (h1<= s[t].i )
			{
				s[h2] = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 16: //for1down
			h1 = s[t - 1];
			if(h1 >=s[t].i)
			{
				s[h2] = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 17: //for2down
			h2 = s[t - 2];
			h1 = s[h2].i - 1;
			if(h1 > s[t].i) 
			{
				s[h2] = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 18: //mark stack
			h1 = btab[tab[ir.y].refe].vsize;
			t = t+5;
			s[t - 1] = h1 -1; 
			s[t] = ir.y;
			break;
		case 19:	//call
			h1 = t - ir.y;
			h2 = s[h1+4];
			h3 = tab[h2].lev;
			h4 = s[h1+3].i + h1;
			s[h1+1] = pc;
			s[h1+3] = b;
			for(h3 = t+1;h3<=h4;h3++)
				s[h3] = 0;
			b = h1;
			t = h4;
			pc = tab[h2].adr;
			break;
		case 20:	//index1
			h1 = ir.y;
			h2 = atab[h1].low;
			h3 = s[t];
			if(h3 < h2)
				ps = _T("inxchk");
			else
			{
				if(h3 > atab[h1].high)
					ps = _T("inxchk");
				else
				{
					t = t - 1;
					s[t] = s[t].i + h3 -h2;
				}
			}
			break;
		case 21:	//index
			h1 = ir.y;
			h2 = atab[h1].low;
			h3 = s[t];
			if(h3 < h2)
				ps = _T("inxchk");
			else
			{
				if(h3 > atab[h1].high)
					ps =_T("inxchk");
				else
				{
					t = t - 1;
					s[t] = s[t].i + (h3 - h2)*atab[h1].elsize;
				}
			}
			break;
		case 22:	//load block
			h1 = s[t];
			t = t - 1;
			h2 = ir.y + t;
			while(t < h2)
			{
				t = t + 1;
				s[t] = s[h1];
				h1 = h1 + 1;
			}
			break;
		case 23:	//copy block
			h1 = s[t - 1];
			h2 = s[t];
			h3 = h1 + ir.y;
			while(h1 < h3)
			{
				s[h1] = s[h2];
				h1 = h1 + 1;
				h2 = h2 + 1;
			}
			t = t - 2;
			break;
		case 24:	//literral
			t = t +1;
			s[t] = ir.y;
			break;
		case 25:	//loaD REAL
			t = t + 1;
			s[t] = rconst[ir.y];
			break;
		case 26:	//float
			h1 = t - ir.y;
			s[h1].f = s[h1];
			break;
		case 27:	//reak
			//
			break;
		case 28:	//write strig
			//
			break;
		case 29: //write1
			//
			break;
		case 30:	//write2
			//
			break;
		case 31: 
			ps = _T("fin");
			break;
		case 32: //exit procedure
			t = b - 1;
			pc = s[b+1];
			break;
		case 33:	//exit function
			t = b;
			pc = s[b+1];
			break;
		case 34: 
			s[t] = s[s[t]];
			break;
		case 35:
			if(s[t].b == true)
				s[t] = 0;
			else
				s[t] = true;
			break;
		case 36:
			s[t] = - s[t].i;
			break;
		case 37:
			//
			break;
		case 38:	//store
			s[s[t - 1]] = s[t];
			t = t - 2;
			break;
		case 39:	
			t = t -1;
			if(s[t] == s[t+1]) 
				s[t] = 0;
			else
				s[t] = true;
			break;
		case 40:
			t = t -1;
			if(s[t].f != s[t+1].f)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 41:
			t = t - 1;
			if(s[t].f < s[t+1].f)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 42:
			t = t - 1;
			if(s[t].f <= s[t+1].f)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 43:
			t = t - 1;
			if(s[t].f > s[t+1].f)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 44:
			t = t - 1;
			if(s[t].f >= s[t+1].f)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 45:
			t = t - 1;
			if(s[t].i == s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 46:
			t = t - 1;
			if(s[t].i != s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 47:
			t = t - 1;
			if(s[t].i < s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 48:
			t = t - 1;
			if(s[t].i <= s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 49:
			t = t - 1;
			if(s[t].i > s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 50:
			t = t - 1;
			if(s[t].i >= s[t+1].i)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 51:		//or
			t = t - 1;
			if(s[t].b || s[t+1].b)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 52:                   
			t = t -1;
			s[t] = s[t].i + s[t + 1].i;
			break;
		case 53:
			t = t - 1;
			s[t] = s[t].i - s[t+1].i;
			break;
		case 54:
			t = t -1;
			s[t] = s[t].f + s[t + 1].f;
			break;
		case 55:
			t = t -1;
			s[t] = s[t].f - s[t + 1].f;
		case 56:
			t = t -1;
			if(s[t].b && s[t+1].b)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 57:
			t = t -1;
			s[t] = s[t].i * s[t+1].i;
			break;
		case 58:	//idiv
			t = t -1;
			if(s[t+1].i = 0)
				ps = _T("divchk");
			else
				s[t] = int(s[t].i/s[t+1].i);
			break;
		case 59:
			t = t -1;
			if(s[t+1].i == 0)
				ps =_T("divchk");
			else
				s[t] = s[t].i % s[t+1].i;
			break;
		case 60:	//mul
			t = t - 1;
			s[t] = s[t].f*s[t+1].f;
			break;
		case 61:	//rdiv
			t = t -1;
			s[t] = s[t].f /s[t+1].f;
			break;
		case 62:
			//
			break;
		case 63:
			//
			break;
		case 64:       //str ==
			t = t - 1;
			if(s[t].m_pStr == s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 65:
			t = t - 1;
			if(s[t].m_pStr != s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 66:
			t = t - 1;
			if(s[t].m_pStr < s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 67:
			t = t - 1;
			if(s[t].m_pStr <= s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 68:
			t = t - 1;
			if(s[t].m_pStr > s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 69:
			t = t - 1;
			if(s[t].m_pStr >= s[t+1].m_pStr)
				s[t] = true;
			else
				s[t] = false;
			break;
		case 70:	//LDS
			t = t +1;
			s[t] = sconst[ir.y];
			break;
		case 71: //ADC
			t = t - 1;
			*(s[t].m_pStr) += *(s[t+1].m_pStr);
			break;
		case 72: //FLC
			s[t - ir.y].toString();
			break;
		case 73:	//VAL
			  /*if ir.x > upperBound(ids_objs) Then
			  ps = 'upper dataStore'
			  elseif ir.y > int(ids_objs[ir.x].object.datawindow.column.count) then
			  ps = 'upper datastore colCount'
			  elseif s[t] > ids_objs[ir.x].fowCount() then
			  ps = 'upper dataStore RowCount'
			  else
			  s[t] = ids_objs[ir.x].object.data[s[t],ir.y]
			  string	ls_coltype
			  ls_colType = lower(ids_objs[ir.x].describe('#'+string(ir.y)+'.coltype'))
			  CHOOSE case Left ( ls_coltype , 5 )
			  case "char("						//  CHARACTER DATATYPE
			  case "date"							//  DATE DATATYPE
			  s[t] = string(s[t],'yyyy/mm/dd')
			  case "datet"						//  DATETIME DATATYPE
			  s[t] = string(s[t],'yyyy/mm/dd')
			  case "decim"						//  DECIMAL DATATYPE
			  case "numbe", "doubl", "real"	//  DOUBLE DATATYPE	
			  case "int", "uint"				//  int DATATYPE	
			  case "time", "times"				//  TIME DATATYPE
			  s[t] = string(s[t],'hh:mm:ss am/pm')
			  END CHOOSE
			  End if*/
			  break;
			case 74:	//ROW
			  t ++;
			  //s[t] = ids_objs[ir.y].getRow();
			  break;
			case 80: //CELL;
				{
				  t ++;
				  if(m_pGrid)
				  {
				    int row,col;
					if(m_pGrid->ConvertNameToRowCol(sconst[ir.y],col,row))
					{
						s[t] = m_pGrid->GetCells(col,row);
					}else
						s[t]=_T("");
				   }
				  else
					s[t]=_T("");
				}
			  break;
			case 81: //CELL SETS
			  t ++;
			  break;
		}
	}while(ps == _T("run"));

	if(!(ps==_T("fin")))
	{
		//MessageBox('Run Error',ps)
		//setNull(s[t])
		return -1;
	}

	value = s[t];
	return 1;
}

int  CComplier::of_calcexpression (LPTSTR source1,var & value)
{
	of_setsource(source1);
	nextch();
	insymbol();

	CStr *	fsys;
	item		x;

	expression((LPTSTR *&)fsys,x);
	emit(31);	//quit

	return  interpret(value);
}


int CComplier::of_comppcode (LPTSTR source1)
{
	of_setsource(source1);
	nextch();
	insymbol();

	CStr *	fsys;
	item		x;

	expression((LPTSTR *&)fsys,x);
	emit(31);	//quit

	typ = x.typ;

	return 1;
}


int CComplier::of_execpcode ()
{
	var value;
	return  interpret(value);
}


LPSTR CComplier::printtable()
{
	const int codeNumber = 75;

	int		i;
	CStr	ls_syntax;
	TCHAR  buf[50];

	for(i=0; i< CODECOUNT ;i++)
	{
		if(code[i].x == 0 && code[i].y==0)
			::StringCbPrintf(buf,50, "%s", mne[code[i].f]);
		else if(code[i].x != 0 && code[i].y==0)
			::StringCbPrintf(buf, 50,"%s     %10i", mne[code[i].f],code[i].x);
		else if(code[i].x != 0 && code[i].y!=0)
			::StringCbPrintf(buf, 50,"%s     %10i     %10i", mne[code[i].f],code[i].x,code[i].y);
		else //if(code[i].x == 0 && code[i].y!=0)
			::StringCbPrintf(buf, 50,"%s               %10i", mne[code[i].f],code[i].y);

		if(i ==0)
			ls_syntax = (LPTSTR)buf;
		else
			ls_syntax +=  _T("\r\n");/* + (CStr)(LPTSTR)buf*/;
	}
	return ls_syntax;
}


int  CComplier::of_enterstr (float x)
{
	rconst[c2+1] = x;
	c1 = 0;

	while(rconst[c1] != x )
	{
		c1++;
	}

	if(c1>c2)c2= c1;

	return c2;
}

int  CComplier::enterstr(LPTSTR x)
{
	sc2++;
	sc1 = sc2;
	
	if(x==NULL)
	{
		sconst[sc2] = NULL;
		isconstpos += 1;
	}else 
	{
		int len = _tcslen(x);
		_tcscpy_s(psconst + isconstpos,len + 1,x);
		sconst[sc2] = psconst + isconstpos;
		isconstpos += len + 1;
	}
	return sc2;
	/*
	sconst[sc2+1] = x;
	sc1 = 0;

	while(sconst[sc1] != x )
	{
		sc1++;
	}

	if(sc1>sc2)sc2= sc1;

	return sc2;*/
}

/*public function integer of_setdslist (ref datastore ads_objs[], boolean ab_cols);ids_objs = ads_objs

long	ll_colCount
long	ll_i,ll_j,ll_index=1
string	ls_colType

if ab_cols then
tagCols	temp[]
cols = temp

For ll_i = 1 To upperBound(ads_objs)
ll_colCount = long(ads_objs[ll_i].object.datawindow.column.count)
For ll_j = 1 To ll_colCount
cols[ll_index].name = ads_objs[ll_i].describe('#'+string(ll_j)+'.name')
if pos(ads_objs[ll_i].describe('#'+string(ll_j)+'_t.name'),'?!') >0 Then
cols[ll_index].desc = ads_objs[ll_i].describe('#'+string(ll_j)+'_t.name')
else
cols[ll_index].desc = cols[ll_index].name
End if
cols[ll_index].index = ll_i
cols[ll_index].colId = ll_j

ls_coltype = Lower(ads_objs[ll_i].Describe ( '#'+string(ll_j) + ".ColType" ))
CHOOSE CASE Left ( ls_coltype , 5 )
CASE "char("						//  CHARACTER DATATYPE
cols[ll_index].typ='strs'
CASE "date"							//  DATE DATATYPE
cols[ll_index].typ='strs'
CASE "datet"						//  DATETIME DATATYPE
cols[ll_index].typ='strs'
CASE "decim"						//  DECIMAL DATATYPE
cols[ll_index].typ='reals'
CASE "numbe", "doubl", "real"	//  DOUBLE DATATYPE	
cols[ll_index].typ='reals'
CASE "long", "ulong"				//  LONG DATATYPE	
cols[ll_index].typ='ints'
CASE "time", "times"				//  TIME DATATYPE
cols[ll_index].typ='strs'
END CHOOSE
ll_index++
Next
Next
End if

return upperBound(ids_objs)
end function
*/
int of_setcols (LPTSTR as_name[],  LPTSTR as_desc[], int al_index[], int al_order[], LPTSTR as_type[])
{
	/*
	cols = temp

	For ll_i = 1 To upperBound(as_name)
	cols[ll_i].name = as_name[ll_i]
	cols[ll_i].desc = as_desc[ll_i]
	cols[ll_i].index = al_index[ll_i]
	cols[ll_i].colid = al_order[ll_i]
	cols[ll_i].typ = as_type[ll_i]
	Next
	*/
	return 1;
}

int  CComplier::getdsindex (LPTSTR as_name, int &al_index, int &al_colid, LPTSTR &as_type)
{
	int		ll_row;
	bool	lb_find = false;
	for(ll_row=0;ll_row<=this->m_pArrayMan->GetUpperBound((LPVOID &)cols);ll_row++)
	{
		if((CStr)as_name == cols[ll_row].desc)
		{
			lb_find = true;
			break;
		}
	}

	if(!lb_find)
	{
		for(ll_row=0;ll_row<=this->m_pArrayMan->GetUpperBound((LPVOID &)cols);ll_row++)
		{
			if((CStr)as_name == cols[ll_row].name)
			{
				lb_find = true;
				break;
			}
		}
	}	
	if(!lb_find)return 0;

	al_index = cols[ll_row].index;
	al_colid = cols[ll_row].colid;
	as_type =  cols[ll_row].typ;

	return ll_row;
}

int CComplier::constex(LPTSTR fsys[], conrec &c)
{
	c.tp=_T("nonty");
	c.val = 0;

	int		sign;

	if(sy.m_pData && in(constbegsys,sizeof(constbegsys)/sizeof(constbegsys[0]),(LPTSTR)sy) >= 0)
	{
		if(sy==_T("charcon"))
		{
			c.tp = _T("chars");
			c.val = inum;
			insymbol();
		}else if(sy==_T("strs"))
		{
			c.tp=_T("strs");
			c.val = snum;
			insymbol();
		}else
		{
			sign = 1;
			if(sy==_T("plus") || sy==_T("minus"))
			{
				if(sy==_T("minus"))sign = -1 ;
				insymbol();
			}
			if(sy==_T("indent"))
			{
				int	x;
				x=loc(snum);
				if(x > 0)
				{
					if(tab[x].obj !=_T("konstant"))
						error(_T("25"));
					else
					{
						c.tp = tab[x].typ;
						if(c.tp ==_T("reals"))
							c.val = sign*rconst[tab[x].adr];
						else if (c.tp ==_T("strs"))
							c.val = sconst[tab[x].adr];
						else
							c.val = tab[x].adr;
					}
					insymbol();
				}else if(sy==_T("ints"))
				{
					c.tp=_T("ints");
					c.val = sign*inum;
					insymbol();
				}else if(sy==_T("reals"))
				{
					c.tp=_T("reals");
					c.val = (sign*rnum);  //
					insymbol();
				}else
				{

				}
			}
		}
	}

	return 1;
}


int  CComplier::arraytyp (int &aref, int &arsz)
{
	CStr	elty;
	conrec	low,high;
	int		elrf,elsz;

	return 1;
}

int CComplier::of_dsreset ()
{
	//tagCols	temps[]

	//cols = temps
	return 1;
}

int CComplier::construct()
{
	int spsNumber = 255;
	int index;
	index = m_pArrayMan->AddItem((LPVOID &)sps,sizeof(LPTSTR),spsNumber);
	m_pArrayMan->m_pItemUpperBound[index] = spsNumber - 1;
	
	sps['+'] =_T("plus");
	sps['-'] =_T("minus");
	sps['*'] =_T("times");
	sps['/'] =_T("rdiv");
	sps['('] =_T("lparent");
	sps[')'] =_T("rparent");
	sps['='] =_T("eql");
	sps[','] =_T("comma");
	sps['['] =_T("lbrack");
	sps[']'] =_T("rbrack");
	sps['"'] =_T("neq");
	sps['!'] =_T("andsy");
	sps[';'] =_T("semicolon");

	enter("       ",_T("vvariable"),_T("notyp"),0);
	enter(_T("false"),_T("konstant"),_T("bools"),0);
	enter(_T("true"),_T("konstant"),_T("bools"),1);
	enter(_T("real"),_T("typel"),_T("reals"),1);
	enter(_T("char"),_T("typel"),_T("chars"),1);
	enter(_T("boolean"),_T("typel"),_T("bools"),1);
	enter(_T("integer"),_T("typel"),_T("ints"),1);
	enter(_T("string"),_T("typel"),_T("strs"),1);

	enter(_T("abs"),_T("function"),_T("reals"),0);
	enter(_T("sqr"),_T("function"),_T("reals"),2);
	enter(_T("odd"),_T("function"),_T("bools"),4);
	enter(_T("chr"),_T("function"),_T("chars"),5);
	enter(_T("ord"),_T("function"),_T("ints"),6);
	enter(_T("succ"),_T("function"),_T("chars"),7);
	enter(_T("pred"),_T("function"),_T("chars"),8);
	enter(_T("round"),_T("function"),_T("ints"),9);
	enter(_T("trunc"),_T("function"),_T("ints"),10);
	enter(_T("sin"),_T("function"),_T("reals"),11);
	enter(_T("cos"),_T("function"),_T("reals"),12);
	enter(_T("exp"),_T("function"),_T("reals"),13);
	enter(_T("ln"),_T("function"),_T("reals"),14);
	enter(_T("sqrt"),_T("function"),_T("reals"),15);
	enter(_T("arctan"),_T("function"),_T("reals"),16);

	enter(_T("asc"),_T("function"),_T("ints"),1001);
	enter(_T("char"),_T("function"),_T("strs"),1002);
	enter(_T("fill"),_T("function"),_T("strs"),1003);
	enter(_T("left"),_T("function"),_T("strs"),1004);
	enter(_T("lefttrim"),_T("function"),_T("strs"),1005);
	enter(_T("len"),_T("function"),_T("ints"),1006);
	enter(_T("lower"),_T("function"),_T("strs"),1007);
	enter(_T("match"),_T("function"),_T("bools"),1008);
	enter(_T("mid"),_T("function"),_T("strs"),1009);
	enter(_T("pos"),_T("function"),_T("ints"),1010);
	enter(_T("replace"),_T("function"),_T("strs"),1011);
	enter(_T("reverse"),_T("function"),_T("strs"),1012);
	enter(_T("right"),_T("function"),_T("strs"),1013);
	enter(_T("righttrim"),_T("function"),_T("strs"),1014);
	enter(_T("space"),_T("function"),_T("strs"),1015);
	enter(_T("trim"),_T("function"),_T("strs"),1016);
	enter(_T("upper"),_T("function"),_T("strs"),1017);

	return 1;
}

int CComplier::constdesc()
{
	insymbol();
	//test([indent],blockbegsys,2);  //检查标志附是否重复
	while( sy=="indent")
	{
		enter(snum,_T("konstant"),_T("notyp"),-1);
		insymbol();
		if(sy==_T("eql"))
			insymbol();
		else
		{
			error(_T("16"));
		}
	}
	return 1;
}
