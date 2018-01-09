#include "StdAfx.h"
#include "..\include\XXmlContentSvr.hpp"
#include "msxml2.h"
#include <comdef.h>
#include <time.h>
#include <atlbase.h>
#include "XDataView.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XXmlContentSvr::XXmlContentSvr(void):m_pLists(NULL),m_pRoot(NULL),m_pStrRoot(NULL),
		m_pXPaths(new std::vector<LPTSTR >()),m_pUri(NULL),m_pUrl(NULL),
		m_pCols(new std::vector<LPTSTR >()),m_pvcTreeData(NULL),m_pBuffers(NULL) ,m_pDDLBData(NULL),m_pRows(NULL),
		FShowOrigin(false),m_pQueryStr(NULL),m_pTotalStr(NULL)
	{
	}

	XXmlContentSvr::~XXmlContentSvr(void)
	{
		if(m_pLists)m_pLists->Release();
		if(m_pRoot)m_pRoot->Release();
		if(m_pStrRoot)delete m_pStrRoot;
		if(m_pXPaths)
		{
			for(int i=0;i<(int)m_pXPaths->size();i++)
			{
				if((*m_pXPaths)[i])delete (*m_pXPaths)[i];
			}
			delete m_pXPaths;
		}
		if(m_pCols)
		{
			for(int i=0;i<(int)m_pCols->size();i++)
			{
				if((*m_pCols)[i])delete (*m_pCols)[i];
			}
			delete m_pCols;
		}
		if(m_pUri)delete m_pUri;
		if(m_pUrl)delete m_pUrl;
		if(m_pvcTreeData)delete m_pvcTreeData;
		if(m_pBuffers)delete m_pBuffers;
		if(m_pDDLBData)delete m_pDDLBData;
		if(m_pQueryStr) m_pQueryStr->clear();
		if(m_pTotalStr)
		{
			m_pTotalStr->clear();
			delete m_pTotalStr;
		}

		
		if(m_pRows)delete m_pRows;
	}

	XAPI STDMETHODIMP_(IXMLDOMNode *) XXmlContentSvr::GetItemNode(int ACol,int ARow)
	{
		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);
		//2017.8.23 if(_tcsicmp((*m_pXPaths)[ACol - 1],_T("."))==0||_tcsicmp((*m_pXPaths)[ACol - 1],_T("name()"))==0)
		if(_tcsicmp((*m_pXPaths)[ACol],_T("."))==0||_tcsicmp((*m_pXPaths)[ACol],_T("name()"))==0)
		{
			return pNode;	
		}else
		{
			//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
			pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
			pNode->Release();
		}

		return pNodeEx;
	}

	XAPI STDMETHODIMP_(IXMLDOMElement *) XXmlContentSvr:: GetRowElement(int ARow)
	{
		IXMLDOMNode * pNode;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);

		if(pNode)
		{
			pElement = xbXml::GetElement(pNode);
			pNode->Release();
		}
		return pElement;
	}

	XAPI STDMETHODIMP_(LPCTSTR) XXmlContentSvr::GetItemString(LPTSTR pColName,int ARow)
	{
		if(ARow < 1 || ARow > GetRowCount() || !pColName) return NULL;

		int i=0;
		for(i=0;i<(int)m_pCols->size();i++)
		{
			if(_tcsicmp(pColName,(*m_pCols)[i])==0)break;
		}
		if(i>=(int)m_pCols->size())
		{
			IXMLDOMNode * pNode;
			IXMLDOMNode * pNodeEx;
			IXMLDOMElement * pElement = NULL;
			LPCTSTR	pData = NULL;
			m_pLists->get_item(ARow - 1,&pNode);
			if(!pNode) return NULL;

			BSTR xx;
			pNode->get_xml(&xx);
			::SysFreeString(xx);

			if(_tcsicmp(pColName,_T("name()"))==0)
			{
				BSTR bstr;
				pElement = xbXml::GetElement(pNode);
				pElement->get_tagName(&bstr);
				if(bstr)
				{
					if(!m_pBuffers)m_pBuffers = new set<wstring>;
					m_pBuffers->insert(bstr);

					pData = (*m_pBuffers->find(bstr)).c_str(); //Hxsoft::XUtil::tcsdupFromBstr(bstr);
					::SysFreeString(bstr);
				}
				pNode->Release();
				pElement->Release();
				return pData;
			}else
			{
				pNode->selectSingleNode(pColName,&pNodeEx);
				if(pNodeEx)
				{
					BSTR bstr;
					pNodeEx->get_text(&bstr);
					if(bstr)
					{
						if(!m_pBuffers)m_pBuffers = new set<wstring>;
						m_pBuffers->insert(bstr);

						pData = (*m_pBuffers->find(bstr)).c_str(); //Hxsoft::XUtil::tcsdupFromBstr(bstr);
						::SysFreeString(bstr);
					}
					pNodeEx->Release();
					return pData;
				}
				pNode->Release();
				return NULL;
			}
		}
		//return GetItemString(i+1,ARow);
		return GetItemString(i,ARow);
	}

	XAPI STDMETHODIMP_(LPCTSTR) XXmlContentSvr::GetItemString(int ACol,int ARow)
	{
		LPCTSTR	pData = NULL;
		if(ARow < 1 || ACol < 0) return NULL;
		if(ARow > GetRowCount()) return NULL;
		if(ACol >= (int)m_pCols->size()) return NULL;

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);

		//2017.8.23 if(ACol >0  && _tcsicmp((*m_pXPaths)[ACol - 1],_T("name()"))==0)
		if(ACol >0  && _tcsicmp((*m_pXPaths)[ACol],_T("name()"))==0)
		{
			BSTR bstr;
			pElement = xbXml::GetElement(pNode);
			pElement->get_tagName(&bstr);
			if(bstr)
			{
				if(!m_pBuffers)m_pBuffers = new set<wstring>;
				m_pBuffers->insert(bstr);

				pData = (*m_pBuffers->find(bstr)).c_str(); //Hxsoft::XUtil::tcsdupFromBstr(bstr);
				::SysFreeString(bstr);
			}
			pNode->Release();
			pElement->Release();
		}else
		{
			if(ACol==0)
			{
				pNode->selectSingleNode(L"@guid",&pNodeEx);
			}else
			{
				//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
				pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
			}
			if(pNodeEx)
			{
				BSTR bstr;
				pNodeEx->get_text(&bstr);
				if(bstr)
				{
					if(!m_pBuffers)m_pBuffers = new set<wstring>;
					m_pBuffers->insert(bstr);

					pData = (*m_pBuffers->find(bstr)).c_str(); //Hxsoft::XUtil::tcsdupFromBstr(bstr);
					::SysFreeString(bstr);
				}
				pNodeEx->Release();
			}
			pNode->Release();
		}

		return pData;
	}
	
	XAPI STDMETHODIMP_(LPCTSTR) XXmlContentSvr::GetItemDisplayString(int ACol,int ARow)
	{
		LPCTSTR	pData = NULL;
		if(ARow < 1 || ACol < 1) return NULL;
		if(ARow > GetRowCount()) return NULL;
		if(ACol >= (int)m_pCols->size()) return NULL;

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);

		//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
		pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
		
		if(pNodeEx)
		{
			pElement = xbXml::GetElement(pNodeEx);
			if(pElement)
			{
				BSTR bstr;
				_variant_t v;
				pElement->getAttribute(L"_displaystring",&v);
				if(v.bstrVal)
				{
					bstr = ::SysAllocString(v.bstrVal);
					if(!m_pBuffers)m_pBuffers = new set<wstring>;
					m_pBuffers->insert(bstr);

					pData = (*m_pBuffers->find(bstr)).c_str(); //Hxsoft::XUtil::tcsdupFromBstr(bstr);
					::SysFreeString(bstr);
				}
			}
			pNodeEx->Release();
		}
		pNode->Release();

		return pData;
	}

	XAPI STDMETHODIMP_(LPCTSTR) XXmlContentSvr::GetItemDisplayString(LPTSTR pColName,int ARow)
	{
		int i=0;
		for(i=0;i<(int)m_pCols->size();i++)
		{
			if(_tcsicmp(pColName,(*m_pCols)[i])==0)break;
		}
		if(i<(int)m_pCols->size())
		{
			//2017.8.23 return GetItemDisplayString(i+1,ARow);
			return GetItemDisplayString(i,ARow);
		}
		return NULL;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemDisplayString(int ACol,int ARow, LPTSTR pData)
	{
		if(ARow < 1 || ACol < 0) return NULL;
		if(ARow > GetRowCount()) return NULL;
		if(ACol > (int)m_pCols->size()) return NULL;

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);

		//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
		pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
		if(!pNodeEx)
		{
			SetItemString(ACol, ARow, L"");
			//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
			pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
		}
		if(pNodeEx)
		{
			//2017.8.23 if((*m_pXPaths)[ACol - 1][0] != '@')
			if((*m_pXPaths)[ACol][0] != '@')
			{
				IXMLDOMElement * pElementEx;
				pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **) &pElementEx);
				_variant_t var;
				pElementEx->getAttribute(L"update.origindisplaystring",&var);
				if(!var.bstrVal)
				{
					_variant_t bstr;
					pElementEx->getAttribute(L"_displaystring", &bstr);
					if(bstr.bstrVal)
						pElementEx->setAttribute(L"update.origindisplaystring",bstr);
					else
						pElementEx->setAttribute(L"update.origindisplaystring",(_variant_t)L"");
				}
				pElementEx->setAttribute(L"update.modify",(_variant_t)1);
				pElementEx->Release();
			}
			pElement = xbXml::GetElement(pNodeEx);
			if(pElement)
				pElement->setAttribute(L"_displaystring",(_variant_t)pData);

			pNodeEx->Release();
		}
		pNode->Release();
		return true;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemDisplayStringEx(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		if(ARow < 1 || !pColName) return false;
		if(ARow > GetRowCount()) return false;

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);
		if(!pNode) return false;

		pNode->selectSingleNode(pColName,&pNodeEx);
		if(!pNodeEx) 
		{
			SetItemStringEx(pColName, ARow, L"");
			pNode->selectSingleNode(pColName,&pNodeEx);
		}
		if(pNodeEx)
		{
			pElement = xbXml::GetElement(pNodeEx);
			if(pElement)
			{
				if(pColName[0] != '@')
				{
					_variant_t var;
					pElement->getAttribute(L"update.origindisplaystring",&var);
					if(!var.bstrVal)
					{
						_variant_t bstr;
						pElement->getAttribute(L"_displaystring", &bstr);
						if(bstr.bstrVal)
							pElement->setAttribute(L"update.origindisplaystring",bstr);
						else
							pElement->setAttribute(L"update.origindisplaystring",(_variant_t)L"");
					}
				}
				pElement->setAttribute(L"update.modify",(_variant_t)1);
				pElement->setAttribute(L"_displaystring",(_variant_t)pData);
			}

			pNodeEx->Release();
		}
		pNode->Release();
		return true;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemDisplayString(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		int i=0;
		for(i=0;i<(int)m_pCols->size();i++)
		{
			if(_tcsicmp(pColName,(*m_pCols)[i])==0)break;
		}
		if(i<(int)m_pCols->size())
		{
			//2017.8.23 return SetItemDisplayString(i+1,ARow,pData);
			return SetItemDisplayString(i,ARow,pData);
		}
		return false;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::InsertRow(int nRow)
	{
		IXMLDOMNodePtr gNode = NULL;
		IXMLDOMNodePtr pNode = NULL;
		IXMLDOMElementPtr Node = NULL;
		IXMLDOMNodePtr pNodeEx = NULL;
		IXMLDOMElementPtr NewElement;
		IXMLDOMNodeListPtr pTempNodes = NULL;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodePtr  NewNode;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;


		BSTR bstr;
		m_xbXml.m_pXmlDoc->get_xml(&bstr);
		::SysFreeString(bstr);

		IXMLDOMElementPtr pDocElement;
		m_xbXml.m_pXmlDoc->get_documentElement(&pDocElement);
		wstring StrPath = this->GetXPath();
		int nPosLast = -1;
		if(!pDocElement)
		{
			wstring strRoot;
			nPosLast = (int)StrPath.find('/');
			if(nPosLast < 0) return false;

			strRoot = StrPath.substr(0,nPosLast);
			StrPath = StrPath.substr(nPosLast + 1);
			if(strRoot.c_str()[0]=='(')
				StrPath = (wstring)L"("+ StrPath;

			m_xbXml.m_pXmlDoc->createElement((BSTR)strRoot.c_str(),&pDocElement);
			m_xbXml.m_pXmlDoc->appendChild(pDocElement,NULL);
		}else
		{
			wstring strRoot;
			nPosLast = (int)StrPath.find('/');
			if(nPosLast < 0) return false;

			strRoot = StrPath.substr(0,nPosLast);
			StrPath = StrPath.substr(nPosLast + 1);
			if(strRoot.c_str()[0]=='(')
				StrPath = (wstring)L"("+ StrPath;
		}
		gNode = pDocElement;

		wstring StrNode;
		wstring StrName;
		//if no exists pNode then create parent Node
		nPosLast = (int)StrPath.find_last_of(_T("/"));
		if(nPosLast >0)
		{
			StrNode = StrPath.substr(nPosLast + 1);
			StrPath = StrPath.substr(0,nPosLast);
		}
		else
		{
			StrNode= StrPath;
			StrPath = _T("");
			if(StrNode[0]=='\0') return true;
		}

		pNode = NULL;
		gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
		if(pNode)
		{
			pNodeEx = pNode;
		}
		else
		{
			nPosLast = (int)StrPath.find_last_of(_T("/"));
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrPath.compare(_T(""))!=0)
				{
					pNode = NULL;
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
					if(pNode)break;
					nPosLast = (int)StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					m_xbXml.m_pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
					vcElements.push_back(pNewElement);
				}
				if(StrPath.compare(_T(""))!=0)
					pNode = gNode;
				else
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);

				for(int i=(int)vcElements.size() - 1;i>1;i--)
				{
					vcElements[i]->appendChild(vcElements[i - 1],NULL);
				}
				if(vcElements.size() > 0)
				{
					pNode->appendChild(vcElements[vcElements.size() - 1],NULL);
					pNodeEx = vcElements[0];
				}else
					pNodeEx = pNode;
			}else
			{
				if(StrPath[0] !='\0')
				{
					StrName = StrPath.c_str();
					//if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					//if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					StrPath = _T("");
					gNode->selectSingleNode((LPTSTR)StrName.c_str(),&pNode);
					if(!pNode)
					{
						m_xbXml.m_pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
						gNode->appendChild(pNewElement,NULL);
						pNodeEx = pNewElement; 
					}else
						pNodeEx = pNode;
				}else
					pNodeEx = gNode;
			}
		}
		if(!pNodeEx) return -1;
				
		if(StrNode.find(_T("["))>0) StrNode = StrNode.substr(0,StrNode.find(_T("[")));
		m_xbXml.m_pXmlDoc->createElement((BSTR)StrNode.c_str(),&Node);

		//create not exits column node
		for(int i=0;i<(int)this->m_pCols->size();i++)
		{
			wstring path = (*m_pXPaths)[i];
			if(path[0]=='/' &&path[1]=='/')continue; 
			if(path[0]=='/')path = path.substr(1);
			int pos = (int)path.find(_T("/"));
			if(pos >0)
			{
				StrName = path.substr(0,pos);
				path = path.substr(pos +1);
			}else
			{
				StrName = path;
				path = _T("");
			}
			//if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
			//if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

			Node->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
			long nTempLen = 0;
			pTempNodes->get_length(&nTempLen);
			if(nTempLen < 1)
			{
				if(StrName[0]=='@')
				{
					StrName = StrName.substr(1);
					((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
					continue;
				}else
				{
					m_xbXml.m_pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
					Node->appendChild(NewElement,NULL);
				}
			}else
			{
				if(StrName[0]=='@')continue;
				pTempNodes->get_item(0,&NewNode);
				NewElement = NewNode;
			}
			while(path.compare(_T(""))!=0)
			{
				int pos = (int)path.find(_T("/"));
				if(pos >=0)
				{
					StrName =  path.substr(0,pos);
					path = path.substr(pos + 1);
				}else
				{
					StrName = path;
					path = _T("");
				}
				NewElement->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
				pTempNodes->get_length(&nTempLen);
				if(nTempLen < 1)
				{
					if(StrName[0]=='@')
					{
						StrName = StrName.substr(1);
						((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
						continue;
					}else
					{
						m_xbXml.m_pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
						NewElement->appendChild(NewElement1,NULL);
						NewElement = NewElement1;
					}
				}else
				{
					if(StrName[0]=='@')continue;
					pTempNodes->get_item(0,&NewNode);
					NewElement = NewNode;
				}
			}
		}
		//end create not exists column

		bool bAppend = false;
		if(nRow<=0)
		{
			pNodeEx->appendChild(Node,NULL);
			bAppend = true;
		}
		else
		{
			long nLen;
			m_pLists->get_length(&nLen);
			if(nRow > nLen)
			{
				pNodeEx->appendChild(Node,NULL);
				bAppend = true;
			}
			else
			{
				m_pLists->get_item(nRow - 1,&pNode);
				VARIANT v;
				v.vt = VT_DISPATCH;
				v.pdispVal = (IDispatch*)pNode;

				HRESULT hr = pNodeEx->insertBefore(Node,v,NULL);
			}
		}

		IXMLDOMElement* pElement1;
		Node->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
		pElement1->setAttribute(L"update.new",(_variant_t)1);
		pElement1->Release();
		
		SetXPath(m_pStrRoot);


		/*BSTR bstr;
		m_xbXml.m_pXmlDoc->get_xml(&bstr);
		::SysFreeString(bstr);*/

		_variant_t var;
		int trow = nRow;
		if(bAppend)trow = this->GetRowCount();
		IXMLDOMElement* pElement = GetRowElement(trow);
		if(pElement)
		{
			pElement->getAttribute(L"guid",&var);

			if(!var.bstrVal || var.bstrVal[0]=='\0')
			{
				GUID guid;
				int k=1;
				CoCreateGuid(&guid);
				TCHAR pStrGuid[64];
				StringFromGUID2(guid,pStrGuid,64);
				((IXMLDOMElementPtr)pElement)->setAttribute(L"guid",(_variant_t)pStrGuid);
			}else
			{
				((IXMLDOMElementPtr)pElement)->setAttribute(L"guid",var);
			}
			pElement->Release();
		}
		
		if(bAppend)
			return this->GetRowCount();
		else
			return nRow;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::DeleteRow(int nRow)
	{
		long nLen;
		if(nRow < 1) return false;

		if(m_pTotalStr) m_pTotalStr->clear();

		m_pLists->get_length(&nLen);
		if(nRow > nLen) return false;

		IXMLDOMNodePtr pNode = NULL;
		IXMLDOMNodePtr pNodeEx = NULL;


		m_pLists->get_item(nRow - 1,&pNode);

		
		pNode->get_parentNode(&pNodeEx);
		if(!pNodeEx) return false;

		IXMLDOMElement * pElement;
		_variant_t var;
		pNode->QueryInterface(IID_IXMLDOMElement,(void **) &pElement);
		pElement->getAttribute(L"update.new",&var);
		if(var.bstrVal && var.bstrVal[0]=='1')
		{
			pElement->Release();
			pNodeEx->removeChild(pNode,NULL);
		}else
			pElement->setAttribute(L"update.delete",(_variant_t)1);

		SetXPath(m_pStrRoot);
		return false;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemString(int ACol,int ARow, TCHAR * pData)
	{
		if(ARow < 1||ACol < 1) return false;
		if(ARow > GetRowCount()) return false;
		if(ACol > (int)m_pCols->size()) return false;

		if(m_pTotalStr && m_pTotalStr->find((std::wstring)L"sum:"+(*m_pCols)[ACol])!=m_pTotalStr->end())
		{
			m_pTotalStr->erase(m_pTotalStr->find((std::wstring)L"sum:"+(*m_pCols)[ACol]));
		}
		if(m_pTotalStr && m_pTotalStr->find((std::wstring)L"avg:"+(*m_pCols)[ACol ])!=m_pTotalStr->end())
		{
			m_pTotalStr->erase(m_pTotalStr->find((std::wstring)L"avg:"+(*m_pCols)[ACol]));
		}

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);

		//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
		pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
		/*add  2012-12-17 grid中没有列则xml新增列或属性*/
		if(!pNodeEx)
		{
			IXMLDOMElement * pElementNew = NULL;
			//2017.8.23 if((*m_pXPaths)[ACol - 1][0]=='@')
			if((*m_pXPaths)[ACol][0]=='@')
			{
				//2017.8.23 wstring StrName = (*m_pXPaths)[ACol - 1];
				wstring StrName = (*m_pXPaths)[ACol];
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementNew);
				pElementNew->setAttribute((LPTSTR)StrName.substr(1).c_str(),(_variant_t)pData);
			}
			else
			{
				xbXml xml;
				xml.LoadXml(L"<root/>");
				IXMLDOMDocument2 * pXmlDoc = xml.GetXmlDoc();
				IXMLDOMNode * pNodeNew;
				//2017.8.23 pXmlDoc->createElement((*m_pXPaths)[ACol - 1], &pElementNew);
				pXmlDoc->createElement((*m_pXPaths)[ACol ], &pElementNew);
				pElementNew->cloneNode(VARIANT_TRUE,&pNodeNew);
				pNode->appendChild(pNodeNew, NULL);
				pNodeNew->Release();
			}
			//2017.8.23 pNode->selectSingleNode((*m_pXPaths)[ACol - 1],&pNodeEx);
			pNode->selectSingleNode((*m_pXPaths)[ACol],&pNodeEx);
			pElementNew->Release();
		}
		/*end add*/
		if(pNodeEx)
		{
			//2017.8.23 if((*m_pXPaths)[ACol - 1][0] != '@')
			if((*m_pXPaths)[ACol][0] != '@')
			{
				IXMLDOMElement * pElementEx;
				pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **) &pElementEx);
				_variant_t var;
				pElementEx->getAttribute(L"update.origin",&var);
				if(!var.bstrVal)
				{
					BSTR bstr;
					pElementEx->get_text(&bstr);
					if(bstr)
						pElementEx->setAttribute(L"update.origin",(_variant_t)bstr);
					else
						pElementEx->setAttribute(L"update.origin",(_variant_t)L"");
				}
				pElementEx->setAttribute(L"update.modify",(_variant_t)1);
				pElementEx->Release();
			}
			pNodeEx->put_text(pData);
			pNodeEx->Release();
		}
		pNode->QueryInterface(IID_IXMLDOMElement,(void **) &pElement);
		pElement->setAttribute(L"update.modify",(_variant_t)1);
		pElement->Release();

		pNode->Release();

		return true;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemString(LPTSTR pColName,int ARow, TCHAR * pData)
	{
		int i=0;
		for(i=0;i<(int)m_pCols->size();i++)
		{
			if(_tcsicmp(pColName,(*m_pCols)[i])==0)break;
		}
		if(i>=(int)m_pCols->size()) return NULL;
		//return SetItemString(i+1,ARow,pData);
		return SetItemString(i,ARow,pData);
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemStringEx(LPTSTR pColName,int ARow, TCHAR * pData)
	{
		if(ARow < 1 || !pColName) return false;
		if(ARow > GetRowCount()) return false;

		if(m_pTotalStr && m_pTotalStr->find((std::wstring)L"sum:"+pColName)!=m_pTotalStr->end())
		{
			m_pTotalStr->erase(m_pTotalStr->find((std::wstring)L"sum:"+pColName));
		}
		if(m_pTotalStr && m_pTotalStr->find((std::wstring)L"avg:"+pColName)!=m_pTotalStr->end())
		{
			m_pTotalStr->erase(m_pTotalStr->find((std::wstring)L"avg:"+pColName));
		}

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		IXMLDOMElement * pElement = NULL;
		m_pLists->get_item(ARow - 1,&pNode);
		if(!pNode) return NULL;

		pNode->selectSingleNode(pColName,&pNodeEx);
		if(!pNodeEx)
		{
			IXMLDOMElement * pElementNew = NULL;
			if(pColName[0] =='@')
			{
				wstring StrName = pColName;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementNew);
				pElementNew->setAttribute((LPTSTR)StrName.substr(1).c_str(),(_variant_t)pData);
			}
			else
			{
				xbXml xml;
				xml.LoadXml(L"<root/>");
				IXMLDOMDocument2 * pXmlDoc = xml.GetXmlDoc();
				IXMLDOMNode * pNodeNew;
				pXmlDoc->createElement(pColName, &pElementNew);
				pElementNew->cloneNode(VARIANT_TRUE,&pNodeNew);
				pNode->appendChild(pNodeNew, NULL);
				pNodeNew->Release();
			}
			pNode->selectSingleNode(pColName,&pNodeEx);
			pElementNew->Release();
		}
		if(pNodeEx)
		{
			if(pColName[0] != '@')
			{
				IXMLDOMElement * pElementEx;
				pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **) &pElementEx);
				_variant_t var;
				pElementEx->getAttribute(L"update.origin",&var);
				if(!var.bstrVal)
				{
					BSTR bstr;
					pElementEx->get_text(&bstr);
					if(bstr)
						pElementEx->setAttribute(L"update.origin",(_variant_t)bstr);
					else
						pElementEx->setAttribute(L"update.origin",(_variant_t)L"");
				}
				pElementEx->setAttribute(L"update.modify",(_variant_t)1);
				pElementEx->Release();
			}
			pNodeEx->put_text(pData);
			pNodeEx->Release();
		}
		pNode->QueryInterface(IID_IXMLDOMElement,(void **) &pElement);
		pElement->setAttribute(L"update.modify",(_variant_t)1);
		pElement->Release();

		pNode->Release();

		return true;
	}

	XAPI STDMETHODIMP_(void *) XXmlContentSvr::GetItemData(int ACol,int ARow)
	{
		return NULL;
	}
	XAPI STDMETHODIMP_(bool)  XXmlContentSvr::SetItemData(int ACol,int ARow,void * pData)
	{
		return NULL;
	}
	XAPI STDMETHODIMP_(IContentSvr::eDataType) XXmlContentSvr::GetItemType(int ACol,int ARow)
	{
		return IContentSvr::none;
	}
	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetItemType(int ACol,int ARow, IContentSvr::eDataType dt)
	{
		return IContentSvr::none;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::GetColCount()
	{
		//2017.8.23 return (int)m_pCols->size();
		return ((int)m_pCols->size()) - 1;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::GetRowCount()
	{
		long nLen = 0;
		if(!m_pLists) return 0;
		m_pLists->get_length(&nLen);
		return nLen;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::Update()
	{
		return 0;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::Retrieve(LPTSTR pXmlStr)
	{
		int nLen;
		BYTE * pData;
		if(m_pUri)
		{
			if(xfUriLoader::ProcessCmd(m_pUrl,m_pUri,pXmlStr,pData,nLen)!=1) return 0;
		}
		else
		{
			if(!m_pUrl) return 0;
			if(xfUriLoader::Load(m_pUrl,m_pUri,pXmlStr,pData,nLen,false)!=1)return 0;
		}

		if(pData)
		{
			if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
			{
				LoadXml((LPTSTR)(pData + 2));
				xfUriLoader::FreeData(pData);
				return this->GetRowCount();
			}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
			{
				int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
				psz[_convert]='\0';
				xfUriLoader::FreeData(pData);
				LoadXml(psz);
				delete psz;
				return this->GetRowCount();
			}
			if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
			{
				int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
				psz[_convert]='\0';
				xfUriLoader::FreeData(pData);
				LoadXml(psz);
				delete psz;
				return this->GetRowCount();
			}else //should utf-16
			{
				LoadXml((LPTSTR)pData);
				xfUriLoader::FreeData(pData);
				return this->GetRowCount();
			}
		}
		return this->GetRowCount();
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::Load(LPTSTR pStrFile)
	{
		bool bResult = m_xbXml.Load(pStrFile);
		if(m_pStrRoot)SetXPath(m_pStrRoot);
		if(m_pRoot)
		{
			m_pRoot->Release();
			m_pRoot = NULL;
		}
		return bResult;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::GetContent(BSTR &bstr)
	{
		m_xbXml.m_pXmlDoc->get_xml(&bstr);
		return 1;
	}

	XAPI STDMETHODIMP_(LPTSTR) XXmlContentSvr::GetContent()
	{
		BSTR bstr;
		m_xbXml.m_pXmlDoc->get_xml(&bstr);
		return _tcsdup(bstr);
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::GetContent(BSTR &bstr,LPTSTR pGuid)
	{
		IXMLDOMElement* pElement;
		m_xbXml.m_pXmlDoc->get_documentElement(&pElement);
		pElement->setAttribute(L"guid",(_variant_t)pGuid);
		pElement->Release();
		m_xbXml.m_pXmlDoc->get_xml(&bstr);
		return 1;
	}


	XAPI STDMETHODIMP_(bool) XXmlContentSvr::Load(IXMLDOMElement* pElement)
	{
		if(m_pQueryStr && (int)m_pQueryStr->size() > 0)
		{
			for(int i=0; i<(int)m_pQueryStr->size(); i++)
			{
				(*m_pQueryStr)[i].clear();
			}
			m_pQueryStr->clear();
		}
		if(m_pTotalStr) m_pTotalStr->clear();

		m_pvcTreeData = NULL;
		BSTR bstr;
		pElement->get_xml(&bstr);
		if(bstr)
		{
			bool bResult = LoadXml(bstr);
			::SysFreeString(bstr);
			return bResult;
		}
		return false;
	}

	XAPI STDMETHODIMP_(int) XXmlContentSvr::Reset()
	{
		this->LoadXml(L"<data/");
		return 1;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::LoadXml(LPTSTR pStrXml)
	{
		bool bResult = m_xbXml.LoadXml(pStrXml);
		if(m_pStrRoot)SetXPath(m_pStrRoot);
		if(m_pRoot)
		{
			m_pRoot->Release();
			m_pRoot = NULL;
		}
		return bResult;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetXPath(LPTSTR pXPath)
	{
		if(m_pStrRoot && pXPath && _tcscmp(m_pStrRoot,pXPath) != 0)//m_pStrRoot != pXPath)
			//delete m_pStrRoot;
			m_pStrRoot = NULL;
		if(m_pLists)m_pLists->Release();
		m_pStrRoot = pXPath;

		BSTR bstr = ::SysAllocString(pXPath);
		TCHAR* pCondition = L"(count(@update.delete)=0 or @update.delete='0')";
		LPTSTR pStr = new TCHAR[0x8000]; //new TCHAR[_tcslen(bstr) + _tcslen(pCondition) + 17] ;
		pStr[0] = 0;

		if(bstr[_tcslen(bstr) -1]==']')
		{
			if(_tcslen(bstr)) bstr[_tcslen(bstr) - 1] = 0;
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			if(!this->FShowOrigin)
			{
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L" and ");
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
			}
			else
				_tcscat_s(pStr,_tcslen(pStr) + 2,L"]");	
		}else
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			if(!this->FShowOrigin)
			{
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"[");
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
				_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
			}
		}
		if(m_pQueryStr && (int)m_pQueryStr->size() > 0)
		{
			if(pStr[_tcslen(pStr) - 1] == ']')
				pStr[_tcslen(pStr) - 1] = 0;
			else
				_tcscat_s(pStr,_tcslen(pStr) + 5,L"[1=1");
			for(int i=0; i<(int)m_pQueryStr->size(); i++)
			{
				std::vector<LPTSTR> array1 = (*m_pQueryStr)[i];
				if(array1[0] && array1[0][0]=='*' && array1[0][1]==0)
				{
					LPTSTR pFilerStr = array1[1];
					_tcscat_s(pStr,_tcslen(pStr) + 6,L" and ");
					_tcscat_s(pStr,_tcslen(pStr) + 6,L"(1=2 ");
					for(int m=1;m<(int)m_pCols->size();m++)
					{
						_tcscat_s(pStr,_tcslen(pStr) + 6,L" or ");
						if((*m_pCols)[m] && (*m_pCols)[m][0]=='@')
						{
							_tcscat_s(pStr,_tcslen(pStr) + _tcslen((*m_pCols)[m]) + 1,(*m_pCols)[m]);
							_tcscat_s(pStr,_tcslen(pStr) + 2,L"'");
							_tcscat_s(pStr,_tcslen(pStr) + _tcslen(pFilerStr) + 1,pFilerStr);
							_tcscat_s(pStr,_tcslen(pStr) + 2,L"'");
						}else
						{
							_stprintf_s(pStr+_tcslen(pStr),1024,_T("(contains(%s, '%s') or (count(%s/@_displaystring)=1 and contains(%s/@_displaystring,'%s')))") ,(*m_pCols)[m], pFilerStr, (*m_pCols)[m], (*m_pCols)[m],pFilerStr);
						}
					}
					_tcscat_s(pStr,_tcslen(pStr) + 2,L")");
				}
				else
				{
					for(int j=1; j<(int)array1.size(); j++)
					{
						LPTSTR pFilerStr = array1[j];
						if(!pFilerStr) continue;
						_tcscat_s(pStr,_tcslen(pStr) + 6,L" and ");
						_tcscat_s(pStr,_tcslen(pStr) + _tcslen(pFilerStr) + 1, pFilerStr);
					}
				}
			}
			_tcscat_s(pStr,_tcslen(pStr) + 2,L"]");		
		}

		BSTR bstr1;
		m_xbXml.m_pXmlDoc->get_xml(&bstr1);
		::SysFreeString(bstr1);

		if(m_pRoot)
			m_pRoot->selectNodes(pStr,&m_pLists);
		else
			m_xbXml.m_pXmlDoc->selectNodes(pStr,&m_pLists);
		delete pStr;
		::SysFreeString(bstr);
		return true;
	}

	int XXmlContentSvr::ResetUpdateStatus()
	{
		BSTR bstr = ::SysAllocString(m_pStrRoot);
		IXMLDOMNodeList * pLists;
		IXMLDOMNode * pNode;
		IXMLDOMNode * pParentNode;
		IXMLDOMElement * pElement;
		long nLen;
		TCHAR* pCondition;
		LPTSTR pStr ;

		bool barray = false;
		if(bstr[_tcslen(bstr) -1]==']')
		{
			if(_tcslen(bstr)) bstr[_tcslen(bstr) - 1] = 0;
			barray = true;
		}

		//remove deleted row
		pCondition = L"@update.delete='1'";
		pStr = new TCHAR[_tcslen(bstr) + _tcslen(pCondition) + 17] ;
		pStr[0] = 0;

		if(barray)
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L" and ");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}else
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"[");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}

		if(m_pRoot)
			m_pRoot->selectNodes(pStr,&pLists);
		else
			m_xbXml.m_pXmlDoc->selectNodes(pStr,&pLists);
		delete pStr;

		pLists->get_length(&nLen);

		for(int i=0;i<nLen;i++)
		{
			pLists->get_item(i,&pNode);
			pNode->get_parentNode(&pParentNode);
			pParentNode->removeChild(pNode,NULL);
			pNode->Release();
			pParentNode->Release();
		}

		//reset new
		pCondition = L"(@update.new='1' and @update.modify='1')";
		pStr = new TCHAR[_tcslen(bstr) + _tcslen(pCondition) + 17] ;
		pStr[0] = 0;

		if(barray)
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L" and ");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}else
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"[");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}

		if(m_pRoot)
			m_pRoot->selectNodes(pStr,&pLists);
		else
			m_xbXml.m_pXmlDoc->selectNodes(pStr,&pLists);
		delete pStr;

		pLists->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pLists->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);

			pElement->removeAttribute(L"update.new");
			pElement->removeAttribute(L"update.modify");
			pElement->Release();
			pNode->Release();
		}

		//reset modify
		pCondition = L"@update.modify='1'";
		pStr = new TCHAR[_tcslen(bstr) + _tcslen(pCondition) + 17] ;
		pStr[0] = 0;

		if(barray)
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L" and ");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}else
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"[");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 17,L"]");
		}

		if(m_pRoot)
			m_pRoot->selectNodes(pStr,&pLists);
		else
			m_xbXml.m_pXmlDoc->selectNodes(pStr,&pLists);
		delete pStr;

		pLists->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pLists->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);

			pElement->removeAttribute(L"update.modify");
			pElement->Release();
			pNode->Release();
		}
		//reset origin  origindisplaystring
		pCondition = L"@update.modify='1'";
		pStr = new TCHAR[_tcslen(bstr) + _tcslen(pCondition) + 19] ;
		pStr[0] = 0;

		if(barray)
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,L" and ");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,L"]");
		}else
		{
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,bstr);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,L"/*[");
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,pCondition);
			_tcscat_s(pStr,_tcslen(bstr) + _tcslen(pCondition) + 19,L"]");
		}

		if(m_pRoot)
			m_pRoot->selectNodes(pStr,&pLists);
		else
			m_xbXml.m_pXmlDoc->selectNodes(pStr,&pLists);
		delete pStr;

		IXMLDOMNode * pNodeOri;
		pLists->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pLists->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);

			pElement->removeAttribute(L"update.modify");
			pElement->selectSingleNode(L"@update.origin", &pNodeOri);
			if(pNodeOri)
			{
				pElement->removeAttribute(L"update.origin");
				pNodeOri->Release();
			}
			pElement->selectSingleNode(L"@update.origindisplaystring", &pNodeOri);
			if(pNodeOri)
			{
				pElement->removeAttribute(L"update.origindisplaystring");
				pNodeOri->Release();
			}
			pElement->Release();
			pNode->Release();
		}

		::SysFreeString(bstr);

		return true;
	}


	XAPI STDMETHODIMP_(LPTSTR) XXmlContentSvr::GetXPath()
	{
		return m_pStrRoot;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::SetRootElement(IXMLDOMElement  * pRoot)
	{
		if(m_pRoot)m_pRoot->Release();
		m_pRoot = pRoot;
		pRoot->AddRef();
		if(m_pStrRoot)SetXPath(m_pStrRoot);
		return true;
	}

	XAPI STDMETHODIMP_(bool) XXmlContentSvr::Initial(IXMLDOMElement  * pElement)
	{
		m_pCols->clear();
		m_pXPaths->clear();
		if(m_pLists)m_pLists->Release();
		m_pLists = NULL;
		if(m_pStrRoot)
		{
			delete m_pStrRoot;
			m_pStrRoot = NULL;
		}
		if(m_pUri)
		{
			delete m_pUri;
			m_pUri = NULL;
		}
		if(m_pUrl)
		{
			delete m_pUrl;
			m_pUrl = NULL;
		}

		if(pElement)
		{
			BSTR xx;
			pElement->get_xml(&xx);
			m_pStrRoot = xbXml::GetStringAttribe(pElement,_T("xroot"));
			m_pUri = xbXml::GetStringAttribe(pElement,_T("uri"));
			m_pUrl = xbXml::GetStringAttribe(pElement,_T("url"));

			IXMLDOMNodeList*	pNodeList;
			IXMLDOMNode* pNode;
			IXMLDOMElement* pElementEx;
			long	nLen;
			pElement->selectNodes(L"col",&pNodeList);
			pNodeList->get_length(&nLen);
			for(int i=0;i<nLen;i++)
			{
				pNodeList->get_item(i,&pNode);
				pElementEx = xbXml::GetElement(pNode);
				pNode->Release();
				m_pCols->push_back(xbXml::GetStringAttribe(pElementEx,_T("name")));
				m_pXPaths->push_back(xbXml::GetStringAttribe(pElementEx,_T("xpath")));
				pElementEx->Release();
			}
			pNodeList->Release();
		}
		
		IXMLDOMElementPtr pDocElement;
		m_xbXml.m_pXmlDoc->get_documentElement(&pDocElement);
		if(pDocElement) m_xbXml.m_pXmlDoc->removeChild(pDocElement,NULL);


		return true;
	}

	int XXmlContentSvr::SetTreeData(vector<TTreeItem>* pTreeData)
	{
		m_pvcTreeData = pTreeData;
		return 1;
	}

	int  XXmlContentSvr::GetTreeNextItem(vector<TTreeItem>* pTreeData,int nIndex)
	{
		int nLevel = (*pTreeData)[nIndex].nLevel;
		for(int i=nIndex + 1;i<(int)pTreeData->size();i++)
		{
			if((*pTreeData)[i].nLevel < nLevel) return -1;
			if((*pTreeData)[i].nLevel == nLevel) return i;
		}
		return -1;
	}

	int  XXmlContentSvr::GetTreeParentItem(vector<TTreeItem>* pTreeData,int nIndex)
	{
		int nLevel = (*pTreeData)[nIndex].nLevel;
		for(int i=nIndex - 1;i>=0;i--)
		{
			if((*pTreeData)[i].nLevel < nLevel) return i;
		}
		return -1;
	}


	int XXmlContentSvr::GetTreeIndex(vector<TTreeItem>* pTreeData,int tRow)
	{
		int nLevel = 0;
		if(tRow < 1) return -1;
		if(tRow > (int)(*pTreeData).size()) return -1; 
		for(int i=0;i<(int)(*pTreeData).size();i++)
		{
			if(--tRow==0) return i;
			if(!(*pTreeData)[i].expanded)
			{
				nLevel = (*pTreeData)[i].nLevel;
				while(++i<(int)(*pTreeData).size() && (*pTreeData)[i].nLevel > nLevel);
				if(i < (int)(*pTreeData).size())i--;
			}
		}
		return -1;
	}

	
	int XXmlContentSvr::GetBIData(IXMLDOMDocument2 * pXmlDoc)
	{
		VARIANT_BOOL bRes;
		pXmlDoc->loadXML(L"<data/>",&bRes);
		IXMLDOMElement* pRoot;
		pXmlDoc->get_documentElement(&pRoot);
		long len;
		m_pLists->get_length(&len);
		for(int i=0;i<len;i++)
		{
			IXMLDOMNode * pNode;
			IXMLDOMNode * pNodex;
			m_pLists->get_item(i,&pNode);
			pNode->cloneNode(VARIANT_TRUE,&pNodex);
			pRoot->appendChild(pNodex,0);
			pNode->Release();
		}
		pRoot->Release();
		return 1;
	}

	int XXmlContentSvr::GetContentRow(int nRow)
	{
		vector<TTreeItem>* pTreeData;
		if(GetTreeData(pTreeData)>=0)
		{
			int nIndex = GetTreeIndex(pTreeData,nRow);
			if(nIndex>=0)
				return (*pTreeData)[nIndex].nContentRow;
			else
				return -1;
		}else
			return nRow;
	}

	LPCTSTR XXmlContentSvr::GetTotalItemString(LPCTSTR pTotalColumn,LPCTSTR pFormat)
	{
		if(!pTotalColumn) return NULL;

		if(m_pTotalStr && m_pTotalStr->find(pTotalColumn)!=m_pTotalStr->end())
			return (*m_pTotalStr)[pTotalColumn].c_str();


		bool ksum = false;
		bool kavg = false;
		bool kcount = false;

		if(wcsstr(pTotalColumn,L"sum:")) ksum=true;
		else if(wcsstr(pTotalColumn,L"avg:")) kavg=true;
		else if(wcsstr(pTotalColumn,L"count:")) kcount=true;
		else return NULL;


		if(m_pLists)
		{
			bool kfloat = false;
			double rval=0.0;
			int ival = 0;
			long len;
			m_pLists->get_length(&len);
			if(kcount)
			{
				ival = len;
			}else
			{
				for(int i=0;i<len;i++)
				{
					IXMLDOMNode * node = 0;
					m_pLists->get_item(i,&node);
					IXMLDOMNode * rnode = 0;
					node->selectSingleNode((BSTR)wcschr(pTotalColumn,L':')+1,&rnode);
					node->Release();
					if(rnode)
					{
						BSTR bstr = 0;
						rnode->get_text(&bstr);
						rnode->Release();
						if(bstr)
						{
							bool bDelete = false;
							wchar_t* sval = bstr;
							if(pFormat)
								sval = Hxsoft::XUtil::FormatDecimalString(sval,(LPTSTR)pFormat,bDelete);
							if(!kfloat && wcschr((const wchar_t*)sval,L'.'))
							{
								rval = ival;
								kfloat = true;
							}
							if(kfloat)
								rval += _wtof(sval);
							else
								ival += _wtoi(sval);
							::SysFreeString(bstr);
							if(bDelete) delete sval;
						}
					}
				}
			}
			if(kavg && kfloat)
				rval = rval/len;
			else if(kavg && !kfloat)
			{
				kfloat = true;
				rval = ival*1.0/len;
			}
			wchar_t str[255];
			if(kfloat)
				swprintf_s(str,255,L"%f",rval);
			else
				swprintf_s(str,255,L"%d",ival);
			if(!m_pTotalStr) m_pTotalStr = new std::map<std::wstring,std::wstring>;
			m_pTotalStr->insert(std::pair<std::wstring,std::wstring>(pTotalColumn,str));
			return (*m_pTotalStr)[pTotalColumn].c_str();
		}
		return NULL;
	}

	LPCTSTR XXmlContentSvr::GetTotalItemString(LPCTSTR pTotalColumn)
	{
		return GetTotalItemString(pTotalColumn,NULL);
	}

	LPTSTR XXmlContentSvr::GetColumnName(int ACol)
	{
		if(ACol < 0 || ACol >= (int)m_pCols->size()) return NULL;
		return (*m_pCols)[ACol ];
	}

	int  XXmlContentSvr::GetColumnIndex(LPCTSTR pColName)
	{
		int i=0;
		if(!pColName) return -1;
		for(i=0;i<(int)m_pCols->size();i++)
		{
			if(_tcsicmp(pColName,(*m_pCols)[i])==0) return i;//i + 1;
		}
		return -1;
	}



	int XXmlContentSvr::Sort(vector<wstring>* m_pCols,vector<bool>* m_pOrders,vector<wstring>* m_pColTypes,bool bMovePositon)
	{
		int nRowCount = GetRowCount();
		if(nRowCount < 1) return 0;
		
		int nSize = (int)m_pCols->size();
		if(nSize < 1) return 0;

		vector<int> vPos;
		vector<vector<LPCTSTR>*>Vals;
		for(int i=0;i<nSize;i++)
		{
			vector<LPCTSTR>* pItem = new vector<LPCTSTR>;
			Vals.push_back(pItem);
			for(int row=1;row <=nRowCount;row++)
			{
				LPCTSTR str = this->GetItemDisplayString((LPTSTR)(*m_pCols)[i].c_str(),row);
				if(str)
					pItem->push_back(str);
				else
					pItem->push_back(this->GetItemString((LPTSTR)(*m_pCols)[i].c_str(),row));
				if(i==0) vPos.push_back(row - 1);
			}
		}
		
		FSortItem item;
		item.pOrders =  m_pOrders;
		item.pPos = &vPos;
		item.pVals = &Vals;
		item.pColTypes = m_pColTypes;

		QuickSort(item,nRowCount);

		IXMLDOMNode * pNode;
		IXMLDOMNode * pParentNode;
		vector<IXMLDOMNode *> nodes;
		vector<IXMLDOMNode *> pnodes;
		for(int i=0;i<nRowCount;i++)
		{
			m_pLists->get_item(i,&pNode);
			pNode->get_parentNode(&pParentNode);
			pParentNode->removeChild(pNode,NULL);
			nodes.push_back(pNode);
			pnodes.push_back(pParentNode);
		}

		for(int i=0;i<nRowCount;i++)
		{
			int nIndex = (*item.pPos)[i];
			pnodes[i]->appendChild(nodes[nIndex],NULL);
		}
		this->SetXPath(m_pStrRoot);
		return 1;
	}
	XXmlContentSvr * XXmlContentSvr::CreateInstance()
	{
		return new XXmlContentSvr();
	}

	bool XXmlContentSvr::MoveRow(int row, int movecount)
	{
		int nRowCount = GetRowCount();
		if(nRowCount < 1) return false;

		int row2 = row + movecount;
		if(row == row2) return false;
		if(row < 1 || row > nRowCount) return false;
		if(row2 < 1 || row2 > nRowCount) return false;

		IXMLDOMNode * pNode = NULL;
		IXMLDOMNode * pNode2 = NULL;
		IXMLDOMNode * pParentNode = NULL;

		m_pLists->get_item(row - 1, &pNode);
		m_pLists->get_item(row2 - 1, &pNode2);
		if(!pNode || !pNode2) return false;

		pNode->get_parentNode(&pParentNode);
		CComVariant varRef;
		HRESULT hr;
		if(movecount < 0)
		{
			pParentNode->removeChild(pNode, NULL);
			varRef = pNode2;
			hr = pParentNode->insertBefore(pNode, varRef, NULL);
		}
		else
		{
			pParentNode->removeChild(pNode2, NULL);
			varRef = pNode;
			hr = pParentNode->insertBefore(pNode2, varRef, NULL);
		}
		if (FAILED(hr)) return false;
		varRef.Clear();
		this->SetXPath(m_pStrRoot);
		return true;
	}
	int XXmlContentSvr::SetShowOrigin(bool f)
	{
		FShowOrigin = f;
		return 1;
	}
	int XXmlContentSvr::ColumnFilter(LPTSTR pColumn, LPTSTR pStr)
	{
		if(!m_pQueryStr)
			m_pQueryStr = new std::vector<std::vector<LPTSTR> >();

		if(!pColumn)
		{
			m_pQueryStr->clear();
		}
		else
		{
			int i = 0;
			for(i=0; i<(int)m_pQueryStr->size(); i++)
			{
				LPTSTR pText = (*m_pQueryStr)[i][0];
				if(pText && pColumn && _tcscmp(pText, pColumn) == 0) break;
			}

			if(i<(int)m_pQueryStr->size())
			{
				(*m_pQueryStr)[i].clear();
				(*m_pQueryStr).erase((*m_pQueryStr).begin() + i);
			}

			if(pStr && pStr[0]!='\0')
			{	
				std::vector<LPTSTR> * array1 = new std::vector<LPTSTR>();
				array1->push_back(pColumn);
				array1->push_back(pStr);
				m_pQueryStr->push_back(*array1);
			}
		}
		if(m_pTotalStr) m_pTotalStr->clear();
		SetXPath(GetXPath());
		return 1;
	}
}}}
