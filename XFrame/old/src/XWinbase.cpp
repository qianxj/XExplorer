#include "stdafx.h"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		// create window
		HWND XWinbase::Create(DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, XWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam )
		{
			return CreateEx(NULL,dwStyle,pszWndClass,pszWndTitle,rtPos,pParentWin,uID,hInstance,hMenu,lpParam);
		}

		HWND XWinbase::CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, XWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam )
		{
			return CreateEx(NULL,dwStyle,pszWndClass,pszWndTitle,rtPos,pParentWin?pParentWin->m_hWnd:(HWND)NULL,uID,hInstance,hMenu,lpParam);
		}

		HWND XWinbase::CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam  )
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

		int XWinbase::HandleCommandMessage(UINT message, int wmId, int wmEvent)
		{
			return 0;
		}

		int XWinbase::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
		{
			return 0;
		}

		int XWinbase::OnCreate(UINT message,WPARAM wParam, LPARAM lParam)
		{
			return 0;
		}
	}
}
