#pragma once

#include "xbxbase.h"
#include "shlwapi.h"
#include "WinInet.h"
namespace Hxsoft{ namespace XFrame
{
	class  XFRAME_API xfUriLoader :
		public xbObject
	{
	public:
		xfUriLoader(void);
		~xfUriLoader(void);
	public:
		static class xfConnect* GetConnect(LPCTSTR pStrUrl,bool bReconnect = false);
		static class xfConnect* GetNewConnect(LPCTSTR pStrUrl);
		static int ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len);
		static int ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPVOID pExtStr,int nDataLen,BYTE * &pData,int &len,bool bcompress=true);
		static int FetchXml(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml);
		static int ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len,HWND hWnd);
	public:
		static int Load(LPCTSTR pStrUrl,LPCTSTR pStrUri, BYTE * &pData,int &len,bool bCache=true);
		static int Load(LPCTSTR pStrUrl,LPCTSTR pStrUri,LPCTSTR pExtStr, BYTE * &pData,int &len,bool bCache=true);
		static int FreeData(BYTE * pData);
		static int GetCacheFileName(LPCTSTR pStrUrl,LPTSTR pStrCacheFile,DWORD dwSize=MAX_PATH,bool Loaded=false);
		static int GetCacheFileName(LPCTSTR pStrUrl,LPCTSTR pExtStr,LPTSTR pStrCacheFile,DWORD dwSize=MAX_PATH,bool Loaded=false);
		static bool CacheFileExists(LPCTSTR pStrUrl,LPTSTR pStrCacheFile,DWORD dwSize=MAX_PATH);
		static bool UrlCombine(LPCTSTR pszBase,LPCTSTR pszRelative,LPTSTR pszCombined,LPDWORD pcchCombined,DWORD dwFlags=URL_ESCAPE_UNSAFE);
	public:
		static LPTSTR LoadScript(LPTSTR pStrUrl);
		static void	  FreeScript(LPTSTR pStrUrl);
		static FILETIME GetScriptDT(LPTSTR pStrUrl);
		static FILETIME GetScriptVER(LPTSTR pStrUrl);
	};
}}
