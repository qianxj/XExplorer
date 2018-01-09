#pragma once
#include "commctrl.h"
#include "xframe.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	#define XTabStyle	int;
	#define	XTabState	int;
	
	class XOFBaseFrmSvr;
	class XOFFICE_API CXTabSheet :public xfControl
	{
	public:
		enum XTabDrawStyle{tdsRoundRect,tdsRoundRectEx,tdsShape,tdsEclips,tdsEclipsEx,tdsCanClose};
		enum XTabArraow{taTop,taBottom,taLeft,taRight};
	public:
		CXTabSheet(void);
		~CXTabSheet(void);
	public:
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
		XOFBaseFrmSvr*	m_pOfficeSvr;
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

	public:
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
		bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		virtual class xfWin* GetWin();
	public:
		bool SetEventListons();
		bool UnSetEventListons();
		virtual bool GetClientRect(RECT *pRect); 
		virtual HWND GetHWND();
	};
}}}
