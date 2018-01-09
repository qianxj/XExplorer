#include "stdafx.h"
#include "xuxutil.h"
#include "xcTreeXmlDataSource.hpp"
#include "xcTreectrl.hpp"
#include "xframe.h"

namespace Hxsoft
{
	namespace XFrame
	{
		LPTSTR TransStrEx(LPTSTR pSrc,xcTreeCtrl* pTree,HTREEITEM hItem);
		LPTSTR TransStr(LPTSTR pSrc,xcTreeCtrl* pTree,HTREEITEM hItem)
		{
			if(_tcsstr(pSrc,_T("[xpath:"))) return TransStrEx(pSrc,pTree,hItem);

			LPTSTR pBuf;
			LPTSTR pStr;
			LPTSTR pData;
			LPTSTR pSource;
			LPTSTR pFindStr = _T("[:data]");
			int nPos,nLimit;
			int nDataLen = 0;
			nLimit = ((int)::_tcslen(pSrc))/32 * 32 + 32;
			nPos = 0;
			pBuf = new TCHAR[nLimit];
			ZeroMemory(pBuf,sizeof(TCHAR)*nLimit);

			pStr = _tcsstr(pSrc,pFindStr);
			if(!pStr) return pSrc;

			TVITEMEX tvi;
			tvi.mask = TVIF_PARAM;
			tvi.hItem = hItem; 
			::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
			pData = (LPTSTR)tvi.lParam;
			nDataLen = (int)_tcslen(pData);

			pSource = pSrc;
			while(true)
			{
				if(pStr != pSource)
				{
					_tcsncpy_s(pBuf + nPos,nLimit - nPos,pSource,pSource - pStr);
					nPos += (int)(pSource - pStr);
				}
				if(nPos + nDataLen >=  nLimit - 1)
				{
					nLimit = (nPos + nDataLen)/32 * 32 + 32;
					LPTSTR pTemp = new TCHAR[nLimit];
					ZeroMemory(pTemp,sizeof(TCHAR)*nLimit);
					_tcscpy_s(pTemp,nLimit,pBuf);
					delete pBuf;
					pBuf = pTemp;
				}
				_tcsncpy_s(pBuf + nPos,nLimit - nPos,pData,::_tcslen(pData));
				nPos += (int)_tcslen(pData);

				pStr += ::_tcslen(pFindStr);
				pSource = pStr;
				pStr = _tcsstr(pSource,pFindStr);
				if(!pStr)
				{
					if(nPos + (int)_tcslen(pSource) >= nLimit - 1)
					{
						nLimit = (nPos + (int)_tcslen(pSource))/32 * 32 + 32;
						LPTSTR pTemp = new TCHAR[nLimit];
						ZeroMemory(pTemp,sizeof(TCHAR)*nLimit);
						_tcscpy_s(pTemp,nLimit,pBuf);
						delete pBuf;
						pBuf = pTemp;
					}
					_tcsncpy_s(pBuf + nPos,nLimit - nPos,pSource,(int)::_tcslen(pSource));
					nPos +=(int)_tcslen(pSource);
					return pBuf;
				}
			}
			return pBuf;
		};

		LPTSTR TransStrEx(LPTSTR pSrc,xcTreeCtrl* pTree,HTREEITEM hItem)
		{
			LPTSTR pBuf;
			LPTSTR pStr;
			LPTSTR pSource;
			LPTSTR pFindStr = _T("[xpath:");
			int nPos,nLimit;
			int nDataLen = 0;
			nLimit = ((int)::_tcslen(pSrc))/32 * 32 + 256;
			nPos = 0;
			pBuf = new TCHAR[nLimit];
			ZeroMemory(pBuf,sizeof(TCHAR)*nLimit);

			TCHAR xpath[255];
			::ZeroMemory(xpath,255);

			pStr = _tcsstr(pSrc,pFindStr);
			if(!pStr) return pSrc;

			TVITEMEX tvi;
			tvi.mask = TVIF_PARAM;
			tvi.hItem = hItem; 
			::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
			
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			pElement = (IXMLDOMElement *)tvi.lParam;
			pSource = pSrc;
			while(true)
			{
				if(pStr != pSource)
				{
					_tcsncpy_s(pBuf + nPos,nLimit - nPos,pSource,pSource - pStr);
					nPos += (int)(pSource - pStr);
				}
				pStr += _tcslen(_T("[xpath:"));
				int ipos = 0;
				while(pStr[0]!=0 && pStr[0]!=']')
				{
					xpath[ipos++] = pStr[0];
					pStr++;
				}

				//should process error
				pStr++;
				BSTR bstr;
				pElement->selectSingleNode(xpath,&pNode);
				pNode->get_text(&bstr);
				pNode->Release();

				nDataLen = (int)_tcslen(bstr);
				if(nPos + nDataLen >=  nLimit - 1)
				{
					nLimit = (nPos + nDataLen)/32 * 32 + 256;
					LPTSTR pTemp = new TCHAR[nLimit];
					ZeroMemory(pTemp,sizeof(TCHAR)*nLimit);
					_tcscpy_s(pTemp,nLimit,pBuf);
					delete pBuf;
					pBuf = pTemp;
				}
				_tcsncpy_s(pBuf + nPos,nLimit - nPos,bstr,nDataLen);
				nPos += nDataLen;
				::SysFreeString(bstr);

				pSource = pStr;
				pStr = _tcsstr(pSource,pFindStr);
				if(!pStr)
				{
					if(nPos + (int)_tcslen(pSource) >= nLimit - 1)
					{
						nLimit = (nPos + (int)_tcslen(pSource))/32 * 32 + 256;
						LPTSTR pTemp = new TCHAR[nLimit];
						ZeroMemory(pTemp,sizeof(TCHAR)*nLimit);
						_tcscpy_s(pTemp,nLimit,pBuf);
						delete pBuf;
						pBuf = pTemp;
					}
					_tcsncpy_s(pBuf + nPos,nLimit - nPos,pSource,(int)::_tcslen(pSource));
					nPos +=(int)_tcslen(pSource);
					return pBuf;
				}
			}
			return pBuf;
		};

		xcTreeXmlDataSource::~xcTreeXmlDataSource(void)
		{
			if(m_strXRootPath)delete m_strXRootPath;
			if(m_strXPath)delete m_strXPath;
			if(m_strXData)delete m_strXData;
			if(m_strXText)delete m_strXText;
			if(m_strXSrc)delete m_strXSrc;
			if(m_strXUrl)delete m_strXUrl;
			if(m_strXImage) delete m_strXImage;
			if(m_strXSelectedImage)delete m_strXSelectedImage;

			if(m_strXDataEx)delete m_strXDataEx;
			if(m_strXTextEx)delete m_strXTextEx;
			if(m_strXSrcEx)delete m_strXSrcEx;
			if(m_strXUrlEx)delete m_strXUrlEx;
			if(m_strXImageEx) delete m_strXImageEx;
			if(m_strXSelectedImageEx)delete m_strXSelectedImageEx;
			if(m_strExpSrc)delete m_strExpSrc;
			if(m_ChildPath)delete m_ChildPath;
			if(m_strParam) delete m_strParam;

			for(int i=0;i<(int)(*m_pElementsLists).size();i++)
			{
				if((*m_pElementsLists)[i])(*m_pElementsLists)[i]->Release();
			}
			(*m_pElementsLists).clear();
			if(m_pElementsLists)delete m_pElementsLists;
			if(m_pXml)delete m_pXml;
			if(m_pXmlEx)delete m_pXmlEx;
			if(m_pXmlElement)m_pXmlElement->Release();
		}

		xcTreeXmlDataSource::xcTreeXmlDataSource():m_bXmlLoaded(false),m_strXRootPath(NULL),m_strXPath(NULL),m_strXData(NULL),m_strXText(NULL),m_strXSrc(NULL),m_strXUrl(NULL),m_strXImage(NULL),m_strXSelectedImage(NULL),
			m_strXDataEx(NULL),m_strXTextEx(NULL),m_strXSrcEx(NULL),m_strXUrlEx(NULL),m_strXImageEx(NULL),m_strXSelectedImageEx(NULL),m_strExpSrc(NULL),m_ChildPath(NULL),
			m_pElementsLists(new vector<IXMLDOMElement *>),m_pXml(new xbXml),m_pXmlEx(new xbXml),m_pXmlElement(NULL),m_strParam(NULL)
		{
		}

		int xcTreeXmlDataSource::LoadXmlFile(LPTSTR strFile)
		{
			VARIANT_BOOL vtResult;
			m_pXml->m_pXmlDoc->load((_variant_t)strFile,&vtResult);
			return 1;
		}
		int xcTreeXmlDataSource::LoadXmlStr(LPTSTR xmlStr)
		{
			VARIANT_BOOL vtResult;
			m_pXml->m_pXmlDoc->loadXML((_bstr_t)xmlStr,&vtResult);
			return 1;
		}
		int xcTreeXmlDataSource::LoadXmlFileEx(LPTSTR strFile)
		{
			VARIANT_BOOL vtResult;
			m_pXmlEx->m_pXmlDoc->load((_variant_t)strFile,&vtResult);
			return 1;
		}
		int xcTreeXmlDataSource::LoadXmlStrEx(LPTSTR xmlStr)
		{
			VARIANT_BOOL vtResult;
			m_pXmlEx->m_pXmlDoc->loadXML((_bstr_t)xmlStr,&vtResult);
			return 1;
		}

		bool xcTreeXmlDataSource::HasChild(xcTreeCtrl* pTree, HTREEITEM	pItem)
		{
			IXMLDOMNodeListPtr	pList;
			IXMLDOMNode*		pNode;
			IXMLDOMElement*		pElement;
			TVITEM tvi; 
			long	len;
			int		level = 1;

			HTREEITEM pParentItem;
			pParentItem = pItem;
			while(pParentItem && pParentItem != m_hItem )
			{
				level++;
				pParentItem = TreeView_GetParent(pTree->m_hWnd,pParentItem);
				if(!pParentItem && m_hItem ==TVI_ROOT)
				{
					pParentItem = TVI_ROOT;
					break; 
				}
			}
			if(!m_bLoop ||pItem == m_hItem ||(pParentItem && level ==this->m_RLevel))
			{
				if(::_tcsstr(m_strXSrc,_T("[:data]")) ||::_tcsstr(m_strXSrc,_T("[xpath:")))
				{
					LPTSTR pStrFile ;
					pStrFile = TransStr(m_strXSrc,pTree,pItem);  

					TCHAR Path[MAX_PATH];
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,pStrFile,strUrl,&dwSize,0);
					bool reload = 
						(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
					LPTSTR strParam = NULL;
					if(m_strParam)
					{
						strParam = new TCHAR[255]; 
						_stprintf_s(strParam,255,_T("<args><arg name='__param' value='%s' type='string'/></args>") ,m_strParam);
					}
					if(xfUriLoader::GetCacheFileName(strUrl,strParam,Path,MAX_PATH,reload)==1)
					//if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
					{
						LoadXmlFile(Path);
						if(pStrFile != m_strXSrc)delete pStrFile;
						IXMLDOMElement * pElt;
						this->m_pXml->m_pXmlDoc->get_documentElement(&pElt);
						if(pElt)
						{
							pElt->setAttribute(L"url_ref",(_variant_t)strUrl);
							pElt->Release();
						}
					}else
					{
						if(pStrFile != m_strXSrc)delete pStrFile;
						return 0;
					}
				}else if(!m_bXmlLoaded )
				{
					TCHAR Path[MAX_PATH];
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,m_strXSrc,strUrl,&dwSize,0);
					bool reload = 
						(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
					LPTSTR strParam = NULL;
					if(m_strParam)
					{
						strParam = new TCHAR[255]; 
						_stprintf_s(strParam,255,_T("<args><arg name='__param' value='%s' type='string'/></args>") ,m_strParam);
					}
					if(xfUriLoader::GetCacheFileName(strUrl,strParam,Path,MAX_PATH,reload)==1)
					//if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
					{
						LoadXmlFile(Path);
						m_bXmlLoaded = true;
						IXMLDOMElement * pElt;
						this->m_pXml->m_pXmlDoc->get_documentElement(&pElt);
						if(pElt)
						{
							pElt->setAttribute(L"url_ref",(_variant_t)strUrl);
							pElt->Release();
						}
					}else
						return 0;
				}
				if(m_strXRootPath && ::_tcscmp(m_strXRootPath,_T(""))!=0)
				{
					if(m_pXmlElement)
						m_pXmlElement->selectNodes(this->m_strXRootPath,&pList);
					else
					{
						if(m_strXSrc && m_strXSrc[0]=='.')
						{
							HTREEITEM hItem;
							hItem = TreeView_GetParent(pTree->m_hWnd,pItem);
							TVITEMEX tvi;
							tvi.mask = TVIF_PARAM;
							tvi.hItem = hItem;//pItem; 
							::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
							IXMLDOMElement* pElement = (IXMLDOMElement *)tvi.lParam;
							if(!pElement) return 1;
							pElement->selectNodes(this->m_strXRootPath,&pList); 
						}else
						{
							this->m_pXml->m_pXmlDoc->selectNodes(this->m_strXRootPath,&pList);
						}
					}
					pList->get_length(&len);
					if(len < 1) return false;
					if(pList)
					{
						pList->get_item(0,&pNode);
						pNode->selectNodes(this->m_strXPath,&pList);
						pNode->Release();
					}else
						return false;
				}else
				{
					if(m_pXmlElement)
						m_pXmlElement->selectNodes(this->m_strXPath,&pList);
					else
					{
						if(m_strXSrc && m_strXSrc[0]=='.')
						{
							HTREEITEM hItem;
							hItem = TreeView_GetParent(pTree->m_hWnd,pItem);
							TVITEMEX tvi;
							tvi.mask = TVIF_PARAM;
							tvi.hItem = hItem;//pItem; 
							::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
							IXMLDOMElement* pElement = (IXMLDOMElement *)tvi.lParam;
							if(!pElement) return 1;
							pElement->selectNodes(this->m_strXPath,&pList);
						}else
						{
							this->m_pXml->m_pXmlDoc->selectNodes(this->m_strXPath,&pList);
						}
					}
					if(!pList) return false;
				}
				pList->get_length(&len);
				if(len >0)
					return true;
				else
					return false;
			}else
			{
				if(pParentItem)
				{
					tvi.mask =  TVIF_PARAM;
					tvi.hItem = pItem;
					TreeView_GetItem(pTree->m_hWnd,&tvi);
					pElement = (IXMLDOMElement* )(tvi.lParam);

					if(m_ChildPath)
						pElement->selectNodes(m_ChildPath,&pList);
					else
						pElement->selectNodes(L"*",&pList);

					pList->get_length(&len);
					if(len < 1 && this->m_strExpSrc)
					{
						BSTR bstr;
						IXMLDOMNode * pNodeEx;
						pElement->selectSingleNode(m_strExpSrc,&pNodeEx);
						if(pNodeEx)
						{
							pNodeEx->get_text(&bstr);
							if(bstr)
							{
								TCHAR Path[MAX_PATH];
								TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
								DWORD dwSize = INTERNET_MAX_URL_LENGTH;

								IXMLDOMElement * pElt;
								IXMLDOMNode * pDocNode;
								pElement->selectSingleNode(L"/",&pDocNode);
								_variant_t tvar;
								if(pDocNode)
								{
									DOMNodeType NodeType;
									pDocNode->get_nodeType(&NodeType);
									if(NodeType != NODE_ELEMENT)
									{
										pDocNode->Release();
										pElement->selectSingleNode(L"/*",&pDocNode);
									}
								}

								if( pDocNode)
								{

									pDocNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElt);
									if(pElt)
									{
										_variant_t	var;
										pElt->getAttribute(L"url_ref",&var);
										if(var.bstrVal)
											xfUriLoader::UrlCombineW(var.bstrVal,bstr,strUrl,&dwSize,0);
										else
											xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,bstr,strUrl,&dwSize,0);
										pElt->Release();
									}
									pDocNode->Release();
								}
								else
									xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,bstr,strUrl,&dwSize,0);

								bool reload = 
									(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
								if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
								{
									
									xbXml xml;
									xml.Load(Path);
									IXMLDOMElement * pDocElement;
									IXMLDOMNodeList * pListex;
									IXMLDOMNode* pChildNode;
									xml.m_pXmlDoc->get_documentElement(&pDocElement);
									if(m_ChildPath)
										pDocElement->selectNodes(m_ChildPath,&pListex);
									else
										pDocElement->selectNodes(L"*",&pListex);
									long nLen;
									pListex->get_length(&nLen);
									for(int m=0;m< nLen;m++)
									{
										pListex->get_item(m,&pChildNode);
										pElement->appendChild(pChildNode,NULL);
										pChildNode->Release();
									}
									pListex->Release();
									pDocElement->Release();

									if(m_ChildPath)
										pElement->selectNodes(m_ChildPath,&pList);
									else
										pElement->selectNodes(L"*",&pList);
									pList->get_length(&len);
								}
								::SysFreeString(bstr);
							}
							pNodeEx->Release();
						}
					}

					if(len > 0) 
						return true;
					else
						return false;
				}
			}
			return false;
		}

		int xcTreeXmlDataSource::ExpandItem(xcTreeCtrl* pTree, HTREEITEM	pItem )
		{
			IXMLDOMNodeListPtr	pList;
			IXMLDOMNode*		pNode;
			IXMLDOMNode*		pNodeEx;
			IXMLDOMElement*		pElement;
			IXMLDOMElement*		pElementEx;
			TVITEM tvi; 
			TVINSERTSTRUCT tvins; 
			HTREEITEM hItem;
			long	len;
			int		level = 1;

			HTREEITEM pParentItem;
			pParentItem = pItem;
			while(pParentItem && pParentItem != m_hItem )
			{
				level++;
				pParentItem = TreeView_GetParent(pTree->m_hWnd,pParentItem);
				if(!pParentItem && m_hItem ==TVI_ROOT)
				{
					pParentItem = TVI_ROOT;
					break; 
				}
			}
			if(!m_bLoop ||pItem == m_hItem ||(pParentItem && level ==this->m_RLevel))
			{
				if(::_tcsstr(m_strXSrc,_T("[:data]")) || ::_tcsstr(m_strXSrc,_T("[xpath:")))
				{
					LPTSTR pStrFile ;
					pStrFile = TransStr(m_strXSrc,pTree,pItem);

					TCHAR Path[MAX_PATH];
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,pStrFile,strUrl,&dwSize,0);
					bool reload = 
						(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
					LPTSTR strParam = NULL;
					if(m_strParam)
					{
						strParam = new TCHAR[255]; 
						_stprintf_s(strParam,255,_T("<args><arg name='__param' value='%s' type='string'/></args>") ,m_strParam);
					}
					if(xfUriLoader::GetCacheFileName(strUrl,strParam,Path,MAX_PATH,reload)==1)
					//if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
					{
						LoadXmlFile(Path);
						if(pStrFile != m_strXSrc)delete pStrFile;
						IXMLDOMElement * pElt;
						this->m_pXml->m_pXmlDoc->get_documentElement(&pElt);
						if(pElt)
						{
							pElt->setAttribute(L"url_ref",(_variant_t)strUrl);
							pElt->Release();
						}
					}else
					{
						if(pStrFile != m_strXSrc)delete pStrFile;
						return 0;
					}
					
				}else if(!m_bXmlLoaded )
				{
					TCHAR Path[MAX_PATH];
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,m_strXSrc,strUrl,&dwSize,0);
					bool reload = 
						(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
					LPTSTR strParam = NULL;
					if(m_strParam)
					{
						strParam = new TCHAR[255]; 
						_stprintf_s(strParam,255,_T("<args><arg name='__param' value='%s' type='string'/></args>") ,m_strParam);
					}
					if(xfUriLoader::GetCacheFileName(strUrl,strParam,Path,MAX_PATH,reload)==1)
					//if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
					{
						LoadXmlFile(Path);
						m_bXmlLoaded = true;
						IXMLDOMElement * pElt;
						this->m_pXml->m_pXmlDoc->get_documentElement(&pElt);
						if(pElt)
						{
							pElt->setAttribute(L"url_ref",(_variant_t)strUrl);
							pElt->Release();
						}
					}else
						return 0;
				}

				if(m_strXRootPath && ::_tcscmp(m_strXRootPath,_T(""))!=0)
				{
					if(m_pXmlElement)
						m_pXmlElement->selectNodes(this->m_strXRootPath,&pList);
					else
					{
						if(m_strXSrc && m_strXSrc[0]=='.')
						{
							TVITEMEX tvi;
							tvi.mask = TVIF_PARAM;
							tvi.hItem = pItem; 
							::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
							IXMLDOMElement* pElement = (IXMLDOMElement *)tvi.lParam;
							if(!pElement) return 1;
							pElement->selectNodes(this->m_strXRootPath,&pList); 
						}else
						{
							this->m_pXml->m_pXmlDoc->selectNodes(this->m_strXRootPath,&pList);
						}
					}
					pList->get_length(&len);
					if(len < 1) return 1;
					if(pList)
					{
						pList->get_item(0,&pNode);
						pNode->selectNodes(this->m_strXPath,&pList);
						pNode->Release();
					}else
						return -1;
				}else
				{
					if(m_pXmlElement)
						m_pXmlElement->selectNodes(this->m_strXPath,&pList);
					else
					{
						if(m_strXSrc && m_strXSrc[0]=='.')
						{
							TVITEMEX tvi;
							tvi.mask = TVIF_PARAM;
							tvi.hItem = pItem; 
							::SendMessage(pTree->m_hWnd,TVM_GETITEM,0,(LPARAM)&tvi);
							IXMLDOMElement* pElement = (IXMLDOMElement *)tvi.lParam;
							if(!pElement) return 1;
							pElement->selectNodes(this->m_strXPath,&pList);
						}else
						{
							this->m_pXml->m_pXmlDoc->selectNodes(this->m_strXPath,&pList);
						}
					}
					if(!pList) return -1;
				}
				pList->get_length(&len);
				for(long i=0;i<len;i++)
				{
					pList->get_item(i,&pNode);
					//load extent xml file
					if(m_strXSrcEx && m_strXSrcEx[0]!='0')
					{
						pNode->selectSingleNode(m_strXSrcEx,&pNodeEx);
						BSTR	bstrSrcEx = NULL;
						if(pNodeEx)pNodeEx->get_text(&bstrSrcEx);
						if(bstrSrcEx)
						{
							// Set the text of the item. 
							LPTSTR pStrFile = XUtil::tcsdupFromBstr(bstrSrcEx);


							TCHAR Path[MAX_PATH];
							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,pStrFile,strUrl,&dwSize,0);
							bool reload = 
								(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
							if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
							{
								LoadXmlFileEx( Path);
								this->m_bXmlLoaded = true;
								IXMLDOMElement * pElt;
								this->m_pXmlEx->m_pXmlDoc->get_documentElement(&pElt);
								if(pElt)
								{
									pElt->setAttribute(L"url_ref",(_variant_t)strUrl);
									pElt->Release();
								}
							}

							::SysFreeString(bstrSrcEx);
							::delete pStrFile;
						}
					}

					tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					pNodeEx = NULL;
					BSTR	bstrText = NULL;
					if(m_strXText  && _tcscmp(m_strXText,_T("name()"))==0)
					{
						BSTR bstr=NULL;
						IXMLDOMElement * pElemet0;
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElemet0);
						pElemet0->get_tagName(&bstrText);
						pElemet0->Release();

					}else
					{
						if(m_strXTextEx && m_strXTextEx[0]!='0' && m_pXmlEx->m_pXmlDoc)
						{
							m_pXmlEx->m_pXmlDoc->selectSingleNode(this->m_strXTextEx,&pNodeEx);
						}else if(m_strXText)
						{
							pNode->selectSingleNode(this->m_strXText,&pNodeEx);
						}

						if(pNodeEx)pNodeEx->get_text(&bstrText);
					}
					if(bstrText)
					{
						// Set the text of the item. 
						tvi.pszText = XUtil::tcsdupFromBstr(bstrText);
						tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
						::SysFreeString(bstrText);
					}else
					{
						tvi.pszText = NULL;
						tvi.cchTextMax = 0;
					}
					if(pNodeEx)pNodeEx->Release();


					pNodeEx = NULL;
					tvi.iImage = m_iImage;
					if(m_strXImageEx && m_strXImageEx[0]!='0' && m_pXmlEx->m_pXmlDoc)
					{
						m_pXmlEx->m_pXmlDoc->selectSingleNode(this->m_strXImageEx,&pNodeEx);
					}else if(this->m_strXImage)
					{
						pNode->selectSingleNode(this->m_strXImage,&pNodeEx);
					}

					BSTR	bstrImage = NULL;
					if(pNodeEx)pNodeEx->get_text(&bstrImage);
					if(bstrImage)
					{
						WCHAR * pNextStr;
						tvi.iImage = ::wcstol(bstrImage,&pNextStr,10);
						::SysFreeString(bstrImage);
						m_iImage  = tvi.iImage;
					}else
					{
						tvi.iImage = m_iImage;
					}
					if(pNodeEx)pNodeEx->Release();

					pNodeEx=NULL;
					if(m_strXSelectedImageEx && m_strXSelectedImageEx[0]!='0' && m_pXmlEx->m_pXmlDoc)
					{
						m_pXmlEx->m_pXmlDoc->selectSingleNode(this->m_strXSelectedImageEx,&pNodeEx);
					}else if(this->m_strXSelectedImage)
					{
						pNode->selectSingleNode(this->m_strXSelectedImage,&pNodeEx);
					}

					BSTR	bstrSelectedImage = NULL;
					tvi.iSelectedImage = m_iImage; 
					if(pNodeEx)pNodeEx->get_text(&bstrSelectedImage);
					if(bstrSelectedImage)
					{
						WCHAR * pNextStr;
						tvi.iSelectedImage = ::wcstol(bstrSelectedImage,&pNextStr,10);
						::SysFreeString(bstrSelectedImage);
					}else 
					{
						tvi.iSelectedImage = m_iImage;
						if(m_iSelectedImage >=0) tvi.iSelectedImage = m_iSelectedImage; 
					}
					if(pNodeEx)pNodeEx->Release();

					//get data
					pNodeEx = NULL;
					tvi.lParam = NULL;
					if(m_bLoop || (m_strXData && ::_tcscmp(m_strXData,_T("."))==0))
					{
						pElementEx = (IXMLDOMElementPtr)pNode;
						tvi.lParam = (LPARAM)(void *)pElementEx;
						pElementEx->AddRef();
						(*m_pElementsLists).push_back(pElementEx);
					}else if(m_strXDataEx && m_strXDataEx[0]!='0' && m_pXmlEx->m_pXmlDoc)
					{
						m_pXmlEx->m_pXmlDoc->selectSingleNode(this->m_strXDataEx,&pNodeEx);
					}else if(this->m_strXData)
					{
						pNode->selectSingleNode(this->m_strXData,&pNodeEx);
					}

					BSTR	bstrValue = NULL;
					if(pNodeEx)pNodeEx->get_text(&bstrValue);
					if(bstrValue)
					{
						// Set the text of the item. 
						tvi.lParam = (LPARAM)XUtil::tcsdupFromBstr(bstrValue);
						::SysFreeString(bstrValue);
					}
					if(pNodeEx)pNodeEx->Release();
					pNode->Release();


					// Save the heading level in the item's application-defined 
					// data area. 
					tvins.item = tvi; 
					tvins.hInsertAfter = NULL; 

					// Set the parent item based on the specified level. 
					tvins.hParent = pItem; 

					// Add the item to the tree-view control. 
					hItem = (HTREEITEM)SendMessage(pTree->m_hWnd, TVM_INSERTITEM, 0,(LPARAM)(LPTVINSERTSTRUCT)&tvins); 
					
					//not retrieve all
					tvi.mask = TVIF_CHILDREN;
					tvi.hItem = hItem;
					tvi.cChildren = HasChild(pTree,hItem);
					TreeView_SetItem(pTree->m_hWnd,&tvi);

				}
			}else
			{
				if(pParentItem)
				{
					tvi.mask =  TVIF_PARAM;
					tvi.hItem = pItem;
					TreeView_GetItem(pTree->m_hWnd,&tvi);
					pElement = (IXMLDOMElement* )(tvi.lParam);

					if(pElement)
					{
						if(m_ChildPath)
							pElement->selectNodes(m_ChildPath,&pList);
						else
							pElement->selectNodes(L"*",&pList);
					}

					pList->get_length(&len);
					if(len < 1 && this->m_strExpSrc)
					{
						BSTR bstr;
						IXMLDOMNode * pNodeEx;
						pElement->selectSingleNode(m_strExpSrc,&pNodeEx);
						if(pNodeEx)
						{
							pNodeEx->get_text(&bstr);
							if(bstr)
							{
								TCHAR Path[MAX_PATH];
								TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
								DWORD dwSize = INTERNET_MAX_URL_LENGTH;

								IXMLDOMElement * pElt;
								IXMLDOMNode * pDocNode;
								pElement->selectSingleNode(L"/",&pDocNode);
								_variant_t tvar;
								if(pDocNode)
								{
									DOMNodeType NodeType;
									pDocNode->get_nodeType(&NodeType);
									if(NodeType != NODE_ELEMENT)
									{
										pDocNode->Release();
										pElement->selectSingleNode(L"/*",&pDocNode);
									}
								}

								if( pDocNode)
								{

									pDocNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElt);
									if(pElt)
									{
										_variant_t	var;
										pElt->getAttribute(L"url_ref",&var);
										if(var.bstrVal)
											xfUriLoader::UrlCombineW(var.bstrVal,bstr,strUrl,&dwSize,0);
										else
											xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,bstr,strUrl,&dwSize,0);
										pElt->Release();
									}
									pDocNode->Release();
								}
								else
									xfUriLoader::UrlCombineW(pTree->m_pxfNode->GetXPage()->m_pStrUrl,bstr,strUrl,&dwSize,0);

								bool reload = 
									(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
								if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
								{
									xbXml xml;
									xml.Load(Path);
									IXMLDOMElement * pDocElement;
									IXMLDOMNodeList * pListex;
									IXMLDOMNode* pChildNode;
									xml.m_pXmlDoc->get_documentElement(&pDocElement);
									if(m_ChildPath)
										pDocElement->selectNodes(m_ChildPath,&pListex);
									else
										pDocElement->selectNodes(L"*",&pListex);
									long nLen;
									pListex->get_length(&nLen);
									for(int m=0;m< nLen;m++)
									{
										pListex->get_item(m,&pChildNode);
										pElement->appendChild(pChildNode,NULL);
										pChildNode->Release();
									}
									pListex->Release();
									pDocElement->Release();

									if(m_ChildPath)
										pElement->selectNodes(m_ChildPath,&pList);
									else
										pElement->selectNodes(L"*",&pList);
									pList->get_length(&len);
								}
								::SysFreeString(bstr);
							}
							pNodeEx->Release();
						}
					}
					for(long i=0;i<len;i++)
					{
						tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						pList->get_item(i,&pNode);

						BSTR	bstrText=NULL;
						if(m_strXText  && _tcscmp(m_strXText,_T("name()"))==0)
						{
							BSTR bstr=NULL;
							IXMLDOMElement * pElemet0;
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElemet0);
							pElemet0->get_tagName(&bstrText);
							pElemet0->Release();

						}else
						{
							pNode->selectSingleNode(this->m_strXText,&pNodeEx);
							if(pNodeEx)pNodeEx->get_text(&bstrText);
							if(pNodeEx)pNodeEx->Release();
						}
						if(bstrText)
						{
							// Set the text of the item. 
							tvi.pszText = XUtil::tcsdupFromBstr(bstrText);
							tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
							::SysFreeString(bstrText);
						}else
						{
							tvi.pszText = NULL;
							tvi.cchTextMax = 0;
						}

						if(this->m_strXImage)
						{
							pNode->selectSingleNode(this->m_strXImage,&pNodeEx);
							BSTR	bstrText = NULL;
							if(pNodeEx)pNodeEx->get_text(&bstrText);
							if(bstrText)
							{
								WCHAR * pNextStr;
								tvi.iImage = ::wcstol(bstrText,&pNextStr,10);
								::SysFreeString(bstrText);
								m_iImage  = tvi.iImage;
							}else
							{
								tvi.iImage = m_iImage;
							}
							if(pNodeEx)pNodeEx->Release();
						}else
						{
							tvi.iImage = m_iImage;
						}

						if(this->m_strXSelectedImage)
						{
							pNode->selectSingleNode(this->m_strXSelectedImage,&pNodeEx);
							BSTR	bstrText = NULL;
							if(pNodeEx)pNodeEx->get_text(&bstrText);
							if(bstrText)
							{
								WCHAR * pNextStr;
								tvi.iSelectedImage = ::wcstol(bstrText,&pNextStr,10);
								::SysFreeString(bstrText);
							}else
							{
								tvi.iSelectedImage = m_iImage;
								if(m_iSelectedImage >=0) tvi.iSelectedImage = m_iSelectedImage; 
							}
							if(pNodeEx)pNodeEx->Release();
						}else
						{
							tvi.iSelectedImage = m_iImage; 
							if(m_iSelectedImage >=0) tvi.iSelectedImage = m_iSelectedImage; 
						}

						//get data
						tvi.lParam = NULL;
						if(m_bLoop || ::_tcscmp(m_strXData,_T("."))==0)
						{
							pElementEx = (IXMLDOMElementPtr)pNode;
							tvi.lParam = (LPARAM)(void *)pElementEx;
							(*m_pElementsLists).push_back(pElementEx);
						}else
						{
							pNode->selectSingleNode(this->m_strXData,&pNodeEx);
							BSTR	bstrValue = NULL;
							if(pNodeEx)pNodeEx->get_text(&bstrValue);
							if(bstrValue)
							{
								// Set the text of the item. 
								tvi.lParam = (LPARAM)XUtil::tcsdupFromBstr(bstrValue);
								::SysFreeString(bstrValue);
							}
							if(pNodeEx)pNodeEx->Release();
							pNode->Release();
						}

						// Save the heading level in the item's application-defined 
						// data area. 
						tvins.item = tvi; 
						tvins.hInsertAfter = NULL; 

						// Set the parent item based on the specified level. 
						tvins.hParent = pItem ;

						// Add the item to the tree-view control. 
						hItem = (HTREEITEM)SendMessage(pTree->m_hWnd, TVM_INSERTITEM, 0,(LPARAM)(LPTVINSERTSTRUCT)&tvins); 

						//not retrieve all
						tvi.mask = TVIF_CHILDREN;
						tvi.hItem = hItem;
						tvi.cChildren = HasChild(pTree,hItem);
						TreeView_SetItem(pTree->m_hWnd,&tvi);
					}
				}
			}
			return 1;
		}

		int xcTreeXmlDataSource::GetChildItems(HTREEITEM	pItem)
		{
			return 1; 
		}
	}
}
