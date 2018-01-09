#pragma once
#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame
{
	struct CacheItem
	{
		DWORD ver;
		std::wstring m_cacheFile;
	};

	class xfCacheMgr :public xbObject
	{
	public:
		xfCacheMgr(void);
		~xfCacheMgr(void);
	public:
		std::map<std::wstring, CacheItem> m_cacheFiles;
		int InsertCacheFile(LPTSTR pStrUrl,LPTSTR pCacheFile, DWORD ver = 0);
		LPCTSTR GetCacheFile(LPTSTR pStrUrl);
		DWORD GetCacheVersion(LPTSTR pStrUrl);
	public:
		CRITICAL_SECTION m_cs;
	};
}}
