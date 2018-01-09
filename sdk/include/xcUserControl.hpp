#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcUserControl:
			public xfControl
		{
		public:
			xcUserControl();
			~xcUserControl();
		public:
			//bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		public:
			//virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			void Initial();
		};
	}
}
