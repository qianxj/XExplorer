#include "stdafx.h"
#include "shlobj.h"
#include "xcFile.hpp"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		xcFile::xcFile():m_bHover(false),m_pAgentNodeTool(NULL),m_bAutoUpload(false),m_bIsUpload(false),m_sPath(NULL),m_sFolder(NULL)
		{
			m_sTxtWidth=60;
			m_bTransparent = true;
			m_bFocusRect=false;
			this->AttachListon(WM_ERASEBKGND,(FNEvent)&xcFile::EvtEraseBkgn);
			this->AttachListon(WM_PAINT,(FNEvent)&xcFile::EvtPaint);
			this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcFile::EvtMouseMove);
			this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcFile::EvtMouseLeave);
			this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcFile::EvtLButtonUp);
			this->AttachListon(WM_TIMER,(FNEvent)&xcFile::EvtTimer);
			this->AttachListon(WM_SETFOCUS, (FNEvent)&xcFile::EvtSetFocus);
			this->AttachListon(WM_KILLFOCUS, (FNEvent)&xcFile::EvtKillFocus);
			this->AttachListon(WM_KEYDOWN, (FNEvent)&xcFile::EvtKeyDown);
		}
		xcFile::~xcFile()
		{
		}

		bool xcFile::EvtEraseBkgn(TEvent* pEvent,LPARAM lParam)
		{
			pEvent->dwRet = 1;
			return true;
		}

		int xcFile::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rect;
			if(pDrawRect)
				rect = *pDrawRect;
			else
				GetClientRect(&rect);

			rect.left += 1;
			rect.top += 1;

			if(this->m_bHover)
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

			::SetBkMode(hPaintDC,TRANSPARENT);
			
			LOGFONT	logFont;
			const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
			XUtil::Css::GetFont(pBoxStyle,logFont);
	
			HFONT	hFont =::CreateFontIndirect(&logFont);
			hFont = (HFONT)::SelectObject(hPaintDC, hFont);

			COLORREF color;
			if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
				color = ::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);

			RECT sRect;
			sRect = rect;
			sRect.right = sRect.right - m_sTxtWidth -4;
			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(49,106,197));
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);
			::MoveToEx(hPaintDC,sRect.left,sRect.bottom - 1,NULL); 
			::LineTo(hPaintDC,sRect.right,sRect.bottom - 1);
			if(m_sPath)
			{
				sRect.left += 2;
				::DrawText(hPaintDC,m_sPath,(int)::_tcslen(m_sPath),&sRect, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);
			}

			RECT tRect;
			tRect = rect;
			tRect.left = tRect.right - m_sTxtWidth;
			if(m_pxfNode && m_pxfNode->m_pCaption)
			{
				::DrawText(hPaintDC,m_pxfNode->m_pCaption,(int)::_tcslen(m_pxfNode->m_pCaption),&tRect, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_CENTER);

				if(::GetFocus()==this->GetHWND())
				{
					::InflateRect(&tRect,-1,-1);
					::DrawFocusRect(hPaintDC,&tRect);
				}
				if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
					::SetTextColor(hPaintDC,color);
			}
			hFont = (HFONT)::SelectObject(hPaintDC, hFont);
			::DeleteObject(hFont);
			return 1;
		}

		bool xcFile::EvtPaint(TEvent* pEvent,LPARAM lParam)
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

		bool xcFile::EvtKeyDown(TEvent* pEvent,LPARAM lParam)
		{
			if(pEvent->key.nChar==VK_SPACE)
			{
				this->SwitchLayer();
				PostMessage(GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)m_pxfNode->m_strNodeName,(LPARAM)this);
			}
			return true;
		}

		bool xcFile::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			rect.left = rect.right - m_sTxtWidth;
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(this->m_hWnd,&pt);
			if(pt.x>rect.left)
			{
				if(!m_bFocusRect)
				{
					DrawFocusRect(hdc,&rect);
					m_bFocusRect=true;
				}
			}
			else
			{
				if(m_bFocusRect)
				{
					DrawFocusRect(hdc,&rect);
					m_bFocusRect=false;
				}
			}

			if(!m_bHover)
			{
				TRACKMOUSEEVENT	tve;
				tve.cbSize = sizeof(TRACKMOUSEEVENT);
				tve.dwFlags = TME_LEAVE;
				tve.hwndTrack = pEvent->hWnd;
				tve.dwHoverTime = HOVER_DEFAULT;

				this->m_bHover = true;
				//InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,&m_pxfNode->m_AreaRect,false);
				//InvalidateRect(m_hWnd,NULL,false);
				TrackMouseEvent(&tve);

				IXMLDOMNode * pNode;
				m_pxfNode->m_pElement->selectSingleNode(_T("help"),&pNode);
				if(pNode)
				{
					::SetTimer(pEvent->hWnd,1000,300,NULL);
					pNode->Release();
				}
			}
			return true;
		}
		bool xcFile::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			rect.left = rect.right - m_sTxtWidth;
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(this->m_hWnd,&pt);
			if(pt.x>rect.left)
			{
				if(m_bFocusRect)
				{
					DrawFocusRect(hdc,&rect);
					m_bFocusRect=false;
				}
			}

			this->m_bHover = false;
			//::InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,&m_pxfNode->m_AreaRect,false);
			::InvalidateRect(m_hWnd,NULL,false);
			return true;
		}
		bool xcFile::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
		{
			this->SwitchLayer();
			RECT rect;
			::GetWindowRect(this->m_hWnd,&rect);
			POINT pt;
			::GetCursorPos(&pt);
			if(rect.right - pt.x < m_sTxtWidth)
			{
/*				BROWSEINFO f;
				f.hwndOwner = this->m_hWnd;
				f.pidlRoot = NULL;
				f.lpszTitle = _T("请选择图片");
				f.ulFlags=BIF_NONEWFOLDERBUTTON;
				f.iImage=NULL;
				f.lParam=NULL;
				f.lpfn=NULL;
				f.pszDisplayName=NULL;
				LPITEMIDLIST fn = ::SHBrowseForFolder(&f);
				LPWSTR fname;
*/				
				TCHAR filter[]= _T("图片文件(*.jpg;*.bmp;*.gif;*.png)\0*.jpg;*.bmp;*.gif;*.png\0全部文件(*.*)\0*.*\0");
				TCHAR szFileName[MAX_PATH];
				TCHAR szPath[MAX_PATH];
				TCHAR szCustomFilter[MAX_PATH];
				int	 nFilterIndex = 1;

				ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
				lstrcpy(szCustomFilter,_T("图片文件*.jpg;*.bmp;*.gif;*.png\0*.jpg\0"));

				OPENFILENAME ofn;
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = this->m_hWnd;
				ofn.hInstance = NULL;
				ofn.lpstrFilter = filter;
				//ofn.lpstrCustomFilter = szCustomFilter;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
				ofn.nFilterIndex = nFilterIndex;
				ofn.lpstrFile = szPath;
				ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
				ofn.lpstrFileTitle = szFileName;
				ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
				ofn.lpstrTitle = _T("图片选择...");
				ofn.lpstrDefExt = _T("jpg");
				ofn.lpstrInitialDir = m_sFolder;
				ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
				ofn.lCustData = 0;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;
				
				if(::GetOpenFileName(&ofn))
				{
					LPTSTR pStr = new TCHAR[lstrlen(szPath)+3];
					_tcscpy(pStr,szPath);
					SetFilePath(pStr);
				}

				PostMessage(GetParent(this->m_hWnd),WM_XCOMMAND,(WPARAM)m_pxfNode->m_strNodeName,(LPARAM)this);
			}
			return true;
		}

		bool xcFile::EvtSetFocus(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			rect.left = rect.right - m_sTxtWidth;
			DrawFocusRect(hdc,&rect);
			::ReleaseDC(GetHWND(),hdc);

			return true;

		}
		bool xcFile::EvtKillFocus(TEvent* pEvent,LPARAM lParam)
		{
			RECT rect;
			this->GetClientRect(&rect);
			HDC hdc = ::GetDC(GetHWND());
			::InflateRect(&rect,-1,-1);
			rect.left = rect.right - m_sTxtWidth;
			DrawFocusRect(hdc,&rect);
			::ReleaseDC(GetHWND(),hdc);
			return true;
		}


		bool xcFile::EvtTimer(TEvent* pEvent,LPARAM lParam)
		{
			KillTimer(pEvent->hWnd,1000);
			if(m_bHover)
			{
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
			}
			return true;
		}

		void xcFile::Initial()
		{
			if(m_pxfNode && m_pxfNode->m_pElement)
			{
				VARIANT var0;
				m_pxfNode->m_pElement->getAttribute(L"path",&var0);
				if(var0.vt==VT_BSTR && var0.bstrVal)
					m_sPath = XUtil::tcsdupFromBstr(var0.bstrVal);

				VARIANT var1;
				m_pxfNode->m_pElement->getAttribute(L"folder",&var1);
				if(var1.vt==VT_BSTR && var1.bstrVal)
					m_sFolder = XUtil::tcsdupFromBstr(var1.bstrVal);

				VARIANT var;
				m_pxfNode->m_pElement->getAttribute(L"autoupload",&var);
				if(var.vt==VT_BSTR && var.bstrVal)
				{
					if(::_tcsicmp(var.bstrVal,_T("yes"))==0 || ::_tcsicmp(var.bstrVal,_T("true"))==0 ||
						::_tcsicmp(var.bstrVal,_T("1"))==0)
						m_bAutoUpload = true;
				}
				::VariantClear(&var);
			}
		}
		int xcFile::SetFilePath(LPTSTR szPath)
		{
			m_sPath = szPath;
			RECT rect;
			this->GetClientRect(&rect);
			::InvalidateRect(this->m_hWnd,&rect,true);
			return 1;
		}
		int xcFile::SetFileFolder(LPTSTR szFolder)
		{
			m_sFolder = szFolder;
			return 1;
		}

	}
}