#include "StdAfx.h"
#include "..\include\XFlowChapter.hpp"
#include "..\include\XFlowPara.hpp"
#include "XShape.h"
#include "xcell.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XFlowChapter::XFlowChapter():m_pContent(NULL)
	{
	}

	XFlowChapter::~XFlowChapter()
	{
	}

	//获取设定高度和宽度信息
	POINT XFlowChapter::GetExtent()
	{
		POINT pt={0,0};
		return pt;
	}
	//获取实际占用的高度和宽度信息
	POINT XFlowChapter::GetRealExtent()
	{
		POINT pt={0,0};
		return pt;
	}

	int XFlowChapter::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		return 1;
	}

	int XFlowChapter::DoDraw(XWordDataSvr* pDataSvr,XFlowDocument* pDocument,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset,RECT * pDrawRect,RECT * pUpdateRect)
	{
		if(!m_pContent) return -1;

		int		nHeight = 0;
		RECT	rt = FRect;
		POINT	ptStart ={0,0};
		XFlowPara *pPara = new XFlowPara;
		for(int i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			if((*m_pContent->m_pChilds)[i]->eWordDataType == _XWordDataControlParagraph)
			{
				RECT rt = FRect;
				XWordDataControlParagraph * pBlock = (XWordDataControlParagraph *)(*m_pContent->m_pChilds)[i];
				::OffsetRect(&rt,ptStart.x,ptStart.y);

				RECT rc;
				XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)pBlock->m_pObj;
				if(pCellSvr)
				{
					::SetRect(&rc,0,0,pBlock->width,pBlock->height);
					pCellSvr->SetFRect(rc);

					RECT ert;
					::SetRect(&ert,pBlock->x,pBlock->y,pBlock->x + pBlock->width,pBlock->y + pBlock->height);
					::OffsetRect(&ert,rt.left,rt.top);
					
					pCellSvr->SetOffset(*(POINT *)&ert);

					pCellSvr->DoDraw(hPaintDC,&ert);
				}
				ptStart.y += pBlock->height + 15;
				continue;
			}

			pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
			pPara->FRect = FRect;
			pPara->FContentRect = FRect;
			::OffsetRect(&pPara->FRect,ptStart.x,ptStart.y);
			::OffsetRect(&pPara->FContentRect,ptStart.x,ptStart.y);

			pPara->m_exRects.clear();
			RECT ert;
			for(int k=0;k<pPara->m_pContent->m_pChilds->size();k++)
			{
				XWordDataBase * pData = (*pPara->m_pContent->m_pChilds)[k];
				if(pData->eWordDataType ==_XWordDataSpanControlBlock)
				{
					::SetRect(&ert,pData->x,pData->y,pData->x + pData->width,pData->y + pData->height);
					//::Rectangle(hPaintDC,ert.left + ptStart.x,ert.top + ptStart.y ,ert.right + ptStart.x,ert.bottom + ptStart.y);
					pPara->AddExRect(ert);

					XWordDataSpanControlBlock * pBlock = (XWordDataSpanControlBlock*)(*pPara->m_pContent->m_pChilds)[k];
					if(pBlock->m_pStrShape)
					{
						//assert(_tcschr(pBlock->m_pStrShape,':')>=0);
						XShapeObject * pShape = NULL;

						TCHAR cat[255];
						_stprintf(cat,L"%s",L"uri.hxsoft.com/xarch/xshape/");
						if(_tcschr(pBlock->m_pStrShape,':')>=0)
						{
							_tcsncpy_s(cat+_tcslen(cat),255 - _tcslen(cat),pBlock->m_pStrShape,_tcschr(pBlock->m_pStrShape,':') - pBlock->m_pStrShape );
							cat[_tcslen(cat) + _tcschr(pBlock->m_pStrShape,':') - pBlock->m_pStrShape] = 0;
						}
						xfApp::GetApp()->m_pShapeMgr->CreateShape(cat,_tcschr(pBlock->m_pStrShape,':') + 1,pShape,true);
						RECT rc = ert;
						::OffsetRect(&rc,pPara->FRect.left,pPara->FRect.top);
						::InflateRect(&rc,-3,-3);
						if(pShape)
						{
							if(pBlock->bSelected)
								pShape->DoDraw(hPaintDC,rc,1|2);
							else
								pShape->DoDraw(hPaintDC,rc,1);
						}
					}
					else if(pBlock->m_pStrControlClass)
					{
						if(false && _tcsicmp(pBlock->m_pStrControlClass,L"xcell")==0)
						{
							XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)pBlock->m_pObj;
							RECT rc = ert;
							::OffsetRect(&rc,pPara->FRect.left,pPara->FRect.top);
							pCellSvr->DoDraw(hPaintDC,&rc);
						}else
						{
							xbObject *	pObject;
							xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,pBlock->m_pStrControlClass,pObject);
							xfControl*  pxfControl = (xfControl *)pObject;
							if(pxfControl)
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
								}

								if(_tcsicmp(pBlock->m_pStrControlClass,L"ximage")==0)
								{
									pxfControl->Initial((IXMLDOMElement*)pData->param);
								}
								else
									pxfControl->Initial();

								RECT rc = ert;
								::OffsetRect(&rc,pPara->FRect.left,pPara->FRect.top);

								pxfControl->DoDraw(hPaintDC,&rc);
								//pxfControl->Release();

								if(pBlock->bSelected)
								{
									HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
									hPen = (HPEN)SelectObject(hPaintDC,hPen);
									
									POINT pt;
									//topleft
									pt.x = rc.left,	pt.y = rc.top;
									::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
									//topright
									pt.x = rc.right,	pt.y = rc.top;
									::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
									//bottomleft
									pt.x = rc.left,	pt.y = rc.bottom;
									::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
									//bottomright
									pt.x = rc.right,	pt.y = rc.bottom;
									::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);

									hPen = (HPEN)SelectObject(hPaintDC,hPen);
									::DeleteObject(hPen);
								}
							}
						}
					}
				}
			}

			SIZE extent = pPara->CalcExtent(pDataSvr,pParentBoxStyle,hPaintDC);
			nHeight = extent.cy;
			/*if(ptStart.y + extent.cy < pDrawRect->top)
			{
				//非可视区，继续下一段
				ptStart.y += extent.cy;
				continue;
			}*/

			pPara->DoDraw(pDataSvr,pDocument,pParentBoxStyle,hPaintDC,ptStart,pDrawRect,pUpdateRect);
			ptStart.y += extent.cy + 15;

			//超出视区,则退出
			if(ptStart.y > pDrawRect->bottom) break;
		}
		if(pPara) delete pPara;

		return 1;
	}

	//重新计算位置信息
	SIZE XFlowChapter::CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC)
	{
		SIZE extent={0,0};
		if(!m_pContent) return extent;

		int		nHeight = 0;
		int		nWidth = FRect.right - FRect.left;

		XFlowPara *pPara = new XFlowPara;
		for(int i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			if((*m_pContent->m_pChilds)[i]->eWordDataType == _XWordDataControlParagraph)
			{
				XWordDataControlParagraph * pBlock = (XWordDataControlParagraph *)(*m_pContent->m_pChilds)[i];
				nHeight += pBlock->height + 15;
				continue;
			}

			pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
			pPara->FContentRect = pPara->FRect;

			pPara->m_exRects.clear();
			RECT ert;
			for(int k=0;k<pPara->m_pContent->m_pChilds->size();k++)
			{
				XWordDataBase * pData = (*pPara->m_pContent->m_pChilds)[k];
				if(pData->eWordDataType ==_XWordDataSpanControlBlock)
				{
					::SetRect(&ert,pData->x,pData->y,pData->x + pData->width,pData->y + pData->height);
					pPara->AddExRect(ert);
				}
			}

			RECT rc ;
			::SetRect(&rc,0,0,nWidth ,0);
			pPara->FRect = rc;
			pPara->FContentRect = pPara->FRect;
			SIZE extent = pPara->CalcExtent(pDataSvr,pParentBoxStyle,hDC);
			nHeight += extent.cy;
		}
		if(pPara) delete pPara;

		extent.cx = nWidth;
		extent.cy = nHeight;
		return extent;
	}
	
	XFlowPara * XFlowChapter::GetParaAtPointer(XWordDataSvr* pDataSvr,POINT pt)
	{
		if(!m_pContent) return NULL;

		int		nHeight = 0;
		int		i = 0;
		RECT	rt = FRect;
		POINT	ptStart ={0,0};
		XFlowPara *pPara = new XFlowPara;
		HDC hDC = ::GetDC(NULL);
		
		for(i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			if((*m_pContent->m_pChilds)[i]->eWordDataType == _XWordDataControlParagraph)
			{
				XWordDataControlParagraph * pBlock = (XWordDataControlParagraph *)(*m_pContent->m_pChilds)[i];
				nHeight += pBlock->height + 15;
				if(ptStart.y + nHeight + FRect.top > pt.y)
				{
					pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
					pPara->FRect = FRect;
					pPara->FContentRect = FRect;
					::OffsetRect(&pPara->FRect,ptStart.x,ptStart.y);
					::OffsetRect(&pPara->FContentRect,ptStart.x,ptStart.y);
					break;
				}

				ptStart.y += nHeight;
				continue;
			}

			pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
			pPara->FRect = FRect;
			pPara->FContentRect = FRect;
			::OffsetRect(&pPara->FRect,ptStart.x,ptStart.y);
			::OffsetRect(&pPara->FContentRect,ptStart.x,ptStart.y);

			pPara->m_exRects.clear();
			RECT ert;
			for(int k=0;k<pPara->m_pContent->m_pChilds->size();k++)
			{
				XWordDataBase * pData = (*pPara->m_pContent->m_pChilds)[k];
				if(pData->eWordDataType ==_XWordDataSpanControlBlock)
				{
					::SetRect(&ert,pData->x,pData->y,pData->x + pData->width,pData->y + pData->height);
					pPara->AddExRect(ert);
				}
			}

			SIZE extent = pPara->CalcExtent(pDataSvr,NULL,hDC);
			nHeight = extent.cy;

			ptStart.y += extent.cy + 15;

			//超出视区,则退出
			if(ptStart.y + FRect.top > pt.y) break;
		}
		::ReleaseDC(NULL,hDC);
		if(i<this->m_pContent->m_pChilds->size()) return pPara;
		if(pPara) delete pPara;
		return NULL;
	}
	
	int XFlowChapter::GetIndexByBlock(XWordDataBase *pBlock)
	{
		XFlowPara * pPara = new XFlowPara;
		for(int i=0;i<m_pContent->m_pChilds->size();i++)
		{
			pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
			if(pPara->GetIndexByBlock(pBlock)>=0)
			{
				delete pPara;
				return i;
			}
		}
		delete pPara;

		return -1;
	}

	XFlowPara* XFlowChapter::GetParaByBlock(XWordDataSvr* pDataSvr,XWordDataBase *pBlock)
	{
		if(!m_pContent) return NULL;

		int		nHeight = 0;
		int		i = 0;
		RECT	rt = FRect;
		POINT	ptStart ={0,0};
		XFlowPara *pPara = new XFlowPara;
		HDC hDC = ::GetDC(NULL);
		
		for(i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			if((*m_pContent->m_pChilds)[i]->eWordDataType == _XWordDataControlParagraph)
			{
				XWordDataControlParagraph * pBlock = (XWordDataControlParagraph *)(*m_pContent->m_pChilds)[i];
				ptStart.y += pBlock->height + 15;
				continue;
			}

			pPara->m_pContent = (XWordDataContentParagraph *)(*m_pContent->m_pChilds)[i];
			pPara->FRect = FRect;
			pPara->FContentRect = FRect;
			::OffsetRect(&pPara->FRect,ptStart.x,ptStart.y);
			::OffsetRect(&pPara->FContentRect,ptStart.x,ptStart.y);


			pPara->m_exRects.clear();
			RECT ert;
			for(int k=0;k<pPara->m_pContent->m_pChilds->size();k++)
			{
				XWordDataBase * pData = (*pPara->m_pContent->m_pChilds)[k];
				if(pData->eWordDataType ==_XWordDataSpanControlBlock)
				{
					::SetRect(&ert,pData->x,pData->y,pData->x + pData->width,pData->y + pData->height);
					pPara->AddExRect(ert);
				}
			}
			if(pPara->GetIndexByBlock(pBlock)>=0)break;

			SIZE extent = pPara->CalcExtent(pDataSvr,NULL,hDC);
			nHeight = extent.cy;

			ptStart.y += extent.cy + 15;

		}
		::ReleaseDC(NULL,hDC);
		if(i<this->m_pContent->m_pChilds->size()) return pPara;
		if(pPara) delete pPara;

		return NULL;
	}
}}}}