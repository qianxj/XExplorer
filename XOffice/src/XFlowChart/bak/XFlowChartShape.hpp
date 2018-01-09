#pragma once

#include <xframe.h>
#include <xofbase.h>
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	#define XFONORMAL	0x0000
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

	class XFlowChartObject
	{
	public:
		XFlowChartObject();
		~XFlowChartObject();
	public:
		typedef int XFlowChartObjectStatus;
		int m_nFlowObjectIns ;

		enum shape {rect,line,roundrect,ellipserect,ployrect,bezierrect,bezierrects,flowstart,flowend};
		RECT FRect;

		POINT * m_ptHands;
		int m_nRowHeight;
		XFlowChartObjectStatus m_Status;
		COLORREF	m_startColor;
		COLORREF	m_endColor;

		LPTSTR	m_pCaption;
		//class XFlowChartObjectLink*	pLinks;
		//PXFlowChartObjectCategory *	m_ppItems;
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
		//int AddItem(int nCategory, PXFlowChartObjectItem pItem);
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
}}}}
