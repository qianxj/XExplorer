#pragma once
#include <xframe.h>
#include "xceditsvr.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcPropSheetCtrl:
			public xfControl
		{
		public:
			xcPropSheetCtrl();
			~xcPropSheetCtrl();
		public:
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		public:
			xbXml * m_pXml; 
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			XControl::CEditSvr * m_pEditSvr;
			static LPTSTR GetCellContent(xfControl* m_pOwner, int ACol,int ARow);
			static int SetCellContent(xfControl* m_pOwner, int ACol,int ARow, LPTSTR pStr);
			static int GetCellRect(xfControl* m_pOwner, int ACol,int ARow,RECT &rt);
			static LPTSTR GetEditType(xfControl* m_pOwner, int ACol,int ARow);
			static HFONT GetEditFont(xfControl* m_pOwner,int ACol,int ARow);
			RECT m_editRt;
		public:
			bool	ib_sort;
			long	il_startx,il_starty;
			long	il_width,il_height;

			long	il_treebp ;
			long	il_treebox ;
			long	il_toolh ;
			long	il_labelsw ;
			long	il_startrow ; //from start 1
			long	il_cureditrow ;
			long	il_cellheight ;
			//long	il_row;
			wstring	is_colname;
			long	il_startY ;
		public:
			HFONT	m_hFont;
		};
	}
}
