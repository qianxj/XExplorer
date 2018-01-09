#include "StdAfx.h"
#include "..\include\xfCacheMgr.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfCacheMgr::xfCacheMgr(void)
	{
		InitializeCriticalSection(&m_cs);
	}

	xfCacheMgr::~xfCacheMgr(void)
	{
		DeleteCriticalSection(&m_cs);
	}

	int xfCacheMgr::InsertCacheFile(LPTSTR pStrUrl,LPTSTR pCacheFile, DWORD ver)
	{
		std::map<std::wstring,CacheItem>::iterator it;
		it = m_cacheFiles.find(pStrUrl);
		
		::EnterCriticalSection(&m_cs);
		if(it== m_cacheFiles.end())
		{
			std::pair<std::wstring,CacheItem> pr;
			pr.first = pStrUrl;
			pr.second.m_cacheFile = pCacheFile;
			pr.second.ver = ver;
			m_cacheFiles.insert(pr);
		}else
			(*it).second.ver = ver;
		::LeaveCriticalSection(&m_cs);

		return 1;
	}
	LPCTSTR xfCacheMgr::GetCacheFile(LPTSTR pStrUrl)
	{
		std::map<std::wstring,CacheItem>::iterator it;
		it = m_cacheFiles.find(pStrUrl);
		if(it== m_cacheFiles.end()) return NULL;
		return (*it).second.m_cacheFile.c_str();
	}
	DWORD xfCacheMgr::GetCacheVersion(LPTSTR pStrUrl)
	{
		std::map<std::wstring,CacheItem>::iterator it;
		it = m_cacheFiles.find(pStrUrl);

		if(it== m_cacheFiles.end()) return -1;
		return (*it).second.ver;
	}
}}
