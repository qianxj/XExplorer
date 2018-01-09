#include "StdAfx.h"
#include "..\include\XGridSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XGridSheetSvr::XGridSheetSvr(void)
	{
	}

	XGridSheetSvr::~XGridSheetSvr(void)
	{
	}

	XAPI STDMETHODIMP_(TRect &) GetFRect()
	{
	}
	XAPI STDMETHODIMP_(int) SetFRect(TRect &rect)
	{
	}

	XAPI STDMETHODIMP_(IRowSelector *) GetRowSelector()
	{
	}
	XAPI STDMETHODIMP_(IHeaderSelector *) GetHeaderSelector()
	{
	}
	XAPI STDMETHODIMP_(void) GetRowSelector(IRowSelector *)
	{
	}
	XAPI STDMETHODIMP_(void) GetHeaderSelector(IHeaderSelector *)
	{
	}
	XAPI STDMETHODIMP_(void) SetGridSvr(XGridSvr *)
	{
	}
	XAPI STDMETHODIMP_(XGridSvr *) GetGridSvr()
	{
	}

	XAPI STDMETHODIMP_(IScrollbar *) GetVScrollbar()
	{
	}
	XAPI STDMETHODIMP_(IScrollbar *) GetHScrollbar()
	{
	}
	XAPI STDMETHODIMP_(void) SetVScrollbar(IScrollbar *)
	{
	}
	XAPI STDMETHODIMP_(void) SetHScrollbar(IScrollbar *)
	{
	}

	XAPI STDMETHODIMP_(class XOfficeSheet *) GetView()
	{
	}
}}}
