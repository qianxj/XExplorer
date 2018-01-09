#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	class XPageSheetDataSvr :
		public XOfficeData
	{
	public:
		XPageSheetDataSvr(void);
		~XPageSheetDataSvr(void);
	public:
		xfXPage * m_pxfXPage;
		LPTSTR m_pStrTemplate;

	};
}}}}
