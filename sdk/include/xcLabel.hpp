#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcLabel:
			public xfControl
		{
		public:
			xcLabel();
		public:
			~xcLabel();
		public:
			bool	m_bHover;
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
			bool xcLabel::EvtEraseBkgn(TEvent* pEvent,LPARAM lParam);
			bool xcLabel::EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool xcLabel::EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool xcLabel::EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
			bool xcLabel::EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
		};
	}
}
