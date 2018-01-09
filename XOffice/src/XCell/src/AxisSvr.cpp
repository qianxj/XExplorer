#include "stdafx.h"
#include "xofbase.h"
#include "axissvr.hpp"
#include "addrsvr.hpp"
#include "optionsvr.hpp"
#include "xuxutil.h"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{

	//template class ScSummableCompressedArray<XCROW, USHORT>;

	CAxisSvr::CAxisSvr(void)
	{
		m_pRowHeights = new XUtil::xuSummableCsArray< XCROW, USHORT>( MAXROW, COptionSvr::stdRowHeight);
		m_pColWidths  = new XUtil::xuSummableCsArray< XCCOL, USHORT>( MAXCOL, COptionSvr::stdColumnWidth);
		
		m_pRowHeights->SetValue(0,COptionSvr::stdColHeaderHeight);
		m_pColWidths->SetValue(0,0,COptionSvr::stdRowSelectorWidth);
	}
	CAxisSvr::~CAxisSvr(void)
	{
		if(m_pRowHeights) delete m_pRowHeights;
		if(m_pColWidths) delete m_pColWidths;
	}
	int CAxisSvr::GetColWidth(XCCOL column, bool flag)
	{
		//return m_pColWidths->GetValue(column);
		int rowH =m_pColWidths->GetValue(column);
		if(flag && rowH > 40000)
		{
			return rowH - 65536;
		}
		else if(!flag && (rowH < 0 || rowH > 40000))
		{
			return -1;
		}
		return rowH;
	}
	int CAxisSvr::GetRowHeight(XCROW row)
	{
		int rowH = m_pRowHeights->GetValue(row);
		if(rowH > 40000) 
			return rowH - 65536;
		else 
			return rowH;
	}

	int CAxisSvr::SetColWidth(XCCOL column,int NewWidth)
	{
		int nw = m_pColWidths->GetValue(column);
		//if(NewWidth < 0) NewWidth = 0;
		m_pColWidths->SetValue(column,NewWidth);
		return nw;
	}
	int CAxisSvr::SetRowHeight(XCROW row ,int NewHeight)
	{
		m_pRowHeights->SetValue(row,NewHeight);
		return 1;
	}
	int CAxisSvr::SetRowHeight(int startrow ,int endrow,int value)
	{
		m_pRowHeights->SetValue(startrow,endrow,value);
		return 1;
	}
	
	int CAxisSvr::SetColWidth(int startcol ,int endcol,int value)
	{
		m_pColWidths->SetValue(startcol,endcol,value);
		return 1;
	}
	int CAxisSvr::GetSumRowHeight(int startRow,int endRow,int FGridLine)
	{
		int Result = 0;
		int i;
		for(i=startRow; i<=endRow; i++)
		{
			int RowHeight = GetRowHeight(i);
			if(RowHeight > 0)
				Result += RowHeight;// + FGridLine;
		}
		return Result;
		//return m_pRowHeights->SumValues(startRow,endRow) + FGridLine * (endRow - startRow);
	}
	int CAxisSvr::GetSumColWidth(int startCol,int endCol,int FGridLine)
	{
		return m_pColWidths->SumValues(startCol,endCol) + FGridLine * (endCol - startCol);
	}
	int CAxisSvr::GetRowLinePos(int startRow,int nHeight,bool & bPosGridLine,int  FGridLine)
	{
		int nRow = startRow - 1;
		while(nHeight > 0)
		{
			nRow++;
			if(GetRowHeight(nRow) > 0)
				nHeight -= GetRowHeight(nRow) - FGridLine;
		}
		bPosGridLine = nHeight==0?true:false;
		return nRow;
	}

	int CAxisSvr::GetColLinePos(int startCol,int nWidth,bool & bPosGridLine,int  FGridLine)
	{
		int nCol = startCol - 1;
		while(nWidth > 0)
		{
			nCol++;
			nWidth -= GetColWidth(nCol) - FGridLine;
		}
		bPosGridLine = nWidth==0?true:false;
		return nCol;
	}

}}}}
