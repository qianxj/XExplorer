// XExplorer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "XExplorer.h"
#include "xframe.h"
#include "xuxutil.h"
#include "xshape.h"
#include "xcControl.h"
#include <gdiplus.h>
#include <atlbase.h>

using namespace Hxsoft::XFrame;

// 全局变量:
HINSTANCE hInst;								// 当前实例


xfApp theXApp;
extern  XFRAME_API xfApp* g_pXApp; 

#define   UM_ICONNOTIFY   WM_USER+100


int TransToMap(IXMLDOMNode* pNode,std::wstring path, map<std::wstring,std::wstring> &m)
{
	IXMLDOMNode* pItem = NULL;
	IXMLDOMNode* pAttrItem = NULL;
	IXMLDOMNodeList* pNodeList = NULL;
	BSTR bstr;
	pNode->selectSingleNode(L"@name",&pAttrItem);
	if(pAttrItem)
	{
		pAttrItem->get_text(&bstr);
		pAttrItem->Release();
		if(bstr && bstr[0]) 
			if(path==L"")
				path =  bstr;
			else
				path = path + L"/" + bstr;
		if(bstr) ::SysFreeString(bstr);
	}
	pNode->selectNodes(L"file",&pNodeList);
	long nLen;
	std::wstring wfile;
	std::wstring wversion;
	pNodeList->get_length(&nLen);
	for(int i=0;i<nLen;i++)
	{
		pNodeList->get_item(i,&pItem);
		pItem->selectSingleNode(L"@name",&pAttrItem);
		wfile = L"";
		if(pAttrItem)
		{
			pAttrItem->get_text(&bstr);
			pAttrItem->Release();
			if(bstr && bstr[0])
			{
				if(path==L"")
					wfile =  bstr;
				else
					wfile = path + L"/" + bstr;
			}
			if(bstr) ::SysFreeString(bstr);
		}
		if(wfile==L"")
		{
			pItem->Release();
			continue;
		}
		wversion =L"";
		pItem->selectSingleNode(L"@version",&pAttrItem);
		if(pAttrItem)
		{
			pAttrItem->get_text(&bstr);
			pAttrItem->Release();
			if(bstr && bstr[0])
			{
				wversion = bstr;
			}
			if(bstr) ::SysFreeString(bstr);
		}
		pItem->Release();
		m.insert(std::pair<std::wstring,std::wstring>(wfile,wversion));
	}
	pNodeList->Release();

	pNode->selectNodes(L"folder",&pNodeList);
	pNodeList->get_length(&nLen);
	for(int i=0;i<nLen;i++)
	{
		pNodeList->get_item(i,&pItem);
		TransToMap(pItem,path,m);
		pItem->Release();
	}
	pNodeList->Release();

	return 1;
}

bool CheckUpdater(LPTSTR pAppName)
{

	bool bResult = false;

	xbXml xml;
	xbXml xv,xl;
	IXMLDOMNode* pNode=NULL;
	std::vector<std::wstring> diff;
	std::vector<std::wstring> dfile;
	xml.Load(L"updater/updater.xml");

	BSTR bstrVer = NULL;
	BSTR bstrServer = NULL;
	TCHAR buf[255];
	_stprintf_s(buf,255,L"//application[@name='%s']/@version",pAppName);
	xml.GetXmlDoc()->selectSingleNode(buf,&pNode);
	if(pNode)pNode->get_text(&bstrVer);
	if(pNode)pNode->Release();
	xml.GetXmlDoc()->selectSingleNode(L"//server[@update='yes'][1]/@url",&pNode);
	if(!pNode)
		xml.GetXmlDoc()->selectSingleNode(L"//server[1]/@url",&pNode);
	if(pNode)pNode->get_text(&bstrServer);
	if(pNode)pNode->Release();

	if(!bstrVer || !bstrServer) return false;

	DWORD dwSize;
	TCHAR strHostname[INTERNET_MAX_HOST_NAME_LENGTH ];
	dwSize = INTERNET_MAX_HOST_NAME_LENGTH;
	UrlGetPart(bstrServer,strHostname,&dwSize,URL_PART_HOSTNAME,URL_ESCAPE_UNSAFE);

	USES_CONVERSION;
	if(gethostbyname(T2A(strHostname)))
	{
		xfConnect* pConnect = xfUriLoader::GetConnect(bstrServer);
		if(!pConnect) return false;

		TCHAR cmd[255];
		_stprintf_s(cmd,255,L"update/version?ver=%s&application=%s",bstrVer,pAppName);
		BYTE* pData = NULL;
		int nLen = 0;
		if(xfUriLoader::ProcessCmd(NULL,cmd,NULL,pData,nLen)==1)
		{
			if(!pData) return false;
			std::wstring ws;
			map<std::wstring, std::wstring> mdest;
			map<std::wstring, std::wstring> msrc;

			if(xv.LoadXml(LPTSTR(pData+2)))
			{
				ws = (std::wstring)L"updater/update-xexplorer-"+bstrVer+L".xml";
				if(xl.Load(ws.c_str()))
				{
					xl.GetXmlDoc()->selectSingleNode(L"application/folder[1]",&pNode);
					TransToMap(pNode,L"",mdest);
					pNode->Release();
				}
				xv.GetXmlDoc()->selectSingleNode(L"application/folder[1]",&pNode);
				TransToMap(pNode,L"",msrc);
				pNode->Release();

				map<std::wstring, std::wstring>::const_iterator tr,tr1;
				for(tr=msrc.begin();tr!=msrc.end();tr++)
				{
					if(tr->second!=L"")
					{
						tr1 = mdest.find(tr->first);
						if( tr1!=mdest.end() && tr1->second!=tr->second)
							diff.push_back(tr->first);
						else if(tr1==mdest.end())
							diff.push_back(tr->first);
					}
				}
			}
		}
		if(pData) xfUriLoader::FreeData(pData);

		if(::PathFileExists(L"update.bat"))
			::DeleteFile(L"update.bat");
		
		if(::PathFileExists(L"updater/tmp"))
			::RemoveDirectory(L"updater/tmp");
		::CreateDirectory(L"updater/tmp",NULL);

		std::wstring wcopy = (std::wstring)L"if exist resource\\image\\icons (echo \"\") else md resource\\image\\icons ";
		for(int i=0;i<(int)diff.size();i++)
		{
			_stprintf_s(cmd,255,L"update/file?file=%s&application=%s",diff[i].c_str(),pAppName);
			BYTE* pData = NULL;
			int nLen = 0;
			TCHAR tempFile[255];
			if(xfUriLoader::ProcessCmd(NULL,cmd,NULL,pData,nLen)==1)
			{
				GetTempFileName(L"updater/tmp",L"upd",0,tempFile);
				DWORD dwSize;
				HANDLE hFile = ::CreateFile(tempFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				WriteFile(hFile,pData,nLen,&dwSize,NULL);
				CloseHandle(hFile);
				if(_tcscmp(diff[i].c_str(), L"updater/updater.xml") == 0) //更新update.xml
					xml.Load(tempFile);
				else
				{
					if(wcopy!=L"")
						wcopy += (std::wstring)L"\r\ncopy /Y \"" + tempFile+L"\" /B \""+ diff[i] +L"\"";
					else
						wcopy =(std::wstring)L"copy /Y \"" + tempFile+L"\" /B \""+ diff[i] +L"\"";
				}
			}
		}
		if(wcopy!=L"")
		{
			wcopy = (std::wstring)L"taskkill /f /im \"XExplorer.exe\"" + (std::wstring)L"\r\n" + wcopy;
			wcopy += (std::wstring)L"\r\nrd \"updater\\tmp\" /s/q";
			wcopy += (std::wstring)L"\r\ndel /q \"updater\\update-xexplorer-" + bstrVer + (std::wstring)L".xml\"";
			wcopy += (std::wstring)L"\r\nstart \"\" \"XExplorer.exe\"";
			DWORD dwSize;
			HANDLE hFile = ::CreateFile(L"update.bat",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			WriteFile(hFile,T2A(wcopy.c_str()),wcopy.length(),&dwSize,NULL);
			CloseHandle(hFile);

			_stprintf_s(buf,255,L"//application[@name='%s']/@version",pAppName);
			xv.GetXmlDoc()->selectSingleNode(buf,&pNode);
			if(pNode)
			{
				BSTR bVer;
				pNode->get_text(&bVer);
				pNode->Release();
				if(bVer)
				{
					std::wstring ws = (std::wstring)L"updater/update-xexplorer-"+bVer+L".xml";
					xv.GetXmlDoc()->save((_variant_t)ws.c_str());
					xml.GetXmlDoc()->selectSingleNode(buf,&pNode);
					pNode->put_text(bVer);
					xml.GetXmlDoc()->save((_variant_t)"updater/updater.xml");
					::SysFreeString(bVer);
					pNode->Release();
					bResult = true;
				}
			}
		}

		if(pConnect)
		{
			pConnect->CloseSocket(pConnect->m_socket);
			xfApp::GetApp()->m_pConnect = NULL;
		}
	}
	if(bstrVer)::SysFreeString(bstrVer);
	if(bstrServer)::SysFreeString(bstrServer);
	return bResult;
}
xfWin * openDialogParam(LPTSTR lpCmdLine)
{
	char * str = Hxsoft::XUtil::strdupFromBstr(lpCmdLine);
	char username[30],password[50];
	sscanf_s(str, "%*[^=]=%[^&]&%*[^=]=%[^&]", username,password);

	BSTR bstr; 
	int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, username, (int)strlen(username), NULL, 0);
	bstr = SysAllocStringLen(NULL,cchWide + 1);
	bstr[cchWide]=0;
	::MultiByteToWideChar(CP_UTF8, 0, username, (int)strlen(username), bstr, cchWide + 1);

	xfApp::GetApp()->m_pUser = (LPTSTR)bstr;

	cchWide = ::MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), NULL, 0);
	bstr = SysAllocStringLen(NULL,cchWide + 1);
	bstr[cchWide]=0;
	::MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), bstr, cchWide + 1);
	xfApp::GetApp()->m_pPassWord = (LPTSTR)bstr;

	LPVOID p = (LPVOID)lpCmdLine;
	return xfWin::LoadXPage(NULL,lpCmdLine,NULL,hInst, p);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hInst = hInstance;
	g_pXApp = &theXApp;

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
   :: CoInitialize(NULL);

   xfApp::GetApp()->m_hInst = hInst;
   xfApp::GetApp()->XFrameStartUp(hInst,IDI_XEXPLORER,IDI_SMALL);
   
   XSEdit::LoadScintillaDll(_T("SciLexer.dll"));

  
	ULONG_PTR gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if(lpCmdLine && !_tcsstr(lpCmdLine,L"-noupdate")) 
	{
		bool updateRet = CheckUpdater(L"xexplorer");
		if(updateRet && ::PathFileExists(L"update.bat"))
		{
			CoUninitialize();
			xfApp::GetApp()->XFrameShutDown(hInst);
			//Shell_NotifyIcon(NIM_DELETE, &nid);
			if(gdiplusToken != 0 )Gdiplus::GdiplusShutdown(gdiplusToken);

			ShellExecute(NULL, L"open", L"update.bat", NULL, NULL, SW_HIDE);
			return 1;
		}

		if(!updateRet && ::PathFileExists(L"update.bat"))
			::DeleteFile(L"update.bat");
	}else
		if(lpCmdLine) lpCmdLine+= _tcsstr(lpCmdLine,L"-noupdate") - lpCmdLine + _tcslen(L"-noupdate");

   //g_pXApp->m_pShapeMgr->RegisterShapesByFile(_T("xconfig\\shapes.xml"));

   if(lpCmdLine && _tcsstr(lpCmdLine,L"-dev"))
		xfApp::GetApp()->m_bDev = true;
   else
	   xfApp::GetApp()->m_bDev = false;

   xfWin * pWin;
   if(!lpCmdLine || lpCmdLine[0]=='\0' || (lpCmdLine && _tcscmp(lpCmdLine,L"-dev") == 0))
   {
		pWin = (xfWin::LoadXPage(_T("index.xpage"),NULL,hInst));
		
		LPCTSTR bstrVer = IXAServer::GetVersion();
		if(pWin && pWin->GetControl(_T("statusbar0")))
			if(bstrVer) SendMessage(pWin->GetControl(_T("statusbar0"))->m_hWnd,SB_SETTEXT,0,(LPARAM)bstrVer);
   }
   else
   {
		DWORD dwSize;
		TCHAR strScheme[INTERNET_MAX_SCHEME_LENGTH];
		dwSize = INTERNET_MAX_SCHEME_LENGTH;
		UrlGetPart(lpCmdLine,strScheme,&dwSize,URL_PART_SCHEME,0);
		if(::StrCmpI(strScheme,L"http")!=0)
		{
			pWin = (xfWin::LoadXPage(lpCmdLine,NULL,hInst));
			//pWin = openDialogParam(lpCmdLine);
		}
		else
		{
			pWin = openDialogParam(lpCmdLine);
			//pWin = (xfWin::LoadXPage(NULL,lpCmdLine,NULL,hInst)); 
		}
		if(!pWin)
		{
			CoUninitialize();
			xfApp::GetApp()->XFrameShutDown(hInst);
			return -1;
		}
    }

    NOTIFYICONDATA nid = {0};
	nid.cbSize   =   sizeof(nid);
	nid.hWnd   =   pWin->m_hWnd;
	nid.uID   =  1;       
	nid.uFlags   =   NIF_MESSAGE   |   NIF_ICON   |   NIF_TIP;     
	nid.uCallbackMessage   =   UM_ICONNOTIFY;     
	_tcscpy_s(nid.szTip,128,_T("this is application Explorer"));
	//nid.uFlags = NIF_ICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XEXPLORER1)); 

    //Add the notification to the tray
    Shell_NotifyIcon(NIM_ADD, &nid);

	/*
    //Update the icon of the notification
    nid.uFlags = NIF_ICON;
    nid.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_SAMPLEICON2));
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    //remove the notification from the tray
    Shell_NotifyIcon(NIM_DELETE, &nid);
	*/

   xfMdiFrame *	pFrame = dynamic_cast<xfMdiFrame *>(pWin);
   if(pFrame)
   {
	   xfApp::GetApp()->m_hWnd = pFrame->m_hWnd;
	   xfApp::GetApp()->m_hMdiClient = pFrame->m_hMdiClient;
   }else
	   xfApp::GetApp()->m_hWnd = pWin->m_hWnd;

   hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XEXPLORER));

	// 主消息循环:
	BOOL bRet;

	int nIdelLevel = 0;
	bool bIdle = false;

	DWORD dwStart = 0;
	DWORD dwIdle = 0;
	DWORD dRet = 0;
	bool  bExit = false;
	DWORD   dwLastMsg = NULL;
	for(;;)
	{
		dwStart = 0;
		while(bIdle)
		{
			dwStart = GetTickCount();
			dRet = MsgWaitForMultipleObjects(0,NULL,false,20,QS_ALLINPUT);
			if(dRet == WAIT_TIMEOUT)
			{
				nIdelLevel = -1; 
				if(dwIdle==0)
					while(xfApp::GetApp()->OnIdle(nIdelLevel++));

				dwIdle += GetTickCount() - dwStart;
				xfApp::GetApp()->Idle(dwIdle);
			}else
			{
				bIdle = false; 
				break;
			}
		}

		do
		{
			bRet = GetMessage(&msg, (HWND) NULL, 0, 0);
			if(bRet==0)
			{
				xfApp::GetApp()->ExitApp();
				bExit = true;
				break;
			}

			if (bRet == -1)
			{
				// handle the error and possibly exit
			}
			else 
			{ 
				if(pFrame )
				{
					if (!TranslateMDISysAccel(pFrame->m_hMdiClient, &msg) && 
						!TranslateAccelerator(pFrame->m_hWnd, hAccelTable , &msg))
					{ 
						TranslateMessage(&msg); 
						DispatchMessage(&msg); 
					}
				}else
				{
					if (!TranslateAccelerator(pWin->m_hWnd, hAccelTable , &msg))
					{ 
						TranslateMessage(&msg); 
						DispatchMessage(&msg); 
					}
				}
				if(msg.message != WM_PAINT && msg.message != 0x0118)
				{
					/*if(msg.message == WM_MOUSEMOVE || msg.message == WM_NCMOUSEMOVE)
					{
						if(dwLastMsg == msg.message && ptLast.x == msg.pt.x && ptLast.y == msg.pt.y )
						{
							bIdle = true;
							dwIdle = 0;
						}else
						{
							dwLastMsg = msg.message;
							ptLast = msg.pt;
						}
					}else
					{*/
						bIdle = true;
						dwIdle = 0;
					//}
				}
			} 
		}while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
		if(bExit)break;
	}

	CoUninitialize();
	xfApp::GetApp()->XFrameShutDown(hInst);

	Shell_NotifyIcon(NIM_DELETE, &nid);

	if(gdiplusToken != 0 )Gdiplus::GdiplusShutdown(gdiplusToken);

	return (int) msg.wParam;
}

