#pragma once

#include "xvmdef.h"
#include "xcolumn.h"
#include "xtuple.h"

#define DEEPSREF 32
#define DEEPSDIM 16

namespace xcode
{
	struct xrefdim
	{
		int reftype; //0 all 1 dircet pos 2 expr 3 fromto 4 rangle  | 0x80->first combo
		union
		{
			int pos;
			int pc;
			int from;
			int * rangle;
		};
		int to;
		int rangles; //for rangle;
		bool		bcolinfo;
		xcolinfo	colinfo;
		int			hitem;
	};

	struct xrefitem
	{
		xcolinfo	colinfo;
		int			tupleid;
		int			hitem;
		int			npos;
		xrefdim		refdim;
		xrefdim*	refdims[DEEPSDIM]; //last item pointer to refdim
		int			dimcount; //count of refdims;
		void *		paddr; 
		bool		bfield;
	};

	struct xref
	{
		void*		paddr; //this is pointer last addr;
		xrefitem	refitem; 
		xrefitem*	refitems[DEEPSREF]; //last item pointer to refitem
		int			items; //count of refitems
		dataitem*	ptdata; //pointer stack data
	};

	bool  getdclv(xvmcore &v,dataitem& d, xtuple &u,int ntyp = 1,xtuple * psym = null,int args = -1);
	bool  EnsureRef(xvmcore &v,int nIndex);
	bool  EnsureRef(xvmcore &v,dataitem & d);
	
	bool  CompRef(xvmcore &v,int nIndex,lptstr pref);
	bool  CompRef(xvmcore &v,dataitem & d,lptstr pref);

	void* GetRefAddr(xvmcore &v,dataitem & d);
	int   GetRefType(xvmcore &v,dataitem & d);
}