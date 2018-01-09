#pragma once

#ifdef XSM_EXPORTS
#define XSM_API __declspec(dllexport)
#else
#define XSM_API __declspec(dllimport)
#endif

#pragma warning( disable: 4251 )
#pragma warning( disable: 4311 )
#pragma warning( disable: 4267 )
#pragma warning( disable: 4244 )

typedef int atom;
typedef int dword;
typedef  unsigned int uint;
typedef wchar_t * lptstr;
//typedef unsigned char Byte;
#if !defined(Byte)
	#define Byte unsigned char
#endif
#if !defined(tchar)
	#define tchar wchar_t
#endif


#if !defined(null)
	#define null 0
#endif

