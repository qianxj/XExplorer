#pragma once

#include "xframe.h"
namespace Hxsoft
{
	namespace XFrame
	{
		
		class xcSpliterBar :
			public xfControl
		{
			enum eSizeState
			{
				_nosizing,
				_sizing
			};

		public:
			xcSpliterBar(void);
			~xcSpliterBar(void);
		public:
			eSizeState SizeState;
			POINT m_MousePt;	
		public:
			xfNode*	m_pxfNodeOne;
			xfNode*	m_pxfNodeTwo;
		public:
			int DrawSizeBar(POINT pt);
			int AdjustSplitePos(POINT pt);
			int PrepareSizingNode();
		public:
			bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			bool EvtSetCursor(TEvent* pEvent,LPARAM lParam);
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect = NULL);
		};
	}
}
