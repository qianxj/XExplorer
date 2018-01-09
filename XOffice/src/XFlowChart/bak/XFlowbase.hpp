#pragma once

#include <xframe.h>
#include <xofbase.h>
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	typedef struct tagXFlowObjectItem
	{
		LPTSTR m_StrLabel;
	}FXFlowObjectItem,*PXFlowObjectItem;

	#define XFONORMAL 0x0000
	#define XFOSELECTED 0x0001
	#define XFOMOVING	0x0002
	#define XFORESIZING	0x0004

	typedef int XFOHitPosition;
	#define XFOHITNONE		0x0000
	#define XFOHITLEFTTOP		0x0001	
	#define XFOHITRIGHTTOP		0x0002	
	#define XFOHITRIGHTBOTTOM	0x0004	
	#define XFOHITLEFTBOTTOM	0x0008	

	#define XFOHITTOPCENTER		0x0010	
	#define XFOHITRIGHTCENTER	0x0020	
	#define XFOHITBOTTOMCENTER	0x0040	
	#define XFOHITLEFTCENTER	0x0080

	class XFlowObjectLink;
	class XFlowObject
	{
	public:
		XFlowObject();
		~XFlowObject();
	public:
		typedef int XFlowObjectStatus;
		typedef PXFlowObjectItem * PXFlowObjectCategory;
		int m_nFlowObjectIns ;

		enum shape {rect,line,roundrect,ellipserect,ployrect,bezierrect,bezierrects,flowstart,flowend};
		RECT FRect;

		POINT * m_ptHands;
		int m_nRowHeight;
		XFlowObjectStatus m_Status;
		COLORREF	m_startColor;
		COLORREF	m_endColor;

		LPTSTR	m_pCaption;
		XFlowObjectLink*	pLinks;
		PXFlowObjectCategory *	m_ppItems;
		int	m_nCategory;
		int * m_nCategoryItems;
		LPVOID m_pData;
	private:
		int m_nLimit;
		int * m_pLimits;
	public:
		virtual void DoDraw(HDC hPaintDC);
		virtual void DrawBackGround(HDC hPaintDC,RECT rect);
		shape	m_shape;

		POINT	*m_pPloys;
		int		m_nPloys;
	private:
		int		m_nPloysLimit;
		int		m_nPloysIns;

	public:
		XFOHitPosition GetHitResizePat(POINT pt);
	public:
		int AddItem(int nCategory, PXFlowObjectItem pItem);
		void RemoveItem(int nCategory,int nItem);
		int AddCategory();
		void RemoveCategory(int nCategory);
	public:
		void InvalidatePatRgn(HWND hWnd);
		virtual void HitAdjust(XFOHitPosition htpos,int cx,int cy);
	public:
		int AddPoint(POINT pt);
		int AddPoint(int px,int py);
		void Removepoint(int nIndex);
		int InsertPoint(int nIndex,POINT pt);
		void ClearPoint();
	};

	class XFlowObjectLink :public XFlowObject	
	{
	public:
		XFlowObjectLink();
	public:
		int LinkPos;
		int LinkType;
		XFlowObject*	pFromFlowObject;
		XFlowObject*	pToFlowObject;
		virtual void DoDraw(HDC hPaintDC);
	};


	class XFLOWCHART_API XFlowbase:
		public XOfficeSheetSvr
	{
	public:
		XFlowbase();
		~XFlowbase();
	public:
		XFlowObject ** m_ppXFlowObjects;
		XFlowObjectLink ** m_ppXFlowObjectLinks;
		int	m_nObjectCount;
		int	m_nObjectLinkCount;

	public:
		typedef int XFlowBaseStatus;
		XFlowBaseStatus m_status;

	private:
		int m_nObjectLement;
		int m_nObjectLinkLement;
		int m_nObjectIns;
		POINT	m_ptMouse;
		POINT	m_ptHittest;
	public:
		virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);

	public:
		virtual void OnLButtonDown(UINT nFlags,POINT point);
		virtual void OnLButtonUp(UINT nFlags,POINT point);
		virtual void OnMouseMove(UINT nFlags,POINT point);
		virtual BOOL OnSetCursor( HWND hWnd, UINT nHitTest, UINT message );
		virtual void OnChar( UINT nChar,UINT nRepCnt,UINT nFlags);
		virtual int  OnSize(UINT nFlags, POINT point);
	public:
		virtual int  AddFlowObject(XFlowObject*  pFlowObject);
		virtual void RemoveFlowObject(int nPos);
		virtual int AddFlowObjectLink(XFlowObjectLink * pFlowObjectLink);
		virtual void RemoveFlowObjectLink(int nPos);
	public:
		XFlowObject * m_pHitObject;
		XFlowObject * m_pHitObjectLink;
		XFOHitPosition	m_nHitPos;
	};
}}}}
