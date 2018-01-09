#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcScroolBox:
			public xfControl
		{
			public:
				xcScroolBox();
				~xcScroolBox();
			public:
				void Initial();
			public:
				virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
			public:
				bool EvtPaint(TEvent* pEvent,LPARAM lParam);

			public:
				xfWin* m_wChild;
		};
	}
}
