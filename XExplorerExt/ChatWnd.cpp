#include "StdAfx.h"
#include "ChatWnd.h"
#include "time.h"

CChatWnd::CChatWnd(void)
{
	this->AttachListon(WM_IDLEUPDATECMDUI,(FNEvent)&CChatWnd::EvtUpdateCmdUI);
}

CChatWnd::~CChatWnd(void)
{
	if((LPTSTR)m_pParam)delete (LPTSTR)m_pParam;
}

bool CChatWnd::EvtUpdateCmdUI(TEvent* pEvent,LPARAM lParam)
{
	TCHAR strCmd[512];
	::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xchat/getmsg %s"),(LPCTSTR)m_pParam);

	BYTE* pData;
	int nLen;
	if(xfUriLoader::ProcessCmd(m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen)==1)
	{
		if(nLen<2) 
		{
			xfUriLoader::FreeData(pData);
			return true;
		}
		xfControl * pControl = GetControl(_T("msgshow"));
		if(::GetWindowTextLength(pControl->m_hWnd)!=0)
		{
			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)L"\r\n\r\n-------------------------------\r\n");
		}
		::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
		::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)(LPTSTR)(pData +2));
		xfUriLoader::FreeData(pData);
	}
	return true;
}

int CChatWnd::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 0;
	if(_tcsicmp(pStrID,_T("xmFileClose"))==0)
	{
		::DestroyWindow(m_hWnd);
		return 1;
	}
	if(_tcsicmp(pStrID,_T("xmMsgSend"))==0)
	{
		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xchat/newmsg %s"),(LPTSTR)m_pParam);
		xbXml xml;

		long nLen;
		LPTSTR pStrMsg;
		nLen = ::GetWindowTextLength(GetControl(_T("msgedit"))->m_hWnd);
		if(nLen < 1) return 1;
		pStrMsg = new TCHAR[nLen + 1];
		::GetWindowText(GetControl(_T("msgedit"))->m_hWnd,pStrMsg,nLen + 1);
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,pStrMsg,xml)==1)
		{
			::SetWindowText(GetControl(_T("msgedit"))->m_hWnd,NULL);
			
			 xfControl * pControl = GetControl(_T("msgshow"));

			if(::GetWindowTextLength(pControl->m_hWnd)!=0)
			{
				::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
				::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)L"\r\n\r\n");
			}
			
			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)xfApp::GetApp()->GetUser());
			
			__time64_t long_time;
			struct tm newtime;
			wchar_t timebuf[128];

			_time64( &long_time ); 
			_localtime64_s( &newtime, &long_time );
			wcsftime(timebuf,128,_T("%Y-%m-%d %H:%M"),&newtime);

			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)L":\r\n");

			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)L"date:");

			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)timebuf);

			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)L":\r\n");

			::SendMessage(pControl->m_hWnd,EM_SETSEL,99999999,99999999);
			::SendMessage(pControl->m_hWnd,EM_REPLACESEL,true,(LPARAM)pStrMsg);
		}
		if(pStrMsg)delete pStrMsg;

		return 1;
	}
	return 0;
}
int CChatWnd::OnOpen()
{
	//LPTSTR pUserID;
	xfControl* pControl = GetControl(_T("userid"));
	pControl->m_pxfNode->m_pCaption = _tcsdup((LPTSTR)m_pParam);

	int nLen = ::GetWindowTextLength(m_hWnd);
	TCHAR* pStr = new TCHAR[nLen + 16 + _tcslen(pControl->m_pxfNode->m_pCaption)];
	::GetWindowText(m_hWnd,pStr,nLen + 1);
	_stprintf_s(pStr,nLen + 16 + _tcslen(pControl->m_pxfNode->m_pCaption),L"%s (%s)",pStr,pControl->m_pxfNode->m_pCaption);
	::SetWindowText(m_hWnd,pStr);

	return 1;
}

