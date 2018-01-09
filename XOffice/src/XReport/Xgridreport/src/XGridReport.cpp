#include "StdAfx.h"
#include "..\include\XGridReport.hpp"
#include "XGridReportsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XGridReport::XGridReport(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XGridrReportSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);
	}

	XGridReport::~XGridReport(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;

	}
}}}}
