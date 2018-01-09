#pragma once

#include <xframe.h>
#include <xofbase.h>
#include "XFlowChartObject.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	#define FXOLineStyle int
	#define FXOLINENORMAL 0x0000
	#define FXOLINEDIRECT 0x0001
	#define FXOLINECURVE  0x0002
	#define FXOLINEOWNER  0x0100

	class XFLOWCHART_API XFlowChartLink
	{
	public:
		XFlowChartLink();
		~XFlowChartLink();
	public:
		XFlowChartObject * m_pStartObject;
		XFlowChartObject * m_pEndObject;
		POINT	m_ptStart;
		int m_nStartIndex;
		int m_nEndIndex;
		POINT m_ptEnd;
	public:
		LPTSTR m_pGuid;
	public:
		XFOState m_FState;
		FXOLineStyle m_FLineStyle;
	public:
		virtual void DoDraw(HDC hPaintDC,RECT rc);
		virtual void DoDrawArrow(HDC hPaintDC,POINT ptStart,POINT ptEnd,bool bStart = true);
		//virtual void DrawBackGround(HDC hPaintDC,RECT rect);
		vector<POINT>* m_pPloys;
		int DrawHoted(HDC hPaintDC,int nIndex);
	public:
		int AutoCalcPoints(const vector<RECT>* rects = NULL);
		int AutoCalcPoints(POINT ptStart,int paStart,POINT ptEnd,int paEnd, FXOLineStyle dwLineStyle,const vector<RECT>* rects,vector<POINT>* pts);
	public:
		vector<POINT>* m_phtPoints;
		vector<POINT>* m_pstPoints;
	public:
		int LoadObject(vector<class XFlowChartObject *> &DrawObjects, IXMLDOMElement * pElement);
		bool IsHoted(int x,int y);
	public:
		XFlowChartObject * GetStartObject();
		XFlowChartObject * GetEndObject();
	};
}}}}
