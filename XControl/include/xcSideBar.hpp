#pragma once

#include "xframe.h"
namespace Hxsoft
{
	namespace XFrame
	{		
		class xcSideBar :
			public xfControl
		{
		enum eSideState{_side1, _center, _side2, _none};

		public:
			xcSideBar(void);
			~xcSideBar(void);
		public:
			eSideState SideState;
			xfNode*	m_pxfNodeOne;
			xfNode*	m_pxfNodeTwo;
			TLayerFlow	m_pxfNodeOneLayer;
			TLayerFlow	m_pxfNodeTwoLayer;
			LPTSTR	m_pxfNodeOneName;
			LPTSTR	m_pxfNodeTwoName;
		public:
			bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect = NULL);
		public:
			int PrepareSizingNode();
			void Initial();
		};
	}
}
