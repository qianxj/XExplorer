#ifndef VLANG_VLMALLOC_H_
#define VLANG_VLMALLOC_H_

#define USE_TCMALLOC1
#ifdef USE_TCMALLOC
//use google tcmalloc to alloc memory
#include "tcmalloc.h"

namespace vl
{
	inline void*	vl_malloc(size_t size){return tc_malloc(size);}
	inline void		vl_free(void* ptr){return tc_free(ptr);}
	inline void*	vl_realloc(void* ptr, size_t size){return tc_realloc(ptr,size);}
	inline void*	vl_calloc(size_t nmemb, size_t size){return tc_calloc(nmemb,size);}
	inline void		vl_cfree(void* ptr){return tc_cfree(ptr);}

	inline void*	vl_memalign(size_t __alignment, size_t __size) {return tc_memalign(__alignment,__size);}
	inline int		vl_posix_memalign(void** ptr,  size_t align, size_t size){return tc_posix_memalign(ptr,align,size);}
	inline void*	vl_valloc(size_t __size){return tc_valloc(__size);}
	inline void*	vl_pvalloc(size_t __size){return tc_pvalloc(__size);}

	inline void		vl_malloc_stats(void){return tc_malloc_stats();}
	inline int		vl_mallopt(int cmd, int value){return vl_mallopt(cmd,value);}

	inline size_t	vl_malloc_size(void* ptr){return tc_malloc_size(ptr);}

	inline int		allocSpace(size_t size) { return (int)tc_calloc(1,size);}
	inline void *	allocSpaceEx(size_t size) { return tc_calloc(1,size);}
	inline void		freeSpace(void * p) { return tc_cfree(p);}
	inline void		freeSpace(int p) { return tc_cfree((void *)p);}
	template<class T> inline T * handleTo(void * p) {return (T*)p;}
	template<class T> inline T * handleTo(int p) {return (T*)p;}
	inline wchar_t*		allocString(wchar_t * p)
	{
		if(!p) return 0;
		int cx = 0;
		while(p[cx])cx++;
		wchar_t* s= (wchar_t *)tc_calloc(cx + 1,sizeof(wchar_t));
		while(p){*(s++)=*(p++);}
		return p - cx;
	} 
	inline void freeString(wchar_t * p) { if(p) tc_cfree(p);}
}
#else

#include "stdlib.h"
#include "memory.h"
namespace vl
{
	inline void*	vl_malloc(size_t size){return malloc(size);}
	inline void		vl_free(void* ptr){return free(ptr);}
	inline void*	vl_realloc(void* ptr, size_t size){return realloc(ptr,size);}
	inline void*	vl_calloc(size_t nmemb, size_t size){return calloc(nmemb,size);}
	inline void		vl_cfree(void* ptr){return free(ptr);}

	//inline void*	vl_memalign(size_t __alignment, size_t __size) {return memalign(__alignment,__size);}
	//inline int	vl_posix_memalign(void** ptr,  size_t align, size_t size){return posix_memalign(ptr,align,size);}
	//inline void*	vl_valloc(size_t __size){return valloc(__size);}
	//inline void*	vl_pvalloc(size_t __size){return pvalloc(__size);}

	//inline void	vl_malloc_stats(void){return malloc_stats();}
	//inline int	vl_mallopt(int cmd, int value){return mallopt(cmd,value);}

	//inline size_t	vl_malloc_size(void* ptr){return malloc_size(ptr);}

	inline int		allocSpace(size_t size) { return (int)calloc(1,size);}
	inline void *	allocSpaceEx(size_t size) { return calloc(1,size);}
	inline void		freeSpace(void * p) { return vl_cfree(p);}
	inline void		freeSpace(int p) { return vl_cfree((void *)p);}
	template<class T> inline T * handleTo(void * p) {return (T*)p;}
	template<class T> inline T * handleTo(int p) {return (T*)p;}
	inline wchar_t*		allocString(wchar_t * p)
	{
		if(!p) return 0;
		int cx = 0;
		while(p[cx])cx++;
		wchar_t* s= (wchar_t *)calloc(cx + 1,sizeof(wchar_t));
		while(p){*(s++)=*(p++);}
		return p - cx;
	} 
	inline void freeString(wchar_t * p) { if(p) vl_cfree(p);}
}
#endif

#endif