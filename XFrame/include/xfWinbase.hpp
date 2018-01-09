#pragma once
#include "xbxbase.h"
#include "xfareaobject.hpp"

namespace Hxsoft{ namespace XFrame
{

	#define WST_RESPONSELOOP 0x0004
	#define WST_RESPONSEWIN 0x0008

	class xfNode;
	class XFRAME_API xfWinbase :
		public xfAreaObject
	{
	public:
		xfWinbase(void);
		~xfWinbase(void);
	public:
		HWND m_hWnd;
	public:
		DWORD m_dwStatus;

	public:
		// create window
		HWND Create(DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, xfWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
		HWND CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, xfWinbase * pParentWin, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
		HWND CreateEx(DWORD dwExStyle,DWORD dwStyle, LPTSTR pszWndClass, LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL );
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	public:
		static int SendMessageToChilds(HWND hWnd, UINT message,	WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyWin = TRUE);
	};
}}
