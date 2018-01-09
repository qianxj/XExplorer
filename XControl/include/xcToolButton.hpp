#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcToolButton:
			public xfControl
		{
		public:
			xcToolButton();
			~xcToolButton();
		private:
			LPTSTR m_pSrc;
		public:
			bool EvtEraseBkgn(TEvent* pEvent,LPARAM lParam);
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
			bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			bool EvtTimer(TEvent* pEvent,LPARAM lParam);
			bool EvtSetFocus(TEvent* pEvent,LPARAM lParam);
			bool EvtKillFocus(TEvent* pEvent,LPARAM lParam);
			bool EvtKeyDown(TEvent* pEvent,LPARAM lParam);

		public:
			bool	m_bHover;
			bool	m_bChecked;
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			enum EImageFlow{ImageFlow_Left,ImageFlow_Right,ImageFlow_Top,ImageFlow_Bottom,ImageFlow_None,
				ImageFlow_NoneText,ImageFlow_Background};
		public:
			void Initial();
		public:
			xfNode * m_pAgentNodeTool;
		public:
			HWND m_TipHWND;

		};
	}
}
