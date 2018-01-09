#pragma once

#include <vector>
#include <map>
#include "md5.h"

using namespace std;

namespace Hxsoft{ namespace XUtil
{
	int XUTIL_API Compress(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen);
	ULONG XUTIL_API CompressBound(ULONG nSrcLen);
	int XUTIL_API UnCompress (BYTE *dest,   ULONG *destLen,const BYTE *source, ULONG sourceLen);

	int XUTIL_API CalcCssStrToNumber(LPTSTR cssStr,int Radix = 10);
	int XUTIL_API GetTokens( LPCTSTR StrSource, LPTSTR spStr, vector<LPTSTR> & StrTokens);

	inline void XUTIL_API FreeStr(void * p)
	{
		if(p) delete p;
	}

	#ifdef UNICODE
		#define tcsdupFromBstr wcsdupFromBstr
	#else
		#define tcsdupFromBstr strdupFromBstr
	#endif

	inline LPSTR /*XUTIL_API*/ strdupFromBstr(BSTR bstr)
	{
		int _convert = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
		char* pszA = new char[_convert];
		int nRet = WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszA, _convert, NULL, NULL);
		return pszA;
	}
	inline LPWSTR /*XUTIL_API*/ wcsdupFromBstr(BSTR bstr)
	{
		LPWSTR pStr = new wchar_t[::wcslen(bstr)+1];
		::wcscpy_s(pStr,::wcslen(bstr)+1,bstr);
		return pStr;
	}

	LPSTR XUTIL_API MD5(LPSTR str);
	LPTSTR XUTIL_API MD5(LPTSTR str);
	int XUTIL_API SaveToFile(LPTSTR pFile,LPTSTR pData,LPTSTR ext, LPTSTR pFilter, LPTSTR pFileName=NULL);

	LPTSTR XUTIL_API ToXmlText(LPCTSTR str);
	LPTSTR XUTIL_API UnXMLText(LPCTSTR str);

	LPTSTR XUTIL_API Round(wchar_t* str, int part, bool &bfree, bool full = false);
	LPTSTR XUTIL_API FormatDecimalString(LPTSTR  str, LPTSTR format, bool & bfree);
}}