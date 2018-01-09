#include "StdAfx.h"
#include "..\include\XPageSheetDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	XPageSheetDataSvr::XPageSheetDataSvr(void):m_pxfXPage(NULL),m_pStrTemplate(NULL)
	{
	}

	XPageSheetDataSvr::~XPageSheetDataSvr(void)
	{
		if(m_pxfXPage) delete m_pxfXPage;
		if(m_pStrTemplate)delete m_pStrTemplate;
	}
}}}}
