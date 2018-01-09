#pragma once
#include "xframe.h"
#include "xaxissvr.hpp"
#include "iheaderselector.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XCellHeader  :
		public IHeaderSelector
	{
	public:
		XCellHeader(xfAreaObject* pOwner,IAxisSvr * pAxisSvr,XOfficeStyleSvr * pStyleSvr);
		~XCellHeader(void);
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
		int		m_nSCol; //sizing hit Col
		int		m_nSPos; //sizing hit Col pos
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	private:
		int m_nStartCol;
		int m_nColCount;
	public:
		virtual int GetStartCol(){return m_nStartCol;}
		virtual int GetColCount(){return m_nColCount;}
		void SetStartCol(int nCol){m_nStartCol = nCol;}
		void SetColCount(int nColCount){m_nColCount = nColCount;}
	protected:
		void DrawSizingLine(POINT pt);
	private:
		TGridState	FGridState;
	public:
		TGridState GetState();
		void SetState(TGridState state);
	public:
		virtual TGridState CalcSizingState(POINT point,RECT &rect);
		virtual TGridState CalcSizingState(POINT point,RECT &rect,int &ACol,int &nPos);
	public:
		bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		bool EvtLDoubleClick(TEvent * pEvent, LPARAM lParam);
	public:
		virtual bool IsColSelected(int nCol);
		virtual bool IsColMarked(int nCol);
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
