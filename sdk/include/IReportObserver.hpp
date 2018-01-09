#pragma once

#include "xbxbase.h"
#include "xofobject.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class IReport;
	class IReportObserver :public IDwViewObserver
	{
	public:
		virtual int RowFoucsChanging(IReport * pDwTable, int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int RowFoucsChanged(IReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}

		virtual int ItemChanged(IReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemChanging(IReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int Clicked(IReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int DoubleClicked(IReport * pDwTable,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int EditChanging(IReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int EditChanged(IReport * pDwTable,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemFocueChanged(IReport * pDwTable,int nRow,int nCol)
		{
			return 0;
		}
		virtual int ItemFocueChanging(IReport * pDwTable,int nRow,int nCol)
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
