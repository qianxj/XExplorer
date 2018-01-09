#pragma once
#include "xvmdef.h"
#include "strpool.h"
#include "xstack.h"
#include "xtuple.h"
#include "atoms.h"
#include "xcolumn.h"
#include "xdatamgr.h"
#include "xstring.h"
#include "xvmerror.h"

namespace xcode
{
	enum symItem
	{
		si_type=0, si_name,si_ref, si_addr,si_dims,si_arrayref,si_typeref,si_typ0,si_cargs,si_args,si_body,si_sym,si_block,si_spec,
		si_native,si_from,si_alias,si_namespace,si_fieldid,si_dimex
	};

	struct xtuple;
	class XVM_API xvmcore
	{
	public:
		xvmcore(void);
		~xvmcore(void); 
	public:
		strpool * pstrpool;
	public:
		xstack<dataitem>* pstack;
		xcolmgr * m_colmgr;
		xtuplemgr * m_tuplemgr;
		xdatamgr * m_datamgr;
		xstack<dataitem>* m_pdata;
		int	m_pc;
		int m_bp;
		int m_blockid;
		int m_symblockid;
	public:
		int m_hSymId;
		int m_hSymItem;
		int m_hSymDtlId;
	public:
		xtuple*		m_psymtab;
		xstack<xtuple*>* m_psymstack;
	public:
		xtuple*		m_pAddr;
	public:
		xarrays<code *,code> * m_codes;
		//xarrays<code *,code> * m_codesEx;
		struct labels
		{
			int pc;
			tchar * label;
		};
		xarrays<labels*,labels> * m_labels;
	public:
		void initsymtab(xtuple* ptab);
		bool m_initsstate;
		void reset();
	public:
		void*  eval(code x,int pobj = 0);
		void*  exec(bool step= false);
		void*  exec(int pc,bool step= false);
		void*  exec(int pc,int epc,bool step= false);
	
	public:
		bool call(xtuple  x,int nargs, xtuple * px=null);
	public:
		int loadfct(tchar * file, tchar * fname);

	public:
		void* processdcl(code cd,int vindex,  int htuple = -1,int  hpitem = -1);
		void*  processdcl(code cd,int vindex,  int htuple ,int  hpitem ,xarrays<code *,code> * pCodes ,int & pc);
		void* processfct(code cd);
		int processfct(code cd,xtuple * psym,xtuple &x,xarrays<code *,code> * pCodes,int &pc);
		//void* processclass(code cd);
		int processclass(code cd,xtuple * psym,xtuple &x);
	public:
		bool getmethod(xtuple * xc, tchar * f,xtuple &x,int n,xtuple &px);
		bool getfields(xtuple * xc, xtuple &x,bool bfirst = true,xtuple * px=null);
		bool issuper(tchar *child,tchar * super);
		bool isfromnative(tchar * sclass,xtuple * x);
	public:
		int showTuple( xtuple* t,bool data);
		int showTuple( xtuple* t,bool data, xstring &str,xcolinfo *pinfo=null,bool borigin = false);
		int showTuple(dataitem &d,bool data);
		int showTuple(dataitem &d,bool data, xstring &str);
	public :
		int toString(dataitem &d, xstring &str);
		int toString(tchar*  v, xstring &str);
	public:
		int output(tchar * ptext);
		xvmerror* m_error;
	public:
		dataitem v[16];
	public:
		int newXvmClass(tchar * pclass, int nativept = 0,bool bnative = false);
	public:
		bool m_bChecked;
	public:
		int error(LPTSTR pText);
	public:
		int m_level;
	};
}

