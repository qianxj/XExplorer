#ifndef VLANG_VARRAYEX_H_
#define VLANG_VARRAYEX_H_

#include "varray.hpp"
namespace vl
{
	inline int pop()
	{
		varraymeta * p = getMetaBlock(cid);
		if(p->size>0)
		{
			p->size--;
		}
		return p->size;
	}

	inline int varray::push()
	{
		varraymeta * pMeta = getMetaBlock(cid);

		//hited?
		if(pMeta->ptHited)
		{
			if(pMeta->ptHitedIndex + pMeta->blockSize > pMeta->size)
			{
				if(pMeta->size!=pMeta->ptHited->size)pMeta->ptHited->size=pMeta->size; 
				//hited direct push data
				int nItem = pMeta->ptHited->size % getMetaBlock()->blockSize;
				pMeta->ptHited->data[nItem *pMeta->nElement]=val;
				pMeta->ptHited->size++;
				return  pMeta->size++;
			}
		}

		int nItem = pMeta->size % pMeta->blockSize;
		vArrayDataBlock * pLast = handleTo<vArrayDataBlock>(pMeta->ptLast);
		if(nItem==0) pLast = allocDataBlock(cid);
		pLast->size++;
		pLast->data[nItem*pMeta->nElement]=val;

		//setup hited
		pMeta->ptHited = pLast;
		pMeta->ptHitedIndex = pMeta->size - nItem ;

		return pMeta->size++;
	}
}
#endif
