#include "StdAfx.h"
#include "..\include\XDFPagesSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	tagDrawObjectInfo infos[]=
	{
		{_T("control"),_control},
		{_T("image"),_image},
		{_T("line"),_line},
		{_T("rect"),_rect},
		{_T("round"),_round},
		{_T("text"),_text},
		{_T("shape"),_shape}
	};

	eXDFDrawObject FindDrawObject(LPTSTR pstrItem)
	{
		for(int i=0;i<sizeof(infos)/sizeof(infos[0]);i++)
		{
			if(_tcsicmp(infos[i].pStrItemName,pstrItem)==0) return infos[i].eItem;
		}
		return _none;
	}

	tagShape* FindShapeByName(XDFPageItem* pItem,LPTSTR pstrName)
	{
		for(int i=0;i<pItem->m_Shapes.size();i++)
		{
			if(::_tcsicmp(pItem->m_Shapes[i]->name,pstrName)==0) return pItem->m_Shapes[i];
		}
		for(int i=0;i<pItem->m_pXDFPagesSvr->m_Shapes.size();i++)
		{
			if(::_tcsicmp(pItem->m_pXDFPagesSvr->m_Shapes[i]->name,pstrName)==0) return pItem->m_pXDFPagesSvr->m_Shapes[i];
		}
		return NULL;
	}

	tagShape* FindShapeByID(XDFPageItem* pItem,int ShapeID)
	{
		for(int i=0;i<pItem->m_Shapes.size();i++)
		{
			if(pItem->m_Shapes[i]->id==ShapeID) return pItem->m_Shapes[i];
		}
		for(int i=0;i<pItem->m_pXDFPagesSvr->m_Shapes.size();i++)
		{
			if(pItem->m_pXDFPagesSvr->m_Shapes[i]->id==ShapeID) return pItem->m_pXDFPagesSvr->m_Shapes[i];
		}
		return NULL;
	}


	int GetExtent(XUtil::Css::TMeasure &mes,int extent,bool bHorz)
	{
		if(mes.measureRef==XUtil::Css::_percentage)
			return (int)(extent*mes.percentage/100.0);
		else
			return  (int)XUtil::Css::GetPixelExtent(&mes,bHorz); 
		return 0;
	}
	int XDFDrawObject::DoDraw(HDC hPaintDC,RECT rtPage,XOfficeStyleSvr * pStyleSvr,XDFPageItem * pItem)
	{
		int x0,y0;
		int x1,y1;

		int nStartX = GetExtent(startx,rtPage.right - rtPage.left,true);
		int nStartY = GetExtent(starty,rtPage.bottom - rtPage.top,false);
		int nEndX = GetExtent(endx,rtPage.right - rtPage.left,true);
		int nEndY = GetExtent(endy,rtPage.bottom - rtPage.top,false);
		int nWidth = GetExtent(width,rtPage.right - rtPage.left,true);
		int nHeight = GetExtent(height,rtPage.bottom - rtPage.top,false);
		
		x0 = nStartX >= 0?nStartX + rtPage.left : rtPage.right + nStartX;
		y0 = nStartY >= 0?nStartY + rtPage.top : rtPage.bottom + nStartY;
		x1 = nEndX >= 0?nEndX + rtPage.left : rtPage.right + nEndX;
		y1 = nEndY >= 0?nEndY + rtPage.top : rtPage.bottom + nEndY;

		switch(this->eObject)
		{
		case _line:
			{
				HPEN hPen = ::CreatePen(PS_SOLID,nWidth,RGB(0,0,0));
				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				
				::MoveToEx(hPaintDC,x0,y0,NULL);
				::LineTo(hPaintDC,x1,y1);

				hPen = (HPEN)SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
			break;
		case _rect:
			{
				::Rectangle(hPaintDC,x0,y0,x1,y1);
			}
			break;
		case _round:
			{
				::RoundRect(hPaintDC,x0,y0,x1,y1,5,5);
			}
			break;
		case _text:
			{
				RECT rect;
				::SetRect(&rect,x0,y0,x1,y1);
				::DrawText(hPaintDC,this->pStrText,(int)::_tcslen(pStrText),&rect,DT_WORDBREAK);
			}
			break;
		case _shape:
			{
				if(!pItem)break;
				tagShape *pShape;
				if(this->refname)
					pShape = FindShapeByName(pItem,this->refname);
				else if(this->refid)
					pShape = FindShapeByID(pItem,this->refid);
				if(pShape)
				{
					RECT rect;
					::SetRect(&rect,x0,y0,x1,y1);
					for(int j=0;j<pShape->m_DrawObjs.size();j++)
					{
						RECT rect;
						::SetRect(&rect,x0,y0,x1,y1);
						pShape->m_DrawObjs[j]->DoDraw(hPaintDC,rect,pStyleSvr,pItem);
					}
				}

			}
		}
		return 0;
	}


	XDFPagesSvr::XDFPagesSvr(void):m_nPageBarWidth(100),m_curPage(0)
	{
	}

	XDFPagesSvr::~XDFPagesSvr(void)
	{
		for(int i=0;i<m_Pages.size();i++)
		{
			if(m_Pages[i]) delete m_Pages[i]; 
		}
	}
}}}}
