#include "stdafx.h"
#include "xcImage.hpp"
#include <gdiplus.h>
#include "iximage.hpp"

using namespace Gdiplus;

namespace Hxsoft{namespace XFrame
{
	xcImage::xcImage():pStream(NULL),m_pImage(NULL),m_pSrc(NULL),m_bScroolbar(false),m_nMaxWidth(-1)
	{
		m_TopLeft.x = 0;
		m_TopLeft.y = 0;

		// Initialize GDI+.
		//~gdiplusToken = 0;
		//~Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		//~Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		this->AttachListon(WM_PAINT,(FNEvent)&xcImage::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcImage::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&xcImage::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&xcImage::EvtMouseMove);
		//this->AttachListon(WM_TIMER,(FNEvent)&xcToolButton::EvtTimer);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&xcImage::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&xcImage::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&xcImage::EvtHScroll);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&xcImage::EvtDoubleClicked);
		this->AttachListon(WM_TIMER,(FNEvent)&xcImage::EvtTimer);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&xcImage::EvtKeyDown);
		this->AttachListon(WM_CHAR,(FNEvent)&xcImage::EvtChar);
		this->AttachListon(WM_MOUSELEAVE,(FNEvent)&xcImage::EvtMouseLeave);
	}

	xcImage::~xcImage()
	{
		//~if(gdiplusToken != 0 )Gdiplus::GdiplusShutdown(gdiplusToken);
		
		if(m_pImage)delete (Image *)m_pImage;
		if(pStream)pStream->Release();
	}

	int xcImage::DoDraw(HDC hPaintDC,RECT * rc)
	{
		RECT rt ;
		if(rc)
			rt = *rc;
		else
			GetClientRect(&rt);

		if(!this->m_bTransparent)
		{
			if(m_pxfNode)
			{
				const XUtil::Css::TBoxStyle * pBoxStyle =  m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex);
				if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
					XUtil::Css::DrawCssFrame(hPaintDC,&rt,pBoxStyle);
			}else
			{
				HBRUSH hBrush;
				hBrush = ::CreateSolidBrush(RGB(255,255,255));
				::FillRect(hPaintDC,&rt,hBrush);
				::DeleteObject(hBrush);
			}
		}

		int nWidth;
		int nHeight;

		if(m_pImage)
		{
			Graphics graph(hPaintDC);
			graph.SetPageUnit(UnitPixel);
		
			nWidth = ((Image *)m_pImage)->GetWidth();
			nHeight = ((Image *)m_pImage)->GetHeight();

			if(m_bScroolbar)
			{
				if(m_nMaxWidth > 0 && m_nMaxWidth < nWidth)
				{
					nHeight = (int)1.0 * nHeight * m_nMaxWidth / nWidth;
					nWidth = m_nMaxWidth;
				}
				graph.DrawImage(((Image *)m_pImage), m_TopLeft.x * -1, m_TopLeft.y * -1, nWidth, nHeight);
			}
			else
			{
				if(rt.right - rt.left < 1) rt.right = rt.left + nWidth;
				if(rt.bottom - rt.top < 1) rt.bottom = rt.top + nHeight;

				if(nWidth > 0)
				{
					if(1.0*(rt.right - rt.left)/nWidth*nHeight > rt.bottom - rt.top)
					{
						graph.DrawImage(((Image *)m_pImage),rt.left,rt.top,(int)1.0 *(rt.bottom - rt.top) * nWidth /nHeight ,rt.bottom - rt.top);
					}else
						graph.DrawImage(((Image *)m_pImage),rt.left,rt.top,rt.right - rt.left ,(int)1.0 *(rt.right - rt.left) * nHeight /nWidth);
				}
			}
			
		}
		UpdateScrollPos();
		return 1;
	}

	void xcImage::Initial()
	{
		if(m_pSrc)
			LoadFromUrl(NULL,m_pSrc);
		return Initial(m_pxfNode->m_pElement);
	}

	void xcImage::Initial(IXMLDOMElement * pElement)
	{
		VARIANT	val;
		if(!pElement) return;
		
		pElement->getAttribute(L"transparent",&val);
		if(val.bstrVal && 
			(_tcsicmp(val.bstrVal,L"true")==0||_tcsicmp(val.bstrVal,L"1")==0))
		{
			m_bTransparent = true;
		}
		::VariantClear(&val);

		pElement->getAttribute(L"scroolbar",&val);
		if(val.bstrVal && _tcsicmp(val.bstrVal,L"auto")==0)
		{
			m_bScroolbar = true;
		}
		::VariantClear(&val);

		pElement->getAttribute(L"maxwidth",&val);
		if(val.bstrVal)
		{
			int width = _ttol(val.bstrVal);
			if(width < 0) width = -1;
			m_nMaxWidth = width;
		}
		::VariantClear(&val);

		pElement->getAttribute(L"src",&val);
		if(val.bstrVal)
		{
			BSTR src = val.bstrVal;
			m_pSrc = new TCHAR[255];
			_tcscpy(m_pSrc,src);
			//m_pSrc = _T("/business/products/chanpin/6548321.jpg");
			if(m_pxfNode)
				LoadFromUrl(m_pxfNode->GetXPage()->m_pStrUrl,val.bstrVal);
			else
				LoadFromUrl(NULL,val.bstrVal);
		}
		::VariantClear(&val);
	}


	bool xcImage::LoadFromByte(byte * pData,int nLen)
	{
		if(pStream)
		{
			pStream->Release();
			if(m_pImage)delete m_pImage;
		}
		pStream = new CByteStream(pData,nLen);

		m_pImage = Image::FromStream(pStream);

		return true;
	}

	bool xcImage::LoadFromUrl(LPCTSTR pServerUrl,LPCTSTR pUrl)
	{
		byte * pData = NULL;
		int nLen;

		if(pStream)
		{
			pStream->Release();
			if(m_pImage)delete m_pImage;
		}
		pStream = NULL;
		m_pImage = NULL;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		if(pServerUrl)
			xfUriLoader::UrlCombineW(pServerUrl,pUrl,strUrl,&dwSize,0);
		else
			_tcscpy_s(strUrl,INTERNET_MAX_URL_LENGTH,pUrl);

		if(xfUriLoader::Load(strUrl,NULL,pData,nLen,false)==1)
		{
			LoadFromByte(pData,nLen);
			return true;
		}else
		{
			
			//if(UrlIsFileUrl(strUrl))
			//{
				DWORD dwLen = 0,dwLenRead=0;
				HANDLE fp1 = CreateFile(strUrl,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(fp1!=INVALID_HANDLE_VALUE )
				{
					dwLen = ::GetFileSize(fp1,NULL);
					if(dwLen > 0) pData = new unsigned char[dwLen];
					::ReadFile(fp1,pData,dwLen,&dwLenRead,NULL);
					::CloseHandle(fp1);
					nLen = (int)dwLen;
					LoadFromByte(pData,nLen);
					return true;
				}
			//}
		}

		if(pData) xfUriLoader::FreeData(pData);
		return false;
	}

	int xcImage::TopLeftMoved(POINT pt)
	{
		m_TopLeft = pt;
		::InvalidateRect(GetHWND(),NULL,false);
		return 1;
	}

	bool xcImage::EvtMouseWheel(TEvent* pEvent,LPARAM lParam)
	{
		if(pEvent->wheel.zDelta > 0)
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
		else
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
		InvalidateRect(GetHWND(),NULL,false);
		return true;
	}

	bool xcImage::EvtHScroll(TEvent* pEvent,LPARAM lParam)
	{
		ModifyScrollBar(SB_HORZ,pEvent->scroll.nSBCode,pEvent->scroll.nPos);
		InvalidateRect(GetHWND(),NULL,false);
		return true;
	}
	bool xcImage::EvtVScroll(TEvent* pEvent,LPARAM lParam)
	{
		ModifyScrollBar(SB_VERT,pEvent->scroll.nSBCode,pEvent->scroll.nPos);
		InvalidateRect(GetHWND(),NULL,false);
		return true;
	}

	bool xcImage::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		RECT	rc;
		HDC hPaintDC = pEvent->paint.hdc;
		GetClientRect(&rc);
		
		HDC hMemDC = ::CreateCompatibleDC(hPaintDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(hPaintDC,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);

		xfNodeControl* pNode = (xfNodeControl *)m_pxfNode;
		if(pNode && pNode->m_hBackGround)
		{
			HDC hDC = ::CreateCompatibleDC(hPaintDC);
			HBITMAP hBmp = (HBITMAP)pNode->m_hBackGround;
			hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
			::BitBlt(hMemDC,rc.left,rc.top,rc.right - rc.left + 1,rc.bottom - rc.top + 1,hDC,rc.left,rc.top,SRCCOPY);
			hBmp = (HBITMAP)::SelectObject(hDC,hBmp);
			::DeleteDC(hDC);
		}

		DoDraw(hMemDC,&rc);
		::BitBlt(hPaintDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top + 1,hMemDC,rc.left,rc.top,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hMemDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hMemDC);
		

		return true;
	}

	bool xcImage::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		IMGNNMHDR evt;
		memset(&evt,0,sizeof(IMGNNMHDR));
		evt.hwndFrom = this->m_hWnd;
		evt.path = m_pSrc;
		evt.code = IMG_LCLICK;
		::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
		return true;
	}

	bool xcImage::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
	{

		return true;
	}

	bool xcImage::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
	{
		return true;
	}

	bool xcImage::EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam)
	{
		IMGNNMHDR evt;
		memset(&evt,0,sizeof(IMGNNMHDR));
		evt.hwndFrom = this->m_hWnd;
		evt.path = m_pSrc;
		evt.code = IMG_LDBCLICK;
		::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
		return true;
	}
	bool xcImage::EvtMouseLeave(TEvent *pEvent ,LPARAM lParam)
	{
		IMGNNMHDR evt;
		memset(&evt,0,sizeof(IMGNNMHDR));
		evt.hwndFrom = this->m_hWnd;
		evt.path = m_pSrc;
		evt.code = IMG_MOUSELEAVE;
		::SendMessage(::GetParent(m_hWnd),WM_NOTIFY,0,(LPARAM)&evt);
	
		return true;
	}

		
	bool xcImage::EvtTimer(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}

	bool xcImage::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}

	bool xcImage::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{

		return true;
	}

	SIZE xcImage::GetContentExtent()
	{
		int nWidth = m_pImage?((Image *)m_pImage)->GetWidth():-1;
		int nHeight = m_pImage?((Image *)m_pImage)->GetHeight():-1;
		SIZE sz={nWidth,nHeight}; 
		return sz;
	}

	xbObject * xcImage::GetInterface()
	{
		if(!m_pInterFace)
		{
			IXImage * pIXImage = new IXImage();
			m_pInterFace = (xbObject *)pIXImage;
			pIXImage->m_pControl = this;
		}
		return this->m_pInterFace;
	}

	bool xcImage::SaveToFileEx(LPTSTR pFile)
	{
		if(!pStream) return false;

		byte * pData = pStream->m_pArray;
		int nLen = pStream->m_nByteSize;

		HANDLE fp1;
		DWORD nWrite;
		fp1 = CreateFile(pFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

		::WriteFile(fp1,pData,nLen,&nWrite,NULL);
		::CloseHandle(fp1);

		return true;
	}

	bool xcImage::SaveToFile(LPTSTR pFile)
	{
		TCHAR szFileName[255],szPath[255];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;
		ZeroMemory(szFileName,255);
		ZeroMemory(szPath,255);
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("JPEG 文件(*.jpg)\0*.jpg;*.jpeg;*.png;*.gif;*.bmp\0全部文件(*.*)\0*.*\0"));
		if(pFile) lstrcpy(szPath,pFile);

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = szCustomFilter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = 255;
		ofn.lpstrTitle = L"请指定文件名";
		ofn.lpstrDefExt = L"jpg";
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if(::GetSaveFileName(&ofn))
			return SaveToFileEx(szPath);
		return false;
	}

	void xcImage::UpdateScrollPos()
	{
		if(!m_pImage) return;
		if(!m_bScroolbar) return;

		RECT rt;
		GetClientRect(&rt);
		int nWidth = ((Image *)m_pImage)->GetWidth();
		int nHeight = ((Image *)m_pImage)->GetHeight();
		if(m_nMaxWidth > 0 && m_nMaxWidth < nWidth)
		{
			nHeight = (int)1.0 * nHeight * m_nMaxWidth / nWidth;
			nWidth = m_nMaxWidth;
		}

		if(nHeight > rt.bottom - rt.top)
		{
			SCROLLINFO si; 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE; 
			si.nMin   = 0;
			si.nMax   = nHeight;
			si.nPage  = rt.bottom - rt.top; 
			SetScrollInfo(GetHWND(), SB_VERT, &si, true); 
			ShowScrollBar(GetHWND(),SB_VERT,true);
		}
		else
		{
			ShowScrollBar(GetHWND(),SB_VERT,false);
		}

		if(nWidth > rt.right - rt.left)
		{
			SCROLLINFO si; 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE; 
			si.nMin   = 0;
			si.nMax   = nWidth;
			si.nPage  = rt.right - rt.left; 
			SetScrollInfo(GetHWND(), SB_HORZ, &si, true); 
			ShowScrollBar(GetHWND(),SB_HORZ,true);
		}
		else
		{
			ShowScrollBar(GetHWND(),SB_HORZ,false);
		}
	}

	void xcImage::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
	{
		if(!m_pImage) return;
		if(!m_bScroolbar) return;

		RECT rt;
		GetClientRect(&rt);
		int nWidth = ((Image *)m_pImage)->GetWidth();
		int nHeight = ((Image *)m_pImage)->GetHeight();
		if(m_nMaxWidth > 0 && m_nMaxWidth < nWidth)
		{
			nHeight = (int)1.0 * nHeight * m_nMaxWidth / nWidth;
			nWidth = m_nMaxWidth;
		}

		int Value;
		int Value1;
		if(ScrollBar == SB_HORZ) 
		{
			Value = m_TopLeft.x;
			Value1 = rt.right - rt.left;
		}
		else
		{
			Value = m_TopLeft.y;
			Value1 = rt.bottom - rt.top;
		}
		int Result;
		Result = Value;
		switch(ScrollCode) 
		{
		case SB_LINEUP:
			Result = Value - 1;
			break;
		case SB_LINEDOWN:
			Result = Value + 1;
			break;
		case SB_PAGEUP:
			Result = Value - Value1;
			break;
		case SB_PAGEDOWN:
			Result = Value + Value1;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			SCROLLINFO si; 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_ALL;
			GetScrollInfo (GetHWND(), ScrollBar, &si);
			Result = si.nTrackPos * 1;
			break;
		case SB_BOTTOM:
			Result = Value + 1;
			break;
		case SB_TOP:
			Result = Value - 1;
			break;
		}
		if(Result < 0) Result = 0;
		POINT pt;
		pt.y = m_TopLeft.y;
		pt.x = m_TopLeft.x;
		if(ScrollBar == SB_HORZ)
		{
			pt.x = Result;
			if(Result >= nWidth) return;
		}
		else
		{
			pt.y = Result;
			if(Result >= nHeight) return;
		}
		SetScrollPos(GetHWND(), ScrollBar,Result,true);
		TopLeftMoved(pt);
	}
}}