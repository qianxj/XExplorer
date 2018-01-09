#pragma once
#include <windows.h>
#include <tchar.h>
#include "xvmcore.h"
#include "atoms.h"
#include "foperate.h"
#include "xvmerror.h"
#include <vector>
#include "xpackage.h"

using namespace xcode;
using namespace std;

class XVM_API xsscomplier
{
public:
	xsscomplier();
	~xsscomplier();
public:
	xvmcore* m_pxvm;
public:
	bool istype();
public:
	int  eval(tchar * str);
	int  procdescseq();
	int  procdescv(int spec =0,bool barg = false,xvmcore* pxvm = null);
	int  procdesct(int spec =0,bool barg = false,xvmcore* pxvm=null);
	int  proctypeitem(tchar * buf,int nlen,int &index);
	int  procstmtinclude();
	int  procclass();
	int  procinterface();
	int  procclass(int typ ); 
	int  procfct(int spec = 0,xvmcore* pxvm=null);
	int  procexpr();
	int  procexpr(int sy1,int state = 0);
	int  procexprterm(int state = 0);
	int  procexprname(tchar *buf,bool bsingle = true,int state = 0,bool shouldnest = false);
	int  proceparaval();

	int  procstmt(bool bcontinue=true);
	int  procstmtcompound();
	int  procstmtexpr();
	int  procstmtlabeled();
	int  procstmtselection();
	int  procstmtiteration();
	int  procstmtjump();
	int  procstmtdeclaration();
	int  procstmttry();
	int  procstmtwith();
	int  procstmtusing();

	int getexeccodes(int start,int end);
	int  proctuple();
	int  procparalist();
public:
	tchar * m_psource;
	int	m_nrow;
	size_t m_nlen;
	int cc;
	tchar ch;
	int blockid ;
	int curblock;
public:
	int enter(tchar * pcode);
public:
	int sy;
	union
	{
		double rnum;
		int    inum;
		tchar snum[255];
	};
	tchar sclass[255];
	
public:
	int readscale(void);
	int adjustscale(int e);
	int adjustscale(double &rnum, int e);
	int insymbol(void);
	tchar nextch();
public:
	int error(tchar * ptext,bool bprocess = false);
	int output(tchar * ptext);
	void procerror();
public:
	int getOrder(tchar * p);
	int getType(tchar * p);
	int showTuple(dataitem &d,bool data=true);
	int showTuple( xtuple* t,bool data=true);
	int getOpIndent(tchar * p);
	static tchar * getOpIndentStr(int op);
	static tchar* getTypeStr(int p);
	static tchar* getOrderStr(int p);
public:
	xvmerror* m_error;
	vector<LPTSTR > * m_pCodes;
	xPackageMgr * m_pPackageMgr;
public:
	bool m_bChecked;
public:
	LPTSTR getVarAutoString(LPTSTR var);
	LPTSTR getFctTipString(LPTSTR var);
};



