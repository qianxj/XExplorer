#pragma once
#include "xbxbase.h"
#include "tchar.h"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	typedef struct tagIHtContentItem 
	{
		enum IHtContentItemType {eNone,eText,eObject,eElement,eInt,eDouble,eGuid,eHtObject};
		IHtContentItemType et;
		union 
		{
			LPTSTR text;
			LPVOID object;
			double dval;
			int	   ival;
			GUID   guid;
		};
		
		void CopyFrom(tagIHtContentItem * pSrc)
		{
			switch(et)
			{
			case eText:
				if(text)delete text;
			}
			switch(pSrc->et)
			{
			case eText:
				text = _tcsdup(pSrc->text);
				break;
			case eDouble:
				dval = pSrc->dval;
				break;
			case eInt:
				ival = pSrc->ival;
			case eGuid:
				guid = pSrc->guid;
			default:
				object = pSrc->object;
				break;
			}
		}
	} IHtContentItem, * LPIHtContentItem;

	class IHtTableContent : public xbObject
	{
	public:
		virtual LPIHtContentItem GetItemData(int ACol,int ARow)=0;
		virtual int  SetItemData(int ACol,int ARow,LPIHtContentItem pItem)=0;
		virtual TCHAR * GetItemString(int ACol,int ARow)=0;
		virtual int SetItemString(int ACol,int ARow, TCHAR * pData)=0;
		
		virtual int CopyTo(int FromRow,int ToRow)=0;
		virtual int MoveRow(int FromRow,int ToRow)=0;
		virtual int MoveRow(int FromRow,int ToRow,int Number)=0;
		virtual int MoveCol(int FromIndex,int ToIndex)=0;
		
		virtual void ClearRowData(int ARow)=0;

		virtual IHtContentItem::IHtContentItemType GetItemType(int ACol,int ARow)=0;
		virtual int SetItemType(int ACol,int ARow, IHtContentItem::IHtContentItemType dt)=0;

		virtual int GetValidRow()=0;
		virtual int GetValidCol(int ARow)=0;
		virtual int GetValidRowCol(int &ACol,int &ARow)=0;
		virtual int GetValidCol()=0;
		//clear data and data type
		virtual int Reset()=0;

	public:		
		virtual int SetRowCount(int RowCount)=0;
		virtual int SetColCount(int ColCount)=0;

		virtual int GetRowCount()=0;
		virtual int GetColCount()=0;
	};

}}}