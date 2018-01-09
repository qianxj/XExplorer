#include "stdafx.h"
#include "hxcomplier.h"
#include "stdio.h"
#include "math.h"
#include "xuXUtil.h"

CHxComplier::keyw	CHxComplier::constbegsys[] ={plus,minus,intcon,realcon,charcon,indent};
CHxComplier::keyw	CHxComplier::typebegsys[] ={indent,arraysy,recordsy};
CHxComplier::keyw	CHxComplier::blockbegsys[] ={constsy,typesy,varsy,procsy,funcsy,beginsy};
CHxComplier::keyw	CHxComplier::facbegsys[] ={intcon,realcon,charcon,indent,lparent,notsy,strcon};
CHxComplier::keyw	CHxComplier::statbegsys[] ={beginsy,ifsy,whilesy,repeatsy,forsy,casesy};
CHxComplier::keyw	CHxComplier::stantyps[] ={notyp,ints,reals,chars,strs};

CHxComplier::keyw CHxComplier::symbol[]={intcon,realcon,charcon,stringcon,notsy,
		plus,minus,times,idiv,imod,
		andsy,orsy,eql,neq,gtr,lss,leq,
		lparent,rparent,lbrack,rbrack,comma,semicolon,period,colon,becomes,
		constsy,typesy,varsy,funcsy,
		procsy,arraysy,recordsy,programsy,indent,
		beginsy,ifsy,casesy,repeatsy,whilesy,forsy,
		endsy,elsesy,untilsy,ofsy,dosy,tosy,downtosy,thensy};

LPTSTR  CHxComplier::key[]={_T("and"),_T("array"),_T("begin"),_T("case"),_T("const"),_T("div"),_T("do"),_T("downto"),_T("else"),_T("end"),_T("for"),_T("function"),
		_T("if"),_T("mod"),_T("not"),_T("of"),_T("or"),_T("procedure"),_T("program"),_T("record"),_T("repeat"),_T("then"),_T("to"),_T("type"),
		_T("until"),_T("var"),_T("while")};

CHxComplier::keyw CHxComplier::ksy[]={andsy,arraysy,beginsy,casesy,constsy,divsy,dosy,downtosy,elsesy,endsy,forsy,functionsy,
		ifsy,modsy,notsy,ofsy,orsy,proceduresy,programsy,recordsy,repeatsy,thensy,tosy,typesy,
		untilsy,varsy,whilesy};

CHxComplier::keyw CHxComplier::objs[]={konstant,vvariable,typel,procedure,function};
//int CHxComplier::types[]={notyp,ints,reals,bools,chars,arrays,records};
//int	CHxComplier::typsy[]={notyp,ints,reals,chars,bools,array,record};
CHxComplier::keyw	CHxComplier::codesy[]={cnv,fct,eql,neq,gtr,lss,leq,neq};

LPTSTR CHxComplier::mne[]={_T("LDA"),_T("LOD"),_T("LDI"),_T("DIS"),_T("   "),_T("   "),_T("   "),_T("   "),_T("FCT"),_T("INT"),_T("JMP"),_T("JPC"),_T("SWT"),_T("CAS"),
			 _T("F1U"),_T("F2U"),_T("F1D"),_T("F2D"),_T("MKS"),_T("CAL"),_T("IDX"),_T("IXX"),_T("LDB"),_T("CPB"),_T("LDC"),_T("LDR"),_T("FLT"),_T("RED"),
			 _T("WRS"),_T("WRW"),_T("WRU"),_T("HLT"),_T("EXP"),_T("EXF"),_T("LDT"),_T("NOT"),_T("MUS"),_T("WRR"),_T("STO"),_T("EQR"),_T("NER"),_T("LSR"),
			 _T("LER"),_T("GTR"),_T("GER"),_T("EQL"),_T("NEQ"),_T("LSS"),_T("LEQ"),_T("GRT"),_T("GEQ"),_T("ORR"),_T("ADD"),_T("SUB"),_T("ADR"),_T("SUR"),
			 _T("AND"),_T("MUL"),_T("DIV"),_T("MOD"),_T("MUR"),_T("DIR"),_T("RDL"),_T("WRL"),_T("EQC"),_T("NEC"),_T("LSC"),_T("LEC"),_T("GRC"),_T("GEC"),
			 _T("LDS"),_T("ADC"),_T("FLC"),_T("VAL"),_T("ROW")};

#define STRINIT  isnum=-1; \
				if(snumex) \
				{	\
                    snum[0]='\0';\
					delete snumex;\
					snumex=NULL ; \
                }	
#define STREXPAND(c)  isnum++; \
					 if(isnum < 255 )\
					  {					\
							snum[isnum]=c;	\
							snum[isnum + 1]='\0'; \
					   }else if(isnum<4096) \
					   {					\
                           snumex = new TCHAR[4097];  \
						   ::_tcscpy_s(snumex,isnum+1,snum); \
							snumex[isnum]=c;	\
							snumex[isnum + 1]='\0'; \
						}else \
						{     \
						   if(isnum % 4096==0) \
                           {                     \
                               LPTSTR ts = new TCHAR[4096 * ((isnum>>16) +1) +1]; \
                               _tcscpy_s(ts, isnum+1,snumex);\
							   delete snumex; \
							   snumex = ts; \
							   snumex[isnum]=c;	\
							   snumex[isnum + 1]='\0'; \
                           }                     \
                        }

LPTSTR LeftTrim(LPTSTR pBuf)
{
	int nPos = 0;
	int nIndex = 0;
	while(pBuf[nPos]=='\t'||pBuf[nPos]=='\r'||pBuf[nPos]=='\n'||pBuf[nPos]==' ')nPos++;
	while(pBuf[nPos] != '\0')
		pBuf[nIndex++] = pBuf[nPos++];
	pBuf[nIndex]='\0';
	return pBuf;
}

LPTSTR MakeReverse(LPTSTR pBuf)
{
	int nLen = (int)_tcslen(pBuf);
	int nPos = nLen;
	while(nPos>=1)
		pBuf[nPos] = pBuf[nPos -1];

	while(nPos < nLen)
	{
		pBuf[nPos] = pBuf[nLen - nPos];
		nPos++;
	}
	pBuf[nLen]='\0';
	return pBuf;
}

LPTSTR Right(LPTSTR pBuf,int nLen)
{
	int Len = (int)_tcslen(pBuf);
	if(Len <= nLen) return pBuf;
	
	int nIndex = 0;
	int nPos = Len - nLen;
	while(nIndex < nLen)
	{
		pBuf[nIndex] = pBuf[nIndex + Len - nLen];
		nIndex++;
	}
	pBuf[nIndex]='\0';

	return pBuf;
}
LPTSTR RightTrim(LPTSTR pBuf)
{
	int Len = (int)_tcslen(pBuf);
	int nPos = Len;
	while(nPos >=0 && pBuf[nPos]=='\t' || pBuf[nPos]=='\r' ||
		pBuf[nPos]=='\n' || pBuf[nPos]==' ')nPos--;
	return pBuf;
}

CHxComplier::CHxComplier():FRow(0)
{
	snumex = NULL;
	psconst = NULL;
	sconst = NULL;
	rconst = NULL; 
	codes = NULL;
	tab = NULL;

	m_pGrid = NULL;

	psconst = new TCHAR[SCCONSTINIT];
	sconst = new LPTSTR[255];

	rconst = new float[REALCONSTINIT];
	codes = new order[CODESINIT];

	tabln = 0;
	tab = new tagtab[TABLESINIT];

	construct();
	tablni = tabln;
}

CHxComplier::~CHxComplier()
{
	if(snumex) delete snumex; 
	if(psconst)delete psconst;
	if(sconst)delete[] sconst;
	if(rconst)delete rconst; 
	if(codes)delete codes;
	if(tab)delete tab;
}
// 设置源代码
int CHxComplier::SetSource(LPTSTR szSource)
{
	source = szSource;
	sourcelen =(int)::_tcslen(source);
	irow = 1;
	cc = -1;
	ch='\0';

	if(snumex) delete snumex; 
	snumex = NULL;
	
	//if(psconst)delete psconst;
	//if(sconst)delete[] sconst;

	sc2 = -1;

	//psconst = new TCHAR[SCCONSTINIT];
	//sconst = new LPTSTR[255];
	isconstpos = 0;

	//if(rconst)delete rconst; 
	//rconst = new float[REALCONSTINIT];
	c2=-1;

	//if(codes)delete codes;
	//codes = new order[CODESINIT];
	codesln = 0;

	//if(tab)delete tab;
	//tab = new tagtab[TABLESINIT];
	//tabln = 0;
	tabln = tablni;

	//construct();

	sy = undef;

	return 0;
}

//get next char
int CHxComplier::nextch(void)
{
	if(cc == sourcelen)
		ch='\0';
	else
	{
		cc++;
		ch=source[cc];
		if(ch=='\r')irow++;
	}
	return 0;
}


// read float scale
int CHxComplier::readscale(void)
{
	int sign = 1;
	int s = 0;
	nextch();
	if(ch=='+')
	{
		sign = 1;
		nextch();
	}
	else if(ch=='-')
	{
		sign = -1;
		nextch();
	}
	if(ch <'0'|| ch >'9')
		error(_T("readscale"));
	do
	{
		s = s*10 + ch -'0';
	}while(ch>='0' && ch<='9');
		
	return sign*s;;
}

// error information
int CHxComplier::error(LPTSTR)
{
	return 0;
}

// adjust real scale
int CHxComplier::adjustscale(int e)
{
	int s;
	int i;
	s= e>0?e:-e;
	if(e>0)
		for(i=1;i<=s;i++)
			rnum *= 10;
	else
		for(i=1;i<=s;i++)
			rnum /= 10;

	return 0;
}
int CHxComplier::adjustscale(float &rnum, int e)
{
	int s;
	int i;
	s= e>0?e:-e;
	if(e>0)
		for(i=1;i<=s;i++)
			rnum *= 10;
	else
		for(i=1;i<=s;i++)
			rnum /= 10;

	return 0;
}

// word symbol
int CHxComplier::insymbol(void)
{
	int j;
	int i;
	int k;

	sy = undef;
	snum[0] = '\0';

	STRINIT;

	rnum = 0;
	inum = 0;

L1:
	//space char
	while(ch==' ' ||ch=='\t' || ch=='\r'||ch=='\n')
		nextch();

	//end source
	if(ch=='\0')
	{
		sy=undef;
		return 0;
	}
	
	//key or indent
	if((ch>='a' && ch<='z')||(ch>='A' && ch<='Z')||ch=='$' || ch>255)
	{
		snum[0]='\0';
		do
		{
			STREXPAND(ch);
			nextch();
		}while(ch =='_' || ch > 255  || (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9')||ch=='$');
		_tcslwr_s(snum);
		
		i=1;
		j=sizeof(key)/sizeof(key[0]);
		do
		{
			k=(i+j)/2;
			if(_tcscmp(snum,key[k - 1])<=0)j=k - 1;
			if(_tcscmp(snum,key[k - 1])>=0) i=k + 1;
		}while( i<=j);
		if(i - 1> j)
			sy = ksy[k - 1];
		else
			sy=indent;
	}
	//int or real const 
	else if (ch>='0' && ch<='9')
	{
		inum = 0;
		sy=intcon;
		do
		{
			inum  = inum *10 + ch - '0';
			nextch();
		}while(ch>='0' && ch<='9');
		if(ch=='.')
		{
			int e;
			nextch();
			if(ch=='.')
				ch=':';
			else
			{
				sy = realcon;
				rnum = (float)inum;
				e=0;
				while (ch>='0' && ch<='9')
				{
					e= e - 1;
					rnum = rnum*10 + ch - '0';
					nextch();
				}
				if(e==0) error(_T("decimal"));
				if(ch=='e' ||ch=='E')e= readscale();
				if(e !=0)adjustscale(e);
			}
			if(ch=='e'||ch=='E')
			{
				sy=realcon;
				rnum = (float)inum;
				e = 0;
				e = readscale();
				if(e != 0)adjustscale(e);
			}
		}
	}else if(ch==':')
	{
		nextch();
		if(ch=='=')
		{
			sy=becomes;
			nextch();
		}else
			sy=colon;
	}else if(ch=='<')
	{
		nextch();
		if(ch=='=')
		{
			sy=leq;
			nextch();
		}else if(ch=='>')
		{
			sy=neq;
			nextch();
		}else
			sy=lss;
	}else if(ch=='>')
	{
		nextch();
		if(ch=='=')
		{
			sy=geq;
			nextch();
		}else
			sy=gtr;
	}else if(ch=='.')
	{
		nextch();
		if(ch=='.')
		{
			sy=colon;
			nextch();
		}else
			sy=period;
	}
	else if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='(' || ch==')' || ch=='[' || ch==']'
		|| ch=='=' || ch==';' || ch==',')  
	{
		sy = sps[ch];
		nextch();
	}
	//string
	else if(ch=='"')
	{
		sy=strcon;
		STRINIT;
L2:		nextch();
		while(!(ch=='\\' || ch=='\0' || ch=='"'))
		{
			STREXPAND(ch);
			nextch();
		}
		if(ch=='\0')
		{
			error(_T("string"));
			nextch();
		}else if(ch=='\\')
		{
			nextch();
			if(ch=='\'')
			{
				STREXPAND('\'');
			}
			else if( ch=='"')
			{
				STREXPAND('\"');
			}
			else if(ch=='r')
			{
				STREXPAND('\r');
			}
			else if(ch=='n')
			{
				STREXPAND('\n');
			}
			else if(ch=='t')
			{
				STREXPAND('\t');
			}
			else
			{
				STREXPAND('\\');
			}
			goto L2;
		}
		nextch();
	}
	//sring
	else if(ch=='\'')
	{
		sy=strcon;
		STRINIT;
L3:		nextch();
		while(!(ch=='\\' || ch=='\0' || ch=='\''))
		{
			STREXPAND(ch);
			nextch();
		}
		if(ch=='\0')
		{
			error(_T("string"));
			nextch();
		}else if(ch=='\\')
		{
			nextch();
			if(ch=='\'')
			{
				STREXPAND('\'');
			}
			else if( ch=='"')
			{
				STREXPAND('\"');
			}
			else if(ch=='r')
			{
				STREXPAND('\r');
			}
			else if(ch=='n')
			{
				STREXPAND('\n');
			}
			else if(ch=='t')
			{
				STREXPAND('\t');
			}
			else
			{
				STREXPAND('\\');
			}
			goto L3;
		}
		nextch();
	}
	//comment
	else if(ch=='/')
	{
		nextch();
		if(ch=='/')
		{
		   do
		   {
			   nextch();
		   }while(ch!='\r' && ch!='\0');
		   if(ch!='\0')
		   {
				nextch();
				goto L1;
		   }
		}else if(ch=='*')
		{
L4:			do 
			{ 
				nextch();
			}while(ch!='*' && ch!='\0');
		   if(ch!='\0')
		   {
				nextch();
				if(ch!='/')goto L4;
				goto L1;
		   }
		}
	}else
	{
		error(_T("24"));
		nextch();
		goto L1;
	}
	return 0;
}

int CHxComplier::expression(keyw* & fsys, item & x)
{
	int syprev;
	item y;
	keyw headers[]={eql,neq,gtr,lss,leq,neq};
	simplesexpression(fsys,x);

	syprev = sy;
	if(in( headers,sizeof(headers)/sizeof(headers[0]),sy) < 0) return 1;
	insymbol();
	simplesexpression(fsys,y);
	
	if((x.typ==notyp||x.typ==ints||x.typ==bools||x.typ==chars) && x.typ == y.typ)
	{
		if(syprev==eql)
			emit(45);
		else if(syprev==neq)
			emit(46);
		else if(syprev==lss)
			emit(47);
		else if(syprev==leq)
			emit(48);
		else if(syprev==gtr)
			emit(49);
		else if(syprev==geq)
			emit(50);
	}else if(x.typ==ints)
	{
		x.typ=reals;
		emit1(26,1);
	}else if(y.typ==ints)
	{
		y.typ=reals;
		emit1(26,0);
	}
	if(x.typ==reals && y.typ== reals)
	{
		if(syprev== eql)
			emit(45);
		else if(syprev==neq)
			emit(46);
		else if(syprev== lss)
			emit(47);
		else if(syprev== leq)
			emit(48);
		else if(syprev== gtr)
			emit(49);
		else if(syprev== geq)
			emit(50);
	}
	x.typ = bools;

	return 1;
}

int CHxComplier::simplesexpression(keyw* & fsys, item & x)
{
	keyw	op;
	item 		y;

	if(sy==plus || sy==minus)
	{
		op = sy;
		insymbol();
		term(fsys,x);
		if(x.typ > reals)              //??
			error(_T("simplessExpression 1"));
		else
		{
			if(op == minus)
				emit(36);
		}
	}else
		term(fsys,x);

	while(sy== plus  || sy== minus || sy== orsy)
	{
		op = sy;
		insymbol();
		term(fsys,y);
		if(op== orsy)
		{
			if(x.typ== bools &&  y.typ== bools)
				emit(51);
			else
			{
				if (x.typ != notyp && x.typ != notyp)
				{
					error(_T("simplessExpression 2"));
					x.typ = notyp;
				}
			}
		}else
		{
			x.typ = resulttype(x.typ,y.typ);
			if(x.typ== strs)
				emit(71);
			else if(x.typ== ints )
			{
				if(op == plus)
					emit(52); //ADD
				else
					emit(53); //MINUS
			}else if(x.typ==reals)
			{
				if(op == plus)
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

int CHxComplier::term(keyw* & fsys, item & x)
{
	keyw	op;
	item	y;

	factor(fsys,x);
	while(sy==times||sy==rdiv||sy==idiv||sy==imod||sy==andsy)  
	{
		op = sy;
		insymbol();
		factor(fsys,y);

		if(op == rdiv && x.typ == ints && y.typ== ints ) op = idiv ;

		if(op == times)
		{
			x.typ =resulttype(x.typ,y.typ);
			if(x.typ== ints) 
				emit(57); //MUL
			else if(x.typ==reals || x.typ==strs)
				emit(60); //MUR

		}else if(op== rdiv)
		{
			if(x.typ= ints)
			{
				emit1(26,1);
				x.typ= reals;
			}
			if(y.typ=ints)
			{
				emit1(26,0);
				y.typ=reals;
			}
			if(x.typ==reals && y.typ==reals)
				emit(61);
			else
			{
				if(x.typ !=notyp && y.typ !=notyp)
				{
					error(_T("term 1"));
					x.typ=notyp;
				}
			}
		}else if(op==andsy)
		{
			if(x.typ==bools && y.typ ==bools) 
				emit(56);
			else
			{
				if(x.typ !=notyp &&  y.typ !=notyp)
				{
					error(_T("term 2"));
					x.typ = notyp;
				}
			}
		}else
		{
			if(x.typ ==ints && y.typ==ints)
			{
				if(op==idiv)
					emit(58);
				else
					emit(59);
			}else
			{
				if(x.typ !=notyp &&  y.typ !=notyp)
				{
					error(_T("term 3"));
					x.typ = notyp;
				}
			}
		}
	}
	return 1;
}

int CHxComplier::factor(keyw* & fsys, item & x)
{
	int i,f;

	x.typ = notyp;
	x.refe = 0; 

	while(in(facbegsys,sizeof(facbegsys)/sizeof(facbegsys[0]),sy) >=0)
	{
		if(sy== indent)
		{
			i=loc(snum);
			if(i< 0)
			{
				int index = enterstr(snum);
				TCHAR pStr[255];
				_tcscpy_s(pStr,255,snum);
				insymbol();
				if(sy==colon)
				{
					insymbol();
					int pos = 0;

					TCHAR buf[40];
					_tcscpy_s(buf,40,sconst[index]);
					pos = (int)::_tcslen(buf);
					buf[pos++]=':';
					_tcscpy_s(buf + pos,40 - pos - 1,snum);
					emit1(70,enterstr(buf));
					x.typ =strs;
					insymbol();

				}else
				{
					int ll_index,ll_colid;
					LPTSTR ls_type;
					i = getdsindex(pStr,ll_index,ll_colid,ls_type);
					if(i < 0)
					{
						x.typ = GetCellType(sconst[index]);
						emit1(80,index);
						//insymbol();
						
						return 1;
					}else
					{
						//emit1(74,ll_index);	//getRow
						emit2(73,ll_index,ll_colid);	//getValue
						x.typ = strs; //reals;//notyp; //reals;// ls_type;

						//insymbol();
						return 1;
					}
				}
			}
			insymbol();
			if(tab[i].obj== konstant)
			{
				x.typ = tab[i].typ;
				x.refe = -1;
				if(x.typ == reals)
					emit1(25,tab[i].adr);
				else
					emit1(24,tab[i].adr);
			}else if(tab[i].obj== vvariable)
			{
				x.typ = tab[i].typ;
				x.refe = tab[i].refe;
				if(sy== lbrack || sy== lparent ||sy== period )
				{
					if(tab[i].normal)
						f = 0;
					else
						f=1;
					emit2(f, tab[i].lev, tab[i].adr);
					//!			selector(fsys,x)
					if( in(stantyps,sizeof(stantyps)/sizeof(stantyps),x.typ) >=0)
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
			}else if(tab[i].obj== typel || tab[i].obj== procedure)
			{
				error(_T("factor 3"));
			}else if(tab[i].obj== function)
			{
				x.typ = tab[i].typ;
				if(tab[i].lev != 0)
				{
					//!			call(fsys,i)
				}
				else
					standfct(fsys,tab[i].adr,i,x);
			}
		}else if(sy==charcon||sy==intcon||sy==realcon||sy==strcon)
		{
			if(sy==realcon)
			{
				x.typ= reals;
				enterreal(rnum);
				emit1(25,c1);
			}else if(sy==strcon)
			{ 
				x.typ = strs;
				enterstr(snum);
				emit1(70,sc1);
			}else
			{
				if(sy== charcon)
					x.typ = chars;
				else
					x.typ = ints;
				emit1(24,inum); //LDC
			}
			x.refe = -1;
			insymbol();
		}else if(sy== lparent)
		{
			insymbol();
			expression(fsys,x);
			if(sy== rparent)
				insymbol();
			else
				error(_T("factor 4"));
		}else if(sy==notsy)
		{
			insymbol();
			factor(fsys,x);
			if (x.typ==bools)
				emit(35);
			else
			{
				if(x.typ == notyp)
					error(_T("factor 5"));
			}
			//!	test(fsys,facbegsys,6)
		}
	}
	return  1;
}

int CHxComplier::standfct(keyw* & fsys, int n, int i,item &x)
{
	keyw *	ts=NULL;
	item		y[10];
	int count =0;

	if( sy==lparent )
		insymbol();
	else
		error(_T("standfct lparent"));

	if( n < 17 )
	{
		//int index = this->m_pArrayMan->AddItemData((LPVOID &)fsys);
		//fsys[index]=rparent;
		expression(fsys,x);
		switch(n)
		{
		case 0: //abs
		case 2: //sqr
			{
				count = 2;
				ts = new keyw[2];
				ts[0] = ints;
				ts[1] = reals;
				tab[i].typ = x.typ;
				if(x.typ==reals)n++;
			}
			break;
		case 4: //odd
		case 5: //char
			{
				count = 1;
				ts = new keyw[1];
				ts[0] = ints;
			}
			break;
		case 6: //odr
			{
				count = 3;
				ts = new keyw[3];
				ts[0] = ints;
				ts[1] = bools;
				ts[2] = chars;
			}
			break;
		case 7: //succ
		case 8:
			{
				count = 3;
				ts = new keyw[3];
				ts[0] = ints;
				ts[1] = bools;
				ts[2] = chars;
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
				ts = new keyw[2];
				ts[0] = ints;
				ts[1] = reals;
				if(x.typ==ints)
				{
					//emit1(26,x.refe);
					emit1(26,0);
					x.refe =  c2 + 1;  //??
				}


			}
			break;
		}
		if(in(ts,count,x.typ) >= 0)
			emit1(8,n);
		else
			if( x.typ != notyp)error(_T("48"));
	}else if (n==17 ||n==18) //read ,write
	{
		if( sy != indent)
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
				if( x.typ != ints)
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
				if( x.typ != strs)
					error(_T("args should string"));
				else
					emit1(8,n);
			}
			break;
		case 1003: //fill,left,right
		case 1004:
		case 1013:
		case 1018:
			{
				if( x.typ != strs && x.typ !=notyp) 
				{
					error(_T("args1 should string"));
					break;
				}
				if(x.typ ==notyp)
				{
					emit1(72,0);
				}

				insymbol();
				expression(fsys,y[0]);
				if( y[0].typ !=ints && y[0].typ !=notyp )
					error(_T("args2 should integer"));
				if(y[0].typ ==notyp)
				{
					emit1(8,1002); //toint
				}
				emit1(8,n);
			}
			break;
		case 1008: //match
			{
				if( x.typ == strs)
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ != strs )
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
				if( x.typ == strs)
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ != ints )
						error(_T("arg2 should integer"));
					else
					{
						if( sy== comma)
						{
							insymbol();	//drop comma
							expression(fsys,y[1]);
							if( y[1].typ != ints)
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
				if( x.typ == strs)
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( y[0].typ !=strs)
						error(_T("arg2( should string"));
					else
					{
						if( sy=comma)
						{
							insymbol();	//drop comma
							expression(fsys,y[1]);
							if(y[1].typ != ints)
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
				if( x.typ == strs)
				{
					insymbol();	//drop comma
					expression(fsys,y[0]);
					if( x.typ !=ints)
						error(_T("arg2 should integer"));
					else
					{
						insymbol();	//drop comma
						expression(fsys,y[1]);
						if( y[1].typ !=ints)
							error(_T("arg3 should integer"));
						else
						{
							insymbol();	//drop comma
							expression(fsys,y[2]);
							if( y[2].typ !=strs)
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
		case 2001: //sum
		case 2002: //avg
		case 2003: //count
			emit1(8,n);
			break;
		case 3001: //getrow
		case 3002: //currentrow
		case 3003: //currentrow
			emit1(8,n);
		}
	}

	if(ts) delete[] ts;
	x.typ = tab[i].typ;
	if( sy == rparent)
		insymbol();
	else
	{
		error(_T("standfct rparent"));
		return 0;
	}
	return 1;
}


int CHxComplier::in(keyw*  fsys, int count, keyw val)
{
	for(int i=0;i<count;i++)
	{
		if(val == fsys[i])return i;
	}
	return -1;
}

int CHxComplier::emit(int fct)
{
	codes[codesln].f = fct;
	codes[codesln].x  = 0 ;
	codes[codesln].y  = 0 ;
	codesln++;

	return 0;
}

int CHxComplier::emit1(int fct, int b)
{
	codes[codesln].f = fct;
	codes[codesln].x  = 0 ;
	codes[codesln].y  = b ;
	codesln++;
	return 0;
}

int CHxComplier::emit2(int fct, int a, int b)
{
	codes[codesln].f = fct;
	codes[codesln].x  = a ;
	codes[codesln].y  = b ;
	codesln++;
	return 0;
}

CHxComplier::keyw CHxComplier::resulttype(keyw l1, keyw l2)
{
	keyw t1,t2;
	t1 = l1;
	t2 = l2;

	if(t1==t2) return l1;
	if((t1==arrays && t2 !=arrays) ||(t1!=arrays && t2 ==arrays)) return undef;
	if((t1==recorders && t2 !=recorders) ||(t1!=recorders && t2 ==recorders)) return undef;

	if(t1==notyp) return l2;
	if(t2==notyp) return l1;

	if(t2==reals &&  t1==ints)
	{
		emit1(26,1);
		return l2;
	}
	if(t1==reals &&  t2==ints)
	{
		emit1(26,0);
		return l1;
	}
	if(t1==strs &&  t2!=strs)
	{
		emit1(72,0);
		return l1;
	}
	if(t1!=strs &&  t2==strs)
	{
		emit1(72,1);
		return l2;
	}
	return chars;
}

int CHxComplier::loc(LPTSTR szName)
{
	for(int i=0;i<tabln;i++)
	{
		if(::_tcsicmp(tab[i].name,szName)==0) return i;
	}
	return -1;
}

int CHxComplier::enterstr(LPTSTR x)
{
	sc2++;
	sc1 = sc2;
	
	if(x==NULL)
	{
		sconst[sc2] = NULL;
		isconstpos += 1;
	}else 
	{
		int len = (int)_tcslen(x);
		_tcscpy_s(psconst + isconstpos,len + 1,x);
		sconst[sc2] = psconst + isconstpos;
		isconstpos += len + 1;
	}
	return sc2;
}

int CHxComplier::enterreal(float x)
{
	rconst[c2+1] = x;          //??
	c1 = 0;

	while( rconst[c1] != x )
		c1++;

	if(c1>c2 ) c2= c1;
	return c2;
}

int CHxComplier::construct(void)
{
	sps['+'] =plus;
	sps['-'] =minus;
	sps['*'] =times;
	sps['/'] =rdiv;
	sps['('] =lparent;
	sps[')'] =rparent;
	sps['='] =eql;
	sps[','] =comma;
	sps['['] =lbrack;
	sps[']'] =rbrack;
	sps['"'] =neq;
	sps['!'] =andsy;
	sps[';'] =semicolon;

	enter(_T("       "),vvariable,notyp,0);
	enter(_T("false"),konstant,bools,0);
	enter(_T("true"),konstant,bools,1);
	enter(_T("real"),typel,reals,1);
	enter(_T("char"),typel,chars,1);
	enter(_T("boolean"),typel,bools,1);
	enter(_T("integer"),typel,ints,1);
	enter(_T("string"),typel,strs,1);

	enter(_T("abs"),function,reals,0);
	enter(_T("sqr"),function,reals,2);
	enter(_T("odd"),function,bools,4);
	enter(_T("chr"),function,chars,5);
	enter(_T("ord"),function,ints,6);
	enter(_T("succ"),function,chars,7);
	enter(_T("pred"),function,chars,8);
	enter(_T("round"),function,ints,9);
	enter(_T("trunc"),function,ints,10);
	enter(_T("sin"),function,reals,11);
	enter(_T("cos"),function,reals,12);
	enter(_T("exp"),function,reals,13);
	enter(_T("ln"),function,reals,14);
	enter(_T("sqrt"),function,reals,15);
	enter(_T("arctan"),function,reals,16);

	enter(_T("asc"),function,ints,1001);
	enter(_T("char"),function,strs,1002);
	enter(_T("fill"),function,strs,1003);
	enter(_T("left"),function,strs,1004);
	enter(_T("lefttrim"),function,strs,1005);
	enter(_T("len"),function,ints,1006);
	enter(_T("lower"),function,strs,1007);
	enter(_T("match"),function,bools,1008);
	enter(_T("mid"),function,strs,1009);
	enter(_T("pos"),function,ints,1010);
	enter(_T("replace"),function,strs,1011);
	enter(_T("reverse"),function,strs,1012);
	enter(_T("right"),function,strs,1013);
	enter(_T("righttrim"),function,strs,1014);
	enter(_T("space"),function,strs,1015);
	enter(_T("trim"),function,strs,1016);
	enter(_T("upper"),function,strs,1017);
	enter(_T("sround"),function,strs,1018);

	enter(_T("sum"),function,reals,2001);
	enter(_T("avg"),function,reals,2002);
	enter(_T("count"),function,ints,2003);

	enter(_T("getrow"),function,ints,3001);
	enter(_T("currentrow"),function,ints,3002);
	enter(_T("rowcount"),function,ints,3003);

	return 0;
}

int CHxComplier::enter(LPTSTR x0, keyw x1, keyw x2, short x3)
{
	int t;
	t = tabln;
	tabln++;
	tab[t].name = x0;
	tab[t].link = t - 1;
	tab[t].obj  = x1;
	tab[t].typ  = x2;
	tab[t].refe = 0;
	tab[t].normal = true;
	tab[t].lev = 0;
	tab[t].adr = x3;
	return 0;
}

int CHxComplier::of_calcexpression(LPTSTR as_source,var &val)
{
	SetSource(as_source);
	nextch();
	insymbol();

	keyw *	fsys = NULL;
	item		x;

	expression((keyw *&)fsys,x);
	emit(31);	//quit

	/*
	for(int i=0;i<codesln;i++)
	{
		TRACE("%7i%12s%7i%7i\r\n",i,mne[codes[i].f],codes[i].x,codes[i].y);
	}
	*/
	interpret(val);

	return 0;
}

int CHxComplier::interpret(var & value)
{
	order	ir;
	int	pc,t,b,h1,h2,h3,h4;
	int	lncnt,ocnt,/*blkcnt,*/chrcnt;

	std::wstring	ps;
	int		fld[100];	//fld[]
	int		disp[100];  //disp[]

	var*	s = new var[STACKMAX];

	s[1].i = 0;
	s[1].sv_type = ints;
	s[2].i = 0;
	s[2].sv_type = ints;
	s[3].i = 0;
	s[3].sv_type = ints;
	//s[4] = btab[1].lastadr
	s[4].i = 0;
	s[4].sv_type = ints;

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
		ir = codes[pc];
		pc = pc+1;
		ocnt = ocnt + 1;

		switch(ir.f)
		{
		case 0:	//load address
			t++;
			s[t].i = disp[ir.x] + ir.y;
			s[t].sv_type = ints;
			break;
		case 1:	//load value
			t++;
			s[t].i = s[disp[ir.x] + ir.y].i;
			s[t].sv_type = ints;
			break;
		case 2:	//load indirect
			t++;
			s[t].i = s[s[disp[ir.x] + ir.y].i].i;
			s[t].sv_type = ints;
			break;
		case 3:	//update display
			h1 = ir.y;
			h2 = ir.x;
			h3 = b;
			do
			{
				disp[h1] = h3;
				h1 = h1 - 1;
				h3 = s[h3+2].i;
			}while(h2!=h1);
			break;
		case 8: //fucntion
			switch(ir.y)
			{
			case 0: 
				s[t].i = abs(s[t].i);
				s[t].sv_type = ints;
				break;
			case 1: 
				s[t].f = abs(s[t].f);
				s[t].sv_type = reals;
				break;
			case 2:
				s[t].i = (int)sqrt((float)s[t].i);
				s[t].sv_type = ints;
				break;
			case 3:
				s[t].f = sqrt(s[t].f);
				s[t].sv_type = reals;
				break;
			case 4:
				//s[t] = odd(s[t].i);
				break;
			case 5:
				s[t].i = s[t].ch;   //chr
				s[t].sv_type = ints;
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
				s[t].sv_type = reals;
				break;
			case 12:
				s[t].f = cos(s[t].f);
				s[t].sv_type = reals;
				break;
			case 13:
				s[t].f = exp(s[t].f);
				s[t].sv_type = reals;
				break;
			case 14:
				//s[t] = ln(s[t].f);
				break;
			case 15:
				s[t].f = sqrt(s[t].f);
				s[t].sv_type = reals;
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
				{
					t = t - 1;
					int len;
					int cssize;
					len = s[t+1].i * (int)::_tcslen(s[t].m_pStr);
					if(len < DEFSTRSIZE)
						cssize = DEFSTRSIZE;
					else
						cssize = len + 1;
					LPTSTR  pStr;
					pStr = new TCHAR[cssize];
					pStr[0]='\0';
					for(int i=0;i<s[t+1].i;i++)
					{
						::_tcscpy_s(pStr + i* ::_tcslen(s[t].m_pStr),cssize,s[t].m_pStr);
					}
					if(s[t].bStrNeedDeleted)delete s[t].m_pStr;
					s[t].m_pStr = pStr;
					s[t].bStrNeedDeleted = true;
				}
				break;
			case 1004:  //left
				{
					t = t - 1;
					if(!s[t].bStrNeedDeleted)
					{
						LPTSTR  pStr;
						pStr = new TCHAR[s[t].StrSize];
						::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
						s[t].m_pStr = pStr;
					}
					int len = s[t].m_pStr?(int)::_tcslen(s[t].m_pStr):0;
					s[t].m_pStr[s[t+1].i> len?len:s[t+1].i]='\0';
					s[t].bStrNeedDeleted = true;
				}
				break;
			case 1018:  //sround
				{
					t = t - 1;
					bool bdelete = s[t].bStrNeedDeleted;
					int n = s[t+1].i;
					s[t].m_pStr = Hxsoft::XUtil::Round(s[t].m_pStr,3,bdelete);
					s[t].bStrNeedDeleted = bdelete;
				}
				break;
			case 1005: //trimleft...
				{
					if(!s[t].bStrNeedDeleted)
					{
						LPTSTR  pStr;
						pStr = new TCHAR[s[t].StrSize];
						::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
						s[t].m_pStr = pStr;
					}
					LeftTrim(s[t].m_pStr);
					s[t].bStrNeedDeleted = true;
				}
				break;

			case 1006: //len...
				s[t].i = s[t].m_pStr?(int)::_tcslen(s[t].m_pStr):0;
				s[t].sv_type = ints;
				if(s[t].bStrNeedDeleted)delete s[t].m_pStr;
				break;

			case 1007:  //lower...
				{
					if(s[t].m_pStr)
					{
						if(!s[t].bStrNeedDeleted)
						{
							LPTSTR  pStr;
							pStr = new TCHAR[s[t].StrSize];
							::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
							s[t].m_pStr = pStr;
						}
						::_tcslwr_s(s[t].m_pStr,s[t].StrSize);
						s[t].bStrNeedDeleted = true;
					}
				}
				break;
			case 1008: //match
				//s[t] = match(s[t],s[t+1]);
				s[t].b = false; //未实现
				break;
			case 1009: //mid...
				{
					t = t - 2;
					if(s[t+2].i == 0)s[t+2].i = s[t].m_pStr?(int)::_tcslen(s[t].m_pStr) - s[t+1].i:0;
					if(s[t+2].i+s[t+1].i > (int)::_tcslen(s[t].m_pStr))s[t+2].i= (int)::_tcslen(s[t].m_pStr) - s[t+1].i;
					if(s[t].m_pStr)
					{
						if(s[t+1].i >= (int)::_tcslen(s[t].m_pStr))
						{
							if(s[t].bStrNeedDeleted)
							{
								s[t].m_pStr[0]='\0';
							}else
							{
								s[t].m_pStr = new TCHAR[DEFSTRSIZE];
								s[t].m_pStr[0]='\0';
							}
							s[t].bStrNeedDeleted = true;
						}
						else
						{
							LPTSTR  pStr;
							pStr = new TCHAR[s[t].StrSize];
							::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr + s[t+1].i);
							if(s[t].bStrNeedDeleted)delete s[t].m_pStr;
							s[t].m_pStr = pStr;
							s[t].m_pStr[s[t+2].i]='\0';
							s[t].bStrNeedDeleted = true;
						}
					}
				}
				break;
			case 1010:	//pos...
				{
					t = t - 2;
					LPTSTR pStr = _tcsstr(s[t].m_pStr + s[t+2].i,s[t+1].m_pStr);
					if(!pStr)
						s[t].i = -1;
					else
						s[t].i = (int)(pStr - s[t].m_pStr) + s[t+2].i;
					s[t].sv_type = ints;
					if(s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
					if(s[t].bStrNeedDeleted)delete s[t].m_pStr;
				}
				break;
			case 1011: //replace
				{
					t = t - 3;
					//s[t] = replace(s[t],s[t+1],s[t+2],s[t+3]); //未实现
				}
				break;
			case 1012: //reverse...
				{
					if(s[t].m_pStr)
					{
						if(!s[t].bStrNeedDeleted)
						{
							LPTSTR  pStr;
							pStr = new TCHAR[s[t].StrSize];
							::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
							s[t].m_pStr = pStr;
						}
						MakeReverse(s[t].m_pStr);
						s[t].bStrNeedDeleted = true;
					}
					break;
				}
			case 1013: //right...
				{
					t = t -1;
					if(!s[t].bStrNeedDeleted)
					{
						LPTSTR  pStr;
						pStr = new TCHAR[s[t].StrSize];
						::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
						s[t].m_pStr = pStr;
					}
					Right(s[t].m_pStr,s[t+1].i);
					s[t].bStrNeedDeleted = true;
				}
				break;
			case 1014: //righttrim...
				{
					if(!s[t].bStrNeedDeleted)
					{
						LPTSTR  pStr;
						pStr = new TCHAR[s[t].StrSize];
						::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
						s[t].m_pStr = pStr;
					}
					RightTrim(s[t].m_pStr);
					s[t].bStrNeedDeleted = true;
				}
				break; 
			case 1015: //space...
				{
					TCHAR*  pBuf = new TCHAR[s[t].i + 1];
					pBuf[s[t].i] ='\0';
					_tcsset_s(pBuf,s[t].i + 1,' ');
					s[t].m_pStr=pBuf;
					s[t].sv_type=strs;
					s[t].bStrNeedDeleted = true;
				}
				break;
			case 1016: //trim...
				{
					if(!s[t].bStrNeedDeleted)
					{
						LPTSTR  pStr;
						pStr = new TCHAR[s[t].StrSize];
						::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
						s[t].m_pStr = pStr;
					}
					LeftTrim(s[t].m_pStr);
					RightTrim(s[t].m_pStr);
					s[t].bStrNeedDeleted = true;
				}
				break;
			case 1017: //upper...
				{
					if(s[t].m_pStr)
					{
						if(!s[t].bStrNeedDeleted)
						{
							LPTSTR  pStr;
							pStr = new TCHAR[s[t].StrSize];
							::_tcscpy_s(pStr,s[t].StrSize,s[t].m_pStr);
							s[t].m_pStr = pStr;
						}
						::_tcsupr_s(s[t].m_pStr,s[t].StrSize);
						s[t].bStrNeedDeleted = true;
					}
				}
				break;
			case 3001: //getrow()
				{
					t = t + 1;
					s[t].sv_type = ints;
					s[t].i = -1;
					 CDwTable * pDwTable = m_pGrid->GetGridDataSvr()->GetDwTableFromCell(FCol,FRow);
					 LPTSTR pStr = NULL;
					 int nRow,nCol;
					 if(pDwTable)
					 {
						pDwTable->getRowColumnFromCell(FCol,FRow,nRow,nCol);
						s[t].i = nRow;
						break;
					 }
					 break;
				}
			case 3002: //currentrow()
				{
					t = t + 1;
					s[t].sv_type = ints;
					s[t].i = -1;
					 CDwTable * pDwTable = m_pGrid->GetGridDataSvr()->GetDwTableFromCell(FCol,FRow);
					 LPTSTR pStr = NULL;
					 //int nRow,nCol;
					 if(pDwTable)
					 {
						s[t].i = pDwTable->GetRow();
						break;
					 }
					 break;
				}
			case 3003: //rowcont()
				{
					t = t + 1;
					s[t].sv_type = ints;
					s[t].i = -1;
					 CDwTable * pDwTable = m_pGrid->GetGridDataSvr()->GetDwTableFromCell(FCol,FRow);
					 LPTSTR pStr = NULL;
					 //int nRow,nCol;
					 if(pDwTable)
					 {
						s[t].i = pDwTable->GetRowCount();
						break;
					 }
					 break;
				}
			case 2001: //sum
				{
					if(s[t].m_pStr)
					{
						LPTSTR p1,p2,p3;
						LPTSTR pStr;
						keyw typ;
						keyw styp = ints;
						int	ival;
						float rval;
						int sival = 0;
						float srval = 0;

						p1 = s[t].m_pStr;
						if(p1[0]=='=')
						{
							CHxComplier * pComplier = new CHxComplier();
							pComplier->m_pGrid = this->m_pGrid;
							pComplier->FRow = this->FRow;
							pComplier->FCol = this->FCol;
							var val;
							val.sv_type=undef;
							pComplier->of_calcexpression(p1+1,val);
							if(pComplier) delete pComplier;
							/*if(val.sv_type==reals)
							{
								if(styp ==ints)
								{
									srval = (float)sival;
									styp = reals;
								}
								srval += val.f;
							}
							if(val.sv_type==ints)
							{
								if(styp ==ints)
									sival += val.i;
								else
									srval += val.i;

							}*/
							if(s[t].bStrNeedDeleted)delete s[t].m_pStr;
							s[t].m_pStr = 0;
							s[t].bStrNeedDeleted = false;
							if(val.sv_type==strs) 
							{
								s[t].m_pStr = val.m_pStr;
								if(val.bStrNeedDeleted && val.m_pStr)
								{
									s[t].bStrNeedDeleted = true;
								}
							}else
							{
								s[t].m_pStr = val.toString();
								s[t].bStrNeedDeleted = true;
							}
							p1 = s[t].m_pStr;
						}
						while(p1)
						{
							p2 = ::_tcschr(p1,',');
							if(p2)p2[0]='\0';
							if(isNumber(p1,typ,ival,rval))
							{
								if(typ ==reals)
								{
									if(styp ==ints)
									{
										srval = (float)sival;
										styp = reals;
									}
									srval += rval;
								}else
								{
									sival += ival;
								}
							}else
							{
								if(m_pGrid)
								{
									p3 = ::_tcschr(p1,':');
									int srow,scol,erow,ecol,trow,tcol;
									bool	browcol;
									bool	bcalc = false;
									if(p3)
									{
										p3[0]='\0';
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											if(this->ParseCellName(p3+1,ecol,erow,browcol,browcol))
											{
												tcol = ecol;trow = erow;
												if(scol > ecol){ecol = scol;scol = tcol;}
												if(srow > erow){erow = srow;srow = trow;}
												bcalc = true;
											}
										}
										p3[0]=':';
									}else
									{
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											erow = srow;ecol=scol; bcalc = true;
										}

									}
									if(bcalc)
									{
										TGridRect	srect;
										for(int i=scol;i<=ecol;i++)
										{
											for(int j=srow;j<=erow;j++)
											{
												m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,srect);
												if(srect.left!=i||srect.top!=j) continue;
												PStrItem pItem;
												pItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
												if(pItem && pItem->pStrFormula)
												{
													CHxComplier * pComplier = new CHxComplier();
													pComplier->m_pGrid = this->m_pGrid;
													var val;
													val.sv_type=undef;
													pComplier->of_calcexpression(pItem->pStrFormula,val);
													if(pComplier) delete pComplier;
													if(val.sv_type==reals)
													{
														if(styp ==ints)
														{
															srval = (float)sival;
															styp = reals;
														}
														srval += val.f;
													}
													if(val.sv_type==ints)
													{
														if(styp ==ints)
															sival += val.i;
														else
															srval += val.i;

													}
													if(val.sv_type==strs && val.bStrNeedDeleted && val.m_pStr)
														delete val.m_pStr;
												}else
												{
													pStr = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(i,j);
													if(pStr&& pStr[0]!='\0')
													{
														//val.sv_type=undef;
														if(isNumber(pStr,typ,ival,rval))
														{
															if(typ ==reals)
															{
																if(styp ==ints)
																{
																	srval = (float)sival;
																	styp = reals;
																}
																srval += rval;
															}else
															{
																if(styp ==ints)
																	sival += ival;
																else
																	srval += ival;
															}
														}
													}
												}
											}
										}
									}
								}
							}
							if(p2)p2[0]=',';
							if(p2)
								p1 = p2 + 1;
							else
								p1 = NULL;
						}
						if(styp==ints)
						{
							s[t].i=sival;s[t].f=(float)sival;s[t].sv_type =ints;
						}else
						{
							s[t].f=srval;s[t].sv_type =reals;
						}
						if(s[t].bStrNeedDeleted) if(s[t].m_pStr) delete s[t].m_pStr;
					}
				}
				break;
			case 2002: //avg
				{
					if(s[t].m_pStr)
					{
						LPTSTR p1,p2,p3;
						LPTSTR pStr;
						keyw typ;
						keyw styp = ints;
						int	ival;
						int cnt = 0;
						float rval;
						int sival = 0;
						float srval = 0;

						p1 = s[t].m_pStr;;
						while(p1)
						{
							p2 = ::_tcschr(p1,',');
							if(p2)p2[0]='\0';
							if(isNumber(p1,typ,ival,rval))
							{
								cnt++;
								if(typ ==reals)
								{
									if(styp ==ints)
									{
										srval = (float)sival;
										styp = reals;
									}
									srval += rval;
								}else
								{
									sival += ival;
								}
							}else
							{
								if(m_pGrid)
								{
									p3 = ::_tcschr(p1,':');
									int srow,scol,erow,ecol,trow,tcol;
									bool	browcol;
									bool	bcalc = false;
									if(p3)
									{
										p3[0]='\0';
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											if(this->ParseCellName(p3+1,ecol,erow,browcol,browcol))
											{
												tcol = ecol;trow = erow;
												if(scol > ecol){ecol = scol;scol = tcol;}
												if(srow > erow){erow = srow;srow = trow;}
												bcalc = true;
											}
										}
										p3[0]=':';
									}else
									{
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											erow = srow;ecol=scol; bcalc = true;
										}

									}
									if(bcalc)
									{
										TGridRect srect;
										for(int i=scol;i<=ecol;i++)
										{
											for(int j=srow;j<=erow;j++)
											{
												this->m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,srect);
												if(srect.left!=i||srect.top!=j) continue;
												PStrItem pItem;
												pItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
												if(pItem && pItem->pStrFormula)
												{
													CHxComplier * pComplier = new CHxComplier();
													pComplier->m_pGrid = this->m_pGrid;
													var val;
													val.sv_type=undef;
													pComplier->of_calcexpression(pItem->pStrFormula,val);
													if(pComplier) delete pComplier;
													if(val.sv_type==reals)
													{
														cnt++;
														if(styp ==ints)
														{
															srval = (float)sival;
															styp = reals;
														}
														srval += val.f;
													}
													if(val.sv_type==ints)
													{
														cnt++;
														if(styp ==ints)
															sival += val.i;
														else
															srval += val.i;

													}
													if(val.sv_type==strs && val.bStrNeedDeleted && val.m_pStr)
														delete val.m_pStr;
												}else
												{
													pStr = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(i,j);
													if(pStr&& pStr[0]!='\0')
													{
														if(isNumber(pStr,typ,ival,rval))
														{
															cnt++;
															if(typ ==reals)
															{
																if(styp ==ints)
																{
																	srval = (float)sival;
																	styp = reals;
																}
																srval += rval;
															}else
															{
																if(styp ==ints)
																	sival += ival;
																else
																	srval += ival;
															}
														}
													}
												}
											}
										}
									}
								}
							}
							if(p2)p2[0]=',';
							if(p2)
								p1 = p2 + 1;
							else
								p1 = NULL;
						}
						if(styp==ints)
						{
							s[t].i=sival;s[t].f=(float)sival;s[t].sv_type =ints;
						}else
						{
							s[t].f=srval;s[t].sv_type =reals;
						}
						if(cnt >0){s[t].i=sival/cnt;s[t].f=s[t].f/cnt; s[t].sv_type=reals;}
						if(s[t].bStrNeedDeleted) if(s[t].m_pStr) delete s[t].m_pStr;
					}
				}
				break;
			case 2003: //count
				{
					if(s[t].m_pStr)
					{
						LPTSTR p1,p2,p3;
						LPTSTR pStr;
						keyw typ;
						keyw styp = ints;
						int	ival;
						int cnt = 0;
						float rval;
						int sival = 0;
						float srval = 0;

						p1 = s[t].m_pStr;;
						while(p1)
						{
							p2 = ::_tcschr(p1,',');
							if(p2)p2[0]='\0';
							if(isNumber(p1,typ,ival,rval))
							{
								cnt++;
							}else
							{
								if(m_pGrid)
								{
									p3 = ::_tcschr(p1,':');
									int srow,scol,erow,ecol,trow,tcol;
									bool	browcol;
									bool	bcalc = false;
									if(p3)
									{
										p3[0]='\0';
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											if(this->ParseCellName(p3+1,ecol,erow,browcol,browcol))
											{
												tcol = ecol;trow = erow;
												if(scol > ecol){ecol = scol;scol = tcol;}
												if(srow > erow){erow = srow;srow = trow;}
												bcalc = true;
											}
										}
										p3[0]=':';
									}else
									{
										if(this->ParseCellName(p1,scol,srow,browcol,browcol))
										{
											erow = srow;ecol=scol; bcalc = true;
										}

									}
									if(bcalc)
									{
										TGridRect srect;
										for(int i=scol;i<=ecol;i++)
										{
											for(int j=srow;j<=erow;j++)
											{
												m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,srect);
												if(srect.left!=i||srect.top!=j) continue;
												PStrItem pItem;
												pItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
												if(pItem && pItem->pStrFormula)
												{
													CHxComplier * pComplier = new CHxComplier();
													pComplier->m_pGrid = this->m_pGrid;
													var val;
													val.sv_type=undef;
													pComplier->of_calcexpression(pItem->pStrFormula,val);
													if(pComplier) delete pComplier;
													if(val.sv_type==reals)cnt++;
													if(val.sv_type==ints)cnt++;
													if(val.sv_type==strs && val.bStrNeedDeleted && val.m_pStr)
														delete val.m_pStr;
												}else
												{
													pStr = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(i,j);
													if(pStr&& pStr[0]!='\0')
													{
														if(isNumber(pStr,typ,ival,rval))cnt++;
													}
												}
											}
										}
									}
								}
							}
							if(p2)p2[0]=',';
							if(p2)
								p1 = p2 + 1;
							else
								p1 = NULL;
						}
						s[t].i=cnt;s[t].f=(float)cnt;s[t].sv_type =ints;
						if(s[t].bStrNeedDeleted) if(s[t].m_pStr) delete s[t].m_pStr;
					}
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
			h1 = s[t].i;
			h2 = ir.y;
			t = t -1;
			h3 =0;
			do
			{
				if(codes[h2].f != 13)
				{
					h3 = 1;
					ps =_T("caschk");
				}else
				{
					if(codes[h2].y == h1)
					{
						h3 = 1;
						pc = codes[h2+1].y;
					}else
						h2 = h2+2;
				}
			}while(h3 == 0);
			break;
		case 14:	//for1up
			h1 = s[t - 1].i;
			if(h1 < s[t].i)
				s[s[t - 2].i].i = h1;
			else
			{
				t = t - 3;
				pc = ir.y;
			}
			break;
		case 15: //for2up
			h2 = s[t - 2].i;
			h1 = s[h2].i + 1;
			if (h1<= s[t].i )
			{
				s[h2].i = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 16: //for1down
			h1 = s[t - 1].i;
			if(h1 >=s[t].i)
			{
				s[h2].i = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 17: //for2down
			h2 = s[t - 2].i;
			h1 = s[h2].i - 1;
			if(h1 > s[t].i) 
			{
				s[h2].i = h1;
				pc = ir.y;
			}else
				t = t - 3;
			break;
		case 18: //mark stack
/*			h1 = btab[tab[ir.y].refe].vsize;
			t = t+5;
			s[t - 1].i = h1 -1; 
			s[t].i = ir.y;*/
			break;
		case 19:	//call
			h1 = t - ir.y;
			h2 = s[h1+4].i;
			h3 = tab[h2].lev;
			h4 = s[h1+3].i + h1;
			s[h1+1].i = pc;
			s[h1+3].i = b;
			for(h3 = t+1;h3<=h4;h3++)
				s[h3].i = 0;
			b = h1;
			t = h4;
			pc = tab[h2].adr;
			break;
		case 20:	//index1
/*			h1 = ir.y;
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
			}*/
			break;
		case 21:	//index
/*			h1 = ir.y;
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
			}*/
			break;
		case 22:	//load block
			h1 = s[t].i;
			t = t - 1;
			h2 = ir.y + t;
			while(t < h2)
			{
				t = t + 1;
				s[t].i = s[h1].i;
				h1 = h1 + 1;
			}
			break;
		case 23:	//copy block
			h1 = s[t - 1].i;
			h2 = s[t].i;
			h3 = h1 + ir.y;
			while(h1 < h3)
			{
				s[h1].i = s[h2].i;
				h1 = h1 + 1;
				h2 = h2 + 1;
			}
			t = t - 2;
			break;
		case 24:	//literral
			t = t +1;
			s[t].i = ir.y;
			s[t].sv_type=ints;
			break;
		case 25:	//loaD REAL
			t = t + 1;
			s[t].f = rconst[ir.y];
			s[t].sv_type=reals;
			break;
		case 26:	//float
			h1 = t - ir.y;
			s[h1].toFloat();
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
			pc = s[b+1].i;
			break;
		case 33:	//exit function
			t = b;
			pc = s[b+1].i;
			break;
		case 34: 
			s[t].i = s[s[t].i].i;
			s[t].sv_type=ints;
			break;
		case 35:
			if(s[t].b == true)
				s[t].b = 0;
			else
				s[t].b = true;
			s[t].sv_type=ints;
			break;
		case 36:
			s[t].i = - s[t].i;
			break;
		case 37:
			//
			break;
		case 38:	//store
			s[s[t - 1].i].i = s[t].i;
			t = t - 2;
			break;
		case 39:	
			t = t -1;
			if(s[t].i == s[t+1].i) 
				s[t].b = 0;
			else
				s[t].b = true;
			break;
		case 40:
			t = t -1;
			if(s[t].f != s[t+1].f)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 41:
			t = t - 1;
			if(s[t].f < s[t+1].f)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 42:
			t = t - 1;
			if(s[t].f <= s[t+1].f)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 43:
			t = t - 1;
			if(s[t].f > s[t+1].f)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 44:
			t = t - 1;
			if(s[t].f >= s[t+1].f)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 45:
			t = t - 1;
			if(s[t].i == s[t+1].i)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 46:
			t = t - 1;
			if(s[t].i != s[t+1].i) 
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 47:
			t = t - 1;
			if(s[t].i < s[t+1].i)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 48:
			t = t - 1;
			if(s[t].i <= s[t+1].i)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 49:
			t = t - 1;
			if(s[t].i > s[t+1].i)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 50:
			t = t - 1;
			if(s[t].i >= s[t+1].i)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 51:		//or
			t = t - 1;
			if(s[t].b || s[t+1].b)
				s[t].b = true;
			else
				s[t].b = false;
			break;
		case 52:      //ADD             
			t = t -1;
			s[t].i = s[t].i + s[t + 1].i;
			s[t].sv_type = ints;
			break;
		case 53:   //SUB
			t = t - 1;
			s[t].i = s[t].i - s[t+1].i;
			s[t].sv_type = ints; 
			break;
		case 54:  //ADR
			t = t -1;
			s[t].f = s[t].f + s[t + 1].f;
			s[t].sv_type = reals;
			break;
		case 55:
			t = t -1;
			s[t].f = s[t].f - s[t + 1].f; //SUR
			s[t].sv_type = reals;
		case 56:  //AND
			t = t -1;
			if(s[t].b && s[t+1].b)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			break;
		case 57: //MUL
			t = t -1;
			s[t].i = s[t].i * s[t+1].i;
			s[t].sv_type = ints;
			break;
		case 58:	//idiv
			t = t -1;
			if(s[t+1].i == 0)
				ps = _T("divchk");
			else
				s[t].i = int(s[t].i/s[t+1].i);
			s[t].sv_type = ints;
			break;
		case 59: //mod
			t = t -1;
			if(s[t+1].i == 0)
				ps =_T("divchk");
			else
				s[t].i = s[t].i % s[t+1].i;
			s[t].sv_type = ints;
			break;
		case 60:	//rmul
			t = t - 1;
			s[t].f = s[t].f*s[t+1].f;
			s[t].sv_type = reals;
			break;
		case 61:	//rdiv
			t = t -1;
			s[t].f = s[t].f /s[t+1].f;
			s[t].sv_type = reals;
			break;
		case 62:
			//
			break;
		case 63:
			//
			break;
		case 64:       //str ==
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr)==0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 65:
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr)!=0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 66:
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr) < 0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 67:
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr) <= 0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 68:
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr) > 0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 69:
			t = t - 1;
			if(::_tcscmp(s[t].m_pStr ,s[t+1].m_pStr) >= 0)
				s[t].b = true;
			else
				s[t].b = false;
			s[t].sv_type = bools;
			if(s[t].m_pStr && s[t].bStrNeedDeleted)delete s[t].m_pStr;
			if(s[t+1].m_pStr && s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 70:	//LDS
			t = t +1;
			s[t].m_pStr = sconst[ir.y];
			s[t].bStrNeedDeleted = false;
			s[t].sv_type=/*keyw::*/strs;
			s[t].StrSize = (int)::_tcslen(s[t].m_pStr) + 1;
			break;
		case 71: //ADC
			t = t - 1;
			if(!s[t+1].m_pStr)break;
			if(::_tcslen(s[t+1].m_pStr)==0)
			{
				if(s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
				break;
			}
			if(!s[t].m_pStr)
			{
				s[t].m_pStr = s[t + 1].m_pStr;
				s[t].bStrNeedDeleted = s[t + 1].bStrNeedDeleted;
				break;
			}
			int len1,len2,len;
			int alloclen;
			LPTSTR	pStr;
			len1 = (int)::_tcslen(s[t].m_pStr);
			len2 = (int)::_tcslen(s[t + 1].m_pStr);
			len = len1 + len2;
			if(len + 1 > s[t].StrSize)
			{
				if((len + 1) % ARRAYINC ==0)
					alloclen = (len + 1) / DEFSTRSIZE ;
				else
					alloclen = (len + 1) / DEFSTRSIZE  + DEFSTRSIZE;
				pStr = new TCHAR[alloclen];
				s[t].StrSize = alloclen;

				if(s[t].m_pStr)::_tcscpy_s(pStr,alloclen - 1,s[t].m_pStr);
				if(s[t].bStrNeedDeleted) delete s[t].m_pStr;
				s[t].m_pStr = pStr;
			}

			::_tcscpy_s(s[t].m_pStr + len1,len + 1,s[t + 1].m_pStr);
			s[t].bStrNeedDeleted = true;

			if(s[t+1].bStrNeedDeleted)delete s[t+1].m_pStr;
			break;
		case 72: //FLC
			s[t - ir.y].toString();
			break;
		case 73:	//VAL
			t ++;
			s[t].bStrNeedDeleted = false;
			s[t].sv_type=strs;
			if(ir.x >= (int)m_pGrid->GetGridDataSvr()->FBandDwTables.size())
				ps = _T("upper dataStore");
			else if(ir.y > m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->getColumnCount())
			  ps =_T("upper datastore colCount");
//			else if(/*s[t].i*/FRow > m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->GetRowCount())
//			  ps = _T("upper dataStore RowCount");
			 else
			 {
				 CDwTable * pDwTable = m_pGrid->GetGridDataSvr()->GetDwTableFromCell(FCol,FRow);
				 LPTSTR pStr = NULL;
				 int nRow,nCol;
				 if(pDwTable == m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x])
				 {
					pDwTable->getRowColumnFromCell(FCol,FRow,nRow,nCol);
					pStr = pDwTable->getDwItemText(/*s[t].i*/nRow,ir.y);
				 }else
				 {
					 int ARow,ACol;
					 m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->getCellFromRowColumn(1,ir.y,ACol,ARow);
					 if(ARow > FRow)
						pStr = m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->getDwItemText(/*s[t].i*/1,ir.y);
					 else
					 {
						 m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->getCellFromRowColumn(m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->GetRowCount(),ir.y,ACol,ARow);
						 if(ARow < FRow)
							pStr = m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->getDwItemText(/*s[t].i*/m_pGrid->GetGridDataSvr()->FBandDwTables[ir.x]->GetRowCount(),ir.y);

					 }

				 }
				 s[t].m_pStr = pStr;
				 s[t].bStrNeedDeleted = false;
				 s[t].sv_type=/*keyw::*/strs;
				 if(pStr) 
					 s[t].StrSize = (int)::_tcslen(s[t].m_pStr) + 1;
				 else
					s[t].StrSize = 0;

				 keyw typ;
				 int inum;
				 float rnum;
				 if(this->isNumber(s[t].m_pStr,typ,inum,rnum))
				 {
					 if(typ==ints)
					 {
						 s[t].i = inum;
						 s[t].f = (float)inum;
					 }
					 if(typ==reals)
					 {
						 s[t].f = rnum;
						 s[t].i = int(rnum);
					 }
				 }

				  /*string	ls_coltype
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
				  END CHOOSE*/
			  }
			  break;
			case 74:	//ROW
			  t ++;
			  //s[t] = ids_objs[ir.y].getRow();
			  break;
			case 80: //CELL;
				{
					int col,row;
					bool acol,arow;
				  
					t ++;
					s[t].bStrNeedDeleted = false;
					s[t].sv_type=strs;
					if(this->ParseCellName(sconst[ir.y],col,row,acol,arow))
					{
						if(m_pGrid)
						{
							TGridRect	srect;
							m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(col,row,srect);
							if(srect.left==col ||srect.top == row)
							{
								PStrItem pItem;
								pItem  = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(col,row);
								if(pItem && pItem->pStrFormula)
								{
									CHxComplier* pComplier = new CHxComplier();
									pComplier->m_pGrid = this->m_pGrid;
									var val;
									pComplier->of_calcexpression(pItem->pStrFormula,val);
									::memcpy_s(&s[t], sizeof(var),&val,sizeof(var));
									if(s[t].sv_type==ints)s[t].f = s[t].f;
									if(s[t].sv_type==reals)s[t].i = s[t].i;
									if(pComplier) delete pComplier;
								}else
								{
									s[t].m_pStr = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(col,row);
								}
							}else
							{
								s[t].m_pStr=_T("");
							}
						}
						else
							s[t].m_pStr=sconst[ir.y];//_T("");
					}
					else
					{
						s[t].m_pStr=_T("");;
					}
					if(s[t].sv_type==strs){
						if(!s[t].m_pStr)
							s[t].StrSize = 0;
						else
							s[t].StrSize =  (int)::_tcslen(s[t].m_pStr)+1;

						keyw typ;
						int inum;
						float rnum;
						if(this->isNumber(s[t].m_pStr,typ,inum,rnum))
						{
							if(typ==ints)
							{
								s[t].i = inum;
								s[t].f = (float)inum;
							}
							if(typ==reals)
							{
								s[t].f = rnum;
								s[t].i = int(rnum);
							}
						}
					}
				  /*
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
					s[t]=_T("");*/
				}
			  break;
			case 81: //CELL SETS
			  t ++;
			  break;
		}
	}while(ps == _T("run"));

	if(!(ps==_T("fin")))
	{
		return -1;
	}

	::memcpy_s(&value, sizeof(var),&s[t],sizeof(var));
	if(s) delete s;
	return 1;
}

// parse excell cell name
bool CHxComplier::ParseCellName(LPTSTR szName, int & col, int & row, bool & acol, bool & arow)
{
	int cc = 0;
	int ch;
	int len = (int)::_tcslen(szName);
	acol = false;
	arow = false;
	col = 0;
	row = 0;

	//parse col;
	if(cc >=len) return false;
	ch=szName[cc];
	cc++;
	if(ch=='$')
	{
		acol = true;
		if(cc >=len) return false;
		ch=szName[cc];
		cc++;
	}
	
	
	if(ch>='a' && ch<='z')
	{
		col = ch - 'a' + 1;
		if(cc >=len) return false;
		ch=szName[cc];
		cc++;
		if(ch>='a' && ch<='z')
		{
			col = col * 26 + ch - 'a' + 1;
			if(cc >=len) return false;
			ch=szName[cc];
			cc++;
		} 
		if(ch=='$')
		{
			arow = true;
			if(cc >=len) return false;
			ch=szName[cc];
			cc++;
		}
		while(ch>='0' && ch <='9')
		{
			row = row * 10 + ch - '0';
			if(cc >=len)break;
			ch=szName[cc];
			cc++;
		}
		if(row <1) return false;
		return true;
	}else
		return false;

	return false;
}

bool CHxComplier::isNumber(LPTSTR x0)
{
	int inum;
	float rnum;
	keyw typ;
	return isNumber(x0,typ,inum,rnum);
}

bool CHxComplier::isNumber(LPTSTR x0,keyw &typ,int & inum,float &rnum)
{
	if(!x0)
	{
		typ=ints;
		inum=0;
		rnum=0;
		return true;
	}
	int cc = 0;
	int ch;
	int len = (int)::_tcslen(x0);
	int sign=1;
	
	typ=undef;
	inum=0;
	rnum=0;
	if(cc < len) ch=x0[cc++]; else ch='\0';
	while(ch=='\r'||ch=='\t'||ch==' '||ch=='\n')
	{
		if(cc < len) ch=x0[cc++]; else ch='\0';
	}

	if(len<1 ||  ch=='\0')
	{
		inum=0;
		rnum = 0;
		typ=ints;
		return true;
	}
	
	if(ch=='+')
	{
		sign = 1;
		if(cc < len) ch=x0[cc++]; else ch='\0';
	}
	else if(ch=='-')
	{
		sign = -1;
		if(cc < len) ch=x0[cc++]; else ch='\0';
	}
	

	if (ch>='0' && ch<='9')
	{
		inum = 0;
		typ=ints;
		do
		{
			inum  = inum *10 + ch - '0';
			if(cc < len) ch=x0[cc++]; else ch='\0';

		}while(ch>='0' && ch<='9');
		if(ch=='.')
		{
			int e;
			if(cc < len) ch=x0[cc++]; else ch='\0';
			typ = reals;
			rnum = (float)inum;
			e=0;
			while (ch>='0' && ch<='9')
			{
				e= e - 1;
				rnum = rnum*10 + ch - '0';
				if(cc < len) ch=x0[cc++]; else ch='\0';
			}
			if(e==0) return false;
			if(ch=='e' ||ch=='E')
			{
				int sign1 = 1;
				int s = 0;
				if(cc < len) ch=x0[cc++]; else ch='\0';
				if(ch=='+')
				{
					sign1 = 1;
					if(cc < len) ch=x0[cc++]; else ch='\0';
				}
				else if(ch=='-')
				{
					sign1 = -1;
					if(cc < len) ch=x0[cc++]; else ch='\0';
				}
				if(ch <'0'|| ch >'9')
					return false;
				do
				{
					s = s*10 + ch -'0';
				}while(ch>='0' && ch<='9');

				e =  sign1*s;;
			}
			if(e !=0)adjustscale(rnum,e);
		}if(ch=='e'||ch=='E')
		{
			typ=reals;
			rnum = (float)inum;
			int e = 0;
			e = readscale();
			if(e != 0)adjustscale(rnum,e);
		}
	}

	if(typ==ints)
	{
		inum = sign * inum;
		rnum = (float)inum;
	}
	if(typ==reals) rnum = sign * rnum;
	
	while(ch!='\0')
	{
		if(cc < len) ch=x0[cc++]; else ch='\0';
		if(!(ch=='\r'||ch=='\t'||ch==' '||ch=='\n')) return false;
	}
	
	if(cc==len && (typ==reals||typ==ints))return true;
	return false;
}

CHxComplier::keyw CHxComplier::GetCellType(LPTSTR szCell)
{
	keyw typ = strs;
	int inum;
	float rnum;
	int col,row;
	bool acol,arow;

	if(this->ParseCellName(szCell,col,row,acol,arow))
	{
		if(m_pGrid)
		{
			LPTSTR pText;
			PStrItem pItem;
			TGridRect	srect;
			pItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(col,row);
			m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(col,row,srect);
			if(srect.left==col ||srect.top == row)
			{
				if(pItem && pItem->pStrFormula)
				{
					CHxComplier complier;
					complier.m_pGrid = this->m_pGrid;
					var val;
					complier.of_calcexpression(pItem->pStrFormula,val);
					typ = val.sv_type;
					if(val.sv_type==strs && val.bStrNeedDeleted && val.m_pStr)delete val.m_pStr;
				}else
				{
					pText = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(col,row);
					if(pText)
					{
						if(this->isNumber(pText,typ,inum,rnum))
							return typ;
						else
							return strs;
					}
				}
			}else
			{
				typ=ints;
			}
		}
	}
	return typ;
}

// 得到关联的单元格
int CHxComplier::GetRCells(LPTSTR szFormula, RECT pRect[] ,DWORD pAbs[])
{
	int index = 0;
	return GetRCells(szFormula,pRect,pAbs,index);
}

int CHxComplier::GetRCells(LPTSTR szFormula, RECT pRect[] ,DWORD pAbs[],int sIndex)
{
	int i;
	int scol,srow;
	bool sacol,sarow;
	int ecol,erow;
	bool eacol,earow;
	int trow,tcol;
	
	CHxComplier complier;

	SetSource(szFormula);
	nextch();
	insymbol();
	while(sy!=undef)
	{
		if(sy==indent)
		{
			i=loc(snum);
			if(i< 0)
			{
				if(this->ParseCellName(snum,scol,srow,sacol,sarow))
				{
					insymbol();
					if(sy==colon)
					{
						insymbol();
						this->ParseCellName(snum,ecol,erow,eacol,earow);
						tcol = ecol;trow = erow;
						if(scol > ecol){ecol = scol;scol = tcol;}
						if(srow > erow){erow = srow;srow = trow;}
						insymbol();

					}else
					{
						ecol =scol;erow = srow;eacol = sacol;earow = sarow;
					}
					::SetRect(pRect +sIndex,scol,srow,ecol,erow);
					pAbs[sIndex] = 0;
					if(sacol) pAbs[sIndex] |= 0x1;
					if(sarow) pAbs[sIndex] |= 0x2;
					if(eacol) pAbs[sIndex] |= 0x4;
					if(earow) pAbs[sIndex] |= 0x8;
					sIndex++;
				}else
				{
					insymbol();
				}
			}else if(::_tcscmp(snum,_T("sum"))==0||::_tcscmp(snum,_T("avg"))==0||::_tcscmp(snum,_T("count"))==0)
			{
				insymbol();
				if(sy==lparent)
				{
					int pos =cc;
					int epos = cc;
					insymbol();
					while(ch!='\0' && sy!=rparent && sy!=comma)
					{
						epos = cc;
						insymbol();
					}
					if(epos >pos && (sy==rparent || sy==comma))
					{
						LPTSTR pExp;
						TCHAR och;
						och = source[epos];
						source[epos]='\0';

						pExp = source + pos;
						var v;
						complier.of_calcexpression(pExp,v);
						if(v.sv_type==strs)
						{
							sIndex = complier.GetRCells(v.m_pStr,	pRect,pAbs,sIndex);
							if(v.bStrNeedDeleted && v.m_pStr)delete v.m_pStr;
						}
						source[epos]=och;
					}
				}
				insymbol();
			}else
				insymbol();
		}else
			insymbol();
	}
	return sIndex;
}

bool CHxComplier::GetRCellFormula(LPTSTR  pFormula, int ACol , int ARow , LPTSTR  & pRFormula)
{
	int srow,scol;
	bool sacol,sarow;
	bool	bChanged = false,bReplace=false,bColError = false;
	int i;
	int cc0;
	this->SetSource(pFormula);
	TCHAR cs[255];
	TCHAR * formula = NULL;

	TCHAR colName[3];
	TCHAR colName1[3];
	LPTSTR pFormat = new TCHAR[::_tcslen(pFormula)+1];
	nextch();
	cc0 = cc;
	insymbol();
	while(sy!=undef)
	{
		if(sy==indent)
		{
			i=loc(snum);
			if(i< 0)
			{
				if(this->ParseCellName(snum,scol,srow,sacol,sarow))
				{
					bColError = false;
					if( scol/26/26 < 1)
					{
						if( (scol - 1)/26 > 0)
						{
							colName[0] = (scol - 1)/26 - 1 + 'A';
							colName[1] = (scol -1)% 26 +'A';
							colName[2] = '\0';
						}else
						{
							colName[0] = (scol -1)% 26 +'A';
							colName[1] = '\0';
						}
					}else
						bColError = true;
					if( (scol + ACol)/26/26 < 1)
					{
						if( (scol + ACol - 1)/26 > 0)
						{
							colName1[0] = ((scol + ACol) - 1)/26 - 1 + 'A';
							colName1[1] = ((scol + ACol) -1)% 26 +'A';
							colName1[2] = '\0';
						}else
						{
							colName1[0] = ((scol + ACol) -1)% 26 +'A';
							colName1[1] = '\0';
						}
					}else
						bColError = true;
					bReplace = true;
					//if(sacol && sarow)cs.Format(_T("$%s$%i"),colName,srow); not replace;
					if(sacol && !sarow)
						{_stprintf_s(cs,255,_T("$%s%i"),colName,srow + ARow); bChanged = true; bReplace = true;}
					if(!sacol && sarow)
						{ _stprintf_s(cs,255,_T("%s$%i"),colName1,srow);bChanged = true; bReplace = true;}
					if(!sacol && !sarow)
						{ _stprintf_s(cs,255,_T("%s%i"),colName1,srow + ARow);bChanged = true; bReplace = true;}
					if(cc - cc0 - ::_tcslen(snum) > 0 && bReplace && !bColError)
					{
						TCHAR ch1;
						ch1 = source[cc - ::_tcslen(snum)];
						source[cc - ::_tcslen(snum)]='\0';

						//formula.Append(source + cc0);
						//formula.Append(cs);

						int nLen = (int)_tcslen(source + cc0) + (int)_tcslen(cs) + 1;
						formula = new TCHAR[nLen];
						formula[0]='\0';
						_tcscat_s(formula,nLen ,source + cc0);
						_tcscat_s(formula,nLen ,cs);

						source[cc - ::_tcslen(snum)]=ch1;
						cc0 = cc;
					}
				}
			}
		}
		insymbol();
	}
	if(cc0 <= sourcelen - 1)
	{
		LPTSTR pStr = new TCHAR[_tcslen(formula) + _tcslen(source + cc0)];
		pStr[0]='\0';
		_tcscat_s(pStr,_tcslen(formula) + _tcslen(source + cc0),formula + cc0);
		if(formula)delete formula;
		formula = pStr;

	}
	if(bChanged)
	{
		pRFormula = new TCHAR[_tcslen(formula)+1];
		::_tcscpy_s(pRFormula,_tcslen(formula)+1,formula);
	}
	if(formula)delete formula;
	return bChanged;
}

int CHxComplier::getdsindex (LPTSTR as_name, int &al_index, int &al_colid, LPTSTR &as_type)
{
	LPTSTR pTableName = NULL;
	if(_tcschr(as_name,'.'))
	{
		pTableName = new TCHAR[ _tcschr(as_name,'.') - as_name + 1];
		_tcsncpy_s(pTableName,_tcschr(as_name,'.') - as_name + 1,as_name,_tcschr(as_name,'.') - as_name);
		as_name = _tcschr(as_name,'.') + 1;
	}

	int i=0;
	if( pTableName)
	{
		for(i=0;i<(int)m_pGrid->GetGridDataSvr()->FBandDwTables.size();i++)
		{
			if(_tcsicmp(pTableName,m_pGrid->GetGridDataSvr()->FBandDwTables[i]->m_strBoundGroup)==0) break;
		}
	}
	if(i>=(int)m_pGrid->GetGridDataSvr()->FBandDwTables.size()) return -1;

	if(pTableName)
	{
		int j;
		delete pTableName;
		for(j=1;j<=m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnCount();j++)
		{
			if(_tcsicmp(as_name,m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnName(j))==0)break;
		}
		if(j <= m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnCount())
		{
			al_index = i;
			al_colid = j;
			return al_index;
		}else
			return -1;
	}else
	{
		for(i=0;i<(int)m_pGrid->GetGridDataSvr()->FBandDwTables.size();i++)
		{
			int j;
			for(j=1;j<=m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnCount();j++)
			{
				if(_tcsicmp(as_name,m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnName(j))==0)break;
			}
			if(j <= m_pGrid->GetGridDataSvr()->FBandDwTables[i]->getColumnCount())
			{
				al_index = i;
				al_colid = j;
				return al_index;
			}
		}
	}
	return -1;
}


