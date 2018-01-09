#include "StdAfx.h"
#include "..\include\XWordVRuler.hpp"
#include "xwordlayer.hpp"
#include "xwordsheetsvr.hpp"
#include "xflowdocument.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	int XWordVRuler::m_nWidth = 24;
	XWordVRuler::XWordVRuler(void):m_pLayer(NULL),m_pSheetSvr(NULL),m_pFlowDocument(NULL)
	{
	}

	XWordVRuler::~XWordVRuler(void)
	{
	}
	
	int XWordVRuler::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		LPTSTR pStyle = ::_tcsdup(_T("background-color:#d8e7fc none #d8e7fc none"));
		XUtil::Css::TBoxStyle * pBoxStyle = XUtil::xuCCssStyle::ParseCssStyle(pStyle );
		delete pStyle;

		RECT rc ;
		if(pDrawRect)
			rc = *pDrawRect;
		else
			rc = FRect;
		XUtil::Css::DrawCssFrame(hPaintDC,&rc,pBoxStyle);

		delete pBoxStyle;

		int nSave = ::SaveDC(hPaintDC);
		IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);
		if(m_pLayer)
			OffsetViewportOrgEx(hPaintDC,0, - m_pLayer->m_ptScroll.y,NULL);
		else
			OffsetViewportOrgEx(hPaintDC,0, - m_pFlowDocument->m_ptScroll.y,NULL);

		LOGFONT	logFont;
		XUtil::Css::GetFont(NULL,logFont);
		
		logFont.lfHeight = -14; //10.5
		HFONT hFont = ::CreateFontIndirect(&logFont);

		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		
		SIZE sz;
		GetTextExtentPoint32(hPaintDC,_T("ÖÐ¹ú"),2,&sz);
		
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);
		
		RECT rect,lrect;
		if(m_pLayer)
		{
			for(int n=1;n<=this->m_pLayer->m_nPage;n++)
			{
				m_pLayer->CalcContentPageRect(n - 1,&m_pLayer->FRect,rect);
				HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,255));
				::SetRect(&lrect,FRect.left + 4,rect.top,FRect.right - 6,rect.bottom);
				::FillRect(hPaintDC,&lrect,hBrush);
				::DeleteObject(hBrush);

				hBrush = ::CreateSolidBrush(RGB(158,190, 245));
				::SetRect(&lrect,FRect.left + 4,rect.top - m_pLayer->m_rcPageMargin.top,FRect.right - 6,rect.top);
				::FillRect(hPaintDC,&lrect,hBrush);

				::SetRect(&lrect,FRect.left + 4,rect.bottom ,FRect.right - 6,rect.bottom +  m_pLayer->m_rcPageMargin.bottom);
				::FillRect(hPaintDC,&lrect,hBrush);
				::DeleteObject(hBrush);

				int y = rect.top - (int)(sz.cy*1.5/2.0) ;
				HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				logFont.lfHeight = -11;
				//logFont.lfEscapement = 900;
				hFont = ::CreateFontIndirect(&logFont);
				hFont = (HFONT)::SelectObject(hPaintDC,hFont);

				TCHAR buf[30];
				int nIndex = -1;
				RECT sRect;
				int hbr = ::SetBkMode(hPaintDC,TRANSPARENT);
				while(y <= rect.bottom + this->m_pLayer->m_rcPageMargin.bottom)
				{
					nIndex += 1;
					if(nIndex >=2 && nIndex % 2 ==0)
					{
						::_stprintf_s(buf,30,_T("%i"),nIndex);
						::SetRect(&sRect,FRect.left + 4 ,y,FRect.right -5,y + (int)(sz.cy * 1.5));
						if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
					::MoveToEx(hPaintDC,FRect.left + 6,y,NULL);
					::LineTo(hPaintDC,FRect.right - 8,y);
					y += (int)(sz.cy * 1.5);
				}
				nIndex = 0;
				y = rect.top - 2*sz.cy;
				while(y >= rect.top - this->m_pLayer->m_rcPageMargin.top)
				{
					nIndex += 1;
					if(nIndex >=2 && nIndex % 2 ==0)
					{
						::_stprintf_s(buf,30,_T("%i"),nIndex);
						::SetRect(&sRect,FRect.left + 4,y ,FRect.right - 5,y + (int)(sz.cy * 1.5));
						if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
					::MoveToEx(hPaintDC,FRect.left + 6,y,NULL);
					::LineTo(hPaintDC,FRect.right - 8,y);
					y -= (int)(sz.cy * 1.5);
				}

				::SetBkMode(hPaintDC,hbr);
				hFont = (HFONT)::SelectObject(hPaintDC,hFont);
				::DeleteObject(hFont);

				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
		}else
		{
			for(int n=1;n<=this->m_pFlowDocument->m_nPage;n++)
			{
				m_pFlowDocument->CalcContentPageRect(n - 1,&m_pFlowDocument->FRect,rect);
				HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,255));
				::SetRect(&lrect,FRect.left + 4,rect.top,FRect.right - 6,rect.bottom);
				::FillRect(hPaintDC,&lrect,hBrush);
				::DeleteObject(hBrush);

				hBrush = ::CreateSolidBrush(RGB(158,190, 245));
				::SetRect(&lrect,FRect.left + 4,rect.top - m_pFlowDocument->m_rcPageMargin.top,FRect.right - 6,rect.top);
				::FillRect(hPaintDC,&lrect,hBrush);

				::SetRect(&lrect,FRect.left + 4,rect.bottom ,FRect.right - 6,rect.bottom +  m_pFlowDocument->m_rcPageMargin.bottom);
				::FillRect(hPaintDC,&lrect,hBrush);
				::DeleteObject(hBrush);

				int y = rect.top - (int)(sz.cy*1.5/2.0) ;
				HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				logFont.lfHeight = -11;
				//logFont.lfEscapement = 900;
				hFont = ::CreateFontIndirect(&logFont);
				hFont = (HFONT)::SelectObject(hPaintDC,hFont);

				TCHAR buf[30];
				int nIndex = -1;
				RECT sRect;
				int hbr = ::SetBkMode(hPaintDC,TRANSPARENT);
				while(y <= rect.bottom + this->m_pFlowDocument->m_rcPageMargin.bottom)
				{
					nIndex += 1;
					if(nIndex >=2 && nIndex % 2 ==0)
					{
						::_stprintf_s(buf,30,_T("%i"),nIndex);
						::SetRect(&sRect,FRect.left + 4 ,y,FRect.right -5,y + (int)(sz.cy * 1.5));
						if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
					::MoveToEx(hPaintDC,FRect.left + 6,y,NULL);
					::LineTo(hPaintDC,FRect.right - 8,y);
					y += (int)(sz.cy * 1.5);
				}
				nIndex = 0;
				y = rect.top - 2*sz.cy;
				while(y >= rect.top - this->m_pFlowDocument->m_rcPageMargin.top)
				{
					nIndex += 1;
					if(nIndex >=2 && nIndex % 2 ==0)
					{
						::_stprintf_s(buf,30,_T("%i"),nIndex);
						::SetRect(&sRect,FRect.left + 4,y ,FRect.right - 5,y + (int)(sz.cy * 1.5));
						if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
					::MoveToEx(hPaintDC,FRect.left + 6,y,NULL);
					::LineTo(hPaintDC,FRect.right - 8,y);
					y -= (int)(sz.cy * 1.5);
				}

				::SetBkMode(hPaintDC,hbr);
				hFont = (HFONT)::SelectObject(hPaintDC,hFont);
				::DeleteObject(hFont);

				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
		}
		::RestoreDC(hPaintDC,nSave);

		return 1;
	}

	int XWordVRuler::SetFRect(RECT &rc)
	{
		FRect = rc;
		return 1;
	}
}}}}