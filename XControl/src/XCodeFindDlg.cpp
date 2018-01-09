#include "StdAfx.h"
#include "..\include\XCodeFindDlg.hpp"
#include "ixsedit.hpp"
#include "scintilla.h"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	XCodeFindDlg::XCodeFindDlg(void)
	{
	}

	XCodeFindDlg::~XCodeFindDlg(void)
	{
	}

	int XCodeFindDlg::OnOpen()
	{
		m_pEdit = (IXSEdit *)m_pParam;
		return 1;
	}

	int XCodeFindDlg::OnXCommand(LPCTSTR	pStrID,xfControl * pControl)
	{
		if(pStrID && _tcsicmp(pStrID,L"xmFind")==0)
		{
			DWORD dwFindFlag = 0;
			bool bReg = false;
			bool bArrowUp = false;
			int ret =::SendMessage(GetControl(L"wholeword")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_WHOLEWORD;
			ret =::SendMessage(GetControl(L"matchcase")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_MATCHCASE;
			ret =::SendMessage(GetControl(L"reg")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)
			{
				dwFindFlag |= SCFIND_REGEXP;
				bReg = true;
			}
			ret =::SendMessage(GetControl(L"posix")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_REGEXP;
			ret =::SendMessage(GetControl(L"up")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED) bArrowUp = true;

			LPTSTR pStr;
			int nLen = ::GetWindowTextLength(GetControl(L"findstr")->GetHWND());
			pStr = new TCHAR[nLen +1];
			::GetWindowText(GetControl(L"findstr")->GetHWND(),pStr,nLen+1);

			m_pEdit->SetSearchflags(dwFindFlag);
			m_pEdit->SetSearchText(pStr);
			if(!bReg)
			{
				if(bArrowUp)
					m_pEdit->m_bSearchFound = m_pEdit->SearchBackward(m_pEdit->m_pSearchText);
				else
					m_pEdit->m_bSearchFound = m_pEdit->SearchForward(m_pEdit->m_pSearchText);
			}else
				m_pEdit->m_bSearchFound = m_pEdit->SearchForward(m_pEdit->m_pSearchText);
			
			return 1;
		}

		if(pStrID && _tcsicmp(pStrID,L"xmReplace")==0)
		{
			LPTSTR pStr;
			int nLen = ::GetWindowTextLength(GetControl(L"replacestr")->GetHWND());
			pStr = new TCHAR[nLen +1];
			::GetWindowText(GetControl(L"replacestr")->GetHWND(),pStr,nLen+1);
			m_pEdit->SetReplaceText(pStr);
			if(m_pEdit->m_bSearchFound)m_pEdit->ReplaceSearchedText(m_pEdit->m_pReplaceText);
			
			DWORD dwFindFlag = 0;
			bool bReg = false;
			bool bArrowUp = false;
			int ret =::SendMessage(GetControl(L"wholeword")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_WHOLEWORD;
			ret =::SendMessage(GetControl(L"matchcase")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_MATCHCASE;
			ret =::SendMessage(GetControl(L"reg")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)
			{
				dwFindFlag |= SCFIND_REGEXP;
				bReg = true;
			}
			ret =::SendMessage(GetControl(L"posix")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_REGEXP;
			ret =::SendMessage(GetControl(L"up")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED) bArrowUp = true;

			nLen = ::GetWindowTextLength(GetControl(L"findstr")->GetHWND());
			pStr = new TCHAR[nLen +1];
			::GetWindowText(GetControl(L"findstr")->GetHWND(),pStr,nLen+1);

			m_pEdit->SetSearchflags(dwFindFlag);
			m_pEdit->SetSearchText(pStr);
			if(!bReg)
			{
				if(bArrowUp)
					m_pEdit->m_bSearchFound = m_pEdit->SearchBackward(m_pEdit->m_pSearchText);
				else
					m_pEdit->m_bSearchFound = m_pEdit->SearchForward(m_pEdit->m_pSearchText);
			}else
				m_pEdit->m_bSearchFound = m_pEdit->SearchForward(m_pEdit->m_pSearchText);
		}

		if(pStrID && _tcsicmp(pStrID,L"xmReplaceAll")==0)
		{
			DWORD dwFindFlag = 0;
			bool bReg = false;
			bool bArrowUp = false;
			int ret =::SendMessage(GetControl(L"wholeword")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_WHOLEWORD;
			ret =::SendMessage(GetControl(L"matchcase")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_MATCHCASE;
			ret =::SendMessage(GetControl(L"reg")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)
			{
				dwFindFlag |= SCFIND_REGEXP;
				bReg = true;
			}
			ret =::SendMessage(GetControl(L"posix")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  dwFindFlag |= SCFIND_REGEXP;
			ret =::SendMessage(GetControl(L"up")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED) bArrowUp = true;

			m_pEdit->SetSearchflags(dwFindFlag);

			LPTSTR pStr;
			int nLen = ::GetWindowTextLength(GetControl(L"findstr")->GetHWND());
			pStr = new TCHAR[nLen +1];
			::GetWindowText(GetControl(L"findstr")->GetHWND(),pStr,nLen+1);
			m_pEdit->SetSearchText(pStr);

			nLen = ::GetWindowTextLength(GetControl(L"replacestr")->GetHWND());
			pStr = new TCHAR[nLen +1];
			::GetWindowText(GetControl(L"replacestr")->GetHWND(),pStr,nLen+1);
			m_pEdit->SetReplaceText(pStr);


			bool bSelection = false;
			ret =::SendMessage(GetControl(L"selection")->GetHWND(),BM_GETCHECK,0,0);
			if(ret==BST_CHECKED)  bSelection = true;
			m_pEdit->ReplaceAll(m_pEdit->m_pSearchText,m_pEdit->m_pReplaceText,bSelection);
			
			return 1;
		}

		if(pStrID && _tcsicmp(pStrID,L"xmMarkAll")==0)
		{
			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,L"xmClose")==0)
		{
			::DestroyWindow(m_hWnd);
			return 1;
		}

		return 0;
	}
}}}
