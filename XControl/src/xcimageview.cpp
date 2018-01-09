#include "stdafx.h"
#include "xcImageView.hpp"
#include "xframe.h"
#include "iximageview.hpp"

#include <gdiplus.h>
using namespace Gdiplus;

namespace Hxsoft{namespace XFrame
{
	xcImageView::xcImageView():m_bPlay(false),m_timer(50),m_nPlayType(0),m_Count(0),
		m_nWait(0),m_nStep(1),m_bSizing(false),m_mIndex(0),m_bOnlyOneShow(false),m_offset(0),
		m_bSBoerder(true),m_bCanOne(true),m_bEditLabel(false),m_pLabelEdit(NULL),m_bCanEditLabel(false),
		m_bSelectedMode(0),m_bShowLast(true)
	{
		m_ItemSize.cx = 128;
		m_ItemSize.cy = 128;
		m_ItemSizeEx.cx = 128;
		m_ItemSizeEx.cy = 128;
		m_Labelsize.cx = 0;
		m_Labelsize.cy = 16;
		m_TopLeft.x = 0;
		m_TopLeft.y = 0;
		::SetRect(&m_marginRect,12,12,12,12);
		m_spliterSize.cx = 16;
		m_spliterSize.cy = 16;
		m_mvstart.x = 0;
		m_mvstart.y = 0;

		m_mousePt.x = 0;
		m_mousePt.y = 0;

		m_pRowSelects = new XUtil::xuCsArray<INT32, BYTE>(65536,0);
		//m_pImageXml = new XUtil::xuCsArray<INT32, imageBlock *>(65536,NULL);

		// Initialize GDI+.
		gdiplusToken = 0;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		this->AttachListon(WM_PAINT,(FNEvent)&xcImageView::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcImageView::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcImageView::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcImageView::EvtMouseMove);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&xcImageView::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&xcImageView::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&xcImageView::EvtHScroll);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&xcImageView::EvtSetCursor);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&xcImageView::EvtDoubleClicked);
		this->AttachListon(WM_RBUTTONDBLCLK,(FNEvent)&xcImageView::EvtDoubleClickedEx);
		this->AttachListon(WM_TIMER,(FNEvent)&xcImageView::EvtTimer);
		//this->AttachListon(WM_KEYDOWN,(FNEvent)&xcImageView::EvtKeyDown);
		this->AttachListon(WM_CHAR,(FNEvent)&xcImageView::EvtChar);
	}

	xcImageView::~xcImageView()
	{
		//for(int i=0;i<(int)m_pImages.size();i++)
		//{
		//	//if(m_pImages[i])delete m_pImages[i];
		//}
		m_pImages.clear();
		m_ImageUrls.clear();
		m_LabelText.clear();

		if(m_pRowSelects) delete m_pRowSelects;
		//if(m_pImageXml) delete m_pImageXml;

		if(gdiplusToken != 0 )Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	int xcImageView::DoDraw(HDC hPaintDC,RECT * rc)
	{

		RECT rt ;
		if(rc)
			rt = *rc;
		else
			GetClientRect(&rt);

		HBRUSH hBrush;
		hBrush = ::CreateSolidBrush(RGB(255,255,255));
		::FillRect(hPaintDC,&rt,hBrush);
		::DeleteObject(hBrush);

		const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
		if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
			XUtil::Css::DrawCssFrame(hPaintDC,&rt,pBoxStyle);

		if(m_Count<1) return 1;

		rt.left   += m_marginRect.left;
		rt.right  -= m_marginRect.right;
		rt.top    += m_marginRect.top;
		rt.bottom -= m_marginRect.bottom;

		int startx = rt.left  ;
		int starty = rt.top ;

		int count = m_TopLeft.y;
		int widths,heights;
		int width,height;


		Image* pImage = NULL;
		Graphics graph(hPaintDC);
		graph.SetPageUnit(UnitPixel);

		if(m_bOnlyOneShow)
		{
			if(m_bPlay)
			{
				int cx = - m_offset;
				int cnt=0;
				int nIndex= m_mIndex;

L1:				//_stprintf_s(path,255,L"%s\\cache\\%d.jpg",mpath,nNumber +  nIndex);
				//pImage = Image::FromFile(path);

				if(nIndex >= m_Count -1)nIndex = 0;
				if(m_pImages[nIndex])
					pImage = (Image *)m_pImages[nIndex];
				else
				{
					this->LoadFromUrl(m_pxfNode->GetXPage()->m_pStrUrl,
						this->m_ImageUrls[nIndex].c_str(),m_pImages[nIndex]);
					pImage = (Image *)m_pImages[nIndex];
				}
				if(!pImage) return 1;
				width = pImage->GetWidth();
				height = pImage->GetHeight();
				cnt = 0;
				while(width <1)
				{
					//if(pImage)delete pImage;
					if(nIndex >= m_Count -1)
						nIndex = 0;
					else
						nIndex++;

					//_stprintf_s(path,255,L"%s\\cache\\%d.jpg",mpath,nNumber + nIndex);
					//pImage = Image::FromFile(path);
					if(m_pImages[nIndex])
						pImage = (Image *)m_pImages[nIndex];
					else
					{
						this->LoadFromUrl(m_pxfNode->GetXPage()->m_pStrUrl,
							this->m_ImageUrls[nIndex].c_str(),m_pImages[nIndex]);
						pImage = (Image *)m_pImages[nIndex];
					}
					if(!pImage) return 1;
					width = pImage->GetWidth();
					height = pImage->GetHeight();
					if(cnt >= m_Count -1)
					{
						//if(pImage)delete pImage;
						return true;
					}
					cnt++;
				}
				if(cx + width < startx)
				{
					m_mIndex++;
					nIndex= m_mIndex;
					cx += width;
					m_offset = 0;
					//if(pImage)delete pImage;
					goto L1;
				}
				if(cx < rt.right)
				{
					graph.DrawImage(pImage,startx - m_offset ,starty,width,height );
					//if(pImage)delete pImage;
					cx += width;
					startx += width + m_spliterSize.cx;
					if(nIndex >= m_Count -1)
						nIndex = 0;
					else
						nIndex++;
					goto L1;
				}else
				{
					//if(pImage)delete pImage;
					return true;
				}
				return true;
			}else
			{
				int nIndex = m_mIndex;
				//_stprintf_s(path,255,L"%s\\cache\\%d.jpg",mpath,nNumber + m_mIndex);
				//pImage = Image::FromFile(path);
				if(m_pImages[nIndex])
					pImage = (Image *)m_pImages[nIndex];
				else
				{
					this->LoadFromUrl(m_pxfNode->GetXPage()->m_pStrUrl,
						this->m_ImageUrls[nIndex].c_str(),m_pImages[nIndex]);
					pImage = (Image *)m_pImages[nIndex];
				}
				if(!pImage) return true;
				width = pImage->GetWidth();
				height = pImage->GetHeight();
				if(width > 0  && height > 0)
				{
					graph.DrawImage(pImage,startx,starty,width,height );
				}
				//if(pImage)delete pImage;
			}
			return 1;
		}

		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);
		HFONT hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hPaintDC, hFont);
		//delete hFont;
		HPEN hPen;
		hPen = ::CreatePen(PS_SOLID,1,RGB(218,218,218));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		int hbr = ::SetBkMode(hPaintDC,TRANSPARENT);

		if(m_ItemSize.cy <= 0 + this->m_Labelsize.cy)
			m_ItemSize.cy = rt.bottom - rt.top - 1;
		int rtheight = starty;
		if(!m_bShowLast) rtheight += m_ItemSize.cy;
		//while(starty /*+ m_ItemSize.cy*/ < rt.bottom)
		while(rtheight < rt.bottom)
		{
			if(m_ItemSize.cx <= 0)
				m_ItemSize.cx = rt.right - rt.left - m_Labelsize.cx -1;
			while(startx + m_ItemSize.cx < rt.right)
			{
				//_stprintf_s(path,255,L"%s\\cache\\%d.jpg",mpath,count + nNumber);
				//pImage = Image::FromFile(path);
				if(count >= (int)m_pImages.size()) continue;
				if(m_pImages[count])
					pImage = (Image *)m_pImages[count];
				else
				{
					this->LoadFromUrl(m_pxfNode->GetXPage()->m_pStrUrl,
						this->m_ImageUrls[count].c_str(),m_pImages[count]);
					pImage = (Image *)m_pImages[count];
				}
				if(pImage)
				{
					width = pImage->GetWidth();
					height = pImage->GetHeight();
					if(width > 0  && height > 0)
					{
						if(width/m_ItemSize.cx < height/m_ItemSize.cy)
						{
							heights = m_ItemSize.cy;
							widths = heights * width/height;   
							if(widths >= m_ItemSize.cx)
							{
								widths = m_ItemSize.cx;
								heights = widths * height/width;  
							}
						}else
						{
							widths = m_ItemSize.cx;
							heights = widths * height/width;   
							if(heights >= m_ItemSize.cy)
							{
								heights = m_ItemSize.cy;
								widths = heights * width/height;  
							}
						}
						graph.DrawImage(pImage,startx,starty ,widths,heights);
						//if(pImage)delete pImage;
					}

					if(((m_bSelectedMode == 1 && this->IsRowSelected(count)) || (m_bSelectedMode != 1 && count == m_mIndex)) && m_bSBoerder)
					{
						RECT FocRect;
						::SetRect(&FocRect,startx,starty ,startx+m_ItemSize.cx,starty+m_ItemSize.cy);

						HBRUSH hbrush = ::CreateSolidBrush(RGB(0,0,0));
						int height = FocRect.bottom - FocRect.top;
						int width = FocRect.right - FocRect.left;
						hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);

						::PatBlt(hPaintDC,FocRect.left - 2,FocRect.top - 2,3,m_ItemSize.cy + 4,PATCOPY);
						::PatBlt(hPaintDC,FocRect.right -1,FocRect.top - 2,3,m_ItemSize.cy - 1 ,PATCOPY);
						::PatBlt(hPaintDC,FocRect.left - 2,FocRect.top - 2 ,m_ItemSize.cx + 4,3,PATCOPY);
						::PatBlt(hPaintDC,FocRect.left - 2,FocRect.bottom - 1,m_ItemSize.cx - 1, 3,PATCOPY);
						::PatBlt(hPaintDC,FocRect.right - 2, FocRect.bottom - 2, 5 , 5,PATCOPY);

						hbrush =(HBRUSH)::SelectObject(hPaintDC,hbrush);
						::DeleteObject(hbrush);
					}
					else
					{
						::MoveToEx(hPaintDC,startx,starty,NULL);
						::LineTo(hPaintDC,startx + m_ItemSize.cx,starty);
						::LineTo(hPaintDC,startx + m_ItemSize.cx,starty + m_ItemSize.cy);
						::LineTo(hPaintDC,startx ,starty + m_ItemSize.cy);
						::LineTo(hPaintDC,startx ,starty);
					}
				}
				if(m_bEditLabel && count == m_mIndex)
				{
					if(!m_pLabelEdit)
					{
						RECT textRect;
						::SetRect(&textRect,startx,starty+m_ItemSize.cy-m_Labelsize.cy,startx+m_ItemSize.cx,starty+m_ItemSize.cy);					
						m_pLabelEdit = new  xcImageEdit();
						m_pLabelEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_BORDER;
						m_pLabelEdit->m_pszWinClass =_tcsdup(_T("edit"));
						m_pLabelEdit->CreateControl(NULL,textRect,(HWND)GetHWND(),0,xfApp::GetApp()->GetInstance());

						::SendMessage(m_pLabelEdit->GetHWND(),WM_SETFONT,(WPARAM)hFont,false);	
					}
					UpdateLabelText();
					::MoveWindow(m_pLabelEdit->GetHWND(),startx,starty+m_ItemSize.cy-m_Labelsize.cy,m_ItemSize.cx,m_Labelsize.cy,false);
					::ShowWindow(m_pLabelEdit->GetHWND(),true);
				}
				else
				{
					LPTSTR pLabelText= (LPTSTR)m_LabelText[count].c_str();
					if(m_Labelsize.cy > 0 && pLabelText)
					{
						RECT textRect;
						::SetRect(&textRect,startx,starty+m_ItemSize.cy-m_Labelsize.cy,startx+m_ItemSize.cx,starty+m_ItemSize.cy);					
						
						if(_tcschr(pLabelText,'\r'))
							::DrawTextW(hPaintDC,pLabelText,(int)::_tcslen(pLabelText),&textRect,DT_LEFT|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);							
						else
						{
							SIZE sz;
							GetTextExtentPoint32(hPaintDC,pLabelText,(int)::_tcslen(pLabelText),&sz);
							if(sz.cx < textRect.right - textRect.left)
								::DrawText(hPaintDC,pLabelText,(int)::_tcslen(pLabelText),&textRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_EDITCONTROL);
							else
								::DrawTextW(hPaintDC,pLabelText,(int)::_tcslen(pLabelText),&textRect,DT_LEFT|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
						}
					}
				}
				/*if(m_pImageXml->GetValue(count))
				{
					imageBlock * pItem = (imageBlock *)m_pImageXml->GetValue(count);
					RECT textRect;
					::SetRect(&textRect,startx,starty,startx+m_ItemSize.cx,starty+m_ItemSize.cy);					
					DoDrawBlock(hPaintDC, textRect, pItem);
				}*/
				startx += m_ItemSize.cx + m_spliterSize.cx;
				count ++;
				if(count >=m_Count) break;
			}
			startx = rt.left;
			starty += m_ItemSize.cy + m_spliterSize.cy;
			if(count >=m_Count) break;
			rtheight = starty;
			if(!m_bShowLast) rtheight += m_ItemSize.cy;
		}
		UpdateScrollPos();
		::SetBkMode(hPaintDC,hbr);
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		hFont = (HFONT)::SelectObject(hPaintDC, hFont);
		//delete hFont;
		return 1;
	}
/*
	int xcImageView::DoDrawBlock(HDC hPaintDC,RECT rc,imageBlock * pItem)
	{
		long p = 1.0 * m_ItemSizeEx.cx / m_ItemSize.cx;
		RECT rt = ((imageBlock *)pItem)->FRect;
		int height = rt.bottom - rt.top;
		int width = rt.right - rt.left;
		rt.top = rt.top * p + rc.top;
		rt.left = rt.left * p + rc.left;
		rt.right = rt.left + width * p;
		rt.bottom = rt.top + height * p;
		if(rt.bottom <= rc.top || rt.right <= rt.left || rt.top >= rc.bottom || rt.left >=rc.right) return 1;
		pItem->pControl->DoDraw(hPaintDC,&rt);
		if(pItem->pChilds)
		{
			for(int i=0; i<(int)pItem->pChilds->size(); i++)
				DoDrawBlock(hPaintDC, rc, (*pItem->pChilds)[i]);
		}
		return 1;
	}

	xcImageView::imageBlock * xcImageView::ParseXml(IXMLDOMElement * pElement)
	{
		imageBlock * pItem = NULL;
		BSTR bstr= NULL;
		pElement->get_tagName(&bstr);
		if(_tcsicmp(bstr,L"control")==0)
		{
			pItem = new imageBlock;
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
					pItem->pControl =  dynamic_cast<xfControl*>(pObject);
					pItem->pControl->Initial(pElement);

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
						if(nt==NODE_ELEMENT)
						{
							IXMLDOMElement* pElm;
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElm);
							imageBlock* bk = ParseXml(pElm);
							if(bk)
							{
								if(!pItem->pChilds) pItem->pChilds= new vector<imageBlock *>();
								pItem->pChilds->push_back(bk);
							}
						}
					}
				}
			}
		}
		return pItem;
	}
*/
	void xcImageView::Initial(IXMLDOMElement * pElement)
	{
		LPTSTR pImageSize = xbXml::GetStringAttribe(pElement,_T("imagesize"));
		if(pImageSize && _ttol(pImageSize) > 0)
		{
			int v = _ttol(pImageSize);
			this->m_ItemSize.cx =  v;
			this->m_ItemSize.cy =  v + this->m_Labelsize.cy;
			delete pImageSize;
		}

		LPTSTR pImageWidth = xbXml::GetStringAttribe(pElement,_T("imagewidth"));
		{
			if(pImageWidth /*&& _ttol(pImageWidth) > 0*/)
			{
				int v = _ttol(pImageWidth);
				this->m_ItemSize.cx =  v;
				this->m_ItemSizeEx.cx =  v;
				delete pImageWidth;
			}
		}
		LPTSTR pTextHeight = xbXml::GetStringAttribe(pElement,_T("textsize"));
		{
			if(pTextHeight && _ttol(pTextHeight) >= 0)
			{
				int v = _ttol(pTextHeight);
				this->m_Labelsize.cy = v;
				delete pTextHeight;
			}
		}
		LPTSTR pImageHeight = xbXml::GetStringAttribe(pElement,_T("imageheight"));
		{
			if(pImageHeight /*&& _ttol(pImageHeight) > 0*/)
			{
				int v = _ttol(pImageHeight);
				this->m_ItemSize.cy =  v + this->m_Labelsize.cy;
				this->m_ItemSizeEx.cy =  v + this->m_Labelsize.cy;
				delete pImageHeight;
			}
		}
		LPTSTR pTextEdit = xbXml::GetStringAttribe(pElement,_T("textedit"));
		if(pTextEdit && _tcscmp(pTextEdit, L"1") == 0)
		{
			this->m_bCanEditLabel = true;
			delete pTextEdit;
		}
		LPTSTR pHasSBorder = xbXml::GetStringAttribe(pElement,_T("selectborder"));
		if(pHasSBorder && _tcscmp(pHasSBorder, L"1") == 0)
		{
			this->m_bSBoerder = true;
			delete pHasSBorder;
		}
		LPTSTR pHasToBig = xbXml::GetStringAttribe(pElement,_T("canone"));
		if(pHasToBig && _tcscmp(pHasToBig, L"0") == 0)
		{
			this->m_bCanOne = false;
			delete pHasToBig;
		}
		LPTSTR pShowLast = xbXml::GetStringAttribe(pElement,_T("showlast"));
		if(pShowLast && _tcscmp(pShowLast, L"0") == 0)
		{
			this->m_bShowLast = false;
			delete pShowLast;
		}


		/*
		LPTSTR pCount = xbXml::GetStringAttribe(pElement,_T("count"));
		{
			if(pCount && _ttol(pCount) >= 0)
			{
				int v = _ttol(pCount);
				m_Count = v;
				delete pCount;
			}
		}*/
	}

	void xcImageView::Initial()
	{
		Initial(GetXfNode()->m_pElement);
	}

	bool xcImageView::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		if(m_bOnlyOneShow) return true;
		if(!m_bCanOne) return true;

		if(m_bSizing)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_SIZEALL);
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}else
		{
			POINT point = pEvent->pt;
			ScreenToClient(GetHWND(),&point);
			int mx = point.x;
			int my = point.y;
			int nIndex = this->GetItemAtPoint(mx,my);
			if(::GetFocus()!= GetHWND()) ::SetFocus(GetHWND());
			if(nIndex>=0)
			{
				RECT rt;
				this->GetItemRect(nIndex,rt);
				if(abs(rt.right - mx) < 4 && abs(rt.bottom - my) < 4)
				{
					HCURSOR hCursor = LoadCursor(NULL,IDC_SIZEALL);
					hCursor = ::SetCursor(hCursor);
					pEvent->dwRet = 1;
				}
			}
		}
		return true;
	}

	int xcImageView::TopLeftMoved(POINT pt)
	{
		m_TopLeft = pt;
		::InvalidateRect(GetHWND(),NULL,false);
		return 1;
	}

	bool xcImageView::EvtMouseWheel(TEvent* pEvent,LPARAM lParam)
	{
		if(m_bOnlyOneShow)
		{
			if(pEvent->wheel.zDelta > 0)
			{
				if(m_mIndex >0)
				{
					m_mIndex--;
					::InvalidateRect(GetHWND(),NULL,false);
				}
			}else
			{
				if(m_mIndex < m_Count -1)
				{
					m_mIndex++;
					::InvalidateRect(GetHWND(),NULL,false);
				}
			}
		}
		POINT pt;
		pt = m_TopLeft;

		RECT rc;
		GetClientRect(&rc);
		int nAmt = GetHNumberPerPages(rc);
		if(nAmt < 1)nAmt = 1;
		if(pEvent->wheel.zDelta > 0)
		{
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
			/*if(pt.y > nAmt )
			{
				pt.y -= nAmt;
				TopLeftMoved(pt);
			}else
			{
				if(pt.y > 0)
				{
					pt.y = 0;
					TopLeftMoved(pt);
				}

			}*/

		}
		else
		{
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
			/*if(pt.y < m_Count - 1)
			{
				if(pt.y + nAmt <m_Count -1)
				{
					pt.y += nAmt;
					TopLeftMoved(pt);
				}
			}*/
		}
		return true;
	}

	bool xcImageView::EvtHScroll(TEvent* pEvent,LPARAM lParam)
	{
		return true;
	}
	bool xcImageView::EvtVScroll(TEvent* pEvent,LPARAM lParam)
	{
		ModifyScrollBar(SB_VERT,pEvent->scroll.nSBCode,pEvent->scroll.nPos);
		return true;
	}

	bool xcImageView::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		RECT	rc;
		HDC hPaintDC = pEvent->paint.hdc;
		GetClientRect(&rc);
		
		HDC hMemDC = ::CreateCompatibleDC(hPaintDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(hPaintDC,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);

		DoDraw(hMemDC,&rc);
		::BitBlt(hPaintDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top + 1,hMemDC,rc.left,rc.top,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hMemDC);
		

		return true;
	}

	int xcImageView::DrawSizingLine(int sx,int sy ,POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_SOLID,1,RGB(0,128,192));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		::MoveToEx(hDC,sx,sy,NULL);
		::LineTo(hDC,pt.x,sy);
		::LineTo(hDC,pt.x,pt.y);
		::LineTo(hDC,sx,pt.y);
		::LineTo(hDC,sx,sy);

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);

		return 1;
	}

	int xcImageView::SelectRow(int nRow,bool bSelect)
	{
		if(nRow==0)
		{
			if(bSelect)
			{
				m_pRowSelects->SetValue(1,GetCount(),1);
				return 1;
			}else
			{
				m_pRowSelects->Reset(0);
				return 1;
			}
		}
		if(nRow > GetCount()) return -1;
		if(nRow < 1) return -1;

		m_pRowSelects->SetValue(nRow -1,bSelect?1:0);
		return 1;
	}
	
	int xcImageView::SelectRow(int sRow,int eRow, bool bSelect)
	{
		int nRow1 = sRow < eRow ? sRow:eRow;
		int nRow2 = sRow > eRow ? sRow:eRow;
		
		if(nRow2 < 1) return -1;
		if(nRow1 > GetCount()) return -1;
		if(nRow1 < 1) nRow1 = 1;
		if(nRow2 > GetCount()) nRow2 = GetCount();
		m_pRowSelects->SetValue(nRow1 -1 ,nRow2 - 1, bSelect?1:0);

		return 1;
	}

	int xcImageView::GetNextSelectRow(int nStartRow)
	{
		nStartRow++;
		if(nStartRow < 1) nStartRow = 1 ;
		int Rows = GetCount();
		while(nStartRow <= Rows)
		{
			if(m_pRowSelects->GetValue(nStartRow - 1) ==1) return nStartRow;
			nStartRow ++;
		}
		return -1;
	}

	bool xcImageView::IsRowSelected(int nRow)
	{
		nRow++;
		if(nRow < 1) return false;
		if(nRow >GetCount())return false;
		return m_pRowSelects->GetValue(nRow - 1)==1?true:false;
	}

	bool xcImageView::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		if(m_bPlay)
		{
			m_bPlay = false;
			::KillTimer(GetHWND(),10000);
		}
		if(m_bOnlyOneShow) return true;

		int mx = pEvent->mouse.point.x;
		int my = pEvent->mouse.point.y;
		int nIndex = this->GetItemAtPoint(mx,my);
		if(::GetFocus()!= GetHWND()) ::SetFocus(GetHWND());
		if(nIndex>=0)
		{
			RECT rt;
			this->GetItemRect(nIndex,rt);
			if(abs(rt.right - mx) < 8 && abs(rt.bottom - my) < 8)
			{
				m_mousePt = pEvent->mouse.point;
				m_mvstart.x = rt.left;
				m_mvstart.y = rt.top;
				m_bSizing = true;
				this->SetCapture();
				DrawSizingLine(m_mvstart.x,m_mvstart.y,m_mousePt);
				return true;
			}
			if(m_bCanEditLabel)
			{
				if(my > rt.bottom - m_Labelsize.cy)
				{
					m_bEditLabel = true;
					UpdateImageText();
				}
				else
				{
					m_bEditLabel = false;
					if(m_pLabelEdit)
					{
						UpdateImageText();
						::DestroyWindow(m_pLabelEdit->GetHWND());
						m_pLabelEdit = NULL;
					}
				}
			}

			if(m_bSelectedMode == 1)
			{
				if(pEvent->mouse.nFlags & MK_SHIFT)
				{
					if(m_mIndex >= 0)
						this->SelectRow(m_mIndex+1,nIndex+1,true);
					else
						this->SelectRow(nIndex+1,true);
				}else if(pEvent->mouse.nFlags & MK_CONTROL)
				{
					this->SelectRow(nIndex+1,!IsRowSelected(nIndex));
				}else
				{
					this->SelectRow(0,false);
					this->SelectRow(nIndex+1,true);				
				}
			}

			if(m_mIndex != nIndex && nIndex < (int)this->m_ImageUrls.size())
			{
				m_mIndex = nIndex;

				IMGNNMHDR evt;
				memset(&evt,0,sizeof(IMGNNMHDR));
				evt.hwndFrom = this->m_hWnd;
				evt.path = (LPTSTR)this->m_ImageUrls[nIndex].c_str();
				evt.text = (LPTSTR)this->m_LabelText[nIndex].c_str();
				evt.index = nIndex;
				evt.code = IMG_LCLICK;
				::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
			}
			::InvalidateRect(GetHWND(),NULL,false);
		}
		return true;
	}

	bool xcImageView::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
	{
		if(m_bSizing)
		{
			this->ReleaseCapture();
			m_mousePt = pEvent->mouse.point;
			DrawSizingLine(m_mvstart.x,m_mvstart.y,m_mousePt);
			m_bSizing = false;

			RECT rc;
			GetItemRect(m_mIndex,rc);
			if(abs(rc.left - m_mvstart.x)>= 4 || abs(rc.right - m_mvstart.y)>= 4)
			{
				if(abs(m_mousePt.x - m_mvstart.x)>= 16 && abs(m_mousePt.y - m_mvstart.y)>= 16)
				{
					m_ItemSize.cx = m_mousePt.x - m_mvstart.x;
					m_ItemSize.cy = m_mousePt.y - m_mvstart.y;
				}
				if(!GetItemRect(m_mIndex,rc)) m_TopLeft.y = m_mIndex;
				::InvalidateRect(GetHWND(),NULL,true);
			}
		}
		return true;
	}

	bool xcImageView::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
	{
		if(m_bSizing)
		{
			DrawSizingLine(m_mvstart.x,m_mvstart.y,m_mousePt);
			m_mousePt = pEvent->mouse.point;
			DrawSizingLine(m_mvstart.x,m_mvstart.y,m_mousePt);
		}
		return true;
	}

	int xcImageView::GetCount()
	{
		return m_Count;
	}

	int xcImageView::SetCount(int count)
	{
		m_Count = count;
		return 1;
	}
	
	int xcImageView::GetHNumberPerPages(RECT &rc)
	{
		int count = 0;
		int cx = rc.right - rc.left - m_marginRect.left - m_marginRect.right;

		while(true)
		{
			if(cx >= m_ItemSize.cx)
			{
				count ++;
				cx -= m_ItemSize.cx + m_spliterSize.cx;
			}else
				break;
		}
		return count;

	}
	int xcImageView::GetVNumberPerPages(RECT &rc)
	{
		int count = 0;
		int cy = rc.bottom - rc.top - m_marginRect.top - m_marginRect.bottom;
		
		while(true)
		{
			if(cy >= m_ItemSize.cy)
			{
				count ++;
				cy -= m_ItemSize.cy + m_spliterSize.cy;
			}else
				break;
		}
		return count;
	}

	bool xcImageView::GetItemRect(int nIndex,RECT &rc)
	{
		int start;
		int end;
		RECT rt;
		GetClientRect(&rt);
		start = m_TopLeft.y;

		int sx = GetHNumberPerPages(rt);
		int sy = GetVNumberPerPages(rt);
		end = start + sx * sy  - 1;
		if(nIndex >= start && nIndex <= end) 
		{
			nIndex -= start;
			rc = rt;
			rc.top += m_marginRect.top;
			rc.left += m_marginRect.left;

			int nRow =  nIndex /sx; 
			int nCol = nIndex % sx;
			rc.left = nCol * m_ItemSize.cx + nCol * m_spliterSize.cx + m_marginRect.left;
			rc.top  = nRow * m_ItemSize.cy + nRow * m_spliterSize.cy + m_marginRect.top;

			rc.right = rc.left + m_ItemSize.cx;
			rc.bottom = rc.top + m_ItemSize.cy;
			return true;
		}else
			return false;
	}
	
	int  xcImageView::GetItemAtPoint(int x,int y)
	{
		RECT rt;
		GetClientRect(&rt);
		
		if(x < rt.left + m_marginRect.left || y < rt.top + m_marginRect.top) return -1;
		if(x > rt.right  + m_marginRect.right  || y > rt.bottom + m_marginRect.bottom) return -1;

		x -= rt.left + m_marginRect.left;
		y -= rt.top + m_marginRect.top;
		
		int sx = GetHNumberPerPages(rt);
		int sy = GetVNumberPerPages(rt);

		int nRow =  (y -1) / (m_ItemSize.cy + m_spliterSize.cy);
		int nCol =  (x -1) / (m_ItemSize.cx + m_spliterSize.cx);
		if(nCol >= sx) return -1;
		if(nRow < 0) nRow = 0;
		if(nCol < 0) nCol = 0;

		x -= nCol * (m_ItemSize.cx + m_spliterSize.cx);
		y -= nRow * (m_ItemSize.cy + m_spliterSize.cy);

		if(x > m_ItemSize.cx ) return -1;
		if(y > m_ItemSize.cy ) return -1;

		
		return nRow * sx + nCol + m_TopLeft.y;
	}

	bool xcImageView::EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam)
	{
		if(m_bPlay)
		{
			m_bPlay = false;
			::KillTimer(GetHWND(),10000);
		}

		int mx = pEvent->mouse.point.x;
		int my = pEvent->mouse.point.y;
		int nIndex = this->GetItemAtPoint(mx,my);

		if(nIndex >=0 && nIndex < (int)this->m_ImageUrls.size())
		{
			IMGNNMHDR evt;
			memset(&evt,0,sizeof(IMGNNMHDR));
			evt.hwndFrom = this->m_hWnd;
			evt.path = (LPTSTR)this->m_ImageUrls[nIndex].c_str();
			evt.text = (LPTSTR)this->m_LabelText[nIndex].c_str();
			evt.index = nIndex;
			evt.code = IMG_LDBCLICK;
			::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
		}

		if(!m_bCanOne)
		{
			if(m_bOnlyOneShow)
			{
				m_bOnlyOneShow = false;
				::InvalidateRect(GetHWND(),NULL,false);
			}
			return true;
		}
		if(!m_bOnlyOneShow)
		{
			if(nIndex >= 0)
			{
				m_mIndex = nIndex;
				m_bOnlyOneShow = true;
				::InvalidateRect(GetHWND(),NULL,false);
			}
		}else
		{
			m_bOnlyOneShow = false;
			::InvalidateRect(GetHWND(),NULL,false);
		}

		return true;
	}

	bool xcImageView::EvtDoubleClickedEx(TEvent *pEvent ,LPARAM lParam)
	{
		if(!m_bPlay)
		{
			m_bPlay = true;
			m_offset = 0;

			int mx = pEvent->mouse.point.x;
			int my = pEvent->mouse.point.y;
			int nIndex = this->GetItemAtPoint(mx,my);
			if(nIndex >= 0)
			{
				m_mIndex = nIndex;
			}else
				m_mIndex = m_TopLeft.x;
			m_bOnlyOneShow = true;
			::InvalidateRect(GetHWND(),NULL,false);
			::SetTimer(GetHWND(),10000,m_timer,NULL);
		}else
		{
			m_bPlay = false;
			::KillTimer(GetHWND(),10000);
		}
		return true;
	}
	
	bool xcImageView::EvtTimer(TEvent *pEvent ,LPARAM lParam)
	{
		m_offset += m_nStep;
		ScrollWindowEx(GetHWND(),- m_nStep,0,NULL,NULL,NULL,NULL,SW_INVALIDATE);
		UpdateWindow(GetHWND());
		return true;
	}

	bool xcImageView::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		int k = pEvent->key.nChar;
		//向上翻页 
		if (k == 33 || k==38 || k==37)
		{
			ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
		}		
		//向下翻页
		else if (k == 32 || k==34 || k==40 || k==39)
		{
			ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
		}	
		return true;
	}

	int xcImageView::Preview(int percent)
	{
		if(m_bPlay)m_nStep *= percent / 100.0;
		else if(!m_bOnlyOneShow)
		{
			m_ItemSize.cx = m_ItemSizeEx.cx * percent / 100.0;
			m_ItemSize.cy = m_ItemSizeEx.cy * percent / 100.0;
			::InvalidateRect(GetHWND(),NULL,false);
		}
		return 1;	
	}

	bool xcImageView::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{
		if(pEvent->key.nChar=='+' || pEvent->key.nChar=='=')
		{
			if(m_bPlay)m_nStep++;
			else if(!m_bOnlyOneShow)
			{
				m_ItemSize.cx += 2;
				m_ItemSize.cy += 2;
				::InvalidateRect(GetHWND(),NULL,false);
			}
		}
		else if(pEvent->key.nChar=='_' || pEvent->key.nChar=='-')
		{
			if(m_bPlay && m_nStep > 1)
				m_nStep--;
			else if(!m_bOnlyOneShow)
			{
				if(m_ItemSize.cx > 8) m_ItemSize.cx-= 2;
				if(m_ItemSize.cy > 8) m_ItemSize.cy-= 2;
				::InvalidateRect(GetHWND(),NULL,false);
			}
		}
		return true;
	}

	int xcImageView::AddImages(LPCTSTR pUrl, LPTSTR pText)
	{
		m_ImageUrls.push_back(pUrl);
		m_pImages.push_back(NULL);
		m_LabelText.push_back(pText);
		m_Count++;
		return 1;
	}

	int xcImageView::InsertImages(LPCTSTR pUrl, LPTSTR pText, int nIndex)
	{
		if(nIndex < 0 || nIndex >= this->GetCount())
			return AddImages(pUrl, pText);

		m_ImageUrls.insert(m_ImageUrls.begin() + nIndex, pUrl);
		m_pImages.insert(m_pImages.begin() + nIndex, NULL);
		m_LabelText.insert(m_LabelText.begin() + nIndex, pText);
		m_Count++;
		return 1;
	}

	int xcImageView::RemoveImage(int nIndex)
	{
		//if(m_pImages[nIndex])delete m_pImages[nIndex];
		m_pImages.erase(m_pImages.begin() + nIndex);
		m_ImageUrls.erase(m_ImageUrls.begin() + nIndex);
		m_LabelText.erase(m_LabelText.begin() + nIndex);
		m_Count--;
		return 1;
	}

	int xcImageView::Reset()
	{
		/*for(int i=1;i<(int)m_pImages.size();i++)
		{
			if(m_pImages[i])delete m_pImages[i];
		}*/
		m_pImages.clear();
		m_ImageUrls.clear();
		m_LabelText.clear();

		m_pRowSelects->Reset(0);
		//m_pImageXml->Reset(NULL);

		m_bPlay=false;
		m_timer=50;
		m_nPlayType=0;
		m_Count=0;
		m_nWait=0;
		m_nStep=1;
		m_bSizing=false;
		m_mIndex=0;
		m_bOnlyOneShow=false;
		m_offset=0;
		
		m_TopLeft.x = 0;
		m_TopLeft.y = 0;

		UpdateScrollPos();

		return 1;
	}

	bool xcImageView::LoadFromByte(byte * pData,int nLen,void * &pImage)
	{
		CByteStream * pStream = NULL;
		pStream = new CByteStream(pData,nLen);
		pImage = Image::FromStream(pStream);
		
		return true;
	}

	bool xcImageView::LoadFromUrl(LPCTSTR pServerUrl,LPCTSTR pUrl,void * &pImage)
	{
		byte * pData = NULL;
		int nLen;

		pImage = NULL;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		if(pServerUrl)
			xfUriLoader::UrlCombineW(pServerUrl,pUrl,strUrl,&dwSize,0);
		else
			_tcscpy_s(strUrl,INTERNET_MAX_URL_LENGTH,pUrl);
		if(xfUriLoader::Load(strUrl,NULL,pData,nLen,false)==1)
		{
			LoadFromByte(pData,nLen,pImage);
			return true;
		}
		if(pData) xfUriLoader::FreeData(pData);
		return false;
	}
	
	xbObject * xcImageView::GetInterface()
	{
		if(!m_pInterFace)
		{
			IXImageView * pIXImageView = new IXImageView();
			m_pInterFace = (xbObject *)pIXImageView;
			pIXImageView->m_pControl = this;
		}
		return this->m_pInterFace;
	}

	void xcImageView::UpdateScrollPos()
	{
		RECT rt;
		GetClientRect(&rt);
		int sx = GetHNumberPerPages(rt);
		int sy = GetVNumberPerPages(rt);

		int rows = sy<=0?1:sy;
		int cols = sx<=0?1:sx;
		int count = this->GetCount();

		if(rows * cols < count)
		{
			SCROLLINFO si; 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE; 
			si.nMin   = 0; 
			si.nMax   = count / cols + 1;
			si.nPage  = rows; 
			SetScrollInfo(GetHWND(), SB_VERT, &si, true); 
			ShowScrollBar(GetHWND(),SB_VERT,true);
		}
		else
			ShowScrollBar(GetHWND(),SB_VERT,false);
	}
	void xcImageView::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
	{
		RECT rt;
		GetClientRect(&rt);
		int sx = GetHNumberPerPages(rt);
		int sy = GetVNumberPerPages(rt);

		int Value = m_TopLeft.y;
		int Result;
		Result = Value;
		switch(ScrollCode) 
		{
		case SB_LINEUP:
			Result = Value - sx;
			break;
		case SB_LINEDOWN:
			Result = Value + sx;
			break;
		case SB_PAGEUP:
			Result = Value - sx * sy;
			break;
		case SB_PAGEDOWN:
			Result = Value + sx * sy;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			SCROLLINFO si; 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_ALL;
			GetScrollInfo (GetHWND(), ScrollBar, &si);
			Result = si.nTrackPos * sx;
			break;
		case SB_BOTTOM:
			Result = Value + sx;
			break;
		case SB_TOP:
			Result = Value - sx;
			break;
		}
		if(Result < 0) Result = 0;
		if(Result >= GetCount()) return;
		POINT pt;
		pt.y = Result;
		pt.x = m_TopLeft.x;
		SetScrollPos(GetHWND(), ScrollBar,Result / sx,true);
		TopLeftMoved(pt);
	}

	int xcImageView::UpdateLabelText()
	{
		if(m_pLabelEdit)
		{
			int nIndex = m_mIndex;
			LPTSTR pText = (LPTSTR)m_LabelText[nIndex].c_str();
			if(pText)
			{
				::SetWindowText(m_pLabelEdit->GetHWND(), pText);
				::SendMessage(m_pLabelEdit->GetHWND(),EM_SETSEL,0,_tcslen(pText));
			}
			else
				::SetWindowText(m_pLabelEdit->GetHWND(), L"");
		}
		return 1;
	}

	int xcImageView::UpdateImageText()
	{
		if(m_pLabelEdit)
		{
			int dwLen = GetWindowTextLength(m_pLabelEdit->GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(m_pLabelEdit->GetHWND(),pStr,dwLen + 1);

			int nIndex = m_mIndex;
			m_LabelText[nIndex] = ::_tcsdup(pStr);

			IMGNNMHDR evt;
			memset(&evt,0,sizeof(IMGNNMHDR));
			evt.hwndFrom = this->m_hWnd;
			evt.path = (LPTSTR)this->m_ImageUrls[nIndex].c_str();
			evt.text = (LPTSTR)this->m_LabelText[nIndex].c_str();
			evt.index = nIndex;
			evt.code = IMG_LABEL;
			::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
		}
		return 1;	
	}
/*
	void xcImageView::AddImageXml(int nRow, IXMLDOMElement * pXml)
	{
		imageBlock * pItem = ParseXml(pXml);
		m_pImageXml->SetValue(nRow,pItem);
	}*/
}}