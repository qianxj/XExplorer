#pragma once

#include "windows.h"
#include "tchar.h"
#include "xvm.h"
#include "xpcode.h"
#include <vector>
#include <assert.h>

using namespace std;

#define null NULL
#define byte char
#define tchar TCHAR
#define lptstr LPTSTR 
#define lpctstr LPCTSTR

namespace xcode
{
	struct xref;
	struct XVM_API xtuple
	{
		int colId;
		void * data;
	};

	struct XVM_API dataitem
	{
		int  typ;
		void * name;
		int dims;
		int arrayref;
		int tuptyp;
		union 
		{
			float  f;
			double d;
			int	   i;
			int	   ty;
			__int64 q;
			xtuple  x;
			void*  s;
		};
		xref * pref;
	};

	typedef union 
	{
		float  f;
		double d;
		int	   i; 
		int	   ty; //typeid
		__int64 q; //quard
		xtuple  x; //xtuple or list
		void*  s; //pointer string
	} xdi;
};



