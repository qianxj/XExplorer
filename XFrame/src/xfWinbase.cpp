#include "StdAfx.h"
#include "xfWinbase.hpp"
#include "xfApp.hpp"
#include "xfwin.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfWinbase::xfWinbase(void):m_hWnd(NULL),m_dwStatus(0)
	{
	}

	xfWinbase::~xfWinbase(void)
	{
	}
	// create window
	HWND xfWinbase::Create(DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, xfWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam )
	{
		return CreateEx(NULL,dwStyle,pszWndClass,pszWndTitle,rtPos,pParentWin,uID,hInstance,hMenu,lpParam);
	}

	HWND xfWinbase::CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, xfWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam )
	{
		return CreateEx(NULL,dwStyle,pszWndClass,pszWndTitle,rtPos,pParentWin?pParentWin->m_hWnd:(HWND)NULL,uID,hInstance,hMenu,lpParam);
	}

	HWND xfWinbase::CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam  )
	{
		HWND hWnd;
		hWnd = ::CreateWindowEx(dwExStyle,pszWndClass,pszWndTitle,dwStyle,
			rtPos.left,rtPos.top,rtPos.right - rtPos.left,rtPos.bottom - rtPos.top,
			hWndParent, hMenu,hInstance,lpParam==NULL?this:lpParam);
		if(hWnd)
		{
			m_hWnd = hWnd;
			return hWnd;
		}
		return HWND(NULL);
	}

	HWND  xfWinbase::GetHWND()
	{
		return m_hWnd;
	}

	bool  xfWinbase::GetClientRect(RECT *pRect) 
	{
		::GetClientRect(GetHWND(),pRect);
		return true;
	}
	
	int xfWinbase::SendMessageToChilds(HWND hWnd, UINT message,	WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyWin)
	{
		for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			if (bOnlyWin)
			{
				xfWin * pWin = dynamic_cast<xfWin *>((xfWinbase *)::GetProp(hWndChild,_T("this")));
				if (pWin)
				{
					pWin->ProcessMessage(pWin->m_hWnd,message,wParam,lParam);
				}
			}
			else
			{
				::SendMessage(hWndChild, message, wParam, lParam);
			}
			if (bDeep && ::GetTopWindow(hWndChild) != NULL)
			{
				SendMessageToChilds(hWndChild, message, wParam, lParam, bDeep,bOnlyWin);
			}
		}
		return 1;
	}
}}
