#pragma once
#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class IHtAxis : public xbObject
	{
	public:
		virtual int GetExtent(int nIndex)=0;
		virtual int SetExtent(int nIndex,int nVal)=0;
		virtual int SetExtent(int nStartIndex,int nEndIndex, int nVal)=0;
	public:
		virtual int GetSumExtent(int nStartIndex,int nEndIndex,int FGridLine=1)=0;
		virtual int GetLinePos(int startIndex,int nExtent,bool & bPosGridLine,int  FGridLine=1)=0;
	};


	class IHtTableAxis : public xbObject
	{
	public:
		virtual IHtAxis * GetRowAxis() = 0;
		virtual IHtAxis * GetColAxis() = 0;
	public:
		virtual int GetColWidth(int column)=0;
		virtual int GetRowHeight(int row)=0;
	
	public:
		virtual int SetColWidth(int column,int NewWidth)=0;
		virtual int SetRowHeight(int row ,int NewHeight)=0;
	public:
		virtual int SetRowHeight(int startrow ,int endrow,int value)=0;
		virtual int SetColWidth(int startcol ,int endcol,int value)=0;
	public:
		virtual int GetSumRowHeight(int startRow,int endRow,int FGridLine=1)=0;
		virtual int GetSumColWidth(int startCol,int endCol,int FGridLine=1)=0;
		virtual int GetRowLinePos(int startRow,int nHeight,bool & bPosGridLine,int  FGridLine=1)=0;
		virtual int GetColLinePos(int startCol,int nWidth,bool & bPosGridLine,int  FGridLine=1)=0;
	};

}}}