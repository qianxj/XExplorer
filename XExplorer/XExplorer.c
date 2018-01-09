// XExplorer.cpp : 定义应用程序的入口点。
//

#include "start.h"
#include "XExplorer.h"

#define _T(x)	L##x

//#include "xinclude.h"
//using namespace Hxsoft::XFrame;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);  

HWND	hwndMDIClient;
HWND	hwndFrame;

int APIENTRY xWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

   //:: CoInitialize(NULL);
   //XFrameStartUp(hInst);

   // 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XEXPLORER));

	// 主消息循环:
	BOOL bRet;

	while ((bRet = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else 
		{ 
			if (!TranslateMDISysAccel(hwndMDIClient, &msg) && 
				!TranslateAccelerator(hwndFrame, hAccelTable , &msg))
			{ 
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			} 
		} 
	}


	//CoUninitialize();
	//XFrameShutDown(hInst);
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XEXPLORER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_XEXPLORER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_OPEN:
			{
				/*
				TCHAR filter[]= _T("xpage 文件(*.xpage)\0*.xpage\0全部文件(*.*)\0*.*\0");
				TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
				TCHAR szCustomFilter[MAX_PATH];
				int	 nFilterIndex = 1;

				ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
				StrCpy(szCustomFilter,_T("xpage 文件(*.xpage)\0*.xpage\0"));

				OPENFILENAME ofn;
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.hInstance = NULL;
				ofn.lpstrFilter = filter;
				ofn.lpstrCustomFilter = szCustomFilter;
				ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
				ofn.nFilterIndex = nFilterIndex;
				ofn.lpstrFile = szPath;
				ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
				ofn.lpstrFileTitle = szFileName;
				ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
				ofn.lpstrTitle = _T("文件选择");
				ofn.lpstrDefExt = _T("xml");
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
				ofn.lCustData = 0;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;

				if(::GetOpenFileName(&ofn))
				{
					IXMLDOMDocumentPtr pXmlDoc;
					IXMLDOMDocument2* pXmlDoc2;
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

					pXmlDoc->QueryInterface(IID_IXMLDOMDocument2,(void **)&pXmlDoc2);
					pXmlDoc2->setProperty((_bstr_t)L"SelectionLanguage", (_variant_t)L"XPath");
					pXmlDoc2->Release();

					pXmlDoc->put_async(VARIANT_FALSE);

					VARIANT_BOOL bSuccess;
					pXmlDoc->load((_variant_t)szPath,&bSuccess);
					if(bSuccess != VARIANT_TRUE)
					{
						 
						IXMLDOMParseErrorPtr  pXMLErr;
						pXmlDoc->get_parseError(&pXMLErr);

						BSTR bstr = NULL;
						pXMLErr->get_reason(&bstr);
						if(bstr)::SysFreeString(bstr);
						return 0;
 					}
					RECT rc;
					::SetRect(&rc,0,0,400,600);
					XWin<void *> *  pWin = new XWin<void *>;
					pWin->CreateEx(WS_EX_MDICHILD,WS_OVERLAPPEDWINDOW,XFrameWndClass,_T("this is test"),rc,hwndMDIClient,0,hInst);
					 IXMLDOMElementPtr   p;
					pXmlDoc->get_documentElement(&p);
					::GetClientRect(pWin->m_hWnd,&rc);
					CreateFromNode(p,pWin->m_hWnd,rc,hInst);
					//::CreateWindow(_T("button"),_T("Hello"),WS_VISIBLE|WS_CHILD,30,30,120,48,pWin->m_hWnd,NULL,NULL,NULL);
					::ShowWindow(pWin->m_hWnd,SW_SHOW);
					::UpdateWindow(pWin->m_hWnd);
					pWin->m_hDLL = ::LoadLibrary(_T("test.dll"));
				}*/
			}
			break;
		default:
			return DefFrameProc(hWnd, hwndMDIClient, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			CLIENTCREATESTRUCT ccs; 
	 
			// Retrieve the handle to the window menu and assign the 
			// first child window identifier. 
	 
			ccs.hWindowMenu = NULL;//NULL;GetSubMenu(GetMenu(hWnd), 1); 
			ccs.idFirstChild = 0;//IDM_WINDOWCHILD; 
	 
			// Create the MDI client window. 
	 
			hwndMDIClient = CreateWindow( _T("MDICLIENT"), (LPCTSTR) NULL, 
				WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, 
				0, 0, 0, 0, hWnd, (HMENU) 0xCAC, hInst, (LPSTR) &ccs); 
	 
			ShowWindow(hwndMDIClient, SW_SHOW);
		}
		break;/*
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			::Ellipse(hdc,0,0,500,600);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_SIZE:
		{
			UINT fwSizeType;
			int cx,cy;
			cx = LOWORD(lParam); 
			cy = HIWORD(lParam);
			fwSizeType = (UINT)wParam;
			if(fwSizeType !=SIZE_MINIMIZED)
			{
				::SetWindowPos(hwndMDIClient,NULL,0,70,cx,cy - 70,SWP_SHOWWINDOW);
			}
			return 0;
		}
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefFrameProc(hWnd, hwndMDIClient, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
