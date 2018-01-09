#include "StdAfx.h"
#include "..\include\XShowSerializeSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	XShowSerializeSvr::XShowSerializeSvr(void)
	{
	}

	XShowSerializeSvr::~XShowSerializeSvr(void)
	{
	}

	int XShowSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		return 1;
	}
	
	int XShowSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xshow");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();
		return 1;
	}
}}}}
