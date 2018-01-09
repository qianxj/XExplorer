#include "stdafx.h"
#include "xfframe.h"
#include <vector>

using namespace std;

namespace Hxsoft
{
	namespace XFrame
	{

		XWinDesign::~XWinDesign(void)
		{
			if(m_pOldNode)delete m_pOldNode;
		}

		int XWinDesign::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
		{
			if(message==WM_PAINT)
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);
				OnPaint(hdc);
				EndPaint(hWnd, &ps);
				return 0;
			}
			else if(message==WM_SIZE)
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				OnSize((UINT)wParam,point);
				return 0;
			}
			else if(message==WM_ERASEBKGND)
			{
				HDC hdc = (HDC) wParam;
				return OnEraseBkgnd(hdc);
			}
			else if(message==WM_LBUTTONDOWN)
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				return OnLButtonDown((UINT)wParam,point);
			}
			else if(message==WM_LBUTTONUP)
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				return OnLButtonUp(nFlags,point);
			}
			else if(message==WM_MOUSEMOVE)
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				return OnMouseMove(nFlags,point);
			}
			else if(message==WM_SETCURSOR)
			{
				HWND	hWnd = (HWND)wParam;
				UINT	nHitTest = LOWORD(lParam);
				UINT	mouseMsg = HIWORD(lParam);
				return OnSetCursor(hWnd,nHitTest,mouseMsg);
			}
			return 0;
		}


		int XWinDesign::OnPaint(HDC hDC) 
		{
			RECT	rc;
			HDC	hPaintDC;
			HBITMAP	hBmp;

			::GetClientRect(m_hWnd,&rc);
			hPaintDC = ::CreateCompatibleDC(hDC);
			hBmp = ::CreateCompatibleBitmap(hDC,rc.right - rc.left + 1,rc.bottom - rc.top +1);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

			HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
			::FillRect(hPaintDC,&rc,hBrush);
			::DeleteObject(hBrush);
	
			if(m_pNode)m_pNode->DoDraw(hPaintDC);
			::BitBlt(hDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hPaintDC);
			
			return 0;
		}

		 int XWinDesign::LoadDesignFrame(LPTSTR xmlTemplate,bool bFile)
		{
			IXMLDOMDocumentPtr pXmlDoc;
			if(!pXmlDoc)
			{
				//创建DOMDocument对象 
				HRESULT hr = pXmlDoc.CreateInstance(CLSID_DOMDocument40); 
				if(!SUCCEEDED(hr)) 
				{  
					//MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!",MB_OK); 
					HRESULT hr = pXmlDoc.CreateInstance(CLSID_DOMDocument30); 
					if(!SUCCEEDED(hr)) 
					{  
						return 0;
					}
				}
				pXmlDoc->put_async(VARIANT_FALSE);
			}
			VARIANT_BOOL vtResult;
			if(bFile)
				pXmlDoc->load((_variant_t)xmlTemplate,&vtResult);
			else
				pXmlDoc->loadXML(xmlTemplate,&vtResult);

			if(vtResult != VARIANT_TRUE) return NULL;

			IXMLDOMElementPtr pNodeEx ;
			pXmlDoc->get_documentElement(&pNodeEx);

			VARIANT	val;
			LPTSTR strWinType = NULL;
			LPTSTR pXScript = NULL;
			LPTSTR pXDll = NULL;
			LPTSTR pstrGuid = NULL;
			LPTSTR pWinID = NULL;

			pNodeEx->getAttribute(L"type",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				strWinType =XUtil::tcsdupFromBstr(val.bstrVal); 
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"xscript",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pXScript = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"xdll",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pXDll = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"name",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pWinID = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"guid",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pstrGuid = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			XNode * pXNode = new XNode();
			pXNode->m_pOwerControl= this;
			m_pOldNode = m_pNode;
			m_pNode = pXNode;
			pXNode->ParseNode(pNodeEx);

			RECT rc;
			::SetRect(&rc,0,0,0,0);
			pNodeEx->getAttribute(L"rect",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				vector<LPTSTR> vals;
				LPTSTR pstrVal;
				pstrVal = XUtil::tcsdupFromBstr(val.bstrVal);
				if(XUtil::GetTokens(pstrVal,_T(","),vals)==4)
				{
					SetRect(&rc,XUtil::CalcCssStrToNumber(vals[0]),XUtil::CalcCssStrToNumber(vals[1]),
						XUtil::CalcCssStrToNumber(vals[2]),XUtil::CalcCssStrToNumber(vals[3]));
				}
				if(pstrVal) delete pstrVal;
			}
			::VariantClear(&val);

		

			if(strWinType)delete strWinType;
			if(pXScript) delete  pXScript;
			if(pXDll) delete pXDll;
			if(pstrGuid) delete pstrGuid;
			if(pWinID) delete pWinID;

			pXNode->CreateNodeControl(m_hWnd);
			::GetClientRect(m_hWnd,&rc);
			pXNode->m_AreaRect = rc;
			pXNode->AdjustRect();
			pXNode->AdjustCtrlRect();
			::InvalidateRect(m_hWnd,NULL,true);

			return 1;

		}

		int XWinDesign::OnLButtonDown( UINT nFlags, POINT point ) 
		{
			m_pSpliterBar = this->m_pNode->GetSpliterBarAtPointer(point);
			if(m_pSpliterBar)
			{
				m_MousePt = point;
				m_pSpliterBar->DrawSizeBar(m_hWnd,m_MousePt);
				::SetCapture(m_hWnd);
			}
			return 1;
		}


		int  XWinDesign::OnMouseMove( UINT nFlags, POINT point )
		{
			if(m_pSpliterBar)
			{
				m_pSpliterBar->DrawSizeBar(m_hWnd,m_MousePt);
				m_MousePt = point;
				m_pSpliterBar->DrawSizeBar(m_hWnd,m_MousePt);
			}
			m_MousePt = point;
			return 1;
		}


		int XWinDesign::OnLButtonUp( UINT nFlags, POINT point )
		{
			if(m_pSpliterBar)
			{
				m_pSpliterBar->DrawSizeBar(m_hWnd,m_MousePt);
				ReleaseCapture();
				m_pSpliterBar->AdjustSplitePos(m_MousePt);
				m_pSpliterBar = NULL;
				::InvalidateRect(m_hWnd,NULL,false);
			}
			return 1;
		}


		BOOL XWinDesign::OnSetCursor( HWND hWnd, UINT nHitTest, UINT message )
		{
			HCURSOR Cur = NULL;
			XSpliterBar *  pSpliterBar = NULL;
			if ( hWnd == this->m_hWnd && nHitTest==HTCLIENT )
			{
				if(this->m_pSpliterBar)
					pSpliterBar = m_pSpliterBar;
				else
				{
					pSpliterBar = this->m_pNode->GetSpliterBarAtPointer(m_MousePt);
				}
			}
			if(pSpliterBar)
			{
				if(pSpliterBar->m_bIsVertbar)
				{
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crVSplit
				}else
				{
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crHSplit
				}
			}
			if(Cur != NULL)
			{
				SetCursor(Cur);
				return TRUE;
			}
			return false;
		}


		int XWinDesign::OnSize(UINT nFlags, POINT point)
		{
			RECT	rc;
			if(nFlags !=SIZE_MINIMIZED)
			{
				if(m_hWnd)
				{
					::GetClientRect(m_hWnd,&rc);
					if(m_pNode)
					{
						m_pNode->AdjustRect(rc);
						m_pNode->AdjustCtrlRect();
					}
				}
			}
			return 0;
		}

		bool XWinDesign::OnEraseBkgnd(HDC hdc)
		{
			return true;
		}
	}
}