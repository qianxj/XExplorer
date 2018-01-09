#include "StdAfx.h"
#include "..\include\XPageSheetSvr.hpp"
#include "XPageSheetSerializeSvr.hpp"
#include "xPageSheetdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	XPageSheetSvr::XPageSheetSvr(void):m_ControlCreated(false)
	{
		SetRect(&FRect,0,0,0,0);
		m_pSerializeSvr = new XPageSheetSerializeSvr;
		m_pDataSvr = new XPageSheetDataSvr;
		this->AttachListon(WM_XCOMMAND,(FNEvent)&XPageSheetSvr::EvtXCommand);
	}

	XPageSheetSvr::~XPageSheetSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;

	}

	bool XPageSheetSvr::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
	{
		LPTSTR	pStrID = pEvent->xcommand.pStrID;

		if(pStrID && _tcscmp(pStrID,_T("xmTemplateUrlOK"))==0)
		{
			xfControl * pControl = ((xfControl *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("XPageTemplateUrl"));
			if(pControl)
			{
				TCHAR buf[512];
				::GetWindowText(pControl->m_hWnd,buf,512);
				if(_tcsstr(buf,_T("dev:xpage")) == buf)
				{
					TCHAR strCmd[512];
					::_stprintf_s(strCmd,512, _T("uri.hxsoft.com/xaserver/%s"),buf);
					xbXml xml;
					if(xfUriLoader::FetchXml(pControl->GetWin()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
					{
						if(((XPageSheetDataSvr *)m_pDataSvr)->m_pStrTemplate)
							delete ((XPageSheetDataSvr *)m_pDataSvr)->m_pStrTemplate;
						((XPageSheetDataSvr *)m_pDataSvr)->m_pStrTemplate = _tcsdup(buf);
						if(((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage)
						{
							((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage->m_pxfNode->RemoveControl();
							delete ((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage;
						}
						((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage = new xfXPage();

						IXMLDOMElement* pElement;
						xml.m_pXmlDoc->get_documentElement(&pElement);
						if(pElement)
						{
							((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage->Load(pElement);
							pElement->Release();
						}
						((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustRect(FRect);
						((XPageSheetDataSvr *)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustCtrlRect();
						::InvalidateRect(GetHWND(),NULL,false);
					}
				}
			}
			pEvent->dwRet = 1;
			return true;
		}
		return true;
	}


	int XPageSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		POINT pt;
		::GetViewportOrgEx(hPaintDC,&pt);
		if(pDrawRect && (FRect.right ==0 && FRect.bottom==0 && FRect.top==0 && FRect.left==0))
		{
			RECT rt ;
			::SetRect(&rt,0,0,0,0);
			rt.bottom = pDrawRect->bottom - pDrawRect->top;
			rt.right = pDrawRect->right - pDrawRect->left;
			SetFRect(rt);
		}
		if(pDrawRect)
		{
			::SetViewportOrgEx(hPaintDC,pDrawRect->left,pDrawRect->top,NULL);
			if(((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage)
				((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->DoDraw(hPaintDC);
			
			::SetViewportOrgEx(hPaintDC,pt.x,pt.y,NULL);
		}
		return 0;
	}

	int XPageSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		if(((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage)
		{
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->m_AreaRect = rc; 
			if(!m_ControlCreated)
			{
				((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->CreateNodeControl(m_pSheetCtrl->m_hWnd);
				m_ControlCreated = true;
			}
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustRect();
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustCtrlRect();
		}
		return 0;
	}
	
	int XPageSheetSvr::ShowSheet(bool bShow)
	{
		RECT rect;
		::SetRect(&rect,0,0,0,0);
		if(bShow) rect = FRect;
		if(((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage)
		{
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->m_AreaRect = rect;
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustRect();
			((XPageSheetDataSvr*)m_pDataSvr)->m_pxfXPage->m_pxfNode->AdjustCtrlRect();
		}
		return 1;
	}

}}}}
