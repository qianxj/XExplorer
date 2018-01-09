#pragma once

#include "xofbase.h"
#include "IRowSelector.hpp"
#include "IHeaderSelector.hpp"
#include "XGridSvr.hpp"
#include "IScrollbar.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class  XOFOBJECT_API IGridSheetSvr : public IBoundItem
	{
	public:
		virtual XAPI STDMETHODIMP_(TRect &) GetFRect()=0;
		virtual XAPI STDMETHODIMP_(int) SetFRect(TRect &rect)=0;
	public:
		virtual XAPI STDMETHODIMP_(IRowSelector *) GetRowSelector()=0;
		virtual XAPI STDMETHODIMP_(IHeaderSelector *) GetHeaderSelector()=0;
		virtual XAPI STDMETHODIMP_(void) SetRowSelector(IRowSelector * pRowSelector)=0;
		virtual XAPI STDMETHODIMP_(void) SetHeaderSelector(IHeaderSelector * pHeaderSelector)=0;
		virtual XAPI STDMETHODIMP_(void) SetGridSvr(class XGridSvr * pGridSvr)=0;
		virtual XAPI STDMETHODIMP_(class XGridSvr *) GetGridSvr()=0 ;
	public:
		virtual XAPI STDMETHODIMP_(class CXScrollBar *) GetVScrollbar()=0;
		virtual XAPI STDMETHODIMP_(class CXScrollBar *) GetHScrollbar()=0;
		virtual XAPI STDMETHODIMP_(void) SetVScrollbar(class CXScrollBar *)=0;
		virtual XAPI STDMETHODIMP_(void) SetHScrollbar(class CXScrollBar *)=0;
	public:
		virtual XAPI STDMETHODIMP_(class XOfficeSheet *) GetView()=0;
	};
}}}