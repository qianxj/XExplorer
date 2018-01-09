#include "StdAfx.h"
#include "XWordLayerDocument.hpp"
#include "XWordLayerChapter.hpp"
#include "XWordLayerPara.hpp"
#include "xtree.hpp"
#include "XWordPred.hpp"

XWordLayerDocument::XWordLayerDocument(void):_pageCount(-1)
{
}

XWordLayerDocument::~XWordLayerDocument(void)
{
}


int XWordLayerDocument::getPageCount()
{
	return _pageCount;
}

int XWordLayerDocument::setPageCount(int count,bool redraw)
{
	_pageCount = count;
}

SIZE XWordLayerDocument::GetExtent()
{
	SIZE sz = {0,0};
	vord_t block = get_block();
	if(block)
	{
		sz.cx = block[extent_c::cx];
		sz.cy = block[extent_c::cy];
	}
	return sz;
}
SIZE XWordLayerDocument::GetRealExtent()
{
	SIZE sz = {0,0};
	vord_t block = get_block();

	if(block)
	{
		vord_t item =  block.get_child();
		while(item)
		{
			sz.cx  = sz.cx > item[extent_c::cx] ? sz.cx : item[extent_c::cx];
			sz.cy += item[extent_c::cy];
			item = item.get_next();
		}
	}
	return sz;
}


//获取实际占用的高度和宽度信息
SIZE XWordLayerDocument::CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC)
{
	SIZE size = {0,0};
	vord_t block = pBlock;
	const Css::TBoxStyle * pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	if(!pBoxStyle)
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	XWordLayerBase* layer = 0;
	if(block)
	{
		vord_t item =  block.get_child();
		while(item)
		{
			if(item(block::type)== blocktype::paragraph)
				layer = new XWordLayerPara;
			else if(item(block::type)== blocktype::chapter)
				layer = new XWordLayerChapter;
			
			if(layer)
			{
				layer->set_block(item);
				layer->SetFRect(FRect);
				layer->SetContentRect(FRect);
				SIZE extent = layer->CalcExtent(pDataSvr,item,pBoxStyle,hDC);
				size.cy +=  extent.cy;
				delete layer;
			}
			item = item.get_next();
		}
	}
	block[extent_c::cy] = size.cy;
	block[extent_c::cx] = size.cx;
	return size;
}

int XWordLayerDocument::DoDraw(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect)
{
	vord_t doc = this->get_block();
	const Css::TBoxStyle * pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(doc[css::index] - 1);
	if(!pBoxStyle)
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	XWordLayerPara  para;
	XWordLayerChapter chapter;

	//int 
	XWordLayerBase* layer = 0;
	if(doc)
	{
		for(vord_t item = doc.get_child();item;item = item.get_next())
		{
			SIZE extent = {0,0};
			if(item(block::type)== blocktype::paragraph)
			{
				para.set_block(item);
				extent = para.GetExtent();
				if(pUpdateRect->top < extent.cy)
				{
					para.SetFRect(FRect);
					para.SetContentRect(FRect);
					para.DoDraw(pDataSvr,this,pParentBoxStyle,hPaintDC,ptOffset,pDrawRect,pUpdateRect);
				}
			}else if(item(block::type)== blocktype::chapter)
			{
				chapter.set_block(item);
				extent = chapter.GetExtent();
				if(pUpdateRect->top < extent.cy)
				{
					chapter.SetFRect(FRect);
					chapter.SetContentRect(FRect);
					chapter.DoDraw(pDataSvr,this,pParentBoxStyle,hPaintDC,ptOffset,pDrawRect,pUpdateRect);
				}
			}
			OffsetRect(pUpdateRect,0, - extent.cy);
			if(pUpdateRect->bottom < 0) break;
		}
	}
	
	return 1;
}
