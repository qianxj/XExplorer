#pragma once

#include "xbxbase.h"
#include "xofobject.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class IGridReport;
	class IGridReportObserver :public IDwViewObserver
	{
	public:
		virtual int RowFoucsChanging(IGridReport * pDwTable, int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int RowFoucsChanged(IGridReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}

		virtual int ItemChanged(IGridReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemChanging(IGridReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int Clicked(IGridReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int DoubleClicked(IGridReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int EditChanging(IGridReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int EditChanged(IGridReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemFocueChanged(IGridReport * pDwTable,int nRow,int nCol)
		{
			return 0;
		}
		virtual int ItemFocueChanging(IGridReport * pDwTable,int nRow,int nCol)
		{
			return 0;
		}
		virtual int GetFocus()
		{
			return 0;
		}
		virtual int LoseFocus()
		{
			return 0;
		}
	};
}}}}
