#include "StdAfx.h"
#include "..\include\xcSpliterBar.hpp"

#include "xframe.h"
namespace Hxsoft
{
	namespace XFrame
	{
		xcSpliterBar::xcSpliterBar(void):m_pxfNodeOne(NULL),m_pxfNodeTwo(NULL),SizeState(_nosizing)
		{
			m_bTransparent = true;
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcSpliterBar::EvtMouseMove);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcSpliterBar::EvtLButtonUp);
			this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcSpliterBar::EvtLButtonDown);
			this->AttachListon(WM_SETCURSOR,(FNEvent)&xcSpliterBar::EvtSetCursor);
			this->AttachListon(WM_PAINT,(FNEvent)&xcSpliterBar::EvtPaint);
		}

		xcSpliterBar::~xcSpliterBar(void)
		{
		}

		int xcSpliterBar::DrawSizeBar(POINT pt)
		{
			HWND hWnd = ::GetParent(m_hWnd);
			HDC hDC = ::GetDC(hWnd);
			RECT rc = m_pxfNode->m_ContentRect;
			HBRUSH brush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
			if(m_pxfNode->m_pParentNode->m_LayerFlow==LayerFlow_WE)
			{
				::PatBlt(hDC, pt.x + rc.left, 
					rc.top,	rc.right - rc.left, rc.bottom - rc.top,PATINVERT);
			}else
			{
				::PatBlt(hDC,rc.left  , pt.y + rc.top,	rc.right - rc.left, rc.bottom - rc.top,PATINVERT);
			}
			brush = (HBRUSH)::SelectObject(hDC,brush);
			::DeleteObject(brush);
			::ReleaseDC(hWnd,hDC);
			return 1;
		}

		int xcSpliterBar::PrepareSizingNode()
		{
			for(int i=0;i<(int)m_pxfNode->m_pParentNode->m_pChilds->size();i++)
			{
				if((*m_pxfNode->m_pParentNode->m_pChilds)[i] != m_pxfNode) continue;
				if(i>0)
					m_pxfNodeOne = (*m_pxfNode->m_pParentNode->m_pChilds)[i - 1];
				if(i<(int)m_pxfNode->m_pParentNode->m_pChilds->size() - 1)
					m_pxfNodeTwo = (*m_pxfNode->m_pParentNode->m_pChilds)[i + 1];
			}
			return 1;
		}

		int xcSpliterBar::AdjustSplitePos(POINT pt)
		{
			//prepare resize node
			PrepareSizingNode();
			if(!m_pxfNodeOne || !m_pxfNodeTwo) return 0;

			//start adjust node rect
			int dx=0;
			int	dy=0;
			RECT rc = m_pxfNode->m_ContentRect;
			if(m_pxfNode->m_pParentNode->m_LayerFlow==LayerFlow_WE)
			{
				dx = pt.x;
				m_pxfNodeOne->m_AreaRect.right += dx;
				if(m_pxfNodeOne->m_width > 0)
				{
					m_pxfNodeOne->m_width +=  dx;
				}
				m_pxfNodeTwo->m_AreaRect.left += dx;
				if(m_pxfNodeTwo->m_width > 0)
				{
					m_pxfNodeTwo->m_width -=  dx;
				}
			}
			else
			{
				dy = pt.y;
				m_pxfNodeOne->m_AreaRect.bottom += dy;
				if(m_pxfNodeOne->m_height > 0)
				{
					m_pxfNodeOne->m_height +=  dy;
				}
				m_pxfNodeTwo->m_AreaRect.top += dy;
				if(m_pxfNodeTwo->m_height > 0)
				{
					m_pxfNodeTwo->m_height -=  dy;
				}
			}
			m_pxfNodeOne->CalcContentRect();
			m_pxfNodeOne->AdjustRect();
			m_pxfNodeOne->AdjustCtrlRect();
			m_pxfNodeTwo->CalcContentRect();
			m_pxfNodeTwo->AdjustRect();
			m_pxfNodeTwo->AdjustCtrlRect();

			::OffsetRect(&m_pxfNode->m_AreaRect,dx,dy);
			::OffsetRect(&m_pxfNode->m_ContentRect,dx,dy);
			m_pxfNode->AdjustCtrlRect();

			if(m_pxfNodeOne->GetXPage()->m_pWin)::InvalidateRect(m_pxfNodeOne->GetXPage()->m_pWin->m_hWnd,&m_pxfNodeOne->m_AreaRect,false);
			if(m_pxfNodeTwo->GetXPage()->m_pWin)::InvalidateRect(m_pxfNodeTwo->GetXPage()->m_pWin->m_hWnd,&m_pxfNodeTwo->m_AreaRect,false);

			return 1;
		}

		bool xcSpliterBar::EvtLButtonDown(TEvent* pEvent,LPARAM lParam  )
		{
			UINT nFlags = pEvent->mouse.nFlags;
			POINT point = pEvent->mouse.point;
			m_MousePt = point;
			DrawSizeBar(m_MousePt);
			this->SetCapture();
			SizeState=_sizing;
			return true;
		}
		bool xcSpliterBar:: EvtMouseMove(TEvent* pEvent,LPARAM lParam )
		{
			UINT nFlags = pEvent->mouse.nFlags;
			POINT point = pEvent->mouse.point;
			if(SizeState==_sizing)
			{
				DrawSizeBar(m_MousePt);
				m_MousePt = point;
				DrawSizeBar(m_MousePt);
			}
			m_MousePt = point;
			return true;
		}

		bool xcSpliterBar::EvtLButtonUp( TEvent* pEvent,LPARAM lParam )
		{
			UINT nFlags = pEvent->mouse.nFlags;
			POINT point = pEvent->mouse.point;
			if(SizeState==_sizing)
			{
				DrawSizeBar(m_MousePt);
				this->ReleaseCapture();
				SizeState=_nosizing;
				AdjustSplitePos(m_MousePt);
			}
			return true;
		}

		bool xcSpliterBar::EvtSetCursor( TEvent* pEvent,LPARAM lParam )
		{
			HWND hWnd =pEvent->hWnd;
			UINT nHitTest = pEvent->cursor.nHitTest;
			UINT message = pEvent->cursor.mouseMsg; 
			HCURSOR Cur = NULL;
			if(this->m_pxfNode->m_pParentNode->m_LayerFlow==LayerFlow_WE)
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crVSplit
			else
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crHSplit

			if(Cur != NULL)
			{
				SetCursor(Cur);
				pEvent->dwRet = true;
			}
			return true;
		}
		int xcSpliterBar::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT	rect;
			if(pDrawRect) 
				rect = *pDrawRect;
			else
				GetClientRect(&rect);
			LPCTSTR pCssStr = this->m_pxfNode->GetCssStyle();
			if(pCssStr)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
				if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
				{
					RECT rc;
					this->GetClientRect(&rc);
					XUtil::Css::DrawCssFrame(hPaintDC,&rect,pBoxStyle);
					delete pBoxStyle;
				}else
				{
					COLORREF bkcolor = RGB(227,237,227);// GetSysColor(15);
					HBRUSH hbrush;
					hbrush = ::CreateSolidBrush( bkcolor);
					::FillRect(hPaintDC,&rect,hbrush);
					::DeleteObject(hbrush);
				}
			}else
			{
				COLORREF bkcolor = RGB(227,237,227);// GetSysColor(15);
				HBRUSH hbrush;
				hbrush = ::CreateSolidBrush( bkcolor);
				::FillRect(hPaintDC,&rect,hbrush);
				::DeleteObject(hbrush);
			}
			return 1;
		}
		bool xcSpliterBar::EvtPaint(TEvent* pEvent,LPARAM lParam)
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
	}
}
