#pragma once

#include "ihttableaxis.hpp"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class CHtAxis :	public IHtAxis
	{
	public:
		CHtAxis(int stdExtent);
		~CHtAxis();
	public:
		XUtil::xuSummableCsArray<INT32,INT32> *  m_pExtents;   
	public:
		virtual int GetExtent(int nIndex);
		virtual int SetExtent(int nIndex,int nVal);
		virtual int SetExtent(int nStartIndex,int nEndIndex, int nVal);
	public:
		virtual int GetSumExtent(int nStartIndex,int nEndIndex,int FGridLine=1);
		virtual int GetLinePos(int startIndex,int nExtent,bool & bPosGridLine,int  FGridLine=1);
	};

	class CHtTableAxis :
		public IHtTableAxis
	{
	public:
		CHtTableAxis(void);
		~CHtTableAxis(void);
	public:
		 CHtAxis* m_pRowAxis;
		 CHtAxis* m_pColAxis;
	public:
		IHtAxis * GetRowAxis() {return m_pRowAxis;}
		IHtAxis * GetColAxis() {return m_pColAxis;}
	public:
		enum eAxisExt{ stdRowHeight=24,	stdColumnWidth = 64,stdColHeaderHeight = 24, stdRowSelectorWidth = 28};

	public:
		virtual int GetColWidth(int column);
		virtual int GetRowHeight(int row);

	public:
		virtual int SetColWidth(int column,int NewWidth);
		virtual int SetRowHeight(int row ,int NewHeight);
	public:
		virtual int SetRowHeight(int startrow ,int endrow,int value);
		virtual int SetColWidth(int startcol ,int endcol,int value);
	public:
		virtual int GetSumRowHeight(int startRow,int endRow,int FGridLine=1);
		virtual int GetSumColWidth(int startCol,int endCol,int FGridLine=1);
		virtual int GetRowLinePos(int startRow,int nHeight,bool & bPosGridLine,int  FGridLine=1);
		virtual int GetColLinePos(int startCol,int nWidth,bool & bPosGridLine,int  FGridLine=1);
	};
}}}
