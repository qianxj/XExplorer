#include "StdAfx.h"
#include "..\include\XOfficeFrmSerializeSvr.hpp"
#include "XOfficeFrmDataSvr.hpp"
#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeFrmSerializeSvr::XOfficeFrmSerializeSvr(void)
	{
	}

	XOfficeFrmSerializeSvr::~XOfficeFrmSerializeSvr(void)
	{
	}

	int XOfficeFrmSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XOfficeFrmDataSvr * pFrmDataSvr = dynamic_cast<XOfficeFrmDataSvr *>(pDataSvr);
		
		IXMLDOMElement* pElement = NULL;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodeListPtr	pNodeList;


		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xoffice")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;

		_variant_t var;
		pSheetElement->getAttribute(L"guid",&var);
		if(var.bstrVal)
			this->m_pStrGuid = _tcsdup(var.bstrVal);
		else
		{
			GUID guid;
			CoCreateGuid(&guid);
			TCHAR pStrGuid[48];
			StringFromGUID2(guid,pStrGuid,48);
			m_pStrGuid = _tcsdup(var.bstrVal);
		}
		pSheetElement->getAttribute(L"ver",&var);
		if(var.bstrVal)	m_Ver = var;

		long nLen = 0;
		pSheetElement->selectNodes(L"sheet",&pNodeList);
		pNodeList->get_length(&nLen);

		TCHAR buf[80];
		xbObject * pObject = NULL;
		XOfficeSheetSvr * pSheetSvr = NULL;
		XOfficeSheet * pSheet = NULL;
		for(int i=0;i<nLen;i++)
		{
			pNodeList->get_item(i,&pNode);
			pElement = xbXml::GetElement(pNode);

			_variant_t v;
			pElement->getAttribute(L"src",&v);
			if(v.bstrVal)
			{
				xbXml x;
				if(xfUriLoader::FetchXml(NULL,v.bstrVal,NULL,x)==1)
				{
					_variant_t v1,v2;
					pElement->getAttribute(L"caption",&v1);
					pElement->getAttribute(L"state",&v2);

					pElement->Release();
					x.GetXmlDoc()->get_documentElement(&pElement);

					pElement->setAttribute(L"caption",v1);
					pElement->setAttribute(L"state",v2);
					//BSTR bstr = x.GetXml();
					//::SysFreeString(bstr);
				}
			}

			pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheets[i] = xbXml::GetStringAttribe(pElement,_T("caption"));
			if(!pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheets[i])
			{
				::_stprintf_s(buf,80,_T("sheet %i"),i + 1);
				pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheets[i]=::_tcsdup(buf);
			}
			pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheetsImage[i] = -1;
			pControlName = xbXml::GetStringAttribe( pElement,_T("editer"));
			if(_tcsicmp(pControlName,_T("xcell"))==0 && state & 0x200000)
			{
				IXMLDOMNode * pNodeReport = NULL;
				pElement->selectSingleNode(L"report",&pNodeReport);
				if(pNodeReport)
				{
					delete pControlName;
					pControlName = _tcsdup(_T("xgridex"));
				}
			}
			xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,pControlName,pObject);
			if(pControlName)delete pControlName;
			pSheet = dynamic_cast<XOfficeSheet *>(pObject);
			if(pSheet)
			{
				pSheet->m_pHScrollBar = pFrmDataSvr->m_pOfficeFramSvr->pHScrollBar[0];
				pSheet->m_pVScrollBar = pFrmDataSvr->m_pOfficeFramSvr->pVScrollBar[0];
				pSheet->m_hWnd = pFrmDataSvr->m_pOfficeFramSvr->m_hWnd;
				pSheet->m_Standalone = false;
				
				pSheet->m_pxfNode = pFrmDataSvr->m_pOfficeFramSvr->m_pOfficeFrm->m_pxfNode;
			}
			pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement.push_back(pElement);
			pFrmDataSvr->m_pOfficeFramSvr->m_Sheets.push_back(pSheet);
		}
		pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_SheetSize = nLen;
		return 0;
	}
	
	int XOfficeFrmSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xoffice");
		if(!m_pStrGuid)
		{
			GUID guid;
			CoCreateGuid(&guid);
			TCHAR pStrGuid[48];
			StringFromGUID2(guid,pStrGuid,48);
			m_pStrGuid = _tcsdup(pStrGuid);
		}
		pElement->setAttribute(L"guid",(_variant_t)m_pStrGuid);
		m_Ver++;
		pElement->setAttribute(L"ver",(_variant_t)m_Ver);

		IXMLDOMElement * pSheetElement;

		XOfficeFrmDataSvr * pFrmDataSvr = dynamic_cast<XOfficeFrmDataSvr *>(pDataSvr);
		int cnt = pFrmDataSvr->m_pOfficeFramSvr->m_Sheets.size();
		for(int i=0;i<cnt;i++)
		{
			XOfficeSheet * pSheet;
			pSheet =  pFrmDataSvr->m_pOfficeFramSvr->m_Sheets[i];
			if(pSheet->m_pSheetSvr->m_bContentLoaded)
			{
				pXmlDoc->createElement(L"sheet",&pSheetElement);
				pSheet->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(pSheet->m_pSheetSvr->m_pDataSvr,pSheetElement,pXmlDoc);
				pSheetElement->setAttribute(L"caption",(_variant_t)pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheets[i]);
				pElement->appendChild(pSheetElement,NULL);

				//set guid and ver
				if(!pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i])
				{
					GUID guid;
					CoCreateGuid(&guid);
					TCHAR pStrGuid[48];
					StringFromGUID2(guid,pStrGuid,48);
					pSheetElement->setAttribute(L"guid",(_variant_t)pStrGuid);
					pSheetElement->setAttribute(L"ver",(_variant_t)_T("10000"));
				}else
				{
					_variant_t var;
					pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i]->getAttribute(L"guid",&var);
					if(var.bstrVal)
						pSheetElement->setAttribute(L"guid",var);
					else
					{
						GUID guid;
						CoCreateGuid(&guid);
						TCHAR pStrGuid[48];
						StringFromGUID2(guid,pStrGuid,48);
						pSheetElement->setAttribute(L"guid",(_variant_t)pStrGuid);
					}
					pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i]->getAttribute(L"ver",&var);
					if(var.bstrVal)
						pSheetElement->setAttribute(L"ver",(_variant_t)((int)var + 1));
					else
					{
						pSheetElement->setAttribute(L"ver",(_variant_t)_T("10000"));
					}
				}

				if(pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i])
					pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i]->Release();
				pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i] = pSheetElement;
			}else
			{
				pElement->appendChild(pFrmDataSvr->m_pOfficeFramSvr->m_SheetsElement[i],NULL);
			}

		}
		return 0;
	}

}}}
