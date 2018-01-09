#include "stdafx.h"
#include "xcToolButton.hpp"
#include "xframe.h"
#include <atlimage.h>

namespace Hxsoft
{
	namespace XFrame
	{
		xcToolButton::xcToolButton():m_bHover(false),m_bChecked(false),m_pAgentNodeTool(NULL),m_pSrc(NULL),m_TipHWND(0)
		{
			m_bTransparent = true;
			this->AttachListon(WM_ERASEBKGND,(FNEvent)&xcToolButton::EvtEraseBkgn);
			this->AttachListon(WM_PAINT,(FNEvent)&xcToolButton::EvtPaint);
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcToolButton::EvtMouseMove);
			this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcToolButton::EvtMouseLeave);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcToolButton::EvtLButtonUp);
			//this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcToolButton::EvtLButtonUp);
			this->AttachListon(WM_TIMER,(FNEvent)&xcToolButton::EvtTimer);
			this->AttachListon(WM_SETFOCUS, (FNEvent)&xcToolButton::EvtSetFocus);
			this->AttachListon(WM_KILLFOCUS, (FNEvent)&xcToolButton::EvtKillFocus);
			this->AttachListon(WM_KEYDOWN, (FNEvent)&xcToolButton::EvtKeyDown);
		}
		xcToolButton::~xcToolButton()
		{
		}

		bool xcToolButton::EvtEraseBkgn(TEvent* pEvent,LPARAM lParam)
		{
			pEvent->dwRet = 1;
			return true;
		}

		int xcToolButton::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rect;
			if(pDrawRect)
				rect = *pDrawRect;
			else
				GetClientRect(&rect);

			EImageFlow eImageFlow = ImageFlow_Left;
			LPTSTR pStrImageFlow = xbXml::GetStringAttribe(m_pxfNode->m_pElement,_T("imageflow"));

			if(pStrImageFlow)
			{
				if(::_tcsicmp(pStrImageFlow,_T("left"))==0)
					eImageFlow = ImageFlow_Left;
				else if(::_tcsicmp(pStrImageFlow,_T("right"))==0)
					eImageFlow = ImageFlow_Right;
				else if(::_tcsicmp(pStrImageFlow,_T("top"))==0)
					eImageFlow = ImageFlow_Top;
				else if(::_tcsicmp(pStrImageFlow,_T("bottom"))==0)
					eImageFlow = ImageFlow_Bottom;
				else if(::_tcsicmp(pStrImageFlow,_T("none"))==0)
					eImageFlow = ImageFlow_None;
				else if(::_tcsicmp(pStrImageFlow,_T("nonetext"))==0)
					eImageFlow = ImageFlow_NoneText;
				else if(::_tcsicmp(pStrImageFlow,_T("backgound"))==0)
					eImageFlow = ImageFlow_NoneText;

				if(pStrImageFlow) delete pStrImageFlow;
			}

			rect.left += 1;
			rect.top += 1;

			if(this->m_bHover)
			{
				rect.left -= 1;
				rect.top -= 1;
				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(49,106,197));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				HBRUSH hBrush = ::CreateSolidBrush(RGB(193,210,238));
				hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
				::Rectangle(hPaintDC,rect.left,rect.top,rect.right - 1,rect.bottom - 1);
				hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
				::DeleteObject(hBrush);
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
			if(m_bChecked)
			{
				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(49,106,197));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				HBRUSH hBrush = ::CreateSolidBrush(RGB(193,210,238));
				hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
				::Rectangle(hPaintDC,rect.left,rect.top,rect.right - 1,rect.bottom - 1);
				hBrush = (HBRUSH)::SelectObject(hPaintDC,hBrush);
				::DeleteObject(hBrush);
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}

			RECT sRect;
			RECT tRect;
			tRect = rect;
			if(((xfNodeControl *)m_pxfNode)->m_ImagePos >=0 || m_pSrc)
			{
				int cx=0,cy=0;
				//HIMAGELIST hImageList;
				Hxsoft::XUtil::xuVector<HIMAGELIST> & hImageLists = m_pxfNode->GetXPage()->m_pResourceMgr->GetImageList();
				if(((xfNodeControl *)m_pxfNode)->m_ImageIndex >=0 && ((xfNodeControl *)m_pxfNode)->m_ImageIndex < hImageLists.size())
				{
					int cx,cy;
					HIMAGELIST hImageList = hImageLists[((xfNodeControl *)m_pxfNode)->m_ImageIndex];
					ImageList_GetIconSize(hImageList,&cx,&cy);
					sRect = rect;

					if(eImageFlow==ImageFlow_Right)
					{
						sRect.left = rect.right - 1 - cx;
						sRect.top = rect.top + (rect.bottom - rect.top)/2 - cy/2;
						tRect.right = sRect.left -1;

					}
					else if(eImageFlow==ImageFlow_Top)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.top + 1;
						tRect.top = sRect.top + cy + 1;
					}
					else if(eImageFlow==ImageFlow_Bottom)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.bottom - 1 - cy;
						tRect.bottom = sRect.top - 1;
					}
					else if(eImageFlow==ImageFlow_Background)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.top + (rect.bottom - rect.top)/2 - cy/2;
					}else
					{
						sRect.left = rect.left + 1;
						sRect.top = rect.top + (rect.bottom - rect.top)/2 - cy/2;
						tRect.left = sRect.left + cx;
					}
					if(eImageFlow!=ImageFlow_None)
						::ImageList_Draw(hImageList,((xfNodeControl *)m_pxfNode)->m_ImagePos,hPaintDC, sRect.left,sRect.top ,ILD_NORMAL);
				}
				if(m_pSrc)
				{
					sRect = rect;
					TCHAR path[MAX_PATH];
					::GetModuleFileName(NULL,path,MAX_PATH);
					::PathRemoveFileSpec(path);
					TCHAR buf[MAX_PATH];
					::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\%s"), path, m_pSrc);

					CImage img;
					img.Load(buf);
					cx = img.GetWidth();
					cy = img.GetHeight();
					/*
					HBITMAP	hBmp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					BITMAP bmap;  
					GetObject(hBmp,sizeof(BITMAP),&bmap);  
					cx = bmap.bmWidth;
					cy = bmap.bmHeight;
					*/
					sRect = rect;
					if(eImageFlow==ImageFlow_Right)
					{
						sRect.left = rect.right - 1 - cx;
						sRect.top = rect.top + (rect.bottom - rect.top)/2 - cy/2;
						tRect.right = sRect.left -1;

					}
					else if(eImageFlow==ImageFlow_Top)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.top + 1;
						tRect.top = sRect.top + cy + 1;
					}
					else if(eImageFlow==ImageFlow_Bottom)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.bottom - 1 - cy;
						tRect.bottom = sRect.top - 1;
					}
					else if(eImageFlow==ImageFlow_Background)
					{
						sRect.left = rect.left + (rect.right - rect.left)/2 - cx/2;
						sRect.top  = rect.top + (rect.bottom - rect.top)/2 - cy/2;
					}else
					{
						sRect.left = rect.left + 1;
						sRect.top = rect.top + (rect.bottom - rect.top)/2 - cy/2;
						tRect.left = sRect.left + cx;
					}

					if(!img.IsNull())
					{
						if (img.GetBPP() == 32) //确认该图像包含Alpha通道  png透明
						{
							int i;
							int j;
							for (i = 0; i < img.GetWidth(); i++)
							{
								for (j = 0; j < img.GetHeight(); j++)
								{
									byte *pByte = (byte *)img.GetPixelAddress(i, j);
									pByte[0] = pByte[0] * pByte[3] / 255;
									pByte[1] = pByte[1] * pByte[3] / 255;
									pByte[2] = pByte[2] * pByte[3] / 255;
								}
							}
						}
						//img.TransparentBlt(hPaintDC,sRect.left,sRect.top,img.GetWidth(),img.GetHeight(),RGB(255,255,255));
						img.Draw(hPaintDC, sRect.left,sRect.top);
						img.Destroy();
					}
					/*
					HDC hMemDC = CreateCompatibleDC(hPaintDC);
					hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
					
					BitBlt(hPaintDC,sRect.left,sRect.top,cx,cy,hMemDC,0,0,SRCCOPY);

					hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
					::DeleteObject(hBmp);
					::DeleteDC(hMemDC);
					*/
				}
			}
			if(m_pxfNode && m_pxfNode->m_pCaption)
			{
				::SetBkMode(hPaintDC,TRANSPARENT);
				
				LOGFONT	logFont;
				const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
				XUtil::Css::GetFont(pBoxStyle,logFont);
		
				HFONT	hFont =::CreateFontIndirect(&logFont);
				hFont = (HFONT)::SelectObject(hPaintDC, hFont);

				COLORREF color = RGB(0,0,0);
				if(pBoxStyle && pBoxStyle->mask & cssMaskColor && pBoxStyle->pColor)
					color = ::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
				DWORD Align = DT_LEFT;
				if(pBoxStyle && pBoxStyle->pText && pBoxStyle->pText->mask & cssMaskTextAlign)
				{
					if(pBoxStyle->pText->textalign==XUtil::Css::_center)Align = DT_CENTER;
				}

				if(eImageFlow==ImageFlow_Bottom || eImageFlow==ImageFlow_Top || eImageFlow==ImageFlow_Background)
					::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&tRect, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_CENTER);
				else
				{
					tRect.left += 4;
					::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&tRect, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|Align);
				}
				hFont = (HFONT)::SelectObject(hPaintDC, hFont);
				::DeleteObject(hFont);
				if(::GetFocus()==this->GetHWND())
				{
					::InflateRect(&rect,-1,-1);
					DrawFocusRect(hPaintDC,&rect);
				}
				if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
					::SetTextColor(hPaintDC,color);
			}
			return 1;
		}

		bool xcToolButton::EvtPaint(TEvent* pEvent,LPARAM lParam)
		{
			HDC hdc = pEvent->paint.hdc;
			RECT	rect;
			this->GetClientRect(&rect);

			HBITMAP hOldBmp = NULL;
			HBITMAP hBmp = NULL;
			POINT pt;

			xfNodeControl* pNode = (xfNodeControl *)m_pxfNode;
			HDC hMemDC = ::CreateCompatibleDC(hdc);
			
			hBmp = ::CreateCompatibleBitmap(hdc,rect.right - rect.left + 1,rect.bottom - rect.top +1);
			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			if(pNode->m_hBackGround)
			{
				HDC hDC = ::CreateCompatibleDC(hdc);
				HBITMAP hBmp = (HBITMAP)pNode->m_hBackGround;
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::BitBlt(hMemDC,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hDC,rect.left,rect.top,SRCCOPY);
				hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
				::DeleteDC(hDC);
			}

			::SetViewportOrgEx(hMemDC,- pNode->m_ContentRect.left,- pNode->m_ContentRect.top,&pt);
			((xfNodeControl *)m_pxfNode)->DrawNodeEx(hMemDC); 
			::SetViewportOrgEx(hMemDC,pt.x,pt.y,NULL);

			this->DoDraw(hMemDC);
			::BitBlt(hdc,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1,hMemDC,rect.left,rect.top,SRCCOPY);

			hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hMemDC);

			return true;
		}

		bool xcToolButton::EvtKeyDown(TEvent* pEvent,LPARAM lParam)
		{
			if(pEvent->key.nChar==VK_SPACE)
			{
				this->SwitchLayer();
				PostMessage(GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)m_pxfNode->m_strNodeName,(LPARAM)this);
			}
			return true;
		}

		bool xcToolButton::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
		{
			if(!m_bHover)
			{
				TRACKMOUSEEVENT	tve;
				tve.cbSize = sizeof(TRACKMOUSEEVENT);
				tve.dwFlags = TME_LEAVE;
				tve.hwndTrack = pEvent->hWnd;
				tve.dwHoverTime = HOVER_DEFAULT;

				this->m_bHover = true;
				//InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,&m_pxfNode->m_AreaRect,false);
				InvalidateRect(m_hWnd,NULL,false);
				TrackMouseEvent(&tve);

				if(!m_TipHWND)
				{
					IXMLDOMNode * pNode;
					m_pxfNode->m_pElement->selectSingleNode(_T("help"),&pNode);
					if(pNode)
					{
						::SetTimer(pEvent->hWnd,1000,300,NULL);
						pNode->Release();
					}
				}
			}
			return true;
		}
		bool xcToolButton::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
		{
			this->m_bHover = false;
			//::InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,&m_pxfNode->m_AreaRect,false);
			::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}
		bool xcToolButton::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
		{
			this->SwitchLayer();
			PostMessage(GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)m_pxfNode->m_strNodeName,(LPARAM)this);
			return true;
		}

		bool xcToolButton::EvtSetFocus(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			DrawFocusRect(hdc,&rect);
			::ReleaseDC(GetHWND(),hdc);

			return true;

		}
		bool xcToolButton::EvtKillFocus(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			DrawFocusRect(hdc,&rect);
			::ReleaseDC(GetHWND(),hdc);
			return true;
		}


		bool xcToolButton::EvtTimer(TEvent* pEvent,LPARAM lParam)
		{
			KillTimer(pEvent->hWnd,1000);
			if(m_bHover)
			{
				HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
						  WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
						  //WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  GetHWND(), NULL, 
						  xfApp::GetApp()->GetInstance(), NULL);
				if(hwndTip)
				{
					m_TipHWND = hwndTip;
					SetWindowPos(hwndTip, HWND_TOPMOST,0,0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
					// Associate the tooltip with the tool.
					TOOLINFO toolInfo;
					ZeroMemory(&toolInfo,sizeof(TOOLINFO));
					toolInfo.cbSize = sizeof(toolInfo);
					toolInfo.hwnd = GetHWND();
					toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
					toolInfo.uId = (UINT_PTR)GetHWND();

					IXMLDOMNode * pNode = 0;
					IXMLDOMNode * pNodeTitle = 0;
					m_pxfNode->m_pElement->selectSingleNode(_T("help"),&pNode);
					toolInfo.lpszText  = 0;
					BSTR bstr = 0;
					BSTR bstrTitle = 0;
					if(pNode)
					{
						pNode->get_text(&bstr);
						if(bstr)toolInfo.lpszText = bstr;
						pNode->selectSingleNode(L"@caption",&pNodeTitle);
						if(pNodeTitle)pNodeTitle->get_text(&bstrTitle);
						;
					}
					::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
					::SendMessage(hwndTip, TTM_SETDELAYTIME, (WPARAM)TTDT_AUTOPOP, (LPARAM)1000000);
					::SendMessage(hwndTip, TTM_SETMAXTIPWIDTH,0,500);
					if(bstrTitle)::SendMessage(hwndTip,TTM_SETTITLE,1,(LPARAM)bstrTitle);
							
					if(bstr)::SysFreeString(bstr);
					if(bstrTitle)::SysFreeString(bstrTitle);
				}
				/*
				//set help
				IXMLDOMNode * pNode;
				IXMLDOMElement * pElement;
				m_pxfNode->m_pElement->selectSingleNode(_T("help"),&pNode);
				if(pNode)
				{
					xfNode * pxfNodeEx = m_pxfNode->GetXPage()->GetAgentNode();
					if(pxfNodeEx)
					{
						if(m_pAgentNodeTool)
							m_pxfNode->GetXPage()->SetAgentNodeContent(pxfNodeEx,m_pAgentNodeTool,NULL);
						else
						{
							pElement = xbXml::GetElement(pNode);
							m_pAgentNodeTool = m_pxfNode->GetXPage()->SetAgentNodeContent(pxfNodeEx,pElement,NULL);
							pElement->Release();
						}
					}
					pNode->Release();
				}
				*/
			}
			return true;
		}
		void xcToolButton::Initial()
		{
			if(m_pxfNode && m_pxfNode->m_pElement)
			{
				_variant_t var;
				m_pxfNode->m_pElement->getAttribute(L"checked",&var);
				if(var.bstrVal)
				{
					if(::_tcsicmp(var.bstrVal,_T("yes"))==0 || ::_tcsicmp(var.bstrVal,_T("true"))==0 ||
						::_tcsicmp(var.bstrVal,_T("1"))==0)
						m_bChecked = true;
				}
				m_pxfNode->m_pElement->getAttribute(L"src",&var);
				if(var.bstrVal)
				{
					m_pSrc = _tcsdup(var.bstrVal);
				}
			}
		}
	}
}