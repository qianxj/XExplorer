#pragma once
#include <map>
#include <string>
using namespace std;

namespace Hxsoft{ namespace XFrame
{
	class XUTIL_API IXFUtil
	{
	public:
		IXFUtil(void);
		~IXFUtil(void);
	public:
		static HCURSOR  SetCursorWait();
		static int RestoreCursor(HCURSOR hCursor);
		
		static map<wstring,wstring>* getCacheMap();
		static const wchar_t* GetUrlData(LPWSTR url);
		static bool HasCacheData(LPWSTR url);
		static void CacheUrlData(LPWSTR url,LPWSTR data);

		static int SaveToFile(LPTSTR pFile,LPTSTR pData,LPTSTR ext, LPTSTR pFilter, LPTSTR pFileName=NULL);
		static LPWSTR MD5(LPWSTR pStr);
		static bool SmallImage(LPCTSTR lpszPathName, LPCTSTR lpszNewPathName, int width, int height, int flag=0);
		static LPWSTR OpenDialog(HWND Owner,LPWSTR str);

		static std::wstring MakeComboStyleClassName(std::wstring styleClass);
		static std::wstring EnsureComboStyleClassName(std::wstring styleClass,std::map<std::wstring,std::wstring> &items);
	};
}}