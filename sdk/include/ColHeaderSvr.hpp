#pragma once
#include "xofbase.h"
#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API CColHeaderSvr :public CSvrbase
	{
	public:
		CColHeaderSvr(void);
		~CColHeaderSvr(void);
	public:
		CGridFrameSvr* m_pGridFrameSvr;
	public:
		RECT	FRect;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect,int &ACol,int &nPos);
	public:
		POINT	m_ptMouse;
		int		m_nSCol; //sizing hit row
		int		m_nSPos; //sizing hit row pos
	public:
		virtual bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
	public:
		void DrawSizingLine(POINT pt);
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	};
}}}}
