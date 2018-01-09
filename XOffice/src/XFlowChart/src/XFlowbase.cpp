#include "stdafx.h"
#include "XFlowbase.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowObjectLink::XFlowObjectLink():LinkPos(0),LinkType(0),pFromFlowObject(NULL),pToFlowObject(NULL)
	{
		m_shape = line;
	}
	void XFlowObjectLink::DoDraw(HDC hPaintDC)
	{
		POINT	pt,prevPt;
		::Polyline(hPaintDC,m_pPloys,m_nPloys);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0,255,0));
		hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
		for(int i=0;i<m_nPloys;i++)
		{
			if(i>0) prevPt =pt;
			pt = m_pPloys[i];
			if(this->m_Status & XFOSELECTED)
			{
				::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
			}else
			{
				HPEN hPen = ::CreatePen(PS_DASH,1,RGB(0,0,255));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
				::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
				::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
				::LineTo(hPaintDC, pt.x - 3,pt.y + 3);
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
		}
		hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
		::DeleteObject(hBrush);
	}

	int XFlowObject::AddPoint(int px,int py)
	{
		POINT	pt;
		pt.x = px;
		pt.y = py;
		return AddPoint(pt);
	}
	int XFlowObject::AddPoint(POINT pt)
	{
		if(m_nPloys == m_nPloysLimit)
		{
			m_nPloysLimit += m_nPloysIns;
			POINT * pTemp = new POINT[m_nPloysLimit];
			if(m_pPloys)
			{
				::memcpy_s(pTemp,m_nPloysLimit *sizeof(POINT),m_pPloys,m_nPloys*sizeof(POINT));
				::delete m_pPloys;
			}
			m_pPloys = pTemp;
		}
		m_nPloys++;
		m_pPloys[m_nPloys - 1] = pt;
		return m_nPloys - 1;
	}

	void XFlowObject::Removepoint(int nIndex)
	{
		if(nIndex <0 || nIndex >= m_nPloys) return;
		for(int i= nIndex ;i<m_nPloys - 1;i++)
		{
			m_pPloys[i] = m_pPloys[i+1];
		}
		m_nPloys --;
	}

	int XFlowObject::InsertPoint(int nIndex,POINT pt)
	{
		if(nIndex <0) return -1;
		if(nIndex >= m_nPloys) return AddPoint(pt);
		if(m_nPloys == m_nPloysLimit)
		{
			m_nPloysLimit += m_nPloysIns;
			POINT * pTemp = new POINT[m_nPloysLimit];
			if(m_pPloys)
			{
				::memcpy_s(pTemp,m_nPloysLimit *sizeof(POINT),m_pPloys,m_nPloys*sizeof(POINT));
				::delete m_pPloys;
			}
			m_pPloys = pTemp;
		}
		for(int i= m_nPloys;i>nIndex; i++)
		{
			m_pPloys[i] = m_pPloys[i-1];
		}
		m_nPloys++;
		m_pPloys[nIndex] = pt;
		return nIndex;
	}
	void XFlowObject::ClearPoint()
	{
		if(m_pPloys)delete m_pPloys;
		m_nPloys = 0;
		m_nPloysLimit = 0;
	}

	XFlowObject::XFlowObject():m_ptHands(NULL),m_nRowHeight(24),m_Status(0),m_startColor(0xccdccc),
		m_endColor(0xecfcec),pLinks(NULL),m_ppItems(NULL),m_nCategory(0),m_nCategoryItems(NULL),m_pData(NULL),
		m_nFlowObjectIns(16),m_nLimit(0),m_pLimits(NULL),m_pCaption(NULL),m_shape(bezierrects),
		m_pPloys(NULL),m_nPloys(0),m_nPloysLimit(0),m_nPloysIns(4)
	{
		::SetRect(&FRect,0,0,0,0);
	}

	XFlowObject::~XFlowObject()
	{
		if(m_ptHands) delete[] m_ptHands;
		for(int i=0;i<m_nCategory;i++)
		{
			if(m_ppItems[i])
			{
				for(int j=0;j<m_nCategoryItems[i];j++)
				{
					if(m_ppItems[i][j])
					{
						if( (PXFlowObjectItem)(m_ppItems[i][j])->m_StrLabel)
							delete (PXFlowObjectItem)(m_ppItems[i][j])->m_StrLabel;
						delete m_ppItems[i][j];
					}
				}
				delete m_ppItems[i];
			}
		}
		if(m_ppItems)delete[] m_ppItems;
		if(m_nCategoryItems) delete[] m_nCategoryItems;
		if(m_pLimits) delete[] m_pLimits;
		if(m_pCaption)delete m_pCaption;
		if(m_pPloys)delete[] m_pPloys;
	}

	void XFlowObject::DrawBackGround(HDC hPaintDC,RECT rect)
	{
		TRIVERTEX	vertex[2];
		GRADIENT_RECT grect[1];

		vertex[0].x = rect.left ;
		vertex[0].y = rect.top ;
		vertex[0].Red  = GetRValue(m_startColor)*256;
		vertex[0].Green = GetGValue(m_startColor)*256;
		vertex[0].Blue = GetBValue(m_startColor)*256;

		vertex[1].x = rect.right;
		vertex[1].y = rect.bottom;
		vertex[1].Red= GetRValue(m_endColor)*256;
		vertex[1].Green =GetGValue(m_endColor)*256;
		vertex[1].Blue = GetBValue(m_endColor)*256;

		grect[0].UpperLeft = 0;
		grect[0].LowerRight = 1;

		GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
	}

	void XFlowObject::DoDraw(HDC hPaintDC)
	{
		HRGN	hRgn;
		RECT rect = FRect;
		RECT rect0;

		switch(m_shape)  
		{
		case roundrect:
			RoundRect(hPaintDC,rect.left,rect.top,rect.right,rect.bottom,15,15);
			hRgn = ::CreateRoundRectRgn(rect.left + 1,rect.top + 1,rect.right,rect.bottom,15,15);
			SelectClipRgn(hPaintDC,hRgn);
			DrawBackGround(hPaintDC,rect);
			SelectClipRgn(hPaintDC,NULL);
			::DeleteObject(hRgn);
			break;
		case ellipserect:
			Ellipse(hPaintDC,rect.left,rect.top,rect.right,rect.bottom);
			hRgn = ::CreateEllipticRgn(rect.left + 1,rect.top + 1,rect.right,rect.bottom);
			SelectClipRgn(hPaintDC,hRgn);
			DrawBackGround(hPaintDC,rect);
			SelectClipRgn(hPaintDC,NULL);
			::DeleteObject(hRgn);
			break;
		case flowstart:
		case flowend:
			Ellipse(hPaintDC,rect.left,rect.top,rect.right,rect.bottom);
			hRgn = ::CreateEllipticRgn(rect.left + 1,rect.top + 1,rect.right,rect.bottom);
			SelectClipRgn(hPaintDC,hRgn);
			DrawBackGround(hPaintDC,rect);
			SelectClipRgn(hPaintDC,NULL);
			::DeleteObject(hRgn);
			break;
		case bezierrect:
			::BeginPath(hPaintDC);
			::MoveToEx(hPaintDC,rect.left,rect.bottom - 17,NULL);
			::LineTo(hPaintDC,rect.left,rect.top);
			::LineTo(hPaintDC,rect.right,rect.top);
			::LineTo(hPaintDC,rect.right,rect.bottom - 17);

			POINT points[3];
			points[0].x = rect.left + (rect.right - rect.left)*2/3;
			points[0].y = rect.bottom -17 - 40;
			points[1].x = rect.left + (rect.right - rect.left)*1/3;
			points[1].y = rect.bottom -17 + 40;
			points[2].x = rect.left;
			points[2].y = rect.bottom - 17;
			PolyBezierTo(hPaintDC,points,3);
			::EndPath(hPaintDC);
			hRgn = PathToRegion(hPaintDC);

			SelectClipRgn(hPaintDC,hRgn);
			rect0 = rect;
			InflateRect(&rect0,-1,-1);
			DrawBackGround(hPaintDC,rect0);
			SelectClipRgn(hPaintDC,NULL);
			::DeleteObject(hRgn);

			::MoveToEx(hPaintDC,rect.left,rect.bottom - 17,NULL);
			::LineTo(hPaintDC,rect.left,rect.top);
			::LineTo(hPaintDC,rect.right,rect.top);
			::LineTo(hPaintDC,rect.right,rect.bottom - 17);
			PolyBezierTo(hPaintDC,points,3);

			break;
		case bezierrects:
			rect0 = rect;
			rect0.top -= 12;
			rect0.left += 36;
			rect0.bottom -= 36;
			hRgn = ::CreateRectRgn(0,0,0,0);
			for(int i=0;i<3;i++)
			{
				::OffsetRect(&rect0,-12,12);
				::BeginPath(hPaintDC);
				::MoveToEx(hPaintDC,rect0.left,rect0.bottom - 17,NULL);
				::LineTo(hPaintDC,rect0.left,rect0.top);
				::LineTo(hPaintDC,rect0.right,rect0.top);
				::LineTo(hPaintDC,rect0.right,rect0.bottom - 17);

				POINT points[3];
				points[0].x = rect0.left + (rect0.right - rect0.left)*2/3;
				points[0].y = rect0.bottom -17 - 40;
				points[1].x = rect0.left + (rect0.right - rect0.left)*1/3;
				points[1].y = rect0.bottom -17 + 40;
				points[2].x = rect0.left;
				points[2].y = rect0.bottom - 17;
				PolyBezierTo(hPaintDC,points,3);
				::EndPath(hPaintDC);

				hRgn = PathToRegion(hPaintDC);

				SelectClipRgn(hPaintDC,hRgn);
				InflateRect(&rect0,-1,-1);
				DrawBackGround(hPaintDC,rect0);
				SelectClipRgn(hPaintDC,NULL);
				::DeleteObject(hRgn);

				::MoveToEx(hPaintDC,rect0.left,rect0.bottom - 17,NULL);
				::LineTo(hPaintDC,rect0.left,rect0.top);
				::LineTo(hPaintDC,rect0.right,rect0.top);
				::LineTo(hPaintDC,rect0.right,rect0.bottom - 17);
				PolyBezierTo(hPaintDC,points,3);
			}
			break;
		default:
			::Rectangle(hPaintDC,rect.left,rect.top,rect.right,rect.bottom);
			hRgn = ::CreateRectRgn(rect.left + 1,rect.top + 1,rect.right,rect.bottom);
			SelectClipRgn(hPaintDC,hRgn);
			DrawBackGround(hPaintDC,rect);
			SelectClipRgn(hPaintDC,NULL);
			::DeleteObject(hRgn);
			break;
		}

		RECT	dwrect = FRect;
		if(m_shape == bezierrects) 
		{
			dwrect.top += 24;
			dwrect.right -= 24;
		}

		int startx = dwrect.left + 5;
		int	starty = dwrect.top;
		RECT drect;
		bool	bBreak = false;
		int bkMode = ::SetBkMode(hPaintDC,1);
		for(int i=0;i< m_nCategory;i++)
		{
			for(int j=0;j< m_nCategoryItems[i];j++)
			{
				if(starty + m_nRowHeight > dwrect.bottom)
				{
					bBreak = true;
					break;
				}
				::SetRect(&drect,startx + 2,starty + 2,dwrect.right - 2,starty + m_nRowHeight -2 );
				::DrawText(hPaintDC,m_ppItems[i][j]->m_StrLabel,(int)_tcslen(m_ppItems[i][j]->m_StrLabel), &drect,1);
				starty += m_nRowHeight;
			}
			if(bBreak) break;
		}

		rect = FRect;
		HBRUSH hBrush = ::CreateSolidBrush(RGB(0,255,0));
		hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
		if(this->m_Status & XFOSELECTED)
		{
			int swidth = (rect.right - rect.left + 1)/2;
			int sheight = (rect.bottom - rect.top + 1)/2;

			::Rectangle(hPaintDC,rect.left - 3,rect.top - 3,rect.left + 3,rect.top + 3);
			::Rectangle(hPaintDC,rect.right - 3,rect.top - 3,rect.right + 3,rect.top + 3);
			::Rectangle(hPaintDC,rect.right - 3,rect.bottom - 3,rect.right + 3,rect.bottom + 3);
			::Rectangle(hPaintDC,rect.left - 3,rect.bottom - 3,rect.left + 3,rect.bottom + 3);

			::Rectangle(hPaintDC,rect.left - 3 + swidth,rect.top - 3,rect.left + 3 + swidth,rect.top + 3);
			::Rectangle(hPaintDC,rect.right - 3,rect.top - 3 + sheight,rect.right + 3,rect.top + 3 + sheight);
			::Rectangle(hPaintDC,rect.left - 3 + swidth,rect.bottom - 3,rect.left + 3 + swidth,rect.bottom + 3);
			::Rectangle(hPaintDC,rect.left -3 ,rect.top - 3 +sheight,rect.left +3 ,rect.top + 3 +sheight);
		}else
		{
			int swidth = (rect.right - rect.left + 1)/2;
			int sheight = (rect.bottom - rect.top + 1)/2;
			HPEN hPen = ::CreatePen(PS_DASH,1,RGB(0,0,255));
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);

			RECT	rect1 = rect;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1.left = rect.right;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1.top = rect.bottom;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1 = rect;
			rect1.top = rect.bottom;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1 = rect;
			rect1.left += swidth;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1.left = rect.right;
			rect1.top += sheight;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1.top = rect.bottom;
			rect1.left =rect.left +  swidth;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			rect1 = rect;
			rect1.top = rect.top + sheight;
			::MoveToEx(hPaintDC,rect1.left - 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left + 3,rect1.top + 3);
			::MoveToEx(hPaintDC,rect1.left + 2,rect1.top - 2,NULL);
			::LineTo(hPaintDC, rect1.left - 3,rect1.top + 3);

			hPen = (HPEN)::SelectObject(hPaintDC,hPen);
			::DeleteObject(hPen);


		}
		hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
		::DeleteObject(hBrush);

		::SetBkMode(hPaintDC,bkMode);

	}

	int XFlowObject::AddItem(int nCategory, PXFlowObjectItem pItem)
	{
		if(nCategory >= m_nCategory) return -1;
		if(m_nCategoryItems[nCategory] >= m_pLimits[nCategory])
		{
			PXFlowObjectItem * pTemp;
			m_pLimits[nCategory] += m_nFlowObjectIns;
			pTemp = new PXFlowObjectItem[m_pLimits[nCategory]];
			if(m_ppItems[nCategory])
			{
				::memcpy(pTemp,m_ppItems[nCategory],sizeof(PXFlowObjectItem)*m_nCategoryItems[nCategory]);
				::delete[] m_ppItems[nCategory];
			}
			m_ppItems[nCategory] = pTemp;
		}
		m_nCategoryItems[nCategory]++;
		m_ppItems[nCategory][m_nCategoryItems[nCategory] - 1]=pItem;
		return -1;
	}
	void XFlowObject::RemoveItem(int nCategory,int nItem)
	{
		if(nCategory >= m_nCategory) return;
		if(nItem < 0 || nItem >= m_nCategoryItems[nCategory]) return;
		if(m_ppItems[nCategory][nItem])
		{
			if(m_ppItems[nCategory][nItem]->m_StrLabel) delete m_ppItems[nCategory][nItem]->m_StrLabel;
		}
		for(int i=nItem;i<m_nCategoryItems[nCategory] - 1;i++)
		{
			m_ppItems[nCategory][i] = m_ppItems[nCategory][i + 1];
		}
		m_nCategoryItems[nCategory]--;
	}

	int XFlowObject::AddCategory()
	{
		if(	m_nCategory >= m_nLimit)
		{
			m_nLimit += m_nFlowObjectIns;
			PXFlowObjectCategory * pTemp = new PXFlowObjectCategory[m_nLimit];
			if(m_ppItems  )
			{
				::memcpy(pTemp,this->m_ppItems,this->m_nCategory*sizeof(PXFlowObjectCategory));
				delete m_ppItems;
			}
			m_ppItems = pTemp;
			int * pTemp1 = new int[m_nLimit];
			if(m_nCategoryItems )
			{
				::memcpy(pTemp1,m_nCategoryItems,m_nCategory*sizeof(int));
				delete[] m_nCategoryItems;
			}
			m_nCategoryItems = pTemp1;

			int * pTemp2 = new int[m_nLimit];
			if(m_pLimits )
			{
				::memcpy(pTemp2,m_pLimits,m_nCategory*sizeof(int));
				delete[] m_pLimits;
			}
			m_pLimits = pTemp2;
		}
		m_nCategory++;
		m_ppItems[m_nCategory - 1] = NULL;
		m_nCategoryItems[m_nCategory - 1] = 0;
		m_pLimits[m_nCategory - 1] = 0;
		return m_nCategory - 1;
	}
	void XFlowObject::RemoveCategory(int nCategory)
	{
		if(nCategory <0 || nCategory >= this->m_nCategory) return ;
		for(int i=nCategory;i<m_nCategory - 1;i++)
		{
			m_nCategoryItems[i] = m_nCategoryItems[i++];
		}
		m_nCategory--;
	}
	void XFlowObject::InvalidatePatRgn(HWND hWnd)
	{
		HRGN	hRgn = ::CreateRectRgn(0,0,0,0);
		HRGN	hRgnEx;
		RECT	rect;

		rect = FRect;
		int swidth = (rect.right - rect.left + 1)/2;
		int sheight = (rect.bottom - rect.top + 1)/2;

		::SetRect(&rect,FRect.left - 3 ,FRect.top - 3,
			FRect.left + 3, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.top - 3,
			FRect.right + 3, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.bottom - 3,
			FRect.right + 3, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 ,FRect.bottom - 3,
			FRect.left + 3, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);


		::SetRect(&rect,FRect.left - 3 + swidth,FRect.top - 3,
			FRect.left + 3+swidth, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.top - 3 + sheight,
			FRect.right + 3, FRect.top + 3+ sheight);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 + swidth ,FRect.bottom - 3,
			FRect.left + 3 + swidth, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 ,FRect.top - 3 + sheight,
			FRect.left + 3, FRect.top + 3 +  sheight);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::InvalidateRgn(hWnd,hRgn,false);
		::DeleteObject(hRgn);
	}

	void XFlowObject::HitAdjust(XFOHitPosition htpos,int cx,int cy)
	{
		if(htpos==XFOHITLEFTTOP)
		{
			this->FRect.left += cx;
			this->FRect.top += cy;
		}

		if(htpos==XFOHITRIGHTTOP)
		{
			this->FRect.right += cx;
			this->FRect.top += cy;
		}
		if(htpos==XFOHITRIGHTBOTTOM)
		{
			this->FRect.right += cx;
			this->FRect.bottom += cy;
		}
		if(htpos==XFOHITLEFTBOTTOM)
		{
			this->FRect.left += cx;
			this->FRect.bottom += cy;
		}

		if(htpos==XFOHITTOPCENTER)
		{
			this->FRect.top += cy;
		}
		if(htpos==XFOHITRIGHTCENTER)
		{
			this->FRect.right += cx;
		}
		if(htpos==XFOHITBOTTOMCENTER)
		{
			this->FRect.bottom += cy;
		}
		if(htpos==XFOHITLEFTCENTER)
		{
			this->FRect.left += cx;
		}
	}

	XFOHitPosition XFlowObject::GetHitResizePat(POINT pt)
	{
		int swidth = (FRect.right - FRect.left + 1)/2;
		int sheight = (FRect.bottom - FRect.top + 1)/2;

		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITLEFTTOP;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITRIGHTTOP;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITRIGHTBOTTOM;
		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITLEFTBOTTOM;

		if(pt.x>=FRect.left - 2 + swidth && pt.x<= FRect.left + 2 +swidth && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITTOPCENTER;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.top - 2 + sheight && pt.y<= FRect.top + 2 + sheight) return XFOHITRIGHTCENTER;
		if(pt.x>=FRect.left - 2 + swidth && pt.x<= FRect.left + 2 + swidth && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITBOTTOMCENTER;
		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.top - 2 +sheight&& pt.y<= FRect.top + 2 +sheight) return XFOHITLEFTCENTER;

		return XFOHITNONE;
	}

	XFlowbase::XFlowbase()
	{
		::SetRect(&FRect,0,0,0,0);
		m_nObjectCount = 0;
		m_nObjectLinkCount = 0;
		m_ppXFlowObjects = NULL;
		m_ppXFlowObjectLinks = NULL;

		m_nObjectLement = 0;
		m_nObjectLinkLement = 0;
		m_nObjectIns = 32;
		m_status = XFONORMAL;
	}

	XFlowbase::~XFlowbase()
	{
		for(int i=0;i< m_nObjectCount;i++)
		{
			if(m_ppXFlowObjects[i])delete m_ppXFlowObjects[i];
		}
		if(m_ppXFlowObjects)delete m_ppXFlowObjects;

		for(int i=0;i< m_nObjectLinkCount;i++)
		{
			if(m_ppXFlowObjectLinks[i])delete m_ppXFlowObjectLinks[i];
		}
		if(m_ppXFlowObjectLinks)delete m_ppXFlowObjectLinks;

	}

	void XFlowbase::OnLButtonDown(UINT nFlags,POINT point)
	{
		XFOHitPosition htPos = XFOHITNONE;
		for(int i=0;i< m_nObjectCount;i++)
		{
			if(m_ppXFlowObjects[i]->m_Status & XFOSELECTED)
			{
				htPos = m_ppXFlowObjects[i]->GetHitResizePat(m_ptHittest);
				if(htPos!=XFOHITNONE)
				{
					m_nHitPos = htPos;
					this->m_pHitObject = m_ppXFlowObjects[i];
					break;
				}
			}
		}
		if(htPos!=XFOHITNONE)
		{
			m_status = XFORESIZING;

			this->m_ptMouse = point;
		}else
		{
			if(!(MK_CONTROL & nFlags))
			{
				for(int i = m_nObjectCount - 1;i>=0; i--)
				{
					if(m_ppXFlowObjects[i]->m_Status & XFOSELECTED)
					{
						m_ppXFlowObjects[i]->m_Status = m_ppXFlowObjects[i]->m_Status & ~XFOSELECTED;
						m_ppXFlowObjects[i]->InvalidatePatRgn(m_pSheetCtrl->m_hWnd);
					}
					m_ppXFlowObjects[i]->m_Status = XFONORMAL;
				}
			}
			for(int i = m_nObjectCount - 1;i>=0; i--)
			{
				if(PtInRect(&m_ppXFlowObjects[i]->FRect,point))
				{
					m_ppXFlowObjects[i]->m_Status |= XFOSELECTED;
					m_ppXFlowObjects[i]->InvalidatePatRgn(m_pSheetCtrl->m_hWnd);
					this->m_ptMouse = point;
					break;
				}
			}
			m_status = XFOMOVING;
		}
	}
	void XFlowbase::OnLButtonUp(UINT nFlags,POINT point)
	{
		OnMouseMove(nFlags,point);
		m_status = XFONORMAL;
	}
	void XFlowbase::OnMouseMove(UINT nFlags,POINT point)
	{
		HRGN	hRgn,hRgn0,hRgn1;
		RECT	rect;
		RECT	rectex;
		if(m_status == XFOMOVING)
		{
			hRgn = ::CreateRectRgn(0,0,0,0);
			for(int i=0;i< m_nObjectCount;i++)
			{
				if(m_ppXFlowObjects[i]->m_Status & XFOSELECTED)
				{
					rect = m_ppXFlowObjects[i]->FRect;
					rectex = rect;
					InflateRect(&rectex,3,3);
					hRgn0 = ::CreateRectRgnIndirect(&rectex);
					::OffsetRect(&rect,point.x - m_ptMouse.x,point.y - m_ptMouse.y);
					m_ppXFlowObjects[i]->FRect = rect;
					rectex = rect;
					InflateRect(&rectex,3,3);
					hRgn1 = ::CreateRectRgnIndirect(&rectex);
					::CombineRgn(hRgn0,hRgn0,hRgn1,RGN_OR);
					::CombineRgn(hRgn,hRgn,hRgn0,RGN_OR);
					::DeleteObject(hRgn0);
					::DeleteObject(hRgn1);
					m_ptMouse = point;
				}
			}
			::InvalidateRgn(m_pSheetCtrl->m_hWnd,hRgn,false);
			::DeleteObject(hRgn);
		}
		if(m_status == XFORESIZING)
		{
			hRgn = ::CreateRectRgn(0,0,0,0);
			rect = m_pHitObject->FRect;
			rectex = rect;
			InflateRect(&rectex,3,3);
			hRgn0 = ::CreateRectRgnIndirect(&rectex);

			m_pHitObject->HitAdjust(this->m_nHitPos,point.x - m_ptMouse.x,point.y - m_ptMouse.y);
			rect = m_pHitObject->FRect;
			rectex = rect;
			InflateRect(&rectex,3,3);
			hRgn1 = ::CreateRectRgnIndirect(&rectex);
			::CombineRgn(hRgn0,hRgn0,hRgn1,RGN_OR);
			::CombineRgn(hRgn,hRgn,hRgn0,RGN_OR);
			::DeleteObject(hRgn0);
			::DeleteObject(hRgn1);
			m_ptMouse = point;

			::InvalidateRgn(m_pSheetCtrl->m_hWnd,hRgn,false);
			::DeleteObject(hRgn);
		}
	}

	BOOL XFlowbase::OnSetCursor( HWND hWnd, UINT nHitTest, UINT message )
	{
		HCURSOR Cur = NULL;
		if (hWnd == this->m_pSheetCtrl->m_hWnd && nHitTest==HTCLIENT )
		{
			XFOHitPosition htPos = XFOHITNONE;
			if(this->m_status & XFORESIZING)
				htPos = this->m_nHitPos;
			else
			{
				for(int i=0;i< m_nObjectCount;i++)
				{
					if(m_ppXFlowObjects[i]->m_Status & XFOSELECTED)
					{
						htPos = m_ppXFlowObjects[i]->GetHitResizePat(m_ptHittest);
						if(htPos!=XFOHITNONE)break;
					}
				}
			}
			if(htPos!=XFOHITNONE)
			{
				if(htPos==XFOHITLEFTTOP || htPos==XFOHITRIGHTBOTTOM)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENWSE));//crVSplit
				if(htPos==XFOHITLEFTBOTTOM || htPos==XFOHITRIGHTTOP)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENESW));//crVSplit
				if(htPos==XFOHITLEFTCENTER || htPos==XFOHITRIGHTCENTER)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crVSplit
				if(htPos==XFOHITBOTTOMCENTER || htPos==XFOHITTOPCENTER)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crVSplit
			}
		}
		if(Cur != NULL)
		{
			SetCursor(Cur);
			return TRUE;
		}
		return false;
	}

	int XFlowbase::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		RECT rc;
		if(pDrawRect)
		{
			rc = *pDrawRect;
			int nSaveDC = ::SaveDC(hPaintDC);
			SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
			::IntersectClipRect(hPaintDC,1,1,
				pDrawRect->right - pDrawRect->left - 1 ,
				pDrawRect->bottom - pDrawRect->top -1);
			SetMetaRgn(hPaintDC);

			for(int i=0;i< m_nObjectCount;i++)
			{
				if(m_ppXFlowObjects[i])m_ppXFlowObjects[i]->DoDraw(hPaintDC);
			}

			for(int i=0;i< m_nObjectLinkCount;i++)
			{
				if(m_ppXFlowObjectLinks[i])m_ppXFlowObjectLinks[i]->DoDraw(hPaintDC);
			}

			::RestoreDC(hPaintDC,nSaveDC);
		}
		else
		{
			::GetClientRect(this->m_pSheetCtrl->m_hWnd,&rc);
			for(int i=0;i< m_nObjectCount;i++)
			{
				if(m_ppXFlowObjects[i])m_ppXFlowObjects[i]->DoDraw(hPaintDC);
			}

			for(int i=0;i< m_nObjectLinkCount;i++)
			{
				if(m_ppXFlowObjectLinks[i])m_ppXFlowObjectLinks[i]->DoDraw(hPaintDC);
			}
		}
		return 0;
	}

	int  XFlowbase::AddFlowObject(XFlowObject*  pFlowObject)
	{
		if(m_nObjectCount==m_nObjectLement)
		{
			m_nObjectLement += m_nObjectIns;
			XFlowObject** ppTemp = new XFlowObject *[m_nObjectLement];
			if(m_ppXFlowObjects)
			{
				::memcpy(ppTemp,m_ppXFlowObjects,m_nObjectCount * sizeof(XFlowObject *));
				delete m_ppXFlowObjects;
			}
			m_ppXFlowObjects = ppTemp;
		}
		m_nObjectCount++;
		m_ppXFlowObjects[ m_nObjectCount -1] = pFlowObject;
		return m_nObjectCount -	1;
	}

	void XFlowbase::RemoveFlowObject(int nPos)
	{
		if(nPos <0 || nPos > m_nObjectCount) return;
		if(m_ppXFlowObjects[nPos]) delete m_ppXFlowObjects[nPos];
		for(int i=nPos;i<m_nObjectCount - 1;i++)
		{
			m_ppXFlowObjects[i] = m_ppXFlowObjects[i + 1];
		}
	}

	int XFlowbase::AddFlowObjectLink(XFlowObjectLink * pFlowObjectLink)
	{
		if(m_nObjectLinkCount==m_nObjectLinkLement)
		{
			m_nObjectLinkLement += m_nObjectIns;
			XFlowObjectLink** ppTemp = new XFlowObjectLink *[m_nObjectLinkLement];
			if(m_ppXFlowObjectLinks)
			{
				::memcpy(ppTemp,m_ppXFlowObjectLinks,m_nObjectCount * sizeof(XFlowObjectLink *));
				delete m_ppXFlowObjectLinks;
			}
			m_ppXFlowObjectLinks = ppTemp;
		}
		m_nObjectLinkCount++;
		m_ppXFlowObjectLinks[ m_nObjectLinkCount -1] = pFlowObjectLink;
		return m_nObjectLinkCount -	1;
	}

	void XFlowbase::RemoveFlowObjectLink(int nPos)
	{
		if(nPos <0 || nPos > m_nObjectLinkCount) return;
		if(m_ppXFlowObjectLinks[nPos]) delete m_ppXFlowObjectLinks[nPos];
		for(int i=nPos;i<m_nObjectLinkCount - 1;i++)
		{
			m_ppXFlowObjectLinks[i] = m_ppXFlowObjectLinks[i + 1];
		}
	}

	int XFlowbase::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
	{
		switch (message) 
		{ 
		case WM_CHAR:
			{
				UINT nChar;
				UINT nRepCnt;
				UINT nFlags ;

				nChar = (UINT)wParam;
				nRepCnt =(SHORT) LOWORD(lParam);
				nFlags = (UINT)lParam;
				OnChar(nChar,nRepCnt,nFlags);
				break;
			}
		case WM_SIZE:
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				OnSize((UINT)wParam,point);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;
				this->OnLButtonDown((UINT)dwFlag,point);
				break;
			}
		case WM_LBUTTONUP:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;
				this->OnLButtonUp((UINT)dwFlag,point);
				break;
			}
		case WM_MOUSEMOVE:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;

				this->OnMouseMove((UINT)dwFlag,point);
				break;
			}	
		case  WM_NCHITTEST:
			{
				POINT	pt;
				pt.x = MAKEPOINTS(lParam).x;
				pt.y = MAKEPOINTS(lParam).y;
				ScreenToClient(m_pSheetCtrl->m_hWnd,&pt);
				m_ptHittest = pt;
				break;
			}
		case WM_SETCURSOR:
			{
				HWND	hWnd = (HWND)wParam;
				UINT	nHitTest = LOWORD(lParam);
				UINT	mouseMsg = HIWORD(lParam);
				return OnSetCursor(hWnd,nHitTest,mouseMsg);
			}
		}
		return 0;
	}
	void XFlowbase::OnChar( UINT nChar,UINT nRepCnt,UINT nFlags)
	{
		
	}
	int XFlowbase::OnSize(UINT nFlags, POINT point)
	{
		return 0;
	}
}}}}
