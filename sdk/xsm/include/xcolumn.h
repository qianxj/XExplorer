#pragma once
#include "xvmdef.h"
#include "xalloc.h"
#include "xtree.h"

namespace xcode
{
	struct xcolinfo 
	{
		void * name;
		void * typref;
		int typ;
		int nsize;
		int dims;
		int arrayref;
		bool dimex;

		//tuple info
		int tstate; //tuple state
		int tupleid; //tupleid
		int typ0;
		xtuple psym;
	};

	struct xarrayinfo
	{
		int idxtype;
		int eltype;
		int low;
		int high;
		int elsize;
		int size;
		int ref;
		int next;
	};

	template class xarrays<xarrayinfo*,xarrayinfo>; 
	template class xtree<xcolinfo>;

	class xcolmgr
	{
	public:
		xcolmgr(void);
		~xcolmgr(void);
	public:
		xtree<xcolinfo> * m_ptree;
		xarrays<xarrayinfo*,xarrayinfo> * m_parrays;
	public:
		int newTuple(){ return m_ptree->newTree();}
		int addColInfo(int nTupleId,xcolinfo & colInfo,int nParent =-1);
		int insertColInfo(int nTupleId,xcolinfo & colInfo,int nParent =-1,int nAfter = (2<<30));
		int addArrayAtCol(xcolinfo &colInfo,xarrayinfo &arrayInfo);
		int addArrayAtCol(dataitem &di,xarrayinfo &arrayInfo);
		xcolinfo getColInfo(class xvmcore * v,int hTupleId,int hItem);
	};
}
