#pragma once

#include "xbxbase.h"
#include <vector>
using namespace std;
#include "xshapedrawobject.hpp"

namespace Hxsoft{ namespace XFrame{

	class XSHAPE_API XShapeObject :
		public xbObject
	{
	public:
		XShapeObject(void);
		~XShapeObject(void);
	public:
		vector<class XShapeDrawObject *> * m_DrawObjects;
		int LoadShapeXml(LPTSTR pStrXml);
		int LoadShape(IXMLDOMElement * m_pElement);
	public:
		LPTSTR m_pShapeName;
		LPTSTR m_pShapeLabel;
	public:
		int m_nCssStyle;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT rc,DWORD dwDrawMask = DMASKHOT);
	public:
		int GetHotedPoints(const RECT &rect,vector<POINT>* pPoints);
		int GetSelectPoints(const RECT &rect,vector<POINT>* pPoints);
		int GetHotedArrows( vector<int>* pArrows);
	public:
		bool GetHotHitedObject(int nIndex, XShapeDrawObject * &pObj,int & nIndexObj);
		int GetHotHitedIndex(XShapeDrawObject * pObj,int  nIndexObj);
	};
}}
