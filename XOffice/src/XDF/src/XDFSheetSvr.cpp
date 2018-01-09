#include "StdAfx.h"
#include "..\include\XDFSheetSvr.hpp"
#include "xdfSerializeSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	XDFSheetSvr::XDFSheetSvr(void)
	{
		m_pSerializeSvr = new XDFSerializeSvr;
		m_pDataSvr = new XDFDataSvr;

		/*
		IXMLDOMElement *	pElement;
		XDFSerializeSvr * pSerializeSvr = new XDFSerializeSvr();
		xbXml xml;
		xml.Load(_T("xdoc\\testxdf.xml"));
		xml.m_pXmlDoc->get_documentElement(&pElement);

		pSerializeSvr->LoadSheetData(NULL,pElement,m_pDataSvr);
		delete pSerializeSvr;
		pElement->Release();*/

	}

	XDFSheetSvr::~XDFSheetSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
	}

	int XDFSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		if(!((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr) return 0;

		RECT	rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = this->FRect;

		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9,GetDeviceCaps(hPaintDC, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;

		HFONT hFont = ::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);

		RECT rectex = rect;
		rectex.left += 2;
		rectex.top += 2;
		rectex.right = rectex.left + ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_nPageBarWidth;
		rectex.bottom -= 2;

		HBRUSH hBrush = ::CreateSolidBrush(RGB(144,153,198));
		::FillRect(hPaintDC,&rectex,hBrush);
		::DeleteObject(hBrush);

		RECT rectp = rectex;
		rectp.left = rectex.right +2;
		rectp.right = rect.right - 2;
		hBrush = ::CreateSolidBrush(RGB(144,153,174));
		::FillRect(hPaintDC,&rectp,hBrush);
		::DeleteObject(hBrush);

		RECT rects;
		int swidth;
		rects = rectex;
		TCHAR buf[80];

		rects.left += 15;
		rects.right -= 15;
		rects.top +=10;
		swidth = rects.right - rects.left;
		int nBkMode = ::SetBkMode(hPaintDC,TRANSPARENT);
		for(int i=0;i<((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages.size();i++)
		{
			rects.bottom = rects.top + ::MulDiv(swidth,((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->pageh,
				((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->pagew);
			if(i== ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage)
			{
				HPEN hPen = ::CreatePen(PS_SOLID,2,RGB(0x00,0x00,0xff));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::Rectangle(hPaintDC,rects.left,rects.top,rects.right,rects.bottom);
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}else
				::Rectangle(hPaintDC,rects.left,rects.top,rects.right,rects.bottom);
			((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->srect = rects;

			rects.top = rects.bottom + 18;
			::_stprintf_s(buf,80,_T("ตฺ %i าณ"),i + 1);
			RECT rectt;
			rectt = rectex;
			rectt.top = rects.bottom + 2;
			rectt.bottom += rectt.top + 20;
			::DrawText(hPaintDC,buf,(int)_tcslen(buf),&rectt,DT_CENTER);
			if(rects.top > rect.bottom) break;
		}
		::SetBkMode(hPaintDC,nBkMode);


		RECT rcPage;
		RECT rc = rectp;
		int startY = rc.top + 20;
		int nSaveDC = ::SaveDC(hPaintDC);
		IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);

		/*for(int i=0;i<((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages.size();i++)
		{*/
			int i = ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage;
			rcPage.left = rc.left + (rc.right - rc.left)/2 - ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->pagew/2;
			rcPage.top =  startY;
			rcPage.right = rcPage.left + ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->pagew;
			rcPage.bottom = rcPage.top + ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->pageh;
			::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
			((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->prect = rcPage;

			for(int k=0;k<((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->m_DrawObjs.size();k++)
			{
				((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->m_DrawObjs[k]->DoDraw(hPaintDC,rcPage,((XDFDataSvr *)m_pDataSvr)->m_pStyleSvr,
					((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]);
			}

		//	if(rcPage.bottom > rc.bottom) break; 
		//}
		::RestoreDC(hPaintDC,nSaveDC);

	   	hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);

		return 1;
	}
	int XDFSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		return 0;
	}

	void XDFSheetSvr::OnLButtonDown( UINT nFlags, POINT point )
	{
		RECT rectprev;
		if(!((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr) return ;
		for(int i=0;i<((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages.size();i++)
		{
			if( PtInRect(&((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[i]->srect,point))
			{
				rectprev = ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage]->srect;
				InflateRect(&rectprev,2,2);
				::InvalidateRect(m_pSheetCtrl->m_hWnd,&rectprev,false);

				rectprev = ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage]->prect;
				InflateRect(&rectprev,2,2);
				::InvalidateRect(m_pSheetCtrl->m_hWnd,&rectprev,false);

				((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage = i;

				rectprev = ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage]->srect;
				InflateRect(&rectprev,2,2);
				::InvalidateRect(m_pSheetCtrl->m_hWnd,&rectprev,false);

				rectprev = ((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_Pages[((XDFDataSvr *)m_pDataSvr)->m_pPagesSvr->m_curPage]->prect;
				InflateRect(&rectprev,2,2);
				::InvalidateRect(m_pSheetCtrl->m_hWnd,NULL,false);
			}
		}
		return ;
	}
}}}}
