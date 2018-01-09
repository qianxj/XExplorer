#pragma once

#include "xbxbase.h"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame{
	#define DMASKHOT 0x0001
	#define DMASKSELECTED 0x0002

	#define THOTARROWANY   0x0000
	#define THOTARROWNORTH 0x0001
	#define THOTARROWSOUTH 0x0002
	#define THOTARROWWEST  0x0004
	#define THOTARROWEAST  0x0008
	#define THOTARROWNE	   0x0010
	#define THOTARROWSW	   0x0020
	#define THOTARROWSE    0x0040
	#define THOTARROWNW	   0x0080

	typedef struct XSHAPE_API tagDrawPoint
	{
		#define	TDrawPointMask_X	0x0001
		#define	TDrawPointMask_CX	0x0002
		#define	TDrawPointMask_STRX	0x0004
		#define	TDrawPointMask_Y	0x0010
		#define	TDrawPointMask_CY	0x0020
		#define	TDrawPointMask_STRY	0x0040

		XUtil::Css::TMeasure x;
		LPTSTR pStrX;
		XUtil::Css::TMeasure y;
		LPTSTR pStrY;
		DWORD  mask;
		tagDrawPoint():pStrX(NULL),pStrY(NULL),mask(0){};
		~tagDrawPoint()
		{
			if(pStrX) delete pStrX;
			if(pStrY) delete pStrY;
		};
	}TDrawPoint;

	class XSHAPE_API XShapeDrawObject :
		public xbObject
	{
	public:
		XShapeDrawObject(void);
		~XShapeDrawObject(void);
	public:
		enum TShape {_none,_line,_rect,_round,_ellipse,_image,_text,_control,_shape,_arrowline};
		TShape eShape;

	public:
		LPTSTR	m_pCssStyle;
		int	m_nCssStyle;
		int LoadObject(IXMLDOMElement * pElement);
		int LoadObjectXml(LPTSTR pStrXml);
	public:
		vector<TDrawPoint>* m_pPoints;
		vector<TDrawPoint>* m_phtPoints;
		vector<TDrawPoint>* m_pstPoints;
		vector<int>*	m_phtArrows;
		tagDrawPoint m_ptBorder;
	public:
		XByteStream* m_pImageStream;
		void*  m_pImage;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT rc,DWORD dwDrawMask = 0);
	public:
		int CalcPoints(POINT pointCur ,const RECT &rc, vector<TDrawPoint> * mPoints, vector<POINT> *  points);
		int GetHotedPoints( const RECT &rect, POINT ptCur, vector<POINT>* pPoints);
		int GetSelectPoints(const RECT &rect, POINT ptCur, vector<POINT>* pPoints);
		int GetHotedArrows( vector<int>* pArrows);
	};
}}
