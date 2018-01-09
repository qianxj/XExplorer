#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		#define ZM_POSCHANGED	0x0001;
		struct 	ZMNMHDR : public NMHDR
		{
			int newPos;
			int oldPos;
		};

		class XCONTROL_API xcZoomer:
			public xfControl
		{
		public:
			xcZoomer();
		public:
			~xcZoomer();
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * rc);
			virtual	void Initial();
		public:
			virtual bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtMouseWheel(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtKeyDown(TEvent *pEvent ,LPARAM lParam);
		public:
			int EventPosChanged(int nNewPos);
		public:
			int m_pos;
			int m_max;
			int m_startx;
			int m_starty;
			int m_height;
			int m_width;
			int m_linewidth;
			int m_dy;
		public:
			xbObject * GetInterface(); 
		};
	}
}