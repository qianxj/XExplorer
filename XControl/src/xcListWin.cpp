#include "StdAfx.h"
#include "..\include\xcListWin.hpp"

namespace Hxsoft{namespace XFrame
{
	xcListWin::xcListWin(void):m_nRptIndex(-1)
	{
		/*m_pStrPage=_T("<xframe style=\"background:#cecebe none #cecebe none\" caption=\"\" type=\"main\" winstate=\"\" rect=\"0,0,0,0\" local=\"cn\"><xreport name=\"xreport\"/></xframe>");
		::SetRect(&FRect,0,0,200,200);
		m_pSerializeSvr = new XOffice::XReport::XReportSerializeSvr();
		m_pDataSvrs = new std::vector<XOffice::XReport::XReportDataSvr *>;*/
	}

	xcListWin::~xcListWin(void)
	{
		//if(m_pSerializeSvr)delete m_pSerializeSvr;
		//if(m_pDataSvrs)delete m_pDataSvrs;
	}
	
	int  xcListWin::Show(bool bShow,RECT * pRect)
	{
		/*if(bShow)
		{
			if(pRect)
			if(!m_hWnd)
			{
				xfXPage* pPage = new xfXPage();
				pPage->LoadXPage(m_pStrPage);
				this->Open(pPage,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,NULL);
			}
			FRect = *pRect;
			if(m_nRptIndex >=0)
			{
				SwitchReport(m_nRptIndex);
				m_nRptIndex = -1;
			}
			::SetWindowPos(m_hWnd,HWND_TOPMOST,FRect.left,FRect.top,
				FRect.right - FRect.left,FRect.bottom - FRect.top,SWP_SHOWWINDOW|SWP_NOACTIVATE);
		}else
		{
			if(m_hWnd)
			{
				::IsWindowVisible(m_hWnd);
				ShowWindow(m_hWnd,SW_HIDE);
			}
		}*/
		return 1;
	}

	int xcListWin::SwitchReport(int nIndex)
	{
		/*if(m_hWnd)
		{
			XOffice::XReport::XReportSheetSvr* pReportSvr = (XOffice::XReport::XReportSheetSvr*)((XOffice::XReport::XReportSheet *)this->GetControl(_T("xreport")))->m_pSheetSvr;
			pReportSvr->RemoveRouterMessageTo(pReportSvr->GetReportDataSvr()->m_pRowSelectorSvr);
			pReportSvr->RemoveRouterMessageTo(pReportSvr->GetReportDataSvr()->m_pRightRowSelectorSvr);
			pReportSvr->m_pDataSvr = (*m_pDataSvrs)[nIndex];
			pReportSvr->AddRouterMessageTo(pReportSvr->GetReportDataSvr()->m_pRowSelectorSvr);
			pReportSvr->AddRouterMessageTo(pReportSvr->GetReportDataSvr()->m_pRightRowSelectorSvr);
			
			(*m_pDataSvrs)[nIndex]->m_pRowSelectorSvr->m_pSheetSvr = pReportSvr;
			(*m_pDataSvrs)[nIndex]->m_pRightRowSelectorSvr->m_pSheetSvr = pReportSvr;
			::InvalidateRect(m_hWnd,NULL,false);
		}else
			m_nRptIndex = nIndex;*/ 
		return 1;
	}

	int xcListWin::AddReport(LPTSTR pStr)
	{
		/*
		xbXml xml;
		xml.LoadXml(pStr);
		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);

		XOffice::XReport::XReportDataSvr * pDataSvr = new XOffice::XReport::XReportDataSvr();
		m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr);
		m_pDataSvrs->push_back(pDataSvr);

		pDataSvr->m_pAixsSvr = new XOffice::XReport::XReportAxisSvr;
		pDataSvr->m_pRowSelectorSvr = new XOffice::XReport::XRowSelectorSvr ;
		pDataSvr->m_pRowSelectorSvr->m_pAxisSvr = pDataSvr->m_pAixsSvr;
		pDataSvr->m_pRowSelectorSvr->m_pSheetSvr = NULL;
		pDataSvr->m_pRightRowSelectorSvr = new XOffice::XReport::XRowSelectorSvr ;
		pDataSvr->m_pRightRowSelectorSvr->m_pAxisSvr = pDataSvr->m_pAixsSvr;
		pDataSvr->m_pRightRowSelectorSvr->m_pSheetSvr = NULL;
		pDataSvr->m_pRightRowSelectorSvr->m_nWidth = 5;
		pDataSvr->m_pRightRowSelectorSvr->m_bDrawRowNo = false;

		pElement->Release();

		pDataSvr->m_pContentSvr->vxRetrieve(_T(""));*/
		return 1;
	}
	int xcListWin::SetReportData(int nIndex, LPTSTR pStr)
	{
		//(*m_pDataSvrs)[nIndex]->m_pContentSvr->vxLoadXml(pStr);
		return 1;
	}

}}
