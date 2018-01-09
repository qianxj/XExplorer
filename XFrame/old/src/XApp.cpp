#include "stdafx.h"
#include "shlwapi.h"
#include "xnode.hpp"
#include "xapp.hpp"
#include "xwinbase.hpp"
#include "xtimer.hpp"

Hxsoft::XFrame::XApp* g_pXApp = NULL;

namespace Hxsoft
{
	namespace XFrame
	{
		bool XApp::m_bShutDown = false;
		HANDLE XApp::m_hThreadTimer = NULL;
		DWORD WINAPI XApp::TimerThreadProc( LPVOID lpParam )
		{
			XApp* pThis = (XApp*)lpParam ;
			while(!pThis->m_bShutDown)
			{
				::Sleep(10);
				for(int i=0;i < pThis->m_timerCount;i++)
				{
					pThis->m_pTimers[i]->EventTimer();
				}
			}
			return 1;
		}

		DWORD WINAPI XApp::TimerEventThreadProc( LPVOID lpParam )
		{
			XTimer* pTimer = (XTimer*)lpParam ;
			pTimer->fnTimer(pTimer->m_timerID,pTimer->fnTimer);
			return 1;
		}

		XApp::XApp():m_hInst(NULL),m_hWnd(NULL),m_hMdiClient(NULL),m_timerCount(0),m_pTimers(NULL),m_timeAllocSize(0),m_pXEditTemplate(NULL)
		{
			m_pBrowerModulaPath = new TCHAR[MAX_PATH];
			m_pXConfigPath = new TCHAR[MAX_PATH];

			::GetModuleFileName(NULL,m_pBrowerModulaPath,MAX_PATH);
			PathRemoveFileSpec(m_pBrowerModulaPath);
			m_pXConfigPath[0]='\0';
			_tcscat_s(m_pXConfigPath,MAX_PATH,m_pBrowerModulaPath);
			_tcscat_s(m_pXConfigPath,MAX_PATH,_T("\\xconfig"));
		}
		XApp::~XApp()
		{
			if(m_pTimers) delete[] m_pTimers;
			if(m_pXEditTemplate) delete m_pXEditTemplate;
			if(m_pBrowerModulaPath) delete m_pBrowerModulaPath;
			if(m_pXConfigPath)delete m_pXConfigPath;
		}
		XEditTemplate * XApp::GetXEditTemplate()
		{
			return m_pXEditTemplate;
		}

		LRESULT CALLBACK XApp::XFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			int wmId, wmEvent;
			XWinbase*  pThis;

			pThis = (XWinbase *)::GetProp(hWnd,_T("this"));
			if(pThis && pThis->HandleMessage(hWnd,message,wParam,lParam))
			{
				if(message==WM_ERASEBKGND) return true;
				return 0;
			}

			switch (message)
			{
			case WM_COMMAND:
				{
					wmId    = LOWORD(wParam);
					wmEvent = HIWORD(wParam);

					if(pThis && pThis->HandleCommandMessage(message,wmId,wmEvent)) return 0;
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				break;
			case WM_CREATE:
				{
					LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
					if(lpcs->dwExStyle & WS_EX_MDICHILD)
					{
						MDICREATESTRUCTA * pMdics = (MDICREATESTRUCTA *)lpcs->lpCreateParams;
						if(pMdics)
						{
							if(pMdics->lParam)
							{
								::SetProp(hWnd,_T("this"),(HANDLE)pMdics->lParam);
							}
						}
					}
					else
					{
						if(lpcs->lpCreateParams)
							::SetProp(hWnd,_T("this"),lpcs->lpCreateParams);
					}
					pThis = (XWinbase * )::GetProp(hWnd,_T("this"));
					if(pThis)
						if(pThis->OnCreate(message,wParam,lParam)) return 0;
						if(pThis->HandleMessage(hWnd,message,wParam,lParam)) return 0;

				}
				break;
			case WM_DESTROY:
				//if(pThis)delete pThis;
				//pThis = NULL;
				::SetProp(hWnd,_T("this"),NULL);
				if(XApp::GetXApp()->m_hWnd == hWnd)::PostQuitMessage(0);
				break;
			default:
				{
					if(GetWindowLong(hWnd,GWL_EXSTYLE) & WS_EX_MDICHILD)
						return DefMDIChildProc(hWnd,message, wParam, lParam);
					else 
						return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			return 0;
		}

		int XApp::XFrameStartUp(HINSTANCE hInstance)
		{
			WNDCLASSEX wcex;

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style			= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= XFrameWndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= hInstance;
			wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName	= NULL;
			wcex.lpszClassName	= XFrameWndClass;
			wcex.hIconSm		= LoadIcon(wcex.hInstance, IDI_APPLICATION);

			RegisterClassEx(&wcex);

			if(!m_pXEditTemplate)m_pXEditTemplate = new XEditTemplate;
			TCHAR strTemplateFile[MAX_PATH];
			::_stprintf_s(strTemplateFile,MAX_PATH,_T("%s\\%s"),m_pXConfigPath,_T("edittemplate.xml"));
			this->m_pXEditTemplate->AddXmlTemplateParseFile(NULL,strTemplateFile);
			m_hSEditDll = ::LoadLibrary(_T("SciLexer.dll"));
			//m_hHtmlDll = ::LoadLibrary(_T("QHTMLight.dll"));

			DWORD dwThreadId;
			m_hThreadTimer = CreateThread( 
            NULL,              // default security attributes
            0,                 // use default stack size  
            TimerThreadProc,        // thread function 
            this,             // argument to thread function 
            0,                 // use default creation flags 
            &dwThreadId);   // returns the thread identifier
			
			return 1;
		}

		int XApp::XFrameShutDown(HINSTANCE hInstance)
		{
			UnregisterClass(XFrameWndClass,hInstance);
			if(m_hSEditDll)::FreeLibrary(m_hSEditDll);
			//if(m_hHtmlDll) ::FreeLibrary(m_hHtmlDll);

			if(m_hThreadTimer)
			{
				m_bShutDown = true;
				::WaitForSingleObject(m_hThreadTimer,INFINITE);
				::CloseHandle(m_hThreadTimer);

			}
			return 1;
		}

		HINSTANCE XApp::GetInstance()
		{
			return m_hInst;
		}

		XApp * XApp::GetXApp()
		{
			return g_pXApp;
		}

		int XApp::AddTimer(XTimer * pTimer)
		{
			int i=0;
			for(i=0;i<this->m_timerCount;i++)
			{
				if(!m_pTimers[i])
				{
					m_pTimers[i] = pTimer;
					return 1;
				}
			}
			if(i < this->m_timeAllocSize) 
			{
				m_pTimers[i] = pTimer;
				return 1;
			}
			
			XTimer ** pTemp;
			pTemp = new XTimer * [16];
			::ZeroMemory(pTemp,(m_timeAllocSize + 16)*sizeof(XTimer *));
			::memcpy(pTemp,m_pTimers,m_timerCount * sizeof(XTimer *));
			if(m_pTimers) delete[] m_pTimers;
			m_timeAllocSize += 16;
			m_timerCount+=1; 
			m_pTimers[i] = pTimer;

			return 1;
		}
		int XApp::RemoveTimer(XTimer * pTimer)
		{
			int i=0;
			for(i=0;i<this->m_timerCount;i++)
			{
				if(m_pTimers[i] == pTimer)
				{
					m_pTimers[i] = NULL;
					break;
				}
			}
			return 1;
		}

		int XApp::CreateXmlDoc(IXMLDOMDocument * &pXmlDocument)
		{
			//CreateInstance()
			IXMLDOMDocumentPtr pXmlDoc;
			//创建DOMDocument对象 
			HRESULT hr = pXmlDoc.CreateInstance(CLSID_DOMDocument40); 
			if(!SUCCEEDED(hr)) 
			{  
				//MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!",MB_OK); 
				HRESULT hr =pXmlDoc.CreateInstance(CLSID_DOMDocument30); 
				if(!SUCCEEDED(hr)) 
				{  
					return -1;
				}
			}
			pXmlDoc->put_async(VARIANT_FALSE);
			pXmlDocument = pXmlDoc;
			pXmlDocument->AddRef();
			return 1;
		};
	}
}