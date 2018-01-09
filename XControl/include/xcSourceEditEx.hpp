#pragma once
#include "xframe.h"

namespace Hxsoft{namespace XFrame
{
	class xcSourceEditEx :public xfWin
	{
	public:
		xcSourceEditEx(void);
		~xcSourceEditEx(void); 
	public:
		std::vector<LPTSTR> * m_pTexts;
	public:
		int LoadText(LPTSTR  pText);
	public:
		virtual bool EvtEraseBkgnd(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSize(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtPaint(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtVScroll(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtHScroll(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtMouseWheel(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonDown(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtMouseMove(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonUp(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonDblClk(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSetFocus(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtKeyDown(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtKeyUp(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtChar(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSetCursor(TEvent *pEvent,LPARAM lparam);

		int SetEventListions();
		int UnSetEventListions();

	public:
		BOOL OnEraseBkgnd(HDC pDC);
		void OnSize(UINT nType, int cx, int cy);
		void OnPaint();
		void OnVScroll(UINT nSBCode, UINT nPos, void* pScrollBar);
		void OnHScroll(UINT nSBCode, UINT nPos, void* pScrollBar);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
		void OnLButtonDown(UINT nFlags, POINT point);
		void OnMouseMove(UINT nFlags, POINT point);
		void OnLButtonUp(UINT nFlags, POINT point);
		void OnLButtonDblClk(UINT nFlags, POINT point);
		void OnSetFocus(HWND pOldWnd);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		BOOL OnSetCursor(HWND pWnd, UINT nHitTest, UINT message);

	private:
		HDC m_hMemDC;
		HBITMAP m_hMemBitmap;
		HBITMAP m_pOldMemBitmap;
		HFONT m_hFont;
		HFONT m_pOldFont;

		HWND  m_hListBox;
		COLORREF m_cBgColor;

		SIZE m_szTextExtent;
		SIZE m_szTextSize;

		POINT m_ptTopLeft;

	public:
		int Invalidate(BOOL bDraw);
		int UpdateWindow();
		int FillSolidRect(HDC hDC,LPCRECT lpRect, COLORREF clr);
	public:
		virtual int DoDraw(HDC hdc,RECT * pRect);

	};
}}
