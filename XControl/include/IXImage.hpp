#pragma once

#include "xframe.h"
#include "xcImage.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXImage : public IXFControl
	{
	public:
		IXImage();
		~IXImage();
	public:
		int GetWidth();
		int GetHeight();
	public:
		LPCTSTR  SetUrl(LPCTSTR pServer,LPCTSTR pUrl);
		LPCTSTR  GetUrl();
		LPCTSTR  GetServerUrl();
	public:
		LPTSTR m_pServerUrl;
		LPTSTR m_pUrl;
	public:
		bool SaveToFile();
		bool SaveToFile(LPTSTR pFile);
		bool SaveToFileEx(LPTSTR pFile);
		bool LoadFromUrl(LPCTSTR pServerUrl,LPCTSTR pUrl);

	};
}}