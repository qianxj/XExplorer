#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class  XOFOBJECT_API IAxisSvr :
		public CSvrbase
	{
	public:
		virtual XAPI STDMETHODIMP_(int) GetColWidth(int column) = 0;
		virtual XAPI STDMETHODIMP_(int) GetRowHeight(int row) = 0;
		virtual XAPI STDMETHODIMP_(int) SetColWidth(int column,int value) = 0;
		virtual XAPI STDMETHODIMP_(int) SetRowHeight(int row ,int value) = 0;
		virtual XAPI STDMETHODIMP_(int) SetColWidth(int startcolumn,int endcolumn,int value) = 0;
		virtual XAPI STDMETHODIMP_(int) SetRowHeight(int startrow ,int endrow, int value) = 0;

	public:
		virtual XAPI STDMETHODIMP_(int) GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight) = 0;
		virtual XAPI STDMETHODIMP_(int) GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth) = 0;
	};
}}}