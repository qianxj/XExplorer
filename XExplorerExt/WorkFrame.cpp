#include "StdAfx.h"
#include "WorkFrame.h"
#include "xccontrol.h"

CWorkFrame::CWorkFrame(void)
{
	this->AttachListon(WM_NOTIFY,(FNEvent)&CWorkFrame::EvtNotify);
	this->AttachListon(WM_TIMER,(FNEvent)&CWorkFrame::EvtTimer);
}

CWorkFrame::~CWorkFrame(void)
{
	this->ClearAllListonsTo();

}

bool CWorkFrame::EvtTimer(TEvent* pEvent,LPARAM lParam)
{
	::KillTimer(pEvent->hWnd,10001);

	HWND hWnd = GetControl(_T("xchattree"))->m_hWnd;

	HTREEITEM hItem = TreeView_GetSelection(hWnd );
	TVITEMEX	tvItem;
	tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
	tvItem.hItem = hItem;
	TreeView_GetItem(hWnd,&tvItem);

	IXMLDOMElement * pElement;
	pElement = (IXMLDOMElement* )(tvItem.lParam);

	_variant_t var;
	BSTR bstr;
	pElement->get_tagName(&bstr);
	if(_tcsicmp(bstr,_T("user"))==0)
	{
		pElement->getAttribute(L"userid",&var);

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(GetFrameWindow()->m_pXPage->m_pStrUrl,_T("/business/xchat.xml"),strUrl,&dwSize,0);
		//xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOPMOST, WS_OVERLAPPEDWINDOW,NULL,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),_tcsdup(var.bstrVal));
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),_tcsdup(var.bstrVal));
	}
	::SysFreeString(bstr);
	
	return true;
}

bool CWorkFrame::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	if(pEvent->notify.pnmh->code == NM_DBLCLK && GetControl(_T("xchattree")) && pEvent->notify.pnmh->hwndFrom == GetControl(_T("xchattree"))->m_hWnd)
	{
		::SetTimer(pEvent->hWnd,10001,100,NULL);
	}
	return true;
}

