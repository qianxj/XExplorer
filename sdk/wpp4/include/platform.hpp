#ifndef WPP_PLATFORM_H
#define WPP_PLATFORM_H

#include "os_win32.hpp"

typedef class os_t
{
public:
	static int allocateAlignment();
	static int pageSize();
	static int numberOfProcessers();

	//virtual manage
	static void virtual_alloc(void *p, size_t size);
	static void virtual_free(void *p);

	//heap manage
	static void* heap_alloc(void *p, size_t size);
	static void heap_free(void *p);
	
	//
	static void mem_copy(void * dest,const void * src, int size);

	//print out
public:
	static FARPROC load_module(const wchar_t* pdll, const wchar_t * pfn);
} os_t;

#endif //WPP_PLATFORM_H