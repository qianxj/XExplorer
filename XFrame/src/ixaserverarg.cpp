#include "StdAfx.h"
#include "..\include\IXAServerArg.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXAServerArg::IXAServerArg(void):pValue(NULL)
	{
		xml.LoadXml(_T("<args/>"));
	}

	IXAServerArg::~IXAServerArg(void)
	{
		if(pValue)::SysFreeString(pValue);
		//if(pValue)delete pValue;
	}

	int IXAServerArg::ClearArgs()
	{
		xml.LoadXml(_T("<args/>"));
		return 1;
	}

	int IXAServerArg::SetXml(LPTSTR pStrXml)
	{
		bool bRet = xml.LoadXml(pStrXml);
		return bRet?1:-1;
	}

	int IXAServerArg::RemoveArg(LPCTSTR pArgName)
	{
		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);

		TCHAR buf[255];
		_stprintf_s(buf,255,L"arg[@name='%s']",pArgName);

		IXMLDOMNode * pNode;
		pDocElement->selectSingleNode(buf,&pNode);
		if(pNode)pDocElement->removeChild(pNode,NULL);

		return 1;
	}

	int IXAServerArg::SetArg(LPCTSTR pArgName,_variant_t var,LPCTSTR pTyp)
	{
		IXMLDOMElement * pElement;
		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);

		TCHAR buf[255];
		_stprintf_s(buf,255,L"arg[@name='%s']",pArgName);

		IXMLDOMNode * pNode;
		pDocElement->selectSingleNode(buf,&pNode);
		if(pNode)
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		else
		{
			xml.m_pXmlDoc->createElement(L"arg",&pElement);
			pElement->setAttribute(L"name", (_variant_t)pArgName);
		}
		pElement->setAttribute(L"value",var);
		if(var.bstrVal && wcslen(var.bstrVal)> 1024*32)	pElement->put_text(var.bstrVal);

		if(!pTyp)
		{
			if( var.vt==VT_I1 ||var.vt==VT_I2 || var.vt==VT_I4 || var.vt==VT_I8 || var.vt==VT_INT ||
					var.vt==VT_UI1 ||var.vt==VT_UI2 || var.vt==VT_UI4 || var.vt==VT_UI8 || var.vt==VT_UINT ||
					var.vt==VT_UINT_PTR ||var.vt==VT_INT_PTR)
				pElement->setAttribute(L"type",(_variant_t)L"xs:integer");
			else if(var.vt==VT_BSTR ||var.vt==VT_LPSTR || var.vt==VT_LPWSTR)
				pElement->setAttribute(L"type",(_variant_t)L"xs:string");
			else if(var.vt==VT_R4 ||var.vt==VT_R8)
				pElement->setAttribute(L"type",(_variant_t)L"xs:double");
			else
			{
				pElement->setAttribute(L"type",(_variant_t)L"xs:other");
				pElement->setAttribute(L"typeex",(_variant_t)var.vt);
			}
		}else
			pElement->setAttribute(L"type",(_variant_t)pTyp);
		pDocElement->appendChild(pElement,NULL);
/*
		BSTR bstr;
		_variant_t var1;
		IXMLDOMElement * pElement1;
		xml.m_pXmlDoc->createElement(L"arg",&pElement1);
		pElement1->setAttribute(L"name", (_variant_t)pArgName);
		pElement1->put_text(var.bstrVal);
		pElement1->get_xml(&bstr);
		pElement1->Release();
		::SysFreeString(bstr);

		pElement->getAttribute(L"value",&var1);
		pElement->get_xml(&bstr);
		::SysFreeString(bstr);
		pDocElement->get_xml(&bstr);
		::SysFreeString(bstr);
*/
		if(pNode)pNode->Release();
		pElement->Release();
		pDocElement->Release();

		return 1;
	}
	int IXAServerArg::AddArg(LPCTSTR pArgName,_variant_t var,LPCTSTR pTyp)
	{
		return IXAServerArg::SetArg(pArgName,var,pTyp);
	}

	int IXAServerArg::SetArg(LPCTSTR pArgName,LPCTSTR pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::SetArg(pArgName,(_variant_t)pValue,pTyp);
	}
	int IXAServerArg::AddArg(LPCTSTR pArgName,LPCTSTR pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::AddArg(pArgName,(_variant_t)pValue,pTyp);
	}

	int IXAServerArg::SetArg(LPCTSTR pArgName,int pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::SetArg(pArgName,(_variant_t)pValue,pTyp);

	}
	int IXAServerArg::AddArg(LPCTSTR pArgName,int pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::AddArg(pArgName,(_variant_t)pValue,pTyp);
	}

	int IXAServerArg::SetArg(LPCTSTR pArgName,double pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::SetArg(pArgName,(_variant_t)pValue,pTyp);
	}
	int IXAServerArg::AddArg(LPCTSTR pArgName,double pValue,LPCTSTR pTyp)
	{
		return IXAServerArg::AddArg(pArgName,(_variant_t)pValue,pTyp);
	}

	LPCTSTR IXAServerArg::GetString()
	{
		if(pValue)::SysFreeString(pValue);
		xml.m_pXmlDoc->get_xml(&pValue);
		return pValue;
	}
	
	IXAServerArg * IXAServerArg::CreateInstance()
	{
		return new IXAServerArg();
	}
	int IXAServerArg::ReleaseInstance(IXAServerArg * pArgs)
	{
		if(pArgs) delete pArgs;
		return 1;
	}

	LPTSTR IXAServerArg::GetArgString(LPTSTR pArgName)
	{
		IXMLDOMElement * pElement;
		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);

		TCHAR buf[255];
		_stprintf_s(buf,255,L"arg[@name='%s']",pArgName);

		IXMLDOMNode * pNode;
		pDocElement->selectSingleNode(buf,&pNode);
		if(pNode)
		{
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			
			BSTR bstr = NULL;
			pElement->get_text(&bstr);
			if(bstr && bstr[0])
			{
				wchar_t * str =  _tcsdup(bstr);
				::SysFreeString(bstr);
				return str;
			}else
			{
				_variant_t var;
				pElement->getAttribute(L"value",&var);
				pElement->Release();
				if(var.bstrVal) return _tcsdup(var.bstrVal);
			}
		}
		return NULL;
	}

	int IXAServerArg::GetArgInt(LPTSTR pArgName)
	{
		LPTSTR pStr = GetArgString(pArgName);
		if(!pStr) return 0;
		int value = _ttol(pStr);
		delete pStr;
		return value;
	}

	bool IXAServerArg::GetArgBool(LPTSTR pArgName)
	{
		LPTSTR pStr = GetArgString(pArgName);
		if(!pStr) return 0;

		if(_tcsicmp(pStr,_T("true"))==0)
		{
			delete pStr;
			return true;
		}

		if(_tcsicmp(pStr,_T("false"))==0)
		{
			delete pStr;
			return false;
		}

		if(_tcsicmp(pStr,_T("yes"))==0)
		{
			delete pStr;
			return true;
		}

		if(_tcsicmp(pStr,_T("no"))==0)
		{
			delete pStr;
			return false;
		}

		int value = _ttol(pStr);
		delete pStr;
		return value != 0? true:false;
	}

	double IXAServerArg::GetArgDouble(LPTSTR pArgName)
	{
		LPTSTR pStr = GetArgString(pArgName);
		if(!pStr) return 0;

		LPTSTR pStrNext;
		double value = ::_tcstod(pStr,&pStrNext);
		delete pStr;
		return value;
	}
}}
