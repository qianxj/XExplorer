#include "stdafx.h"
#include "xcZoomer.hpp"
#include "IXZoomer.hpp"

namespace Hxsoft{namespace XFrame
{
	xcZoomer::xcZoomer():m_pos(1),m_max(5),m_startx(2),m_starty(2),m_height(80),m_width(7),m_linewidth(1),m_dy(-1)
	{
		this->AttachListon(WM_PAINT,(FNEvent)&xcZoomer::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcZoomer::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcZoomer::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcZoomer::EvtMouseMove);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&xcZoomer::EvtMouseWheel);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&xcZoomer::EvtDoubleClicked);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&xcZoomer::EvtKeyDown);
	}

	xcZoomer::~xcZoomer()
	{
	}

	int xcZoomer::DoDraw(HDC hPaintDC,RECT * rc)
	{
		RECT rt ;
		if(rc)
			rt = *rc;
		else
			GetClientRect(&rt);

		LPCTSTR pCssStr = this->m_pxfNode->GetCssStyle();
		if(pCssStr)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
			if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
			{
				RECT rc;
				this->GetClientRect(&rc);
				XUtil::Css::DrawCssFrame(hPaintDC,&rt,pBoxStyle);
				delete pBoxStyle;
			}else
			{
				COLORREF bkcolor = RGB(255,255,255);// GetSysColor(15);
				HBRUSH hbrush;
				hbrush = ::CreateSolidBrush( bkcolor);
				::FillRect(hPaintDC,&rt,hbrush);
				::DeleteObject(hbrush);
			}
		}else
		{
			COLORREF bkcolor = RGB(255,255,255);// GetSysColor(15);
			HBRUSH hbrush;
			hbrush = ::CreateSolidBrush( bkcolor);
			::FillRect(hPaintDC,&rt,hbrush);
			::DeleteObject(hbrush);
		}

		int height = m_height;
		if(m_height==-1)
			m_height = rt.bottom - rt.top - 2* m_starty;

		::MoveToEx(hPaintDC,m_startx,m_starty,NULL);
		::LineTo(hPaintDC,m_startx + 13,m_starty);
		::LineTo(hPaintDC,m_startx + 13,m_starty + 10);
		::LineTo(hPaintDC,m_startx ,m_starty + 10);
		::LineTo(hPaintDC,m_startx ,m_starty);

		::MoveToEx(hPaintDC,m_startx + 7,m_starty + 2,NULL);
		::LineTo(hPaintDC,	m_startx + 7,m_starty + 9);
		::MoveToEx(hPaintDC,m_startx + 2,m_starty + 5,NULL);
		::LineTo(hPaintDC,	m_startx + 12,m_starty + 5);

		::MoveToEx(hPaintDC,m_startx + 7,m_starty + 10,NULL);
		::LineTo(hPaintDC, m_startx + 7,m_starty + height - 10);

		::MoveToEx(hPaintDC,m_startx,m_starty + height - 10,NULL);
		::LineTo(hPaintDC,m_startx + 13,m_starty + height - 10);
		::LineTo(hPaintDC,m_startx + 13,m_starty + height );
		::LineTo(hPaintDC,m_startx ,m_starty + height );
		::LineTo(hPaintDC,m_startx ,m_starty + height - 10);

		::MoveToEx(hPaintDC,m_startx + 2,m_starty + height - 5,NULL);
		::LineTo(hPaintDC,	m_startx + 12,m_starty + height - 5);

		m_dy = (m_height - 20)/(m_max + 1);
		for(int i=1;i<=m_max;i++)
		{
			if(i==m_pos)
			{
				POINT pt[5];
				pt[0].x = m_startx + 1;
				pt[0].y = m_starty + height - 10 - i * m_dy -2;
				pt[1].x = m_startx + 9;
				pt[1].y = m_starty + height - 10 - i * m_dy -2;
				pt[2].x = m_startx + 13;
				pt[2].y = m_starty + height - 10 - i * m_dy +2;
				pt[3].x = m_startx + 1;
				pt[3].y = m_starty + height - 10 - i * m_dy +2;
				pt[4].x = m_startx + 1;
				pt[4].y = m_starty + height - 10 - i * m_dy -2;

				BeginPath(hPaintDC);
				::Polyline(hPaintDC,pt,5);
				EndPath(hPaintDC);

				
				HRGN	hrgn;
				hrgn = PathToRegion(hPaintDC);
				hrgn = (HRGN)SelectObject( hPaintDC,hrgn);
				
				
				HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,0));
				::FillRect(hPaintDC,&rt,hBrush);
				::DeleteObject(hBrush);
				
				
				hrgn = (HRGN)SelectObject( hPaintDC,hrgn);
				DeleteObject( hrgn);

				SelectClipRgn(hPaintDC,0);
				::Polyline(hPaintDC,pt,5);
			}else
			{
				::MoveToEx(hPaintDC,m_startx + 1, m_starty + height - 10 - i * m_dy,NULL);
				::LineTo(hPaintDC,m_startx + 12, m_starty + height - 10 - i * m_dy);
			}
		}
		return 1;
	}

	void xcZoomer::Initial()
	{
		IXMLDOMElement * pElement = GetXfNode()->m_pElement;
		LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.startx"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_startx = _ttol(pStr);
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.starty"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_starty = _ttol(pStr);
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.width"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_width = _ttol(pStr);
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.height"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_height = _ttol(pStr);
			delete pStr;
		}else
		{
			if(pStr && _tcsicmp(pStr,L"auto")==0)
			{
				m_height = -1;
				delete pStr;
			}
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.linewidth"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_linewidth = _ttol(pStr);
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.pos"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_pos = _ttol(pStr);
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("zoomer.max"));
		if(pStr && _ttol(pStr) > 0)
		{
			m_max = _ttol(pStr);
			delete pStr;
		}
	}

	bool xcZoomer::EvtMouseWheel(TEvent* pEvent,LPARAM lParam)
	{
		if(pEvent->wheel.zDelta < 0)
			if(m_pos > 1) m_pos--;
		if(pEvent->wheel.zDelta > 0)
			if(m_pos < m_max) m_pos++;
		::InvalidateRect(GetHWND(),NULL,false);
		EventPosChanged(m_pos);
		return true;
	}


	bool xcZoomer::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		RECT	rc;
		HDC hPaintDC = pEvent->paint.hdc;
		GetClientRect(&rc);
		
		HDC hMemDC = ::CreateCompatibleDC(hPaintDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(hPaintDC,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);

		DoDraw(hMemDC,&rc);
		::BitBlt(hPaintDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top + 1,hMemDC,rc.left,rc.top,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hMemDC);
	
		return true;
	}

	bool xcZoomer::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		SetFocus(GetHWND());
		
		RECT rt;
		int height = m_height;
		GetClientRect(&rt);
		if(m_height==-1)height = rt.bottom - rt.top - 2*10;

		int x = pEvent->mouse.point.x;
		int y = pEvent->mouse.point.y;

		if( x >=m_startx && x <= m_startx + 13)
		{
			if(y > m_starty && y < m_starty + 10)
			{
				if(m_pos < m_max)
				{
					m_pos++;
					::InvalidateRect(GetHWND(),NULL,false);
					EventPosChanged(m_pos);
				}
			}else if(y > m_starty + height - 10 && y < m_starty + height)
			{
				if(m_pos > 1)
				{
					m_pos--;
					::InvalidateRect(GetHWND(),NULL,false);
					EventPosChanged(m_pos);
				}
			}else
			{
				int i = 0; 
				for(i=1;i <= m_max; i++)
				{
					if(abs(m_starty + height - 10 - i*m_dy - y)<5) break;
				}
				if(i<=m_max)
				{
					m_pos = i;
					::InvalidateRect(GetHWND(),NULL,false);
					EventPosChanged(m_pos);
				}
			}
		}
		return true;
	}

	int xcZoomer::EventPosChanged(int nNewPos)
	{
		ZMNMHDR hdr;
		memset(&hdr,0,sizeof(ZMNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = ZM_POSCHANGED;
		hdr.newPos = nNewPos;
		hdr.oldPos = m_pos;

		::SendMessage(GetWin()->m_hWnd,WM_NOTIFY,0,(LPARAM)&hdr);

		return 1;
	}


	bool xcZoomer::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
	{
		return true;
	}

	bool xcZoomer::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
	{
		return true;
	}

	bool xcZoomer::EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam)
	{
	
		return true;
	}
		
	bool xcZoomer::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	xbObject * xcZoomer::GetInterface()
	{
		if(!m_pInterFace)
		{
			m_pInterFace  = new IXZoomer;
			((IXFControl *)m_pInterFace)->m_pControl = this;
		}
		return this->m_pInterFace;
	}
}}