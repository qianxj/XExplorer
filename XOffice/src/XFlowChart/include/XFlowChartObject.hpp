#pragma once

#include <xframe.h>
#include <xofbase.h>
#include <xshape.h>
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	typedef int XFODrawState;
	#define XFOSELECTION 0x0000
	#define XFOLINK		 0x0001
	#define XFOWORKNODE	 0x0002

	typedef int XFOState;
	#define XFONORMAL	0x0000
	#define XFOSELECTED 0x0001
	#define XFOMOVING	0x0002
	#define XFORESIZING	0x0004
	#define XFOLINKING	0x0008
	#define XFOCREATING	0x0010
	#define XFOWORKNODING	0x0020
	#define XFOREADONLY	0x1000
    #define XFORUNSTATE 0x8000

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

	class XFLOWCHART_API XFlowChartObject
	{
	public:
		XFlowChartObject();
		~XFlowChartObject();
	public:
		typedef int XFlowChartObjectStatus;
		TDrawPoint m_ptTopLeft;
		TDrawPoint m_ptBottomRight;
		LPTSTR m_pRefUri;
		LPTSTR m_pRefName;
		
	public:
		LPTSTR	m_pCssStyle;
		int m_nCssStyle;
		LPTSTR	m_pStrCaption;
		int LoadObject(IXMLDOMElement * pElement);
		int LoadObjectXml(LPTSTR pStrXml);
	
	public:
		class XFlowChartDataSvr* m_pDataSvr;

	public:
		virtual void DoDraw(HDC hPaintDC,RECT rc);
	public:
		LPTSTR	m_pGuid;
		LPTSTR	m_pStrID;
	public:
		RECT	FRect;	
		void SetFRect(RECT rect);
	public:
		XFOState m_FState;
		XFOHitPosition GetHitResizePat(POINT pt);
	public:
		void InvalidatePatRgn(HWND hWnd,POINT & ptOffset);
		void HitAdjust(XFOHitPosition htpos,int cx,int cy);
	public:
		int TestHotHited(POINT pt,RECT &rc);
		int DrawHoted(HDC hPaintDC,int nIndex);
	public:
		vector<POINT>* m_phtPoints;
		vector<POINT>* m_pstPoints;
		vector<int>* m_phtArrows;
	public:
		IXMLDOMElement* m_pElement;
		IXMLDOMElement* m_pDataElement;
	public:
		LPTSTR GetCaption();
		IXMLDOMElement* GetElement();
		LPTSTR GetShapeType();
		RECT & GetRect();
	public:
		bool SetUrl(LPTSTR pUrl);
		bool SetCaption(LPTSTR pCaption);
		LPTSTR GetUrl();
		LPTSTR GetGuid();
	};
}}}}
