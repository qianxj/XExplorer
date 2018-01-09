#pragma once
#include "xofbase.h"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	template class XCELL_API XUtil::xuVector<RECT>;
	class XCELL_API CSpanRectSvr :public CSvrbase
	{
	public:
		CSpanRectSvr(void);
		~CSpanRectSvr(void);
	public:
		XUtil::xuVector<RECT> SpanRects;
	public:

		void RefreshSpans();
		bool InsertRow(int ARow);
		bool InsertRow(int ARow,int Number,bool bCopyMode);
		bool DeleteRow(int ARow);
		bool MoveRow(int From,int To);

		bool InsertCol(int ACol);
		bool InsertCol(int ACol,int Number,bool bCopyMode);
		bool DeleteCol(int ACol);
		bool MoveCol(int FromIndex,int ToIndex);
	public:
		TGridRect GetFullRect(TGridRect ARect);
		bool Merge(TGridRect ARect);
		bool RMerge(TGridRect ARect);
		bool CMerge(TGridRect ARect);
		bool UnMerge(TGridRect ARect);
	public:
		bool isSpaned(int ACol,int ARow);
		bool isSpaned(int ACol, int ARow, TGridRect &ARect);
		bool isSpaned(int ACol, int ARow, TGridRect &ARect,int &index);
		bool GetSpanedRowCol(int ACol,int ARow,int &SCol,int &SRow);
	};
}}}}
