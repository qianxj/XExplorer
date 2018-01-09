#pragma once

#include <vector>
#include <map>
using namespace std;

namespace Hxsoft
{
	namespace XFrame
	{
		namespace XUtil
		{
			int CalcCssStrToNumber(LPTSTR cssStr,int Radix = 10);
			int GetTokens( LPCTSTR StrSource, LPTSTR spStr, vector<LPTSTR> & StrTokens);
			COLORREF GetColourFromString( LPCTSTR strColour, COLORREF crDefault);

			LPSTR strdupFromBstr(BSTR bstr);
			LPWSTR wcsdupFromBstr(BSTR bstr);
			
			#ifdef UNICODE
				#define tcsdupFromBstr wcsdupFromBstr
			#else
				#define tcsdupFromBstr strdupFromBstr
			#endif
		}
	}
}