#pragma once

#include "xofbase.h"
#include "iAxisSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XAxisSvr :
		public IAxisSvr
	{
	public:
		XAxisSvr();
		XAxisSvr(int stdWidth,int stdHeight,int stdRowSelectorWidth,int stdColHeaderHeight);
		~XAxisSvr(void);
	
	private:
		XUtil::xuSummableCsArray< int, USHORT>*  m_pRowHeights;   
		XUtil::xuSummableCsArray< int, USHORT>*  m_pColWidths;
	
	public:
		virtual XAPI STDMETHODIMP_(int) GetColWidth(int column);
		virtual XAPI STDMETHODIMP_(int) GetRowHeight(int row);
		virtual XAPI STDMETHODIMP_(int) SetColWidth(int column,int value);
		virtual XAPI STDMETHODIMP_(int) SetRowHeight(int row ,int value);
		virtual XAPI STDMETHODIMP_(int) SetColWidth(int startcolumn,int endcolumn,int value);
		virtual XAPI STDMETHODIMP_(int) SetRowHeight(int startrow ,int endrow, int value);

	public:
		virtual XAPI STDMETHODIMP_(int) GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight);
		virtual XAPI STDMETHODIMP_(int) GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth);
	};
}}}
