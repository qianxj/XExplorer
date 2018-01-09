#include "StdAfx.h"
#include "EntityExplorer.h"

CEntityExplorer::CEntityExplorer(void)
{
}

CEntityExplorer::~CEntityExplorer(void)
{
}

int CEntityExplorer::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && ::_tcsicmp(pStrID,_T("xmEntityOpen"))==0)
	{
		_variant_t val;
		pControl->m_pxfNode->m_pElement->getAttribute(L"urlex",&val);
		if(val.bstrVal)
		{
			TEntityParam* pParam = new TEntityParam;
			::_tcscpy_s(pParam->pEntity,255,_T("business/order"));

			XOffice::XReport::XReportSheet * pControl = dynamic_cast<XOffice::XReport::XReportSheet *>(this->GetControl(_T("dwexplorer")));
			int row = ((XOffice::XReport::XReportSheetSvr *)pControl->m_pSheetSvr)->GetRow();
			if(row > ((XOffice::XReport::XReportSheetSvr *)pControl->m_pSheetSvr)->GetRowCount()) return 1;
			LPCTSTR pStrNo = ((XOffice::XReport::XReportSheetSvr *)pControl->m_pSheetSvr)->GetItemString(row,1);
			::_tcscpy_s(pParam->pKeyValue,255,pStrNo);

			xfWin::LoadXPage(NULL,val.bstrVal,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
		}
		return 1;
	}

	if(pStrID && ::_tcsicmp(pStrID,_T("condition-submit"))==0)
	{
		//MessageBox(NULL,_T("xxx"),_T("xxx"),MB_OK);
		xbXml xml;
		xml.LoadXml(_T("<xquery xml=\"true\"/>"));
		IXMLDOMElementPtr m_pDocElement,m_pElement,m_pItemElement;
		xml.m_pXmlDoc->get_documentElement(&m_pDocElement);

		TCHAR strField[255];
		TCHAR strData[255];
		::GetWindowText(pControl->GetWin()->GetControl(_T("queryfield"))->m_hWnd,strField,255);
		::GetWindowText(pControl->GetWin()->GetControl(_T("querydata"))->m_hWnd,strData,255);

		if(strData[0]=='\0') return 1;
		if(strField[0]=='\0') return 1;

		xml.m_pXmlDoc->createElement(_T("condition"),&m_pElement);
		m_pDocElement->appendChild(m_pElement,NULL);

		xml.m_pXmlDoc->createElement(_T("item"),&m_pItemElement);
		//m_pItemElement->setAttribute(L"name",(_variant_t)strField);
		m_pItemElement->setAttribute(L"name",(_variant_t)L"cnt_no");
		m_pItemElement->setAttribute(L"op",(_variant_t)L"like");
		m_pItemElement->setAttribute(L"value",(_variant_t)strData);
		m_pElement->appendChild(m_pItemElement,NULL);
		BSTR bstr = NULL;
		xml.m_pXmlDoc->get_xml(&bstr);
		
		XOffice::XReport::XReportSheet * pControl = dynamic_cast<XOffice::XReport::XReportSheet *>(this->GetControl(_T("dwexplorer")));
		if(pControl)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);
			((XOffice::XReport::XReportSheetSvr *)pControl->m_pSheetSvr)->Retrieve(bstr);
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
		
		::SysFreeString(bstr);
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
	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileExit"))==0)
	{
		::PostQuitMessage(0);
		return 1;
	}

	return 0;
}

int CEntityExplorer::OnOpen()
{
	XOffice::XReport::XReportSheet * pControl = dynamic_cast<XOffice::XReport::XReportSheet *>(this->GetControl(_T("dwexplorer")));
	if(pControl && pControl->m_hWnd)SetFocus(pControl->m_hWnd);
	return 1;
}

xfControl * CEntityExplorer::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}
