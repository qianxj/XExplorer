#include "StdAfx.h"
#include "xOfficeSheet.hpp"
#include "xOfficeSheetsvr.hpp"
#include "xscrollbar.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeSheet::XOfficeSheet(void):m_pxfNodeCreated(false),m_pSheetSvr(NULL),
		m_pVScrollBar(NULL),m_pHScrollBar(NULL),m_Standalone(false),m_pAgentNodeTool(NULL)
	{
		this->AttachListon(WM_PAINT,(FNEvent)&XOfficeSheet::EvtPaint);
		this->AttachListon(WM_SIZE,(FNEvent)&XOfficeSheet::EvtSize);
		this->AttachListon(WM_ERASEBKGND,(FNEvent)&XOfficeSheet::EvtEraseBkgnd);
	}

	XOfficeSheet::~XOfficeSheet(void)
	{
	}

	bool XOfficeSheet::EvtEraseBkgnd(TEvent* pEvent,LPARAM lParam)
	{
		pEvent->dwRet = 1;
		return true;
	}

	bool XOfficeSheet::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		if(m_Standalone && m_pSheetSvr)
		{
			HDC hdc = pEvent->paint.hdc;

			RECT	rc;
			HDC	hPaintDC;
			HBITMAP	hBmp;

			rc = m_pSheetSvr->FRect;
			hPaintDC = ::CreateCompatibleDC(hdc);
			hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left,rc.bottom - rc.top );
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

			HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
			::FillRect(hPaintDC,&rc,hBrush);
			::DeleteObject(hBrush);

			this->DoDraw(hPaintDC);
			::BitBlt(hdc,0,0,rc.right - rc.left,rc.bottom - rc.top,hPaintDC,0,0,SRCCOPY);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hPaintDC); 
		}
		return true;
	}
	bool XOfficeSheet::EvtSize(TEvent* pEvent,LPARAM lParam)
	{
		UINT	nFlags = pEvent->size.nFlags;
		POINT	point = pEvent->size.point;
		RECT	rect;

		::SetRect(&rect,0,0,point.x ,point.y );
		if(m_pxfNodeCreated)
		{
			m_pxfNode->m_AreaRect = rect;
			m_pxfNode->m_ContentRect = rect;
		}
		if(m_pSheetSvr)
			m_pSheetSvr->SetFRect(rect);
		return true;
	}

	int XOfficeSheet::CreatexfNode()
	{
		if(m_pxfNodeCreated) return 0;
		if(!m_pxfNodeCreated) m_pxfNode = new xfNode;
		m_pxfNodeCreated = true;
		return 1;
	}
	int XOfficeSheet::DoDraw(HDC hdc,RECT * pDrawRect)
	{
		if(this->m_pSheetSvr)
		{
			if(pDrawRect)m_DrawRect  = *pDrawRect;
			return m_pSheetSvr->DoDraw(hdc,pDrawRect);
		}
		else
			return 0;
	}
	
	void XOfficeSheet::Initial()
	{
		if(m_pxfNode && m_pSheetSvr)
			m_pSheetSvr->LoadTemplate(m_pxfNode);

		m_pVScrollBar = new CXScrollBar(SB_VERT);
		m_pVScrollBar->m_pOwner = m_pSheetSvr;
		m_pHScrollBar = new CXScrollBar(SB_HORZ);
		m_pHScrollBar->m_pOwner = m_pSheetSvr;

		
		m_pVScrollBar->AddRouterMessageFrom((xfAreaObject *)this);
		m_pHScrollBar->AddRouterMessageFrom((xfAreaObject *)this);

	}

	int XOfficeSheet::FocusedSheet()
	{
		if(this->m_pSheetSvr->m_state & goRunState) return 1;

		//set focuscontrol
		xfNode* pNode = this->m_pxfNode->GetXPage()->GetAgentNode();
		TCHAR buf[80];
		::_stprintf_s(buf,80,_T("%sagent"),this->m_pStrClassName);
		if(pNode &&( pNode->GetXPage()->m_pFocusNodeControl != dynamic_cast<xfNodeControl *>(this->m_pxfNode) ||
			!pNode->GetNodeByName(buf,false)))
		{
			if(m_pAgentNodeTool)
			{
				m_pxfNode->GetXPage()->SetAgentNodeContent(pNode,m_pAgentNodeTool,m_pxfNode);
				if(m_pxfNode && pNode && pNode->GetXPage())
				{
					pNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(this->m_pxfNode);
					this->m_pxfNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(this->m_pxfNode);
				}
				return 1;
			}
			//switch agent information

			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			if(this->m_pSheetSvr->m_state & goRunState)
				::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\%s\\toolbarrun.xml"),path,this->m_pStrClassName); 
			else
				::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\%s\\toolbar.xml"),path,this->m_pStrClassName);

			xbXml xml;
			if(!xml.Load(buf))
			{
				::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\%s\\toolbar.xml"),path,this->m_pStrClassName);
				xml.Load(buf);
			}
			IXMLDOMElement * pElement;
			xml.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				 m_pAgentNodeTool = this->m_pxfNode->GetXPage()->SetAgentNodeContent(pNode,pElement,this->m_pxfNode);
				pElement->Release();
			}else
				this->m_pxfNode->GetXPage()->ClearAgentNodeContent(pNode);

			if(m_pxfNode && pNode && pNode->GetXPage())
			{
				pNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(this->m_pxfNode);
				this->m_pxfNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(this->m_pxfNode);
			}
		}
		return 1;
	}

	int XOfficeSheet::KillFocusedSheet()
	{
		xfNode* pNode = this->m_pxfNode->GetXPage()->GetAgentNode();
		pNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(this->m_pxfNode);
		if(pNode && pNode->GetXPage()->m_pFocusNodeControl != dynamic_cast<xfNodeControl *>(this->m_pxfNode))
		{
			pNode->GetXPage()->ClearAgentNodeContent();
			pNode->GetXPage()->m_pFocusNodeControl = NULL;
		}
		return 1;
	}

}}}
