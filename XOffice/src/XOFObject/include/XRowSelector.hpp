#pragma once
#include "xframe.h"
#include "xaxissvr.hpp"
#include "irowselector.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XRowSelector  :
		public IRowSelector
	{
	public:
		XRowSelector(xfAreaObject* pOwner,IAxisSvr * pAxisSvr,XOfficeStyleSvr * pStyleSvr);
		~XRowSelector(void);
	public:
		//interface from ibounditem
		virtual XAPI STDMETHODIMP_(TRect &) GetFRect(){return FRect;};
		virtual XAPI STDMETHODIMP_(int) SetFRect(TRect &rect);
	protected:
		IAxisSvr * m_pAxisSvr;
		xfAreaObject	* m_pOwner;
		RECT	FRect;
	protected:
		POINT	m_ptMouse;
		int		m_nSRow; //sizing hit row
		int		m_nSPos; //sizing hit row pos
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	private:
		int m_nStartRow;
		int m_nRowCount;
	public:
		virtual int GetStartRow(){return m_nStartRow;}
		virtual int GetRowCount(){return m_nRowCount;}
		void SetStartRow(int nRow){m_nStartRow = nRow;}
		void SetRowCount(int nRowCount){m_nRowCount = nRowCount;}
	protected:
		void DrawSizingLine(POINT pt);
	private:
		TGridState	FGridState;
	public:
		TGridState GetState();
		void SetState(TGridState state);
	public:
		virtual TGridState CalcSizingState(POINT point,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect,int &ARow,int &nPos);
	public:
		bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		bool EvtLDoubleClick(TEvent * pEvent, LPARAM lParam);
	public:
		virtual bool IsRowSelected(int nRow);
		virtual bool IsRowMarked(int nRow);
	public:
		virtual XAPI STDMETHODIMP_(void) OnDraw(HDC hdc,RECT &rect) ;

	protected:
		int m_nStyleIndex;
		int m_nSelectionStyleIndex;
		int m_hotStyleIndex;
		XOfficeStyleSvr * m_pStyleSvr;
	public:
		XOfficeStyleSvr * GetStyleSvr(){return m_pStyleSvr;}
		void  SetStyleSvr( XOfficeStyleSvr * pStyleSvr){m_pStyleSvr = pStyleSvr;}
	public:
		int Invalidate(HWND hWnd);
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	};
}}}
