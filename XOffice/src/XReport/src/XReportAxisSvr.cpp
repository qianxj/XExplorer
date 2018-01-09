#include "stdafx.h"
#include "xofbase.h"
#include "XReportAxisSvr.hpp"
#include "xoptionsvr.hpp"
#include "xuxutil.h"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportAxisSvr::XReportAxisSvr(void)
	{
		this->m_pRowHeights = new XUtil::xuSummableCsArray< int, USHORT>( 0x01<<30, XOptionSvr::stdRowHeight);
		this->m_pColWidths  = new XUtil::xuSummableCsArray< int, USHORT>( 0x01<<30, XOptionSvr::stdColumnWidth);
		
		this->m_pRowHeights->SetValue(0,XOptionSvr::stdColHeaderHeight);
		this->m_pColWidths->SetValue(0,0,XOptionSvr::stdRowSelectorWidth);
	}
	XReportAxisSvr::~XReportAxisSvr(void)
	{
		if(m_pRowHeights) delete m_pRowHeights;
		if(m_pColWidths) delete m_pColWidths;
	}
	int XReportAxisSvr::GetColWidth(int column)
	{
		return m_pColWidths->GetValue(column);
	}
	int XReportAxisSvr::GetRowHeight(int row)
	{
		return m_pRowHeights->GetValue(row);
	}

	int XReportAxisSvr::SetColWidth(int column,int NewWidth)
	{
		m_pColWidths->SetValue(column,NewWidth);
		return 1;
	}
	int XReportAxisSvr::SetRowHeight(int row ,int NewHeight)
	{
		m_pRowHeights->SetValue(row,NewHeight);
		return 1;
	}

}}}}
