#include "StdAfx.h"
#include "MdiFrame.h"
#include "xcControl.h"
#include "moduladlg.h"

CMdiFrame::CMdiFrame(void):CanClosed(true)
{
	this->AttachListon(_T("WorkEnv"),CBN_SELENDOK,(FNEvent)&CMdiFrame::EvtWorkEnvSelected);
	this->AttachListon(WM_CLOSE,(FNEvent)&CMdiFrame::EvtClose);
	//this->AttachListon(WM_TIMER,(FNEvent)&CMdiFrame::EvtTimer);
}

CMdiFrame::~CMdiFrame(void)
{
	this->ClearAllListonsTo();
}

xfControl * CMdiFrame::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}
bool CMdiFrame::EvtTimer(TEvent *pEvent ,LPARAM lParam)
{
	int nIDEvent=(int) pEvent->wParam;
	if(nIDEvent == 0x999)
	{
		::SendMessage(this->GetHWND(), 0x401, (WPARAM)L"SysMsg", NULL);
		//OnXCommand(L"SYS:Msg",NULL);
	}
	return true;
}
int CMdiFrame::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcscmp(pStrID,_T("xmCancelClose"))==0)
	{
		CanClosed = false;
		return 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmReSetLogin"))==0)
	{
		if(::MessageBox(this->GetHWND(), L"确实切换用户吗？",L"提示",MB_OKCANCEL) == 2)
			return 1;

		if(xfApp::GetApp()->m_pDefaultView)
		{
			::ShowWindow(xfApp::GetApp()->m_pDefaultView->GetHWND(), SW_SHOW);
			::SendMessage(xfApp::GetApp()->m_pDefaultView->GetControl(_T("password"))->m_hWnd, WM_LBUTTONDOWN, 0, 0);
			::SendMessage(xfApp::GetApp()->m_pDefaultView->GetControl(_T("password"))->m_hWnd, WM_LBUTTONUP, 0, 0);
			xfApp::GetApp()->m_pDefaultView = NULL;
			::SendMessage(this->GetHWND(),WM_CLOSE,0,0);
		}
		else
		{
			ShellExecute(NULL, L"open", L"XExplorer.exe", NULL, NULL, SW_SHOW);
			::PostMessage(this->GetHWND(),WM_QUIT,0,0);
		}
		return 1;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmWorkNew"))==0)
	{
		//HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		//hCursor = ::SetCursor(hCursor);

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("/developer/modula.xpage"),strUrl,&dwSize,0);

		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);

		CModulaDlg*  pWinEx = new CModulaDlg();
		pWinEx->Create(strUrl,rt);
		pWinEx->m_pParentWin = this;
		//hCursor = ::SetCursor(hCursor);
		//DestroyCursor(hCursor);
		return 1;

	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileOpen"))==0)
	{
		TCHAR filter[]= _T("xpage 文件(*.xpage)\0*.xpage;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("xpage 文件(*.xpage)\0*.xpage\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
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
			xfWin*  pWinEx = xfWin::LoadXPage(szPath,m_hWnd,xfApp::GetApp()->GetInstance());
		}
		return 1;

	}
	if(pStrID && _tcscmp(pStrID,_T("xmNavigateTo"))==0)
	{
		xfNodeControl * pEditNode = dynamic_cast<xfNodeControl *>(pControl->m_pxfNode->GetRootNode()->GetNodeByName(_T("UrlEditor")));
		if(!pEditNode) return 1;
		TCHAR strUrl[4096];
		xfControl * pEditControl = pEditNode->m_pxfControl;
		if(!pEditControl) return 1;
		::GetWindowText(pEditControl->m_hWnd,strUrl,4096);

		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		if(xfApp::GetApp()->m_pConnect)
		{
			delete xfApp::GetApp()->m_pConnect;
			xfApp::GetApp()->m_pConnect = NULL;
		}

		xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,this->m_hWnd,xfApp::GetApp()->GetInstance());
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);

		return 1;
	}

	if(pStrID && (_tcscmp(pStrID,_T("xmSheetClose"))==0 || _tcscmp(pStrID,_T("xmFileClose"))==0))
	{
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0 && pLayerSheet->m_iCurSheet!=0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet];

			LYSNMHDR hdr;
			memset(&hdr,0,sizeof(LYSNMHDR));
			hdr.hwndFrom = pLayerSheet->GetHWND();
			hdr.idFrom = (int)hWnd;
			hdr.code = 0x0003;//LYSN_CLOSESHEET;
			hdr.nSheet = pLayerSheet->m_iCurSheet;
			::SendMessage(m_hWnd,WM_NOTIFY,0,(LPARAM)&hdr);

			if(hdr.code < 0 || hdr.nSheet < 0) return 1;

			pLayerSheet->m_pHwnds->erase(pLayerSheet->m_pHwnds->begin()+pLayerSheet->m_iCurSheet);
			pLayerSheet->RemoveSheet(pLayerSheet->m_iCurSheet);

			::PostMessage(hWnd,WM_POSTDESTROYWINDOW,0,0);
			//::DestroyWindow(hWnd);
		}
		return 1;
	}

	if(pControl && pControl->m_pxfNode)
	{
		LPTSTR pUrl = NULL;
		pUrl = xbXml::GetStringAttribe(pControl->m_pxfNode->m_pElement,_T("url"));
		if(pUrl)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			if(!pControl->m_pxfNode->GetXPage()->m_pStrUrl || 
				UrlIsFileUrl(pControl->m_pxfNode->GetXPage()->m_pStrUrl))
			{
				TCHAR path[MAX_PATH];
				::GetModuleFileName(NULL,path,MAX_PATH);
				::PathRemoveFileSpec(path);
				TCHAR buf[MAX_PATH];
				::_stprintf_s(buf,MAX_PATH,_T("%s\\%s"),path,pUrl);
				xfWin*  pWinEx = xfWin::LoadXPage(buf,NULL,xfApp::GetApp()->GetInstance());
			}else
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,pUrl,strUrl,&dwSize,0);
				xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
				xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,m_hWnd,xfApp::GetApp()->GetInstance());
			}
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
			delete pUrl;
			return 1;
		}
	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileExit"))==0)
	{
		::PostQuitMessage(0);
		return 1;
	}

	return xfMdiFrame::OnXCommand(pStrID,pControl);
}

int CMdiFrame::OnOpen()
{
	if(!xfApp::GetApp()->m_pFrame) xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
	//int nMsg = ::GetPrivateProfileInt(L"default",L"Msg",3,L"cache/user.ini");
	//::SetTimer(GetHWND(),0x999, nMsg * 60000, NULL);

	return 1;

	xfWin * pWin = 0;
	if(!(GetKeyState(VK_CONTROL) & 0x8000))
	{
		pWin = xfWin::LoadXPage(NULL,_T("workstart.xml"),m_hWnd,xfApp::GetApp()->GetInstance());
		if(!pWin)
		{
			pWin = xfWin::LoadXPage(NULL,_T("/uriserver/uri.hxsoft.com/xaserver/dev:xpage[workstart.xpage]"),m_hWnd,xfApp::GetApp()->GetInstance());
			if(!pWin)
				xfWin::LoadXPage(NULL,_T("/uriserver/uri.hxsoft.com/xaserver/dev:xpage[workstartex.xpage]"),m_hWnd,xfApp::GetApp()->GetInstance());
		}
	}else
	{
		pWin = xfWin::LoadXPage(NULL,_T("/uriserver/uri.hxsoft.com/xaserver/dev:xpage[workstart.vx]"),m_hWnd,xfApp::GetApp()->GetInstance());
		if(!pWin)
		{
			if(!xfWin::LoadXPage(NULL,_T("workstart.xml"),m_hWnd,xfApp::GetApp()->GetInstance()))
			{
				xfWin * pWin = xfWin::LoadXPage(NULL,_T("/uriserver/uri.hxsoft.com/xaserver/dev:xpage[workstart.xpage]"),m_hWnd,xfApp::GetApp()->GetInstance());
				if(!pWin)
					xfWin::LoadXPage(NULL,_T("/uriserver/uri.hxsoft.com/xaserver/dev:xpage[workstartex.xpage]"),m_hWnd,xfApp::GetApp()->GetInstance());
			}
		}
	}

	
	return 1;
}

int CMdiFrame::AddSheet(xfWin * pWin)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(pWin)
	{
		pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);
		pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,-1,true);

	}
	return 1;
}

bool  CMdiFrame::EvtWorkEnvSelected(TEvent* pEvent,LPARAM lParam)
{
	TCHAR buf[40];
	TCHAR pSheet[80];
	int nIndex = (int)::SendMessage(this->GetControl(_T("WorkEnv"))->m_hWnd,CB_GETCURSEL,0,0);
    if(nIndex < 0) return true;
	
	_stprintf_s(buf,40,_T("item[%i]"),nIndex + 1); 
	IXMLDOMNode * pNode;
	IXMLDOMElement * pElement;
    _variant_t var;
    pSheet[0]='\0';
    
	this->GetControl(_T("WorkEnv" ))->m_pxfNode->m_pElement->selectSingleNode(buf,&pNode);
    if(pNode)
    {
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		pElement->getAttribute(L"data",&var);
        if(var.bstrVal)
		{
			_tcscpy_s(pSheet,80,var.bstrVal);
		}
		pElement->Release();
		pNode->Release();

	}
	if(pSheet[0])
	{
		delete xfApp::GetApp()->m_WorkEnv;
		xfApp::GetApp()->m_WorkEnv = _tcsdup(pSheet);
	}
		
	return true;
}
bool  CMdiFrame::EvtClose(TEvent* pEvent,LPARAM lParam)
{
	SendMessage(GetHWND(),WM_XCOMMAND,(WPARAM)L"OnFrameClose",0); 
	if(!CanClosed)
	{
		CanClosed = true;
		pEvent->dwRet = 1;
		return true;
	}
	IXAServer::SetLoginLog(GetFrameWindow()->m_pXPage->m_pStrUrl, L"退出");
	if(xfApp::GetApp()->m_pDefaultView)
	{
		xfApp::GetApp()->m_pDefaultView->m_dwStatus &= ~WST_RESPONSELOOP;
		//::SendMessage(xfApp::GetApp()->m_pDefaultView->GetHWND(),WM_CLOSE,0,0);
		if(xfApp::GetApp()->m_pDefaultView->GetHWND())
		::PostMessage(xfApp::GetApp()->m_pDefaultView->GetHWND(),WM_QUIT,0,0);
	}
	return true;
}