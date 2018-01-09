#include <windows.h>
#include <tchar.h>
#include "xvmcore.h"
#include "atoms.h"
#include "foperate.h"
#include "xvmerror.h"
#include <vector>

using namespace xcode;
using namespace std;

class XVM_API xcomplier
{
public:
	xcomplier();
	~xcomplier();
public:
	xvmcore* m_pxvm;
public:
	int  eval(tchar * str);
	int  procdescseq();
	int  procdescv();
	int  procdesct();
	int  procfct();
	int  procclass();
	int  procinterface();
	int  procclass(bool binterface );
	int  procexpr();
	int  procexpr(int sy1);
	int  procexprterm();

	int  procstmt(bool bcontinue=true);
	int  procstmtcompound();
	int  procstmtinclude();
	int  procstmtexpr();
	int  procstmtlabeled();
	int  procstmtselection();
	int  procstmtiteration();
	int  procstmtjump();
	int  procstmtdeclaration();
	int  procstmttry();

	int  proctuple();
	int  procparalist();
public:
	tchar * m_psource;
	int	m_nrow;
	size_t m_nlen;
	int cc;
	tchar ch;
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
	void procerror();
public:
	static int getOrder(tchar * p);
	static int getType(tchar * p);
	int showTuple(dataitem &d,bool data=true);
	int showTuple( xtuple* t,bool data=true);
	static int getOpIndent(tchar * p);
	static tchar * getOpIndentStr(int op);
	static tchar* getTypeStr(int p);
	static tchar* getOrderStr(int p);
	static int parsepcode(xvmcore* m_pxvm,tchar * buf,bool bexec = false,xarrays<code *,code> * pCodes = null,code * pcode = null);
public:
	xvmerror* m_error;
	vector<LPTSTR > * m_pCodes;
};



