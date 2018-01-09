#pragma once
#include "xofbase.h"
#include "xgriddatasvrex.hpp"
#include "XGrideditsvr.hpp"
#include "contentsvr.hpp"
#include "xcelldef.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	struct TGridDrawInfoEx{
		HWND	hWnd;
		HDC	hdc;

		TGridDrawColor DrawColor;

		int					FRowCount;
		int					FColCount;
		int					EffectiveLineWidth;

		TGridCoord FCurrent;
		TGridCoord FTopLeft;
		TGridCoord FAnchor;
		TGridCoord FFillingAnchor;

		TGridCoord* FTopLefts;

		CAxisSvr*		pAxisSvr;
		CContentSvr*	pContentSvr;
		XOfficeSheetSvr*		pSheetSvr;

		TGridState FGridState;
		TGridOptions FOptions;

		TGridRect**	FSelections;
		int			FSelectionSize;

		//RECT	FrameRect;
		RECT	GridRect;

		RECT	ClipRect;

		int		RowSelectorWidth;
		int		ColHeaderHeight;
		int		BandWidth;
		//int		GridSheetPageWidth;
		//int		GridSheetPageHeight;
		int		VScrollbarWidth;
		int		hScrollBarHeight;
		int		SpSizeBarWidth;
		//int		GridSheetPageNavWidth;

		LPTSTR*	pSheets;
		int		SheetsSize;
		int		SheetStart;
		int		SheetCurrent;

		int		HSizeBarPos;
		int		VSizeBarPos;

		int		LastFullVisibleCol;
		int		LastFullVisibleRow;

		int		FullVisBoundaryCol;
		int		FullVisBoundaryRow;

		TGridDrawOption	GridDrawOption;

		bool	bCalcVert;
	};

	class XCELL_API XGridReport : public CSvrbase
	{
	public:
		enum eSysColumn{key,status,note,count};

	public:
		XGridReport(void);
		~XGridReport(void);
	public:
		class CGridDataSvr*  m_pDataTempl;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);

	public:
		class  XGridSheetSvrEx	* m_pSheetSvr;
	public:
		TGridCoord FCurrent;
		TGridCoord FTopLeft;
		int FGridLineWidth;
	public:
		RECT FRect ;
	public:
		class XGridDataSvrEx * GetGridDataSvr();
	public:
		 TGridRect GetSelection();
	public:
		virtual void Initialize();
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();

	public:
		TGridCoord CalcCoordFromPoint(int X, int Y);
		int CalcRowFromPoint(int X, int Y);
		int CalcColFromPoint(int X, int Y);

	public:
		virtual bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtRButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtRButtonUp(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtKeyDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetFocus(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtDoubleClicked(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtMouseWheel(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtHScroll(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtVScroll(TEvent * pEvent, LPARAM lParam);

	public:
		#define  BN_RETRIEVED 1
		#define  BN_ROWINSERTED 2
		#define  BN_ROWDELETED 3

		bool EvtBinderNotify(TEvent * pEvent, LPARAM lParam);
	public:
		int CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfoEx& DrawInfo, int Pos);
		void ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos);
		void MoveTopLeft(int ALeft, int ATop);
		void TopLeftMoved(TGridCoord OldTopLeft);
		void TopLeftChanged();
		int ScrollBarMin(Cardinal ScrollBar);
		int ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft);
		int PageUp(Cardinal ScrollBar,TGridDrawInfoEx& DrawInfo);
		int  PageDown(Cardinal ScrollBar);
		TGridCoord  CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfoEx& DrawInfo);
		int CalcMaxCell(TGridDrawInfoEx &Axis, int Start);
		void UpdateScrollPos();
		void ScrollData(int DX, int DY);
		void ScrollDataInfo(int DX, int DY, TGridDrawInfoEx &DrawInfo);
		bool CalcScroll(TGridDrawInfoEx &AxisInfo, int OldPos, int CurrentPos, int &Amount);
	public:
		CXScrollBar * XGridReport::GetHScrollBar();
		CXScrollBar * XGridReport::GetVScrollBar();

	public:
		int InitDrawInfo(TGridDrawInfoEx * pDrawInfo);
		void Update(){};

	public:
		void OnRowFocusChange(int nOldRow,int nNewRow);
		int OnRowFocusChangeIng(int nOldRow,int nNewRow);
	public:
		int GetColCount();
		int GetCol(){return FCurrent.x;};       //FCol
	public:
		int GetRowCount();
		int GetRow();
		int SetRow(int row);
	private:
		int GetRowHeight();
		int GetPageRowCount();
	public:
		int GetColumnCount();
		LPCTSTR GetColumnName(int col);
		int GetColumnIndex(LPCTSTR pColumn);
	public:
		LPCTSTR GetItemString(int Row,int Col);
		int SetItemString(int Row,int Col,LPTSTR pStr);
		int SetItemStringEx(int Row,LPTSTR ColName,LPTSTR pStr);
	public:
		std::map<std::wstring,std::wstring>* m_ColDecimalFormat;
		LPCTSTR GetColumnDecimalFormat(LPCTSTR pStrCol);
		void SetColumnDecimalFormat(LPCTSTR pStrCol,LPCTSTR pFormat);
	public:
		std::map<std::wstring,std::wstring>* m_ColFormule;
		LPCTSTR GetColumnFormule(LPCTSTR pStrCol);
		void SetColumnFormule(LPCTSTR pStrCol,LPCTSTR pFormule);
	public:
		int DeleteRow(int Row);
		int InsertRow(int nCol);

	public:
		bool Redraw();
	public:
		int LinePos(TGridDrawInfo& AxisInfo ,int  Line);
		bool GetCellRect(TGridDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool BoxRect(TGridDrawInfo& AxisInfo,TGridRect gridRect,RECT &screenRect);
	
	public:
		TGridState FGridState;
		TGridOptions FOptions;
		TScrollStyle FScrollBars;
		TBorderStyle FBorderStyle;

	public:
		class CEditSvrEx*			m_pEditSvr;

	public:
		bool HandleAllocated ;
		bool FEditorMode;
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
		int DwUpdate(IXMLDOMDocument2 * pXmlDoc);
		int SetReadOnly(BOOL bReadOnly);
	public:
		int  GetXml(BSTR &bstr);
		int  GetXml(BSTR &bstr,LPTSTR pGuid);
		int  SetXml(BSTR bstr);
	public:
		XUtil::xuCsArray<INT32, BYTE> * m_pRowSelects;
		COLORREF m_SelectBkColor;
		int m_nSelectionLastRow;
		int m_SelectionMode; //0 unselection 1 singleSelection 2 multiselection 3 multiselectionex 

		int SelectRow(int nRow,bool bSelect=true);
		int SelectRow(int sRow,int eRow, bool bSelect=true);
		int GetNextSelectRow(int nStartRow);
		int GetPrevSelectRow(int nStartRow);
		bool IsRowSelected(int nRow);
		int SetSelectBkColor(COLORREF color);
		int SetSelectionMode(int nMode);
	public:
		#pragma warning(disable: 4251)
		std::vector<xbObserver * > m_pObServers;
		#pragma warning(default: 4251)

		int AddObserver(xbObserver * pObserver);
		int RemoveObserver(xbObserver * pObserver);
		int ClearObserver();
	public:
		int event_ItemFocusChanged(int nRow,int nCol);
		int event_RowFocusChanged(int nRow,int nOldRow);
		int event_GetFocus();
		int event_LoseFocus();
		int event_ItemChanged(int nRow,int nCol,LPTSTR pData,LPTSTR pDisplayData=NULL);
		int event_ItemChanging(int nRow,int nCol,LPTSTR pData);
		int event_DoubleClicked(int nRow,int nCol);
		int event_Clicked(int nRow,int nCol);
		int event_RClicked(int nRow,int nCol,POINT pt);
		int event_EditorLoseFocus();
	public:
		LPCTSTR GetDwColumnAtCellColumn(int ACol);
	public:
		int FSortType;
		vector<wstring>* m_pSortColumns;
		vector<bool> * m_pSortOrders;
		vector<wstring>* m_pSortColumnTypes;
		int SetRowSort(LPTSTR colName, bool colOrder=true);
	public:
		bool m_bWheelVert;
	public:
		int InvalidateGrid();
	public:
		bool m_bShared;
		XGridReport * m_pPrimaryReport;
		bool DwShareTo(XGridReport * pPrimaryReport);
	public:
		bool FAskMask;
		int SetAskMask(bool bAskMask);
	public:
		int SetColumnProps(int nCol,LPCTSTR pXmlProp);
		int CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2);
	public:
		LPCTSTR GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		bool	m_bSort;
		int AllowSort(bool sort);

		int FindByColumn(int startRow, int columnIndex, LPCTSTR columnValue);
		int FindByColumn(int startRow, LPCTSTR column, LPCTSTR columnValue);

		vector<LPCTSTR> m_ReadOnlyColumn;
		int SetReadOnlyColumn(int columnIndex, bool flag=true);
		int SetReadOnlyColumn(LPCTSTR column, bool flag=true);

		int SetColumnWidth(int columnIndex, int width=1);
		int GetColumnWidth(int columnIndex);
		int SetColumnState(int columnIndex, bool state);

		void SetDataToClip(LPCTSTR DataStr);
		int ShowRowTo(int row);

		int FGridLineHeight;
	public:		
		virtual void OnBaseDraw(HDC hdc,RECT &rect, int nSDrawCol, int nEDrawCol=-1); //冻结重画
		virtual void OnFDraw(HDC hdc,RECT &rect); //冻结重画
	public:
		bool IsEditQuery(int ACol,int ARow,bool &bShowButton);
		int  ShowQueryDlg(LPTSTR pData=NULL);
		int  ShowQueryDlgEx(LPTSTR pData=NULL);
		LPTSTR GetItemFormula(LPTSTR Col);
	public:
		bool m_EditUpperMode;
	public:
		void SetEditUpperMode(bool upper);
		bool GetEditUpperMode();
	public:
		bool hasBindData();

	public:
		bool m_bClickEdit; //单击是否能直接编译
		int SetClickEditFlag(bool f);
		LPCTSTR GetTitleName(int col);
		LPCTSTR GetItemStringEx(int nRow,LPTSTR nColumn,DWBuffer dwBuffer=dbuf_normal);

		virtual xbObject*  GetInterface();
		bool SetRowMove(int row, int movecount);
		bool IsTreeRowShow(int row);
		int GetTreeRow(int row);
		int GetTreeRowCount();
		int TreeRowExpand(int row, bool exp);
		int SetHeaderText(int row, int col, LPTSTR name);
		LPTSTR GetColumnProp(int nCol,LPCTSTR pAttribute);
		LPTSTR GetColumnProp(int row, int nCol,LPCTSTR pAttribute);
	public:
		set<wstring>* m_pBuffers;
	};
}}}}