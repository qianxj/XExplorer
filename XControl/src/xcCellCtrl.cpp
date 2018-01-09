#include "stdafx.h"
#include "xnode.hpp"
#include "xcellctrl.hpp"

extern Hxsoft::XFrame::XApp* g_pXApp;

namespace Hxsoft
{
	namespace XFrame 
	{

		XCellCtrl::XCellCtrl()
		{
			m_pCellFrame = new XCell::CFrame();
			XCell::CFrame::SetApplication(g_pXApp);

		}
		XCellCtrl::~XCellCtrl()
		{
			if(m_pCellFrame) delete m_pCellFrame;
		}
		int XCellCtrl::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
		{
			if(message==WM_PAINT)
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);
				if(m_pNode )
				{
					m_pCellFrame->m_AreaRect.left = m_pCellFrame->m_AreaRect.top = 0;
					m_pCellFrame->m_AreaRect.right = this->m_pNode->m_AreaRect.right - this->m_pNode->m_AreaRect.left;
					m_pCellFrame->m_AreaRect.bottom = this->m_pNode->m_AreaRect.bottom - this->m_pNode->m_AreaRect.top;
					m_pCellFrame->m_ClientRect = m_pCellFrame->m_AreaRect;
				}

				RECT	rc;
				HDC	hPaintDC;
				HBITMAP	hBmp;

				rc = m_pCellFrame->m_ClientRect;
				hPaintDC = ::CreateCompatibleDC(hdc);
				hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left + 1,rc.bottom - rc.top +1);
				hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

				HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
				::FillRect(hPaintDC,&rc,hBrush);
				::DeleteObject(hBrush);
		
				
				m_pCellFrame->Draw(hPaintDC);
				::BitBlt(hdc,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
				hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
				::DeleteObject(hBmp);
				::DeleteDC(hPaintDC); 

				

				EndPaint(hWnd, &ps);
				return 1;
			}
			if(message==WM_SETCURSOR)
			{
				BOOL bRet;
				bRet = m_pCellFrame->OnSetCursor(hWnd,(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam));
				if(bRet)return 1;
				return 0;
			}

			if(message==WM_NCHITTEST)
			{
				POINT	point;
				point.x =  MAKEPOINTS(lParam).x; 
				point.y =  MAKEPOINTS(lParam).y; 
				m_pCellFrame->OnNcHitTest(hWnd, point);  
				return 0;
			}
			m_pCellFrame->HandleMessage(hWnd,message,wParam,lParam);

			return XWinbase::HandleMessage(hWnd,  message,wParam, lParam);
		}
	}
}	
