#pragma once

#include "xframe.h"
namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcWCControl :
			public xfControl
		{
		public:
			xcWCControl(void);
			~xcWCControl(void);
		public:
			virtual void Initial(); 
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
		public:
			virtual int AdjustControlRect(RECT rect,bool redraw = true);
		public:
			virtual int GetXml(BSTR &bstr);

			WNDPROC m_fnCallBack;
			static LRESULT CALLBACK fnCallBack(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		};
	}
}