#include "stdafx.h"
#include "xnode.hpp"
#include "xwin.hpp"
#include "xapp.hpp"
#include "xutil.hpp"
#include <vector>
#include "xmdiframe.hpp"

using namespace std;

namespace Hxsoft
{
	namespace XFrame
	{

		XWin::~XWin(void)
		{
			if(m_hDLL) FreeLibrary(m_hDLL);
			if(m_pNode)delete m_pNode;
			for(int i=0;i< (int) this->m_hImageLists.size();i++)
			{
				if(m_hImageLists[i])::ImageList_Destroy(m_hImageLists[i]);
			}
			m_hImageLists.clear();
		}

		int XWin::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
		{
			int  (WINAPI * fnProcessMessage)(XWin*  pWin,UINT message, WPARAM wParam, LPARAM lParam) = NULL;
			if(m_hDLL)
				fnProcessMessage = (int  (WINAPI * )(XWin*  pWin, UINT message, WPARAM wParam, LPARAM lParam))::GetProcAddress(m_hDLL,"HandleMessage");
			if(fnProcessMessage && fnProcessMessage(this,message,wParam,lParam)) return 1;
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


		int XWin::OnPaint(HDC hDC) 
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

		 XWin* XWin::CreateByXPage(LPTSTR xmlTemplate,bool bFile,HWND hParent,HINSTANCE hInst)
		{
			XWin * pWin = NULL;
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

			HMODULE hDLL = NULL;
			if(pXDll)
				hDLL = ::LoadLibrary(pXDll);
			else if(pXScript)
			{
				if(pstrGuid)
				{
					TCHAR sModula[255];
					
					::GetModuleFileName(XApp::GetXApp()->GetInstance(),sModula,255);
					int pos = (int)::_tcslen(sModula);
					while(pos>=0 && sModula[pos]!='\\')pos--;
					if(pos >=0)sModula[pos + 1] = 0;
					::_tcscat_s(sModula,255,_T("ext\\"));
					::_tcscat_s(sModula,255,pstrGuid);
					::_tcscat_s(sModula,255,_T(".dll"));

					hDLL = ::LoadLibrary(sModula);
				}
			}
		   if(hDLL)
		   {
				//init application
			   int  (WINAPI * fnInitXApp)(XApp*  pXApp) = NULL;
			   fnInitXApp = (int  (WINAPI * )(XApp*  pXApp))::GetProcAddress(hDLL,"InitXApp");
			   if(fnInitXApp)fnInitXApp(XApp::GetXApp());

			   //init window
			   XWin *  (WINAPI * fnCreateInstance)(LPTSTR pWinName) = NULL;
			   fnCreateInstance = (XWin *  (WINAPI * )(LPTSTR pWinName))::GetProcAddress(hDLL,"CreateInstance");
			   if(fnCreateInstance)
				   pWin = fnCreateInstance(pWinID);
		   }

			if(!pWin)
			{
				if(::_tcscmp(_T("mdiframe"),strWinType)==0)
					pWin = new XMdiFrame();
				else
					pWin = new XWin();
			}
			XNode * pXNode = new XNode();
			pXNode->m_pOwnerWin = pWin;
			pWin->m_pNode = pXNode;
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

			if(strWinType) 
			{
				if(::_tcscmp(_T("mdiframe"),strWinType)==0)
				{
					pWin->CreateEx(0,WS_VISIBLE|WS_OVERLAPPEDWINDOW,XFrameWndClass,pXNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("sheet"),strWinType)==0)
				{
					pWin->CreateEx(WS_EX_MDICHILD,WS_CHILD|WS_VISIBLE,XFrameWndClass,pXNode->m_pCaption,rc,XApp::GetXApp()->m_hMdiClient,0,hInst);
				}else if(::_tcscmp(_T("main"),strWinType)==0)
				{
					pWin->CreateEx(0,WS_VISIBLE|WS_OVERLAPPEDWINDOW,XFrameWndClass,pXNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("popup"),strWinType)==0)
				{
					pWin->CreateEx(0,WS_OVERLAPPEDWINDOW|WS_POPUP|WS_VISIBLE,XFrameWndClass,pXNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("child"),strWinType)==0)
				{
					pWin->CreateEx(0,WS_VISIBLE|WS_CHILD,XFrameWndClass,pXNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				}else
				{
					pWin->CreateEx(WS_EX_MDICHILD,WS_OVERLAPPEDWINDOW,XFrameWndClass,pXNode->m_pCaption,rc,hParent,0,hInst);
				}
			}else
			{
				pWin->CreateEx(WS_EX_MDICHILD,WS_OVERLAPPEDWINDOW,XFrameWndClass,pXNode->m_pCaption,rc,hParent,0,hInst);
			}

			pWin->m_hDLL = hDLL;

			if(strWinType)delete strWinType;
			if(pXScript) delete  pXScript;
			if(pXDll) delete pXDll;
			if(pstrGuid) delete pstrGuid;
			if(pWinID) delete pWinID;

			pXNode->CreateNodeControl(pWin->m_hWnd);
			::GetClientRect(pWin->m_hWnd,&rc);
			pXNode->m_AreaRect = rc;
			pXNode->AdjustRect();
			pXNode->AdjustCtrlRect();
			::InvalidateRect(pWin->m_hWnd,NULL,true);

			return pWin;
		}

		int XWin::LoadTemplate(LPTSTR xmlTemplate,bool bFile,HWND hParent,HINSTANCE hInst)
		{
			if(!this->m_pXmlDoc)
			{
				//创建DOMDocument对象 
				HRESULT hr = m_pXmlDoc.CreateInstance(CLSID_DOMDocument40); 
				if(!SUCCEEDED(hr)) 
				{  
					//MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!",MB_OK); 
					HRESULT hr = m_pXmlDoc.CreateInstance(CLSID_DOMDocument30); 
					if(!SUCCEEDED(hr)) 
					{  
						return 0;
					}
				}
				m_pXmlDoc->put_async(VARIANT_FALSE);
			}

			VARIANT_BOOL vtResult;
			if(bFile)
				m_pXmlDoc->load((_variant_t)xmlTemplate,&vtResult);
			else
				m_pXmlDoc->loadXML(xmlTemplate,&vtResult);

			if(vtResult != VARIANT_TRUE) return -1;

			IXMLDOMElementPtr pNodeEx ;
			m_pXmlDoc->get_documentElement(&pNodeEx);
			
			VARIANT	val;
			LPTSTR strWinType = NULL;
			LPTSTR strCaption = NULL;
			pNodeEx->getAttribute(L"type",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				strWinType =XUtil::tcsdupFromBstr(val.bstrVal); 
			}
			::VariantClear(&val);


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

			pNodeEx->getAttribute(L"caption",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				strCaption =XUtil::tcsdupFromBstr(val.bstrVal); 
			}
			::VariantClear(&val);

			if(m_pNode)delete m_pNode;
			m_pNode = new XNode();
			m_pNode->m_pOwnerWin = this;
			m_pNode->ParseNode(pNodeEx);

			if(strWinType) 
			{
				if(::_tcscmp(_T("mdiframe"),strWinType)==0)
				{
					CreateEx(0,WS_VISIBLE|WS_OVERLAPPEDWINDOW,XFrameWndClass,strCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("sheet"),strWinType)==0)
				{
					CreateEx(WS_EX_MDICHILD,WS_CHILD|WS_VISIBLE,XFrameWndClass,strCaption,rc,XApp::GetXApp()->m_hMdiClient,0,hInst);
				}else if(::_tcscmp(_T("main"),strWinType)==0)
				{
					CreateEx(0,WS_VISIBLE|WS_OVERLAPPEDWINDOW,XFrameWndClass,strCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("popup"),strWinType)==0)
				{
					CreateEx(0,WS_OVERLAPPEDWINDOW|WS_POPUP|WS_VISIBLE,XFrameWndClass,strCaption,rc,(HWND)NULL,0,hInst);
				}else if(::_tcscmp(_T("child"),strWinType)==0)
				{
					CreateEx(0,WS_VISIBLE|WS_CHILD,XFrameWndClass,strCaption,rc,(HWND)NULL,0,hInst);
				}else
				{
					CreateEx(WS_EX_MDICHILD,WS_OVERLAPPEDWINDOW,XFrameWndClass,strCaption,rc,hParent,0,hInst);
				}
			}else
			{
				CreateEx(WS_EX_MDICHILD,WS_OVERLAPPEDWINDOW,XFrameWndClass,strCaption,rc,hParent,0,hInst);
			}
			if(strCaption)delete strCaption;
			if(strWinType)delete strWinType;

			if(m_pNode->m_pXDll)
				this->m_hDLL = ::LoadLibrary(m_pNode->m_pXDll);
			else if(m_pNode->m_pXScript)
			{
				if(m_pNode->m_pstrGuid)
				{
					TCHAR sModula[255];
					
					::GetModuleFileName(XApp::GetXApp()->GetInstance(),sModula,255);
					int pos = (int)::_tcslen(sModula);
					while(pos>=0 && sModula[pos]!='\\')pos--;
					if(pos >=0)sModula[pos + 1] = 0;
					::_tcscat_s(sModula,255,_T("ext\\"));
					::_tcscat_s(sModula,255,m_pNode->m_pstrGuid);
					::_tcscat_s(sModula,255,_T(".dll"));

					this->m_hDLL = ::LoadLibrary(sModula);
				}
			}
		   
			//init application
		   int  (WINAPI * fnInitXApp)(XApp*  pXApp) = NULL;
		   if(this->m_hDLL)
			   fnInitXApp = (int  (WINAPI * )(XApp*  pXApp))::GetProcAddress(this->m_hDLL,"InitXApp");
		   if(fnInitXApp)fnInitXApp(XApp::GetXApp());;

			m_pNode->CreateNodeControl(m_hWnd);
			::GetClientRect(this->m_hWnd,&rc);
			m_pNode->m_AreaRect = rc;
			m_pNode->AdjustRect();
			m_pNode->AdjustCtrlRect();
			::InvalidateRect(this->m_hWnd,NULL,true);

			return 0;
		}


		int XWin::OnLButtonDown( UINT nFlags, POINT point ) 
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


		int  XWin::OnMouseMove( UINT nFlags, POINT point )
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


		int XWin::OnLButtonUp( UINT nFlags, POINT point )
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


		BOOL XWin::OnSetCursor( HWND hWnd, UINT nHitTest, UINT message )
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


		int XWin::OnSize(UINT nFlags, POINT point)
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

		bool XWin::OnEraseBkgnd(HDC hdc)
		{
			return true;
		}
	}
}