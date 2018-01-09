#include "StdAfx.h"
#include "..\include\XReportSheet.hpp"
#include "..\include\XReportSheetSvr.hpp"
#include "ixreport.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportSheet::XReportSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XReportSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);

		
	}

	XReportSheet::~XReportSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}

	xbObject * XReportSheet::QueryItem(LPTSTR pItem)
	{
		if(_tcsicmp(pItem,_T("svr:data"))==0)
			return ((XReportDataSvr*)m_pSheetSvr->m_pDataSvr)->m_pContentSvr;

		return  XOfficeSheet::QueryItem(pItem);
	}

	xbObject*  XReportSheet::GetInterface()
	{
		if(!m_pInterFace)
		{
			m_pInterFace = new IXReport();
			((IXReport *)m_pInterFace)->m_pSheetSvr = (XReportSheetSvr *)this->m_pSheetSvr;
			((IXReport *)m_pInterFace)->m_pControl = this;
		}
		return m_pInterFace;
	}
}}}}
