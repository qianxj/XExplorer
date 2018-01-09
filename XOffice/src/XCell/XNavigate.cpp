#include "StdAfx.h"
#include "XNavigate.hpp"
#include "XTabsheet.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XCell
{
	CXNavigate::CXNavigate(void):Handle(NULL),m_pTabSheet(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
	}

	CXNavigate::~CXNavigate(void)
	{
	}
	
	int CXNavigate::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
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
			this->Handle = hWnd;

			POINT point;
			UINT_PTR	dwFlag;
			dwFlag = wParam;
			point.x = MAKEPOINTS(lParam).x;
			point.y = MAKEPOINTS(lParam).y;

			if(::PtInRect(&FRect,point))
				this->OnMouseMove((UINT)dwFlag,point);
		}
		return 0;
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
	void CXNavigate::OnLButtonDown(UINT nFlags,POINT point)
	{
		int xpos = point.x - FRect.left;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//First
			m_pTabSheet->m_iStartSheet=0;
			m_pTabSheet->Redraw();
			return ;
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
			return ;
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
			return ;
	}
		xpos -= FRect.bottom - FRect.top;
		if(xpos <= FRect.bottom - FRect.top)
		{
			//last
			m_pTabSheet->m_iStartSheet = m_pTabSheet->m_SheetSize - 1;
			m_pTabSheet->Redraw();
		}
		return ;
	}

	void CXNavigate::OnLButtonUp(UINT nFlags,POINT point)
	{
	}
	void CXNavigate::OnMouseMove(UINT nFlags,POINT point)
	{
	}

}}}
