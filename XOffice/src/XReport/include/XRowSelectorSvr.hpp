#pragma once
#include "xofbase.h"
#include "XReportAxisSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XREPORT_API XRowSelectorSvr :public CSvrbase
	{
	public:
		XRowSelectorSvr(void);
		~XRowSelectorSvr(void);
	public:
		XReportAxisSvr* m_pAxisSvr;
		class XReportSheetSvr* m_pSheetSvr;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual int CalcSizingState(POINT point,RECT &rect);
		virtual int CalcSizingState(POINT point,RECT &rect,int &ARow,int &nPos);
	public:
		RECT FRect;
	public:
		POINT	m_ptMouse;
		int		m_nSRow; //sizing hit row
		int		m_nSPos; //sizing hit row pos
		int		m_nWidth;
		int		m_nIndexStyle;
		int		m_nCellIndexStyle;
		int		m_nSelectionIndexStyle;
		int		m_nCornerIndexStyle;
	public:
		bool	m_bDrawRowNo;
		bool	SholudDrawRowNo(){return m_bDrawRowNo;}
		int		DrawDefaultFrame(HDC hdc,int ARow,RECT &rc);
		int		DrawCorner(HDC hdc,RECT &rc);
	public:
		bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
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
