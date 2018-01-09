#pragma once

#include "xofbase.h"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class  XOFOBJECT_API IAxisSvr :
		public CSvrbase
	{
	public:
		virtual XAPI int GetColWidth(int column) = 0;
		virtual XAPI int GetRowHeight(int row) = 0;
		virtual XAPI int SetColWidth(int column,int value) = 0;
		virtual XAPI int SetRowHeight(int row ,int value) = 0;
		virtual XAPI int SetColWidth(int startcolumn,int endcolumn,int value) = 0;
		virtual XAPI int SetRowHeight(int startrow ,int endrow, int value) = 0;

	public:
		virtual XAPI int GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight) = 0;
		virtual XAPI int GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth) = 0;
	};

	class XOFOBJECT_API XAxisSvr :
		public IAxisSvr
	{
	public:
		XAxisSvr(int stdWidth,int stdHeight,int stdRowSelectorWidth,int stdColHeaderHeight);
		~XAxisSvr(void);
	
	private:
		XUtil::xuSummableCsArray< int, USHORT>*  m_pRowHeights;   
		XUtil::xuSummableCsArray< int, USHORT>*  m_pColWidths;
	
	public:
		virtual XAPI int GetColWidth(int column);
		virtual XAPI int GetRowHeight(int row);
		virtual XAPI int SetColWidth(int column,int value);
		virtual XAPI int SetRowHeight(int row ,int value);
		virtual XAPI int SetColWidth(int startcolumn,int endcolumn,int value);
		virtual XAPI int SetRowHeight(int startrow ,int endrow, int value);

	public:
		virtual XAPI int GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight);
		virtual XAPI int GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth);
	};
}}}
