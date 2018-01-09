#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcScrollBox:
			public xfControl
		{
		public:
			xcScrollBox();
			~xcScrollBox();
		public:
			void Initial();
			void Initial(IXMLDOMElement *pElement);
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			virtual bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtSize(TEvent *pEvent,LPARAM lparam);
		public:
			xfWin* m_wChild;
		public:
			//void UpdateScrollPos();
			//void ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos);
			int CalcScrollBar(int Value, int ScrollCode, int ScrollBar,  int Pos);
			//int ScrollBarMin(int ScrollBar);
			//int ScrollBarMax(int ScrollBar);
			//int PageUp(int ScrollBar);
			//int PageDown(int ScrollBar);
			//POINT  CalcMaxTopLeft(POINT Coord,int ScrollBar);
		public:
			SIZE GetContentExtent();
			int AdjustExtent();
			void UpdateScrollPos();
		public:
			void SetContent(LPTSTR strUrl, LPVOID pParam = 0);

		};
	}
}
