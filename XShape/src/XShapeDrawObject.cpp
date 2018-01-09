#include "StdAfx.h"
#include "..\include\XShapeDrawObject.hpp"
#include <gdiplus.h>
#include "shlwapi.h"

using namespace Gdiplus;

namespace Hxsoft{ namespace XFrame
{
	struct tagDrawObjectInfo
	{
		LPTSTR pStrItemName;
		XShapeDrawObject::TShape eItem;
	};

	tagDrawObjectInfo infos[]=
	{
		{_T("control"),		XShapeDrawObject::_control},
		{_T("image"),		XShapeDrawObject::_image},
		{_T("line"),		XShapeDrawObject::_line},
		{_T("rect"),		XShapeDrawObject::_rect},
		{_T("round"),		XShapeDrawObject::_round},
		{_T("ellipse"),		XShapeDrawObject::_ellipse},
		{_T("text"),		XShapeDrawObject::_text},
		{_T("shape"),		XShapeDrawObject::_shape},
		{_T("arrowline"),	XShapeDrawObject::_arrowline}
	};

	XShapeDrawObject::TShape FindDrawObject(LPTSTR pstrItem)
	{
		for(int i=0;i<sizeof(infos)/sizeof(infos[0]);i++)
		{
			if(_tcsicmp(infos[i].pStrItemName,pstrItem)==0) return infos[i].eItem;
		}
		return XShapeDrawObject::_none;
	}

	int GetExtent(XUtil::Css::TMeasure &mes,int extent,bool bHorz)
	{
		if(mes.measureRef==XUtil::Css::_percentage)
			return (int)(extent*mes.percentage/100.0);
		else
			return  (int)XUtil::Css::GetPixelExtent(&mes,bHorz); 
		return 0;
	}

	XShapeDrawObject::XShapeDrawObject(void):m_pPoints(new vector<TDrawPoint>),m_nCssStyle(NULL),m_phtPoints(new vector<TDrawPoint>),
		m_pstPoints(new vector<TDrawPoint>),m_phtArrows(new vector<int>),m_pImageStream(NULL),m_pImage(NULL),m_pCssStyle(NULL)
	{
	}

	XShapeDrawObject::~XShapeDrawObject(void)
	{
		if(m_pPoints)delete m_pPoints;
		if(m_phtPoints)delete m_phtPoints;
		if(m_pstPoints)delete m_pstPoints;
		if(m_phtArrows)delete m_phtArrows;
		//if(m_pImage)delete (Image *)m_pImage;
		if(m_pImageStream)m_pImageStream->Release();

	}

	int XShapeDrawObject::LoadObject(IXMLDOMElement * pElement)
	{
		BSTR bstrTag;
		_variant_t	val;
		pElement->get_tagName(&bstrTag);
		eShape = FindDrawObject(bstrTag);
		::SysFreeString(bstrTag);

		switch(this->eShape)
		{
		case _image:
		case _rect:
		case _round:
		case _text:
		case _control:
		case _line:
		case _ellipse:
			{
				LPTSTR pStrX;
				LPTSTR pStrY;
				LPTSTR pStrA;
				vector<LPTSTR> vX;
				vector<LPTSTR> vY;
				vector<LPTSTR> vA;
				vector<LPTSTR> vA1;
				int htArrow;

				//hot allow arrow
				pStrA = xbXml::GetStringAttribe(pElement,L"ha");
				XUtil::GetTokens( pStrA, _T(","), vA);
				for(int i=0;i<(int)vA.size();i++)
				{
					vA1.clear();
					htArrow = 0;
					XUtil::GetTokens( vA[i], _T("|"), vA1);
					for(int j=0;j<(int)vA1.size();j++)
					{
						if((vA1[j][0]=='s' ||vA1[j][0]=='S') && vA1[j][1]=='\0')
							htArrow |= THOTARROWSOUTH;
						if((vA1[j][0]=='n' ||vA1[j][0]=='N') && vA1[j][1]=='\0')
							htArrow |= THOTARROWNORTH;
						if((vA1[j][0]=='w' ||vA1[j][0]=='W') && vA1[j][1]=='\0')
							htArrow |= THOTARROWWEST;
						if((vA1[j][0]=='e' ||vA1[j][0]=='E') && vA1[j][1]=='\0')
							htArrow |= THOTARROWEAST;
						if(::_tcsicmp(vA1[j],_T("nw"))==0 || ::_tcsicmp(vA1[j],_T("wn"))==0)
							htArrow |= THOTARROWNW;
						if(::_tcsicmp(vA1[j],_T("ne"))==0 || ::_tcsicmp(vA1[j],_T("en"))==0)
							htArrow |= THOTARROWNE;
						if(::_tcsicmp(vA1[j],_T("sw"))==0 || ::_tcsicmp(vA1[j],_T("ws"))==0)
							htArrow |= THOTARROWSW;
						if(::_tcsicmp(vA1[j],_T("se"))==0 || ::_tcsicmp(vA1[j],_T("es"))==0)
							htArrow |= THOTARROWSE;
					}
					m_phtArrows->push_back(htArrow);
				}

				for(int t=0;t<3;t++)
				{
					vX.clear();
					vY.clear();
					if(t==0)
					{
						pStrX = xbXml::GetStringAttribe(pElement,L"x");
						pStrY = xbXml::GetStringAttribe(pElement,L"y");
					}
					else if(t==1)
					{
						pStrX = xbXml::GetStringAttribe(pElement,L"hx");
						pStrY = xbXml::GetStringAttribe(pElement,L"hy");
					}else
					{
						pStrX = xbXml::GetStringAttribe(pElement,L"sx");
						pStrY = xbXml::GetStringAttribe(pElement,L"sy");
					}

					XUtil::GetTokens( pStrX, _T(","), vX);
					XUtil::GetTokens( pStrY, _T(","), vY);

					int nCount = (int)vX.size();
					if(nCount < (int)vY.size()) nCount = (int)vY.size();

					for(int i=nCount;i<(int)vX.size();i++)
						if(vX[i])delete vX[i];
					for(int i=nCount;i<(int)vY.size();i++)
						if(vY[i])delete vY[i];

					if(t==0)
					{
						this->m_pCssStyle = xbXml::GetStringAttribe(pElement,L"style");

						LPTSTR strWidth  = xbXml::GetStringAttribe(pElement,L"width");
						LPTSTR strHeight = xbXml::GetStringAttribe(pElement,L"height");
						if(strWidth)
						{
							m_ptBorder.mask |= TDrawPointMask_CX;
							XUtil::Css::ProcessCssMeasure(strWidth,&m_ptBorder.x);
							delete strWidth;
						}
						if(strHeight)
						{
							m_ptBorder.mask |= TDrawPointMask_CY;
							XUtil::Css::ProcessCssMeasure(strHeight,&m_ptBorder.y);
							delete strHeight;
						}
					}

					if(nCount==1) //should end;
					{
						TDrawPoint pt;
						pt.mask = TDrawPointMask_STRX;
						pt.pStrX = ::_tcsdup(_T("."));
						pt.mask = TDrawPointMask_STRY;
						pt.pStrY = ::_tcsdup(_T("."));
						if(t==0)
							m_pPoints->push_back(pt);
						else if(t==1)
							m_phtPoints->push_back(pt);
						else
							m_pstPoints->push_back(pt);
					}
					for(int i=0;i<nCount;i++)
					{
						TDrawPoint pt;
						if(vX[i][0]=='.' || vX[i][0]=='@')
						{
							pt.mask |= TDrawPointMask_STRX;
							pt.pStrX = ::_tcsdup(_T("."));
						}else if(vX[i][0]=='E')
						{
							pt.mask |= TDrawPointMask_CX;
							XUtil::Css::ProcessCssMeasure(vX[i] + 1,&pt.x);
						}else
						{
							pt.mask |= TDrawPointMask_X;
							XUtil::Css::ProcessCssMeasure(vX[i],&pt.x);
						}

						if(vY[i][0]=='.' || vY[i][0]=='@')
						{
							pt.mask |= TDrawPointMask_STRY;
							pt.pStrY = ::_tcsdup(_T("."));
						}else if(vY[i][0]=='E')
						{
							pt.mask |= TDrawPointMask_CY;
							XUtil::Css::ProcessCssMeasure(vY[i] + 1,&pt.y);
						}else
						{
							pt.mask |= TDrawPointMask_Y;
							XUtil::Css::ProcessCssMeasure(vY[i],&pt.y);
						}
						if(t==0)
							m_pPoints->push_back(pt);
						else if(t==1)
							m_phtPoints->push_back(pt);
						else
							m_pstPoints->push_back(pt);

					}
					if(pStrX) delete pStrX ;
					if(pStrY) delete pStrY ;
				}
				if(eShape==_image)
				{
					_variant_t v;
					pElement->getAttribute(L"src",&v);
					if(v.bstrVal)
					{
						TCHAR path[MAX_PATH];
						::GetModuleFileName(NULL,path,MAX_PATH);
						::PathRemoveFileSpec(path);
						::PathCombine(path,path,L"shapes\\images\\");
						::PathCombine(path,path,v.bstrVal);
						if(m_pImage) delete m_pImage;
						m_pImage = Image::FromFile(path);
					}
				}
				break;
			}
		}
		return 1;
	}
	int XShapeDrawObject::LoadObjectXml(LPTSTR pStrXml)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = LoadObject(pElement);
		pElement->Release();
		return result;
	}

	int XShapeDrawObject::DoDraw(HDC hPaintDC,RECT rc,DWORD dwDrawMask)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = XUtil::xuCCssStyle::ParseCssStyle(this->m_pCssStyle);
		switch(this->eShape)
		{
		case _rect:
		case _round:
		case _text:
		case _control:
		case _image:
		case _ellipse:
			{
				int nSaveDC = ::SaveDC(hPaintDC);
				if(m_pCssStyle)BeginPath(hPaintDC);

				POINT	ptCur;
				POINT	pt;
				int nLineWidth = 1;
				COLORREF LineColor = RGB(0,0,0);
				DWORD	nColorStyle = PS_SOLID;

				//POINT* lpPoints = new POINT[m_pPoints->size()];

				if(m_pPoints->size() < 2) break;
				if(m_ptBorder.mask & TDrawPointMask_CX)
					nLineWidth = GetExtent(m_ptBorder.x,rc.right - rc.left + 1,true);
					
				HPEN hPen = ::CreatePen(nColorStyle,nLineWidth,LineColor);
				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				
				POINT ptStart={rc.left,rc.top};
				POINT ptEnd  ={rc.right,rc.bottom};
				if(!((*m_pPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[0].pStrX,_T("."))==0 &&
					 (*m_pPoints)[0].mask & TDrawPointMask_STRY &&  ::_tcsicmp((*m_pPoints)[0].pStrY,_T("."))==0))
				{
					pt.x = rc.left;
					pt.y = rc.top;
					GetCurrentPositionEx(hPaintDC,&ptCur);
					if((*m_pPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[0].pStrX,_T("."))==0)
						pt.x = ptCur.x;
					else if((*m_pPoints)[0].mask & TDrawPointMask_CX )
						pt.x = ptCur.x +  GetExtent((*m_pPoints)[0].x,rc.right - rc.left + 1,true);
					else
					{
						double dx = GetExtent((*m_pPoints)[0].x,rc.right - rc.left + 1,true);
						pt.x = dx >= 0? rc.left +dx:rc.right - dx;
					}

					if((*m_pPoints)[0].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pPoints)[0].pStrY,_T("."))==0)
						pt.y = ptCur.y;
					else if((*m_pPoints)[0].mask & TDrawPointMask_CY )
						pt.y = ptCur.y +  GetExtent((*m_pPoints)[0].y,rc.bottom - rc.top + 1,true);
					else
					{
						double dy = GetExtent((*m_pPoints)[0].y,rc.bottom - rc.top + 1,true);
						pt.y = dy>=0 ? rc.top +dy:rc.bottom - dy ;
					}
					ptStart = pt;
				}

				for(int i=1;i<2;i++)
				{
					pt.x = 0;
					pt.y = 0;
					GetCurrentPositionEx(hPaintDC,&ptCur);
					if((*m_pPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[i].pStrX,_T("."))==0)
						pt.x = ptCur.x;
					else if((*m_pPoints)[i].mask & TDrawPointMask_CX )
						pt.x = ptCur.x +  GetExtent((*m_pPoints)[i].x,rc.right - rc.left + 1,true);
					else
						pt.x = rc.left + GetExtent((*m_pPoints)[i].x,rc.right - rc.left + 1,true);

					if((*m_pPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pPoints)[i].pStrY,_T("."))==0)
						pt.y = ptCur.y;
					else if((*m_pPoints)[i].mask & TDrawPointMask_CY )
						pt.y = ptCur.y +  GetExtent((*m_pPoints)[i].y,rc.bottom - rc.top + 1,true);
					else
						pt.y = rc.top + GetExtent((*m_pPoints)[i].y,rc.bottom - rc.top + 1,true);
					 ptEnd = pt;
				}
				if(eShape==_ellipse)
					::Ellipse(hPaintDC,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
				else if(eShape==_rect)
					::Rectangle(hPaintDC,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
				else if(eShape==_round)
					::RoundRect(hPaintDC,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y,5,5);

				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);

				if(m_pCssStyle)
				{
					::EndPath(hPaintDC);
					HRGN hRgn = PathToRegion(hPaintDC);
					SelectClipRgn(hPaintDC,hRgn);
					::DeleteObject(hRgn);
	
					::InflateRect(&rc,2,2);
					XUtil::Css::DrawCssFrame(hPaintDC,&rc,pBoxStyle);
					::InflateRect(&rc,-2,-2);
				}

				::RestoreDC(hPaintDC,nSaveDC);

				if(eShape==_image && m_pImage)
				{
					Graphics graph(hPaintDC);
					graph.SetPageUnit(UnitPixel);
				
					RECT rt ={ptStart.x,ptStart.y,ptEnd.x, ptEnd.y};
					
					int nWidth = ((Image *)m_pImage)->GetWidth();
					int nHeight = ((Image *)m_pImage)->GetHeight();

					if(nWidth > 0)
					{
						if(1.0*(rt.right - rt.left)/nWidth*nHeight > rt.bottom - rt.top)
						{
							graph.DrawImage(((Image *)m_pImage),rt.left +((rt.right - rt.left) - (int)1.0 *(rt.bottom - rt.top) * nWidth /nHeight)/2 ,rt.top,(int)1.0 *(rt.bottom - rt.top) * nWidth /nHeight ,rt.bottom - rt.top);
						}else
							graph.DrawImage(((Image *)m_pImage),rt.left,rt.top + ((rt.bottom - rt.top) - (int)1.0 *(rt.right - rt.left) * nHeight /nWidth)/2.0 ,rt.right - rt.left ,(int)1.0 *(rt.right - rt.left) * nHeight /nWidth);
					}
				}

				if(dwDrawMask & DMASKSELECTED)
				{
					hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
					hPen = (HPEN)SelectObject(hPaintDC,hPen);

					for(int i=0;i<(int)m_pstPoints->size();i++)
					{
						pt.x = 0;
						pt.y = 0;
						GetCurrentPositionEx(hPaintDC,&ptCur);
						if((*m_pstPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pstPoints)[i].pStrX,_T("."))==0)
							pt.x = ptCur.x;
						else if((*m_pstPoints)[i].mask & TDrawPointMask_CX )
							pt.x = ptCur.x +  GetExtent((*m_pstPoints)[i].x,rc.right - rc.left + 1,true);
						else
							pt.x = rc.left + GetExtent((*m_pstPoints)[i].x,rc.right - rc.left + 1,true);

						if((*m_pstPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pstPoints)[i].pStrY,_T("."))==0)
							pt.y = ptCur.y;
						else if((*m_pstPoints)[i].mask & TDrawPointMask_CY )
							pt.y = ptCur.y +  GetExtent((*m_pstPoints)[i].y,rc.bottom - rc.top + 1,true);
						else
							pt.y = rc.top + GetExtent((*m_pstPoints)[i].y,rc.bottom - rc.top + 1,true);
						::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
					}
					hPen = (HPEN)SelectObject(hPaintDC,hPen);
					::DeleteObject(hPen);
				}

				if(dwDrawMask & DMASKHOT)
				{
					hPen = ::CreatePen(PS_DASH,1,RGB(0,0,255));
					hPen = (HPEN)SelectObject(hPaintDC,hPen);

					for(int i=0;i<(int)m_phtPoints->size();i++)
					{
						pt.x = 0;
						pt.y = 0;
						GetCurrentPositionEx(hPaintDC,&ptCur);
						if((*m_phtPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_phtPoints)[i].pStrX,_T("."))==0)
							pt.x = ptCur.x;
						else if((*m_phtPoints)[i].mask & TDrawPointMask_CX )
							pt.x = ptCur.x +  GetExtent((*m_phtPoints)[i].x,rc.right - rc.left + 1,true);
						else
							pt.x = rc.left + GetExtent((*m_phtPoints)[i].x,rc.right - rc.left + 1,true);

						if((*m_phtPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_phtPoints)[i].pStrY,_T("."))==0)
							pt.y = ptCur.y;
						else if((*m_phtPoints)[i].mask & TDrawPointMask_CY )
							pt.y = ptCur.y +  GetExtent((*m_phtPoints)[i].y,rc.bottom - rc.top + 1,true);
						else
							pt.y = rc.top + GetExtent((*m_phtPoints)[i].y,rc.bottom - rc.top + 1,true);

						::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
						::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x - 3,pt.y + 3);
					}
					hPen = (HPEN)SelectObject(hPaintDC,hPen);
					::DeleteObject(hPen);
					
				}
			}
			break;
		case _line:
			{
				int nSaveDC = ::SaveDC(hPaintDC);
				if(m_pCssStyle)BeginPath(hPaintDC);

				POINT	ptCur;
				POINT	pt;
				int nLineWidth = 1;
				COLORREF LineColor = RGB(0,0,0);
				DWORD	nColorStyle = PS_SOLID;

				//POINT* lpPoints = new POINT[m_pPoints->size()];

				if(m_pPoints->size() < 2) break;
				if(m_ptBorder.mask & TDrawPointMask_CX)
					nLineWidth = GetExtent(m_ptBorder.x,rc.right - rc.left + 1,true);
					
				HPEN hPen = ::CreatePen(nColorStyle,nLineWidth,LineColor);
				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				
				if(!((*m_pPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[0].pStrX,_T("."))==0 &&
					 (*m_pPoints)[0].mask & TDrawPointMask_STRY &&  ::_tcsicmp((*m_pPoints)[0].pStrY,_T("."))==0))
				{
					pt.x = rc.left;
					pt.y = rc.top;
					GetCurrentPositionEx(hPaintDC,&ptCur);
					if((*m_pPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[0].pStrX,_T("."))==0)
						pt.x = ptCur.x;
					else if((*m_pPoints)[0].mask & TDrawPointMask_CX )
						pt.x = ptCur.x +  GetExtent((*m_pPoints)[0].x,rc.right - rc.left + 1,true);
					else
						pt.x = rc.left + GetExtent((*m_pPoints)[0].x,rc.right - rc.left + 1,true);

					if((*m_pPoints)[0].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pPoints)[0].pStrY,_T("."))==0)
						pt.y = ptCur.y;
					else if((*m_pPoints)[0].mask & TDrawPointMask_CY )
						pt.y = ptCur.y +  GetExtent((*m_pPoints)[0].y,rc.bottom - rc.top + 1,true);
					else
						pt.y = rc.top + GetExtent((*m_pPoints)[0].y,rc.bottom - rc.top + 1,true);

					::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
					//lpPoints[0] = pt;
				}
				for(int i=1;i<(int)m_pPoints->size();i++)
				{
					pt.x = 0;
					pt.y = 0;
					GetCurrentPositionEx(hPaintDC,&ptCur);
					if((*m_pPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pPoints)[i].pStrX,_T("."))==0)
						pt.x = ptCur.x;
					else if((*m_pPoints)[i].mask & TDrawPointMask_CX )
						pt.x = ptCur.x +  GetExtent((*m_pPoints)[i].x,rc.right - rc.left + 1,true);
					else
						pt.x = rc.left + GetExtent((*m_pPoints)[i].x,rc.right - rc.left + 1,true);

					if((*m_pPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pPoints)[i].pStrY,_T("."))==0)
						pt.y = ptCur.y;
					else if((*m_pPoints)[i].mask & TDrawPointMask_CY )
						pt.y = ptCur.y +  GetExtent((*m_pPoints)[i].y,rc.bottom - rc.top + 1,true);
					else
						pt.y = rc.top + GetExtent((*m_pPoints)[i].y,rc.bottom - rc.top + 1,true);
					::LineTo(hPaintDC,pt.x,pt.y);
					//lpPoints[i] = pt;
				}

				//Polygon(hPaintDC,lpPoints,(int)m_pPoints->size() - 1);
				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);

				if(m_pCssStyle)
				{
					::EndPath(hPaintDC);
					HRGN hRgn = PathToRegion(hPaintDC);
					SelectClipRgn(hPaintDC,hRgn);
					::DeleteObject(hRgn);
	
					::InflateRect(&rc,2,2);
					XUtil::Css::DrawCssFrame(hPaintDC,&rc,pBoxStyle);
					::InflateRect(&rc,-2,-2);
				}

				::RestoreDC(hPaintDC,nSaveDC);

				if(dwDrawMask & DMASKSELECTED)
				{
					hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
					hPen = (HPEN)SelectObject(hPaintDC,hPen);

					for(int i=0;i<(int)m_pstPoints->size();i++)
					{
						pt.x = 0;
						pt.y = 0;
						GetCurrentPositionEx(hPaintDC,&ptCur);
						if((*m_pstPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_pstPoints)[i].pStrX,_T("."))==0)
							pt.x = ptCur.x;
						else if((*m_pstPoints)[i].mask & TDrawPointMask_CX )
							pt.x = ptCur.x +  GetExtent((*m_pstPoints)[i].x,rc.right - rc.left + 1,true);
						else
							pt.x = rc.left + GetExtent((*m_pstPoints)[i].x,rc.right - rc.left + 1,true);

						if((*m_pstPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_pstPoints)[i].pStrY,_T("."))==0)
							pt.y = ptCur.y;
						else if((*m_pstPoints)[i].mask & TDrawPointMask_CY )
							pt.y = ptCur.y +  GetExtent((*m_pstPoints)[i].y,rc.bottom - rc.top + 1,true);
						else
							pt.y = rc.top + GetExtent((*m_pstPoints)[i].y,rc.bottom - rc.top + 1,true);
						::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
					}
					hPen = (HPEN)SelectObject(hPaintDC,hPen);
					::DeleteObject(hPen);
				}

				if(dwDrawMask & DMASKHOT)
				{
					hPen = ::CreatePen(PS_DASH,1,RGB(0,0,255));
					hPen = (HPEN)SelectObject(hPaintDC,hPen);

					for(int i=0;i<(int)m_phtPoints->size();i++)
					{
						pt.x = 0;
						pt.y = 0;
						GetCurrentPositionEx(hPaintDC,&ptCur);
						if((*m_phtPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*m_phtPoints)[i].pStrX,_T("."))==0)
							pt.x = ptCur.x;
						else if((*m_phtPoints)[i].mask & TDrawPointMask_CX )
							pt.x = ptCur.x +  GetExtent((*m_phtPoints)[i].x,rc.right - rc.left + 1,true);
						else
							pt.x = rc.left + GetExtent((*m_phtPoints)[i].x,rc.right - rc.left + 1,true);

						if((*m_phtPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*m_phtPoints)[i].pStrY,_T("."))==0)
							pt.y = ptCur.y;
						else if((*m_phtPoints)[i].mask & TDrawPointMask_CY )
							pt.y = ptCur.y +  GetExtent((*m_phtPoints)[i].y,rc.bottom - rc.top + 1,true);
						else
							pt.y = rc.top + GetExtent((*m_phtPoints)[i].y,rc.bottom - rc.top + 1,true);

						::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
						::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x - 3,pt.y + 3);
					}
					hPen = (HPEN)SelectObject(hPaintDC,hPen);
					::DeleteObject(hPen);
				}
			}
			break;
		case _shape:
			{
				/*
				if(!pItem)break;
				tagShape *pShape;
				if(this->refname)
					pShape = FindShapeByName(pItem,this->refname);
				else if(this->refid)
					pShape = FindShapeByID(pItem,this->refid);
				if(pShape)
				{
					RECT rect;
					::SetRect(&rect,x0,y0,x1,y1);
					for(int j=0;j<pShape->m_DrawObjs.size();j++)
					{
						RECT rect;
						::SetRect(&rect,x0,y0,x1,y1);
						pShape->m_DrawObjs[j]->DoDraw(hPaintDC,rect,pStyleSvr,pItem);
					}
				}
				*/
			}
		}
		if(pBoxStyle) delete pBoxStyle;
		return 0;
	}

	int XShapeDrawObject::CalcPoints(POINT pointCur ,const RECT &rc, vector<TDrawPoint> * mPoints, vector<POINT> *  points)
	{
		POINT pt;
		POINT ptCur;
		ptCur = pointCur;

		if(mPoints->size() >0)
		{
			if(!((*mPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*mPoints)[0].pStrX,_T("."))==0 &&
				(*mPoints)[0].mask & TDrawPointMask_STRY &&  ::_tcsicmp((*mPoints)[0].pStrY,_T("."))==0))
			{
				pt.x = rc.left;
				pt.y = rc.top;
				if((*mPoints)[0].mask & TDrawPointMask_STRX && ::_tcsicmp((*mPoints)[0].pStrX,_T("."))==0)
					pt.x = ptCur.x;
				else if((*mPoints)[0].mask & TDrawPointMask_CX )
					pt.x = ptCur.x +  GetExtent((*mPoints)[0].x,rc.right - rc.left + 1,true);
				else
					pt.x = rc.left + GetExtent((*mPoints)[0].x,rc.right - rc.left + 1,true);

				if((*mPoints)[0].mask & TDrawPointMask_STRY && ::_tcsicmp((*mPoints)[0].pStrY,_T("."))==0)
					pt.y = ptCur.y;
				else if((*mPoints)[0].mask & TDrawPointMask_CY )
					pt.y = ptCur.y +  GetExtent((*mPoints)[0].y,rc.bottom - rc.top + 1,true);
				else
					pt.y = rc.top + GetExtent((*mPoints)[0].y,rc.bottom - rc.top + 1,true);
				points->push_back(pt);
			}
		}

		for(int i=1;i<(int)mPoints->size();i++)
		{
			pt.x = 0;
			pt.y = 0;
			if((*mPoints)[i].mask & TDrawPointMask_STRX && ::_tcsicmp((*mPoints)[i].pStrX,_T("."))==0)
				pt.x = ptCur.x;
			else if((*mPoints)[i].mask & TDrawPointMask_CX )
				pt.x = ptCur.x +  GetExtent((*mPoints)[i].x,rc.right - rc.left + 1,true);
			else
				pt.x = rc.left + GetExtent((*mPoints)[i].x,rc.right - rc.left + 1,true);

			if((*mPoints)[i].mask & TDrawPointMask_STRY && ::_tcsicmp((*mPoints)[i].pStrY,_T("."))==0)
				pt.y = ptCur.y;
			else if((*mPoints)[i].mask & TDrawPointMask_CY )
				pt.y = ptCur.y +  GetExtent((*mPoints)[i].y,rc.bottom - rc.top + 1,true);
			else
				pt.y = rc.top + GetExtent((*mPoints)[i].y,rc.bottom - rc.top + 1,true);
			points->push_back(pt);
		}
		return 1;
	}

	int XShapeDrawObject::GetHotedPoints(const RECT &rect, POINT ptCur, vector<POINT>* pPoints)
	{
		CalcPoints(ptCur,rect,m_phtPoints,pPoints);
		return 1;
	}
	int XShapeDrawObject::GetSelectPoints(const RECT &rect, POINT ptCur, vector<POINT>* pPoints)
	{
		CalcPoints(ptCur,rect,m_pstPoints,pPoints);
		return 1;
	}
	int XShapeDrawObject::GetHotedArrows( vector<int>* pArrows)
	{
		for(int i=0;i<(int)this->m_phtArrows->size();i++)
		{
			pArrows->push_back((*m_phtArrows)[i]);
		}
		return 1;
	}

}}