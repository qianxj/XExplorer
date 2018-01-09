#include "StdAfx.h"
#include "..\include\XAxisSvr.hpp"
#include "xgridoption.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XAxisSvr::XAxisSvr()
	{
		m_pRowHeights = new XUtil::xuSummableCsArray< int, USHORT>( 1<<30, XGridOption::stdRowHeight);
		m_pColWidths  = new XUtil::xuSummableCsArray< int, USHORT>( 1<<30, XGridOption::stdColumnWidth);
		
		m_pRowHeights->SetValue(0,XGridOption::stdColHeaderHeight);
		m_pColWidths->SetValue(0,0,XGridOption::stdRowSelectorWidth); 
	}
	XAxisSvr::XAxisSvr(int stdColumnWidth,int stdRowHeight,int stdRowSelectorWidth,int stdColHeaderHeight):IAxisSvr()
	{
		m_pRowHeights = new XUtil::xuSummableCsArray< int, USHORT>( 1<<30, stdRowHeight);
		m_pColWidths  = new XUtil::xuSummableCsArray< int, USHORT>( 1<<30, stdColumnWidth);
		
		m_pRowHeights->SetValue(0, stdColHeaderHeight);
		m_pColWidths->SetValue(0,0,stdRowSelectorWidth);
	}

	XAxisSvr::~XAxisSvr(void)
	{
		if(m_pRowHeights) delete m_pRowHeights;
		if(m_pColWidths) delete m_pColWidths;  
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::GetColWidth(int column)
	{
		return m_pColWidths->GetValue(column);
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::GetRowHeight(int row)
	{
		return m_pRowHeights->GetValue(row);
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::SetColWidth(int column,int value)
	{
		m_pColWidths->SetValue(column,value);
		return 1;
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::SetRowHeight(int row ,int value)
	{
		m_pRowHeights->SetValue(row,value);
		return 1;
	}
	XAPI STDMETHODIMP_(int) XAxisSvr::SetColWidth(int startcolumn,int endcolumn,int value)
	{
		m_pColWidths->SetValue(startcolumn,endcolumn,value);
		return 1;
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::SetRowHeight(int startrow ,int endrow,int value)
	{
		m_pRowHeights->SetValue(startrow,endrow,value);
		return 1;
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight)
	{
		return m_pRowHeights->SumValues(startrow,endrow) + EffectiveLineHeight*(endrow - startrow);
	}

	XAPI STDMETHODIMP_(int) XAxisSvr::GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth)
	{
		return m_pColWidths->SumValues(startcol,endcol) + EffectiveLineWidth*(endcol - startcol);
	}

}}}
