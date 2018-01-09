#include "stdafx.h"
#include "shlobj.h"
#include "xcScrollBox.hpp"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		xcScrollBox::xcScrollBox():m_wChild(NULL)
		{
			this->AttachListon(WM_PAINT,(FNEvent)&xcScrollBox::EvtPaint);
			this->AttachListon(WM_SIZE, (FNEvent)&xcScrollBox::EvtSize);
		}

		xcScrollBox::~xcScrollBox()
		{
			if(m_wChild)
				::CloseWindow(m_wChild->GetHWND());
			m_wChild = NULL;
			this->ClearAllListonsTo();
		}

		void xcScrollBox::Initial()
		{
			IXMLDOMElement * pElement = GetXfNode()->m_pElement;
			//Initial(pElement);
		}
		void xcScrollBox::Initial(IXMLDOMElement* pElement)
		{
			xfNode * pxfNode = this->GetXfNode();

			IXMLDOMNodeList*	pList;
			IXMLDOMNode*		pNode;
			IXMLDOMElement*     pElementEx;
			long				len;

			pElement->selectNodes(L"*",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				pList->get_item(i,&pNode);
				pElementEx = xbXml::GetElement(pNode);
				xfNode * pxfNodeEx = xfNode::CreateNode(pElementEx);
				if(pxfNodeEx)
				{
					pxfNodeEx->m_pParentNode = pxfNode;
					pxfNode->m_pChilds->push_back(pxfNodeEx);
					pxfNode->ParseNode(pElementEx);
				}
				pElementEx->Release();
				pNode->Release();
			}
			pxfNode->CreateNodeControl(this->GetHWND());
			pxfNode->ShowControl(true);
			pList->Release();
		}

		int xcScrollBox::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			::SetBkMode(hPaintDC,TRANSPARENT);

			if(!m_wChild) return 1;

			AdjustExtent();
			return 1;
		}
		bool xcScrollBox::EvtSize(TEvent *pEvent,LPARAM lparam)
		{
			//RECT	rc;
			UINT nFlags = pEvent->size.nFlags;
			POINT point = pEvent->size.point;
			return true;
		}
		bool xcScrollBox::EvtPaint(TEvent* pEvent,LPARAM lParam)
		{
			HDC hdc = pEvent->paint.hdc;
			RECT	rect;
			this->GetClientRect(&rect);

			HBITMAP hOldBmp = NULL;
			HBITMAP hBmp = NULL;
			POINT pt;

			xfNodeControl* pNode = (xfNodeControl *)m_pxfNode;
			HDC hMemDC = ::CreateCompatibleDC(hdc);
			
			hBmp = ::CreateCompatibleBitmap(hdc,rect.right - rect.left + 1,rect.bottom - rect.top +1);
			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			if(pNode->m_hBackGround)
			{
				HDC hDC = ::CreateCompatibleDC(hdc);
				HBITMAP hBmp = (HBITMAP)pNode->m_hBackGround;
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::BitBlt(hMemDC,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hDC,rect.left,rect.top,SRCCOPY);
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::DeleteDC(hDC);
			}

			::SetViewportOrgEx(hMemDC,- pNode->m_ContentRect.left,- pNode->m_ContentRect.top,&pt);
			((xfNodeControl *)m_pxfNode)->DrawNodeEx(hMemDC); 
			::SetViewportOrgEx(hMemDC,pt.x,pt.y,NULL);

			this->DoDraw(hMemDC);
			::BitBlt(hdc,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hMemDC,rect.left,rect.top,SRCCOPY);

			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hMemDC);

			return true;
		}
		int xcScrollBox::AdjustExtent()
		{
			if(m_wChild) UpdateScrollPos();			
			return 1;
		}
		SIZE xcScrollBox::GetContentExtent()
		{
			SIZE cz;
			if(m_wChild)
			{
				RECT rt;
				::GetClientRect(m_wChild->GetHWND(), &rt);
				cz.cx = rt.right - rt.left;
				cz.cy = rt.bottom - rt.top;
			}
			else
			{
				cz.cx = 0;
				cz.cy = 0;
			}
			return cz;
		}
		void xcScrollBox::UpdateScrollPos()
		{
			SIZE cz = GetContentExtent();
			SCROLLINFO si; 
			RECT rt;
			GetClientRect(&rt);
			if(rt.bottom - rt.top < cz.cy)
			{
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_RANGE | SIF_PAGE; 
				si.nMin   = 0; 
				si.nMax   = cz.cy ; 
				si.nPage  = rt.bottom - rt.top ; 
				SetScrollInfo(GetHWND(), SB_VERT, &si, TRUE); 
			}else
			{
				ShowScrollBar(GetHWND(),SB_VERT,false);
			}

			if(rt.right - rt.left < cz.cx)
			{
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_RANGE | SIF_PAGE; 
				si.nMin   = 0; 
				si.nMax   = cz.cx + 10 ;
				si.nPage  = rt.right - rt.left ; 
				SetScrollInfo(GetHWND(), SB_HORZ, &si, TRUE); 
			}else
			{
				ShowScrollBar(GetHWND(),SB_HORZ,false);
			}
		}

		void xcScrollBox::SetContent(LPTSTR strUrl, LPVOID pParam)
		{
			if(m_wChild)
				::CloseWindow(m_wChild->GetHWND());
			m_wChild = NULL;
			//m_wChild = xfWin::LoadXPage(NULL,strUrl,GetHWND(),xfApp::GetApp()->GetInstance(), pParam);
			
			BYTE* pData = NULL;
			int nLen = 0;

			if(xfUriLoader::Load(strUrl, NULL,pData,nLen)==1)
			{
				TCHAR Path[MAX_PATH];
				xfUriLoader::GetCacheFileName(strUrl,Path);

				xfXPage * pXPage = new xfXPage;
				pXPage->m_pStrUrl = _tcsdup(strUrl);
				pXPage->Load(Path);
				::SetProp(GetHWND(),_T("this"), NULL);
				m_wChild = xfWin::LoadXPageXml(pXPage, this->GetHWND(),xfApp::GetApp()->GetInstance(), pParam);
				::SetParent(m_wChild->GetHWND(), this->GetHWND());
				::SetProp(GetHWND(),_T("this"), this);
				long style = ::GetWindowLong(m_wChild->GetHWND(), GWL_STYLE);
				style = style &~WS_CAPTION & ~WS_BORDER;
				::SetWindowLong(m_wChild->GetHWND(), GWL_STYLE, style);
				//::ShowWindow(m_wChild->GetHWND(), SW_MAXIMIZE);	
				AdjustExtent();
			}
			if(pData)
				xfUriLoader::FreeData(pData);
		}

	}
}