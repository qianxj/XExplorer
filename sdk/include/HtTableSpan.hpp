#pragma once

#include "ihttablespan.hpp"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	BOOL HtPointInGridRect(int Col, int Row,  RECT Rect);
	BOOL HtPtInRect(int X, int Y ,RECT Rect);
	BOOL HtInflateRect(LPRECT pRect,int dx0,int dy0,int dx1,int dy1);
	RECT HtNormalizeRect(RECT &rect);

	class  CHtTableSpan  : public IHtTableSpan
	{
	public:
		CHtTableSpan(void);
		~CHtTableSpan(void);
	public:
		std::vector<RECT> *  m_pSpanRects;
	public:
		virtual void RefreshSpans();
		virtual bool InsertRow(int ARow);
		virtual bool InsertRow(int ARow,int Number,bool bCopyMode);
		virtual bool DeleteRow(int ARow);
		virtual bool MoveRow(int From,int To);

		virtual bool InsertCol(int ACol);
		virtual bool InsertCol(int ACol,int Number,bool bCopyMode);
		virtual bool DeleteCol(int ACol);
		virtual bool MoveCol(int FromIndex,int ToIndex);
	public:
		virtual RECT GetFullRect(RECT ARect);
		virtual bool Merge(RECT ARect);
		virtual bool RMerge(RECT ARect);
		virtual bool CMerge(RECT ARect);
		virtual bool UnMerge(RECT ARect);
	public:
		virtual bool isSpaned(int ACol,int ARow);
		virtual bool isSpaned(int ACol, int ARow, RECT &ARect);
		virtual bool isSpaned(int ACol, int ARow, RECT &ARect,int &index);
		virtual bool GetSpanedRowCol(int ACol,int ARow,int &SCol,int &SRow);
	};
}}}
