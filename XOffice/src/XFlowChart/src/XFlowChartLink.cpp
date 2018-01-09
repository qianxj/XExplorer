#include "stdafx.h"
#include "XFlowchartobject.hpp"
#include "xflowchartdatasvr.hpp"
#include "XFlowChartContentSvr.hpp"
#include "XFlowChartLink.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowChartLink::XFlowChartLink():m_pStartObject(NULL),m_pEndObject(NULL),m_nStartIndex(-1),m_nEndIndex(-1),
		m_FState(XFONORMAL),m_FLineStyle(FXOLINENORMAL), m_pPloys(new vector<POINT>),m_phtPoints(new vector<POINT>),
		m_pstPoints(new vector<POINT>),m_pGuid(NULL)
	{

	}

	XFlowChartLink::~XFlowChartLink()
	{
		if(m_pPloys)	delete m_pPloys;
		if(m_phtPoints) delete m_phtPoints;
		if(m_pstPoints) delete m_pstPoints;
		if(m_pGuid)	delete m_pGuid;
	}

	void XFlowChartLink::DoDraw(HDC hPaintDC,RECT rc)
	{
		if(m_pPloys->size() < 1) return;
		::MoveToEx(hPaintDC,(*m_pPloys)[0].x + rc.left ,(*m_pPloys)[0].y + rc.top ,NULL);
		HPEN hPen ;
		if(this->m_FState & XFOCREATING)
			hPen = ::CreatePen(PS_DOT,1,RGB(0,0,0));
		else
			hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		for(int i=1;i<(int)m_pPloys->size();i++)
		{
			::LineTo(hPaintDC,(*m_pPloys)[i].x + rc.left ,(*m_pPloys)[i].y + rc.top);
		}
		if(this->m_FState & XFOSELECTED)
		{
			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,255));
			HBRUSH hbrush = ::CreateSolidBrush(RGB(255,0,0));
			hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);

			if(this->m_nStartIndex>=0 && this->m_pStartObject)
				m_pStartObject->DrawHoted(hPaintDC,m_nStartIndex);
			if(this->m_nEndIndex>=0 && this->m_pEndObject)
				m_pEndObject->DrawHoted(hPaintDC,m_nEndIndex);

			hPen = (HPEN)SelectObject(hPaintDC,hPen);
			for(int i=0;i<(int)m_pPloys->size();i++)
			{
				POINT pt = (*m_pPloys)[i];
				pt.x += rc.left;
				pt.y += rc.top;

				if(!(i==0 || i== m_pPloys->size() -1))
				{
					::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
					::LineTo(hPaintDC, pt.x - 2,pt.y + 3);
					::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
					::LineTo(hPaintDC, pt.x + 3,pt.y - 2);
					::LineTo(hPaintDC, pt.x - 2,pt.y - 2);
				}

				if(i>0)
				{
					pt.x = ((*m_pPloys)[i - 1].x + (*m_pPloys)[i].x)/2;
					pt.y = ((*m_pPloys)[i - 1].y + (*m_pPloys)[i].y)/2;

					pt.x += rc.left;
					pt.y += rc.top;

					if(abs(pt.x - (*m_pPloys)[i - 1].x)>5 || abs(pt.y - (*m_pPloys)[i - 1].y)>5)
					{
						::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x - 2,pt.y + 3);
						::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
						::LineTo(hPaintDC, pt.x + 3,pt.y - 2);
						::LineTo(hPaintDC, pt.x - 2,pt.y - 2);

						::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
						::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
						::LineTo(hPaintDC, pt.x - 3,pt.y + 3);
					}
				}

			}
			hPen = (HPEN)SelectObject(hPaintDC,hPen);
			::DeleteObject(hPen);
			hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);
			::DeleteObject(hbrush);
		
		}
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		if(m_pPloys->size()>1)
		{
			hPen = ::CreatePen(PS_SOLID,2,RGB(0,0,0));
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);

			POINT pt1,pt2;
			pt1 = (*m_pPloys)[0];
			pt2 = (*m_pPloys)[1];
			pt1.x += rc.left;
			pt1.y += rc.top;
			pt2.x += rc.left;
			pt2.y += rc.top;

			DoDrawArrow(hPaintDC,pt1,pt2,true);

			pt1 = (*m_pPloys)[m_pPloys->size() - 2];
			pt2 = (*m_pPloys)[m_pPloys->size() - 1];
			pt1.x += rc.left;
			pt1.y += rc.top;
			pt2.x += rc.left;
			pt2.y += rc.top;

			DoDrawArrow(hPaintDC,pt1,pt2,false);
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);
			::DeleteObject(hPen);
		}
		::DeleteObject(hPen);
	}
	void XFlowChartLink::DoDrawArrow(HDC hPaintDC,POINT ptStart,POINT ptEnd,bool bStart)
	{
		int lx = 3;
		if(ptStart.x == ptEnd.x && ptStart.y < ptEnd.y) 
		{
			if(bStart ) //arrow should up
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,&pt);
				::LineTo(hPaintDC,ptStart.x - lx, ptStart.y + lx);
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,NULL);
				::LineTo(hPaintDC,ptStart.x + lx, ptStart.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}else //arrow should down
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,&pt);
				::LineTo(hPaintDC,ptEnd.x - lx, ptEnd.y - lx);
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,NULL);
				::LineTo(hPaintDC,ptEnd.x + lx, ptEnd.y - lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}
		}
		if(ptStart.x == ptEnd.x && ptStart.y > ptEnd.y) 
		{
			if(bStart ) //arrow should down
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,&pt);
				::LineTo(hPaintDC,ptStart.x - lx, ptStart.y - lx);
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,NULL);
				::LineTo(hPaintDC,ptStart.x + lx, ptStart.y - lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}else //arrow should down
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,&pt);
				::LineTo(hPaintDC,ptEnd.x - lx, ptEnd.y + lx);
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,NULL);
				::LineTo(hPaintDC,ptEnd.x + lx, ptEnd.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}
		}
		if(ptStart.y == ptEnd.y && ptStart.x < ptEnd.x) 
		{
			if(bStart ) //arrow should left
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,&pt);
				::LineTo(hPaintDC,ptStart.x + lx, ptStart.y - lx);
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,NULL);
				::LineTo(hPaintDC,ptStart.x + lx, ptStart.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}else //arrow should right
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,&pt);
				::LineTo(hPaintDC,ptEnd.x - lx, ptEnd.y - lx);
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,NULL);
				::LineTo(hPaintDC,ptEnd.x - lx, ptEnd.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}
		}
		if(ptStart.y == ptEnd.y && ptStart.x > ptEnd.x) 
		{
			if(bStart ) //arrow should right
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,&pt);
				::LineTo(hPaintDC,ptStart.x - lx, ptStart.y - lx);
				::MoveToEx(hPaintDC,ptStart.x,ptStart.y,NULL);
				::LineTo(hPaintDC,ptStart.x - lx, ptStart.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}else //arrow should left
			{
				POINT pt;
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,&pt);
				::LineTo(hPaintDC,ptEnd.x + lx, ptEnd.y - lx);
				::MoveToEx(hPaintDC,ptEnd.x,ptEnd.y,NULL);
				::LineTo(hPaintDC,ptEnd.x + lx, ptEnd.y + lx);
				::MoveToEx(hPaintDC,pt.x,pt.y,NULL);
			}
		}
	}

	int XFlowChartLink::AutoCalcPoints(const vector<RECT>* rects)
	{
		m_pPloys->clear();
		int paStart =  THOTARROWANY;
		int paEnd = THOTARROWANY;
		if(this->m_nStartIndex>=0)
		{
			paStart = (*m_pStartObject->m_phtArrows)[m_nStartIndex];
			m_ptStart = (*m_pStartObject->m_phtPoints)[m_nStartIndex];
		}
		if(this->m_nEndIndex>=0)
		{
			paEnd = (*m_pEndObject->m_phtArrows)[m_nEndIndex];
			m_ptEnd = (*m_pEndObject->m_phtPoints)[m_nEndIndex];
		}
		return AutoCalcPoints(m_ptStart,paStart,m_ptEnd,paEnd, m_FLineStyle,rects,m_pPloys);
	}

	int XFlowChartLink::AutoCalcPoints(POINT ptStart,int paStart,POINT ptEnd,int paEnd,FXOLineStyle dwLineStyle,const vector<RECT>* rects,vector<POINT>* pts)
	{
		POINT pt;
		pts->push_back(ptStart);
		if(dwLineStyle & FXOLINEDIRECT)
		{
			pts->push_back(ptEnd);
			return 1;
		}
		if(dwLineStyle == FXOLINENORMAL)
		{
			if(ptEnd.x == ptStart.x && ptEnd.y == ptStart.y)
			{
				pts->push_back(ptEnd);
			}else
			{
				if(paStart & THOTARROWNORTH && paEnd & THOTARROWNORTH)
				{
					pt.x = ptStart.x;
					pt.y = (ptStart.y > ptEnd.y ? ptEnd.y : ptStart.y) - 15;
					pts->push_back(pt);

					pt.x = ptEnd.x;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWWEST && paEnd & THOTARROWWEST)
				{
					pt.y = ptStart.y;
					pt.x = (ptStart.x > ptEnd.x ? ptEnd.x : ptStart.x) - 15;
					pts->push_back(pt);

					pt.y = ptEnd.y;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWSOUTH && paEnd & THOTARROWSOUTH)
				{
					pt.x = ptStart.x;
					pt.y = (ptStart.y < ptEnd.y ? ptEnd.y : ptStart.y) + 15;
					pts->push_back(pt);

					pt.x = ptEnd.x;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWEAST && paEnd & THOTARROWEAST)
				{
					pt.y = ptStart.y;
					pt.x = (ptStart.x < ptEnd.x ? ptEnd.x : ptStart.x) + 15;
					pts->push_back(pt);

					pt.y = ptEnd.y;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWSOUTH && paEnd & THOTARROWNORTH)
				{
					pt.x = ptStart.x;
					pt.y = ptStart.y + 15;
					pts->push_back(pt);

					pt.x = (pt.x + ptEnd.x)/2;
					pts->push_back(pt);

					pt.y = ptEnd.y - 15;
					pts->push_back(pt);

					pt.x = ptEnd.x;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWEAST && paEnd & THOTARROWWEST)
				{
					pt.y = ptStart.y;
					pt.x = ptStart.x + 15;
					pts->push_back(pt);

					pt.y = (pt.y + ptEnd.y)/2;
					pts->push_back(pt);

					pt.x = ptEnd.x - 15;
					pts->push_back(pt);

					pt.y = ptEnd.y;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}


				if(paStart & THOTARROWNORTH && paEnd & THOTARROWSOUTH)
				{
					pt.x = ptStart.x;
					pt.y = ptStart.y - 15;
					pts->push_back(pt);

					pt.x = (pt.x + ptEnd.x)/2;
					pts->push_back(pt);

					pt.y = ptEnd.y + 15;
					pts->push_back(pt);

					pt.x = ptEnd.x;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(paStart & THOTARROWWEST && paEnd & THOTARROWEAST)
				{
					pt.y = ptStart.y;
					pt.x = ptStart.x - 15;
					pts->push_back(pt);

					pt.y = (pt.y + ptEnd.y)/2;
					pts->push_back(pt);

					pt.x = ptEnd.x + 15;
					pts->push_back(pt);

					pt.y = ptEnd.y;
					pts->push_back(pt);
					pts->push_back(ptEnd);
					return 1;
				}

				if(ptEnd.x >= ptStart.x && ptEnd.y <= ptStart.y) //arrow east north
				{
					/*
					if(ptEnd.y - ptStart.y > 10 && ptEnd.x - ptStart.x > 10)
					{
						pt.x = ptStart.x;
						pt.y = ptStart.y + 10;
						pts->push_back(pt);
						pt.x = ptEnd.x - 10;
						pts->push_back(pt);
						pt.y = ptEnd.y - 10;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}else
					{
						pt.x = ptStart.x;
						pt.y = ptEnd.y;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}*/
				}else if(ptEnd.x <= ptStart.x && ptEnd.y <= ptStart.y)
				{
					if(ptStart.y - ptEnd.y > 10 && ptStart.x - ptEnd.x > 10)
					{
						pt.x = ptStart.x;
						pt.y = ptStart.y - 10;
						pts->push_back(pt);
						pt.x = ptEnd.x + 10;
						pts->push_back(pt);
						pt.y = ptEnd.y + 10;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}else
					{
						pt.x = ptStart.x;
						pt.y = ptEnd.y;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}
				}else if (ptEnd.x <= ptStart.x && ptEnd.y >= ptStart.y)
				{
					if(ptStart.x - ptEnd.x > 10 && ptEnd.y - ptStart.y   > 10)
					{
						pt.x = ptStart.x;
						pt.y = ptStart.y + 10;
						pts->push_back(pt);
						pt.x = ptEnd.x + 10;
						pts->push_back(pt);
						pt.y = ptEnd.y - 10;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}else
					{
						pt.x = ptStart.x;
						pt.y = ptEnd.y;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}
				}else if (ptEnd.x >= ptStart.x && ptEnd.y <= ptStart.y)
				{
					if( ptStart.y - ptEnd.y > 10 && ptEnd.x - ptStart.x   > 10)
					{
						pt.x = ptStart.x;
						pt.y = ptStart.y - 10;
						pts->push_back(pt);
						pt.x = ptEnd.x + 10;
						pts->push_back(pt);
						pt.y = ptEnd.y + 10;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}else
					{
						pt.x = ptStart.x;
						pt.y = ptEnd.y;
						pts->push_back(pt);
						pts->push_back(ptEnd);
					}
				}
			}
		}
		return 1;
	}

	int XFlowChartLink::LoadObject(vector<XFlowChartObject *> &DrawObjects,IXMLDOMElement * pElement)
	{
		_variant_t val;
		int nIndex;
		pElement->getAttribute(L"startshape",&val);
		if(val.bstrVal)
		{
			nIndex = _ttol(val.bstrVal + 1);
			if(nIndex < (int)DrawObjects.size())
				m_pStartObject = DrawObjects[nIndex];
		}
		pElement->getAttribute(L"startshapepos",&val);
		if(val.bstrVal)m_nStartIndex = val;

		pElement->getAttribute(L"endshape",&val);
		if(val.bstrVal)
		{
			nIndex = _ttol(val.bstrVal + 1);
			if(nIndex < (int)DrawObjects.size())
				m_pEndObject = DrawObjects[nIndex];
		}
		pElement->getAttribute(L"endshapepos",&val);
		if(val.bstrVal)m_nEndIndex = val;

		LPTSTR pStrX;
		LPTSTR pStrY;
		vector<LPTSTR> vX;
		vector<LPTSTR> vY;
		for(int t=0;t<3;t++) 
		{
			vX.clear();
			vY.clear();
			if(t==0)
			{
				pStrX = xbXml::GetStringAttribe(pElement,L"x");
				pStrY = xbXml::GetStringAttribe(pElement,L"y");
			}else if(t==1)
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

			for(int i=0;i<nCount;i++)
			{
				POINT pt;
				pt.x = _ttol(vX[i]);
				pt.y = _ttol(vY[i]);
				if(t==0)
					m_pPloys->push_back(pt);
				else if(t==1)
					m_phtPoints->push_back(pt);
				else
					m_pstPoints->push_back(pt);
			}
			if(pStrX) delete pStrX ;
			if(pStrY) delete pStrY ;
		}
		return 1;
	}
	int XFlowChartLink::DrawHoted(HDC hPaintDC,int nIndex)
	{
		HPEN hPen = ::CreatePen(PS_DASH,1,RGB(0,0,0));
		HBRUSH hbrush = ::CreateSolidBrush(RGB(255,0,0));
		hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);

		POINT pt = (*m_pPloys)[nIndex];//(*m_phtPoints)[nIndex];
		::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 4,pt.y + 4);

		hPen = (HPEN)SelectObject(hPaintDC,hPen);
		::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
		::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
		::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
		::LineTo(hPaintDC, pt.x - 3,pt.y + 3);

		hPen = (HPEN)SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);
		::DeleteObject(hbrush);
		return 1;
	}

	bool XFlowChartLink::IsHoted(int x,int y)
	{
		bool bHoted = false;
		HDC hDC = GetDC(NULL);
		HPEN hPen = ::CreatePen(PS_SOLID,6,RGB(0,0,0));;

		::BeginPath(hDC);
		::MoveToEx(hDC,(*m_pPloys)[0].x,(*m_pPloys)[0].y,NULL);
		hPen = (HPEN)::SelectObject(hDC,hPen);
		for(int i=1;i<(int)m_pPloys->size();i++)
		{
			::LineTo(hDC,(*m_pPloys)[i].x,(*m_pPloys)[i].y);
		}
		::EndPath(hDC);
		::WidenPath(hDC);
		HRGN hRgn = PathToRegion(hDC);
		bHoted = ::PtInRegion(hRgn,x,y)?true:false;
		if(hRgn)::DeleteObject(hRgn);
		hPen = (HPEN)::SelectObject(hDC,hPen);
		if(hPen)::DeleteObject(hPen);
		::ReleaseDC(NULL,hDC);

		return bHoted;
	}

	XFlowChartObject * XFlowChartLink::GetStartObject()
	{
		return this->m_pStartObject;
	}
	XFlowChartObject * XFlowChartLink::GetEndObject()
	{
		return this->m_pEndObject;
	}

}}}}
