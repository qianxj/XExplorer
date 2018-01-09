#pragma once
#include "xofbase.h"
#include "xframe.h"
#include "xreportdatasvr.hpp"
#include "xreporteditsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	#define  TGridState     int
	#define  gsNormal       0x0001
	#define  gsSelecting    0x0002
	#define  gsRowSizing    0x0004
	#define  gsColSizing    0x0008

	struct 	DWRNMHDR : public NMHDR
	{
		void * DwTable;
		int row;
		int col;
		LPTSTR data;
		int oldrow;
		int oldcol;
		LPTSTR colname;
		LPTSTR oldcolname;
	};

	class IXReport;
	class XREPORT_API XReportSheetSvr :
		public XOffice::XOfficeSheetSvr
	{
	public:
		XReportSheetSvr(void);
		~XReportSheetSvr(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);
	public:
		virtual void OnRowFocusChange(int nOldRow,int nNewRow);
		virtual void OnDoubleClicked(int nRow);
		virtual void OnRButtonDown(int nRow,TEvent * pEvent);
		virtual void OnClicked(int nRow,int nCol);
	public:
		TGridState FGridState;
	public:
		void DrawSizingLine(POINT pt);
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
		virtual bool EvtKeyDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetFocus(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtKillFocus(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtDoubleClicked(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtRButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtCommand(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtMouseWheel(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtHScroll(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtVScroll(TEvent * pEvent, LPARAM lParam);
		
	public:
		virtual void Initialize();
	public:	
		CEditSvr * m_pEditSvr;

	public:
#define Cardinal int
#define TGridCoord POINT

		//int CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfo& DrawInfo, int Pos);
		void ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos);
		void MoveTopLeft(int ALeft, int ATop);
		void TopLeftMoved(TGridCoord OldTopLeft);
		void TopLeftChanged();
		int ScrollBarMin(Cardinal ScrollBar);
		int ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft);
		//int PageUp(Cardinal ScrollBar,TGridDrawInfo& DrawInfo);
		//int  PageDown(Cardinal ScrollBar);
		//TGridCoord  CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfo& DrawInfo);
		//int CalcMaxCell(TGridDrawInfo &Axis, int Start);
		void UpdateScrollPos();
		void ScrollData(int DX, int DY);
		//void ScrollDataInfo(int DX, int DY, TGridDrawInfo &DrawInfo);
		//bool CalcScroll(TGridDrawInfo &AxisInfo, int OldPos, int CurrentPos, int &Amount);

		class XScrollBar * GetHScrollBar();
		class XScrollBar * GetVScrollBar();
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		int GetRow();
		int SetRow(int ARow);
		int GetRowCount();
		int Retrieve(LPTSTR pStrXQuery);
		int Retrieve(LPTSTR pStrUrl,LPTSTR pStrXQuery);
	public:
		XReportDataSvr * GetReportDataSvr();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		bool GetDwRect(RECT *pRect);
		bool m_bTreeDoubleClick;

	public:
		bool HandleAllocated ;
		bool FEditorMode;
		bool FCanEditModify;
		int FInplaceCol;
		int FInplaceRow;
		int FGridLineWidth;
	public:
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

	public:
		TGridOptions FOptions;
		TScrollStyle FScrollBars;
		TBorderStyle FBorderStyle;

	public:
		TGridCoord FCurrent;
		TGridCoord FTopLeft;
		TGridCoord FAnchor;

	public:
		bool Redraw();

	public:
		int GetCol(){return FCurrent.x;}
		int SetCol(int nCol);
	public:
		int GetColumnCount();
		LPCTSTR GetColumnName(int col);
		int GetColumnIndex(LPCTSTR pColumn);
	public:
		LPCTSTR GetItemString(int nRow,int nColumn,IDwView::DWBuffer dwBuffer=IDwView::dbuf_normal);
		LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,IDwView::DWBuffer dwBuffer=IDwView::dbuf_normal);
		bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,IDwView::DWBuffer dwBuffer=IDwView::dbuf_normal);
		bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,IDwView::DWBuffer dwBuffer=IDwView::dbuf_normal);

	public:
		int InsertRow(int nRow);
		int DeleteRow(int nRow);

	public:
		int  LoadXml(BSTR bstr);
		int Load(IXMLDOMElement * pElement);
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
		int event_ItemChanged(int nRow,int nCol,LPTSTR pData);
		int event_ItemChanging(int nRow,int nCol,LPTSTR pData);
	public:
		IXMLDOMElement * GetRowElement(int nRow);
	public:
		int InsertCol(int nBeforeCol,LPCTSTR pCaption,LPCTSTR pColName,int nWidth,LPCTSTR pColPath = NULL,LPCTSTR pStyleClass=NULL,
				LPCTSTR pEditClass=NULL,LPCTSTR pFormatClass=NULL);

		int RemoveCol(int nCol);
		int RemoveVCol(int nCol);
		int RemoveCol(LPCTSTR pColName);

		int AddEditStyle(LPCTSTR pName, LPCTSTR pStr);
		int RemoveEditStyle(LPCTSTR pName, LPCTSTR pStr);
		int SetColEditStyle(LPCTSTR pColName,LPCTSTR pClassName);
		int SetDDLBData(LPCTSTR pColName,LPCTSTR pData);
		LPCTSTR GetDDLBData(LPCTSTR pColName);
	public:
		static LPTSTR GetCellContent(XOfficeSheetSvr* m_pOwner, int ACol,int ARow);
		static int SetCellContent(XOfficeSheetSvr* m_pOwner, int ACol,int ARow, LPTSTR pStr);
		static int GetCellRect(XOfficeSheetSvr* m_pOwner, int ACol,int ARow,RECT &rt);
		static LPTSTR GetEditType(XOfficeSheetSvr* m_pOwner, int ACol,int ARow);
		static HFONT GetEditFont(XOfficeSheetSvr* m_pOwner,int ACol,int ARow);
	public:
		bool GetCellRect(int ACol,int ARow,RECT &rt);
		int GetColWidth(int  nCol);
		int GetRowHeight(int nRow);
	public:
		int GetVColumn(int nCol);
	public:
		int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false);
		int ImportClipBoard();

	};
}}}}
