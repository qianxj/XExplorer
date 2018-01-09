#include "stdafx.h"
#include "shlobj.h"
#include "xcPages.hpp"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		xcPages::xcPages()
		{
			m_bTransparent = true;
			m_bFocusButton = -1;
			m_CurPage = 1;
			m_MaxPage = 0;
			m_EditHWND = 0;
			this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcPages::EvtMouseLeave);
			this->AttachListon(WM_PAINT,(FNEvent)&xcPages::EvtPaint);
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcPages::EvtMouseMove);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcPages::EvtLButtonDown);
		}
		xcPages::~xcPages()
		{
		}

		int xcPages::SetMaxPage(int mpage)
		{
			m_MaxPage = mpage;
			RECT rect;
			this->GetClientRect(&rect);
			DrawFocusButtonRect();
			m_bFocusButton = -1;
			::InvalidateRect(this->m_hWnd,NULL,false);
			return 1;
		}
		int xcPages::SetCurPage(int cpage)
		{
			m_CurPage = cpage;
			RECT rect;
			this->GetClientRect(&rect);
			DrawFocusButtonRect();
			m_bFocusButton = -1;
			::InvalidateRect(this->m_hWnd,NULL,false);
			return 1;
		}
		int xcPages::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rect;
			if(pDrawRect)
				rect = *pDrawRect;
			else
				GetClientRect(&rect);

			::SetBkMode(hPaintDC,TRANSPARENT);
			
			LOGFONT	logFont;
			//const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
			//XUtil::Css::GetFont(pBoxStyle,logFont);
			XUtil::Css::GetDefaultFont(logFont);
			_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("ËÎÌו")) + 1, _T("ËÎÌו"));
	
			HFONT	hFont =::CreateFontIndirect(&logFont);
			hFont = (HFONT)::SelectObject(hPaintDC, hFont);

			COLORREF color = RGB(0,0,0);
			//if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
			//	color = ::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);

			RECT xrect;
			::SetRect(&xrect, rect.left + 4, rect.top, rect.left + 24, rect.bottom); 
			::DrawText(hPaintDC,L"|<",2,&xrect, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);

			::SetRect(&xrect, rect.left + 24, rect.top, rect.left + 44, rect.bottom); 
			::DrawText(hPaintDC,L"<",1,&xrect, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);

			::SetRect(&xrect, rect.left + 44, rect.top, rect.left + 64, rect.bottom); 
			::DrawText(hPaintDC,L">",1,&xrect, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);

			::SetRect(&xrect, rect.left + 64, rect.top, rect.left + 84, rect.bottom); 
			::DrawText(hPaintDC,L">|",2,&xrect, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);

			TCHAR str[100]; 
			_stprintf_s(str,100,_T("   %d/%d          Go") ,m_CurPage, m_MaxPage);
			LPCWSTR txt = str;
			::SetRect(&xrect, rect.left + 84, rect.top, rect.left + 284, rect.bottom); 
			::DrawText(hPaintDC,txt,(int)::_tcslen(txt),&xrect, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);

			SIZE si;
			::GetTextExtentPoint32(hPaintDC,txt, (int)::_tcslen(txt), &si);
			m_EditPosition = si.cx + 24;

			if(m_EditHWND > 0)
				::DestroyWindow(m_EditHWND);
			HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(m_hWnd, GWLP_HINSTANCE);
			_stprintf_s(str,10,_T("%d") ,m_CurPage);
			txt = str;
			int top = rect.top;
			int height = rect.bottom - rect.top;
			if(height > si.cy+4)
			{
				top = top + (height - si.cy -4)/2;
				height = si.cy + 4;
			}
			m_EditHWND = ::CreateWindowEx(
				 0,
				 _T("EDIT"),
				 txt,
				 WS_VISIBLE|WS_CHILD|ES_CENTER|ES_NUMBER,
				 m_EditPosition, top, 40, height,
				 m_hWnd,
				 NULL,
				 hInstance,
				 reinterpret_cast<LPSTR>(this));
			HDC eHdc = ::GetDC(m_EditHWND);
			::SetBkMode(eHdc,TRANSPARENT);
			//::SetTextColor(::GetDC(m_EditHWND), color);
			//::SetBkColor(eHdc, RGB(255,0,0));
			//HFONT pFont = (HFONT)::SendMessage(m_hWnd,WM_GETFONT,0,0);
			::SendMessage(m_EditHWND,WM_SETFONT,(WPARAM)hFont,false);	
			::ReleaseDC(m_EditHWND, eHdc);

			hFont = (HFONT)::SelectObject(hPaintDC, hFont);
			::DeleteObject(hFont);
			DrawFocusButtonRect();
			return 1;
		}

		bool xcPages::EvtPaint(TEvent* pEvent,LPARAM lParam)
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
				//::DeleteObject(hBmp);
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
		int xcPages::DrawFocusButtonRect()
		{
			if(m_bFocusButton < 0) 
				return -1;

			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());

			RECT xrect;
			xrect.top = rect.top;
			xrect.bottom = rect.bottom;
			xrect.left = 20 * m_bFocusButton + 4-m_bFocusButton;
			xrect.right = 20 * m_bFocusButton + 24-m_bFocusButton;

			if(m_bFocusButton == 99)
			{
				xrect.left = m_EditPosition+44;
				xrect.right = m_EditPosition+64;
			}
			DrawFocusRect(hdc,&xrect);
			::ReleaseDC(GetHWND(),hdc);
			return 1;
		}
		bool xcPages::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			//HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(this->m_hWnd,&pt);
			int x = pt.x;
			
			bool inButton = false;
			int i;
			for(i=0; i<4;i++)
			{
				if(x>(20*i+4-i) && x<(20*i+24-i))
				{
					inButton = true;
					if(i == m_bFocusButton) break;
					DrawFocusButtonRect();
					m_bFocusButton = i;
					DrawFocusButtonRect();
					break;
				}
			}

			if(!inButton && x > (m_EditPosition+44) && x < (m_EditPosition+64)){
				inButton = true;
				DrawFocusButtonRect();
				m_bFocusButton = 99;
				DrawFocusButtonRect();
			}

			if(!inButton){
				DrawFocusButtonRect();
				m_bFocusButton = -1;
			}
			return true;
		}
		bool xcPages::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
		{
			DrawFocusButtonRect();
			m_bFocusButton = -1;
			//::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}

		bool xcPages::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			//HDC hdc = ::GetDC(GetHWND());
			//::InflateRect(&rect,-1,-1);
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(this->m_hWnd,&pt);
			int x = pt.x;
			
			bool sendMsg = false;
			int i;
			int s = 4;
			for(i=0; i<s;i++)
			{
				if(x>(20*i+4-i) && x<(20*i+24-i))
				{
					sendMsg = true;
					switch(i){ 
						case 0: 
							m_CurPage = 1;
							break;
						case 1:
							m_CurPage = m_CurPage - 1;
							break;
						case 2:
							m_CurPage = m_CurPage + 1;
							break;
						case 3:
							m_CurPage = m_MaxPage;
							break;
					}
					break;
				}
			}
	
			if(x>(m_EditPosition+44) && x < (m_EditPosition+64))
			{
				sendMsg = true;
				int dwLen = GetWindowTextLength(m_EditHWND);
				if(dwLen < 1) return true;
				LPTSTR  pStr = new TCHAR[dwLen + 1];
				GetWindowText(m_EditHWND,pStr,dwLen + 1);
				//::SendMessage(m_EditHWND, WM_GETTEXT, (WPARAM)pStr, dwLen+1);
				m_CurPage =  _wtoi(pStr);
			}

			if(sendMsg)
			{
				if(m_CurPage > m_MaxPage) m_CurPage = m_MaxPage;
				if(m_CurPage < 1) m_CurPage = 1;

				PAGENMHDR hdr;
				memset(&hdr,0,sizeof(PAGENMHDR));
				hdr.cur = m_CurPage;
				hdr.max = m_MaxPage;
				hdr.hwndFrom = this->m_hWnd;
				hdr.idFrom = 0;
				hdr.code = PAGE_CLICK;
				::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&hdr);

				DrawFocusButtonRect();
				m_bFocusButton = -1;
				::InvalidateRect(m_hWnd,&rect,false);
			}
			return true;
		}
		void xcPages::Initial()
		{

		}

	}
}