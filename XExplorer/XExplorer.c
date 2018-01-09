// XExplorer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "start.h"
#include "XExplorer.h"

#define _T(x)	L##x

//#include "xinclude.h"
//using namespace Hxsoft::XFrame;

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

   //:: CoInitialize(NULL);
   //XFrameStartUp(hInst);

   // ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XEXPLORER));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
				TCHAR filter[]= _T("xpage �ļ�(*.xpage)\0*.xpage\0ȫ���ļ�(*.*)\0*.*\0");
				TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
				TCHAR szCustomFilter[MAX_PATH];
				int	 nFilterIndex = 1;

				ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
				ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
				StrCpy(szCustomFilter,_T("xpage �ļ�(*.xpage)\0*.xpage\0"));

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
				ofn.lpstrTitle = _T("�ļ�ѡ��");
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
					//����DOMDocument���� 
					HRESULT hr = pXmlDoc.CreateInstance(CLSID_DOMDocument40); 
					if(!SUCCEEDED(hr)) 
					{  
						//MessageBox("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!",MB_OK); 
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

// �����ڡ������Ϣ�������
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
