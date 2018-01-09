#pragma once
#include "xofbase.h"
#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XGridrReportSvr :
		public XOfficeSheetSvr,public IGridSheetSvr
	{
	public:
		XGridrReportSvr(void);
		~XGridrReportSvr(void);

		virtual int SetFRect(RECT rc);
	public:
		virtual XAPI STDMETHODIMP_(TRect &) GetFRect();
		virtual XAPI STDMETHODIMP_(int) SetFRect(TRect &rect);
	
	protected:
		IRowSelector * m_pRowSelector;
		IHeaderSelector * m_pHeaderSelector;
		XGridSvr*	m_pGridSvr;
	public:
		virtual XAPI STDMETHODIMP_(IRowSelector *) GetRowSelector();
		virtual XAPI STDMETHODIMP_(IHeaderSelector *) GetHeaderSelector();
		virtual XAPI STDMETHODIMP_(void) SetRowSelector(IRowSelector * pRowSelector);
		virtual XAPI STDMETHODIMP_(void) SetHeaderSelector(IHeaderSelector * pHeaderSelector);
		virtual XAPI STDMETHODIMP_(void) SetGridSvr(XGridSvr * pGridSvr);
		virtual XAPI STDMETHODIMP_(XGridSvr *) GetGridSvr() ;
	public:
		virtual XAPI STDMETHODIMP_(CXScrollBar *) GetVScrollbar();
		virtual XAPI STDMETHODIMP_(CXScrollBar *) GetHScrollbar();
		virtual XAPI STDMETHODIMP_(void) SetVScrollbar(CXScrollBar *);
		virtual XAPI STDMETHODIMP_(void) SetHScrollbar(CXScrollBar *);
	public:
		virtual XAPI STDMETHODIMP_(class XOfficeSheet *) GetView();

	public:
		virtual HWND GetHWND(){return GetView()->GetHWND();}
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		virtual int TemplateLoaded(IXMLDOMElement * pElement,int state=goNormalState);

	};
}}}}
