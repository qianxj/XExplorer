#include "stdafx.h"
#include "editsvr.hpp"
#include "gridframesvr.hpp"
#include "XFlowChart.h"
#include "dwtable.hpp"
#include "xcellsheetsvr.hpp"
#include "HxComplier.h"
#include "XGridComplier.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CCellEditer::~CCellEditer()
	{
		if(::IsWindow(GetHWND()))
		{
			this->DettachProcEdit();
			::SetProp(GetHWND(),L"thisex",0);
		}
	}

	LPTSTR	CCellEditer::GetText()
	{
		if(GetHWND())
		{
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}

	int	CCellEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		//LPTSTR pStr = pStrContent
		if(pStrContent)
		{
			if(wcschr(pStrContent,'\n') && !wcschr(pStrContent,'\r')) 
			{
				wchar_t* pStr = new wchar_t[_tcslen(pStrContent)*4];
				int pos = 0;
				int pos1 = 0;
				while(pStrContent[pos])
				{
					if(pStrContent[pos]=='\n')
					{
						pStr[pos1++] = '\r';
						pStr[pos1++]='\n';
						pos++;
					}else
						pStr[pos1++]=pStrContent[pos++];
				}
				pStr[pos1]=0;
				::SetWindowText(GetHWND(),pStr);
				::SendMessage(GetHWND(),EM_SETSEL,0,_tcslen(pStr));
				delete pStr;
			}else
			{
				::SetWindowText(GetHWND(),pStrContent);
				::SendMessage(GetHWND(),EM_SETSEL,0,_tcslen(pStrContent));
			}
		}
		else
		{
			::SetWindowText(GetHWND(),L"");
		}
		return 1;
	}
	int CCellEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CCellEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	int CCellEditer::AttachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(m_hWnd,GWLP_WNDPROC);
			SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(long)&CCellEditer::WindowProc);
			SetProp(m_hWnd,L"thisex",(HANDLE)this);
		}
		else
			m_PrevWndFunc = 0;
		return 1;
	}
	int CCellEditer::DettachProcEdit()
	{
		if(IsWindow(m_hWnd))SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(long)m_PrevWndFunc);
		return 1;
	}

	BOOL CCellEditer::RightSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0 || EndPos == StartPos ) && EndPos == ::GetWindowTextLength(handle);
	}

	BOOL CCellEditer::LeftSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0) && ((EndPos == 0) || (EndPos == ::GetWindowTextLength(handle)));
	}

	void CCellEditer::SelectAll()
	{

	}
	void CCellEditer::Deselect()
	{
		SendMessage(m_hWnd,EM_SETSEL, 0x7FFFFFFF, long(0xFFFFFFFF));
	}

	int CCellEditer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		int Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
		
		TEvent evt;
		evt.key.nChar = nChar;
		evt.key.nFlags = nFlags;
		evt.key.nRepCnt = nRepCnt;

		switch(nChar)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_ESCAPE:
		case VK_RETURN:
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
		case VK_INSERT:
			if(Shift = 0 )
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			else if(Shift == ssShift && ! (m_pEditSvr->m_pGridSvr->CanEditModify())) nChar = 0;
			break;
		case VK_LEFT:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || LeftSide(m_hWnd )))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_RIGHT:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || RightSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_HOME:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || LeftSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_END: 
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || RightSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_F2:
			{
				//ParentEvent;
				if(nChar == VK_F2)
				{
					Deselect();
					nChar = 0;
				}
			}
		case VK_TAB:
			if(!(ssAlt & Shift))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		}
		if((nChar == VK_DELETE) && ! m_pEditSvr->m_pGridSvr->CanEditModify()) nChar = 0;
		return nChar? 0:1;
	}

	int CCellEditer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = ::GetParent(GetHWND());
		hdr.idFrom = nChar;
		hdr.code = DWV_KEYDOWN;
		//hdr.DwTable = this->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		//hdr.oldrow = -1;
		//hdr.row = -1;
		hdr.data = GetText();
		::SendMessage(::GetParent(::GetParent(GetHWND())),WM_NOTIFY,0,(LPARAM)&hdr);

		return 0;
	}
	int CCellEditer::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		int Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;

		int    nStartChar, nEndChar;
		WORD   StartPos = 0;
		
		UINT	 Key;
		Key = nChar;
		if( Key>=32 && Key <=255 && !m_pEditSvr->m_pGridSvr->CanEditAcceptKey(Key))
		{
			Key = 0;
			MessageBeep(0);
		}

		switch(Key)
		{
		case VK_TAB:
		case VK_ESCAPE:
			Key = 0;
			break;
		case VK_RETURN:
			{
				if(Shift & ssAlt)
					break;
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				if(nStartChar == 0 && nEndChar == GetWindowTextLength(m_hWnd))
					Deselect(); else
				{
					SelectAll();
				}
				Key = 0;
				break;
			}
		default:
			if(! m_pEditSvr->m_pGridSvr->CanEditModify()) Key = 0;
		}
		return Key? 0:1;
	}

	LRESULT CALLBACK CCellEditer::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				int  nRet = 0;
				CCellEditer * pThis = ((CCellEditer *)GetProp(hwnd,L"thisex"));
				if(uMsg== WM_KEYDOWN) nRet = pThis->OnKeyDown(nChar, nRepCnt,nFlags);
				if(uMsg== WM_KEYUP) nRet = pThis->OnKeyUp(nChar, nRepCnt,nFlags);
				if(uMsg== WM_CHAR) nRet = pThis->OnChar(nChar, nRepCnt,nFlags);
				if(nRet==0)
				{
					nRet = CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
					if(uMsg== WM_CHAR && pThis->m_pEditSvr->TipHWMD && ::IsWindow(pThis->m_pEditSvr->TipHWMD))
					{
						pThis->m_pEditSvr->ShowTip(pThis->m_pEditSvr->m_ARow, pThis->m_pEditSvr->m_ACol, false);
					}
				}
				return nRet;
			}
		case WM_KILLFOCUS:
			{
				CCellEditer * pThis = ((CCellEditer *)GetProp(hwnd,L"thisex"));
				pThis->m_pEditSvr->HiddenEditor();
			}
		default:
			return CallWindowProc(((CCellEditer *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
		}
	}



	CControlEditer::CControlEditer():m_pxfControl(NULL)
	{
	}

	int CControlEditer::InstanceControl(HWND hParent,LPTSTR xClass)
	{
		m_szCotrolClass = ::_tcsdup(xClass);

		xbObject *	pObject;
		xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,m_szCotrolClass,pObject);
		m_pxfControl = (xfControl *)pObject;
		if(m_pxfControl)
		{
			RECT rect;
			::SetRect(&rect,0,0,0,0);
			m_pxfControl->m_pxfNode= NULL;
			m_pxfControl->m_dwStyle &= ~WS_VISIBLE;
			m_pxfControl->CreateControl(NULL,rect,hParent,0,xfApp::GetApp()->GetInstance());
 			::SetProp(m_pxfControl->GetHWND(),_T("this"),m_pxfControl);
			m_pxfControl->Initial();
			this->m_hWnd = m_pxfControl->GetHWND();
		}
		
		return 1;
	}

	CControlEditer::~CControlEditer()
	{
	}

	LPTSTR	CControlEditer::GetText()
	{
		if(GetHWND())
		{
			LPTSTR cName = new TCHAR[255];
			::GetClassName(GetHWND(), cName, 255);
			if(_tcscmp(cName, _T("SysDateTimePick32")) == 0)
			{
				SYSTEMTIME sysTime;  
				memset(&sysTime,0,sizeof(sysTime));   
				int ret = ::SendMessage(GetHWND(), DTM_GETSYSTEMTIME, 0, (LPARAM)&sysTime);
				if(ret !=  GDT_VALID)
					return NULL;
				LPTSTR pStr = new TCHAR[11];
				_stprintf_s(pStr,11, L"%d-%d-%d", sysTime.wYear,sysTime.wMonth,sysTime.wDay);
				return pStr;
			}
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}

	int	CControlEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		LPTSTR cName = new TCHAR[255];
		::GetClassName(GetHWND(), cName, 255);
		if(_tcscmp(cName, _T("SysDateTimePick32")) == 0)
		{
			int wYear,wMonth,wDay;
			if(pStrContent && _tcscmp(pStrContent, L"") != 0)
			{
				swscanf_s(pStrContent, L"%d-%d-%d ", &wYear,&wMonth,&wDay);
				SYSTEMTIME sysTime;  
				memset(&sysTime,0,sizeof(sysTime));   
				sysTime.wYear = wYear;
				sysTime.wMonth = wMonth; 
				sysTime.wDay = wDay;
				::SendMessage(GetHWND(), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&sysTime);
			}
			else
			{
				::SetWindowText(GetHWND(),NULL);
				::SendMessage(GetHWND(), DTM_SETSYSTEMTIME, GDT_NONE, NULL);
			}
			return 1;
		}

		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CControlEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CControlEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	LPTSTR	CCellCheckBoxEditer::GetText()
	{
		if(GetHWND())
		{
			int r = ::SendMessage(GetHWND(), BM_GETSTATE, 0, 0);
			if(r == 8 || r == 0) return _T("0");
			else return _T("1");
		}
		else
			return NULL;
	}
	int	CCellCheckBoxEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	LPTSTR	CCellCheckBoxEditer::GetTextData()
	{
		return NULL;
	}

	int CCellCheckBoxEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}

	int CCellCheckBoxEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}
	CCellComboBoxEditer::~CCellComboBoxEditer()
	{
		this->DettachProcEdit();
	}

	LPTSTR	CCellComboBoxEditer::GetText()
	{
		if(GetHWND())
		{
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}
	int	CCellComboBoxEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		
		//::SendMessage(GetHWND(),CB_RESETCONTENT ,0,0);  
		//::SendMessage(GetHWND(),CB_ADDSTRING ,0,(LPARAM)pStrContent);  
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	LPTSTR	CCellComboBoxEditer::GetTextData()
	{
		if(GetHWND())
		{
			int i;
			i = SendMessage(GetHWND(), CB_GETCURSEL, NULL, NULL);
			if(CB_ERR==(i = SendMessage(GetHWND(), CB_GETCURSEL, NULL, NULL)))
				return NULL;
			LRESULT strList = ::SendMessage(GetHWND(), CB_GETITEMDATA, i, NULL);
			LPTSTR pStrText = (LPTSTR)strList;
			return pStrText;
		}
		else
			return NULL;
	}

	int CCellComboBoxEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			RECT	rc;
			int height;
			int	itemHeight;

			::GetWindowRect(GetHWND(),&rc);
			itemHeight = (int)::SendMessage(GetHWND(),CB_GETITEMHEIGHT,0,0);
			height = rc.bottom - rc.top + 1 + itemHeight * 30;
			height = height > cy?height:cy;
			::MoveWindow(GetHWND(),x,y,cx,height,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}

	int CCellComboBoxEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	int CCellComboBoxEditer::AttachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			HWND hwnd = ::FindWindowEx(m_hWnd, NULL,NULL,NULL);
			if(hwnd)
			{
				m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(hwnd,GWLP_WNDPROC);
				SetWindowLongPtr(hwnd,GWLP_WNDPROC,(long)&CCellComboBoxEditer::WindowProc);
				SetProp(hwnd,L"thisex",(HANDLE)this);
			}
		}
		else
			m_PrevWndFunc = 0;
		return 1;
	}
	int CCellComboBoxEditer::DettachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			HWND hwnd = ::FindWindowEx(m_hWnd, NULL,NULL,NULL);
			if(hwnd)
			{
				SetWindowLongPtr(hwnd,GWLP_WNDPROC,(long)m_PrevWndFunc);
				::SetProp(hwnd,L"thisex",0);
			}
		}
		return 1;
	}
	int CCellComboBoxEditer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		TEvent evt;
		evt.key.nChar = nChar;
		evt.key.nFlags = nFlags;
		evt.key.nRepCnt = nRepCnt;

		switch(nChar)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_ESCAPE:
		case VK_RETURN:
		case VK_TAB:
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		}
		if((nChar == VK_DELETE) && ! m_pEditSvr->m_pGridSvr->CanEditModify()) nChar = 0;
		return nChar? 0:1;
	}

	LRESULT CALLBACK CCellComboBoxEditer::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				int  nRet = 0;
				CCellComboBoxEditer * pThis = ((CCellComboBoxEditer *)GetProp(hwnd,L"thisex"));
				nRet = pThis->OnKeyDown(nChar, nRepCnt,nFlags);
				if(nRet==0)
					return CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
				else
					return nRet;
			}
		case WM_KILLFOCUS:
			{
				CCellEditer * pThis = ((CCellEditer *)GetProp(hwnd,L"thisex"));
				pThis->m_pEditSvr->HiddenEditor();
			}
		default:
			return CallWindowProc(((CCellComboBoxEditer *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
		}
	}

	LPTSTR	CCellListBoxEditer::GetText()
	{
		if(GetHWND())
		{
			int s = SendMessage(GetHWND(), LB_GETCOUNT, NULL, NULL);
			int i;
			wstring pStrTexts;
			for(i=0;i<s;i++)
			{
				int k = SendMessage(GetHWND(), LB_GETSEL, i, NULL);
				if(k>0)
				{
					int len = ::SendMessage(GetHWND(), LB_GETTEXTLEN, i, NULL);
					if(len < 1) return NULL;
					LPTSTR  pStrText = new TCHAR[len + 1];
					::SendMessage(GetHWND(), LB_GETTEXT, i, (LPARAM)pStrText);
					pStrTexts += pStrText;
					pStrTexts += _T(";");
				}
			}

			#ifdef _UNICODE
			  size_t const size = pStrTexts.size()+1;
			  TCHAR * const tstr = new TCHAR[size];
			  _tcscpy_s(tstr, size, pStrTexts.c_str());
			#else
			  size_t const size = (pStrTexts.size()+1) * sizeof(wchar_t);
			  TCHAR * const tstr = new TCHAR[size];
			 #ifdef _MBCS
			  size_t retval = 0;
			  errno_t const status = wcstombs_s(&retval, tstr, size, pStrTexts.c_str(), size);
			  assert(status == 0);
			  assert(retval != -1);
			 #else
			  int const status = sprintf_s(tstr, size, "%S", pStrTexts.c_str()); 
			  assert(status != -1);
			 #endif
			#endif

			LPTSTR pStr=tstr;
			return pStr;
		}
		else
			return NULL;
	}
	int	CCellListBoxEditer::SetText(LPTSTR pStrContent)
	{
		//if(!GetHWND()) return 0;
		
		//::SendMessage(GetHWND(),CB_RESETCONTENT ,0,0);  
		//::SendMessage(GetHWND(),CB_ADDSTRING ,0,(LPARAM)pStrContent);  
		//::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	LPTSTR	CCellListBoxEditer::GetTextData()
	{
		if(GetHWND())
		{
			int s = SendMessage(GetHWND(), LB_GETCOUNT, NULL, NULL);
			int i;
			//LPTSTR pStrTexts=_T("");
			wstring pStrTexts;
			for(i=0;i<s;i++)
			{
				int k = SendMessage(GetHWND(), LB_GETSEL, i, NULL);
				if(k>0)
				{
					LRESULT strList = ::SendMessage(GetHWND(), LB_GETITEMDATA, i, NULL);
					LPTSTR pStrText = (LPTSTR)strList;
					pStrTexts += pStrText;
					pStrTexts += _T(";");
				}
			}

			#ifdef _UNICODE
			  size_t const size = pStrTexts.size()+1;
			  TCHAR * const tstr = new TCHAR[size];
			  _tcscpy_s(tstr, size, pStrTexts.c_str());
			#else
			  size_t const size = (pStrTexts.size()+1) * sizeof(wchar_t);
			  TCHAR * const tstr = new TCHAR[size];
			 #ifdef _MBCS
			  size_t retval = 0;
			  errno_t const status = wcstombs_s(&retval, tstr, size, pStrTexts.c_str(), size);
			  assert(status == 0);
			  assert(retval != -1);
			 #else
			  int const status = sprintf_s(tstr, size, "%S", pStrTexts.c_str()); 
			  assert(status != -1);
			 #endif
			#endif

			LPTSTR pStrT=tstr;
			return pStrT;
		}
		else
			return NULL;
	}

	int CCellListBoxEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			RECT	rc;
			::GetClientRect(::GetParent(GetHWND()),&rc);
			
			int height = 100;
			int yy = y + cy + 3;
			if(rc.bottom-15-cy-y<height)
			{
				if(rc.top<height)
				{
					height = (rc.bottom-15-cy-y)>(y-5)?rc.bottom-15-cy-y:y-5;
					if(rc.bottom-15-cy-y<y-5)
						yy=3;
				}
				else
				{
					yy=y-height-5;
				}
			}

			::MoveWindow(GetHWND(),x,yy,cx,height,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}

	int CCellListBoxEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	CEditSvr::CEditSvr(void):m_pGridSvr(NULL),m_pEdit(NULL),m_ACol(NULL),m_ARow(NULL)
	{
	}
	CEditSvr::~CEditSvr(void)
	{
		for(int i=0;i<m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]) delete m_EditBuffer[i];
		}
		m_EditBuffer.clear();
	}

	int CEditSvr::MoveLoc(int x,int y,int cx,int cy,int nCmdShow )
	{
		if(m_pEdit) 
			return m_pEdit->MoveLoc(x,y,cx,cy,nCmdShow);
		return 1;
	}
	int CEditSvr::UpdateEditContent(int ACol,int ARow)
	{
		if(! m_pGridSvr) return 0;
		//LPTSTR	pStrText = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemString(ACol,ARow); 
		LPTSTR	pStrText = (LPTSTR)m_pGridSvr->GetItemString(ARow, ACol);
		bool bDelete = false;
		//format
		{
			wchar_t* pDataType = 0;
			wchar_t* pEditFormat = 0;
			_variant_t var;
			xbXml xml;
			IXMLDOMElement * pElement = NULL;
			IXMLDOMNode * pNode;
			PStrItem pItem = (PStrItem)m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
			if(pItem && pItem->pStrXml)
			{
				xml.LoadXml(pItem->pStrXml);
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);

				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"celltype",&var);
				if(var.bstrVal)
					pDataType = _tcsdup(var.bstrVal);
				pElement->getAttribute(L"cellformat",&var);
				if(var.bstrVal)
					pEditFormat = _tcsdup(var.bstrVal);	
				pNode->Release();
				pElement->Release();

				if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0)
				{
					if(pEditFormat && pEditFormat[0])
					{
						pStrText = XUtil::FormatDecimalString((LPTSTR)pStrText,pEditFormat,bDelete);
						int ret = m_pEdit->SetText((LPTSTR)pStrText);
						if(bDelete) delete pStrText;
						return ret;
					}
				}
				if(pEditFormat) delete pEditFormat;
				if(pDataType) delete pDataType;
			}

		}

		if(m_pEdit)
		{
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("button"))==0)
			{
				_variant_t var;
				xbXml xml;
				PStrItem pItem =  m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					pElement->getAttribute(L"edittype",&var);
					if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("checkbox"))==0)
					{
						if(pItem->pStrText)
							::SetWindowText(m_pEdit->GetHWND(),pItem->pStrText);
						else
							::SetWindowText(m_pEdit->GetHWND(),_T(""));
						pElement->getAttribute(L"editchecked",&var);
						if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,pStrText)==0)
							SendMessage(m_pEdit->GetHWND(),BM_SETCHECK,BST_CHECKED,0);
						pElement->Release();
						return 1;
					}
				}
			}
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("combobox"))==0)
			{
				::SendMessage(m_pEdit->GetHWND(),CB_RESETCONTENT ,0,0); 
				_variant_t var;

				LPTSTR pEditType = NULL;
				PStrItem pItem =  m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
				if(pItem)
				{
					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					IXMLDOMElement * pElement = NULL;
					IXMLDOMNode * pNode;
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)pElement->getAttribute(L"ddlbcandedit",&var);
					if(var.bstrVal && _tcscmp(var.bstrVal, _T("Y"))==0)
					{
						::SendMessage(::GetWindow(m_pEdit->GetHWND(), GW_CHILD), EM_SETREADONLY, 0, 0);
					}
					else
					{
						::SendMessage(::GetWindow(m_pEdit->GetHWND(), GW_CHILD), EM_SETREADONLY, 1, 0);
					}
					xbXml ddlb;
					if(pElement)pElement->getAttribute(L"ddlbdataurl",&var);
					if(var.bstrVal && _tcscmp(var.bstrVal, _T(""))!=0)
					{
						//IXAServer::ExecXQuery(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,var.bstrVal,L"<args/>",ddlb);
						LPTSTR pItemStr = var.bstrVal;
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this->m_pGridSvr, 1, ARow, ACol))
								pItemStr = x.GetString();							
						}
						
						TCHAR buf[512];
						if(_tcsstr(pItemStr,_T("xquery:"))==pItemStr)
							::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pItemStr);
						else
							::_stprintf_s(buf,512,_T("%s"),pItemStr);
						TCHAR WKURL[512];
						::_stprintf_s(WKURL,512,_T("<args><arg name='__wkurl' value='%s'/></args>"),pItemStr);
						if(xfUriLoader::FetchXml(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,buf,WKURL,ddlb)!=1) return -1;
					}else
					{
						if(pElement)pElement->getAttribute(L"ddlbxml",&var);
						if(var.bstrVal)ddlb.LoadXml(var.bstrVal);
					}
					if(ddlb.GetXmlDoc())
					{
						IXMLDOMNodeList *pList;
						IXMLDOMNode *pNodeEx;
						IXMLDOMNodeList *pList1;
						IXMLDOMNode *pNodeEx1;
						long nLen;
						//ddlb.m_pXmlDoc->selectNodes(L"//row/显示列",&pList);
						//ddlb.m_pXmlDoc->selectNodes(L"//row/数据列",&pList1);
						ddlb.m_pXmlDoc->selectNodes(L"/node()/node()/node()[position()=1]",&pList);
						ddlb.m_pXmlDoc->selectNodes(L"/node()/node()/node()[position()=2]",&pList1);
						pList->get_length(&nLen);
						int nIndex = -1;
						for(int i=0;i<nLen;i++)
						{
							pList->get_item(i,&pNodeEx);
							pList1->get_item(i,&pNodeEx1);
							BSTR bstr;
							LPTSTR bstr1;
							if(pNodeEx)
								pNodeEx->get_text(&bstr);
							else
								bstr = L"";
							if(pNodeEx1)
								pNodeEx1->get_text(&bstr1);
							else
								bstr1 = L"";
							if(bstr)
							{
								::SendMessage(m_pEdit->GetHWND(),CB_ADDSTRING,0,(LPARAM)bstr);
								::SysFreeString(bstr);
							}
							if(bstr1)
							{
								::SendMessage(m_pEdit->GetHWND(),CB_SETITEMDATA,i,(LPARAM)bstr1);
							}
							if(pStrText && bstr1 && (_tcscmp(pStrText, bstr1) == 0 || _tcscmp(pStrText, bstr) == 0))
								nIndex = i;
							pNodeEx->Release();
						}
						pList->Release();
						pElement->Release();
						if(nIndex > -1)
							::SendMessage(m_pEdit->GetHWND(),CB_SETCURSEL,nIndex,0);
						else
							::SetWindowText(m_pEdit->GetHWND(),pStrText);
					}
				}
				return 0;
				//pStrText = (LPTSTR)m_pGridSvr->GetItemDisplayString(ARow, ACol);
			}
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("listbox"))==0)
			{
				::SendMessage(m_pEdit->GetHWND(),LB_RESETCONTENT ,0,0); 
				_variant_t var;

				LPTSTR pEditType = NULL;
				PStrItem pItem =  m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
				if(pItem)
				{
					vector <wstring> v;
					if(pItem->pStrContent)
					{
						wstring tmp=pItem->pStrContent;
						int p = tmp.find(_T(";"),0);
						if(p==-1)
							v.push_back(tmp);
						while(p>-1)
						{
							v.push_back(tmp.substr(0,p));
							tmp = tmp.substr(p + 1, tmp.length());
							p = tmp.find(_T(";"),0);
						}
					}
					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					IXMLDOMElement * pElement = NULL;
					IXMLDOMNode * pNode;
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)pElement->getAttribute(L"listboxxml",&var);
					if(var.bstrVal)
					{
						xbXml ddlb;
						ddlb.LoadXml(var.bstrVal);
						IXMLDOMNodeList *pList;
						IXMLDOMNode *pNodeEx;
						IXMLDOMNodeList *pList1;
						IXMLDOMNode *pNodeEx1;
						long nLen;
						ddlb.m_pXmlDoc->selectNodes(L"//row/显示列",&pList);
						ddlb.m_pXmlDoc->selectNodes(L"//row/数据列",&pList1);
						pList->get_length(&nLen);
						for(int i=0;i<nLen;i++)
						{
							pList->get_item(i,&pNodeEx);
							pList1->get_item(i,&pNodeEx1);
							BSTR bstr;
							LPTSTR bstr1;
							pNodeEx->get_text(&bstr);
							pNodeEx1->get_text(&bstr1);
							if(bstr)
							{
								::SendMessage(m_pEdit->GetHWND(),LB_ADDSTRING,0,(LPARAM)bstr);
								::SysFreeString(bstr);
							}
							if(bstr1)
							{
								::SendMessage(m_pEdit->GetHWND(),LB_SETITEMDATA,i,(LPARAM)bstr1);
								int j;
								for(j=0;j<(int)v.size();j++)
								{
									if(v[j]==bstr1)
									{
										::SendMessage(m_pEdit->GetHWND(),LB_SETCURSEL,i,NULL);
										::SendMessage(m_pEdit->GetHWND(),LB_SETSEL,(WPARAM)true,i);
										break;
									}
								}
							}
							pNodeEx->Release();
						}
						pList->Release();
						pElement->Release();
					}
				}
			}
			//if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("edit"))==0)
			/*else
			{
				::SendMessage(m_pEdit->GetHWND(),EM_SETREADONLY,false,0);
			}*/
			bool bShowButton;
			bool bQuery = this->m_pGridSvr->IsEditQuery(ACol,ARow,bShowButton);
			if(bQuery && m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("edit"))==0)
			{
				pStrText = (LPTSTR)m_pGridSvr->GetItemDisplayString(ARow, ACol);
				/*PStrItem pItem = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
				//if(pItem && pItem->pStrText) pStrText=pItem->pStrText;

				if(pItem && pItem->pStrXml)
				{
					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					IXMLDOMElement * pElement = NULL;
					IXMLDOMNode * pNode;
					_variant_t var;
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)
					{
						pElement->getAttribute(L"dddwcanedit",&var);
						if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0)
							::SendMessage(m_pEdit->GetHWND(),EM_SETREADONLY,false,0);
						else
							::SendMessage(m_pEdit->GetHWND(),EM_SETREADONLY,true,0);
						pElement->Release();
					}
				}*/
			}else
			{
				if((LPTSTR)m_pGridSvr->GetItemDisplayString(ARow, ACol))
					pStrText = (LPTSTR)m_pGridSvr->GetItemDisplayString(ARow, ACol);
			}
			//if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("combobox"))!=0)
			return m_pEdit->SetText(pStrText);
		}
		return 0;
	}
	int CEditSvr::UpdateCellContent(int ACol,int ARow)
	{
		if(! m_pGridSvr) return 0;
		if(!m_pEdit) return 0;
		LPTSTR pStrText = m_pEdit->GetText();
		LPTSTR pStrText1 = m_pEdit->GetTextData();
		if(this->m_pGridSvr->FOptions & goRunState)
		{ 
			bool bQueryEdit = false;
			PStrItem pItem = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				_variant_t var;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					pElement->getAttribute(L"edittype",&var);
					if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("checkbox"))==0)
					{
						if(_tcsicmp(pStrText,_T("1"))==0 ||_tcsicmp(pStrText,_T("Y"))==0)
						{
							pElement->getAttribute(L"editchecked",&var);
							pStrText1 = var.bstrVal;
						}
						else
						{
							pElement->getAttribute(L"uneditchecked",&var);
							pStrText1 = var.bstrVal;					
						}
						pStrText = NULL;
						if(pItem->pStrText)
							pStrText = pItem->pStrText;
						else
							pStrText = _T("");
					}
					else if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("ddlb"))==0)
					{
						if(!pStrText1 && pStrText)
						{
							int pIndex = ::SendMessage(m_pEdit->m_hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)pStrText);
							if(pIndex<0) pStrText1=pStrText;
							else
							{
								LRESULT strList = ::SendMessage(m_pEdit->m_hWnd, CB_GETITEMDATA, pIndex, NULL);
								pStrText1 = (LPTSTR)strList;
								
							}
						}
					}
					else if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("query"))==0)
					{
						pElement->getAttribute(L"dddwcanedit",&var);
						if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0)
							bQueryEdit = true;
					}
					pElement->Release();
				}
				//checkType
				//输入的数据 不包含选择的数据
				if(pStrText && !pStrText1)
				{
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)
					{
						bool bFormatError = false;
						int nErrorId;
						LPWSTR pNextStr;
						pElement->getAttribute(L"celltype",&var);
						if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:integer"))==0)
						{
							::wcstol(pStrText,&pNextStr,10);
							if(pNextStr && pNextStr[0]!='\0')
							{
								bFormatError=true;
								nErrorId = -1000;
							}
						}
						else if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:decimal")) == 0)
						{
							::wcstod(pStrText,&pNextStr);
							if(pNextStr && pNextStr[0]!='\0')
							{
								bFormatError=true;
								nErrorId = -1001;
							}
						}
						else if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:date")) == 0)
						{
							char * pStrText_1 = XUtil::strdupFromBstr(pStrText);
							int nYear,nMonth,nDay;
							int ndRet=sscanf(pStrText_1,"%d-%d-%d",&nYear,&nMonth,&nDay); 
							if(ndRet != 3) ndRet=sscanf(pStrText_1,"%d/%d/%d",&nYear,&nMonth,&nDay); 
							if(ndRet == 3)
							{
								if(nMonth<1 || nMonth>13) bFormatError=true;
								else if(nDay<1 || nDay>31) bFormatError=true;
								else if((nMonth==4||nMonth==6||nMonth==9||nMonth==11) && nDay>30) bFormatError=true;
								else if(nMonth==2){
									if(nDay>29) bFormatError=true;
									else if(((nYear%100==0 && nYear%400!=0)||nYear%4!=0) && nDay>28) bFormatError=true;
								}
							}
							else
								bFormatError=true;
							nErrorId = -1002;
						}
						if(bFormatError)
						{
							DWNMHDR hdr;
							memset(&hdr,0,sizeof(DWNMHDR));
							hdr.hwndFrom = m_pGridSvr->GetHWND();
							hdr.idFrom = nErrorId;
							hdr.code = DWV_ITEMERROR;
							hdr.data = pStrText;
							hdr.col = ACol;
							hdr.row = ARow;
							if(pItem->pStrColumn)
								hdr.colname = pItem->pStrColumn;
							::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							if(hdr.idFrom == 0) return -1;
						}
					}
				}
			}

/*			LPTSTR pStrText1;
			if(pItem && pItem->pStrText && pItem->pStrContent)
				pStrText1 = m_pEdit->GetTextData(pItem->pStrText);
			else
				pStrText1 = m_pEdit->GetTextData(NULL);
			LPTSTR pStrT=NULL;
			if(pItem && pItem->pStrText)
			{
				pStrT = _tcsdup(pItem->pStrText);
				delete pItem->pStrText;
			}
			if(!pStrText && pStrText1) pStrText=pStrText1;

*/			if(pStrText1)
			{
				CDwTable * pDwTable = m_pGridSvr->GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
				if(pDwTable)
				{
					int nRow,nCol;
					pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
					LPTSTR pOldValue = pDwTable->getDwItemText(nRow,nCol);
					if(!pOldValue || !pStrText1 ||  _tcsicmp(pOldValue,pStrText1)!=0)
					{
						if(pDwTable->event_ItemChanging(nRow,nCol,pStrText1)<0) return -1;
						pDwTable->setDwItemText(nRow,nCol,pStrText1);
						pDwTable->setDwItemDisplayText(nRow,nCol,pStrText);
						pDwTable->event_ItemChanged(nRow,nCol,pStrText1,pStrText);

						CELLNMHDR hdr;
						memset(&hdr,0,sizeof(CELLNMHDR));
						hdr.hwndFrom = m_pGridSvr->GetHWND();
						hdr.idFrom = 0;
						hdr.code = CELL_ITEMCHANGED;
						hdr.currentrow = m_pGridSvr->FCurrent.y;
						hdr.currentcol = m_pGridSvr->FCurrent.x;
						hdr.anchorrow  = m_pGridSvr->FAnchor.y;
						hdr.anchorcol  = m_pGridSvr->FAnchor.x;
						hdr.data = pStrText1;
						::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
					}
				}
/*				CDwTable * pDwTable = m_pGridSvr->GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
				if(!pStrText1 || !pStrT ||  _tcsicmp(pStrT,pStrText1)!=0)
				{
					if(pDwTable)
					{
						int nRow,nCol;
						pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
						pDwTable->SetItemModified(nRow,nCol);
					}
				}
				if(pStrText1 && pItem)
					pItem->pStrText =  _tcsdup(pStrText1);

				if(pDwTable)
				{
					int nRow,nCol;
					pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
					LPTSTR pOldValue = pDwTable->getDwItemText(nRow,nCol);
					if(!pOldValue || !pStrText ||  _tcsicmp(pOldValue,pStrText)!=0)
					{
						if(pDwTable->event_ItemChanging(nRow,nCol,pStrText)<0) return -1;
						pDwTable->setDwItemText(nRow,nCol,pStrText);
						pDwTable->event_ItemChanged(nRow,nCol,pStrText);

						CELLNMHDR hdr;
						memset(&hdr,0,sizeof(CELLNMHDR));
						hdr.hwndFrom = m_pGridSvr->GetHWND();
						hdr.idFrom = 0;
						hdr.code = CELL_ITEMCHANGED;
						hdr.currentrow = m_pGridSvr->FCurrent.y;
						hdr.currentcol = m_pGridSvr->FCurrent.x;
						hdr.anchorrow  = m_pGridSvr->FAnchor.y;
						hdr.anchorcol  = m_pGridSvr->FAnchor.x;
						hdr.data = pStrText;
						::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
					}
				}
*/			}
			else
			{
				CDwTable * pDwTable = m_pGridSvr->GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
				if(pDwTable)
				{
					int nRow,nCol;
					pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
					bool bShowButton;
					bool bQuery = this->m_pGridSvr->IsEditQuery(ACol,ARow,bShowButton);
					LPTSTR pOldValue = pDwTable->getDwItemText(nRow,nCol);
					if(!pOldValue || !pStrText ||  _tcsicmp(pOldValue,pStrText)!=0)
					{
						if(pDwTable->event_ItemChanging(nRow,nCol,pStrText)<0) return -1;
						if(bQuery && (!pStrText || ::_tcscmp(pStrText,L"") == 0))
						{
							//this->m_pGridSvr->ShowQueryDlg();
							pDwTable->setDwItemText(nRow,nCol,L"");
							pDwTable->setDwItemDisplayText(nRow,nCol,_T(""));
							pDwTable->event_ItemChanged(nRow,nCol,_T(""),_T(""));
						}
						else if(bQuery && bQueryEdit)
						{
							LPTSTR pDisplayText = pDwTable->getDwItemDisplayText(nRow,nCol);
							if((!pDisplayText && pStrText) || (pDisplayText && pStrText && _tcsicmp(pDisplayText,pStrText)!=0))
							{
								pDwTable->setDwItemText(nRow,nCol,pStrText);
								pDwTable->setDwItemDisplayText(nRow,nCol,pStrText);
								pDwTable->event_ItemChanged(nRow,nCol,pStrText,pStrText);
							}
						}
						else if(bQuery)
						{
							//
						}
						else
						{
							LPTSTR pStr2 = pDwTable->getDwItemDisplayText(nRow,nCol);
							if(!pStr2) pStr2 = pDwTable->getDwItemText(nRow,nCol);
							LPTSTR pStr11 = pStrText?_tcsdup(pStrText):NULL;
							LPTSTR pStr22 = pStr2?_tcsdup(pStr2):NULL;
							if((pStr2 && pStrText && _tcscmp(::_tcsupr(pStr11),_tcsupr(pStr22)) != 0) || (!pStr2 && pStrText) || (pStr2 && !pStrText))
							{ 
								if(!pStrText) pStrText=_T("");
								pDwTable->setDwItemText(nRow,nCol,pStrText);
								if((pStrText || _tcsicmp(pStrText,_T(""))==0) && pDwTable->getDwItemDisplayText(nRow,nCol))
								{
									pDwTable->setDwItemDisplayText(nRow,nCol,_T(""));
									pDwTable->event_ItemChanged(nRow,nCol,pStrText,_T(""));
								}
								else
									pDwTable->event_ItemChanged(nRow,nCol,pStrText);


								CELLNMHDR hdr;
								memset(&hdr,0,sizeof(CELLNMHDR));
								hdr.hwndFrom = m_pGridSvr->GetHWND();
								hdr.idFrom = 0;
								hdr.code = CELL_ITEMCHANGED;
								hdr.currentrow = m_pGridSvr->FCurrent.y;
								hdr.currentcol = m_pGridSvr->FCurrent.x;
								hdr.anchorrow  = m_pGridSvr->FAnchor.y;
								hdr.anchorcol  = m_pGridSvr->FAnchor.x;
								hdr.data = pStrText;
								::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							}
							if(pStr11) delete pStr11;
							if(pStr22) delete pStr22;
						}
					}
					else if(bQuery)
					{
						::InvalidateRect(m_pGridSvr->GetHWND(),NULL,false);
					}
				}
			}
		}else
		{
			this->m_pGridSvr->GetGridDataSvr()->m_pContentSvr->SetItemString(ACol,ARow,pStrText);
			
			CELLNMHDR hdr;
			memset(&hdr,0,sizeof(CELLNMHDR));
			hdr.hwndFrom = m_pGridSvr->GetHWND();
			hdr.idFrom = 0;
			hdr.code = CELL_ITEMCHANGED;
			hdr.currentrow = m_pGridSvr->FCurrent.y;
			hdr.currentcol = m_pGridSvr->FCurrent.x;
			hdr.anchorrow  = m_pGridSvr->FAnchor.y;
			hdr.anchorcol  = m_pGridSvr->FAnchor.x;
			hdr.data = pStrText;
			::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		}

		//if(pStrText) delete pStrText;
		//if(pStrText1) delete pStrText1;

		return 1;
	}
	LPTSTR CEditSvr::GetEditContent()
	{
		if(m_pEdit)
		{
			LPTSTR	pStrText = NULL;
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("checkbox"))==0)
			{
				_variant_t var;
				xbXml xml;
				PStrItem pItem =  m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(m_ACol,m_ARow );
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					if(SendMessage(m_pEdit->GetHWND(),BM_GETCHECK,0,0)==BST_CHECKED)
					{
						pElement->getAttribute(L"editchecked",&var);
						pElement->Release();
						return _tcsdup(var.bstrVal);
					}else
					{
						pElement->getAttribute(L"uneditchecked",&var);
						pElement->Release();
						return _tcsdup(var.bstrVal);
					}
				}
			}
			pStrText = m_pEdit->GetText();
			return pStrText;
		}
		return NULL;
	}
	CInplaceEdit * CEditSvr::GetEditCtl(int ACol,int ARow)
	{
		CInplaceEdit::eEditType	editType;
		LPTSTR pXClass = NULL;

#if 0		
		if(ACol==1 && ARow==1)
		{
			editType=CInplaceEdit::_combobox;
		}else if(ACol==2 && ARow==2)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xoffice";
		}else if(ACol==2 && ARow==3)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xflowchart";
		}else if(ACol==3 && ARow==3)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xcell";
		}else if(ACol==2 && ARow==4)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xwordsheet";
		}else
		{
#endif		
			LPTSTR pEditType = NULL;
			PStrItem pItem = this->m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
			IXMLDOMElement * pElement = NULL;
			_variant_t var;
			if(pItem)
			{
				IXMLDOMNode * pNode;
				if(pItem->pStrXml)
				{	xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pElement->getAttribute(L"edittype",&var);
						if(var.bstrVal)
							pEditType = _tcsdup(var.bstrVal);
						pNode->Release();
						if(pElement)pElement->Release();
					}
				}
			}
			if(pEditType && _tcsicmp(pEditType,_T("datetimepick"))==0)
			{
				editType=CInplaceEdit::_control;
				pXClass = L"xdatetimepick";
			}else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0)
			{
				editType=CInplaceEdit::_combobox;
			}else if(pEditType && _tcsicmp(pEditType,_T("listbox"))==0)
			{
				editType=CInplaceEdit::_listbox;
			}else if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0)
			{
				editType=CInplaceEdit::_checkbox;
			}
			else if(pEditType && _tcsicmp(pEditType,_T("mask.date"))==0)
			{
				editType=CInplaceEdit::_datetime;
			}
			else
				editType=CInplaceEdit::_edit;

			if(pEditType)delete pEditType;
#if 0
		}
#endif		

		for(int i=0;i< m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]->editType == editType) 
				if(editType !=CInplaceEdit::_control)
					return m_EditBuffer[i]->pEdit;
				else
					if(::_wcsicmp(m_EditBuffer[i]->pEdit->m_szCotrolClass,pXClass)==0)
						return m_EditBuffer[i]->pEdit;
		}

		CInplaceEdit * pEdit;
		RECT	rc;
		::SetRect(&rc,0,0,0,0);
		switch(editType)
		{
		case CInplaceEdit::_combobox:
			{
				pEdit = new CCellComboBoxEditer();
				pEdit->m_dwStyle = WS_VISIBLE|WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_VSCROLL|WS_HSCROLL;
				pEdit->m_pszWinClass =_tcsdup(_T("combobox"));
				HWND hWnd = pEdit->CreateControl(NULL,rc,(HWND)m_pGridSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());

				((CCellComboBoxEditer*)pEdit)->AttachProcEdit();}
			break;
		case CInplaceEdit::_listbox:
			{
				LPTSTR pEditType = NULL;
				PStrItem pItem =  m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
				DWORD dwStyle=WS_CHILD|WS_VSCROLL|WS_BORDER;
				if(pItem)
				{
					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					IXMLDOMElement * pElement = NULL;
					IXMLDOMNode * pNode;
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					VARIANT var0;
					if(pElement)pElement->getAttribute(L"listbox_mulit",&var0);
					if(var0.bstrVal)
						if(::_tcsicmp(var0.bstrVal,_T("Y"))==0)
							dwStyle |= LBS_MULTIPLESEL|LBS_MULTICOLUMN|WS_HSCROLL;
					::VariantClear(&var0);

					pElement->Release();
				}
				pEdit = new CCellListBoxEditer();
				pEdit->m_dwStyle = dwStyle;
				pEdit->m_pszWinClass =_tcsdup(_T("listbox"));
				HWND hWnd = pEdit->CreateControl(NULL,rc,(HWND)m_pGridSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
			}
			break;
		case CInplaceEdit::_checkbox:
			{
				pEdit = new  CCellCheckBoxEditer();
				pEdit->m_dwStyle = WS_CHILD|BS_AUTOCHECKBOX|BS_CHECKBOX|BS_VCENTER;
				pEdit->m_pszWinClass =_tcsdup(_T("button"));
				HWND hWnd = pEdit->CreateControl(NULL,rc,(HWND)m_pGridSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
			}
			break;
		case CInplaceEdit::_control:
			pEdit = new CControlEditer();
			((CControlEditer *)pEdit)->InstanceControl((HWND)m_pGridSvr->GetHWND(),pXClass);
			break;
		case CInplaceEdit::_datetime:
			pEdit = new CMaskEditer();
			pEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL;
			pEdit->m_pszWinClass =_tcsdup(_T("edit"));
			pEdit->CreateControl(NULL,rc,(HWND)m_pGridSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
			((CMaskEditer*)pEdit)->AttachProcEdit();
			break;
		default:
			pEdit = new  CCellEditer();
			pEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|(m_pGridSvr->GetEditUpperMode()?ES_UPPERCASE:0);
			pEdit->m_pszWinClass =_tcsdup(_T("edit"));
			pEdit->CreateControl(NULL,rc,(HWND)m_pGridSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
			((CCellEditer*)pEdit)->AttachProcEdit();
		}
		const XUtil::Css::TBoxStyle* pBoxStyle = m_pGridSvr->GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
		LOGFONT	logFont;
		if(pBoxStyle) XUtil::Css::GetFont(pBoxStyle,logFont);
		else XUtil::Css::GetDefaultFont(logFont);
		HFONT	hFont =::CreateFontIndirect(&logFont);
		::SendMessage(pEdit->GetHWND(),WM_SETFONT,(WPARAM)hFont,false);	

		tagEditCtlBuff	* pBuffer = new tagEditCtlBuff();
		pBuffer->editType = editType;
		pBuffer->pEdit = pEdit; 
		this->m_EditBuffer.push_back(pBuffer);
		pEdit->m_pEditSvr = this;
		return pEdit;
	}
	int CEditSvr::Draw(int ACol,int ARow,HDC hdc,RECT rect)
	{
		return 1;
	}

	int	CEditSvr::SelectCell(int ACol,int ARow)
	{
		if(m_pEdit)
			//this->UpdateCellContent(m_ARow,m_ACol);
			this->UpdateCellContent(m_ACol,m_ARow);

		bool bQuery;
		bool bShowButton;
		bQuery = this->m_pGridSvr->IsEditQuery(ACol,ARow,bShowButton);

		RECT rc;
		m_ARow = ARow;
		m_ACol = ACol;
		m_pEdit = this->GetEditCtl(ACol,ARow);

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = this->m_pGridSvr->GetHWND();
		hdr.idFrom = (UINT_PTR)m_pEdit->GetHWND();
		hdr.code = DWV_ITEMBEINGEDIT;//DWV_CLICKED;
		hdr.DwTable = NULL;
		hdr.row = ARow;
		hdr.col = ACol;	
		PStrItem pItem = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
		if(pItem && pItem->pStrColumn)
			hdr.colname = pItem->pStrColumn;
		if(pItem && pItem->pStrGroup)
			hdr.data = pItem->pStrGroup;
		::SendMessage(::GetParent(this->m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		UpdateEditContent(ACol,ARow);
		m_pGridSvr->GetCellRect(ACol,ARow,rc);
		InflateRect(&rc,-1,-1);
		if(bQuery) rc.right -= 16;
		::OffsetRect(&rc,m_pGridSvr->m_pSheetSvr->GetOffset().x,
				m_pGridSvr->m_pSheetSvr->GetOffset().y);
		
		if(m_pEdit->m_pszWinClass && _tcscmp(m_pEdit->m_pszWinClass,_T("combobox")) == 0)
		{
			int dTop = (rc.bottom - rc.top)/2 - 8;
			rc.top += dTop;
		}
		MoveLoc(rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SW_SHOW);
		/*if(m_pEdit->m_pszWinClass && _tcscmp(m_pEdit->m_pszWinClass,_T("edit")) == 0)
		{
			if(bQuery && m_pGridSvr->FOptions & goRunState)
				::SendMessage(m_pEdit->GetHWND(),EM_SETREADONLY,true,0);
			else
				::SendMessage(m_pEdit->GetHWND(),EM_SETREADONLY,false,0);
		}*/
		ShowTip(ARow, ACol);
		::SetFocus(m_pEdit->GetHWND());
		return 1;
	}

	int CEditSvr::ShowTip(int ARow, int ACol, bool f)
	{
		LPTSTR pUrl = NULL;
		if(f)
		{
			//if(ARow == m_ARow && ACol == m_ACol) return 1;

			if(TipHWMD && ::IsWindow(TipHWMD)) ::DestroyWindow(TipHWMD);

			pUrl = m_pGridSvr->GetItemProp(ACol, ARow, L"windowurl");
			if(!pUrl || _tcscmp(pUrl, L"") == 0) return -1;
		}

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGridSvr->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_EDITTEXTCHANGED;
		hdr.DwTable = NULL;//m_pGridSvr->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		if(m_pGridSvr->GetGridDataSvr()->GetDwTableFromCell(ACol,ARow))
		{
			int nRow, nCol;
			m_pGridSvr->GetGridDataSvr()->GetDwTableFromCell(ACol,ARow)->getRowColumnFromCell(ACol, ARow, nRow, nCol);
			hdr.row = nRow;
			hdr.col = nCol;
		}else
		{
			hdr.row = ARow;
			hdr.col = ACol;
		}
		if(TipHWMD && ::IsWindow(TipHWMD))
			hdr.oldrow = (int)TipHWMD;
		else
			hdr.oldrow = 0;
		hdr.oldcol = (int)this->m_pEdit->GetHWND();
		PStrItem pItem = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
		if(pItem && pItem->pStrColumn)
			hdr.colname = pItem->pStrColumn;
		hdr.data = this->m_pEdit->GetText();
		::SendMessage(::GetParent(m_pGridSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		if(hdr.oldrow < 0) 
		{
			if(!f && TipHWMD && ::IsWindow(TipHWMD)) ::DestroyWindow(TipHWMD);
			return 1;
		}
		if(!f) return 1;

		TCHAR buf[512];
		if(_tcsstr(pUrl, _T("dev:xpage")) == pUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pUrl);

		TGridDrawInfo GridInfo;
		m_pGridSvr->InitDrawInfo(&GridInfo);

		RECT rc;
		m_pGridSvr->GetCellRect(GridInfo,m_pGridSvr->FCurrent.x,m_pGridSvr->FCurrent.y,rc);

		POINT ptStart,ptEnd;
		ptStart.x = rc.left;
		ptStart.y = rc.top;
		ClientToScreen(m_pGridSvr->GetHWND(),&ptStart);
		ptEnd.x = rc.right;
		ptEnd.y = rc.bottom;
		ClientToScreen(m_pGridSvr->GetHWND(),&ptEnd);
		RECT rt;
		::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
		CDwTable * pDwTable =  m_pGridSvr->GetCurDwTable();
		IDwTable * pTable = NULL;
		if(pDwTable) pTable = (IDwTable *)pDwTable->GetInterface();
		xfWin* pWin = xfWin::LoadXPage(NULL,buf,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,/*WS_DLGFRAME|WS_THICKFRAME|*/WS_POPUP,&rt,::GetParent(m_pGridSvr->GetHWND()),NULL,(LPVOID)pTable);
		TipHWMD = pWin->GetHWND();
		ShowWindow(TipHWMD,SW_SHOWNOACTIVATE);
		return 1;
	}

	int CEditSvr::HiddenEditor()
	{
		if(m_pEdit)
		{
			int isVisible = ::IsWindowVisible(m_pEdit->GetHWND());
			if(isVisible != 0)
			{
				bool bUpdateCell = true;
				bool bShowButton;
				bool bQuery = this->m_pGridSvr->IsEditQuery(m_ACol,m_ARow,bShowButton);
				if(bQuery && m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("edit"))==0)
				{
					PStrItem pItem = m_pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(m_ACol,m_ARow);

					if(pItem && pItem->pStrXml)
					{
						xbXml xml;
						xml.LoadXml(pItem->pStrXml);
						IXMLDOMElement * pElement = NULL;
						IXMLDOMNode * pNode;
						_variant_t var;
						xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
						if(pNode)
						{
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
							pNode->Release();
						}
						if(pElement)
						{
							pElement->getAttribute(L"dddwcanedit",&var);
							if(var.bstrVal && _tcsicmp(var.bstrVal,L"是")!=0)
								bUpdateCell = false;
							pElement->Release();
						}
					}
				}
				if(!bUpdateCell)
				{
					LPTSTR pStrText = m_pEdit->GetText();
					if(!pStrText || (pStrText && ::_tcscmp(pStrText, L"") == 0)) bUpdateCell = true;
				}

				if(bUpdateCell) this->UpdateCellContent(m_ACol,m_ARow);
				if(m_pEdit)
				::ShowWindow(m_pEdit->GetHWND(), SW_HIDE);
			}
			m_pEdit = NULL;
		}
		return 1;
	}
	

	CMaskEditer::~CMaskEditer()
	{
		if(::IsWindow(GetHWND()))
		{
			this->DettachProcEdit();
			::SetProp(GetHWND(),L"thisex",0);
		}
	}

	LPTSTR	CMaskEditer::GetText()
	{
		if(GetHWND())
		{
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			if(wcsstr(pStr,L"0000-00-00")||wcsstr(pStr,L"0000/00/00")||wcsstr(pStr,L"1900-00-00")||wcsstr(pStr,L"1900/00/00"))
			{
				delete pStr;
				return NULL;
			}
			if(pStr[5]=='0'&& pStr[6]=='0') pStr[6]='1';
			if(pStr[8]=='0'&& pStr[9]=='0') pStr[9]='1';
			return pStr;
		}
		else
			return NULL;
	}

	int	CMaskEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		//LPTSTR pStr = pStrContent
		if(pStrContent && pStrContent[0])
		{
			::SetWindowText(GetHWND(),pStrContent);
			::SendMessage(GetHWND(),EM_SETSEL,0,_tcslen(pStrContent));
		}
		else
		{
			::SetWindowText(GetHWND(),L"1900-00-00");
		}
		return 1;
	}
	int CMaskEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CMaskEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	int CMaskEditer::AttachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(m_hWnd,GWLP_WNDPROC);
			SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(long)&CMaskEditer::WindowProc);
			SetProp(m_hWnd,L"thisex",(HANDLE)this);
		}
		else
			m_PrevWndFunc = 0;
		return 1;
	}
	int CMaskEditer::DettachProcEdit()
	{
		if(IsWindow(m_hWnd))SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(long)m_PrevWndFunc);
		return 1;
	}

	BOOL CMaskEditer::RightSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0 || EndPos == StartPos ) && EndPos == ::GetWindowTextLength(handle);
	}

	BOOL CMaskEditer::LeftSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0) && ((EndPos == 0) || (EndPos == ::GetWindowTextLength(handle)));
	}

	void CMaskEditer::SelectAll()
	{

	}
	void CMaskEditer::Deselect()
	{
		SendMessage(m_hWnd,EM_SETSEL, 0x7FFFFFFF, long(0xFFFFFFFF));
	}

	int CMaskEditer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		int Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
		
		TEvent evt;
		evt.key.nChar = nChar;
		evt.key.nFlags = nFlags;
		evt.key.nRepCnt = nRepCnt;

		switch(nChar)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_ESCAPE:
		case VK_RETURN:
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
		case VK_INSERT:
			if(Shift = 0 )
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			else if(Shift == ssShift && ! (m_pEditSvr->m_pGridSvr->CanEditModify())) nChar = 0;
			break;
		case VK_LEFT:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || LeftSide(m_hWnd )))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_RIGHT:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || RightSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_HOME:
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || LeftSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_END: 
			if(/*goAlwaysShowEditor&m_pEditSvr->m_pGridSvr->FOptions &&*/ (ssCtrl&Shift || RightSide(m_hWnd)))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		case VK_F2:
			{
				//ParentEvent;
				if(nChar == VK_F2)
				{
					Deselect();
					nChar = 0;
				}
			}
		case VK_TAB:
			if(!(ssAlt & Shift))
			{
				m_pEditSvr->m_pGridSvr->EvtKeyDown(&evt,0);
				nChar = 0;
			}
			break;
		}
		if((nChar == VK_DELETE) && ! m_pEditSvr->m_pGridSvr->CanEditModify()) nChar = 0;
		if(nChar==VK_BACK)
		{
			//PostMessage(m_hWnd,EM_REPLACESEL,true,(LPARAM)"0");
			nChar = 0;
		}
		return nChar? 0:1;
	}

	int CMaskEditer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = ::GetParent(GetHWND());
		hdr.idFrom = nChar;
		hdr.code = DWV_KEYDOWN;
		//hdr.DwTable = this->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		//hdr.oldrow = -1;
		//hdr.row = -1;
		wchar_t buf[32];
		GetWindowText(m_hWnd,buf,31);
		hdr.data = buf;
		::SendMessage(::GetParent(::GetParent(GetHWND())),WM_NOTIFY,0,(LPARAM)&hdr);

		return 0;
	}
	int CMaskEditer::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		int Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;

		int    nStartChar, nEndChar;
		WORD   StartPos = 0;
		
		UINT	 Key;
		Key = nChar;
		if( Key>=32 && Key <=255 && !m_pEditSvr->m_pGridSvr->CanEditAcceptKey(Key))
		{
			Key = 0;
			MessageBeep(0);
		}

		switch(Key)
		{
		case VK_TAB:
		case VK_ESCAPE:
			Key = 0;
			break;
		case VK_BACK:
			{
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				if(nStartChar ==5 || nStartChar == 8 ) nStartChar--;
				if(nStartChar > 0)
				{
					nStartChar--;
					SendMessage(m_hWnd,EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nStartChar+1);
					SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"0");
					SendMessage(m_hWnd,EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nStartChar);
				}
				Key = 0;
			}
			break;
		case VK_RETURN:
			{
				if(Shift & ssAlt)
					break;
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				if(nStartChar == 0 && nEndChar == GetWindowTextLength(m_hWnd))
					Deselect(); else
				{
					SelectAll();
				}
				Key = 0;
				break;
			}
		default:
			if(! m_pEditSvr->m_pGridSvr->CanEditModify())
			{
				Key = 0;
				break;
			}
			

			if(Key == '=')
			{
				SYSTEMTIME dt;
				GetLocalTime(&dt);
				wchar_t buf[32];
				//_stprintf(buf, _T("%02d-%02d-%02d %02d:%02d:%02d"), dt.wYear,dt.wMonth,dt.wDay, dt.wHour, dt.wMinute, dt.wSecond);
				wsprintf(buf, L"%02d-%02d-%02d", dt.wYear,dt.wMonth,dt.wDay);
				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)0,(LPARAM)10);
				SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				Key = 0;
				break;
			}
			if(Key == '+')
			{
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				wchar_t buf[32];
				GetWindowText(m_hWnd,buf,31);
				SYSTEMTIME dt;
				dt.wYear = (buf[0] -'0')*1000 + (buf[1] -'0')*100 + (buf[2]-'0')*10 + (buf[3] -'0');
				dt.wMonth = (buf[5] -'0')*12 + (buf[6] -'0');
				dt.wDay = (buf[8] -'0')*10 + (buf[9] -'0');
				if(dt.wYear >=2099 && nStartChar <= 4)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}
				if(dt.wYear >=2099 && dt.wMonth ==12 && nStartChar <=7)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}
				if(dt.wYear >=2099 && dt.wMonth ==12 && dt.wDay ==31 && nStartChar <=10)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}

				if(nStartChar <= 4)
				{
					dt.wYear ++;
				}else if(nStartChar <= 7)
				{
					int month = dt.wMonth;
					if(dt.wMonth ==12)
					{
						dt.wYear ++;
						dt.wMonth = 1;
					}
					else 
					{
						if((month==1 || month==3 || month==5 || month==7 || month==8 || month==10) && dt.wDay==31)
						{
							dt.wDay=31;
						}
						dt.wMonth++;
					}
				}
				else if(nStartChar <= 10)
				{
					int month = dt.wMonth;
					int year = dt.wYear;
					if(dt.wDay == 28 && dt.wMonth == 2)
					{
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							dt.wMonth++;
							dt.wDay =1;
						}else
						{
							dt.wDay++;
						}
					}
					else if(dt.wDay == 29 && dt.wMonth == 2)
					{
						dt.wMonth++;
						dt.wDay =1;
					}
					else if((month==2 || month==4 || month==6 || month==9 || month==11) && dt.wDay==30)
					{
						dt.wMonth++;
						dt.wDay =1;
					}
					else if(dt.wDay ==31)
					{
						if(dt.wMonth ==12)
						{
							dt.wYear++;
							dt.wMonth = 1;
						}else
							dt.wMonth++;
						dt.wDay =1;
					}else
						dt.wDay++;
				}

				wsprintf(buf, L"%02d-%02d-%02d", dt.wYear,dt.wMonth,dt.wDay);
				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)0,(LPARAM)10);
				SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nStartChar);
				Key = 0;
				break;
			}
			else if(Key == '-')
			{
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				wchar_t buf[32];
				GetWindowText(m_hWnd,buf,31);
				SYSTEMTIME dt;
				dt.wYear = (buf[0] -'0')*1000 + (buf[1] -'0')*100 + (buf[2]-'0')*10 + (buf[3] -'0');
				dt.wMonth = (buf[5] -'0')*12 + (buf[6] -'0');
				dt.wDay = (buf[8] -'0')*10 + (buf[9] -'0');
				if(dt.wYear ==0 && nStartChar <= 4)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}
				if(dt.wYear ==0 && dt.wMonth ==0 && nStartChar <=7)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}
				if(dt.wYear ==0 && dt.wMonth ==0 && dt.wDay ==0 && nStartChar <=10)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}

				if(nStartChar <= 4)
				{
					dt.wYear --;
				}else if(nStartChar <= 7)
				{
					int month = dt.wMonth - 1;
					if(dt.wMonth ==1)
					{
						dt.wYear --;
						dt.wMonth = 12;
					}else if(dt.wMonth =3 && dt.wDay >=29)
					{
						dt.wMonth = 2;
						int year = dt.wYear;
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							dt.wDay = 28;
						}else
							dt.wDay = 29;
					}else if(month==4 || month==6 || month==9 || month==11)
					{
						if(dt.wDay==31) dt.wDay = 30;
						dt.wMonth--;
					}else
						dt.wMonth--;

				}
				else if(nStartChar <= 10)
				{
					if(dt.wDay ==1)
					{
						if(dt.wMonth==1)
						{
							dt.wYear--;
							dt.wMonth = 12;
						}
						else
							dt.wMonth--;
	
						int month = dt.wMonth;
						int year = dt.wYear;
						if(month==2)
						{
							if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
							{
								dt.wDay = 28;
							}else
								dt.wDay = 29;
						}
						else if(month==4 || month==6 || month==9 || month==11)
							dt.wDay = 30;
						else
							dt.wDay = 31;
					}
					else
						dt.wDay--;
				}

				wsprintf(buf, L"%02d-%02d-%02d", dt.wYear,dt.wMonth,dt.wDay);
				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)0,(LPARAM)10);
				SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nStartChar);
				Key = 0;
				break;
			}

			if(  Key>=32 && Key <=255 && (Key < '0' || Key > '9'))
			{
				Key = 0;
				MessageBeep(0);
				break;
			}
			else if( Key >= '0' || Key <= '9')
			{
				SendMessage(m_hWnd,EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
				wchar_t buf[32];
				 GetWindowText(m_hWnd,buf,31);
				if(nStartChar>9)
				{
					Key = 0;
					MessageBeep(0);
					break;
				}
				if(nStartChar==0)
				{
					if(Key!='0' && Key!='1' && Key!='2')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
				}
				else if(nStartChar < 4)
				{
					if(buf[6]=='2' && buf[8]=='2' && buf[9]=='9')
					{
						int year = (nStartChar==0?Key-'0':buf[0] -'0')*1000+(nStartChar==1?Key-'0':buf[1]-'0')*100+(nStartChar==2?Key -'0':buf[2] - '0')*10+(nStartChar==3?Key-'0':buf[3] - '0');
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"8");
						}
					}
				}

				if(nStartChar ==4 || nStartChar == 7) nStartChar++;
				if(nStartChar==5)
				{
					if(Key>'1')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='1' && buf[6] >'2')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='0' && buf[6]=='2' && buf[8]=='2' && buf[9]=='9')
					{
						int year = (buf[0] -'0')*1000+(buf[1]-'0')*100+(buf[2] - '0')*10+(buf[3] - '0');
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"8");
							break;
						}
						break;
					}
					
					if(buf[8]=='3' && buf[9]=='1')
					{
						int month=(Key - '0')*10 + (buf[6] - '0');
						if(month==2 || month==4 || month==6 || month==9 || month==11)
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"0");
						}
					}
				}
				else if(nStartChar==6)
				{
					if(Key>'2' && buf[5]!='0')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='2' && buf[5]=='0' && ((buf[8]=='2' && buf[9]=='9') || buf[8]=='3'))
					{
						int year = (buf[0] -'0')*1000+(buf[1]-'0')*100+(buf[2] - '0')*10+(buf[3] - '0');
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)8,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"28");
						}
						else if(buf[8]=='3')
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)8,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"29");
						}
					}
					else if(buf[8]=='3' && buf[9]=='1')
					{
						int month=(buf[5] - '0')*10 + (Key - '0');
						if(month==2 || month==4 || month==6 || month==9 || month==11)
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"0");
						}
					}

				}
				else if(nStartChar==8)
				{
					if(Key>'3')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='3' && buf[9] >='2')
					{
						int month=(buf[5] - '0')*10 + (Key - '0');
						if(month==2 || month==4 || month==6 || month==9 || month==11)
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"0");
						}else
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"1");
						}
					}
					if(Key=='3' && buf[5]=='0' && buf[6]=='2')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='3' && buf[9]=='1')
					{
						int month=(buf[5] - '0')*10 + (buf[6] - '0');
						if(month==2 || month==4 || month==6 || month==9 || month==11)
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"0");
							buf[9]='0';
						}
					}
					if(Key=='2' && buf[9]=='9' && buf[5]=='0' && buf[6]=='2')
					{
						int year = (buf[0] -'0')*1000+(buf[1]-'0')*100+(buf[2] - '0')*10+(buf[3] - '0');
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							SendMessage(m_hWnd,EM_SETSEL,(WPARAM)9,(LPARAM)10);
							SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)L"8");
						}
					}
				}
				else if(nStartChar==9)
				{
					if(buf[8] =='3' &&  Key>='2')
					{
						Key = 0;
						MessageBeep(0);
						break;
					}
					if(Key=='1' && buf[8]=='3')
					{
						int month=(buf[5] - '0')*10 + (buf[6] - '0');
						if(month==2 || month==4 || month==6 || month==9 || month==11)
						{
							Key = 0;
							MessageBeep(0);
							break;
						}
					}
					if(Key=='0' && buf[8]=='3')
					{
						if(buf[5]=='0' && buf[6]=='2')
						{
							Key = 0;
							MessageBeep(0);
							break;
						}
					}
					if(Key=='9' && buf[5]=='0' && buf[6]=='2')
					{
						int year = (buf[0] -'0')*1000+(buf[1]-'0')*100+(buf[2] - '0')*10+(buf[3] - '0');
						if(year !=0 && !(year % 400 ==0 ||(year % 4==0 && year % 100 != 0)))
						{
							Key = 0;
							MessageBeep(0);
							break;
						}
					}
				}

				SendMessage(m_hWnd,EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nStartChar+1);
				nEndChar = nStartChar+1;
				if(nEndChar < 10)
				{
					wchar_t buf[24];
					buf[0] = Key;
					buf[1] = 0;
					SendMessage(m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
					Key = 0;
				}
			}
		}
		return Key? 0:1;
	}

	LRESULT CALLBACK CMaskEditer::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			{
				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT) LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				int  nRet = 0;
				CMaskEditer * pThis = ((CMaskEditer *)GetProp(hwnd,L"thisex"));
				if(uMsg== WM_KEYDOWN) nRet = pThis->OnKeyDown(nChar, nRepCnt,nFlags);
				if(uMsg== WM_KEYUP) nRet = pThis->OnKeyUp(nChar, nRepCnt,nFlags);
				if(uMsg== WM_CHAR) nRet = pThis->OnChar(nChar, nRepCnt,nFlags);
				if(nRet==0)
				{
					nRet = CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
					if(uMsg== WM_CHAR && pThis->m_pEditSvr->TipHWMD && ::IsWindow(pThis->m_pEditSvr->TipHWMD))
					{
						pThis->m_pEditSvr->ShowTip(pThis->m_pEditSvr->m_ARow, pThis->m_pEditSvr->m_ACol, false);
					}
				}
				return nRet;
			}
		case WM_KILLFOCUS:
			{
				CMaskEditer * pThis = ((CMaskEditer *)GetProp(hwnd,L"thisex"));
				pThis->m_pEditSvr->HiddenEditor();
			}
		default:
			return CallWindowProc(((CMaskEditer *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
		}
	}

}}}}
