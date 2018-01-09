#include "StdAfx.h"
#include "xcLayerSheet.hpp"
#include "ilayersheet.hpp"

namespace Hxsoft{ namespace XFrame
{
	xcLayerSheet::xcLayerSheet(void):m_iCurSheet(0),m_iStartSheet(0),m_pSheets(NULL),m_SheetAllocSize(0),m_SheetSize(0),m_hImageList(NULL),
		m_cxImage(16),m_cyImage(16),m_pSheetsImage(NULL),m_TabArrow(taTop),m_TabDrawStyle(tdsShape),m_pWidths(NULL),m_pLayer(NULL),m_bMdiSheet(false),
		m_pFrame(NULL),m_pHwnds(new std::vector<HWND>),m_pSheetState(NULL)
	{
		m_bTransparent = true;
		::SetRect(&FRect,0,0,0,0);
		this->AttachListon(WM_PAINT,(FNEvent)&xcLayerSheet::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcLayerSheet::EvtLButtonDown);
	}

	xcLayerSheet::~xcLayerSheet(void)
	{	
		delete m_pHwnds;
		Clear();
	}

	int xcLayerSheet::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		return this->OnDraw(hPaintDC);
	}

	bool xcLayerSheet::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		RECT	rc;
		HDC	hPaintDC;
		HBITMAP	hBmp;

		HDC hdc = pEvent->paint.hdc;

		rc.left = rc.top = 0;
		rc.right = this->m_pxfNode->m_ContentRect.right - this->m_pxfNode->m_ContentRect.left;
		rc.bottom = this->m_pxfNode->m_ContentRect.bottom - this->m_pxfNode->m_ContentRect.top;

		xfNodeLayers * pNodeLayers = dynamic_cast<xfNodeLayers *>(m_pxfNode);
		if(pNodeLayers)
		{
			if(this->m_TabArrow==taTop)
				rc.bottom = pNodeLayers->m_tabSize;
			else if(this->m_TabArrow==taBottom)
				rc.top = rc.bottom - pNodeLayers->m_tabSize;
			else if(this->m_TabArrow==taLeft)
				rc.right = pNodeLayers->m_tabSize;
			else if(this->m_TabArrow==taRight)
				rc.left = rc.right - pNodeLayers->m_tabSize;
		}
		::OffsetRect(&rc,- rc.left,- rc.top);

		FRect =  rc;
		hPaintDC = ::CreateCompatibleDC(hdc);
		hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left,rc.bottom - rc.top);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

		HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		DoDraw(hPaintDC);
		::BitBlt(hdc,0,0,rc.right - rc.left ,rc.bottom - rc.top,hPaintDC,0,0,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hPaintDC); 
		return true;
	}

	bool xcLayerSheet::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		UINT nFlags = pEvent->mouse.nFlags;
		POINT point = pEvent->mouse.point;

		int px;
		px = point.x - FRect.left;
		for(int i=m_iStartSheet;i<m_SheetSize;i++)
		{
			int sWidth = m_pWidths[i];
			if(GetSheetState(i) == _hidden) sWidth = 0;
			if(px < sWidth)
			{
				if(this->m_TabDrawStyle == tdsCanClose && px>=sWidth-16)
					this->RemoveSheet(i);
				else if(m_iCurSheet != i)
					this->EventSheetChanged(i);
				break;
			}
			px -= sWidth;
		}
		return true;
	}

	int xcLayerSheet::DrawEclipseExSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}

	int xcLayerSheet::DrawEclipseSheet(HDC hdc,int iSheet, RECT rect)
	{
		if(iSheet < 0 || iSheet > m_SheetSize - 1) return -1;
		
		LPTSTR label = m_pSheets[iSheet];
		int iImage = m_pSheetsImage[iSheet];

		int startx,starty;
		int endx,endy;
		int nArc = 7;
		int startbmpw = m_cxImage;

		POINT	pt;
		startx = rect.left;
		starty = rect.top + 2;
		endy = rect.bottom - 2;
		endx = rect.right;

		if( iSheet < m_iCurSheet)
		{
			if(m_TabArrow ==taTop)
			{
				MoveToEx(hdc,startx,endy,&pt);
				LineTo(hdc,startx,starty + nArc  );
				Arc(hdc,startx ,starty  , startx + 2*nArc ,starty + 2*nArc,startx + nArc  ,starty   ,startx  , starty + nArc );
			}else if(m_TabArrow ==taBottom)
			{
				MoveToEx(hdc,startx,starty ,&pt);
				LineTo(hdc,startx,endy - nArc  );
				Arc(hdc,startx ,endy , startx + 2*nArc ,endy - 2*nArc ,startx   ,endy - nArc  ,startx + nArc , endy );
			}
		}else if(iSheet == m_iCurSheet)
		{
			POINT aptRectangle[4] ;
			if(m_TabArrow ==taTop)
			{
				BeginPath(hdc);
				aptRectangle[0].x = endx + 18   - 5;
				aptRectangle[0].y = starty  + 8;
				aptRectangle[1].x = endx + 10  -  5;
				aptRectangle[1].y =  starty  + 17;
				aptRectangle[2].x = endx + 0  -  5;
				aptRectangle[2].y =  starty  + 0;
				aptRectangle[3].x = endx + 35  -  5;
				aptRectangle[3].y = endy;
				
				MoveToEx(hdc,startx + nArc,starty ,&pt);
				ArcTo(hdc,startx  ,starty  , startx + 2*nArc ,starty + 2*nArc,startx + nArc  ,starty,startx  , starty + nArc);
				LineTo(hdc,startx,endy  );
				LineTo(hdc,aptRectangle[3].x,aptRectangle[3].y);
							
				PolyBezierTo(hdc,aptRectangle,3);
				LineTo(hdc,startx + nArc,starty);

				EndPath(hdc);
				
				HRGN	hrgn;
				hrgn = PathToRegion(hdc);
				hrgn = (HRGN)SelectObject( hdc,hrgn);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = startx;
				vertex[0].y = starty;
				vertex[0].Red = 255*256;
				vertex[0].Green = 255*256;
				vertex[0].Blue = 255*256 ;

				vertex[1].x = endx + 40 ;
				vertex[1].y = endy;
				vertex[1].Red = 157 *256;
				vertex[1].Green = 167 *256;
				vertex[1].Blue = 195 *256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;
				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);

				hrgn = (HRGN)SelectObject( hdc,hrgn);
				DeleteObject( hrgn);
				
				SelectClipRgn(hdc,0);
				MoveToEx(hdc,aptRectangle[3].x,aptRectangle[3].y,&pt);
				PolyBezierTo(hdc,aptRectangle,3 );
			}else if(m_TabArrow ==taBottom)
			{
				BeginPath(hdc);
				aptRectangle[0].x = endx + 18  - 5;
				aptRectangle[0].y = endy - 8;
				aptRectangle[1].x = endx + 10  -  5;
				aptRectangle[1].y = endy - 17;
				aptRectangle[2].x = endx + 0  -  5;
				aptRectangle[2].y = endy  + 0 ;
				aptRectangle[3].x = endx + 35  -  5;
				aptRectangle[3].y = starty ;
				
				MoveToEx(hdc,startx + nArc,starty,&pt);
				ArcTo(hdc,startx  ,endy  , startx + 2*nArc ,endy - 2*nArc,startx + nArc  ,endy,startx  , endy - nArc);
				LineTo(hdc,startx,starty   );
				LineTo(hdc,aptRectangle[3].x,aptRectangle[3].y);
							
				PolyBezierTo(hdc,aptRectangle,3);
				LineTo(hdc,startx + nArc,endy);

				EndPath(hdc);
				
				HRGN	hrgn;
				hrgn = PathToRegion(hdc);
				hrgn = (HRGN)SelectObject( hdc,hrgn);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = startx;
				vertex[0].y = endy;
				vertex[0].Red = 255*256;
				vertex[0].Green = 255*256;
				vertex[0].Blue = 255*256 ;

				vertex[1].x = endx + 40 ;
				vertex[1].y = starty;
				vertex[1].Red = 157 *256;
				vertex[1].Green = 167 *256;
				vertex[1].Blue = 195 *256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;
				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);

				hrgn = (HRGN)SelectObject( hdc,hrgn);
				DeleteObject( hrgn);
				
				SelectClipRgn(hdc,0);
				MoveToEx(hdc,aptRectangle[3].x,aptRectangle[3].y,&pt);
				PolyBezierTo(hdc,aptRectangle,3 );

			}
			m_pWidths[iSheet] += 18;
		}

		if(iImage && m_hImageList)
			::ImageList_Draw(m_hImageList,iImage,hdc, startx + 5,starty + 2 ,ILD_NORMAL);

		int nlen = 0;
		if(label)
			nlen = (int)_tcslen(label);
		TextOut(hdc,startx  + (iImage>0?startbmpw + 5:5),starty + 2,label,nlen);
		
		if(iSheet > m_iCurSheet)
		{
			if(m_TabArrow ==taTop)
			{
				startx = rect.right;
				MoveToEx(hdc,startx,endy,&pt);
				LineTo(hdc,startx,starty + nArc  );
				Arc(hdc,startx - 2*nArc ,starty + 2*nArc,startx  ,starty ,startx , starty + nArc ,startx - nArc  ,starty);
			}else if(m_TabArrow ==taBottom)
			{
				startx = rect.right;
				MoveToEx(hdc,startx,starty ,&pt);
				LineTo(hdc,startx,endy -  nArc  );
				Arc(hdc,startx - 2*nArc ,endy -  2*nArc,startx  ,endy ,startx , endy - nArc ,startx - nArc  ,endy);
			}
		}
		return 1;
	}
	int xcLayerSheet::DrawCloseSheet(HDC hdc,int iSheet, RECT rect)
	{
		if(iSheet < 0 || iSheet > m_SheetSize - 1) return -1;  
		
		LPTSTR label = m_pSheets[iSheet];
		int iImage = m_pSheetsImage[iSheet];

		int startx,starty;
		int endx,endy;
		int startbmpw = m_cxImage;
		int iClose = 16;

		startx = rect.left;
		starty = rect.top ;
		endy = rect.bottom - 1;
		endx = rect.right + iClose;

		POINT	points[4];

		points[0].x = startx;
		points[0].y = starty;
		points[1].x = endx;
		points[1].y = starty;
		points[2].x = endx;
		points[2].y = endy;
		points[3].x = startx;
		points[3].y = endy;
			
		int hbr = ::SetBkMode(hdc,0);
		COLORREF bkcolor = RGB(227,237,227);// GetSysColor(15);

		HBRUSH hbrush;
		if(iSheet != m_iCurSheet)
		{
			hbrush = ::CreateSolidBrush( bkcolor);
		}else
		{
			hbrush = ::CreateSolidBrush(RGB(255,255,255));
		}
		hbrush = (HBRUSH)::SelectObject(hdc, hbrush);
		HPEN hPen;
		HPEN hPenOld;
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hPenOld = (HPEN)SelectObject(hdc, hPen);
		//::Polygon(hdc,points,4);
		::DeleteObject(hbrush);

		int h = iClose/4;
		int h2 = (endy - starty - iClose + 1)/2;
		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		hPen = (HPEN)SelectObject(hdc, hPen);
		//¿ò
		int banJin = 4;
		//BeginPath(hdc);
		MoveToEx(hdc, startx, endy, NULL);
		LineTo(hdc, startx,starty+banJin);
		MoveToEx(hdc,startx,starty+banJin,NULL);
		Arc(hdc,startx, starty, startx+banJin*2, starty+banJin*2, startx+banJin, starty, startx, starty+banJin);
		MoveToEx(hdc,startx+banJin, starty,NULL);
		LineTo(hdc, endx-banJin,starty);
		MoveToEx(hdc,endx-banJin,starty,NULL);
		Arc(hdc,endx-banJin*2, starty, endx, starty+banJin*2, endx, starty+banJin, endx-banJin, starty);
		MoveToEx(hdc,endx, starty+banJin,NULL);
		LineTo(hdc, endx,endy);
		MoveToEx(hdc,endx,endy,NULL);
		LineTo(hdc, startx,endy);

		SelectObject(hdc, hPenOld);
		DeleteObject(hPen);
		//O
		//MoveToEx(hdc,endx - iClose/2,h2 ,NULL);
		//ArcTo(hdc,endx-iClose, h2, endx, iClose+h2,endx - iClose/2, h2, endx - iClose/2, h2);
		//X
		MoveToEx(hdc, endx-iClose+h, h2+h, NULL);
		LineTo(hdc, endx-h ,iClose+h2-h);
		MoveToEx(hdc, endx-h, h2+h, NULL);
		LineTo(hdc, endx-iClose+h ,iClose+h2-h);
		//EndPath(hdc);
			
		//::SetRect(&rect,points[1].x,points[0].y + 2,points[2].x,points[2].y - 1);
		::DrawText(hdc,label,(int)::_tcslen(label),&rect,DT_CENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
		::SetBkMode(hdc,hbr);

		m_pWidths[iSheet] += iClose;
		return 1;
	}

	int xcLayerSheet::DrawShapeSheet(HDC hdc,int iSheet, RECT rect)
	{
		if(iSheet < 0 || iSheet > m_SheetSize - 1) return -1;  
		
		LPTSTR label = m_pSheets[iSheet];
		int iImage = m_pSheetsImage[iSheet];

		int startx,starty;
		int endx,endy;
		int height,width;
		int startbmpw = m_cxImage;

		startx = rect.left;
		starty = rect.top ;
		endy = rect.bottom - 1;
		endx = rect.right;

		height = endy - starty;
		width  = endx - startx;

		POINT	points[4];
		if(m_TabArrow ==taBottom)
		{
			points[0].x = startx;
			points[0].y = starty;
			points[1].x = startx + height/2;
			points[1].y = starty + height;
			points[2].x = startx + width  + height/2;
			points[2].y = points[1].y ;
			points[3].x = startx + width  + height;
			points[3].y = points[0].y;
		}else if(m_TabArrow ==taTop)
		{
			points[0].x = startx + height/2;
			points[0].y = starty;
			points[1].x = startx;
			points[1].y = starty + height;
			points[2].x = startx + width + height;
			points[2].y = points[1].y ;
			points[3].x = startx + width + height/2;
			points[3].y = starty ;
		}
			
		int hbr = ::SetBkMode(hdc,0);
		COLORREF bkcolor = RGB(227,237,227);// GetSysColor(15);

		HBRUSH hbrush;
		if(iSheet != m_iCurSheet)
		{
			hbrush = ::CreateSolidBrush( bkcolor);
		}else
		{
			hbrush = ::CreateSolidBrush( RGB(255,255,255));
		}
		hbrush = (HBRUSH)::SelectObject(hdc, hbrush);
		::Polygon(hdc,points,4) ;
		hbrush = (HBRUSH)::SelectObject(hdc, hbrush);
		::DeleteObject(hbrush);
			
		::SetRect(&rect,points[1].x,points[0].y + 2,points[2].x,points[2].y - 1);
		::DrawText(hdc,m_pSheets[iSheet],(int)::_tcslen(m_pSheets[iSheet]),&rect,DT_CENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
		::SetBkMode(hdc,hbr);
		m_pWidths[iSheet] += height;
		return 1;
	}
	int xcLayerSheet::DrawRoundRectSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}
	int xcLayerSheet::DrawRoundRectExSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}

	int xcLayerSheet::DrawSheet(HDC hdc,int iSheet, RECT rect)
	{
		if(this->m_TabDrawStyle ==tdsEclips)
			return DrawEclipseSheet(hdc,iSheet, rect);
		if(this->m_TabDrawStyle ==tdsShape) 
			return this->DrawShapeSheet(hdc,iSheet, rect);
		if(this->m_TabDrawStyle ==tdsCanClose)
			return this->DrawCloseSheet(hdc,iSheet, rect);

		return 1;
	}

	int xcLayerSheet::ReCalcWidths(HDC hdc)
	{
		SIZE size;
		for(int i=0;i<m_SheetSize;i++)
		{
			int len = 0;
			if(m_pSheets[i]) len = (int)::_tcslen(m_pSheets[i]);
			GetTextExtentPoint32(hdc,m_pSheets[i],len,&size);
			m_pWidths[i] = size.cx + 10;
			if(m_pSheetsImage[i] >=0) m_pWidths[i] += m_cxImage;
		}
		return 1;
	}

	int xcLayerSheet::OnDraw(HDC hdc)
	{
		LOGFONT	logFont;
		/*
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("ËÎÌå")) + 1, _T("ËÎÌå"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;*/

		RECT rect = FRect;
		rect.left = FRect.left + 1;
		rect.top = FRect.top + 1;
		int nCssIndex = 0;
		if(this->GetXfNode() && this->GetXfNode()->m_nCssIndex)
			nCssIndex = this->GetXfNode()->m_nCssIndex;
		if(nCssIndex>=0)
		{
			const XUtil::Css::tagBoxStyle * pCssStyle =this->GetXfNode()->GetXPage()->m_pCCssStyle->GetBoxStyle(nCssIndex);
			if(pCssStyle && pCssStyle->mask &cssMaskBackGround)
			if(pCssStyle && (( pCssStyle->mask & cssMaskBackGround && pCssStyle->pBackGround)||(pCssStyle->mask&cssMaskBorder && pCssStyle->pBorder)))
			{
				Hxsoft::XUtil::Css::DrawCssFrame(hdc, &rect,pCssStyle);
				//delete pCssStyle;
			}
			Hxsoft::XUtil::Css::GetFont( pCssStyle,logFont);
		}else
		{
			HBRUSH	hBrush;
			hBrush = ::CreateSolidBrush(RGB(179,208,182));//::GetSysColorBrush(COLOR_3DLIGHT);
			::FillRect(hdc,&rect,hBrush);
			
			::DeleteObject(hBrush);
			Hxsoft::XUtil::Css::GetDefaultFont(logFont);
		}
		
		
		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		ReCalcWidths(hdc);
		HPEN hPen = ::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
		hPen = (HPEN)::SelectObject(hdc,hPen);
		COLORREF bkcolor = GetSysColor(15);
		int hbr = ::SetBkMode(hdc,0);

		rect = FRect;
		int	startx = FRect.left + 3; 
		int starty = FRect.top + 1;
		int nArc = 7;
		int startx0 =  FRect.left + 3;
		POINT	pt;

		rect.top = starty;
		
		/*
		int nIndexEnd = m_iStartSheet;
		int sx = startx;
		for(int i=this->m_iStartSheet;i<m_SheetSize;i++)
		{
			rect.left = sx;
			rect.right = rect.left + m_pWidths[i];
			if(rect.right >= FRect.right) break;
			nIndexEnd = i;
			sx = rect.left + m_pWidths[i];
		}

		for(int i=nIndexEnd; nIndexEnd<m_SheetSize && i>=m_iStartSheet;i--)
		{
			rect.right = sx;
			rect.left = rect.right - m_pWidths[i];
			DrawSheet(hdc,i,rect);
			sx = rect.right - m_pWidths[i];
		}
		*/
		
		for(int i=this->m_iStartSheet;i<m_SheetSize;i++)
		{
			rect.left = startx;
			rect.right = rect.left + m_pWidths[i];
			if(rect.right >= FRect.right) break;
			if(GetSheetState(i) == _hidden) continue;
			DrawSheet(hdc,i,rect);
			startx = rect.left + m_pWidths[i];
		}
		

		if(this->m_TabDrawStyle ==tdsEclips)
		{
			if( m_SheetSize >0)
			{
				if(m_TabArrow ==taTop)
				{
					if(m_SheetSize - 1==m_iCurSheet)
					{
						MoveToEx(hdc,startx0 + nArc , starty  ,&pt);
						LineTo(hdc,startx + m_pWidths[m_SheetSize -1] - nArc  ,starty);
					}else
					{
						MoveToEx(hdc,startx0 + nArc , starty  ,&pt);
						LineTo(hdc,startx  -  5  ,starty);
					}
				}else if(m_TabArrow ==taBottom)
				{
					int height = FRect.bottom - FRect.top - 2;
					if(m_SheetSize - 1==m_iCurSheet)
					{
						MoveToEx(hdc,startx0 + nArc , starty + height  ,&pt);
						LineTo(hdc,startx + m_pWidths[m_SheetSize -1] - nArc  ,starty + height);
					}else
					{
						MoveToEx(hdc,startx0 + nArc , starty + height  ,&pt);
						LineTo(hdc,startx  -  5  ,starty + height);
					}
				}
			}
		}

		::SetBkMode(hdc,hbr);
		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);

		return 0;
	}

	int xcLayerSheet::Redraw()
	{
		if(m_hWnd)
		{
			HDC	hdc;
			hdc = ::GetDC(m_hWnd);
			OnDraw(hdc);
			::ReleaseDC(m_hWnd,hdc);
		}
		return 1;
	}
	int  xcLayerSheet::EventSheetChanged(int NewCurSheet)
	{
		LYSNMHDR hdr;
		memset(&hdr,0,sizeof(LYSNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = LYSN_PRESELECTSHEET;
		hdr.nSheet = m_iCurSheet;

		::SendMessage(GetWin()->m_hWnd,WM_NOTIFY,0,(LPARAM)&hdr);

		m_iCurSheet = NewCurSheet;
		//if(m_pLayer) m_pLayer->SwitchTo(NewCurSheet);
		if(this->m_pFrame && NewCurSheet>=0 && NewCurSheet < (int)m_pHwnds->size())
		{
			if(m_pFrame->PreSelectSheet(NewCurSheet)<0) return -1;
			xfNode * pNode = m_pxfNode->GetXPage()->GetAgentNode();

			xfWin * pWin = (xfWin *)GetProp((*m_pHwnds)[NewCurSheet],_T("this"));
			if(pNode && !(pWin && pWin->m_pXPage && pNode->GetXPage()->m_pFocusNodeControl && pNode->GetXPage() &&
				pWin->m_pXPage->GetFocusNodeControl() == pNode->GetXPage()->m_pFocusNodeControl))
			{
				pNode->GetXPage()->ClearAgentNodeContent();
				pNode->GetXPage()->m_pFocusNodeControl = NULL;
			}

			::SendMessage(m_pFrame->m_hMdiClient,WM_MDIACTIVATE,(WPARAM)(*m_pHwnds)[NewCurSheet],0L);
			m_pFrame->SelectSheet(NewCurSheet);
		}else
		{
			if(m_pxfNode)
			{
				xfNodeLayers * pNodeLayer = dynamic_cast<xfNodeLayers *>(m_pxfNode);
				if(pNodeLayer) pNodeLayer->SelectSheet(NewCurSheet,true);

			}
		}

		memset(&hdr,0,sizeof(LYSNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.code = LYSN_SELECTEDSHEET;
		hdr.nSheet = NewCurSheet;
		::SendMessage(GetWin()->m_hWnd,WM_NOTIFY,0,(LPARAM)&hdr);

		Redraw();

		return 1;
	}

	int xcLayerSheet::SelectSheet(int NewCurSheet)
	{
		int nRet = EventSheetChanged(NewCurSheet);


		return nRet;
	}

	void xcLayerSheet::SetImageExtent(int cx,int cy)
	{
		m_cxImage = cx;
		m_cyImage = cy;
	}

	int xcLayerSheet::AppendSheetAlloc(int inc)
	{
		LPTSTR * pTemp = new LPTSTR[m_SheetAllocSize + inc];
		int *	pTempImage = new int[m_SheetAllocSize + inc];
		int * pTempWidth = new int[m_SheetAllocSize + inc];

		::memset(pTempWidth,-1,sizeof(int)*m_SheetSize);

		if(m_SheetSize > 0 && m_pSheets)
		{
			::CopyMemory(pTemp,m_pSheets,sizeof(LPTSTR)*m_SheetSize);
			::CopyMemory(pTempImage,m_pSheetsImage,sizeof(int)*m_SheetSize);
			::CopyMemory(pTempWidth,m_pWidths,sizeof(int)*m_SheetSize);

		}
		if(m_pSheets) delete m_pSheets;
		if(m_pSheetsImage) delete m_pSheetsImage;
		if(m_pWidths)delete m_pWidths;
		m_pSheets = pTemp;
		m_pSheetsImage = pTempImage;
		m_pWidths =  pTempWidth;
		
		m_SheetAllocSize += inc;
		return 1;
	}
	int xcLayerSheet::AddSheet(LPTSTR pSheet,int iImage, bool bToCurrent)
	{
		LPTSTR pStrSheet = ::_tcsdup(pSheet);

		if(m_SheetSize==m_SheetAllocSize)AppendSheetAlloc(16);
		m_SheetSize ++;
		m_pSheets[m_SheetSize - 1] = pStrSheet;
		m_pSheetsImage[m_SheetSize - 1] = iImage;
		if(m_hWnd)
		{
			if(bToCurrent)
				EventSheetChanged(m_SheetSize - 1);
			else
				Redraw();
		}

		return m_SheetSize - 1;
	}

	int xcLayerSheet::GetSheetCount()
	{
		return m_SheetSize;
	}

	int xcLayerSheet::InsertSheet(int iPos,LPTSTR pSheet,int iImage, bool bToCurrent)
	{
		if(iPos > m_SheetSize -1 || iPos < 0) return AddSheet(pSheet,bToCurrent);
		if(m_SheetSize==m_SheetAllocSize)AppendSheetAlloc(16);
		for(int i= m_SheetSize -1;i>=iPos;i--)
		{
			m_pSheets[i+1] = m_pSheets[i];
			m_pSheetsImage[i+1] = m_pSheetsImage[i];
		}
		LPTSTR pStrSheet = ::_tcsdup(pSheet);
		m_pSheets[iPos] = pStrSheet;
		m_pSheetsImage[iPos] = iImage;
		m_SheetSize ++;

		if(m_iCurSheet >=iPos) m_iCurSheet ++;
		//if(m_iStartSheet >= iPos) m_iStartSheet ++;
		if(m_hWnd)
		{
			if(bToCurrent)
				EventSheetChanged(iPos);
			else
				Redraw();
		}
		return iPos;
	}

	int xcLayerSheet::RemoveSheet(int iPos)
	{
		if(iPos < 0) return -1;
		if(iPos > m_SheetSize) return -1;
		if(m_pSheets[iPos]) delete m_pSheets[iPos];

		for(int i=iPos;i<m_SheetSize - 1;i++)
		{
			m_pSheets[i] = m_pSheets[i+1];
			m_pSheetsImage[i] = m_pSheetsImage[i + 1];
		}
		if(m_SheetSize >0)
		{
			m_pSheets[m_SheetSize] = NULL;
			m_pSheetsImage[m_SheetSize] = -1;
			m_pWidths[m_SheetSize] = 0;
		}
		m_SheetSize--;
		bool bSheetChaged = m_iCurSheet ==iPos?true:false;
		if(m_iCurSheet >iPos) m_iCurSheet --;
		if(m_iStartSheet > iPos) m_iStartSheet --;
		if(m_iCurSheet >= m_SheetSize)m_iCurSheet --;
		if(bSheetChaged)
			EventSheetChanged(m_iCurSheet);
		else
			Redraw();
		return 1;
	}
	int xcLayerSheet::Clear()
	{
		if(m_pSheets)
		{
			for(int i=0;i<m_SheetSize;i++)
			{
				if(m_pSheets[i]) delete m_pSheets[i];
			}
			delete[] m_pSheets;
			if(m_pSheetsImage) delete m_pSheetsImage;
			if(m_pWidths) delete m_pWidths;
			m_pSheets = NULL;
			m_pWidths = NULL;
			m_pSheetsImage = NULL;
		}
		return 1;
	}
	int xcLayerSheet::MoveSheet(int From,int To)
	{
		if(From < 0||From > m_SheetSize -1) return -1;
		if(To < 0 || To > m_SheetSize -1) return -1;

		if(From == To) return 1;
		
		LPTSTR	pTemp = m_pSheets[To];
		int temp = m_pSheetsImage[To];
		int tempw = m_pWidths[To];
		
		m_pSheets[To] = m_pSheets[From];
		m_pSheetsImage[To] = m_pSheetsImage[From];
		m_pWidths[To] =  m_pWidths[From];

		m_pSheets[From] = pTemp;
		m_pSheetsImage[From] =temp;
		m_pWidths[From] =tempw;
		
		Redraw();
		return 1;
	}
	void xcLayerSheet::Initial()
	{
		/*
		IXMLDOMNodeListPtr		pList;
		IXMLDOMNodePtr			pNode;
		IXMLDOMElement*			pElement;
		long	len;
		LPTSTR	pStr;
		this->m_pxfNode->m_pElement->selectNodes(L"layer",&pList);
		pList->get_length(&len);

		for(int i=0;i<len;i++)
		{
			pList->get_item(i,&pNode);
			pElement = xbXml::GetElement(pNode);
			pStr = xbXml::GetStringAttribe(pElement,_T("caption"));
			if(pStr)
			{
				this->AddSheet(pStr);
				delete pStr;
			}else
				this->AddSheet(_T("Untitled"));
		}
		if(len >0) this->m_iCurSheet = 0;
		*/
		m_TabArrow = taTop;
		m_TabDrawStyle = tdsEclips;

		if(m_pxfNode && m_pxfNode->m_pElement)
		{
			_variant_t var;
			m_pxfNode->m_pElement->getAttribute(L"mdisheet",&var);
			if(var.bstrVal)
			{
				if(::_tcsicmp(var.bstrVal,_T("yes"))==0 || ::_tcsicmp(var.bstrVal,_T("true"))==0 ||
					::_tcsicmp(var.bstrVal,_T("1"))==0)
				{
					this->m_bMdiClient = true;
					m_pFrame = dynamic_cast<xfMdiFrame*>(m_pxfNode->GetXPage()->m_pWin);
					if(m_pFrame) return ;
				}
			}
		}
		if(m_pxfNode)
		{
			for(int i=0;i<(int)m_pxfNode->m_pChilds->size();i++)
			{
				this->AddSheet((*m_pxfNode->m_pChilds)[i]->m_pCaption);
			}
			if(m_pxfNode->m_pChilds->size() >0) this->m_iCurSheet = 0;

			xfNodeLayers * pLayersNode = dynamic_cast<xfNodeLayers *>(m_pxfNode);
			if(pLayersNode)
			{
				m_TabArrow = (XTabArraow)pLayersNode->m_nTabArrow;
				m_TabDrawStyle = (XTabDrawStyle)pLayersNode->m_nTabDrawStyle;
			}
		}
	}
	xbObject* xcLayerSheet::GetInterface()
	{
		if(!m_pInterFace)
		{
			ILayerSheet * pILayer = new ILayerSheet();
			pILayer->m_pLayerSheet = this;
			m_pInterFace = (xbObject *)pILayer;
			pILayer->m_pControl = this;
		}
		return this->m_pInterFace;
	}
	
	int  xcLayerSheet::GetXml(BSTR &bstr)
	{
		bstr = NULL;
		xfNodeLayers * pLayers = dynamic_cast<xfNodeLayers *>(m_pxfNode);
		if(pLayers)
		{	
			wstring ws;
			for(int i=0;i<(int)pLayers->m_pChilds->size();i++)
			{
				bstr = NULL;
				(*pLayers->m_pChilds)[i]->GetXml(bstr);
				if(bstr)
					ws = ws + L"<sheet>" + bstr + L"</sheet>" ;
				else
					ws += L"<sheet/>" ;
			}
			bstr = ::SysAllocString(ws.c_str());
			return 1;
		}
		return 0;
	}

	int xcLayerSheet::SetCloseButton(int nSheet, bool bClose)
	{

		/*HBITMAP hbp = NULL;
		TCHAR path[MAX_PATH];
		::GetModuleFileName(NULL,path,MAX_PATH);
		::PathRemoveFileSpec(path);
		TCHAR buf[MAX_PATH];
		::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\close.bmp"),path);
		hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
		hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

		int dTop = (dRect.bottom - dRect.top)/2 - 8;
		BitBlt(hdc,dRect.left,dRect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

		hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
		::DeleteObject(hbp);
		::DeleteDC(hMemDC);*/
		return 1;
	}

	int xcLayerSheet::SetSheetState(int sheet, int state)
	{
		if(!m_pSheetState) m_pSheetState = new vector<sheetState>;
		for(int i=(int)m_pSheetState->size(); i<=sheet;i++)
			m_pSheetState->push_back(_show);
		(*m_pSheetState)[sheet] = (sheetState)state;
		return 1;
	}

	int xcLayerSheet::GetSheetState(int sheet)
	{
		if(!m_pSheetState) return _show;
		if(sheet >= (int)m_pSheetState->size()) return _show;
		return (int)((*m_pSheetState)[sheet]);
	}

	int xcLayerSheet::SetSheetText(int sheet, LPTSTR text)
	{
		if(sheet < 0 || sheet >= GetSheetCount()) return -1;
		m_pSheets[sheet] = text;
		return 1;
	}

	LPTSTR xcLayerSheet::GetSheetText(int sheet)
	{
		if(sheet < 0 || sheet >= GetSheetCount()) return NULL;
		return m_pSheets[sheet];
	}

}}
