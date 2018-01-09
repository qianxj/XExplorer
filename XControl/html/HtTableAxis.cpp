#include "HtTableAxis.hpp"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	CHtAxis::CHtAxis(int stdExtent)
	{
		m_pExtents = new XUtil::xuSummableCsArray< INT32, INT32>( 1<<30, stdExtent);
	}

	CHtAxis::~CHtAxis()
	{
		if(m_pExtents)delete m_pExtents;
	}
	int CHtAxis::GetExtent(int nIndex)
	{
		return m_pExtents->GetValue(nIndex);
	}
	int CHtAxis::SetExtent(int nIndex,int nVal)
	{
		m_pExtents->SetValue(nIndex,nVal);
		return 1;
	}
	int CHtAxis::SetExtent(int nStartIndex,int nEndIndex, int nVal)
	{
		m_pExtents->SetValue(nStartIndex,nEndIndex,nVal);
		return 1;
	}
	
	int CHtAxis::GetSumExtent(int nStartIndex,int nEndIndex,int FGridLine)
	{
		return m_pExtents->SumValues(nStartIndex, nEndIndex) + FGridLine * ( nEndIndex - nStartIndex);
	}

	int CHtAxis::GetLinePos(int startIndex,int nExtent,bool & bPosGridLine,int  FGridLine)
	{
		int nIndex = startIndex - 1;
		while(nExtent > 0)
		{
			nIndex++;
			nExtent -= GetExtent(nIndex) - FGridLine;
		}
		bPosGridLine = nExtent==0?true:false;
		return nIndex;
	}


	CHtTableAxis::CHtTableAxis(void)
	{
		m_pRowAxis = new CHtAxis(stdRowHeight);
		m_pColAxis = new CHtAxis(stdColumnWidth);

		m_pRowAxis->SetExtent(0,0, stdColHeaderHeight);
		m_pColAxis->SetExtent( 0,0, stdRowSelectorWidth);
	}

	CHtTableAxis::~CHtTableAxis(void)
	{
		if(m_pRowAxis) delete m_pRowAxis;
		if(m_pColAxis) delete m_pColAxis;
	}

	int CHtTableAxis::GetColWidth(int column)
	{
		return m_pColAxis->GetExtent(column);
	}

	int CHtTableAxis::GetRowHeight(int row)
	{
		return m_pRowAxis->GetExtent(row);
	}

	int CHtTableAxis::SetColWidth(int column,int NewWidth)
	{
		m_pColAxis->SetExtent(column,NewWidth);

		return 1;
	}
	int CHtTableAxis::SetRowHeight(int row ,int NewHeight)
	{
		m_pRowAxis->SetExtent(row,NewHeight);
		return 1;
	}
	int CHtTableAxis::SetRowHeight(int startrow ,int endrow,int value)
	{
		m_pRowAxis->SetExtent(startrow,endrow,value);
		return 1;
	}

	int CHtTableAxis::SetColWidth(int startcol ,int endcol,int value)
	{
		m_pColAxis->SetExtent(startcol,endcol,value);
		return 1;
	}
	int CHtTableAxis::GetSumRowHeight(int startRow,int endRow,int FGridLine)
	{
		return m_pRowAxis->GetSumExtent(startRow,endRow) + FGridLine * (endRow - startRow);
	}
	int CHtTableAxis::GetSumColWidth(int startCol,int endCol,int FGridLine)
	{
		return m_pColAxis->GetSumExtent(startCol,endCol) + FGridLine * (endCol - startCol);
	}
	int CHtTableAxis::GetRowLinePos(int startRow,int nHeight,bool & bPosGridLine,int  FGridLine)
	{
		return  m_pRowAxis->GetLinePos(startRow,nHeight,bPosGridLine,FGridLine);
	}

	int CHtTableAxis::GetColLinePos(int startCol,int nWidth,bool & bPosGridLine,int  FGridLine)
	{
		return  m_pColAxis->GetLinePos(startCol,nWidth,bPosGridLine,FGridLine);
	}
}}}
