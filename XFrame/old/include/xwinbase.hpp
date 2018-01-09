#pragma once
#include "xbase.hpp"

namespace Hxsoft{
	namespace XFrame{
		class XFRAME_API XWinbase: public XBase
		{
		public:
			// window handle
			HWND m_hWnd;

		public:
			XWinbase(void):m_hWnd(NULL){};
		public:
			~XWinbase(void){};
		public:
			// create window
			HWND Create(DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, XWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
			HWND CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, XWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
			HWND CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
			virtual int HandleCommandMessage(UINT message, int wmId, int wmEvent);
			virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
			virtual int OnCreate(UINT message,WPARAM wParam, LPARAM lParam);
		};
	}
}