#pragma once

#include "ihttablecontent.hpp"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class CHtTableContent :
		public IHtTableContent
	{
	public:
		CHtTableContent(void);
		~CHtTableContent(void);
	
	protected:
		XUtil::xuSpList<XUtil::xuSpList<void *>*>*  m_pData;
		int FSysCount;
	public:
		virtual LPIHtContentItem GetItemData(int ACol,int ARow);
		virtual int  SetItemData(int ACol,int ARow,LPIHtContentItem pItem);
		virtual TCHAR * GetItemString(int ACol,int ARow);
		virtual int SetItemString(int ACol,int ARow, TCHAR * pData);
		
		virtual int CopyTo(int FromRow,int ToRow);
		virtual int MoveRow(int FromRow,int ToRow);
		virtual int MoveRow(int FromRow,int ToRow,int Number);
		virtual int MoveCol(int FromIndex,int ToIndex);
		
		virtual void ClearRowData(int ARow);

		virtual IHtContentItem::IHtContentItemType GetItemType(int ACol,int ARow);
		virtual int SetItemType(int ACol,int ARow, IHtContentItem::IHtContentItemType dt);

		virtual int GetValidRow();
		virtual int GetValidCol(int ARow);
		virtual int GetValidRowCol(int &ACol,int &ARow);
		virtual int GetValidCol();
		//clear data and data type
		virtual int Reset();

	public:		
		virtual int SetRowCount(int RowCount);
		virtual int SetColCount(int ColCount);

		virtual int GetRowCount();
		virtual int GetColCount();
	public:
		int FRowCount;
		int FColCount;
	};
}}}
