#include "StdAfx.h"
#include "..\include\XPageSheetSerializeSvr.hpp"
#include "xPageSheetDatasvr.hpp"
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	XPageSheetSerializeSvr::XPageSheetSerializeSvr(void)
	{
	}

	XPageSheetSerializeSvr::~XPageSheetSerializeSvr(void)
	{
	}

	int XPageSheetSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XPageSheetDataSvr * pXPageDataSvr = (XPageSheetDataSvr *)pDataSvr;
		
		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xpagesheet")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;  
		}else
			return 0;
		if(!pXPageDataSvr->m_pxfXPage)
			pXPageDataSvr->m_pxfXPage = new xfXPage();

		_variant_t var;
		pSheetElement->getAttribute(L"templateurl",&var);
		if(var.bstrVal)
		{
			pXPageDataSvr->m_pStrTemplate = _tcsdup(var.bstrVal);
			if(_tcsstr(var.bstrVal,_T("dev:xpage")) == var.bstrVal)
			{
				TCHAR strCmd[512];
				::_stprintf_s(strCmd,512, _T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
				xbXml xml;
				if(xfUriLoader::FetchXml(pStrUrl,strCmd,NULL,xml)==1)
				{
					IXMLDOMElement * pElement;
					xml.m_pXmlDoc->get_documentElement(&pElement);
					if(pElement)pXPageDataSvr->m_pxfXPage->Load(pElement);
					pElement->Release();
				}
			}
		}else
		{
			pXPageDataSvr->m_pxfXPage->Load(pSheetElement);
		}
		pSheetElement->AddRef();
		return 1;
	}

	int XPageSheetSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xpagesheet");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release(); 
		if(((XPageSheetDataSvr *)pDataSvr)->m_pStrTemplate)
			pElement->setAttribute(L"templateurl",(_variant_t)((XPageSheetDataSvr *)pDataSvr)->m_pStrTemplate);
		return 1;
	}
}}}}
