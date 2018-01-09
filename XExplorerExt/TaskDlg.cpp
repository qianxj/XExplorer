#include "StdAfx.h"
#include "TaskDlg.h"
#include "xflowchart.h"

CTaskDlg::CTaskDlg(void)
{
}

CTaskDlg::~CTaskDlg(void)
{
	TEntityParam* pParam = (TEntityParam*)this->m_pParam;
	if(pParam)delete pParam;
}

int CTaskDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 1;

	if(_tcsicmp(pStrID,_T("taskclose"))==0)
	{
		m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(_tcsicmp(pStrID,_T("taskadd"))==0)
	{

		TCHAR pName[255];
		::GetWindowText(GetControl(_T("taskname"))->m_hWnd,pName,255);

		if(_tcsstr(pName,_T("<<@编号>>")))
		{
			MessageBox(m_hWnd,_T("请用具体编号代替<<@编号>>"),_T("提示"),MB_OK);
			return 1;
		}

		BSTR bstr;
		this->m_pXPage->m_pxfNode->GetXml(bstr);

		TEntityParam* pParam = (TEntityParam*)this->m_pParam;
		if(!pParam->pEntity)
		{
			if(bstr)::SysFreeString(bstr);
			return 1;
		}

		xbXml xml;
		LPTSTR pFlowGuid = pParam->pEntity;
		int nLen = (int)wcslen(bstr) + 1 + 2 * (int)wcslen(L"<workflow>")+1;
		LPTSTR pBuf = new TCHAR[nLen];
		_stprintf_s(pBuf,nLen,_T("<workflow>%s</workflow>"),bstr);
		if(bstr)::SysFreeString(bstr);
		xml.LoadXml(pBuf);
		if(pBuf)delete pBuf;

		IXMLDOMElement  * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		pElement->setAttribute(L"refguid",(_variant_t)pFlowGuid);
		pElement->Release();

		xml.m_pXmlDoc->get_xml(&bstr);


		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow/new %s"),pFlowGuid);
		
		LPBYTE pData = NULL;
		xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,bstr,pData,nLen);
		if(pData) delete pData;
		if(bstr)::SysFreeString(bstr);

		m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	return 1;
}
int CTaskDlg::OnOpen()
{
	TEntityParam* pParam = (TEntityParam*)this->m_pParam;
	if(pParam)
	{
		LPTSTR pFlowGuid = pParam->pEntity;
		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:workflow/getitem %s"),pFlowGuid);

		::SetWindowText(GetControl(_T("taskname"))->m_hWnd,pParam->pWorkNodePath);

		xbXml xml;
		IXMLDOMElement * pElement;
		IXMLDOMNode * pNode;
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
		{
			xml.m_pXmlDoc->selectSingleNode(L"/sheet",&pNode);
			if(!pNode)
				xml.m_pXmlDoc->selectSingleNode(L"/xdoc/sheet[1]",&pNode);
			if(!pNode) return true;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			if(!pElement) return true;

			//get flowchart
			XOffice::XFlowChart::XFlowSheet* pSheet =  (XOffice::XFlowChart::XFlowSheet*)GetControl(_T("flowchart"));
			XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = new XOffice::XFlowChart::XFlowChartDataSvr();
			XOffice::XFlowChart::XFlowChartDataSvr * pOldDataSvr;
			pSheet->m_pSheetSvr->m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr);

			pOldDataSvr = (XOffice::XFlowChart::XFlowChartDataSvr *)pSheet->m_pSheetSvr->m_pDataSvr;
			((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->Clear();
			pSheet->m_pSheetSvr->m_pDataSvr = pDataSvr;
			if(pOldDataSvr)delete pOldDataSvr;

			::InvalidateRect(pSheet->m_hWnd,NULL,false);

			pElement->Release();
		}
	}
	return 1;
}

