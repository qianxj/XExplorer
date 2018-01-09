#pragma once

namespace Hxsoft{ namespace XFrame{ namespace XCell
{
	class CXTabSheet;
	class CXNavigate
	{
	public:
		CXNavigate(void);
		~CXNavigate(void);

	public:
		HWND	Handle;
		RECT	FRect;
		CXTabSheet* m_pTabSheet;
	public:
		virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
		virtual int OnDraw(HDC hdc);
	public:
		void OnLButtonDown(UINT nFlags,POINT point);
		void OnLButtonUp(UINT nFlags,POINT point);
		void OnMouseMove(UINT nFlags,POINT point);
	};
}}}
