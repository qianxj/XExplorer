#pragma once

#include "xofbase.h"
#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XGridHeaderEx;
	class CGridHeaderListBoxQuery : public Hxsoft::XFrame::xfControl
	{
		public:
			int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
		public:
			static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		public:
			int AttachProcEdit();
			int DettachProcEdit();
		public:
			WNDPROC m_PrevWndFunc;	
			XGridHeaderEx * pGridHeaderEx;
	};

	class XCELL_API XGridHeaderEx : public CSvrbase
	{
	public:
		XGridHeaderEx(void);
		~XGridHeaderEx(void);
	public:
		class CGridDataSvr*  m_pDataTempl;
	public:
		class XGridReport* m_pGridReportSvr;
	public:
		int GetTitleHeight();
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
		virtual bool EvtRButtonDown(TEvent * pEvent, LPARAM lParam);
	public:
		void DrawSizingLine(POINT pt);
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		TGridState FState;
	public:
		int MoveColumn(int oldColumn, int newColumn);
		int MoveingColumn(int oldColumn, int newColumn);
	public:
		int m_nFSCol;//冻结开始列
		int m_nFECol;//冻结结束列
		virtual void OnBaseDraw(HDC hdc,RECT &rect, int nSDrawCol, int nEDrawCol=-1);
		virtual void OnFDraw(HDC hdc,RECT &rect);
		int GetFColumnWidth();
		bool m_bColQuery;
		int m_nQueryColIndex;
		CGridHeaderListBoxQuery * m_pColumnQueryList;
		int ShowColQueryDlg(int col);
		int CloseColQueryDlg();
		int ColumnQuery(LPTSTR pStr);
		int ColumnQuery(LPTSTR pColumn, LPTSTR pStr);
	};

}}}}
