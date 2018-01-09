#pragma once

#include "xbxbase.h"
namespace Hxsoft{ namespace XFrame
{
	class IDwView;
	class XOFOBJECT_API IDwViewObserver :
		public xbObserver
	{
	public:
		virtual int RowFoucsChanging(IDwView * pDwView, int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int RowFoucsChanged(IDwView * pDwView,int nNewRow,int nOldRow)
		{
			return 0;
		}

		virtual int ItemChanged(IDwView * pDwView,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemChanging(IDwView * pDwView,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int Clicked(IDwView * pDwView,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int DoubleClicked(IDwView * pDwView,int nNewRow,int nOldRow)
		{
			return 0;
		}
		virtual int EditChanging(IDwView * pDwView,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int EditChanged(IDwView * pDwView,int nRow,int nCol,LPCTSTR pData)
		{
			return 0;
		}
		virtual int ItemFocueChanged(IDwView * pDwView,int nRow,int nCol)
		{
			return 0;
		}
		virtual int ItemFocueChanging(IDwView * pDwView,int nRow,int nCol)
		{
			return 0;
		}
		virtual int GetFocus(IDwView  * pLostDwTable)
		{
			return 0;
		}
		virtual int LoseFocus(IDwView  * pFocusedDwTable)
		{
			return 0;
		}
	};
}}