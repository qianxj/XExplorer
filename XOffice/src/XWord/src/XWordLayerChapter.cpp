#include "StdAfx.h"
#include "..\include\XWordLayerChapter.hpp"
#include "XWordLayerDocument.hpp"
#include "XWordLayerPara.hpp"
#include "xtree.hpp"
#include "XWordPred.hpp"
#include "XWordDevice.hpp"
#include "XWCodeAsm.hpp"
#include "XWCodeInter.hpp"
#include "XWCodeContext.hpp"

using namespace Hxsoft::XFrame::XOffice;

XWordLayerChapter::XWordLayerChapter(void)
{
}

XWordLayerChapter::~XWordLayerChapter(void)
{
}


SIZE XWordLayerChapter::GetExtent()
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

SIZE XWordLayerChapter::GetRealExtent()
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

SIZE XWordLayerChapter::CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC)
{
	SIZE size = {0,0};
	vord_t block = pBlock;
	const Css::TBoxStyle * pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	if(!pBoxStyle)
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	XWordLayerPara layer;
	XWordDevice device;
	device.SetDC(hDC);
	XWCodeAsm wasm;
	XWCodeInter machine;
	XWCodeContentExtent context;
	int pc = 0;
	if(block)
	{
		vord_t item =  block.get_child();
		while(item)
		{
			context.rowHeights.clear();
			context.extent.cx = 0;
			context.extent.cy = 0;
			if(item(block::type)== blocktype::paragraph)
			{
				if(item[control::type])
				{
					RECT rt ={item[control::left],item[control::top],
						item[control::right],item[control::bottom]};
					if(!item[control::object])
					{
						xbObject *	pObject;
						Hxsoft::XFrame::xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,item[control::type],pObject);
						Hxsoft::XFrame::xfControl*  pxfControl = (Hxsoft::XFrame::xfControl *)pObject;
						if(pxfControl)
						{
							/*
							if(_tcsicmp(item[control::type],L"xcell")==0)
							{
								RECT rect;
								::SetRect(&rect,0,0,0,0);
								pxfControl->m_pxfNode= NULL;
								pxfControl->m_dwStyle &= ~WS_VISIBLE;
								pxfControl->CreateControl(NULL,rect,GetDesktopWindow(),0,xfApp::GetApp()->GetInstance());
								::SetProp(pxfControl->m_hWnd,_T("this"),pxfControl);

								XOfficeSheet * pSheet = dynamic_cast<XOffice::XOfficeSheet *>(pxfControl);
								if(pSheet)
								{
									pSheet->m_Standalone = true;
									_variant_t v;
									IXMLDOMElement * pElement = (IXMLDOMElement*)item[control::element];
									pElement->getAttribute(L"src",&v);
									if(v.bstrVal && v.bstrVal[0])
									{
										xbXml x;
										IXMLDOMElement * pObjElement = NULL;
										if(xfUriLoader::FetchXml(NULL,v.bstrVal,NULL,x)==1)
										{
											x.GetXmlDoc()->get_documentElement(&pObjElement);
											pSheet->m_pSheetSvr->LoadTemplate(pObjElement,goRunState);
											pObjElement->Release();
										}
									}else
									{
										pSheet->m_pSheetSvr->LoadTemplate(pElement,goRunState);
									}
								}
							}
							else*/ if(_tcsicmp(item[control::type],L"ximage")==0)
							{
								pxfControl->Initial((IXMLDOMElement*)item[control::element]);
							}
							else
								pxfControl->Initial();
							item[control::object] = pxfControl;
						}
					}
					SIZE sz = {0,0};
					if(_wcsicmp(item[control::type],L"xcell")!=0)
						sz = ((Hxsoft::XFrame::xfControl *)item[control::object])->GetContentExtent();
					else
						sz.cx = item[control::right] - item[control::left];
					if( item[control::bottom] - item[control::top] > 0)
						sz.cy = item[control::bottom] - item[control::top] +1;


					if(_wcsicmp(item[control::type],L"ximage")==0)
					{
						RECT rt ={item[control::left],item[control::top],
							item[control::right],item[control::bottom]};
						rt = GetImageBoundRect(sz,rt);
						if(rt.right > item[control::right])   item[control::right] = rt.right;
						if(rt.bottom > item[control::bottom]) item[control::bottom] = rt.bottom;
					}

					sz.cy = item[control::bottom];

					item[extent_c::cy] = sz.cy;
					item[extent_c::cx] = sz.cx;
					item[render::rowcount] = 1;
					size.cy +=  sz.cy + 6;
				}
				else
				{
					layer.set_block(item);
					layer.SetFRect(FRect);
					layer.SetContentRect(FRect);

					pc = (int) (wasm.pc - wasm.code);
					layer.CalcLayer(pDataSvr,(Css::TBoxStyle *)pBoxStyle,&wasm,&device);
					context.extent.cx = 0;
					context.extent.cy = 0;
					machine.Inter(&context,wasm.code + pc);
					SIZE extent = context.extent;

					if(extent.cy)
					{
						item[render::pc]  = pc;
						item[render::epc] = (int)(wasm.pc - wasm.code);
					}
					item[extent_c::cy] = extent.cy;
					item[extent_c::cx] = extent.cx;
					item[render::rowcount] = (int)context.rowHeights.size();
					size.cy +=  extent.cy + 6;
				}
			}
			item = item.get_next();
		}
	}
	block[extent_c::cy] = size.cy;
	block[extent_c::cx] = size.cx;
	block[render::code] = wasm.code;
	block[render::codelimit] = wasm.limit_;
	block[render::pc] = (int)(wasm.pc - wasm.code);

	return size;
}

int XWordLayerChapter::DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect)
{
	vord_t block = this->get_block(); 
	const Css::TBoxStyle * pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	if(!pBoxStyle)
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	XWordLayerPara para;
	POINT ptStart = ptOffset;

	XWordDevice device;
	device.SetPaintDC(hPaintDC);
	XWCodeInter machine;
	XWCodePaintContext context(&device);
	context.pDataSvr = pDataSvr;

	unsigned char * code = (unsigned char *)block[render::code];

	for(vord_t item =  block.get_child();item;item = item.get_next())
	{
		para.SetFRect(FRect);
		para.SetContentRect(FRect);
		::OffsetRect(&para.FRect,ptStart.x,ptStart.y);
		::OffsetRect(&para.FContentRect,ptStart.x,ptStart.y);
		
		context.ptPoint.x = para.FContentRect.left;
		context.ptPoint.y = para.FContentRect.top;

		if(item[block::type] == blocktype::paragraph)
		{
			para.set_block(item);
			if(pUpdateRect->top > para.GetExtent().cy) continue;
			if(item[control::type])
			{
				RECT rt ={item[control::left],item[control::top],
					item[control::right],item[control::bottom]};
				::OffsetRect(&rt,FContentRect.left,FContentRect.top);
				::OffsetRect(&rt,ptStart.x, ptStart.y);
				if(!item[control::object])
				{
					xbObject *	pObject;
					Hxsoft::XFrame::xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,item[control::type],pObject);
					Hxsoft::XFrame::xfControl*  pxfControl = (Hxsoft::XFrame::xfControl *)pObject;
					if(pxfControl)
					{
						/*RECT rect;
						::SetRect(&rect,0,0,0,0);
						pxfControl->m_pxfNode= NULL;
						pxfControl->m_dwStyle &= ~WS_VISIBLE;
						pxfControl->CreateControl(NULL,rect,GetDesktopWindow(),0,xfApp::GetApp()->GetInstance());
						::SetProp(pxfControl->m_hWnd,_T("this"),pxfControl);

						XOfficeSheet * pSheet = dynamic_cast<XOffice::XOfficeSheet *>(pxfControl);
						if(pSheet)
						{
							pSheet->m_Standalone = true;
							_variant_t v;
							IXMLDOMElement * pElement = (IXMLDOMElement*)pData->param;
							pElement->getAttribute(L"src",&v);
							if(v.bstrVal && v.bstrVal[0])
							{
								xbXml x;
								IXMLDOMElement * pObjElement = NULL;
								if(xfUriLoader::FetchXml(NULL,v.bstrVal,NULL,x)==1)
								{
									x.GetXmlDoc()->get_documentElement(&pObjElement);
									pSheet->m_pSheetSvr->LoadTemplate(pObjElement,goRunState);
									pObjElement->Release();
								}
							}else
							{
								pSheet->m_pSheetSvr->LoadTemplate(pElement,goRunState);
							}
						}*/

						if(_tcsicmp(item[control::type],L"ximage")==0)
						{
							pxfControl->Initial((IXMLDOMElement*)item[control::element]);
						}
						else
							pxfControl->Initial();

						item[control::object] = pxfControl;
					}
				}

				SIZE sz = ((Hxsoft::XFrame::xfControl *)item[control::object])->GetContentExtent();
				((Hxsoft::XFrame::xfControl*)item[control::object])->DoDraw(hPaintDC,&rt);
				if(rt.bottom - rt.top > 1)
					ptStart.y += rt.bottom - rt.top + 6;
				else
					ptStart.y += sz.cy + 6;
			}else
			{
				SIZE sz = para.GetExtent();
				//render
				int pc = item[render::pc];
				context.GetDevice()->rcContentPage = & FRect;
				machine.Inter(&context,code + pc);
				ptStart.y += sz.cy + 6;
			}
			OffsetRect(pUpdateRect,0, - para.GetExtent().cy);
			if(pUpdateRect->bottom < 0) break;
		}
	}
	return 1;
}

RECT XWordLayerChapter::GetImageBoundRect(SIZE sz,RECT &rt)
{
	RECT rcImage = GetImageRect(sz,rt);
	RECT rc = rt;
	if(rc.right - rc.left < rcImage.right - rcImage.left)
	{
		rc.left = rcImage.left;
		rc.right = rcImage.right;
	}
	if(rc.bottom - rc.top < rcImage.bottom - rcImage.top)
	{
		rc.top = rcImage.top;
		rc.bottom = rcImage.bottom;
	}
	return rc;
}

RECT XWordLayerChapter::GetImageRect(SIZE sz,RECT &rt)
{
	int nWidth  = sz.cx;
	int nHeight = sz.cy;

	RECT rc;
	if(rt.right  - rt.left < 1) rt.right = rt.left + nWidth;
	if(rt.bottom - rt.top < 1) rt.bottom = rt.top + nHeight;

	if(nWidth > 0)
	{
		if(1.0*(rt.right - rt.left)/nWidth*nHeight > rt.bottom - rt.top)
		{
			SetRect(&rc,rt.left,rt.top,
				rt.left + (int)1.0 *(rt.bottom - rt.top) * nWidth /nHeight ,
				rt.bottom);
		}else
			SetRect(&rc,rt.left,rt.top,rt.right ,
				rt.top + (int)1.0 *(rt.right - rt.left) * nHeight /nWidth);
	}
	return rc;
}

