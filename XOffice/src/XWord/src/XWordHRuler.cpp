#include "StdAfx.h"
#include "..\include\XWordHRuler.hpp"
#include "xwordlayer.hpp"
#include "xwordsheetsvr.hpp"
#include "xuxutil.h"
#include "xflowdocument.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	int XWordHRuler::m_nHeight = 24;
	XWordHRuler::XWordHRuler(void):m_pLayer(NULL),m_pSheetSvr(NULL),m_pFlowDocument(NULL)
	{
	}

	XWordHRuler::~XWordHRuler(void)
	{
	}

	int XWordHRuler::DoDraw(HDC hPaintDC,RECT * pDrawRect)
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
			OffsetViewportOrgEx(hPaintDC,- m_pLayer->m_ptScroll.x, 0,NULL);
		else
			OffsetViewportOrgEx(hPaintDC,- m_pFlowDocument->m_ptScroll.x, 0,NULL);

		LOGFONT	logFont;
		XUtil::Css::GetFont(NULL,logFont);
		
		logFont.lfHeight = -14; //10.5
		HFONT hFont = ::CreateFontIndirect(&logFont);

		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		
		SIZE sz;
		GetTextExtentPoint32(hPaintDC,_T("ÖÐ¹ú"),2,&sz);
		sz.cx = sz.cx /2;
		
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);
		
		RECT rect,lrect;

		if(m_pLayer)
			m_pLayer->CalcContentPageRect(0,&m_pLayer->FRect,rect);
		else
			m_pFlowDocument->CalcContentPageRect(0,&m_pFlowDocument->FRect,rect);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,255));
		::SetRect(&lrect,rect.left,FRect.top + 4,rect.right,FRect.bottom - 6);
		::FillRect(hPaintDC,&lrect,hBrush);
		::DeleteObject(hBrush);

		hBrush = ::CreateSolidBrush(RGB(158,190, 245));
		if(m_pLayer)
			::SetRect(&lrect,rect.left - m_pLayer->m_rcPageMargin.left,FRect.top + 4,rect.left,FRect.bottom - 6);
		else
			::SetRect(&lrect,rect.left - m_pFlowDocument->m_rcPageMargin.left,FRect.top + 4,rect.left,FRect.bottom - 6);

		::FillRect(hPaintDC,&lrect,hBrush);

		if(m_pLayer)
			::SetRect(&lrect,rect.right,FRect.top + 4,rect.right + m_pLayer->m_rcPageMargin.right,FRect.bottom - 6);
		else
			::SetRect(&lrect,rect.right,FRect.top + 4,rect.right + m_pFlowDocument->m_rcPageMargin.right,FRect.bottom - 6);

		::FillRect(hPaintDC,&lrect,hBrush);
		::DeleteObject(hBrush);

		int x = rect.left;
		HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);

		logFont.lfHeight = -11;
		hFont = ::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);

		TCHAR buf[30];
		int nIndex = -2;
		RECT sRect;
		int hbr = ::SetBkMode(hPaintDC,TRANSPARENT);
		if(m_pLayer)
		{
			while(x <= rect.right + this->m_pLayer->m_rcPageMargin.right)
			{
				nIndex += 2;
				::_stprintf_s(buf,30,_T("%i"),nIndex);
				::SetRect(&sRect,x - 20,FRect.top + 4,x+20,FRect.bottom - 5);
				if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				::MoveToEx(hPaintDC,x,FRect.bottom - 5,NULL);
				::LineTo(hPaintDC,x,FRect.bottom - 1);
				x += 2 * sz.cx;
			}
		}else
		{
			while(x <= rect.right + this->m_pFlowDocument->m_rcPageMargin.right)
			{
				nIndex += 2;
				::_stprintf_s(buf,30,_T("%i"),nIndex);
				::SetRect(&sRect,x - 20,FRect.top + 4,x+20,FRect.bottom - 5);
				if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				::MoveToEx(hPaintDC,x,FRect.bottom - 5,NULL);
				::LineTo(hPaintDC,x,FRect.bottom - 1);
				x += 2 * sz.cx;
			}
		}
		nIndex = 0;
		x = rect.left - 2*sz.cx;
		if(m_pLayer)
		{
			while(x >= rect.left - this->m_pLayer->m_rcPageMargin.left)
			{
				nIndex += 2;
				::_stprintf_s(buf,30,_T("%i"),nIndex);
				::SetRect(&sRect,x - 20,FRect.top + 4,x+20,FRect.bottom - 5);
				if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				::MoveToEx(hPaintDC,x,FRect.bottom - 5,NULL);
				::LineTo(hPaintDC,x,FRect.bottom - 1);
				x -= 2 * sz.cx;
			}
		}else
		{
			while(x >= rect.left - this->m_pFlowDocument->m_rcPageMargin.left)
			{
				nIndex += 2;
				::_stprintf_s(buf,30,_T("%i"),nIndex);
				::SetRect(&sRect,x - 20,FRect.top + 4,x+20,FRect.bottom - 5);
				if(nIndex > 0)::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&sRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				::MoveToEx(hPaintDC,x,FRect.bottom - 5,NULL);
				::LineTo(hPaintDC,x,FRect.bottom - 1);
				x -= 2 * sz.cx;
			}
		}
		::SetBkMode(hPaintDC,hbr);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);

		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);

		::RestoreDC(hPaintDC,nSave);

		return 1;
	}

	int XWordHRuler::SetFRect(RECT &rc)
	{
		FRect = rc;
		return 1;
	}
}}}}
