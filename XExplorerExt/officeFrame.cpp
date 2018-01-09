#include "StdAfx.h"
#include "OfficeFrame.h"
#include "xccontrol.h"
#include "XOffice.h"

COfficeFrame::COfficeFrame(void):m_nDocIndex(0)
{
}

COfficeFrame::~COfficeFrame(void)
{
}

int COfficeFrame::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcscmp(pStrID,_T("xmFileOpen"))==0)
	{
		TCHAR filter[]= _T("xoffice 文件(*.xdoc)\0*.xdoc;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("xoffice 文件(*.xdoc)\0*.xdoc\0"));

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
			TCHAR pStrFormat[]=_T("<xframe style=\"background:#cecebe none #cecebe none\" caption=\"%s\" type=\"sheet\" winstate=\"maximize\" rect=\"0,0,0,0\" xdll=\"ext\\XExplorerExt.dll\"><xoffice templatefile=\"%s\" name=\"xoffice\"/></xframe>");
			int nLen = (int)::_tcslen(pStrFormat) + 1 + 2*MAX_PATH +10;
			LPTSTR pStrXml = new TCHAR[nLen];
			::_stprintf_s(pStrXml,nLen,pStrFormat,szFileName,szPath);
			xfWin*  pWinEx = xfWin::LoadXPageXml(pStrXml,m_hWnd,xfApp::GetApp()->GetInstance());
			delete pStrXml;
			if(pWinEx)InvalidateRect(pWinEx->GetControl(_T("xoffice"))->m_hWnd,NULL,false);
		}
		return 1;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileNew"))==0)
	{
		TCHAR pStrFormat[]=_T("<xframe style=\"background:#cecebe none #cecebe none\" caption=\"%s%i\" type=\"sheet\" winstate=\"maximize\" rect=\"0,0,0,0\" xdll=\"ext\\XExplorerExt.dll\"><xoffice templatefile=\"%s\" name=\"xoffice\"/></xframe>");
		int nLen = (int)::_tcslen(pStrFormat) + 1 + 2*MAX_PATH +10;
		LPTSTR pStrXml = new TCHAR[nLen];
		::_stprintf_s(pStrXml,nLen,pStrFormat,_T("文档"),++m_nDocIndex,_T(""));
		xfWin*  pWinEx = xfWin::LoadXPageXml(pStrXml,m_hWnd,xfApp::GetApp()->GetInstance());
		
		delete pStrXml;

		return 1;
	}

 	if(pStrID && _tcscmp(pStrID,_T("xmFileSave"))==0)
	{
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if((int)pLayerSheet->m_pHwnds->size()< 1) return 1;
		HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet];
		xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));
		XOffice::XOfficeFrm * pOfficeFrm = dynamic_cast<XOffice::XOfficeFrm *>(pWin->GetControl(L"xoffice"));
		if(!pOfficeFrm) return 1;

		LPTSTR ext =_T("xdoc");
		LPTSTR filter = _T("xoffice 文件(*.xdoc)\0*.xdoc;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[255],szPath[255];
		int	 nFilterIndex = 1;
		ZeroMemory(szFileName,255);
		ZeroMemory(szPath,255);

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = this->GetHWND();
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = 255;
		ofn.lpstrTitle = _T("另存为");
		ofn.lpstrDefExt = ext;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
	
		if(::GetSaveFileName(&ofn))
		{
			xbXml xml;
			xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xdoc/>");
			
			IXMLDOMElement * pDocElement;
			xml.m_pXmlDoc->get_documentElement(&pDocElement);
			pOfficeFrm->m_pFrameSvr->m_pSerializeSvr->SerializeSheetData(pOfficeFrm->m_pFrameSvr->m_pDataSvr,pDocElement,xml.m_pXmlDoc);
			pDocElement->Release();
			xml.m_pXmlDoc->save((_variant_t)szPath);
			return 1;
		}
		return 1;
	}
 	if(pStrID && _tcscmp(pStrID,_T("xmFileSaveAs"))==0)
	{

	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileClose"))==0)
	{
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			pLayerSheet->m_pHwnds->erase(pLayerSheet->m_pHwnds->begin()+pLayerSheet->m_iCurSheet);
			pLayerSheet->RemoveSheet(pLayerSheet->m_iCurSheet);
			::DestroyWindow(hWnd);
		}
		return 1;
	}


	if(pStrID && _tcscmp(pStrID,_T("xmFileExit"))==0)
	{
		::PostQuitMessage(0);
		return 1;
	}
	return 0;
}

xfControl * COfficeFrame::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int COfficeFrame::AddSheet(xfWin * pWin)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(pWin)
	{
		pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);
		pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,-1,true);
	}
	return 1;
}

