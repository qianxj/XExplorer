#include "StdAfx.h"
#include "..\include\XBIReportSheet.hpp"
#include "..\include\XBIReportSheetSvr.hpp"
#include "ibireport.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XBIReportSheet::XBIReportSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XBIReportSheetSvr;
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);
	}

	XBIReportSheet::~XBIReportSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}

	xbObject*  XBIReportSheet::GetInterface()
	{
		if(!m_pInterFace) m_pInterFace = new IBIReport;
		((IBIReport*)m_pInterFace)->m_pControl = this;
		return m_pInterFace;
	}
}}}}