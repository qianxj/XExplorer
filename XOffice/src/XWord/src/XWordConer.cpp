#include "StdAfx.h"
#include "..\include\XWordConer.hpp"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordConer::XWordConer(void)
	{
	}

	XWordConer::~XWordConer(void)
	{
	}
	int XWordConer::DoDraw(HDC hPaintDC,RECT * pDrawRect)
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
		
		::InflateRect(&rc,-5,-5);
		HPEN hPen = CreatePen(PS_SOLID,1,RGB(75, 120, 202));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);

		::MoveToEx(hPaintDC,rc.left,rc.top,NULL);
		::LineTo(hPaintDC,rc.right,rc.top);
		::LineTo(hPaintDC,rc.right,rc.bottom);
		::LineTo(hPaintDC,rc.left,rc.bottom);
		::LineTo(hPaintDC,rc.left,rc.top);
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);

		hPen = CreatePen(PS_SOLID,1,RGB(255,255,255));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);

		::MoveToEx(hPaintDC,rc.left+1,rc.top+1,NULL);
		::LineTo(hPaintDC,rc.right - 1,rc.top + 1);
		::MoveToEx(hPaintDC,rc.left+1,rc.top+1,NULL);
		::LineTo(hPaintDC,rc.left + 1,rc.bottom - 1);
		
		::MoveToEx(hPaintDC,rc.left+1,rc.bottom+1,NULL);
		::LineTo(hPaintDC,rc.right ,rc.bottom + 1);

		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		return 1;
	}

	int XWordConer::SetFRect(RECT &rc)
	{
		FRect = rc;
		return 1;
	}
}}}}
