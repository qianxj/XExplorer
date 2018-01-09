#include "StdAfx.h"
#include "xbXml.hpp"
#include "xuxutil.h"

//#include "pugixml.hpp"
//#include "rapidxml.hpp"
#include "xuXml.hpp"

namespace Hxsoft{ namespace XFrame
{
EXTERN_C const CLSID CLSID_DOMDocument60;


	xbXml::xbXml(void)
	{
		DWORD dwClsContext = CLSCTX_ALL;
		//IUnknown* pIUnknown;
		HRESULT hr;

		//创建DOMDocument对象 
		/*hr = CoCreateInstance(CLSID_DOMDocument60,NULL, dwClsContext, IID_IUnknown, reinterpret_cast<void**>(&pIUnknown));
		if (!SUCCEEDED(hr))
		hr = CoCreateInstance(CLSID_DOMDocument50,NULL, dwClsContext, IID_IUnknown, reinterpret_cast<void**>(&pIUnknown));
		if (!SUCCEEDED(hr))*/
		/*hr = CoCreateInstance(CLSID_DOMDocument40,NULL, dwClsContext, IID_IUnknown, reinterpret_cast<void**>(&pIUnknown));
		if (!SUCCEEDED(hr))
			hr = CoCreateInstance(CLSID_DOMDocument30,NULL, dwClsContext, IID_IUnknown, reinterpret_cast<void**>(&pIUnknown));
		if (!SUCCEEDED(hr)) return;

		hr = OleRun(pIUnknown);
		if (SUCCEEDED(hr)){
			hr = pIUnknown->QueryInterface(IID_IXMLDOMDocument2, reinterpret_cast<void**>(&m_pXmlDoc));
		}else
		{
			m_pXmlDoc->Release();
			m_pXmlDoc = NULL;
			return;
		}

		pIUnknown->Release();
		if (!SUCCEEDED(hr))
		{
			m_pXmlDoc->Release();
			m_pXmlDoc = NULL;
			return;
		}*/

		m_pXmlDoc = NULL;

		GUID guid;
		hr = CLSIDFromString((LPOLESTR)L"Msxml2.DOMDocument.6.0",&guid);
		if(!SUCCEEDED(hr)) guid = CLSID_DOMDocument40;
		hr = CoCreateInstance(guid, 
					 NULL, 
					 CLSCTX_INPROC_SERVER, 
					 IID_IXMLDOMDocument2,
					 (void**)&m_pXmlDoc);

		if (!SUCCEEDED(hr))
			hr = CoCreateInstance(CLSID_DOMDocument30, 
						 NULL, 
						 CLSCTX_INPROC_SERVER, 
						 IID_IXMLDOMDocument2,
						 (void**)&m_pXmlDoc);

		m_pXmlDoc->put_async(VARIANT_FALSE);
		m_pXmlDoc->put_validateOnParse(VARIANT_FALSE);
		_variant_t var = "xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:ms='urn:schemas-microsoft-com:xslt'";
		m_pXmlDoc->setProperty(L"SelectionNamespaces", var); 
		var = L"XPath";
		m_pXmlDoc->setProperty(L"SelectionLanguage", var); 
	}

	xbXml::~xbXml(void)
	{
		if(m_pXmlDoc)m_pXmlDoc->Release();
	}

	bool xbXml::Load(LPCTSTR pStrFile)
	{
		VARIANT_BOOL vtResult;
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = ::SysAllocString(pStrFile);

		HRESULT hr = m_pXmlDoc->load(var,&vtResult);
		::VariantClear(&var);

		if(vtResult == VARIANT_TRUE)
			return true;
		else
			return false;
	}

	bool xbXml::LoadXml(LPCTSTR pStrXml)
	{
		VARIANT_BOOL vtResult;
		ULONG dwStart = GetTickCount();
		m_pXmlDoc->loadXML((_bstr_t)pStrXml,&vtResult);
		ULONG dwUsed = GetTickCount() - dwStart;
		if(dwUsed > 1000)
		{
			/*
			pugi::xml_document doc;
			dwStart = GetTickCount();
			pugi::xml_parse_result result = doc.load_buffer(pStrXml, wcslen(pStrXml)*sizeof(wchar_t));
			const char * error = result.description();
			dwUsed =  GetTickCount() - dwStart;
			
			dwStart = GetTickCount();
			rapidxml::xml_document<wchar_t> doc1;	// character type defaults to char
			doc1.parse<0>((wchar_t*)pStrXml);    // 0 means default parse flags
			dwUsed =  GetTickCount() - dwStart;
			dwUsed = dwUsed;
			*/
			dwStart = GetTickCount();
			pStrXml = XUtil::Xml::SkipPI(pStrXml);
			pStrXml = XUtil::Xml::FirstChildElement(pStrXml,true);
			int CountItem = 0;
			while(pStrXml && *pStrXml)
			{
				CountItem++;
				pStrXml = XUtil::Xml::NextSblingElement(pStrXml);
			}
			dwUsed =  GetTickCount() - dwStart;
			dwUsed = dwUsed;
		}
		if(vtResult == VARIANT_TRUE)
			return true;
		else
			return false;
	}

	LPTSTR xbXml::GetStringAttribe(IXMLDOMElement * pElement,LPCTSTR attrib)
	{
		LPTSTR pStrAttrib = NULL;
		_variant_t var;
		pElement->getAttribute((_bstr_t)attrib,&var);
		if(!var.bstrVal) return NULL;

		pStrAttrib = XUtil::tcsdupFromBstr(var.bstrVal);

		return pStrAttrib;
	}
	IXMLDOMElement * xbXml::GetElement(IXMLDOMNode * pNode)
	{
		IXMLDOMElement *	pElemnt = NULL;
		if(pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElemnt)!=S_OK) return NULL;
		return pElemnt;
	}
	LPTSTR	xbXml::BStr2T(BSTR bstr)
	{
		return XUtil::tcsdupFromBstr(bstr);
	}

	IXMLDOMDocument2* xbXml::GetXmlDoc()
	{
		//IDispatch* pDisp;
		//m_pXmlDoc->QueryInterface(IID_IDispatch,(void **)&pDisp);
		//return pDisp;
		return m_pXmlDoc;
	}

	BSTR xbXml::GetXml()
	{
		BSTR bstr;
		if(m_pXmlDoc)
		{
			m_pXmlDoc->get_xml(&bstr);
			return bstr;
		}
		return NULL;
	}

	xbXml * xbXml::CreateInstance()
	{
		xbXml * pxml = new xbXml();
		return pxml;
	}

	void xbXml::ReleaseInstance(xbXml * p)
	{
		delete p;
	}
	IXMLDOMSchemaCollection2* xbXml::CreateSchemaCache()
	{
		IXMLDOMSchemaCollection2* pCollection = NULL;
		HRESULT hr = CoCreateInstance(CLSID_XMLSchemaCache40, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IXMLDOMSchemaCollection,
			(void**)&pCollection);
		/*if (!SUCCEEDED(hr))
			HRESULT hr = CoCreateInstance(CLSID_XMLSchemaCache60, 
				NULL, 
				CLSCTX_INPROC_SERVER, 
				IID_IXMLDOMSchemaCollection,
				(void**)&pCollection);*/
		if (!SUCCEEDED(hr))
			HRESULT hr = CoCreateInstance(CLSID_XMLSchemaCache30, 
				NULL, 
				CLSCTX_INPROC_SERVER, 
				IID_IXMLDOMSchemaCollection2,
				(void**)&pCollection);
		return pCollection;
	}
	bool xbXml::ValidSchema(LPTSTR pSchema ,BSTR &bstr)
	{
		xbXml x;
		bstr = NULL;
		if(!x.LoadXml(pSchema))
		{
			IXMLDOMParseError* pError;
			x.GetXmlDoc()->get_parseError(&pError);
			if(pError)
			{
				pError->get_reason(&bstr);
				pError->Release();
			}
			return false;
		}
		IXMLDOMSchemaCollection2* pCollection = CreateSchemaCache();
		HRESULT hr = pCollection->add(L"http://www.hxsoft.com/schema",(_variant_t)x.GetXmlDoc());
		if( !SUCCEEDED( hr ) )
		{
			ISupportErrorInfo * spInfo = NULL;
			pCollection->QueryInterface(IID_ISupportErrorInfo,(void **)&spInfo);
			if(spInfo)
			{
				hr = spInfo->InterfaceSupportsErrorInfo( IID_IXMLDOMSchemaCollection2);	
				if( SUCCEEDED( hr ) )
				{
					IErrorInfo * pError=NULL;
					::GetErrorInfo(0,&pError);
					if(pError)
						pError->GetDescription(&bstr);
					pError->Release();
				}
				spInfo->Release();
			}
		}
		if(pCollection) pCollection->Release();

		return true;
	}
	bool xbXml::ValidSchema(xbXml &xml ,xbXml &schema ,BSTR &bstr)
	{
		IXMLDOMSchemaCollection2* pCollection = CreateSchemaCache();
		HRESULT hr = pCollection->add(L"http://www.hxsoft.com/schema",(_variant_t)schema.GetXmlDoc());
		xml.GetXmlDoc()->putref_schemas((_variant_t)pCollection);
		IXMLDOMParseError* pError;
		hr = xml.GetXmlDoc()->validate(&pError);
		long errCode;
		pError->get_errorCode(&errCode);
		if(errCode != S_OK)
		{
			long nLine;
			long nLinePos;
			long nErrCode;
			TCHAR ErrText[4096]={0};
			pError->get_line(&nLine);
			pError->get_linepos(&nLinePos);
			pError->get_errorCode(&nErrCode);
			pError->get_reason(&bstr);
			_stprintf_s(ErrText,4096,_T("%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
			::SysFreeString(bstr);
			::SysAllocString(ErrText);
			pError->Release();
			return false;
		}
		pCollection->Release();
		return true;
	}

	int xbXml::processElement(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaElement * pSchemaEle)
	{
		IXMLDOMElement *pEle = NULL;
		BSTR bstr = NULL;
		pDocument->createElement(L"item",&pEle);
		pSchemaEle->get_name(&bstr);
		pEle->setAttribute(L"colname",(_variant_t)bstr);
		pEle->setAttribute(L"name",(_variant_t)bstr);
		pEle->setAttribute(L"coltype",(_variant_t)L"");
		pEle->setAttribute(L"pkey",(_variant_t)L"");
		pEle->setAttribute(L"fkey",(_variant_t)L"");
		pEle->setAttribute(L"ver",(_variant_t)L"");
		pEle->setAttribute(L"note",(_variant_t)L"");
		IVBSAXAttributes* pItems;
		pSchemaEle->get_unhandledAttributes(&pItems);
		int nlen = 0;
		pItems->get_length(&nlen);
		for(int i=0;i<nlen;i++)
		{
			BSTR strName = NULL;
			pItems->getQName(i,&strName);
			if(strName && (_tcsicmp(strName,L"hx:cname")==0 || 
							_tcsicmp(strName,L"hx:cname")==0))
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"name",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			if(strName && _tcsicmp(strName,L"hx:coltype")==0)
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"coltype",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			if(strName && _tcsicmp(strName,L"hx:pkey")==0)
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"pkey",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			if(strName && _tcsicmp(strName,L"hx:fkey")==0)
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"fkey",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			if(strName && (_tcsicmp(strName,L"hx:ver")==0 ||
					_tcsicmp(strName,L"hx:version")==0))
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"ver",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			if(strName && _tcsicmp(strName,L"hx:note")==0)
			{
				BSTR strValue = NULL;
				pItems->getValue(i,&strValue);
				if(strValue)
				{
					pEle->setAttribute(L"note",(_variant_t)strValue);
					::SysFreeString(strValue);
				}
			}
			::SysFreeString(strName);
		}

		variant_t mo;
		pSchemaEle->get_maxOccurs(&mo);

		ISchemaType * pType;
		pSchemaEle->get_type(&pType);
		SOMITEMTYPE ItemType;
		pType->get_itemType(&ItemType);
		if(ItemType==SOMITEM_COMPLEXTYPE)
		{
			processComplexType(pDocument,pEle,(ISchemaComplexType *)pType);
			if(mo.bstrVal)
			{
				int mc = ::_ttol(mo.bstrVal);
				if(mc==1)
					pEle->setAttribute(L"type",(_variant_t)L"单行集合");
				else
					pEle->setAttribute(L"type",(_variant_t)L"多行集合");
			}else
				pEle->setAttribute(L"type",(_variant_t)L"单行集合");
		}else
		{
			processSimpleType(pDocument,pEle,pType);
		}
		pType->Release();

		pParentEle->appendChild(pEle,NULL);
		pEle->Release();
		::SysFreeString(bstr);

		return 1;
	}

	int xbXml::processGroup(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaModelGroup * pGroup)
	{
		ISchemaItemCollection * pCollect;
		pGroup->get_particles(&pCollect);
		long nLen;
		pCollect->get_length(&nLen);
		ISchemaItem * pItem;
		for(int i=0;i<nLen;i++)
		{
			pCollect->get_item(i,&pItem);
			SOMITEMTYPE ItemType;
			pItem->get_itemType(&ItemType);
			if(ItemType== SOMITEM_ELEMENT)
			{
				ISchemaElement * pSchemaEle;
				pItem->QueryInterface(IID_ISchemaElement,(void **) &pSchemaEle);
				processElement(pDocument,pParentEle,pSchemaEle);
			}else if((ItemType & SOMITEM_GROUP) ==SOMITEM_GROUP)
			{
				ISchemaModelGroup * pGroupEx;
				pItem->QueryInterface(IID_ISchemaElement,(void **) &pGroupEx);
				processGroup(pDocument,pParentEle,pGroupEx);
			}else if(ItemType== SOMITEM_ANY)
			{
			}
		}
		return 1;
	}

	int xbXml::processComplexType(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaComplexType * pType)
	{
		ISchemaModelGroup * pGroup;
		pType->get_contentModel(&pGroup);
		if(pGroup)processGroup(pDocument,pParentEle, pGroup); 
		return 1;
	}
	int xbXml::processSimpleType(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaType * pType)
	{
		BSTR bstr;
		pType->get_name(&bstr);
		pParentEle->setAttribute(L"type",(_variant_t)bstr);
		::SysFreeString(bstr);
		return 1;
	}
	int xbXml::GetSchemaXml(LPTSTR pSchema,xbXml &x)
	{
		IXMLDOMSchemaCollection2* pCollection = NULL;
		HRESULT hr = CoCreateInstance(CLSID_XMLSchemaCache40, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IXMLDOMSchemaCollection,
			(void**)&pCollection);
		if (!SUCCEEDED(hr))
			HRESULT hr = CoCreateInstance(CLSID_XMLSchemaCache30, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IXMLDOMSchemaCollection2,
			(void**)&pCollection);

		IXMLDOMElement* pDocEle;
		xbXml xs;
		xs.LoadXml(pSchema);
		xs.GetXmlDoc()->get_documentElement(&pDocEle);
		if(!pDocEle) return -1;
		BSTR bstr = NULL;
		pDocEle->get_xml(&bstr);
		::SysFreeString(bstr);
		hr = pCollection->add(L"http://www.hxsoft.com/schema",(_variant_t)xs.GetXmlDoc());
		if( SUCCEEDED( hr ) )
		{
			ISchema * pSchema=NULL;
			hr = pCollection->getSchema(L"http://www.hxsoft.com/schema",&pSchema);
			if(pSchema)
			{
				ISchemaItemCollection* pItems;
				hr = pSchema->get_elements(&pItems);
				ISchemaItem * pItem;
				long nLen;
				hr = pItems->get_length(&nLen);
				if(nLen > 0)
				{
					pItems->get_item(0,&pItem);
					x.LoadXml(L"<root/>");
					IXMLDOMElement * pNodeSchema;
					x.m_pXmlDoc->createElement(L"schema",&pNodeSchema);

					ISchemaElement * pSchemaEle;
					pItem->QueryInterface(IID_ISchemaElement,(void **) &pSchemaEle);
					processElement(x.GetXmlDoc(),pNodeSchema,pSchemaEle);
					pSchemaEle->Release();
					pItem->Release();

					IXMLDOMElement * pRoot ;
					x.m_pXmlDoc->get_documentElement(&pRoot);
					pRoot->appendChild(pNodeSchema,NULL);
					pNodeSchema->Release();
					pRoot->Release();
				}
				pItems->Release();
				pSchema->Release();
			}
		}else
		{
			ISupportErrorInfo * spInfo = NULL;
			pCollection->QueryInterface(IID_ISupportErrorInfo,(void **)&spInfo);
			if(spInfo)
			{
				hr = spInfo->InterfaceSupportsErrorInfo( IID_IXMLDOMSchemaCollection2);	
				if( SUCCEEDED( hr ) )
				{
					IErrorInfo * pError=NULL;
					::GetErrorInfo(0,&pError);
					BSTR bstr= NULL;
					if(pError)
					{
						pError->GetDescription(&bstr);
						if(bstr)::SysFreeString(bstr);
					}
					pError->Release();
				}
				spInfo->Release();
			}
		}
		pCollection->Release();
		return 1;
	}
}}
