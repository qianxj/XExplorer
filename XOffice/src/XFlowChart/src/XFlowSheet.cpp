#include "StdAfx.h"
#include "XFlowSheet.hpp"
#include "XFlowSheetSvr.hpp"
#include "iflowchart.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowSheet::XFlowSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XFlowSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);
	}

	XFlowSheet::~XFlowSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}
	xbObject*  XFlowSheet::GetInterface()
	{
		if(!m_pInterFace)
		{
			m_pInterFace = new IFlowChart();
			((IFlowChart *)m_pInterFace)->m_pFlowChart = this;
			((IFlowChart *)m_pInterFace)->m_pControl = this;
		}
		return m_pInterFace;
	}
}}}}
