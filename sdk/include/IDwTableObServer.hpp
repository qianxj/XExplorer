#pragma once

#include "xbxbase.h"
#include "xofobject.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class IDwTable;
	class  IDwTableObserver :
		public IDwViewObserver 
	{
	public:
		virtual int RowFoucsChanging(IDwTable * pDwTable, int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int RowFoucsChanged(IDwTable * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}

		virtual int ItemChanged(IDwTable * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemChanging(IDwTable * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int Clicked(IDwTable * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int DoubleClicked(IDwTable * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int EditChanging(IDwTable * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int EditChanged(IDwTable * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemFocueChanged(IDwTable * pDwTable,int nRow,int nCol)
		{
			return 0;
		}
		virtual int ItemFocueChanging(IDwTable * pDwTable,int nRow,int nCol)
		{
			return 0;
		}
		virtual int GetFocus(IDwTable  * pLostDwTable)
		{
			return 0;
		}
		virtual int LoseFocus(IDwTable  * pFocusedDwTable)
		{
			return 0;
		}
	};
}}}}
