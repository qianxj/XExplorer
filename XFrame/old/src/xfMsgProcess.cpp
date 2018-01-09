#include "StdAfx.h"
#include "xfApp.hpp"
#include "..\include\xfMsgProcess.hpp"

namespace Hxsoft{ namespace XFrame
{
	xbObject * xfMsgProcess::m_CaptureObject = NULL;
	xfMsgProcess::xfMsgProcess(void)
	{
	}

	xfMsgProcess::~xfMsgProcess(void)
	{
	}

	int xfMsgProcess::ProcessMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
	{
		int nRet = HandleMessage(hWnd,message,wParam,lParam);
		if(nRet > 0) return nRet;

		switch(message)
		{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);
				OnPaint(hdc);
				EndPaint(hWnd, &ps);
				break;
			}
			break;
		case WM_SIZE:
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				OnSize(nFlags,point);
				break;
			}
			break;
		case WM_ERASEBKGND:
			{
				HDC hdc = (HDC) wParam;
				return OnEraseBkgnd(hdc);
			}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			{
				if(this->IsNotMeCaptured())break;
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				
				RECT rc;
				this->GetClientRect(&rc);
				if(::PtInRect(&rc,point) || this->IsCaptured())
				{
					if( message ==WM_LBUTTONDOWN) OnLButtonDown(nFlags,point);
					else if (message ==WM_RBUTTONDOWN) OnRButtonDown(nFlags,point);
					else if (message ==WM_LBUTTONUP) OnLButtonUp(nFlags,point);
					else if (message ==WM_RBUTTONUP) OnRButtonUp(nFlags,point);
					else OnMouseMove(nFlags,point);
				}
				break;
			}
		case WM_SETCURSOR:
			{
				HWND	hWnd = (HWND)wParam;
				UINT	nHitTest = LOWORD(lParam);
				UINT	mouseMsg = HIWORD(lParam);
				OnSetCursor(hWnd,nHitTest,mouseMsg);
				break;
			}
		case WM_HSCROLL:
			{
				UINT nSBCode = LOWORD(wParam);
				UINT nPos	= HIWORD(wParam);
				HWND hWndCtl = (HWND)lParam;
				OnHScroll(nSBCode, nPos,hWndCtl);
				break;
			}
		case WM_VSCROLL:
			{
				UINT nSBCode = LOWORD(wParam);
				UINT nPos	= HIWORD(wParam);
				HWND hWndCtl = (HWND)lParam;
				OnVScroll(nSBCode, nPos,hWndCtl);
				break;
			}
		case WM_MOUSEWHEEL:
			{
				POINT point;
				UINT_PTR	fwKeys;
				short zDelta;
				fwKeys = GET_KEYSTATE_WPARAM(wParam);
				zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;
				OnMouseWheel(point,fwKeys,zDelta);
				break;
			}
		case WM_CHAR:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				OnChar(nChar,nRepCnt,nFlags);
				break;
			}
		case WM_KEYDOWN: 
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				OnKeyDown(nChar,nRepCnt,nFlags);
				break;
			}
		case WM_SETFOCUS: 
			{
				HWND hHwndLost = (HWND)wParam;
				OnSetFocus(hHwndLost);
				break;
			}
		case WM_KILLFOCUS:
			{
				HWND hHwndFocus = (HWND)wParam;
				OnKillFocus(hHwndFocus);
				break;
			}
		}
		return 0;
	}

	int xfMsgProcess::ProcessCmdMessage(UINT message, int wmId, int wmEvent)
	{
		int nRet = HandleCmdMessage(message,wmId,wmEvent);
		if(nRet > 0) return nRet;
		return 0;
	}

	int  xfMsgProcess::SetCapture()
	{
		int nRet =1;
		EnterCriticalSection(&xfApp::GetApp()->m_csCapture);
		if(m_CaptureObject==this) goto SetCaptureEnd;
		if(m_CaptureObject)
		{
			nRet = -1;
			goto SetCaptureEnd;
		}
		m_CaptureObject =  this;
		::SetCapture(this->GetHWnd());
SetCaptureEnd:
		LeaveCriticalSection(&xfApp::GetApp()->m_csCapture);
		return nRet;
	}

	int  xfMsgProcess::ReleaseCapture()
	{
		int nRet =1;
		EnterCriticalSection(&xfApp::GetApp()->m_csCapture);
		if(m_CaptureObject && m_CaptureObject != this)
		{
			nRet = -1;
			goto ReleaseCaptureEnd;
		}
		if(m_CaptureObject==this)
		{
			m_CaptureObject =  NULL;
			::ReleaseCapture();
		}
ReleaseCaptureEnd:
		LeaveCriticalSection(&xfApp::GetApp()->m_csCapture);
		return nRet;
	}

	bool xfMsgProcess::IsCaptured()
	{
		if(m_CaptureObject==this) 
			return true;
		else
			return false;
	}

	bool xfMsgProcess::HasCaptured()
	{
		if(m_CaptureObject) 
			return true;
		else
			return false;
	}

	xbObject *  xfMsgProcess::GetCaptured()
	{
		return m_CaptureObject;
	}

	bool xfMsgProcess::IsNotMeCaptured()
	{
		if(m_CaptureObject && m_CaptureObject != this) 
			return true;
		else
			return false;
	}
}}
