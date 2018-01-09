#pragma once
#include <memory.h>
#include <stdlib.h>

namespace xcode
{
	template<class T>
	inline T * xalloc(int nsize,bool tozero = true)
	{
		void * p = malloc(sizeof(T)*nsize);
		if(p && tozero) memset(p,0,sizeof(T)*nsize);
		return (T *)p;
	}

	template<class T>
	inline void  xfree(T * p) 
	{
		return free(p);
	}

	template<class T>
	inline void  xmemcpy(void* pSrc,void* pDesc,int nsize)
	{
		memcpy_s(pDesc,sizeof(T)*nsize,pSrc,sizeof(T)*nsize);
	}

	template<class T>
	inline void * xnew() 
	{
		return new T;
	}

	template<class T>
	inline void * xnew(int nsize)
	{
		return new T[nsize];
	}

	template<class T>
	inline void  xdelete(T* p)
	{
		return delete[] p;
	}
}
