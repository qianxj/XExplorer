#include "StdAfx.h"
#include "..\include\IBIReport.hpp"
#include "xbireportsheet.hpp"
#include "xbireportsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	IBIReport::IBIReport(void)
	{
	}

	IBIReport::~IBIReport(void)
	{
	}

	int IBIReport::Load(IXMLDOMElement * pElement)
	{
		return ((XBIReportSheet *)m_pControl)->m_pSheetSvr->LoadTemplate(pElement,goRunState);
	}

	int IBIReport::GetObjectsCount()
	{
		return ((XBIReportSheetSvr *)((XBIReportSheet *)m_pControl)->m_pSheetSvr)->GetObjectsCount();
	}

	xbObject * IBIReport::GetObjectByIndent(int nIndex)
	{
		return ((XBIReportSheetSvr *)((XBIReportSheet *)m_pControl)->m_pSheetSvr)->GetObjectByIndent(nIndex);
	}

}}}}
