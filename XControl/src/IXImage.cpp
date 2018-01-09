#include "stdafx.h"
#include "IXImage.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXImage::IXImage():m_pServerUrl(NULL), m_pUrl(NULL)
	{
		
	}
	IXImage::~IXImage()
	{
		if(m_pServerUrl)delete m_pServerUrl;
		if(m_pUrl)delete m_pUrl;
	}
	int IXImage::GetWidth()
	{
		//return ((xcImage *)m_pControl)->m_pImage;
		return 0;
	}
	int IXImage::GetHeight()
	{
		return 0;
	}
	LPCTSTR  IXImage::SetUrl(LPCTSTR pServer,LPCTSTR pUrl)
	{
		m_pUrl = _tcsdup(pUrl);
		m_pServerUrl = _tcsdup(pServer);
		((xcImage *)m_pControl)->LoadFromUrl(pServer,pUrl);
		return NULL;
	}
	LPCTSTR  IXImage::GetUrl()
	{
		return m_pUrl;
	}
	LPCTSTR  IXImage::GetServerUrl()
	{
		return m_pServerUrl;
	}
	bool  IXImage::SaveToFile()
	{
		return ((xcImage *)m_pControl)->SaveToFile();
	}
	bool  IXImage::SaveToFile(LPTSTR pFile)
	{
		return ((xcImage *)m_pControl)->SaveToFile(pFile);
	}
	bool  IXImage::SaveToFileEx(LPTSTR pFile)
	{
		return ((xcImage *)m_pControl)->SaveToFileEx(pFile);
	}
	bool  IXImage::LoadFromUrl(LPCTSTR pServerUrl,LPCTSTR pUrl)
	{
		return ((xcImage *)m_pControl)->LoadFromUrl(pServerUrl,pUrl);
	}


}}