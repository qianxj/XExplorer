#include "StdAfx.h"
#include "xtabsheet.hpp"
#include "xnavigate.hpp"
#include "xsheetmaint.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	CXNavigate::CXNavigate(void):m_hWnd(NULL),m_pTabSheet(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
		SetEventListons();
	}

	CXNavigate::~CXNavigate(void)
	{
		UnSetEventListons();
	}

	bool CXNavigate::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		OnDraw(pEvent->paint.hdc);
		return true;
	}
	int CXNavigate::OnDraw(HDC hdc)
	{
		int xpos,ypos;
		RECT rect;
		HBRUSH hbrush;
		POINT	points[3];
		int	height;

		rect = this->FRect;
		xpos = rect.left + 3;
		ypos = rect.top;
		height = rect.bottom - rect.top;
		hbrush = ::GetSysColorBrush(COLOR_WINDOWTEXT);
		hbrush = (HBRUSH)::SelectObject(hdc, hbrush);

		::Rectangle(hdc,xpos,ypos + 2, xpos + 2,rect.bottom - 4);xpos+=3;

		points[0].x = xpos;
		points[0].y = rect.bottom - height /2 - 1;
		points[1].x = xpos + 4;
		points[1].y = rect.bottom - height /2 - 4 - 1;
		points[2].x = xpos + 4;
		points[2].y = rect.bottom - height /2 + 4 - 1;
		::Polygon(hdc,points,3) ;

		xpos = rect.left + height;
		xpos = xpos + height/2;
		ypos = rect.bottom - height;
		ypos = ypos + height/2;

		points[0].x = xpos - 2;
		points[0].y = ypos;
		points[1].x = xpos + 2;
		points[1].y = ypos - 4;
		points[2].x = xpos + 2;
		points[2].y = ypos + 4;
		::Polygon(hdc,points,3) ;

		xpos = xpos + height;
		points[0].x = xpos + 2;
		points[0].y = ypos;
		points[1].x = xpos - 2;
		points[1].y = ypos - 4;
		points[2].x = xpos - 2;
		points[2].y = ypos + 4;
		::Polygon(hdc,points,3) ;

		xpos = xpos + height - 2;
		points[0].x = xpos + 2;
		points[0].y = ypos;
		points[1].x = xpos - 2;
		points[1].y = ypos - 4;
		points[2].x = xpos - 2;
		points[2].y = ypos + 4;
		::Polygon(hdc,points,3) ;
		::Rectangle(hdc,xpos + 4,ypos - 5 + 1 , xpos + 4 + 2,ypos + 5 + 1);
		return 1;
	}

	bool CXNavigate::EvtRButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		XSheetMaint* pPopupWin =  new XSheetMaint();
		ClientToScreen(m_hWnd,&point);
		pPopupWin->Create(point,(LPARAM)m_pTabSheet);

		return true;
	}

	bool CXNavigate::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		int xpos = point.x - FRect.left;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//First
			m_pTabSheet->m_iStartSheet=0;
			m_pTabSheet->Redraw();
			return true;
		}
		xpos -= FRect.bottom - FRect.top;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//Prev
			if(m_pTabSheet->m_iStartSheet > 0)
			{
				m_pTabSheet->m_iStartSheet -- ;
				m_pTabSheet->Redraw();
			}
			return true;
		}
		xpos -= FRect.bottom - FRect.top;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//Next
			if(m_pTabSheet->m_iStartSheet  < m_pTabSheet->m_SheetSize - 1)
			{
				m_pTabSheet->m_iStartSheet++;
				m_pTabSheet->Redraw();
			}
			return true;
	}
		xpos -= FRect.bottom - FRect.top;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//last
			m_pTabSheet->m_iStartSheet = m_pTabSheet->m_SheetSize - 1;
			m_pTabSheet->Redraw();
		}
		return true ;
	}
	bool CXNavigate::SetEventListons()
	{
		this->AttachListon(WM_PAINT,(FNEvent)&CXNavigate::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CXNavigate::EvtLButtonDown);
		this->AttachListon(WM_RBUTTONDOWN,(FNEvent)&CXNavigate::EvtRButtonDown);
		return true;
	}
	bool CXNavigate::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool CXNavigate::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		(*pRect).right = m_pTabSheet->FRect.right;
		return true;
	}
}}}
