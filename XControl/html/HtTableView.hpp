#pragma once
#include "ihttableview.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	#define THtGridCoord	POINT
	#define THtGridRect		RECT
	#define	THtGridState	DWORD

	struct THtAxisInfo
	{
		IHtAxis * pAxis;
		int		EffectiveLineWidth;
		int		FixedBoundary;
		int		GridBoundary;
		int		GridExtent;
		int	    LastFullVisibleCell;
		int     FullVisBoundary;
		int     FixedCellCount;
		int     FirstGridCell;
		int     GridCellCount;
		int		GetExtent(int nIndex){return pAxis->GetExtent(nIndex);}
	};
	
	struct THtDrawInfo{
		THtAxisInfo  Vert;
		THtAxisInfo  Horz;
	};

	class CHtTableView : public IHtTableView
	{
	public:
		CHtTableView(void);
		~CHtTableView(void);
	public:
		IHtTableData * m_pTableData;
	public:
		virtual IHtTableData * SetTableData(IHtTableData * pData);
		virtual IHtTableData * GetTableData();

		virtual int GetHeight();
		virtual int GetWidth();

		virtual int GetRowHeight(int nRow);
		virtual int GetColWidth(int nCol);

		virtual LPCTSTR GetItemString(int nCol,int nRow);
		virtual void SetItemString(int nCol,int nRow,LPTSTR pStr);
		
		virtual LPIHtContentItem GetItem(int nCol,int nRow);
		virtual void SetItem(int nCol,int nRow,LPIHtContentItem pItem);

		virtual RECT GetCellRect(int nCol,int nRow);

	public:
		int Initial();
	public:
		THtGridCoord FTopLeft;
		THtGridCoord FCurrent;
		THtGridCoord FAnchor;
		THtGridCoord FFixed;

	public:
		THtGridRect GetSelection();
	public:
		virtual int DoDrawTo(HDC hdc,RECT &rc, int startCol,int startRow); 
		virtual void DrawCells(HDC,RECT &rect,int StartCol,int StartRow,int GridLineWidth = 1);
		virtual void DrawCell(HDC,int ACol,int ARow, RECT &ARect);
	public:
		int GetClientWidth();
		int GetClientHeight();
	public:
		int	GetFixedCols(){return FFixed.x;}
		int GetFixedRows(){return FFixed.y;}
		int	GetLeftCol(){ return FTopLeft.x;}
		int	GetTopRow() { return FTopLeft.y;}
		int GetRow(){ return FCurrent.x;}
		int GetCol(){ return FCurrent.y;}
		int	GetRowCount(){ return m_pTableData->GetContent()->GetRowCount();}
		int GetColCount(){ return m_pTableData->GetContent()->GetColCount();}
	public:
		int	FGridLineWidth; 
		int	GetGridLineWidth(){return FGridLineWidth;}
		void SetGridLineWidth(int nVal ){  FGridLineWidth = nVal ;}

	public:
		//calc draw info
		void CalcDrawInfo(THtDrawInfo &DrawInfo);
		void CalcDrawInfoXY(THtDrawInfo &DrawInfo,int  UseWidth,int  UseHeight);
		void CalcAxis(THtAxisInfo & AxisInfo, int UseExtent);
		void CalcFixedInfo(THtDrawInfo &DrawInfo);
		void CalcFixedAxis(THtAxisInfo &Axis ,int FixedCount,int  FirstCell,int  CellCount);

	public:
		int	LinePos(THtAxisInfo &AxisInfo ,int  Line);
		bool GetCellRect(THtDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan = true);
		bool BoxRect(THtDrawInfo& AxisInfo,THtGridRect gridRect,RECT &screenRect);
		bool CalcSpanRgn(HRGN & hRgn,THtDrawInfo& AxisInfo);
	public:
		void DrawGridLine(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color);
		void DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color);
	public:
		THtGridCoord CalcCoordFromPoint(int X, int Y);
		int CalcRowFromPoint(int X, int Y);
		int CalcColFromPoint(int X, int Y);
	};
}}}
