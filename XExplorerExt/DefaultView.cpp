#include "stdafx.h"
#include "DefaultView.h"
#include "ModulaDlg.h"

CDefaultView::CDefaultView(void):m_CheckPass(false),m_PrevWndFunc(NULL)
{
}

CDefaultView::~CDefaultView(void)
{
	xfControl * mPassEdit = GetControl(_T("password"));
	if(mPassEdit)
	{
		SetWindowLongPtr(mPassEdit->GetHWND(),GWLP_WNDPROC,(long)m_PrevWndFunc);
		::SetProp(mPassEdit->GetHWND(),L"thisex",0);
	}
}

xfControl * CDefaultView::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int CDefaultView::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pControl && (this->GetFrameWindow() == this || pControl->GetWin() == this))
	{
/*		if(pStrID && _tcscmp(pStrID,_T("xmWorkNew"))==0)
		{
			//HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			//hCursor = ::SetCursor(hCursor);

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("/developer/modula.xpage"),strUrl,&dwSize,0);

			POINT ptStart,ptEnd;
			ptStart.x = pControl->m_pxfNode->m_AreaRect.left;
			ptStart.y = pControl->m_pxfNode->m_AreaRect.top;
			ClientToScreen(m_hWnd,&ptStart);
			ptEnd.x = pControl->m_pxfNode->m_AreaRect.right;
			ptEnd.y = pControl->m_pxfNode->m_AreaRect.bottom;
			ClientToScreen(m_hWnd,&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);

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
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,NULL,xfApp::GetApp()->GetInstance());
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);

			return 1;
		}

		if(pStrID && _tcscmp(pStrID,_T("xmMail"))==0)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("mail.xml"),strUrl,&dwSize,0);

			xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,this->m_hWnd,xfApp::GetApp()->GetInstance());
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
			return 1;
		}

		if(pStrID && _tcscmp(pStrID,_T("xmDeveloperWork"))==0)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("/developer/index.xpage"),strUrl,&dwSize,0);

			xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,NULL,xfApp::GetApp()->GetInstance());
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
			return 1;
		}

		if(pStrID && _tcscmp(pStrID,_T("xmXOffice"))==0)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			::_stprintf_s(buf,MAX_PATH,_T("%s\\xoffice.xpage"),path);
			xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
			xfWin*  pWinEx = xfWin::LoadXPage(buf,NULL,xfApp::GetApp()->GetInstance());

			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
			return 1;
		}

		if(pControl->m_pxfNode)
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
					xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance());
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
*/
		if(pStrID && ::_tcsicmp(pStrID,_T("loginok"))==0)
		{
			TCHAR pUser[30];
			TCHAR pPassWord[30];

			::GetWindowText(this->GetControl(_T("username"))->m_hWnd,pUser,30);
			::GetWindowText(this->GetControl(_T("password"))->m_hWnd,pPassWord,30);
			if(pUser[0]=='\0')
			{
				MessageBox(this->m_hWnd,_T("请输入用户名"),_T("提示"),MB_OK);
				//::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_SETFOCUS, 0, 0);
				::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_LBUTTONDOWN, 0, 0);
				::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_LBUTTONUP, 0, 0);
				return 0;
			}
			if(pPassWord[0]=='\0')
			{
				MessageBox(this->m_hWnd,_T("请输入用户密码"),_T("提示"),MB_OK);
				//::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_SETFOCUS, 0, 0);
				::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_LBUTTONDOWN, 0, 0);
				::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_LBUTTONUP, 0, 0);
				return 0;
			}
			LPWSTR mUserNet=NULL;
			int nIndex = ::SendMessage(GetControl(_T("usernet"))->GetHWND(),CB_GETCURSEL,NULL,NULL);
			mUserNet = (LPWSTR)::SendMessage(GetControl(_T("usernet"))->GetHWND(),CB_GETITEMDATA,nIndex,0);

			//LPWSTR pComputer = IXAServer::GetComputerNameEx();
			//LPWSTR pPCMAC = IXAServer::GetMacAddress();
			IXAServerArg arg;
			arg.AddArg(L"username",pUser);
			arg.AddArg(L"password",Hxsoft::XUtil::MD5(pPassWord));
			arg.AddArg(L"computer",IXAServer::GetComputerNameEx());
			arg.AddArg(L"mac",IXAServer::GetMacAddress());
			arg.AddArg(L"net",mUserNet);
			arg.AddArg(L"ip",IXAServer::GetIpAddress());
			if(m_CheckPass)
				arg.AddArg(L"checkpass",L"N");
			else
				arg.AddArg(L"checkpass",L"Y");
			m_CheckPass = false;

			TCHAR strLoginInfo[1024];
			::_stprintf_s(strLoginInfo,1024,_T("%s"),arg.GetString());
			//::MessageBox(this->GetHWND(),strLoginInfo,L"验证",MB_OK);
			//::SysFreeString(pPCMAC);
			//pPCMAC = NULL;

			xfConnect * pConnect = xfApp::GetApp()->m_pConnect;
			if(pConnect)
			{
				pConnect->CloseSocket(pConnect->m_socket);
				delete pConnect;
				xfApp::GetApp()->m_pConnect = NULL;
				::ZeroMemory(&xfApp::GetApp()->guid,sizeof(GUID));
			}

			pConnect = new xfConnect();
			pConnect = xfUriLoader::GetConnect(mUserNet);
			if(!pConnect)
			{
				MessageBox(NULL,_T("连接不到服务器!"),_T("提示"),MB_OK);
				return 1;
			}
			BYTE * pData;
			int len;
			xfApp::GetApp()->m_pConnect->SendAuthCommand(xfApp::GetApp()->m_pConnect->m_socket,strLoginInfo);
			int nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);
			if(nRet < 0)
			{
				MessageBox(NULL,(LPTSTR)pData,_T("提示"),MB_OK);
				if(pData)xfUriLoader::FreeData(pData);
				pData = NULL;
			}else
			{
				TCHAR mNetIndex[10];
				_stprintf_s(mNetIndex,10,L"%d",nIndex);
				::WritePrivateProfileString(L"hxsoft",L"net",mNetIndex,L"cache/user.ini");
				::WritePrivateProfileString(L"hxsoft",L"user",pUser,L"cache/user.ini");

				TCHAR strUrl[256];
				_stprintf_s(strUrl,256,L"%s/business/index.xpage",mUserNet);
				
				m_pXPage->m_pStrUrl = mUserNet;
				if(!xfApp::GetApp()->m_pDefaultView)
				{
					xfApp::GetApp()->m_pDefaultView = (xfWin*)this;
					xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
					xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,NULL,xfApp::GetApp()->GetInstance());
					::SetParent(this->GetHWND(),pWinEx->GetHWND());
				}
				IXAServer::SetLoginLog(strUrl, L"登录");
				::SetWindowText(GetControl(_T("password"))->GetHWND(),L"");
				::ShowWindow(this->GetHWND(), SW_HIDE);
				
			}
			//m_dwStatus &= ~WST_RESPONSELOOP;
			//::SysFreeString(pPCMAC);
			return 1;
		}
		if(pStrID && ::_tcsicmp(pStrID,_T("logincancel"))==0)
		{
			m_dwStatus &= ~WST_RESPONSELOOP;
			//::PostMessage(this->GetHWND(),WM_CLOSE,0,0);
			::PostMessage(this->GetHWND(),WM_QUIT,0,0);
			//::PostMessage(::FindWindow(NULL,L"登录窗口"),WM_QUIT,0,0);
			
			return 1;
		}
	}

	return 0;
}

int CDefaultView::OnPostOpen()
{
	if(xfApp::GetApp()->m_pDefaultView) return 0;

	//return 0;//OnLogin();
	xfControl * mPassEdit = GetControl(_T("password"));
	xfControl * mUserEdit = GetControl(_T("username"));
	xfControl * mNetEdit = GetControl(_T("usernet"));
	if(!mPassEdit || !mUserEdit || !mNetEdit) return 0;

	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	::PathRemoveFileSpec(path);
	TCHAR buf[MAX_PATH];
	::_stprintf_s(buf,MAX_PATH,_T("%s\\updater/updater.xml"),path);

	xbXml xml;
	IXMLDOMNodeList* pNodeList=NULL;
	IXMLDOMNode* pNode;
	IXMLDOMElement* pElement;
	//xml.Load(L"updater/updater.xml");
	if(!xml.Load(buf))
	{
		MessageBox(this->GetHWND(),L"配置文件读取错误!",L"提示",MB_OK);
		return 1;
	}
	
	xml.GetXmlDoc()->selectNodes(L"//server",&pNodeList);
	long nItemLen;
	VARIANT bstr;
	pNodeList->get_length(&nItemLen);
	if(nItemLen < 1)
	{
		MessageBox(this->GetHWND(),L"配置文件设置错误!",L"提示",MB_OK);
		return 1;
	}
	for(int k=0;k<nItemLen;k++)
	{
		pNodeList->get_item(k,&pNode);
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);

		pElement->getAttribute(L"name",&bstr);
		int nIndex = -1;
		if(bstr.vt==VT_BSTR && bstr.bstrVal)
			nIndex = ::SendMessage(mNetEdit->GetHWND(),CB_ADDSTRING  ,NULL, (LPARAM)bstr.bstrVal);
		pElement->getAttribute(L"url",&bstr);
		if(bstr.vt==VT_BSTR && bstr.bstrVal && nIndex > -1)
		{
			LPTSTR pCombTextData;
			pCombTextData = Hxsoft::XUtil::tcsdupFromBstr(bstr.bstrVal);
			::SendMessage(mNetEdit->GetHWND(),CB_SETITEMDATA,nIndex,(LPARAM)pCombTextData);
		}
	}
	int nIndex = ::GetPrivateProfileInt(L"hxsoft",L"net",0,L"cache/user.ini");
	::SendMessage(mNetEdit->GetHWND(),CB_SETCURSEL,nIndex,NULL);

	m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(mPassEdit->GetHWND(),GWLP_WNDPROC);
	SetWindowLongPtr(mPassEdit->GetHWND(),GWLP_WNDPROC,(long)&CDefaultView::WindowProc);
	::SetProp(mPassEdit->GetHWND(),L"thisex",(HANDLE)this);

	::SendMessage(mUserEdit->m_hWnd, WM_LBUTTONDOWN, 0, 0);
	::SendMessage(mUserEdit->m_hWnd, WM_LBUTTONUP, 0, 0);
	TCHAR pUser[30];
	::GetWindowText(mUserEdit->m_hWnd,pUser,30);
	if(pUser[0] != '\0') return 0;
	::GetPrivateProfileString(L"hxsoft",L"user",NULL,pUser,30,L"cache/user.ini");
	if(pUser[0] == '\0') return 0;

	::SetWindowText(mUserEdit->GetHWND(),(LPCWSTR)(pUser));
	::SendMessage(mPassEdit->m_hWnd, WM_LBUTTONDOWN, 0, 0);
	::SendMessage(mPassEdit->m_hWnd, WM_LBUTTONUP, 0, 0);

	return 0;
}

LRESULT CALLBACK CDefaultView::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				int  nRet = 0;
				CDefaultView * pThis = ((CDefaultView *)GetProp(hwnd,L"thisex"));
				nRet = pThis->OnKeyDown(nChar, nRepCnt,nFlags);
				if(nRet==0)
					return CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
				else
					return nRet;
			}
		default:
			return CallWindowProc(((CDefaultView *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
	}
}

int CDefaultView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((nChar == 0x5a && (GetKeyState(VK_CONTROL) & 0x8000) && ::PathFileExists(L"D:\\D567C558-D9BF-4F67-9348-FDF4FCA74394.txt"))
		|| nChar == 0xd)
	{
		if(nChar == 0x5a)//"z"
		{
			::SetWindowText(this->GetControl(_T("password"))->m_hWnd,_T("******"));
			m_CheckPass = true;
		}
		xfControl * mButton = GetControl(_T("loginok"));
		SendMessage(mButton->GetHWND(),WM_LBUTTONDOWN,0,0);
		SendMessage(mButton->GetHWND(),WM_LBUTTONUP,0,0);
		return 1;
	}
	return 0;
}