#include "StdAfx.h"
#include <iostream>
#include "AuthWin.h"
#include "xuxutil.h"


CAuthWin::CAuthWin(void):m_CheckPass(false),m_PrevWndFunc(NULL)
{
}

CAuthWin::~CAuthWin(void)
{
	SetWindowLongPtr(GetHWND(),GWLP_WNDPROC,(long)m_PrevWndFunc);
	::SetProp(GetHWND(),L"thisex",0);
}

int CAuthWin::OnOpen()
{
	xfControl * mPassEdit = GetControl(_T("password"));
	m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(mPassEdit->GetHWND(),GWLP_WNDPROC);
	SetWindowLongPtr(mPassEdit->GetHWND(),GWLP_WNDPROC,(long)&CAuthWin::WindowProc);
	::SetProp(mPassEdit->GetHWND(),L"thisex",(HANDLE)this);

	//::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_SETFOCUS, 0, 0);
	::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_LBUTTONDOWN, 0, 0);
	::SendMessage(this->GetControl(_T("username"))->m_hWnd, WM_LBUTTONUP, 0, 0);
	TCHAR pUser[30];
	::GetWindowText(this->GetControl(_T("username"))->m_hWnd,pUser,30);
	if(pUser[0] != '\0') return 0;
	::GetPrivateProfileString(L"hxsoft",L"user",NULL,pUser,30,L"cache/user.ini");
	if(pUser[0] == '\0') return 0;
	::SetWindowText(this->GetControl(_T("username"))->m_hWnd,(LPCWSTR)(pUser));
	//::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_SETFOCUS, 0, 0);
	::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_LBUTTONDOWN, 0, 0);
	::SendMessage(this->GetControl(_T("password"))->m_hWnd, WM_LBUTTONUP, 0, 0);

	return 0;
}

LRESULT CALLBACK CAuthWin::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				int  nRet = 0;
				CAuthWin * pThis = ((CAuthWin *)GetProp(hwnd,L"thisex"));
				nRet = pThis->OnKeyDown(nChar, nRepCnt,nFlags);
				if(nRet==0)
					return CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
				else
					return nRet;
			}
		default:
			return CallWindowProc(((CAuthWin *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
	}
}

int CAuthWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((nChar == 0x5a && (GetKeyState(VK_CONTROL) & 0x8000) && ::PathFileExists(L"D:\\test.txt"))
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

int CAuthWin::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 0;
	if(::_tcsicmp(pStrID,_T("loginok"))==0)
	{
		LPTSTR AuthInfo = (LPTSTR)this->m_pParam;

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

		LPWSTR pComputer = IXAServer::GetComputerNameEx();
		LPWSTR pPCMAC = IXAServer::GetMacAddress();

		IXAServerArg arg;
		arg.AddArg(L"username",pUser);
		arg.AddArg(L"password",Hxsoft::XUtil::MD5(pPassWord));
		arg.AddArg(L"computer",pComputer);
		arg.AddArg(L"mac",pPCMAC);
		if(m_CheckPass)
			arg.AddArg(L"checkpass",L"N");
		else
			arg.AddArg(L"checkpass",L"Y");

		::_stprintf_s(AuthInfo,1024,_T("%s"),arg.GetString());
		//::_stprintf_s(AuthInfo,255,_T("%s|%s|"),pUser,pPassWord);
		m_dwStatus &= ~WST_RESPONSELOOP;
		::SysFreeString(pPCMAC);
		return 1;
	}
	if(::_tcsicmp(pStrID,_T("logincancel"))==0)
	{
		m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	return 0;
}
