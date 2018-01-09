#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		struct 	PAGENMHDR : public NMHDR
		{
			int max;
			int cur;
		};
		#define	PAGE_CLICK  WM_USER + 0x201

		class XCONTROL_API xcPages:
			public xfControl
		{
		public:
			xcPages();
			~xcPages();
		public:
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
			bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			void Initial();
		public:
			int	m_bFocusButton;
			int DrawFocusButtonRect();

			int m_CurPage;
			int m_MaxPage;
			int m_EditPosition;
			HWND m_EditHWND;

			int SetMaxPage(int mpage);
			int SetCurPage(int cpage);

			//static LRESULT CALLBACK NewProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam);
			//void CALLBACK PagesProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime); 
		};
	}
}
