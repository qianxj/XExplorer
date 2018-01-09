#pragma once
#include "xbxbase.h"
#include "ihttabledata.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class IHtTableView : public xbObject
	{
	public:
		virtual IHtTableData * SetTableData(IHtTableData * pData)=0;
		virtual IHtTableData * GetTableData()=0;

		virtual int GetHeight()=0;
		virtual int GetWidth()=0;

		virtual int GetRowHeight(int nRow)=0;
		virtual int GetColWidth(int nCol)=0;

		virtual LPCTSTR GetItemString(int nCol,int nRow)=0;
		virtual void SetItemString(int nCol,int nRow,LPTSTR pStr)=0;
		
		virtual LPIHtContentItem GetItem(int nCol,int nRow)=0;
		virtual void SetItem(int nCol,int nRow,LPIHtContentItem pItem)=0;

		virtual RECT GetCellRect(int nCol,int nRow)=0;

		virtual int DoDrawTo(HDC hdc,RECT &rc, int startCol,int startRow)=0; 
	};

}}}