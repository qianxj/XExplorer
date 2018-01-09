#pragma once
#include "xframe.h"
#include "vxvxiaxissvr.hxx"
#include "vxvxIHeaderSelector.hxx"
#include "vxvxiofficestylesvr.hxx"
#include "svrbase.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XCellHeader  : CSvrbase
	{
	public:
		XCellHeader();
		~XCellHeader(void);
	public:
		//interface from IBoundSvr
		virtual RECT & IBoundSvr_GetFRect() {return FRect;};
		virtual int IBoundSvr_SetFRect(TRect &rect);
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
		virtual void IHeaderSelector_OnDraw(HDC hdc,RECT &rect) ;
		virtual void IHeaderSelector_Initial(void* pOwner,IAxisSvr * pAxisSvr,IOfficeStyleSvr * pStyleSvr); 

	protected:
		int m_nStyleIndex;
		int m_nSelectionStyleIndex;
		int m_hotStyleIndex;
		IOfficeStyleSvr * m_pStyleSvr;
	public:
		IOfficeStyleSvr * GetStyleSvr(){return m_pStyleSvr;}
		void  SetStyleSvr( IOfficeStyleSvr * pStyleSvr){m_pStyleSvr = pStyleSvr;}
	public:
		int Invalidate(HWND hWnd);
	public:
		virtual HWND vxGetHWND();
		virtual bool vxGetClientRect(RECT *pRect);
	};
}}}
