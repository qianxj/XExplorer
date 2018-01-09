#pragma once
#include "commctrl.h"
#include <xcontrol.h>

namespace Hxsoft{ namespace XFrame
{
	#define XTabStyle	int;
	#define	XTabState	int;
	
	class XLayers;
	class  XLayerSheetCtrl : public XControl 
	{
	public:
		enum XTabDrawStyle{tdsRoundRect,tdsRoundRectEx,tdsShape,tdsEclips,tdsEclipsEx};
		enum XTabArraow{taTop,taBottom,taLeft,taRight};
	public:
		XLayerSheetCtrl(void);
		~XLayerSheetCtrl(void);
	public:
		XLayers*	m_pLayer;

		RECT	FRect;
		int		m_iCurSheet;
		int		m_iStartSheet;
		
		LPTSTR * m_pSheets;
		int	*	m_pSheetsImage;
		int		m_SheetSize;
		int		m_SheetAllocSize;

		int *	m_pWidths;

		HIMAGELIST	m_hImageList;
		int		m_cxImage;
		int		m_cyImage;
		
		XTabArraow	m_TabArrow;
		XTabDrawStyle	m_TabDrawStyle;
	public:
		int AddSheet(LPTSTR pSheet,int iImage = -1,bool bToCurrent = false);
		int InsertSheet(int iPos,LPTSTR pSheet,int iImage=-1,bool bToCurrent = false);
		int RemoveSheet(int iPos);
		int Clear();
		int MoveSheet(int From,int To);
		int AppendSheetAlloc(int inc);
	
	public:
		void SetImageList(HIMAGELIST hImageList){m_hImageList = hImageList;}
		HIMAGELIST GetImageList(){return m_hImageList;}
		void SetImageExtent(int cx = 16,int cy=16);
	public:
		int Redraw();
		int EventSheetChanged(int NewCurSheet);
		int ReCalcWidths(HDC hdc);
		int SelectSheet(int NewCurSheet);

	public:
		virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
		virtual int OnDraw(HDC hdc);
		virtual int DrawSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawEclipseSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawEclipseExSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawShapeSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawRoundRectSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawRoundRectExSheet(HDC hdc,int iSheet, RECT rect);
	public:
		RECT	GetSheetRect(int Index);
		int		GetSheetFromPoint(POINT & pt);
	public:
		void OnLButtonDown(UINT nFlags,POINT point);
		void OnLButtonUp(UINT nFlags,POINT point);
		void OnMouseMove(UINT nFlags,POINT point);
	};
}}
