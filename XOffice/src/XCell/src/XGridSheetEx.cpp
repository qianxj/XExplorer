#include "StdAfx.h"
#include "..\include\XGridSheetEx.hpp"
#include "xgridsheetsvrex.hpp"
#include "IGridReport.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridSheetEx::XGridSheetEx(void): m_pGridReport(NULL)
	{
		m_Standalone = true;
		m_pSheetSvr = new XGridSheetSvrEx;
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);

	}

	XGridSheetEx::~XGridSheetEx(void)
	{
		if(m_Standalone)
		{
			m_pVScrollBar->RemoveRouterMessageFrom((xfAreaObject *)this);
			m_pHScrollBar->RemoveRouterMessageFrom((xfAreaObject *)this);
		}

		if(m_pSheetSvr)delete m_pSheetSvr;
		if(m_pGridReport)delete m_pGridReport;

		if(m_Standalone)
		{
			if(m_pVScrollBar)delete m_pVScrollBar;
			if(m_pHScrollBar)delete m_pHScrollBar;
		}
	}

	xbObject*  XGridSheetEx::GetInterface()
	{
		if(!m_pGridReport) m_pGridReport = new IGridReport;
		m_pGridReport->m_pGridReport = ((XGridSheetSvrEx *)m_pSheetSvr)->m_pGridReport;
		m_pGridReport->m_pControl = this;
		return m_pGridReport;
	}
	
}}}}
