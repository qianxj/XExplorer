#pragma once
#include "xofbase.h"
#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API CRowSelectorSvr :public CSvrbase
	{
	public:
		CRowSelectorSvr(void);
		~CRowSelectorSvr(void);
	public:
		CGridFrameSvr* m_pGridFrameSvr;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect,int &ARow,int &nPos);
	public:
		RECT FRect;
	public:
		POINT	m_ptMouse;
		int		m_nSRow; //sizing hit row
		int		m_nSPos; //sizing hit row pos
	public:
		bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		bool EvtLDoubleClick(TEvent * pEvent, LPARAM lParam);
	public:
		void DrawSizingLine(POINT pt);
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		int m_fold;
	};
}}}}
