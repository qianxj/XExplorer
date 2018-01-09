#pragma once

#include "xbxbase.h"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class IHtTableSpan : public xbObject
	{
	public:
		virtual void RefreshSpans()=0;
		virtual bool InsertRow(int ARow)=0;
		virtual bool InsertRow(int ARow,int Number,bool bCopyMode)=0;
		virtual bool DeleteRow(int ARow)=0;
		virtual bool MoveRow(int From,int To)=0;

		virtual bool InsertCol(int ACol)=0;
		virtual bool InsertCol(int ACol,int Number,bool bCopyMode)=0;
		virtual bool DeleteCol(int ACol)=0;
		virtual bool MoveCol(int FromIndex,int ToIndex)=0;
	public:
		virtual RECT GetFullRect(RECT ARect)=0;
		virtual bool Merge(RECT ARect)=0;
		virtual bool RMerge(RECT ARect)=0;
		virtual bool CMerge(RECT ARect)=0;
		virtual bool UnMerge(RECT ARect)=0;
	public:
		virtual bool isSpaned(int ACol,int ARow)=0;
		virtual bool isSpaned(int ACol, int ARow, RECT &ARect)=0;
		virtual bool isSpaned(int ACol, int ARow, RECT &ARect,int &index)=0;
		virtual bool GetSpanedRowCol(int ACol,int ARow,int &SCol,int &SRow)=0;
	};
}}}
