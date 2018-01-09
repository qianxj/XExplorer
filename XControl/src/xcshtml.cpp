#include "stdafx.h"
#include "xcshtml.hpp"
#include "xframe.h"
#include "ixshtml.hpp"
#include "httableview.hpp"
#include "httabledata.hpp"
#include "XFlowChart.h"
#include "XCell.h"
#include "HtTableStyle.hpp"
#include "HtTableView.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		static LPTSTR shtHtmlType[]={
				L"block",L"text",L"span",L"p",L"br",L"div",L"space",L"a",L"control",L"image",L"body",L"html",L"table"
		};


		xcSHtml::xcSHtml():m_pBlock(NULL),m_pStyleSvr(NULL)
		{
			m_TopLeft.x = 0;
			m_TopLeft.y = 0;
			this->AttachListon(WM_PAINT,(FNEvent)&xcSHtml::EvtPaint);
			this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&xcSHtml::EvtMouseWheel);
			this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&xcSHtml::EvtDoubleClicked);
			this->AttachListon(WM_VSCROLL,(FNEvent)&xcSHtml::EvtVScroll);
			this->AttachListon(WM_HSCROLL,(FNEvent)&xcSHtml::EvtHScroll);
			this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcSHtml::EvtLButtonDown);
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcSHtml::EvtMouseMove);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcSHtml::EvtLButtonUp);
			this->AttachListon(WM_RBUTTONUP,(FNEvent)&xcSHtml::EvtRButtonUp);
			this->AttachListon(WM_SIZE,(FNEvent)&xcSHtml::EvtSize);
		}
		xcSHtml::~xcSHtml()
		{
			if(m_pBlock)delete m_pBlock;
			if(m_pStyleSvr)delete m_pStyleSvr;
		}

		int xcSHtml::LoadHtml(LPCTSTR pHtml)
		{
			if(m_pBlock)delete m_pBlock;
			m_pBlock = ParseXml((LPTSTR)(pHtml));
			m_TopLeft.x = 0;
			m_TopLeft.y = 0;

			AdjustExtent();
			::InvalidateRect(GetHWND(),NULL,false);
			return 1;
		}
		int xcSHtml::AdjustExtent()
		{
			UpdateScrollPos();			
			return 1;
		}

		xcSHtml::htBlock* xcSHtml::ParseXmlByUrl(LPTSTR pStrUrl)
		{
			xbXml x;
			IXMLDOMElement * pElement;
			if(!x.Load(pStrUrl)) return false;
			x.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				htBlock * pBlock = ParseXml(pElement);
				pElement->Release();

				return pBlock;
			}else
				return NULL;
		}

		xcSHtml::htBlock* xcSHtml::ParseXml(LPTSTR pStrHtml)
		{
			xbXml x;
			IXMLDOMElement * pElement;
			if(!x.LoadXml(pStrHtml)) return false;
			x.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				htBlock * pBlock = ParseXml(pElement);
				pElement->Release();
				return pBlock;
			}else
				return NULL;
		}

		xcSHtml::htBlock* xcSHtml::ParseXml(IXMLDOMElement * pElement)
		{
			htBlock * pBlock = NULL;

			BSTR bstr= NULL;
			pElement->get_tagName(&bstr);
			if(!m_pStyleSvr) m_pStyleSvr = new XOffice::XOfficeStyleSvr;
			if(_tcsicmp(bstr,L"style")==0)
			{
				BSTR bstr;
				int nPos = 0;
				int sPos = 0;
				TCHAR buf[255];
				TCHAR css[1024];
				bool bBrach = true;
				pElement->get_text(&bstr);
				
				if(bstr)
				{
					while(true)
					{
						css[0]=0;
						buf[0]=0;
						while(bstr[nPos] && (bstr[nPos]==' '||bstr[nPos]=='\t'||bstr[nPos]=='\r'|| bstr[nPos]=='\n'))nPos++;
						if(!bstr[nPos])break;
						
						//parse css name
						sPos=0;
						while(bstr[nPos] && !(bstr[nPos]==' '||bstr[nPos]=='\t'||bstr[nPos]=='\r'|| bstr[nPos]=='\n'))
						{
							if(sPos < 253) buf[sPos++] = bstr[nPos++];
						}
						buf[sPos++] = 0;
						nPos++;

						//parse css content
						sPos=0;
						while(bstr[nPos] && !(bstr[nPos]=='{'))nPos++;
						if(!bstr[nPos])break;
						if(bstr[nPos]=='{') nPos++;
						css[0] = 0;
						while(bstr[nPos] && !(bstr[nPos]=='}'))
						{
							if(sPos < 1023) css[sPos++] = bstr[nPos++];
						}
						css[sPos++] = 0;
						nPos++;

						m_pStyleSvr->AddCssClass(buf,css);
					}
				}
				return NULL;
			}
			else if(_tcsicmp(bstr,L"html")==0)
			{
				htHtml * pItem = new htHtml;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"body")==0)
			{
				htBody * pItem = new htBody;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"image")==0)
			{
				htImage * pItem = new htImage;
				pItem->pStrUrl = xbXml::GetStringAttribe(pElement,L"src");
				if(!pItem->pStrUrl) pItem->pStrUrl = xbXml::GetStringAttribe(pElement,L"url");
					
				LPTSTR pStr = xbXml::GetStringAttribe(pElement,L"width");
				if(pStr)
				{
					pItem->width = ::_ttol(pStr);
					delete pStr;
				}
				pStr = xbXml::GetStringAttribe(pElement,L"height");
				if(pStr)
				{
					pItem->height = ::_ttol(pStr);
					delete pStr;
				}
				pStr = xbXml::GetStringAttribe(pElement,L"rate");
				if(pStr)
				{
					pItem->rate = ::_tstof(pStr);
					delete pStr;
				}
				pBlock = pItem;
			}else if(_tcsicmp(bstr,L"control")==0 || _tcsicmp(bstr,L"sheet")==0)
			{
				htControl * pItem = new htControl;
				pItem->pStrControlClass = xbXml::GetStringAttribe(pElement,L"controlclass");

				_variant_t var;
				::SetRect(&pItem->FRect,0,0,0,0);
				pElement->getAttribute(L"left",&var);
				if(var.bstrVal)pItem->FRect.left = _ttol(var.bstrVal);
				pElement->getAttribute(L"top",&var);
				if(var.bstrVal)pItem->FRect.top = _ttol(var.bstrVal);
				pElement->getAttribute(L"width",&var);
				if(var.bstrVal)pItem->FRect.right = pItem->FRect.left + _ttol(var.bstrVal);
				pElement->getAttribute(L"height",&var);
				if(var.bstrVal)pItem->FRect.bottom = pItem->FRect.top + _ttol(var.bstrVal);

				if(!pItem->pStrControlClass)
					pItem->pStrControlClass = xbXml::GetStringAttribe(pElement,L"editer");
				if(pItem->pStrControlClass)
				{
					xbObject * pObject;
					xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,pItem->pStrControlClass,pObject);
					if(pObject)
					{
						XOffice::XOfficeSheet * pSheet = dynamic_cast<XOffice::XOfficeSheet *>(pObject);
						IXMLDOMElement*	pDataObject;
						bool bRet = false;
						if(pSheet)
						{
							pSheet->m_Standalone = true;
							pItem->pControl = pSheet;

							_variant_t v;
							pElement->getAttribute(L"src",&v);
							if(v.bstrVal)
							{
								xbXml x;
								IXMLDOMElement * pObjElement = NULL;
								if(xfUriLoader::FetchXml(GetWin()->m_pXPage->m_pStrUrl,v.bstrVal,NULL,x)==1)
								{
									x.GetXmlDoc()->get_documentElement(&pObjElement);
									pSheet->m_pSheetSvr->LoadTemplate(pObjElement,goRunState);
									pDataObject = pObjElement;
									//pObjElement->Release();
									bRet = true;
								}
							}else
							{
								pSheet->m_pSheetSvr->LoadTemplate(pElement,goRunState);
								pDataObject = pElement;
							}
						}else
						{
							pItem->pControl =  dynamic_cast<xfControl*>(pObject);
						}
						IXMLDOMNode * pNode = NULL;
						pElement->selectSingleNode(L"data/node()",&pNode);
						if(pNode)
						{
							pItem->pXmlData = xbXml::GetElement(pNode);
							pNode->Release();
						}
						if(pItem->pXmlData && _tcsicmp(pItem->pStrControlClass,L"xcell")==0)
						{
							XOffice::XCell::IDwTable  * pDwTable = (XOffice::XCell::IDwTable  *)pSheet->GetInterface();
							if(pDwTable)
							{
								pDwTable->m_pDataObject = pDataObject;
								if(pDwTable->m_pDwTable)
								{
									pDwTable->Retrieve(pItem->pXmlData);

									IXMLDOMNode * pNodeX = NULL;
									pElement->selectSingleNode(L"datadiff/node()",&pNodeX);
									if(pNodeX)
									{
										pDwTable->RetrieveDiff(xbXml::GetElement(pNodeX));
										pNodeX->Release();
									}
								}
							}
							SIZE sz=((XOffice::XCell::XCellSheet*)pItem->pControl)->GetContentExtent();
							if(sz.cy + pItem->FRect.top > pItem->FRect.bottom)
								pItem->FRect.bottom = sz.cy + pItem->FRect.top; 
						}
						else
							pItem->pControl->Initial(pElement);
						if(bRet) pDataObject->Release();
					}
				}
				pBlock = pItem;
			}else if(_tcsicmp(bstr,L"a")==0)
			{
				htA * pItem = new htA;
				pItem->pStrUrl = xbXml::GetStringAttribe(pElement,L"src");
				if(!pItem->pStrUrl) pItem->pStrUrl = xbXml::GetStringAttribe(pElement,L"url");
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"space")==0)
			{
				htSpace * pItem = new htSpace;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"div")==0)
			{
				htDiv * pItem = new htDiv;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"br")==0)
			{
				htBr * pItem = new htBr;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"p")==0)
			{
				htP * pItem = new htP;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"span")==0)
			{
				htSpan * pItem = new htSpan;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"text")==0)
			{
				htText * pItem = new htText;
				pBlock = pItem;

			}else if(_tcsicmp(bstr,L"table")==0)
			{
				htTable * pItem = new htTable;
				pBlock = pItem;
			}else
			{
				htBlock * pItem = new htBlock;
				pBlock = pItem;
			}
			::SysFreeString(bstr);

			LPTSTR vi = xbXml::GetStringAttribe(pElement,L"visible");
			if(vi && _tcscmp(vi, L"unlayer") == 0)
				pBlock->bVisible = false;

			pBlock->pData = xbXml::GetStringAttribe(pElement,L"data");
			if(!pBlock->pData)pBlock->pData = xbXml::GetStringAttribe(pElement,L"url");

			pBlock->pCssStyle = xbXml::GetStringAttribe(pElement,L"style");
			pBlock->pCssClass = xbXml::GetStringAttribe(pElement,L"class");
			pBlock->pStrID = xbXml::GetStringAttribe(pElement,L"id");

			if(pBlock->ht==ehtTable)
			{
				XControl::IHtTableView * pView = new XControl::CHtTableView();
				XControl::IHtTableData * pData = new XControl::CHtTableData();

				((Hxsoft::XFrame::XControl::CHtTableStyle*)pData->GetStyle())->SetClassStyle(m_pStyleSvr);
				pData->LoadTable(pElement);
				pView->SetTableData(pData);
				((htTable *)pBlock)->pView = pView;
				return pBlock;
			}

			IXMLDOMNodeList * pNodeList;
			IXMLDOMNode * pNode;
			long nLen;
			pElement->get_childNodes(&pNodeList);
			pNodeList->get_length(&nLen);
			for(int i=0;i<nLen;i++)
			{
				pNodeList->get_item(i,&pNode);
				DOMNodeType nt;
				pNode->get_nodeType(&nt);
				if(nt==NODE_TEXT || nt==NODE_CDATA_SECTION)
				{
					BSTR bstr;
					pNode->get_text(&bstr);

					htText * pItem = new htText;
					pItem->text = _tcsdup(bstr);
					::SysFreeString(bstr);

					if(!pBlock->pChilds) pBlock->pChilds= new vector<htBlock *>();
					pBlock->pChilds->push_back(pItem);
				}else if(nt==NODE_ELEMENT)
				{
					IXMLDOMElement* pElm;
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElm);
					htBlock* bk = ParseXml(pElm);
					if(bk)
					{
						if(!pBlock->pChilds) pBlock->pChilds= new vector<htBlock *>();
						pBlock->pChilds->push_back(bk);
					}
				}
				pNode->Release();
			}
			return pBlock;
		}

		void xcSHtml::Initial()
		{
			Initial(m_pxfNode->m_pElement);
		}

		void xcSHtml::Initial(IXMLDOMElement * pElement)
		{
			if(pElement)
			{
				IXMLDOMNode * pNode;
				IXMLDOMElement* pElementEx;
				BSTR bstr;
				pElement->get_xml(&bstr);
				if(bstr)::SysFreeString(bstr);
				pElement->selectSingleNode(L"html",&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement, (void **)&pElementEx);
					m_pBlock = this->ParseXml(pElementEx);
					pElementEx->Release();
					pNode->Release();
				}
			}
		}

		XUtil::Css::TBoxStyle* xcSHtml::GetBlockCssStyle(htBlock * pBlock)
		{
			if(pBlock->pBoxStyle) return pBlock->pBoxStyle;

			XUtil::Css::TBoxStyle * pStyle = NULL;
			XUtil::Css::TBoxStyle * pStyleEx = NULL;

			//get parent block boxstyle;
			if(pBlock->pParentBlock) pStyle = GetBlockCssStyle(pBlock->pParentBlock);
			
			//parse class boxstyle;
			if(pBlock->pCssClass)
			{
				TCHAR cssClass[255];
				int nPos = 0;
				int sPos = 0;

				while(true)
				{
					while(pBlock->pCssClass[nPos] && (pBlock->pCssClass[nPos]==' '||pBlock->pCssClass[nPos]=='\t'||pBlock->pCssClass[nPos]=='\r'|| pBlock->pCssClass[nPos]=='\n'))nPos++;
					if(!pBlock->pCssClass[nPos])break;
					
					sPos = 1;
					cssClass[0] ='.';
					while(nPos <(int)_tcslen(pBlock->pCssClass) &&  pBlock->pCssClass[nPos] && !(pBlock->pCssClass[nPos]==' '||pBlock->pCssClass[nPos]=='\t'||pBlock->pCssClass[nPos]=='\r'|| pBlock->pCssClass[nPos]=='\n'))
					{
						if(sPos<254) cssClass[sPos++] = pBlock->pCssClass[nPos++];
					}
					cssClass[sPos] = 0;

					if(cssClass[0])
					{
						int nCssIndex =-1;
						if(m_pStyleSvr) nCssIndex = m_pStyleSvr->GetBoxStyleIndexByClass(cssClass);
						if(nCssIndex >=0 )
						{
							pStyleEx = (XUtil::Css::TBoxStyle *)m_pStyleSvr->GetBoxStyle(nCssIndex);
							if(!pStyle) 
							{
								pStyle = new XUtil::Css::TBoxStyle;
								*pStyle = * pStyleEx;
							}else
								pStyle = &(*pStyle + * pStyleEx);
						}
					}

					if(!cssClass[nPos])break;
					nPos++;
				}
			}else if(pBlock->pStrID) //css for id
			{
				TCHAR cssClass[255];
				int nPos = 0;
				int sPos = 1;

				while(pBlock->pStrID[nPos] && (pBlock->pStrID[nPos]==' '||pBlock->pStrID[nPos]=='\t'||pBlock->pStrID[nPos]=='\r'|| pBlock->pStrID[nPos]=='\n'))nPos++;
				if(pBlock->pStrID[nPos])
				{
					sPos = 1;
					cssClass[0] ='#';
					while(pBlock->pStrID[nPos] && !(pBlock->pStrID[nPos]==' '||pBlock->pStrID[nPos]=='\t'||pBlock->pStrID[nPos]=='\r'|| pBlock->pStrID[nPos]=='\n'))
					{
						if(sPos<254) cssClass[sPos++] = pBlock->pStrID[nPos++];
					}
					cssClass[sPos] = 0;

					if(cssClass[1])
					{
						int nCssIndex = -1;
						if(m_pStyleSvr) nCssIndex = m_pStyleSvr->GetBoxStyleIndexByClass(cssClass);
						if(nCssIndex >=0 )
						{
							pStyleEx = (XUtil::Css::TBoxStyle *)m_pStyleSvr->GetBoxStyle(nCssIndex);
							if(!pStyle) 
							{
								pStyle = new XUtil::Css::TBoxStyle;
								*pStyle = * pStyleEx;
							}else
								pStyle = &(*pStyle + * pStyleEx);
						}
					}
				}
			}else
			{
				TCHAR cssClass[255];
				cssClass[0] =0;
				_tcscat_s(cssClass,255,shtHtmlType[pBlock->ht]);

				if(cssClass[1])
				{
					int nCssIndex = -1;
					if(m_pStyleSvr) nCssIndex = m_pStyleSvr->GetBoxStyleIndexByClass(cssClass);
					if(nCssIndex >=0 )
					{
						pStyleEx = (XUtil::Css::TBoxStyle *)m_pStyleSvr->GetBoxStyle(nCssIndex);
						if(!pStyle) 
						{
							pStyle = new XUtil::Css::TBoxStyle;
							*pStyle = * pStyleEx;
						}else
							pStyle = &(*pStyle + * pStyleEx);
					}
				}
			}

			//direct css style
			if(pBlock->pCssStyle)
			{
				pStyleEx = XUtil::xuCCssStyle::ParseCssStyle(pBlock->pCssStyle);
				if(pStyle && pStyleEx)
					pStyle = &(*pStyle + * pStyleEx);
				else if(pStyleEx) pStyle = pStyleEx;
			}

			//if(pStyle) pBlock->pBoxStyle = pStyle;

			return pStyle;
		}

		bool xcSHtml::EvtPaint(TEvent* pEvent,LPARAM lParam)
		{
			HDC hdc = pEvent->paint.hdc;
			RECT	rect;
			this->GetClientRect(&rect);

			HBITMAP hOldBmp = NULL;
			HBITMAP hBmp = NULL;
			if(this->m_bTransparent )
			{
				xfNodeControl* pNode = (xfNodeControl *)m_pxfNode;
				if(pNode->m_hBackGround)
				{
					HDC hMemDC = ::CreateCompatibleDC(hdc);
					hBmp = (HBITMAP)::SelectObject(hMemDC,pNode->m_hBackGround);
					::BitBlt(hdc,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hMemDC,0,0,SRCCOPY);
					hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
					::DeleteDC(hMemDC);
					this->DoDraw(pEvent->paint.hdc);
				}
			}else
			{
				HDC hdc = pEvent->paint.hdc;
				RECT rc =  rect;
				HDC hPaintDC = ::CreateCompatibleDC(hdc);
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
			}
			return true;
		}


		int xcSHtml::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rt;
			if(pDrawRect)
				rt = *pDrawRect;
			else
				GetClientRect(&rt);
			
			m_ctlRect.clear();
			m_Blocks.clear();

			htBlock * pBlock = m_pBlock;
			if(!pBlock) return 0;
			if(m_pBlock->ht==ehtHtml)
			{
				pBlock = NULL;
				for(int i=0;i<(int)m_pBlock->pChilds->size();i++)
				{
					if((*m_pBlock->pChilds)[i] && (*m_pBlock->pChilds)[i]->bVisible && (*m_pBlock->pChilds)[i]->ht==ehtBody)
					{
						pBlock = (*m_pBlock->pChilds)[i];
						break;
					}
				}
			}
			if(!pBlock) return 0;
			
			POINT pt = m_TopLeft;
			pt.x = -pt.x;
			pt.y = -pt.y;
			int rowHeight = 0;
			int hbr;
			hbr = ::SetBkMode(hPaintDC,TRANSPARENT);
			DoDrawBlock(hPaintDC,pBlock,rt,pt, rowHeight);
			hbr = ::SetBkMode(hPaintDC,hbr);

			return 1;

		}

		int xcSHtml::CalcOffset(HDC hPaintDC,htBlock * pBlock, RECT &rt,POINT & ptOffset,int & rowHeight)
		{
			XUtil::Css::TBoxStyle * pStyle = GetBlockCssStyle(pBlock);
			XUtil::Css::TBoxStyle * pStyle1 ;
			
			//draw background;
			if(pBlock->ht ==ehtBody)
			{
			}else if(pBlock->ht ==ehtDiv)
			{

			}

			int nFit;
			SIZE size;
			SIZE sz;
			int * pDx;
			LPTSTR pStr = NULL;
			LOGFONT logFont;
			HFONT hFont;
			int nPos = -1;
			int extent;
			int ntab = 3;
			int tabExtent = 0;
			int rwidth = 0;
			int rtab = 0;
			htBlock * pItem;
			POINT pt;
	
			RECT contentRect = rt;
			if(pStyle) this->GetContentRect(rt, pStyle,contentRect);
			ptOffset.x += contentRect.left - rt.left;

			if(pBlock->pChilds)
			{
				for(int i=0;i<(int)pBlock->pChilds->size();i++)
				{
					pItem = (*pBlock->pChilds)[i];
					if(!pItem->bVisible) continue;
					switch(pItem->ht)
					{
					case ehtTable:
						ptOffset.y += rowHeight + 1;
						ptOffset.x = contentRect.left;

						rowHeight = ((htTable*)pItem)->pView->GetHeight();
						ptOffset.x += ((htTable*)pItem)->pView->GetWidth();
						break;
					case ehtControl:
						{
							ptOffset.y += 8 ;
							ptOffset.y += rowHeight + 1;
							ptOffset.x = contentRect.left;
							SIZE cz = ((htControl *)pItem)->pControl->GetContentExtent();
							if(cz.cy < 0) cz.cy = 400;
							rowHeight = cz.cy;
						}
						break;
					case ehtP:
					case ehtDiv:
						ptOffset.y += 8 ;
						ptOffset.y += rowHeight + 1;
						ptOffset.x = contentRect.left;
						rowHeight = 0;
						pt = ptOffset;
						CalcOffset(hPaintDC,pItem,rt,ptOffset, rowHeight);
						if(pStyle && pStyle->mask & cssMaskWidth )
						{
							int nWidth = (int)Hxsoft::XUtil::Css::GetPixelExtent(pStyle->pWidth);
							if(pt.y==ptOffset.y && ptOffset.x - pt.x < nWidth) ptOffset.x = pt.x + nWidth;

						}
						break;
					case ehtSpan:
						pt = ptOffset;

						CalcOffset(hPaintDC,pItem,rt,ptOffset, rowHeight);
						pStyle1 = GetBlockCssStyle(pItem);
						if(pStyle1 && pStyle1->mask & cssMaskWidth )
						{
							int nWidth = (int)Hxsoft::XUtil::Css::GetPixelExtent(pStyle1->pWidth);
							if(pt.y==ptOffset.y && ptOffset.x - pt.x < nWidth) ptOffset.x = pt.x + nWidth;

						}
						break;
					case ehtText:
						{
							XUtil::Css::GetFont(pStyle,logFont);
							hFont = ::CreateFontIndirect(&logFont);
							hFont = (HFONT)::SelectObject(hPaintDC,hFont);

							//calc char width and row height
							GetTextExtentPoint(hPaintDC,_T("AFGXMafxgm"),10,&sz);
							tabExtent = sz.cx * ntab /10;

							if(pItem->rowHeight ==0)pItem->rowHeight = sz.cy + 2; 
							if(pItem->rowHeight > rowHeight) rowHeight = pItem->rowHeight;

							pStr = ((htText *)pItem)->text;
							pDx = new int[_tcslen(pStr)];
							rwidth = contentRect.right  - ptOffset.x + 1;
							::GetTextExtentExPoint(hPaintDC,pStr,(int)_tcslen(pStr),rwidth,&nFit,pDx,&size);

							while(pStr[0])
							{
								extent = 0;
								rtab = 0;
								extent = 0;
								//get row extent;
								for(int i=0;i<nFit;i++)
								{
									nPos++;
									if(pStr[i] && pStr[i]!='\r' && pStr[i]!='\n')
									{
										if(pStr[i]=='\t')
											extent += tabExtent;
										else
										{
											if(i==0)
												extent += pDx[i];
											else
												extent += pDx[i] - pDx[i -1];
										}
										if(extent > rwidth)
										{
											if(pStr[i]=='\t')
											{
												rtab = extent - rwidth;
												extent = rwidth;
											}else
												extent -= pDx[i];
											nPos--;
											break;
										}
									}else
									{
										if(pStr[i]=='\r' && pStr[i+1]=='\n') nPos++;
										break;
									}
								}

								//if all area not fit one char,should fit one; 
								if(nPos < 0 && ptOffset.x <= contentRect.left)
								{
									nPos = 0;
									extent = pDx[nPos];
								}

								int count = nPos + 1;
								while(pStr[count - 1]=='\r' || pStr[count - 1]=='\n')count--;

								pStr = pStr + nPos + 1 ;
								if(pStr[0])
								{
									rwidth = contentRect.right - contentRect.left + 1;
									ptOffset.y += rowHeight + 1;
									ptOffset.x = contentRect.left;
									nPos = -1;
									rowHeight = pItem->rowHeight;
									::GetTextExtentExPoint(hPaintDC,pStr,(int)_tcslen(pStr),rwidth,&nFit,pDx,&size);
								}else
								{
									ptOffset.x += extent;
									nPos = -1;
								}
							}

							hFont = (HFONT)::SelectObject(hPaintDC,hFont);
							::DeleteObject(hFont);
							delete pDx;
						}
						break;
					}
				}
			}

			return 1;
		}

		int xcSHtml::DoDrawBlock(HDC hPaintDC, xcSHtml::htBlock * pBlock, RECT &rt,POINT & ptOffset,int & rowHeight)
		{
			XUtil::Css::TBoxStyle * pStyle = GetBlockCssStyle(pBlock);
			XUtil::Css::TBoxStyle * pStyle1 ;

			//draw background;
			if(pBlock->ht ==ehtBody)
			{
				XUtil::Css::DrawCssFrame(hPaintDC,&rt,pStyle);
			}else if(pBlock->ht ==ehtDiv)
			{

			}

			int nFit;
			SIZE size;
			SIZE sz;
			int * pDx;
			LPTSTR pStr = NULL;
			LOGFONT logFont;
			HFONT hFont;
			int nPos = -1;
			int extent;
			int ntab = 3;
			int tabExtent = 0;
			int rwidth = 0;
			int rtab = 0;
			htBlock * pItem;
			POINT pt;
	
			RECT contentRect = rt;
			if(pStyle) this->GetContentRect(rt, pStyle,contentRect);
			ptOffset.x += contentRect.left - rt.left;

			if(pBlock->pChilds)
			{
				for(int i=0;i<(int)pBlock->pChilds->size();i++)
				{
					if(ptOffset.y > rt.bottom) break;
					pItem = (*pBlock->pChilds)[i];
					if(!pItem->bVisible) continue;
					switch(pItem->ht)
					{
					case ehtTable:
						{
							ptOffset.y += rowHeight + 1;
							ptOffset.x = contentRect.left ;

							pStyle = GetBlockCssStyle(pItem);
							RECT rt = contentRect;
							if(pStyle) this->GetContentRect(rt, pStyle,contentRect);
							rt.left = ptOffset.x + contentRect.left - rt.left;
							rt.top  = ptOffset.y + contentRect.top - rt.top;
							rt.right = contentRect.right;
							


							((htTable*)pItem)->pView->DoDrawTo(hPaintDC,rt,1,1);

							rowHeight = ((htTable*)pItem)->pView->GetHeight();
							ptOffset.x += ((htTable*)pItem)->pView->GetWidth();
							
							rt.bottom = rt.top + rowHeight; //contentRect.bottom;
							m_ctlRect.push_back(rt);
							m_Blocks.push_back(pItem);
						}
						break;
					case ehtControl:
						{
							RECT rt = ((htControl *)pItem)->FRect;//contentRect;
							SIZE cz = ((htControl *)pItem)->pControl->GetContentExtent();
							if(cz.cy <= 0) cz.cy = ((htControl *)pItem)->FRect.bottom - ((htControl *)pItem)->FRect.top;
							if(cz.cx <= 0) cz.cx = ((htControl *)pItem)->FRect.right - ((htControl *)pItem)->FRect.left;
							if(cz.cy <= 0) cz.cy = 240;
							if(cz.cx <= 0) cz.cx = 2400;

							int nWidth = contentRect.right - contentRect.left;
							pStyle1 = GetBlockCssStyle(pBlock);
							if(pStyle1 && pStyle1->mask & cssMaskWidth )
							{
								nWidth = (int)Hxsoft::XUtil::Css::GetPixelExtent(pStyle1->pWidth);
							}

							if(ptOffset.x + rt.right > nWidth)//»»ÐÐ
							{
								ptOffset.y += 8 ;
								ptOffset.y += rowHeight + 1;
								ptOffset.x = contentRect.left;
								pt = ptOffset;
								
								pt.x += rt.left;
								pt.y += rt.top;
								if(rt.bottom == 0 && rt.right == 0)
								{
									::SetRect(&rt,pt.x,pt.y,pt.x + cz.cx,pt.y + cz.cy);
									rowHeight = cz.cy;
									ptOffset.x = pt.x + cz.cx;
								}
								else
								{
									::SetRect(&rt,pt.x,pt.y,pt.x + rt.right,pt.y + rt.bottom);
									ptOffset.x = pt.x + rt.right;
									rowHeight = rt.bottom - rt.top;
								}
							}
							else  //²»»»ÐÐ
							{
								pt = ptOffset;
								ptOffset.x = pt.x + rt.right;
								::SetRect(&rt,pt.x,pt.y,pt.x + rt.right,pt.y + rt.bottom);
								if(rowHeight < (rt.bottom - rt.top))
									rowHeight = rt.bottom - rt.top;
							}

							m_ctlRect.push_back(rt);
							m_Blocks.push_back(pItem);
							((htControl *)pItem)->pControl->DoDraw(hPaintDC,&rt);
						}
						break;
					case ehtP:
					case ehtDiv:
						{
							ptOffset.y += 8 ;
							ptOffset.y += rowHeight + 1;
							ptOffset.x = contentRect.left;
							rowHeight = 0;
							pt = ptOffset;

							RECT rc;
							rc.left = ptOffset.x;
							rc.top = ptOffset.y;
							DoDrawBlock(hPaintDC,pItem,rt,ptOffset, rowHeight);
							if(pStyle && pStyle->mask & cssMaskWidth )
							{
								int nWidth = (int)Hxsoft::XUtil::Css::GetPixelExtent(pStyle->pWidth);
								if(pt.y==ptOffset.y && ptOffset.x - pt.x < nWidth) ptOffset.x = pt.x + nWidth;

							}
							rc.right = ptOffset.x;
							rc.bottom = ptOffset.y + rowHeight;
							m_ctlRect.push_back(rc);
							m_Blocks.push_back(pItem);
						}
						break;
					case ehtSpan:
						{
							pt = ptOffset;

							RECT rc;
							rc.left = ptOffset.x;
							rc.top = ptOffset.y;
							DoDrawBlock(hPaintDC,pItem,rt,ptOffset, rowHeight);
							pStyle1 = GetBlockCssStyle(pItem);
							if(pStyle1 && pStyle1->mask & cssMaskWidth )
							{
								int nWidth = (int)Hxsoft::XUtil::Css::GetPixelExtent(pStyle1->pWidth);
								if(pt.y==ptOffset.y && ptOffset.x - pt.x < nWidth) ptOffset.x = pt.x + nWidth;
							}
							rc.right = ptOffset.x;
							rc.bottom = ptOffset.y + rowHeight;
							m_ctlRect.push_back(rc);
							m_Blocks.push_back(pItem);
						}
						break;
					case ehtText:
						{
							XUtil::Css::GetFont(pStyle,logFont);
							hFont = ::CreateFontIndirect(&logFont);
							hFont = (HFONT)::SelectObject(hPaintDC,hFont);

							//calc char width and row height
							GetTextExtentPoint(hPaintDC,_T("AFGXMafxgm"),10,&sz);
							tabExtent = sz.cx * ntab /10;

							if(pItem->rowHeight ==0)pItem->rowHeight = sz.cy + 2; 
							if(pItem->rowHeight > rowHeight) rowHeight = pItem->rowHeight;

							pStr = ((htText *)pItem)->text;
							pDx = new int[_tcslen(pStr)];
							rwidth = contentRect.right  - ptOffset.x + 1;
							::GetTextExtentExPoint(hPaintDC,pStr,(int)_tcslen(pStr),rwidth,&nFit,pDx,&size);

							bool bRow = false;
							//set align
							UINT fmode = 0;
							if(pStyle && pStyle->mask & cssMaskText && pStyle->pText )
							{
								if(pStyle->pText->mask & cssMaskTextAlign)
								{
									switch(pStyle->pText->textalign)
									{
									case XUtil::Css::_left:
										fmode |= TA_LEFT;
										break;
									case XUtil::Css::_right:
										fmode |= TA_RIGHT;
										break;
									case XUtil::Css::_center:
										fmode |= TA_CENTER;
										break;
									case XUtil::Css::_justify:
									case XUtil::Css::_inherit:
										fmode |= TA_CENTER;
										break;
									default:
										fmode |= TA_CENTER;
										break;
									}
								}
							}
							if(!fmode)fmode = TA_LEFT;
							fmode = ::SetTextAlign(hPaintDC,fmode);

							//set color
							COLORREF color = RGB(0,0,0);
							if(pStyle && pStyle->mask & cssMaskColor && pStyle->pColor  )
							{
								if(pStyle->pColor->colorRef[0] == XUtil::Css::_rgbcolor)
									color = pStyle->pColor->color[0];
								else if(pStyle->pColor->colorRef[0] == XUtil::Css::_colorindex)
									color = pStyle->pColor->color[0];
							}
							color = ::SetTextColor(hPaintDC,color);

							while(pStr[0])
							{
								extent = 0;
								rtab = 0;
								extent = 0;
								//get row extent;
								for(int i=0;i<nFit;i++)
								{
									nPos++;
									if(pStr[i] && pStr[i]!='\r' && pStr[i]!='\n')
									{
										if(pStr[i]=='\t')
											extent += tabExtent;
										else
										{
											if(i==0)
												extent += pDx[i];
											else
												extent += pDx[i] - pDx[i -1];
										}
										if(extent > rwidth)
										{
											if(pStr[i]=='\t')
											{
												rtab = extent - rwidth;
												extent = rwidth;
											}else
												extent -= pDx[i];
											nPos--;
											break;
										}
									}else
									{
										if(pStr[i]=='\r' && pStr[i+1]=='\n') nPos++;
										break;
									}
								}

								//if all area not fit one char,should fit one; 
								if(nPos < 0 && ptOffset.x <= contentRect.left)
								{
									nPos = 0;
									extent = pDx[nPos];
								}

								int count = nPos + 1;
								while(pStr[count - 1]=='\r' || pStr[count - 1]=='\n')count--;
								if(count > 0)
									ExtTextOut(hPaintDC,ptOffset.x + contentRect.left,ptOffset.y + contentRect.top  /*- rt.top*/,0,NULL,pStr,count,NULL);

								pStr = pStr + nPos + 1 ;
								if(pStr[0])
								{
									rwidth = contentRect.right - contentRect.left + 1;
									ptOffset.y += rowHeight + 1;
									ptOffset.x = contentRect.left;
									nPos = -1;
									rowHeight = pItem->rowHeight;
									bRow = true;
									::GetTextExtentExPoint(hPaintDC,pStr,(int)_tcslen(pStr),rwidth,&nFit,pDx,&size);
								}else
								{
									ptOffset.x += extent;
									nPos = -1;
								}
							}

							color = ::SetTextColor(hPaintDC,color);
							fmode = ::SetTextAlign(hPaintDC,fmode);
							hFont = (HFONT)::SelectObject(hPaintDC,hFont);
							::DeleteObject(hFont);
							delete pDx;
						}
						break;
					}
				}
			}

			return 1;
		}

		int xcSHtml::GetContentRect(RECT &rt, XUtil::Css::TBoxStyle * pStyle, RECT & contentRect)
		{
			contentRect = rt;

			XUtil::Css::TBoxStyle * pCssBoxStyle = pStyle;
			if(pCssBoxStyle->mask & cssMaskMargin)
			{
				if(pCssBoxStyle->pMargin[0])
				{
					int valuex =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0]);
					int valuey =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0],false);
					contentRect.left += valuex;
					contentRect.right -= valuey;
					contentRect.top += valuex;
					contentRect.bottom -= valuey;
				}
				if(pCssBoxStyle->pMargin[1])contentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[1]);
				if(pCssBoxStyle->pMargin[2])contentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[2],false);
				if(pCssBoxStyle->pMargin[3])contentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[3]);
				if(pCssBoxStyle->pMargin[4])contentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[4],false);

			}
			if(pCssBoxStyle->mask & cssMaskPadding)
			{
				if(pCssBoxStyle->pPadding[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[0]);
					contentRect.left += value;
					contentRect.right -= value;
					contentRect.top += value;
					contentRect.bottom -= value;
				}
				if(pCssBoxStyle->pPadding[1])contentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[1]);
				if(pCssBoxStyle->pPadding[2])contentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[2],false);
				if(pCssBoxStyle->pPadding[3])contentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[3]);
				if(pCssBoxStyle->pPadding[4])contentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[4],false);

			}
			if(pCssBoxStyle->mask & cssMaskBorder)
			{
				if(pCssBoxStyle->pBorder[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(&pCssBoxStyle->pBorder[0]->widthmesure);
					contentRect.left += value;
					contentRect.right -= value;
					contentRect.top += value;
					contentRect.bottom -= value;
				}
				if(pCssBoxStyle->pBorder[1])contentRect.left += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[1]);
				if(pCssBoxStyle->pBorder[2])contentRect.top += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[2],false);
				if(pCssBoxStyle->pBorder[3])contentRect.right -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[3]);
				if(pCssBoxStyle->pBorder[4])contentRect.bottom -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[4],false);
			}
			return 1;
		}

		xbObject * xcSHtml::GetInterface()
		{
			if(!m_pInterFace)
			{
				m_pInterFace  = new IXSHtml;
				((IXFControl *)m_pInterFace)->m_pControl = this;
			}
			return this->m_pInterFace;
		}

		SIZE xcSHtml::GetContentExtent()
		{
			SIZE sz;
			sz.cx = -1;
			sz.cy = 0;

			htBlock * pBlock = m_pBlock;
			if(!pBlock) return sz;
			if(m_pBlock->ht==ehtHtml)
			{
				pBlock = NULL;
				for(int i=0;i<(int)m_pBlock->pChilds->size();i++)
				{
					if((*m_pBlock->pChilds)[i] && (*m_pBlock->pChilds)[i]->ht==ehtBody)
					{
						pBlock = (*m_pBlock->pChilds)[i];
						break;
					}
				}
			}
			if(!pBlock) return sz;
			
			POINT pt = {0,0};//m_TopLeft;
			int rowHeight = 0;
			RECT rt;

			GetClientRect(&rt);
			HDC hPaintDC;
			hPaintDC = ::GetDC(GetHWND());

			CalcOffset(hPaintDC,pBlock,rt,pt, rowHeight);
			::ReleaseDC(GetHWND(),hPaintDC);

			pt.y += rowHeight;
			sz.cx = pt.x;
			sz.cy = pt.y;

			return sz;
		}

		void xcSHtml::UpdateScrollPos()
		{
			SIZE cz = GetContentExtent();
			SCROLLINFO si; 
			RECT rt;
			GetClientRect(&rt);
			if(rt.bottom - rt.top < cz.cy)
			{
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_RANGE | SIF_PAGE; 
				si.nMin   = 0; 
				si.nMax   = cz.cy ; 
				si.nPage  = rt.bottom - rt.top ; 
				SetScrollInfo(GetHWND(), SB_VERT, &si, TRUE); 
			}else
			{
				ShowScrollBar(GetHWND(),SB_VERT,false);
			}

			if(rt.right - rt.left < cz.cx)
			{
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_RANGE | SIF_PAGE; 
				si.nMin   = 0; 
				si.nMax   = cz.cx + 10 ;
				si.nPage  = rt.right - rt.left ; 
				SetScrollInfo(GetHWND(), SB_HORZ, &si, TRUE); 
			}else
			{
				ShowScrollBar(GetHWND(),SB_HORZ,false);
			}
		}
		
		int xcSHtml::GetHitedObject(POINT pt)
		{
			for(int i=0;i<(int)this->m_ctlRect.size();i++)
			{
				if(::PtInRect(&this->m_ctlRect[i],pt))return i;
			}
			return -1;
		}

		bool xcSHtml::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
		{
			if(::GetFocus()!=GetHWND()) ::SetFocus(GetHWND());
			POINT pt = pEvent->mouse.point;
			int nIndex = this->GetHitedObject(pt);
			if(nIndex<0) return false;
			htBlock * pItem = m_Blocks[nIndex];
			RECT rt = m_ctlRect[nIndex];
			if(pItem && pItem->ht==ehtTable)
			{
				XControl::CHtTableView * pView = (XControl::CHtTableView *)((xcSHtml::htTable*)pItem)->pView;
				if(pView)
				{
					THtGridCoord coord = pView->CalcCoordFromPoint(/*m_TopLeft.x +*/ pEvent->mouse.point.x - rt.left,
						/*m_TopLeft.y +*/ pEvent->mouse.point.y - rt.top);
					if(coord.x <= pView->GetColCount() && coord.y <= pView->GetRowCount())
					{
						RECT sRect;
						if(pView->GetTableData()->GetSpan()->isSpaned(coord.x,coord.y,sRect))
						{
							coord.x =  sRect.left;
							coord.y =  sRect.top;
						}
						((xcSHtml::htTable*)pItem)->FRow = coord.y;
						((xcSHtml::htTable*)pItem)->FColumn = coord.x;
						//pView->FCurrent = coord;
						//pView->FAnchor = pView->FCurrent;
					}else
					{
						((xcSHtml::htTable*)pItem)->FRow = 0;
						((xcSHtml::htTable*)pItem)->FColumn = 0;
					}
				}
			}
			else if(pItem && pItem->ht==ehtControl)
			{
				//pEvent->mouse.point.x -= rt.left; 
				//pEvent->mouse.point.y -= rt.top;
				
				XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet*>(((htControl *)pItem)->pControl);
				if(pSheet)
				{
					XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = dynamic_cast<XOffice::XFlowChart::XFlowChartDataSvr *>(pSheet->m_pSheetSvr->m_pDataSvr);
					if(!pDataSvr->m_pContentSvr) return true;
					vector<XOffice::XFlowChart::XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
					XOffice::XFlowChart::XFlowChartObject * pObject = NULL;
					for(int i = (int)DrawObjects.size() - 1;i>=0; i--)
					{
						if(PtInRect(&DrawObjects[i]->FRect,pt)) 
						{
							pObject = DrawObjects[i];
							if(pObject->m_pElement)
							{
								_variant_t v;
								pObject->m_pElement->getAttribute(L"url",&v);
								BSTR bstr;
								pObject->m_pElement->get_xml(&bstr);
								::SysFreeString(bstr);
								if(v.bstrVal && v.bstrVal[0])
								{
									HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
									hCursor = ::SetCursor(hCursor);
									IXFWin::OpenWindow(GetWin()->m_pXPage->m_pStrUrl,v.bstrVal,(IXFWin *)GetWin()->GetFrameWindow()->GetInterface(),(LPARAM)NULL,(IXFWin *)NULL,NULL,NULL,(LPRECT)NULL);
									hCursor = ::SetCursor(hCursor);
									DestroyCursor(hCursor);
								}
							}
						}
					}
				}
			}
			/*
			POINT pt = pEvent->mouse.point;
			int nIndex = this->GetHitedObject(pt);
			if(nIndex<0) return false;
			htBlock * pItem = m_Blocks[nIndex];
			RECT rt = m_ctlRect[nIndex];
			if(pItem && pItem->ht==ehtControl)
			{
				//pEvent->mouse.point.x -= rt.left; 
				//pEvent->mouse.point.y -= rt.top;
				
				XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet*>(((htControl *)pItem)->pControl);
				if(pSheet)
				{
					((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->EvtLButtonDown(pEvent,lParam);
					::InvalidateRect(GetHWND(),NULL,false);
				}
			}*/
			return true;
		}

		bool xcSHtml::EvtRButtonUp(TEvent * pEvent, LPARAM lParam)
		{
			POINT pt = pEvent->mouse.point;
			
			int nIndex = this->GetHitedObject(pt);
			if(nIndex >= 0)
			{
				HTOBJCLICKEDNMHDR hdr;
				memset(&hdr,0,sizeof(HTOBJCLICKEDNMHDR));
				hdr.hwndFrom = GetHWND();
				hdr.idFrom = 0;
				hdr.code = HTC_OBJECTRCLICKED;
				hdr.pObject = m_Blocks[nIndex];
				::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
			}
			return true;
		}

		bool xcSHtml::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
		{
			POINT pt = pEvent->mouse.point;
			
			int nIndex = this->GetHitedObject(pt);
			if(nIndex >= 0)
			{
				HTOBJCLICKEDNMHDR hdr;
				memset(&hdr,0,sizeof(HTOBJCLICKEDNMHDR));
				hdr.hwndFrom = GetHWND();
				hdr.idFrom = 0;
				hdr.code = HTC_OBJECTCLICKED;
				hdr.pObject = m_Blocks[nIndex];
				::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
			}
			return true;

			if(nIndex<0) return false;
			htBlock * pItem = m_Blocks[nIndex];
			RECT rt = m_ctlRect[nIndex];
			if(pItem && pItem->ht==ehtControl)
			{
				//pEvent->mouse.point.x -= rt.left; 
				//pEvent->mouse.point.y -= rt.top;
				
				XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet*>(((htControl *)pItem)->pControl);
				if(pSheet)
				{
					((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->EvtLButtonUp(pEvent,lParam);
					::InvalidateRect(GetHWND(),NULL,false);
				}
			}
			return true;
		}
		bool xcSHtml::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
		{
			return true;
			POINT pt = pEvent->mouse.point;
			
			int nIndex = this->GetHitedObject(pt);
			if(nIndex<0) return false;
			htBlock * pItem = m_Blocks[nIndex];
			RECT rt = m_ctlRect[nIndex];
			if(pItem && pItem->ht==ehtControl)
			{
				//pEvent->mouse.point.x -= rt.left; 
				//pEvent->mouse.point.y -= rt.top;
				
				XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet*>(((htControl *)pItem)->pControl);
				if(pSheet)
				{
					((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->EvtMouseMove(pEvent,lParam);
					::InvalidateRect(GetHWND(),NULL,false);
				}
			}
			return true;
		}
		
		bool xcSHtml::EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam)
		{
			POINT pt = pEvent->mouse.point;
			int nIndex = this->GetHitedObject(pt);
			if(nIndex<0) return false;
			HTOBJCLICKEDNMHDR hdr;
			memset(&hdr,0,sizeof(HTOBJCLICKEDNMHDR));
			hdr.hwndFrom = GetHWND();
			hdr.idFrom = 0;
			hdr.code = HTC_OBJECTDBCLICKED;
			hdr.pObject = m_Blocks[nIndex];
			::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
			htBlock * pItem = m_Blocks[nIndex];
			RECT rt = m_ctlRect[nIndex];
			if(pItem && pItem->ht==ehtControl)
			{
				//pEvent->mouse.point.x -= rt.left; 
				//pEvent->mouse.point.y -= rt.top;
				
				XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet*>(((htControl *)pItem)->pControl);
				if(pSheet)
				{
					XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = dynamic_cast<XOffice::XFlowChart::XFlowChartDataSvr *>(pSheet->m_pSheetSvr->m_pDataSvr);
					if(!pDataSvr->m_pContentSvr) return true;
					vector<XOffice::XFlowChart::XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
					XOffice::XFlowChart::XFlowChartObject * pObject = NULL;
					for(int i = (int)DrawObjects.size() - 1;i>=0; i--)
					{
						if(PtInRect(&DrawObjects[i]->FRect,pt)) 
						{
							pObject = DrawObjects[i];
							if(pObject->m_pElement)
							{
								_variant_t v;
								pObject->m_pElement->getAttribute(L"url",&v);
								if(v.bstrVal && v.bstrVal[0])
								{
									HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
									hCursor = ::SetCursor(hCursor);
									IXFWin::OpenWindow(GetWin()->m_pXPage->m_pStrUrl,v.bstrVal,(IXFWin*)GetWin()->GetInterface());
									hCursor = ::SetCursor(hCursor);
									DestroyCursor(hCursor);
								}
							}
						}
					}
				}
			}
			return true;
		}

		bool xcSHtml::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
		{
			if(pEvent->wheel.zDelta > 0)
				ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
			else
				ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
			InvalidateRect(GetHWND(),NULL,false);
			return true;
		}
		bool xcSHtml::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
		{
			ModifyScrollBar(SB_VERT,pEvent->scroll.nSBCode,pEvent->scroll.nPos);
			InvalidateRect(GetHWND(),NULL,false);
			return true;
		}
		bool xcSHtml::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
		{
			ModifyScrollBar(SB_HORZ,pEvent->scroll.nSBCode,pEvent->scroll.nPos);
			InvalidateRect(GetHWND(),NULL,false);
			return true;
		}
		bool xcSHtml::EvtSize(TEvent *pEvent ,LPARAM lParam)
		{
			if(m_pBlock)UpdateScrollPos();
			return true;
		}
		int xcSHtml::CalcScrollBar(int Value, int ScrollCode, int ScrollBar,  int Pos)
		{
			int Result;
			Result = Value;
			switch(ScrollCode) 
			{
			case SB_LINEUP:
				Result = Value - 17;
				break;
			case SB_LINEDOWN:
				Result = Value + 17;
				break;
			case SB_PAGEUP:
				Result = Value - PageUp(ScrollBar);
				break;
			case SB_PAGEDOWN:
				Result = Value + PageDown(ScrollBar);
				break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				SCROLLINFO si; 
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_ALL;
				GetScrollInfo (GetHWND(), ScrollBar, &si);
				Result = si.nTrackPos;
				break;
			case SB_BOTTOM:
				Result = ScrollBarMax(ScrollBar);
				break;
			case SB_TOP:
				Result = ScrollBarMin(ScrollBar);
				break;
			}
			return Result;
		}

		int xcSHtml::ScrollBarMin(int ScrollBar)
		{
			return 0;
		}
		int xcSHtml::ScrollBarMax(int ScrollBar)
		{
			SIZE cz = GetContentExtent();
			RECT rt;
			GetClientRect(&rt);
			if(ScrollBar == SB_HORZ)
				return cz.cx - rt.right;
			else
				return cz.cy - rt.bottom;
		}

		int xcSHtml::PageUp(int ScrollBar)
		{
			RECT rt;
			GetClientRect(&rt);
			if(ScrollBar == SB_HORZ)
			{
				return rt.right - rt.left;
			}else
				return rt.bottom - rt.top;
		}

		int xcSHtml::PageDown(int ScrollBar)
		{
			RECT rt;
			GetClientRect(&rt);
			if(ScrollBar == SB_HORZ)
			{
				return rt.right - rt.left;
			}else
				return rt.bottom - rt.top;
		}

		POINT  xcSHtml::CalcMaxTopLeft(POINT Coord,int ScrollBar)
		{
			RECT rt;
			GetClientRect(&rt);
			if(ScrollBar == SB_HORZ)
			{
				Coord.x -= rt.right;
				if(Coord.x <0) Coord.x = 0;
			}
			else
			{
				Coord.y -= rt.bottom;
				if(Coord.y <0) Coord.y = 0;
			}
			return Coord;
		}

		void xcSHtml::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
		{
			SIZE cz = GetContentExtent();
			POINT NewTopLeft,MaxTopLeft;

			MaxTopLeft.x = cz.cx;
			MaxTopLeft.y = cz.cy ;
			MaxTopLeft = CalcMaxTopLeft(MaxTopLeft,ScrollBar);
			NewTopLeft = m_TopLeft;
			if(ScrollBar == SB_HORZ) 
				NewTopLeft.x = CalcScrollBar(m_TopLeft.x,ScrollCode, ScrollBar, Pos);
			else
				NewTopLeft.y = CalcScrollBar(m_TopLeft.y,ScrollCode, ScrollBar, Pos);
			
			if(NewTopLeft.x < 0)
				NewTopLeft.x =0;
			else if(NewTopLeft.x > MaxTopLeft.x )
				NewTopLeft.x = MaxTopLeft.x;
			if(NewTopLeft.y < 1)
				NewTopLeft.y = 1;
			else if(NewTopLeft.y > MaxTopLeft.y)
				NewTopLeft.y = MaxTopLeft.y;

			SetScrollPos(GetHWND(), ScrollBar,
						ScrollBar == SB_HORZ? NewTopLeft.x:NewTopLeft.y,TRUE);
			m_TopLeft = NewTopLeft;
			
			/*
			int dx = NewTopLeft.x - m_TopLeft.x;
			int dy = NewTopLeft.y - m_TopLeft.y;
			::ScrollWindow(GetHWND(),- dx,- dy,NULL,NULL);
			UpdateWindow(GetHWND());
			RedrawWindow(GetHWND(),NULL,NULL,RDW_UPDATENOW);
			*/
		}
	}
}