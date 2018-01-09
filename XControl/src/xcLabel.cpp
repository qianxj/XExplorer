#include "stdafx.h"
#include "xcLabel.hpp"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		xcLabel::xcLabel():m_bHover(false)
		{
			_variant_t var;
			//IXMLDOMElement * pElement = m_pxfNode->m_pElement;
			//pElement->getAttribute(L"",&var);

			m_bTransparent = true;
			this->AttachListon(WM_ERASEBKGND,(FNEvent)&xcLabel::EvtEraseBkgn);
			this->AttachListon(WM_PAINT,(FNEvent)&xcLabel::EvtPaint);
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcLabel::EvtMouseMove);
			this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcLabel::EvtMouseLeave);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcLabel::EvtLButtonUp);
		};

		xcLabel::~xcLabel()
		{
		};

		bool xcLabel::EvtEraseBkgn(TEvent* pEvent,LPARAM lParam)
		{
			pEvent->dwRet = 1;
			return true;
		}
		bool xcLabel::EvtPaint(TEvent* pEvent,LPARAM lParam)
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

			::SetViewportOrgEx(hMemDC,- pNode->m_AreaRect.left,- pNode->m_AreaRect.top,&pt);
			((xfNodeControl *)m_pxfNode)->DrawNodeEx(hMemDC);
			::SetViewportOrgEx(hMemDC,pt.x,pt.y,NULL);

			this->DoDraw(hMemDC);
			::BitBlt(hdc,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hMemDC,rect.left,rect.top,SRCCOPY);

			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hMemDC);

			return true;
		}
		bool xcLabel::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
		{
			if(!m_bHover)
			{
				TRACKMOUSEEVENT	tve;
				tve.cbSize = sizeof(TRACKMOUSEEVENT);
				tve.dwFlags = TME_LEAVE;
				tve.hwndTrack = m_hWnd;
				tve.dwHoverTime = HOVER_DEFAULT;

				this->m_bHover = true;
				TrackMouseEvent(&tve);
			}
			return true;
		}
		bool xcLabel::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
		{
			m_bHover = false;

			HDC hdc = ::GetDC(m_hWnd);
			RECT	rect;
			::GetClientRect(pEvent->hWnd,&rect);
			::ReleaseDC(pEvent->hWnd,hdc);
			return true;
		}
		bool xcLabel::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
		{
			if(m_pxfNode->m_strNodeName)
				SendMessage(::GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)m_pxfNode->m_strNodeName,(LPARAM)this);
			return true;
		}

		int xcLabel::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT	rect;
			if(pDrawRect) 
				rect = *pDrawRect;
			else
				GetClientRect(&rect);

			rect.left += 1;
			rect.top += 1; 

			if(((xfNodeControl *)m_pxfNode)->m_ImagePos >=0)
			{
				Hxsoft::XUtil::xuVector<HIMAGELIST> & hImageLists = m_pxfNode->GetXPage()->m_pResourceMgr->GetImageList();
				if(hImageLists.size()>0)
				{
					::ImageList_Draw(hImageLists[((xfNodeControl *)m_pxfNode)->m_ImageIndex],((xfNodeControl *)m_pxfNode)->m_ImagePos,hPaintDC, rect.left + 1,rect.top + (rect.bottom - rect.top)/2 - 8 ,ILD_NORMAL);
				}
			}

			//xfWin::OnPaint(hdc);
			if(m_pxfNode && m_pxfNode->m_pCaption)
			{
				::SetBkMode(hPaintDC,TRANSPARENT);
				LOGFONT	logFont;
				const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
				XUtil::Css::GetFont(pBoxStyle,logFont);

				COLORREF color;
				if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
					color = ::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
				DWORD Align = DT_LEFT;
				if(pBoxStyle && pBoxStyle->pText && pBoxStyle->pText->mask & cssMaskTextAlign)
				{
					if(pBoxStyle->pText->textalign==XUtil::Css::_center)Align = DT_CENTER;
				}
				//if(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
				//	XUtil::Css::DrawCssFrame(hPaintDC,&rect,pBoxStyle);

				HFONT	hFont =::CreateFontIndirect(&logFont);
				hFont = (HFONT)::SelectObject(hPaintDC, hFont);
				if(((xfNodeControl *)m_pxfNode)->m_ImagePos >=0) rect.left += 24;
				if(_tcschr(m_pxfNode->m_pCaption,'\r'))
					::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&rect, /*DT_SINGLELINE|*/DT_VCENTER/*|DT_END_ELLIPSIS|*/|DT_WORDBREAK);
				else
				{
					SIZE sz;
					GetTextExtentPoint32(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&sz);
					if(sz.cx < rect.right - rect.left)
						::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&rect, Align|DT_SINGLELINE|DT_VCENTER/*|DT_END_ELLIPSIS|*/|DT_WORDBREAK);
					else
						::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&rect, Align|/*DT_SINGLELINE|*/DT_VCENTER/*|DT_END_ELLIPSIS|*/|DT_WORDBREAK);
				}
				if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
					::SetTextColor(hPaintDC,color);

				hFont = (HFONT)::SelectObject(hPaintDC, hFont);
				::DeleteObject(hFont);
			}
			return 0;
		}
	}
}