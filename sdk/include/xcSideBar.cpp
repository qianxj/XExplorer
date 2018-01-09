#include "StdAfx.h"
#include "xcSideBar.hpp"
#include "xframe.h"
namespace Hxsoft
{
	namespace XFrame
	{
		xcSideBar::xcSideBar(void):m_pxfNodeOne(NULL),m_pxfNodeTwo(NULL),SideState(_none),m_pxfNodeOneName(NULL),m_pxfNodeTwoName(NULL)
		{
			m_bTransparent = true;
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcSideBar::EvtMouseMove);
			this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcSideBar::EvtLButtonDown);
			this->AttachListon(WM_PAINT,(FNEvent)&xcSideBar::EvtPaint);
			this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcSideBar::EvtMouseLeave);
		}

		xcSideBar::~xcSideBar(void)
		{
		}

		int xcSideBar::PrepareSizingNode()
		{
			if(m_pxfNodeOneName)
				m_pxfNodeOne = ((IXNode*)m_pxfNode->GetInterface())->GetXPage()->m_pXPage->m_pxfNode->GetNodeByName(m_pxfNodeOneName);
			if(m_pxfNodeTwoName)
				m_pxfNodeTwo = ((IXNode*)m_pxfNode->GetInterface())->GetXPage()->m_pXPage->m_pxfNode->GetNodeByName(m_pxfNodeTwoName);
			if(m_pxfNodeOne)
				m_pxfNodeOneLayer = m_pxfNodeOne->m_LayerFlow;
			if(m_pxfNodeTwo)
				m_pxfNodeTwoLayer = m_pxfNodeTwo->m_LayerFlow;
			return 1;
		}

		bool xcSideBar::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
		{
			if(SideState == _none) return true;

			if(!m_pxfNodeOne || !m_pxfNodeTwo) PrepareSizingNode();
			if(!m_pxfNodeOne || !m_pxfNodeTwo) return true;

			if(SideState == _side1)
			{
				((IXNode*)m_pxfNodeOne->GetInterface())->SetLayerFlow(LayerFlow_UnLayer | LayerFlow_None);
				((IXNode*)m_pxfNodeTwo->GetInterface())->SetLayerFlow(m_pxfNodeTwoLayer);
			}
			else if(SideState == _center)
			{
				((IXNode*)m_pxfNodeOne->GetInterface())->SetLayerFlow(m_pxfNodeOneLayer);
				((IXNode*)m_pxfNodeTwo->GetInterface())->SetLayerFlow(m_pxfNodeTwoLayer);
			}
			else if(SideState == _side2)
			{
				((IXNode*)m_pxfNodeOne->GetInterface())->SetLayerFlow(m_pxfNodeOneLayer);
				((IXNode*)m_pxfNodeTwo->GetInterface())->SetLayerFlow(LayerFlow_UnLayer | LayerFlow_None);
			}
			SideState = _none;
			::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}

		bool xcSideBar:: EvtMouseMove(TEvent* pEvent,LPARAM lParam )
		{
			POINT point = pEvent->mouse.point;

			RECT rect;
			GetClientRect(&rect);

			int height = rect.bottom - rect.top;
			int width = rect.right - rect.left;
			SideState = _none;
			if(m_pxfNode->m_LayerFlow == LayerFlow_NS)
			{
				width /= 2;
				/*POINT pts[] = {
					{width / 2,0},
					{0,height / 3 - 1},
					{width, height / 3 - 1}
				};*/
				POINT pts[] = {
					{width, 0},
					{width - 6, 6},
					{width + 6, 6}
				};
				HRGN rgn = ::CreatePolygonRgn(pts, 3, ALTERNATE);
				if(::PtInRegion(rgn, point.x, point.y)) SideState = _side1;
				::DeleteObject(rgn);

				/*POINT pts1[] = {
					{0,height / 3 + 1},
					{width - 1, height / 3 + 1},
					{width - 1, height / 3 * 2 - 1},
					{0,height / 3 * 2 - 1}
				};*/
				POINT pts1[] = {
					{width - 7, 8},
					{width + 7, 8},
					{width + 7, 10},
					{width - 7, 10}
				};
				HRGN rgn1 = ::CreatePolygonRgn(pts1, 4, ALTERNATE);
				if(::PtInRegion(rgn1, point.x, point.y)) SideState = _center;
				::DeleteObject(rgn1);				

				/*POINT pts2[] = {
					{width / 2,height - 1},
					{0,height / 3 * 2 + 1},
					{width - 1, height / 3 * 2 + 1}
				};*/
				POINT pts2[] = {
					{width,18},
					{width - 6,12},
					{width + 6, 12}
				};
				HRGN rgn2 = ::CreatePolygonRgn(pts2, 3, ALTERNATE);
				if(::PtInRegion(rgn2, point.x, point.y)) SideState = _side2;
				::DeleteObject(rgn2);
			}
			else
			{
				height /= 2;
				/*POINT pts[] = {
					{0, height / 2},
					{width / 3 - 1, 0},
					{width / 3 - 1, height - 1}
				};*/
				POINT pts[] = {
					{0, height},
					{6, height - 6},
					{6, height + 6}
				};
				HRGN rgn = ::CreatePolygonRgn(pts, 3, ALTERNATE);
				if(::PtInRegion(rgn, point.x, point.y)) SideState = _side1;
				::DeleteObject(rgn);

				/*POINT pts1[] = {
					{width / 3 + 1,0},
					{width / 3 * 2 - 1, 0},
					{width / 3 * 2 - 1, height - 1},
					{width / 3 + 1, height - 1}
				};*/
				POINT pts1[] = {
					{8,height - 7},
					{8, height + 7},
					{10, height + 7},
					{10, height - 7}
				};
				HRGN rgn1 = ::CreatePolygonRgn(pts1, 4, ALTERNATE);
				if(::PtInRegion(rgn1, point.x, point.y)) SideState = _center;
				::DeleteObject(rgn1);				

				/*POINT pts2[] = {
					{width / 3 * 2 + 1,0},
					{width - 1, height / 2},
					{width / 3 * 2 + 1, height - 1}
				};*/
				POINT pts2[] = {
					{18, height},
					{12, height - 6},
					{12, height + 6}
				};
				HRGN rgn2 = ::CreatePolygonRgn(pts2, 3, ALTERNATE);
				if(::PtInRegion(rgn2, point.x, point.y)) SideState = _side2;
				::DeleteObject(rgn2);
			}
			::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}

		int xcSideBar::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rect;
			if(pDrawRect) 
				rect = *pDrawRect;
			else
				GetClientRect(&rect);
			LPCTSTR pCssStr = this->m_pxfNode->GetCssStyle();
			if(pCssStr)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
				if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
				{
					RECT rc;
					this->GetClientRect(&rc);
					XUtil::Css::DrawCssFrame(hPaintDC,&rect,pBoxStyle);
					delete pBoxStyle;
				}else
				{
					::SetBkMode(hPaintDC,TRANSPARENT);
				}
			}
			else
			{
				::SetBkMode(hPaintDC,TRANSPARENT);
			}

			HBRUSH hbrush = ::CreateSolidBrush(RGB(0,0,0));
			hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);
			
			int height = rect.bottom - rect.top;
			int width = rect.right - rect.left;
			//height -= height % 3;
			//width -= width % 2 ;
			HBRUSH hbrush1 = NULL;
			if(m_pxfNode->m_LayerFlow == LayerFlow_NS)
			{
				width /= 2;
				if(SideState == _side1)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts[] = {
					{width / 2,0},
					{0,height / 3 - 1},
					{width, height / 3 - 1}
				};*/
				POINT pts[] = {
					{width, 0},
					{width - 6, 6},
					{width + 6, 6}
				};
				::Polygon(hPaintDC,pts,3);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}

				if(SideState == _center)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				else
				{
					hbrush1 = ::CreateSolidBrush(RGB(192,192,255));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts1[] = {
					{0,height / 3 + 1},
					{width - 1, height / 3 + 1},
					{width - 1, height / 3 * 2 - 1},
					{0,height / 3 * 2 - 1}
				};*/
				POINT pts1[] = {
					{width - 7, 8},
					{width + 7, 8},
					{width + 7, 10},
					{width - 7, 10}
				};
				::Polygon(hPaintDC,pts1,4);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}

				if(SideState == _side2)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts2[] = {
					{width / 2,height - 1},
					{0,height / 3 * 2 + 1},
					{width - 1, height / 3 * 2 + 1}
				};*/
				POINT pts2[] = {
					{width,18},
					{width - 6,12},
					{width + 6, 12}
				};
				::Polygon(hPaintDC,pts2,3);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}
			}
			else
			{
				height /= 2;
				if(SideState == _side1)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts[] = {
					{0, height / 2},
					{width / 3 - 1, 0},
					{width / 3 - 1, height - 1}
				};*/
				POINT pts[] = {
					{0, height},
					{6, height - 6},
					{6, height + 6}
				};
				::Polygon(hPaintDC,pts,3);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}

				if(SideState == _center)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				else
				{
					hbrush1 = ::CreateSolidBrush(RGB(192,192,255));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts1[] = {
					{width / 3 + 1,0},
					{width / 3 * 2 - 1, 0},
					{width / 3 * 2 - 1, height - 1},
					{width / 3 + 1, height - 1}
				};*/
				POINT pts1[] = {
					{8,height - 7},
					{8, height + 7},
					{10, height + 7},
					{10, height - 7}
				};
				::Polygon(hPaintDC,pts1,4);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}

				if(SideState == _side2)
				{
					hbrush1 = ::CreateSolidBrush(RGB(255,255,0));
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
				}
				/*POINT pts2[] = {
					{width / 3 * 2 + 1,0},
					{width - 1, height / 2},
					{width / 3 * 2 + 1, height - 1}
				};*/
				POINT pts2[] = {
					{18, height},
					{12, height - 6},
					{12, height + 6}
				};
				::Polygon(hPaintDC,pts2,3);
				if(hbrush1)
				{
					hbrush1 = (HBRUSH)::SelectObject(hPaintDC,hbrush1);
					::DeleteObject(hbrush1);
					hbrush1 = NULL;
				}
			}

			hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);
			::DeleteObject(hbrush);
			return 1;
		}

		bool xcSideBar::EvtPaint(TEvent* pEvent,LPARAM lParam)
		{
			HDC hdc = pEvent->paint.hdc;
			RECT rect;
			this->GetClientRect(&rect);

			HBITMAP hOldBmp = NULL;
			HBITMAP hBmp = NULL;
			POINT pt;

			xfNodeControl* pNode = (xfNodeControl *)m_pxfNode;
			HDC hMemDC = ::CreateCompatibleDC(hdc);

			hBmp = ::CreateCompatibleBitmap(hdc,rect.right - rect.left + 1,rect.bottom - rect.top +1);
			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			if(pNode->m_hBackGround)
			{
				HDC hDC = ::CreateCompatibleDC(hdc);
				HBITMAP hBmp = (HBITMAP)pNode->m_hBackGround;
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::BitBlt(hMemDC,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hDC,rect.left,rect.top,SRCCOPY);
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::DeleteDC(hDC);
			}

			::SetViewportOrgEx(hMemDC,- pNode->m_ContentRect.left,- pNode->m_ContentRect.top,&pt);
			((xfNodeControl *)m_pxfNode)->DrawNodeEx(hMemDC);
			::SetViewportOrgEx(hMemDC,pt.x,pt.y,NULL);

			this->DoDraw(hMemDC);
			::BitBlt(hdc,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hMemDC,rect.left,rect.top,SRCCOPY);

			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hMemDC);

			return true;
		}

		bool xcSideBar::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
		{
			SideState = _none;
			::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}

		void xcSideBar::Initial()
		{	
			if(m_pxfNode && m_pxfNode->m_pElement)
			{
				_variant_t var;
				m_pxfNode->m_pElement->getAttribute(L"side1",&var);
				if(var.bstrVal)
				{
					if(::_tcsicmp(var.bstrVal,_T(""))!=0)
						m_pxfNodeOneName = ::_tcsdup(var.bstrVal);
					::VariantClear(&var);
				}
				m_pxfNode->m_pElement->getAttribute(L"side2",&var);
				if(var.bstrVal)
				{
					if(::_tcsicmp(var.bstrVal,_T(""))!=0)
						m_pxfNodeTwoName = ::_tcsdup(var.bstrVal);
					::VariantClear(&var);
				}
			}
		}

	}
}



