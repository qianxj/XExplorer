#pragma once
#include "xofbase.h"
#include "xuxutil.h"
#include "addrsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API CAxisSvr :public CSvrbase
	{
	public:
		CAxisSvr(void);
		~CAxisSvr(void);
	public:
		XUtil::xuSummableCsArray< XCROW, USHORT>*  m_pRowHeights;   
		XUtil::xuSummableCsArray< XCCOL, USHORT>*  m_pColWidths;
	public:
		int GetColWidth(XCCOL column, bool flag=false);
		int GetRowHeight(XCROW row);

	public:
		int SetColWidth(XCCOL column,int NewWidth);
		int SetRowHeight(XCROW row ,int NewHeight);
	public:
		int SetRowHeight(int startrow ,int endrow,int value);
		int SetColWidth(int startcol ,int endcol,int value);
	public:
		int GetSumRowHeight(int startRow,int endRow,int FGridLine=1);
		int GetSumColWidth(int startCol,int endCol,int FGridLine=1);
		int GetRowLinePos(int startRow,int nHeight,bool & bPosGridLine,int  FGridLine=1);
		int GetColLinePos(int startCol,int nWidth,bool & bPosGridLine,int  FGridLine=1);
	};
}}}}
