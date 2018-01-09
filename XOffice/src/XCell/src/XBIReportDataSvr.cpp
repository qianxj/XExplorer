#include "StdAfx.h"
#include "..\include\XBIReportDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XBIReportDataSvr::XBIReportDataSvr(void):m_pObjects(new std::vector<xbObject*>)
	{
	}

	XBIReportDataSvr::~XBIReportDataSvr(void)
	{
		for(int i=0;i<(int)m_pObjects->size();i++)
		{
			if((*m_pObjects)[i])delete (*m_pObjects)[i];
		}
		delete m_pObjects;
	}
}}}}
