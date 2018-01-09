#pragma once
#include "commctrl.h"
#include "xframe.h"
#include "vector"

namespace Hxsoft{ namespace XFrame
{
	#define XTabStyle	int;
	#define	XTabState	int;
	
	#define LYSN_PRESELECTSHEET	0x0001;
	#define LYSN_SELECTEDSHEET	0x0002;
	#define LYSN_CLOSESHEET	0x0003;

	struct 	LYSNMHDR : public NMHDR
	{
		int nSheet;
	};

	class XLayers;
	class  XCONTROL_API xcLayerSheet : public xfControl 
	{
	public:
		enum XTabDrawStyle{tdsRoundRect,tdsRoundRectEx,tdsShape,tdsEclips,tdsEclipsEx,tdsCanClose};
		enum XTabArraow{taTop,taBottom,taLeft,taRight};
		enum sheetState{_hidden, _show};
	public:
		xcLayerSheet(void);
		~xcLayerSheet(void);
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
		int GetSheetCount();
	
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
		virtual int OnDraw(HDC hdc);
		virtual int DrawSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawEclipseSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawEclipseExSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawShapeSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawCloseSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawRoundRectSheet(HDC hdc,int iSheet, RECT rect);
		virtual int DrawRoundRectExSheet(HDC hdc,int iSheet, RECT rect);
	public:
		RECT	GetSheetRect(int Index);
		int		GetSheetFromPoint(POINT & pt);
	public:
		bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
	public:
		bool m_bMdiSheet;
		xfMdiFrame * m_pFrame;
		std::vector<HWND>* m_pHwnds;
	public:
		virtual	void Initial();
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public: 
		xbObject * GetInterface(); 
	public:
		virtual int GetXml(BSTR &bstr);
	public:
		bool m_bClose;
		int SetCloseButton(int nSheet, bool bClose);
	public:
		std::vector<sheetState>* m_pSheetState;
		int SetSheetState(int sheet, int state);
		int GetSheetState(int sheet);
		int SetSheetText(int sheet, LPTSTR text);
		LPTSTR GetSheetText(int sheet);
	};
}}
