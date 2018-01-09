#pragma once
#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API CXNavigate :public xfControl
	{
	public:
		CXNavigate(void);
		~CXNavigate(void);

	public:
		HWND	m_hWnd;
		RECT	FRect;
		class CXTabSheet* m_pTabSheet;
	public:
		virtual int OnDraw(HDC hdc);
	public:
		bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		bool EvtRButtonDown(TEvent* pEvent,LPARAM lParam);
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		bool SetEventListons();
		bool UnSetEventListons();
		virtual bool GetClientRect(RECT *pRect); 
	};
}}}
