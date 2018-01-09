#include "StdAfx.h"
#include "..\include\XGridReportSvr.hpp"
#include "xreportserializesvr.hpp"
#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XGridrReportSvr::XGridrReportSvr(void)
	{
		m_pSerializeSvr = new XReportSerializeSvr;
		XGridDataSvr * pDataSvr = new XGridDataSvr();
		pDataSvr->SetIAxisSvr(new XAxisSvr());
		pDataSvr->SetIContentSvr(new XXmlContentSvr());
		
		m_pDataSvr = pDataSvr;
		SetGridSvr(new XGridSvr());
		GetGridSvr()->SetGridDataSvr(pDataSvr);
		GetGridSvr()->SetGridSheetSvr(this);
		SetRowSelector(new XRowSelector(GetGridSvr(),pDataSvr->GetIAxisSvr(),pDataSvr->m_pStyleSvr));
		SetHeaderSelector(new XCellHeader(GetGridSvr(),pDataSvr->GetIAxisSvr(),pDataSvr->m_pStyleSvr));


		GetGridSvr()->AddRouterMessageFrom(dynamic_cast<XOfficeSheetSvr *>(this));
		GetRowSelector()->AddRouterMessageFrom(dynamic_cast<XOfficeSheetSvr *>(this));
		GetHeaderSelector()->AddRouterMessageFrom(dynamic_cast<XOfficeSheetSvr *>(this));
	}

	XGridrReportSvr::~XGridrReportSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr)delete m_pDataSvr;
		if(GetGridSvr())delete GetGridSvr();
		if(GetRowSelector())delete GetRowSelector();
		if(GetHeaderSelector())delete GetHeaderSelector();
	}

	int XGridrReportSvr::SetFRect(RECT rect)
	{
		XOfficeSheetSvr::SetFRect(rect);

		RECT	rectex;
		rectex = rect;
		rectex.top	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0) + 2;
		rectex.right = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0);
		GetRowSelector()->SetFRect(rectex);

		rectex = rect;
		rectex.bottom	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0)  ;
		rectex.left = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0) + 2;
		GetHeaderSelector()->SetFRect(rectex);

		rectex = rect;
		rectex.top	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0) + 3 ;
		rectex.left = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0) + 2; 
		GetGridSvr()->SetFRect(rectex);

		this->FRect = rect;

		return 1;
	}

	XAPI STDMETHODIMP_(TRect &) XGridrReportSvr::GetFRect()
	{
		return FRect;
	}

	XAPI STDMETHODIMP_(int) XGridrReportSvr::SetFRect(TRect &rect)
	{
		XOfficeSheetSvr::SetFRect(rect);

		RECT	rectex;
		rectex = rect;
		rectex.top	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0) + 2;
		rectex.right = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0);
		GetRowSelector()->SetFRect(rectex);

		rectex = rect;
		rectex.bottom	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0)  ;
		rectex.left = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0) + 2;
		GetHeaderSelector()->SetFRect(rectex);

		rectex = rect;
		rectex.top	 = rect.top + GetGridSvr()->GetGridDataSvr()->GetRowHeight(0) + 3 ;
		rectex.left = rect.left + GetGridSvr()->GetGridDataSvr()->GetColWidth(0) + 2; 
		GetGridSvr()->SetFRect(rectex);

		this->FRect = rect;

		return 1;
	}

	XAPI STDMETHODIMP_(IRowSelector *) XGridrReportSvr::GetRowSelector()
	{
		return m_pRowSelector;
	}
	XAPI STDMETHODIMP_(IHeaderSelector *) XGridrReportSvr::GetHeaderSelector()
	{
		return m_pHeaderSelector;
	}
	XAPI STDMETHODIMP_(void) XGridrReportSvr::SetRowSelector(IRowSelector * pRowSelector)
	{
		m_pRowSelector = pRowSelector;
	}	
	XAPI STDMETHODIMP_(void) XGridrReportSvr::SetHeaderSelector(IHeaderSelector * pHeaderSelector)
	{
		m_pHeaderSelector = pHeaderSelector;
	}
	XAPI STDMETHODIMP_(void) XGridrReportSvr::SetGridSvr(XGridSvr * pGridSvr)
	{
		m_pGridSvr = pGridSvr;
	}
	XAPI STDMETHODIMP_(XGridSvr *) XGridrReportSvr::GetGridSvr()
	{
		return m_pGridSvr;
	}

	XAPI STDMETHODIMP_(CXScrollBar *) XGridrReportSvr::GetVScrollbar()
	{
		return GetView()->m_pVScrollBar;
	}
	XAPI STDMETHODIMP_(CXScrollBar *) XGridrReportSvr::GetHScrollbar()
	{
		return GetView()->m_pHScrollBar;

	}
	XAPI STDMETHODIMP_(void) XGridrReportSvr::SetVScrollbar(CXScrollBar *)
	{
	}
	XAPI STDMETHODIMP_(void) XGridrReportSvr::SetHScrollbar(CXScrollBar *)
	{
	}

	XAPI STDMETHODIMP_(class XOfficeSheet *) XGridrReportSvr::GetView()
	{
		return m_pSheetCtrl;
	}

	int XGridrReportSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{

		RECT rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = FRect;
		//DrawBackGround(hPaintDC,rect);
		//DrawConer(hPaintDC,rect);

		if(GetGridSvr())
			GetGridSvr()->OnDraw(hPaintDC,rect);   
		if(GetHeaderSelector())
			GetHeaderSelector()->OnDraw(hPaintDC,rect);
		if(GetRowSelector())
			GetRowSelector()->OnDraw(hPaintDC,rect);
		return 1;
	}
	
	int XGridrReportSvr::TemplateLoaded(IXMLDOMElement * pElement,int state)
	{
		return 1;
	}

}}}}
