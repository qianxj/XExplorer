#include "StdAfx.h"
#include "..\include\XProjectSerializeSvr.hpp"
#include "xprojectdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	XProjectSerializeSvr::XProjectSerializeSvr(void)
	{
	}

	XProjectSerializeSvr::~XProjectSerializeSvr(void)
	{
	}
	int XProjectSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XProjectDataSvr * pProjectDataSvr = (XProjectDataSvr *)pDataSvr;

		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr	pNodeList;

		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodeListPtr pNTaskList;
		IXMLDOMNodeListPtr pTaskList;

		IXMLDOMNodePtr pNodeStyle;
		IXMLDOMNodePtr pNodeTask;
		IXMLDOMNodePtr pNodeNTask;

		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xproject")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;

		long	nLenStyle=-1;
		pSheetElement->selectNodes((_bstr_t)"stylesheet/item",&pStyleList);
		pStyleList->get_length(&nLenStyle);
		for(int i=0;i<nLenStyle;i++)
		{
			LPTSTR pStrClass;
			pStyleList->get_item(i,&pNodeStyle);
			pElement = xbXml::GetElement(pNodeStyle);
			pStrClass = xbXml::GetStringAttribe(pElement,_T("name"));
			if(!pStrClass) continue;

			BSTR bstrText;
			pElement->get_text(&bstrText);
			if(bstrText)
			{
				pProjectDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}


		if(!pProjectDataSvr->m_pTaskSvr)
		pProjectDataSvr->m_pTaskSvr = new XProjectTaskSvr; 
		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pProjectDataSvr->m_pTaskSvr->m_nIndexStyle = pProjectDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		_variant_t	val;
		pSheetElement->selectNodes((_bstr_t)"tasks/task",&pTaskList);
		long	len=-1;
		pTaskList->get_length(&len);
		for(int i=0;i<len;i++)
		{
			pTaskList->get_item(i,&pNodeTask);
			pElement = xbXml::GetElement(pNodeTask);

			TaskItem* pTaskItem = new  TaskItem;
			pElement->getAttribute(L"id",&val);
			pTaskItem->id =  val;
			pTaskItem->m_pStr = xbXml::GetStringAttribe(pElement,_T("desc"));
			
			pElement->getAttribute(L"sdate",&val);
			pTaskItem->sdate = val;
			pElement->getAttribute(L"edate",&val);
			pTaskItem->edate = val;

			pProjectDataSvr->m_pTaskSvr->m_Tasks.push_back(pTaskItem);

			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pTaskItem->m_nIndexStyle = pProjectDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pElement->Release();
			
			long	nLenTask=0;
			pNodeTask->selectNodes(L"ntask",&pNTaskList);
			pNTaskList->get_length(&nLenTask);
			for(int j=0;j<nLenTask;j++)
			{
				pNTaskList->get_item(j,&pNodeNTask);
				pElement = xbXml::GetElement(pNodeNTask);
				
				LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
				if(pStrClass)
				{
					pTaskItem->m_nIndexStyle = pProjectDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
					if(pStrClass)delete pStrClass;
				}
				
				pElement->getAttribute(L"refid",&val); 
				if(val.bstrVal)
					pTaskItem->m_nextTasks.push_back((int)val);
				pElement->Release();
			}
		}
		return 1;
	}
	int  XProjectSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xproject");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();
		return 1;
	}
}}}}
