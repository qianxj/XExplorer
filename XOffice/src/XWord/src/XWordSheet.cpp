#include "StdAfx.h"
#include "XWordSheet.hpp"
#include "XWordSheetsvr.hpp"
#include "XWordView.hpp"
#include "ixword.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordSheet::XWordSheet(void):m_pIWordView(0)
	{
		m_Standalone = true;
#ifdef V1
		m_pSheetSvr = new XWordSheetSvr();
#elif V2
		m_pSheetSvr = new XWordSheetSvr();
#elif V3
		m_pSheetSvr = new XWordView();
#else
		m_pSheetSvr = new XWordView();
#endif

		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);
	}

	XWordSheet::~XWordSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}

	
	xbObject*  XWordSheet::GetInterface()
	{
		if(!m_pIWordView) m_pIWordView = new IXWord;
		m_pIWordView->m_pWordSvr = ((XWordView *)m_pSheetSvr);
		m_pIWordView->m_pControl = this;
		return m_pIWordView;
	}
}}}}
