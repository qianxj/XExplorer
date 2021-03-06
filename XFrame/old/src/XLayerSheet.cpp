#include "StdAfx.h"
#include "XLayerSheet.h"
#include "xnode.h"

namespace Hxsoft{ namespace XFrame
{
	XLayerSheetCtrl::XLayerSheetCtrl(void):m_iCurSheet(0),m_iStartSheet(0),m_pSheets(NULL),m_SheetAllocSize(0),m_SheetSize(0),m_hImageList(NULL),
		m_cxImage(16),m_cyImage(16),m_pSheetsImage(NULL),m_TabArrow(taTop),m_TabDrawStyle(tdsShape),m_pWidths(NULL),m_pLayer(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
	}

	XLayerSheetCtrl::~XLayerSheetCtrl(void)
	{
		Clear();
	}
	int XLayerSheetCtrl::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
	{
		if(message==WM_LBUTTONDOWN)
		{
			POINT point;
			UINT_PTR	dwFlag;
			dwFlag = wParam;
			point.x = MAKEPOINTS(lParam).x;
			point.y = MAKEPOINTS(lParam).y;
			if(::PtInRect(&FRect,point))
				this->OnLButtonDown((UINT)dwFlag,point);
		}
		else if(message==WM_LBUTTONUP)
		{
			POINT point;
			UINT_PTR	dwFlag;
			dwFlag = wParam;
			point.x = MAKEPOINTS(lParam).x;
			point.y = MAKEPOINTS(lParam).y;
			if(::PtInRect(&FRect,point))
				this->OnLButtonUp((UINT)dwFlag,point);
		}
		else if(message==WM_MOUSEMOVE)
		{
			POINT point;
			UINT_PTR	dwFlag;
			dwFlag = wParam;
			point.x = MAKEPOINTS(lParam).x;
			point.y = MAKEPOINTS(lParam).y;

			if(::PtInRect(&FRect,point))
				this->OnMouseMove((UINT)dwFlag,point);
		}else if(message==WM_PAINT)
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);

			RECT	rc;
			HDC	hPaintDC;
			HBITMAP	hBmp;

			rc.left = rc.top = 0;
			rc.right = this->m_pNode->m_ContentRect.right - this->m_pNode->m_ContentRect.left;
			rc.bottom = this->m_pNode->m_ContentRect.bottom - this->m_pNode->m_ContentRect.top;
			FRect =  rc;
			hPaintDC = ::CreateCompatibleDC(hdc);
			hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left + 1,rc.bottom - rc.top +1);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

			HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
			::FillRect(hPaintDC,&rc,hBrush);
			::DeleteObject(hBrush);
	
			this->OnDraw(hPaintDC);
			::BitBlt(hdc,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hPaintDC); 

			EndPaint(hWnd, &ps);
			return 1;
		}
		return 0;
	}

	void XLayerSheetCtrl::OnLButtonDown(UINT nFlags,POINT point)
	{
		int px;
		px = point.x - FRect.left;
		for(int i=m_iStartSheet;i<m_SheetSize;i++)
		{
			if(px < m_pWidths[i])
			{
				this->EventSheetChanged(i);
				break;
			}
			px -= m_pWidths[i];
		}
	}
	void XLayerSheetCtrl::OnLButtonUp(UINT nFlags,POINT point)
	{
	}
	void XLayerSheetCtrl::OnMouseMove(UINT nFlags,POINT point)
	{
	}

	int XLayerSheetCtrl::DrawEclipseExSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}

	int XLayerSheetCtrl::DrawEclipseSheet(HDC hdc,int iSheet, RECT rect)
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

		TextOut(hdc,startx  + (iImage>0?startbmpw:0) + 5 ,starty + 2,label,(int)_tcslen(label));
		
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

	int XLayerSheetCtrl::DrawShapeSheet(HDC hdc,int iSheet, RECT rect)
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
		points[0].x = startx;
		points[0].y = starty;
		points[1].x = startx + height/2;
		points[1].y = starty + height;
		points[2].x = startx + width  + height/2;
		points[2].y = points[1].y ;
		points[3].x = startx + width  + height;
		points[3].y = points[0].y;
			
		int hbr = ::SetBkMode(hdc,0);
		COLORREF bkcolor = GetSysColor(15);

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
		::DrawText(hdc,m_pSheets[iSheet],(int)::_tcslen(m_pSheets[iSheet]),&rect,DT_CENTER|DT_WORD_ELLIPSIS);
		::SetBkMode(hdc,hbr);
		m_pWidths[iSheet] += height;
		return 1;
	}
	int XLayerSheetCtrl::DrawRoundRectSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}
	int XLayerSheetCtrl::DrawRoundRectExSheet(HDC hdc,int iSheet, RECT rect)
	{
		return 1;
	}

	int XLayerSheetCtrl::DrawSheet(HDC hdc,int iSheet, RECT rect)
	{
		if(this->m_TabDrawStyle ==tdsEclips) 
			return DrawEclipseSheet(hdc,iSheet, rect);
		if(this->m_TabDrawStyle ==tdsShape) 
			return this->DrawShapeSheet(hdc,iSheet, rect);

		return 1;
	}

	int XLayerSheetCtrl::ReCalcWidths(HDC hdc)
	{
		SIZE size;
		for(int i=0;i<m_SheetSize;i++)
		{
			GetTextExtentPoint32(hdc,m_pSheets[i],(int)::_tcslen(m_pSheets[i]),&size);
			m_pWidths[i] = size.cx + 10;
			if(m_pSheetsImage[i] >=0) m_pWidths[i] += m_cxImage;
		}
		return 1;
	}

	int XLayerSheetCtrl::OnDraw(HDC hdc)
	{
		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("����")) + 1, _T("����"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;

		HBRUSH	hBrush;
		hBrush = ::CreateSolidBrush(RGB(179,208,182));//::GetSysColorBrush(COLOR_3DLIGHT);
		RECT rect = FRect;
		rect.left = FRect.left + 1;
		rect.top = FRect.top + 1;
		::FillRect(hdc,&rect,hBrush);
		
		::DeleteObject(hBrush);

		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		ReCalcWidths(hdc);

		HPEN hPen = ::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
		hPen = (HPEN)::SelectObject(hdc,hPen);
		COLORREF bkcolor = GetSysColor(15);
		int hbr = ::SetBkMode(hdc,0);

		rect = FRect;
		int	startx = FRect.left; 
		int starty = FRect.top + 1;
		int nArc = 7;
		int startx0 =  FRect.left;
		POINT	pt;

		rect.top = starty;
		for(int i=this->m_iStartSheet;i<m_SheetSize;i++)
		{
			rect.left = startx;
			rect.right = rect.left + m_pWidths[i];
			if(rect.right >= FRect.right) break;
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

	int XLayerSheetCtrl::Redraw()
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
	int  XLayerSheetCtrl::EventSheetChanged(int NewCurSheet)
	{
		m_iCurSheet = NewCurSheet;
		if(m_pLayer) m_pLayer->SwitchTo(NewCurSheet);
		Redraw();

		return 1;
	}

	int XLayerSheetCtrl::SelectSheet(int NewCurSheet)
	{
		return EventSheetChanged(NewCurSheet);
	}

	void XLayerSheetCtrl::SetImageExtent(int cx,int cy)
	{
		m_cxImage = cx;
		m_cyImage = cy;
	}

	int XLayerSheetCtrl::AppendSheetAlloc(int inc)
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
	int XLayerSheetCtrl::AddSheet(LPTSTR pSheet,int iImage, bool bToCurrent)
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

	int XLayerSheetCtrl::InsertSheet(int iPos,LPTSTR pSheet,int iImage, bool bToCurrent)
	{
		if(iPos > m_SheetSize -1) return AddSheet(pSheet,bToCurrent);
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
		if(m_iStartSheet >= iPos) m_iStartSheet ++;
		if(m_hWnd)
		{
			if(bToCurrent)
				EventSheetChanged(iPos);
			else
				Redraw();
		}
		return iPos;
	}

	int XLayerSheetCtrl::RemoveSheet(int iPos)
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
		if(m_iCurSheet >=iPos) m_iCurSheet --;
		if(m_iStartSheet >= iPos) m_iStartSheet --;
		if(bSheetChaged)
			EventSheetChanged(m_iCurSheet);
		else
			Redraw();
		return 1;
	}
	int XLayerSheetCtrl::Clear()
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
	int XLayerSheetCtrl::MoveSheet(int From,int To)
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
}}
