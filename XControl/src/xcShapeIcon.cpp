#include "StdAfx.h"
#include "..\include\xcShapeIcon.hpp"

namespace Hxsoft{namespace XFrame
{
	xcShapeIcon::xcShapeIcon(void):m_pShapes(NULL),m_nIndex(-1),m_pStrShapeUri(NULL)
	{
		m_bTransparent = true;
		this->AttachListon(WM_PAINT,(FNEvent)&xcShapeIcon::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcShapeIcon::EvtLButtonDown);
	}

	xcShapeIcon::~xcShapeIcon(void)
	{
		this->ClearAllListonsTo();
	}
	
	void xcShapeIcon::Initial() 
	{
		VARIANT	svalex;
		m_pxfNode->m_pElement->getAttribute(L"set",&svalex);
		if(svalex.vt==VT_BSTR && svalex.bstrVal)
		{
			m_pStrShapeUri =_tcsdup(svalex.bstrVal);
			m_pShapes = xfApp::GetApp()->m_pShapeMgr->GetShapeSet(svalex.bstrVal);
		}
		::VariantClear(&svalex);
	}
	
	int xcShapeIcon::DoDraw(HDC hPaintDC,RECT * rc)
	{
		if(!m_pShapes) return NULL;
		int startx = 4,starty = 4;
		int sx = 16;
		int sy = 16;
		int ty = 22;
		int mx = 2;
		int my = 2;

		RECT rt;
		int cx = 48;
		int cy = 48;

		startx = sx;
		starty = sy;

		int nIndex = m_nIndex;

		LOGFONT LogFont;
		XUtil::Css::GetDefaultFont(LogFont);
		HFONT hFont = ::CreateFontIndirect(&LogFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		int hbr = SetBkMode(hPaintDC,TRANSPARENT);
		for(int i=0;i< (int)m_pShapes->m_ShapeObjectSetItems->size();i++)
		{
			for(int j=0;j< (int)(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects->size();j++)
			{
				if(startx + cx + sx >= rc->right)
				{
					starty += cy + sy;
					startx = sx;
				}
				::SetRect(&rt,startx + mx,starty + my,startx + cx - mx ,starty + cy - ty - my);
				(*(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j]->DoDraw(hPaintDC,rt,NULL);

				LPTSTR pStr;
				pStr = (*(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j]->m_pShapeLabel;
				if(!pStr) pStr = (*(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j]->m_pShapeName;

				::SetRect(&rt,startx ,starty + my +cy - ty + 2,startx + cx  ,starty + cy - my);
				HBRUSH hBrush;
				
				if(pStr)
				{
					COLORREF color;
					if(nIndex==0)
					{
						hBrush = CreateSolidBrush(RGB(0,0,128));
						FillRect(hPaintDC,&rt,hBrush);
						::DeleteObject(hBrush);
						color = ::SetTextColor(hPaintDC,RGB(255,255,255));


					}
					::DrawText(hPaintDC,pStr,(int)_tcslen(pStr),&rt,DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
					if(nIndex==0)::SetTextColor(hPaintDC,color);
				}
				nIndex--;
				startx += cx + sx;
			}
		}
		SetBkMode(hPaintDC,hbr);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);
		return 1;
	}

	bool xcShapeIcon::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		RECT	rc;

		HDC hPaintDC = pEvent->paint.hdc;
		GetClientRect(&rc);

		DoDraw(hPaintDC,&rc);

		return true;
	}

	int xcShapeIcon::GetItemRect(int nIndex,RECT &rect)
	{
		::SetRect(&rect,0,0,0,0);
		if(!m_pShapes) return NULL;
		int startx = 4,starty = 4;
		int sx = 16;
		int sy = 16;
		int ty = 22;
		int mx = 2;
		int my = 2;

		int cx = 48;
		int cy = 48;

		startx = sx;
		starty = sy;

		RECT	rc;
		GetClientRect(&rc);
		for(int i=0;i< (int)m_pShapes->m_ShapeObjectSetItems->size();i++)
		{
			for(int j=0;j< (int)(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects->size();j++)
			{
				if(startx + cx + sx >= rc.right)
				{
					starty += cy + sy;
					startx = sx;
				}
				if(nIndex==0)
				{
					::SetRect(&rect,startx ,starty + my,startx + cx  ,starty + cy  - my);
					return 1;
				}
				
				nIndex--;
				startx += cx + sx;
			}
		}
		return -1;
	}

	bool xcShapeIcon::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		if(!m_pShapes) return true;
		int startx = 4,starty = 4;
		int sx = 16;
		int sy = 16;
		int ty = 22;
		int mx = 2;

		int my = 2;

		RECT rt;
		int cx = 48;
		int cy = 48;

		startx = sx;
		starty = sy;

		RECT	rc;
		GetClientRect(&rc);

		int nIndex = -1;
		for(int i=0;i< (int)m_pShapes->m_ShapeObjectSetItems->size();i++)
		{
			for(int j=0;j< (int)(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects->size();j++)
			{
				if(startx + cx + sx >= rc.right)
				{
					starty += cy + sy;
					startx = sx;
				}
				::SetRect(&rt,startx,starty + my,startx + cx  ,starty + cy  - my);
				
				if(::PtInRect(&rt,pEvent->mouse.point))
				{
					if(m_nIndex>=0)
					{
						RECT rect;
						GetItemRect(m_nIndex,rect);
						rect.top += cy - ty - my;
						::InvalidateRect(m_hWnd,&rect,true);
					}
					m_nIndex = nIndex + 1;
					::InvalidateRect(m_hWnd,&rt,true);

					TCHAR* buf = new TCHAR[255];
					::_stprintf_s(buf,255,_T("%s:%s"),m_pStrShapeUri,(*(*m_pShapes->m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j]->m_pShapeName);

					SendMessage(GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)buf,(LPARAM)this); 
					
					return true;
				}else
					nIndex++;
				startx += cx + sx;
			}
		}
		return true;
	}
}}
