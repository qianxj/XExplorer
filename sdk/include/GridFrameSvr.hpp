#pragma once
#include "xofbase.h"
#include "axissvr.hpp"
#include "contentsvr.hpp"
#include "optionsvr.hpp"
#include "xuxutil.h"
#include "spanRectsvr.hpp"
#include "xcellStyleSvr.hpp"
#include "editsvr.hpp"
#include "griddatasvr.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	struct pageAxis
	{
		int iPage;
		int startRow;
		int endRow;
		int startCol;
		int endCol;
	};
	class XSheetFrameSvr;
	template class XCELL_API XUtil::xuVector<TGridRect*>;
	class XCELL_API CGridFrameSvr :public CSvrbase
	{
	public:
		CGridFrameSvr(void);
		~CGridFrameSvr(void);
	public:
		CGridDataSvr*		GetGridDataSvr();
		CEditSvr*			m_pEditSvr;
		int					m_RowCount;
		int					m_ColCount;

	public:
		bool	FShowOrigin;
	public:
		RECT	FRect;

	public:
		int	FGridLineWidth;
		
		TGridCoord FCurrent;
		TGridCoord FTopLeft;
		TGridCoord FTopLefts[4];
		TGridCoord FAnchor;
		TGridCoord FFillingAnchor;
		XUtil::xuVector<TGridRect*>	FSelections;

		TGridCoord FOldAnchor;

		TGridState FGridState;
		TGridOptions FOptions;
		TScrollStyle FScrollBars;
		TBorderStyle FBorderStyle;

		int FFixedCols;
		int FFixedRows;
		bool HandleAllocated;

	/*	
	public:
		int FSizingIndex;
		int FSizingPos,FSizingOfs;
		
		int FMoveIndex, FMovePos;
		TPoint FHitTest;
    
	public:		
		BOOL FCanEditModify;
		CInplaceEdit* FInplaceEdit;
		CInplaceEdit* FInplaceEdits[2];
		int FInplaceCol, FInplaceRow;
		int FColOffset;
		BOOL FDefaultDrawing;*/
		BOOL FEditorMode;
	/*
	public:	
		BOOL TabStop;   //default True
	
	public:
		BOOL FSaveCellExtents;
		TGridOptions DesignOptionsBoost;
		BOOL VirtualView;*/
	public:
		inline  int GetFixedRows(){return FFixedRows;}; //default 1
		void    SetFixedRows(int value = 1);
	public:
		inline  int GetFixedCols(){return FFixedCols;}; //default 1
		void SetFixedCols(int value = 1);

	public:
		class XCellSheetSvr	* m_pSheetSvr;
	public:
		virtual void Initialize();
	public:
		int GetRowCount(){return m_RowCount;}
		int GetColCount(){return m_ColCount;}
	public:
		bool FCanEditModify;
		int FInplaceCol;
		int FInplaceRow;

		bool SelectCell(int ACol, int ARow);
		void HideEditor();
		void HideEdit();
		void UpdateEdit();
		void UpdateEditor();
		void ShowEditor();
	public:
		bool CanEditAcceptKey(char Key);
		bool CanGridAcceptKey(WORD Key, TShiftState Shift);
		bool CanEditModify();
		bool CanEditShow();
		bool IsActiveControl();
		void InvalidateEditor();
		void ClampInView(TGridCoord Coord);
	public:
		int GetLeftCol(){return FTopLeft.x;}
		int GetTopRow(){return FTopLeft.y;}
	public:
		int GetHeaderHeight();
		int GetHeaderWidth();

	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual void DrawBackGround(HDC,RECT &rect);
		virtual void DrawCells(HDC,RECT &rect,int StartCol,int StartRow,int GridLineWidth = COptionSvr::stdGridLineWidth);
		virtual void DrawCell(HDC,int ACol,int ARow, RECT &ARect);
		virtual void DrawGridLine(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth = COptionSvr::stdGridLineWidth, COLORREF color=COptionSvr::stdConerColor);
		virtual void DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth = COptionSvr::stdGridLineWidth, COLORREF color=COptionSvr::stdBorderLineColor);
		virtual void DrawSelectionBorder(HDC hdc,RECT &rect);
	
	public:
		inline virtual TGridCoord GetTopLeft(){return this->FTopLeft;}
		inline virtual XUtil::xuVector<TGridRect*> GetSelections(){return FSelections;}
		inline virtual int  GetSelectionSize(){return FSelections.size();}
		inline virtual TGridState GetGridState(){return FGridState;}
		inline virtual TGridCoord GetCurrent(){return FCurrent;}
		inline virtual TGridCoord GetAnchor(){return FAnchor;}
		inline virtual TGridOptions GetGridOptions(){return FOptions;}
		inline virtual TGridOptions GetScrollStyle(){return FScrollBars;}
		TGridRect GetSelection();


	public:
		TGridCoord CalcCoordFromPoint(int X, int Y);
		int CalcRowFromPoint(int X, int Y);
		int CalcColFromPoint(int X, int Y);
		int MoveCurrent(TGridCoord Coord);
		void MoveCurrent(int ACol,int ARow, BOOL MoveAnchor,BOOL Show);
		void MoveAnchor(TGridCoord NewAnchor);

		void SelectionMoved(TGridRect OldSel);
		void AreaMoved(TGridRect OldSel,TGridRect NewSel);
	public:
		inline virtual TBorderStyle GetBorderStyle(){return FBorderStyle;}
	public:
		int SetFRect(RECT &rect);

	public:
		POINT	m_ptMouse;
		POINT	m_ptStart;
		TGridCoord	m_ptPosition;
		TGridCoord	m_ptPositionStart;

		int		m_nSCol; //sizing hit row
		int		m_nSPos; //sizing hit row pos
	public:
		void CalcPageExtents(TGridDrawInfo &DrawInfo, int &PageWidth, int &PageHeight);
		void TimedScroll(TGridScrollDirection Direction);
	public:
		virtual void DoMoveCells();
		virtual BOOL DoMoveCells(POINT start, POINT end);
		virtual BOOL DoDeleteLine(POINT start, POINT end);
		BOOL DoInsLine(POINT start,POINT end);
	public:
		bool DoAutoFilling();
		bool GetFillingSize(int &Cols,int &Rows);
		void MoveFillingAnchor(TGridDrawInfo DrawInfo,TGridCoord  NewFillingAnchor);
	public:
		TGridState	CalcSizingState(POINT point,RECT &rect);
		void DrawSizingLine(POINT pt);
		void DrawMovingLine();
		void DrawInsLine(POINT EndPt);
	public:
		int InitDrawInfo(TGridDrawInfo * pDrawInfo);

		bool EvtPaint(TEvent *pEvent ,LPARAM lParam);
		bool EvtLButtonDown(TEvent *pEvent ,LPARAM lParam);
		bool EvtRButtonDown(TEvent *pEvent ,LPARAM lParam);
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
		bool EvtDoubleClicked(TEvent * pEvent, LPARAM lParam);

		int CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfo& DrawInfo, int Pos);
		void ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos);
		void MoveTopLeft(int ALeft, int ATop);
		void TopLeftMoved(TGridCoord OldTopLeft);
		void TopLeftChanged();
		int ScrollBarMin(Cardinal ScrollBar);
		int ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft);
		int PageUp(Cardinal ScrollBar,TGridDrawInfo& DrawInfo);
		int  PageDown(Cardinal ScrollBar);
		TGridCoord  CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfo& DrawInfo);
		int CalcMaxCell(TGridDrawInfo &Axis, int Start);
		void UpdateScrollPos();
		void ScrollData(int DX, int DY);
		void ScrollDataInfo(int DX, int DY, TGridDrawInfo &DrawInfo);
		bool CalcScroll(TGridDrawInfo &AxisInfo, int OldPos, int CurrentPos, int &Amount);

		CXScrollBar * GetHScrollBar();
		CXScrollBar * GetVScrollBar();
	public:
		int LinePos(TGridDrawInfo& AxisInfo ,int  Line);
		bool GetCellRect(TGridDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool BoxRect(TGridDrawInfo& AxisInfo,TGridRect gridRect,RECT &screenRect);
	public:
		bool CalcSpanRgn(HRGN & hRgn,TGridDrawInfo& AxisInfo);
	public:
		void Update(){};
		int GetVisibleRowCount();
		int GetVisibleColCount();
		void InvalidateGrid();
		void InvalidateRect( TGridRect ARect);
		void InvalidateRow(int ARow);
		void InvalidateCol(int ACol);
		void InvalidateCell(int ACol, int ARow);
	public:
		bool CellChooseFont();
		void SetFont(TGridRect cellRect,LOGFONT &logFont);
		void SetFont(TGridRect cellRect,LOGFONT &logFont,COLORREF Color);
	public:
		bool CellChooseColor();
		bool CellChooseColor(DWORD flag);
		void SetColor(TGridRect cellRect, COLORREF color, DWORD flag);
	public:
		void SetAlign(XUtil::Css::TextAlign align);
		void SetAlign(TGridRect ARect,XUtil::Css::TextAlign align);
		void SetBorder(DWORD nBorder);
		void SetBorder(TGridRect ARect,DWORD nBorder);
		void ShowGrid();
		void ShowGrid(bool bShow);

	public:
		virtual int GetMinHeight(int ARow);
		virtual int GetMinWidth(int ACol);
		void	SetRowHeight(int Index,int value);
		void	SetColWidth(int Index,int value);
		int		GetRowHeight(int Index);
		int		GetColWidth(int Index);

	public:
		int InsertCol(int ACol);
		int InsertRow(int ARow);
		int InsertRow(int ARow,int Number,bool copyMode);
		void DeleteColumn(int ACol);
		void DeleteRow(int ARow);
		int DeleteRow(int ARow,int Number);

		int DoSizeAdjust(int Count, XUtil::xuSummableCsArray< XCROW, USHORT>& Extents, int DefaultExtent, int Current ,int Amount,int Index);
		virtual void SizeChanged(int OldColCount, int OldRowCount);
		void AdjustSize(int Index, int Amount, BOOL Rows);

		void MoveRowData(int FromIndex, int ToIndex);
		void MoveRowData(int FromIndex, int ToIndex,int Number);
		void MoveColData(int FromIndex ,int ToIndex);

		virtual void ResizeCol(int Index, int OldSize, int NewSize);
		virtual void ResizeRow(int Index, int OldSize, int NewSize);
		virtual void RowHeightsChanged();
		virtual void ColWidthsChanged();
		virtual void RowMoved(int FromIndex, int ToIndex);
		virtual void RowMoved(int FromIndex, int ToIndex,int Number);
		virtual void ColumnMoved(int FromIndex,int  ToIndex);
		void MoveAdjust(int  &CellPos,int  FromIndex,int  ToIndex);
		void MoveRow(int FromIndex, int ToIndex);
		void MoveColumn(int FromIndex, int ToIndex);
		BOOL GetTabStops(int Index);
		void Click();
		int GetCol(){return FCurrent.x;};       //FCol
		int GetRow(){return FCurrent.y;};       //FRow
		void FocusCell(int ACol, int ARow, BOOL MoveAnchor);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		int SetRunState(int runstate);
		CDwTable * GetCurDwTable();
		int SetDwGrid(CDwTable * pTable);
	public:
		int Copy();
		int Paste();
		int Cut();
	public:
		HDC GetPrintDC(HWND hWnd, WORD PD_FLAG);
		void Print();
		void Print(HDC hPrintDC);
		int PrintPage(HDC hPrintDC, pageAxis & pAxisPage,RECT * pPageArea);
		int CalcPrintPageAxis(int pageW,int pageH, pageAxis* &pPageAxis ,int &iPageCount);
		int GetPrintArea(RECT & PrintRect);
		int GetPrintHeaderHeight();
		int GetPrintFooterHeight();
		int GetPrintLeftWidth();
		int GetPrintRightWidth();
	public:
		bool IsEditQuery(int ACol,int ARow,bool &bShowButton);
		int  ShowQueryDlg(LPTSTR pData=NULL);
	public:
		LPTSTR GetItemProp(int ACol,int ARow,LPTSTR pItem);
		LPTSTR GetColumnProp(CDwTable * pDwTable,int ACol,LPTSTR pItem);
		LPTSTR GetColumnProp(CDwTable * pDwTable,LPTSTR pColName,LPTSTR pItem);
	public:
		bool FMouseTrace;
		HWND m_TipHWND;
		POINT m_ptTrace;
		int  m_nDelay;
		int  m_nDelayed; //have delayed time
	public:
		bool m_EditUpperMode;
	public:
		void SetEditUpperMode(bool upper);
		bool GetEditUpperMode();
	public:
		int SetReadOnly(BOOL bReadOnly);
		void ClearColor(TGridRect cellRect, DWORD flag); 
	public:
		LPCTSTR GetItemString(int Row,int Col);
		LPCTSTR GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);

	};
}}}}
