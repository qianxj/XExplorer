#include "StdAfx.h"
#include "RulerSheet.h"
#include "xcControl.h"
#include <strstream>
using namespace std;
#include "Scintilla.h"
#include "xreport.h"

RulerSheet::RulerSheet(void):m_nSheet(0)
{
}


RulerSheet::~RulerSheet(void)
{
}

int RulerSheet::MakeData(IXMLDOMNode * pNode,XRulerDataSvr * pContentSvr )
{
	IXMLDOMElementPtr pSchemaElement;
	pContentSvr->treeItems.clear();
	if(pNode)
	{
		pSchemaElement = pNode;
		pContentSvr->Load(pSchemaElement);
		XOffice::XXmlContentSvr::TTreeItem item;
		for(int i=0;i<pContentSvr->GetRowCount();i++)
		{
			IXMLDOMElement * pElement;
			_variant_t val;
			pElement = pContentSvr->GetRowElement(i + 1);
			pElement->getAttribute(L"type",&val);
			if(val.bstrVal && (::_tcsicmp(val.bstrVal,_T("单行集合"))==0 || ::_tcsicmp(val.bstrVal,_T("多行集合"))==0))
			{
				item.children = true;
				item.expanded = true;
				item.nImage = 22;
			}else
			{
				item.children =  false;
				item.expanded = true;
				item.nImage = 5;
			}
			item.nContentCol = 1;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			item.nLevel = 1;
			if(i>0)
			{
				IXMLDOMNode * pNode;
				IXMLDOMNode * pParentNode;
				pElement->get_parentNode(&pParentNode);
				while(pParentNode)
				{
					item.nLevel++;
					pNode = pParentNode;
					pNode->get_parentNode(&pParentNode);
					pNode->Release();
				}
				item.nLevel--;
			}
			item.nSelectedImage = -1;
			pContentSvr->treeItems.push_back(item);
			pElement->Release();
		}
	}
	return 1;
}

int RulerSheet::OnOpen()
{
	xfControl * pEdit = GetControl(L"xcontent");
	if(pEdit)
	{
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}

	pXRulerDataSvr = new XRulerDataSvr();
	xbXml xml;
	xml.LoadXml(_T("<datasource xroot=\"//*\"> \
				  <col name=\"colname\" xpath=\"@colname\" desc=\"列名\" />  \
				  <col name=\"name\" xpath=\"@name\" desc=\"名称\" />  \
				  <col name=\"type\" xpath=\"@type\" desc=\"类型\" /> \
				  <col name=\"coltype\" xpath=\"@coltype\" desc=\"列类型\" />  \
				  <col name=\"pkey\" xpath=\"@pkey\" desc=\"主键\" />  \
				  <col name=\"fkey\" xpath=\"@fkey\" desc=\"外键\" />  \
				  <col name=\"ver\" xpath=\"@ver\" desc=\"版本\" />  \
				  <col name=\"note\" xpath=\"@note\" desc=\"备注\" />  \
				  </datasource>"));
	IXMLDOMElement * pElement ;
	xml.m_pXmlDoc->get_documentElement(&pElement);
	pXRulerDataSvr->Initial(pElement);

	XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)this->GetControl(_T("xreport"));
	XOffice::XXmlContentSvr * pContentSvr = (XOffice::XXmlContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
	if(pContentSvr)
	{
		delete pContentSvr;
		((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr = pXRulerDataSvr;
	}
	return 1;
	
}

int RulerSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		m_pParentWin->OnXCommand(pStrID,pControl);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmValid"))==0)
	{
		xbXml xml;
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xcontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

		xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
		::SetWindowText(pOutPut->m_hWnd,_T(""));
		TCHAR ErrText[4096]={0};
		BSTR bstr;
		if(!xml.LoadXml(pXml))
		{
			IXMLDOMParseError* pError;
			xml.m_pXmlDoc->get_parseError(&pError);
			if(pError)
			{
				long nLine;
				long nLinePos;
				long nErrCode;
				pError->get_line(&nLine);
				pError->get_linepos(&nLinePos);
				pError->get_errorCode(&nErrCode);

				pError->get_reason(&bstr);
				_stprintf_s(ErrText,4096,_T("XML 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_SCROLLCARET,0,0);
				::SysFreeString(pXml);
				return 1;
			}
		}
		TCHAR buf[256];
		::GetWindowText(GetHWND(),buf,255);
		if(_tcsstr(buf,L".xsd") || _tcsstr(buf,L".XSD") || _tcsstr(buf,L".Xsd"))
		{
			bstr = NULL;
			xbXml::ValidSchema(pXml,bstr);
			if(bstr)
			{
				_stprintf_s(ErrText,4096,_T("Xml Schema 语法错误:\r\n%s"),bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);
			}
		}else
		{
			IXMLDOMElement * pElement =  NULL;
			xml.GetXmlDoc()->get_documentElement(&pElement);
			variant_t var;
			pElement->getAttribute(L"schema",&var);
			if(var.bstrVal)
			{
				xbXml x;
				if(xfUriLoader::FetchXml(NULL,var.bstrVal,NULL,x)==1)
				{
					bstr = NULL;
					xbXml::ValidSchema(xml,x,bstr);
					if(bstr)
					{
						_stprintf_s(ErrText,4096,_T("Schema校验:\r\n%s"),bstr);
						::SysFreeString(bstr);
						::SetWindowText(pOutPut->m_hWnd,ErrText);
					}
				}
			}
		}
		::SysFreeString(pXml);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditFind"))==0)
	{

		OnCodeFind();
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditReplace"))==0)
	{
		OnCodeReplace();
		return 1;
	}
	return 0;
}

xfControl * RulerSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int RulerSheet::PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
{
	if(_tcsicmp(pStrLayerSheet,_T("sheet1"))==0)
		m_nSheet = 0;
	else if(_tcsicmp(pStrLayerSheet,_T("sheet2"))==0)
		m_nSheet = 1;

		if(m_nSheet == 1)
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		IXAServerArg arg;
		xfControl * pControl = NULL;

		TCHAR buf[256];
		::GetWindowText(GetHWND(),buf,255);
		if(_tcsstr(buf,L".xsd") || _tcsstr(buf,L".XSD") || _tcsstr(buf,L".Xsd"))
		{
			pControl = GetControl(_T("xcontentview"));
			pControl->GetXfNode()->m_LayerFlow  = LayerFlow_UnLayer;
			pControl->SetLayerFlow(L"unlayer",true);

			xbXml x;
			IXMLDOMElement * pRoot ;
			BSTR bstr;
			XSEdit * pCtl = (XSEdit *)GetControl(_T("xcontent"));
			pCtl->GetContent(&bstr);
			xbXml::GetSchemaXml(bstr,x);
			::SysFreeString(bstr);
			x.m_pXmlDoc->get_documentElement(&pRoot);
			if(pRoot)
			{
				IXMLDOMNode * pNode;
				pRoot->selectSingleNode(L"schema/*[1]",&pNode);
				if(pNode)
				{
					XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
					XRulerDataSvr * pContentSvr = (XRulerDataSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
					MakeData(pNode,pContentSvr);
					pNode->Release();
				}
				pRoot->Release();
			}
		}
		else
		{
			pControl = GetControl(_T("xreport"));
			pControl->GetXfNode()->m_LayerFlow  = LayerFlow_UnLayer;
			pControl->SetLayerFlow(L"unlayer",true);

			IXAServerArg arg;
			BSTR bstr;
			XSEdit * pCtl = (XSEdit *)GetControl(_T("xcontent"));

			pCtl->GetContent(&bstr);
			arg.AddArg(L"content",bstr);
			LPBYTE pData = NULL;
			int len;
			if(IXAServer::ExecXQuery(GetFrameWindow()->m_pXPage->m_pStrUrl,L"[dev.xruler.trans]",arg.GetString(),pData,len)==1)
			{
				xcSHtml* pHtml = (xcSHtml*)GetControl(_T("xcontentview"));
				pHtml->LoadHtml((LPCTSTR)(pData));
				if(pData) xfUriLoader::FreeData(pData);
			}
			::SysFreeString(bstr);
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
	}	
	return 1;
}

int RulerSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}
int RulerSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	return 1;
}
