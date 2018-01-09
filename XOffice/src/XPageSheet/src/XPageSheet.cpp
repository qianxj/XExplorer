#include "StdAfx.h"
#include "..\include\XPageSheet.hpp"
#include "..\include\XPageSheetSvr.hpp"
#include "xpagesheetdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	XPageSheet::XPageSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XPageSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);

	}

	XPageSheet::~XPageSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}
	
	int XPageSheet::FocusedSheet()
	{
		XOfficeSheet::FocusedSheet();
		if(((XPageSheetDataSvr *)m_pSheetSvr->m_pDataSvr)->m_pStrTemplate)
		{
			xfControl * pControl = this->GetWin()->GetFrameWindow()->GetControl(_T("XPageTemplateUrl"));
			if(pControl)
			{
				::SetWindowText(pControl->m_hWnd,((XPageSheetDataSvr *)m_pSheetSvr->m_pDataSvr)->m_pStrTemplate);
			}
		}
		return 1;
	}

}}}}
