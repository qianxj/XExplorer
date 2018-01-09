#pragma once

#include "xframe.h"
#include "xgridoption.hpp"
#include "xgriddatasvr.hpp"
#include "igridsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	struct TGridDrawInfo1;
	struct TGridAxisDrawInfo1;
	class XOFOBJECT_API XGridSvr : public CSvrbase
	{
	public:
		XGridSvr(void);
		~XGridSvr(void);

	public:
		int	FGridLineWidth;
		inline int GetGridLineWidth(){return FGridLineWidth;}
		inline int GetGridLineHeight(){return FGridLineWidth;}
	public:
		XGridDataSvr * m_pGridDataSvr;
		XAPI void SetGridDataSvr(XGridDataSvr * pGridDataSvr);
		XAPI XGridDataSvr * GetGridDataSvr();
	public:
		class IGridSheetSvr * m_pGridSheetSvr;
		XAPI void SetGridSheetSvr(IGridSheetSvr * pGridSheetSvr);
		XAPI IGridSheetSvr * GetGridSheetSvr();

	public:
		//calc draw info
		void CalcDrawInfo(TGridDrawInfo1 &DrawInfo);
	protected:
		void CalcDrawInfoXY(TGridDrawInfo1 &DrawInfo,int  UseWidth,int  UseHeight);
		void CalcAxis(TGridAxisDrawInfo1 &AxisInfo, int UseExtent);
		void CalcFixedInfo(TGridDrawInfo1 &DrawInfo);
		void CalcFixedAxis(TGridAxisDrawInfo1 &Axis ,TGridOptions LineOptions,
			int FixedCount,int  FirstCell,int  CellCount);
	protected:
		int	FRowCount;//row count
		int FColCount;//coumn count
		int FFixedCols;
		int FFixedRows;
	public:
		inline XAPI int GetRowCount(){return FRowCount;}
		inline XAPI int GetColCount(){return FColCount;}
		XAPI void SetRowCount(int value){};
		XAPI void SetColCount(int value){};
		inline XAPI int GetFixedRows(){return FFixedRows;}
		inline XAPI int GetFixedCols(){return FFixedCols;}
		XAPI void SetFixedRows(int value = 1){};
		XAPI void SetFixedCols(int value = 1){};

	protected:
		TGridCoord FCurrent;
		TGridCoord FTopLeft;
		TGridCoord FAnchor;
	public:
		inline XAPI TGridCoord GetCurrent(){return FCurrent;}
		XAPI void SetCurrent(TGridCoord Value);
		inline XAPI TGridCoord GetAnchor(){return FAnchor;}
		XAPI void SetAnchor(TGridCoord Value);
		inline XAPI TGridCoord GetTopLeft(){return FTopLeft;}
		XAPI void SetTopLeft(int ALeft, int ATop);
		XAPI void ClampInView(TGridCoord Coord);
		inline XAPI int GetLeftCol(){return FTopLeft.x;}
		inline XAPI int GetTopRow(){return FTopLeft.y;}
		XAPI TGridRect GetSelection();
		int MoveCurrent(TGridCoord Coord);
		void MoveCurrent(int ACol,int ARow, BOOL MoveAnchor,BOOL Show);
		void MoveAnchor(TGridCoord NewAnchor);
		void SelectionMoved(TGridRect OldSel);
		void AreaMoved(TGridRect OldSel,TGridRect NewSel);
		inline XAPI int GetCol(){return FCurrent.x;};       //FCol
		inline XAPI int GetRow(){return FCurrent.y;};       //FRow
		void MoveTopLeft(int ALeft, int ATop);
		void TopLeftMoved(TGridCoord OldTopLeft);
		void TopLeftChanged();
		BOOL SelectCell(int ACol,int ARow);

	protected:
		RECT FRect;
	public:
		XAPI RECT & GetFRect(){return FRect;}
		XAPI void SetFRect(RECT &rect);
		int DoCalc(const TGridAxisDrawInfo1 &AxisInfo, int N);
		XAPI TGridCoord CalcCoordFromPoint(POINT pt);
		XAPI TGridCoord CalcCoordFromPoint(POINT pt,const TGridDrawInfo1 & DrawInfo);
		XAPI int CalcRowFromPoint(POINT pt);
		XAPI int CalcColFromPoint(POINT pt);

	protected:
		TGridState FGridState;
		TGridOptions FOptions;
	public:
		inline XAPI TGridState GetGridState(){return FGridState;}
		XAPI void SetGridState(TGridState Value);
		inline XAPI TGridOptions GetGridOption(){return FOptions;}
		XAPI void SetGridOptions(TGridOptions Value);

		//axis
	public:
		virtual int GetMinHeight(int ARow);
		virtual int GetMinWidth(int ACol);
		void	SetRowHeight(int Index,int value);
		void	SetColWidth(int Index,int value);
		int		GetRowHeight(int Index);
		int		GetColWidth(int Index);

	protected:
		virtual void Initialize();

	public:
		XAPI int GetHeaderHeight();
		XAPI int GetHeaderWidth();

	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual void DrawBackGround(HDC,RECT &rect);
		virtual void DrawCells(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth = XGridOption::stdGridLineWidth);
		virtual void DrawCell(HDC hdc,int ACol,int ARow, RECT &ARect);
		virtual void DrawGridLine(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth = XGridOption::stdGridLineWidth, COLORREF color=XGridOption::stdConerColor);
		virtual void DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth = XGridOption::stdGridLineWidth, COLORREF color=XGridOption::stdBorderLineColor);
		virtual void DrawSelectionBorder(HDC hdc,RECT &rect);

	protected:
		TScrollStyle FScrollBars;
	public:
		inline XAPI virtual TScrollStyle GetScrollStyle(){return FScrollBars;}
		XAPI void SetScrollStyle(TScrollStyle ScrollBars);

	protected:
		TBorderStyle FBorderStyle;
	public:
		inline virtual TBorderStyle GetBorderStyle(){return FBorderStyle;}
		XAPI  virtual void SetBorderStyle(TBorderStyle BorderStyle);

	public:
		POINT	m_ptMouse;
		POINT	m_ptStart;

		int		m_nSCol; //sizing hit row
		int		m_nSPos; //sizing hit row pos
	protected:
		void CalcPageExtents(TGridDrawInfo1 &DrawInfo, int &PageWidth, int &PageHeight);
		void TimedScroll(TGridScrollDirection Direction);
		bool CanEditAcceptKey(char Key);
		bool CanGridAcceptKey(WORD Key, TShiftState Shift);

	//Window event
	public:	
		bool EvtPaint(TEvent *pEvent ,LPARAM lParam);
		bool EvtLButtonDown(TEvent *pEvent ,LPARAM lParam);
		bool EvtLButtonUp(TEvent *pEvent ,LPARAM lParam);
		bool EvtMouseMove(TEvent *pEvent ,LPARAM lParam);
		bool EvtMouseWheel(TEvent *pEvent ,LPARAM lParam);
		bool EvtVScroll(TEvent *pEvent ,LPARAM lParam);
		bool EvtHScroll(TEvent *pEvent ,LPARAM lParam);
		bool EvtSize(TEvent *pEvent ,LPARAM lParam);
		bool EvtChar(TEvent *pEvent ,LPARAM lParam);
		bool EvtKeyDown(TEvent *pEvent ,LPARAM lParam);
		bool EvtSetFocus(TEvent *pEvent ,LPARAM lParam);
		bool EvtKillFocus(TEvent *pEvent ,LPARAM lParam);
		bool EvtXCommand(TEvent *pEvent ,LPARAM lParam);
		bool EvtTimer(TEvent *pEvent ,LPARAM lParam);
		bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);

		//scroll method
	public:
		int CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfo1& DrawInfo, int Pos);
		void ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos);
		int ScrollBarMin(Cardinal ScrollBar);
		int ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft);
		int PageUp(Cardinal ScrollBar,TGridDrawInfo1& DrawInfo);
		int  PageDown(Cardinal ScrollBar);
		TGridCoord  CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfo1& DrawInfo);
		int CalcMaxCell(TGridAxisDrawInfo1 & Axis, int Start);
		void UpdateScrollPos();
		void ScrollData(int DX, int DY);
		void ScrollDataInfo(int DX, int DY, TGridDrawInfo1 &DrawInfo);
		BOOL CalcScroll(TGridAxisDrawInfo1 &AxisInfo, int OldPos, int CurrentPos, int &Amount);

		CXScrollBar * GetHScrollBar();
		CXScrollBar * GetVScrollBar();
	public:
		int LinePos(TGridAxisDrawInfo1 & AxisInfo ,int  Line);
		bool GetCellRect(TGridDrawInfo1& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool BoxRect(TGridDrawInfo1& AxisInfo,TGridRect gridRect,RECT &screenRect);
	public:
		void Update();
		int GetVisibleRowCount();
		int GetVisibleColCount();
		void InvalidateGrid();
		void InvalidateRect( TGridRect ARect);
		void InvalidateRow(int ARow);
		void InvalidateCol(int ACol);
		void InvalidateCell(int ACol, int ARow);
	public:
		void ShowGrid();
		void ShowGrid(bool bShow);

	public:
		int InsertCol(int ACol);
		int InsertRow(int ARow);
		int InsertRow(int ARow,int Number,bool copyMode);
		void DeleteColumn(int ACol);
		void DeleteRow(int ARow);
		int DeleteRow(int ARow,int Number);

		int DoSizeAdjust(int Count, XUtil::xuSummableCsArray< int, int>& Extents, int DefaultExtent, int Current ,int Amount,int Index);
		virtual void SizeChanged(int OldColCount, int OldRowCount);
		void AdjustSize(int Index, int Amount, BOOL Rows);

		void MoveRowData(int FromIndex, int ToIndex);
		void MoveRowData(int FromIndex, int ToIndex,int Number);
		void MoveColData(int FromIndex ,int ToIndex);

		virtual void ResizeCol(int Index, int OldSize, int NewSize);
		virtual void ResizeRow(int Index, int OldSize, int NewSize);
		virtual void RowHeightChanged();
		virtual void ColWidthChanged();
		virtual void RowMoved(int FromIndex, int ToIndex);
		virtual void RowMoved(int FromIndex, int ToIndex,int Number);
		virtual void ColumnMoved(int FromIndex,int  ToIndex);
		void MoveAdjust(int  &CellPos,int  FromIndex,int  ToIndex);
		void MoveRow(int FromIndex, int ToIndex);
		void MoveColumn(int FromIndex, int ToIndex);
		BOOL GetTabStops(int Index);
		void Click();
		void FocusCell(int ACol, int ARow, BOOL MoveAnchor);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	};
}}}
