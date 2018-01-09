#ifndef WPP_DEBUGER_H
#define WPP_DEBUGER_H
#include "vlang4.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>

#define ASSERT(b) \
{					\
	if(!(b))			\
	{				\
		fwprintf(stderr,L"Asser failed %S,  file %S, line %d\r\n",#b,__FILE__,__LINE__);\
		assert(b);	\
	}	\
}		

#define CHECK(b,...) if(!(b))		\
	{							\
		fwprintf(stderr,L"Checked failed %S,  file %S, line %d\r\n",#b,__FILE__,__LINE__); \
		fwprintf(stderr, __VA_ARGS__);	\
		/*assert(b);*/	DEBUGBREAK		\
	}						  
#define DEBUGBREAK DebugBreak();

class VLANG4_API debuger
{
public:
	 debuger();
	~debuger();
public:
	std::vector<int> * method_stack_;
public:
	void enter_method(int fn);
	void leave_method(int fn);
public:
	static  debuger * get_debuger();
public:
	std::wstring get_method_stack_string();
};

#endif //WPP_DEBUGER_H