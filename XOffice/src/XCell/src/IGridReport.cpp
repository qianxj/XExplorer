#include "StdAfx.h"
#include "..\include\IGridReport.hpp"
#include "xgridReport.hpp"
#include "xgridsheetSvrex.hpp"
#include "XGridHeaderEx.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	IGridReport::IGridReport(void):m_pGridReport(NULL),FDataBISchema(NULL)
	{

	}

	IGridReport::~IGridReport(void)
	{

	}

	int IGridReport::GetRow()
	{
		return m_pGridReport->GetRow();
	}
	int IGridReport::GetRowCount()
	{
		return m_pGridReport->GetRowCount();
	}
	int IGridReport::SetRow(int row)
	{
		m_pGridReport->SetRow(row);
		m_pGridReport->Redraw();
		return 0;
	}
	int IGridReport::GetColumnCount()
	{
		return m_pGridReport->GetColumnCount();
	}
	LPCTSTR IGridReport::GetColumnName(int col)
	{
		return m_pGridReport->GetColumnName(col);
	}
	int IGridReport::GetColumnIndex(LPCTSTR pColumn)
	{
		return m_pGridReport->GetColumnIndex(pColumn);
	}

	LPCTSTR  IGridReport::GetItemString(int row,LPCTSTR pColName,DWBuffer dwBuffer)
	{
		if(!pColName) return NULL;
		int i=0;
		for(i=0;i<=m_pGridReport->GetColumnCount();i++)
		{
			//2017.8.23 if(_tcsicmp(pColName,m_pGridReport->GetColumnName(i+1))==0)break;
			if(_tcsicmp(pColName,m_pGridReport->GetColumnName(i))==0)break;
		}
		if(i >m_pGridReport->GetColumnCount()) return NULL;
		//2017.8.23 return m_pGridReport->GetItemString(row,i + 1);
		return m_pGridReport->GetItemString(row,i);
	}
	LPCTSTR IGridReport::GetItemString(int row,int col,DWBuffer dwBuffer)
	{
		return m_pGridReport->GetItemString(row,col);
	}

	bool  IGridReport::SetItemString(int row,LPCTSTR pColName,LPCTSTR pData,DWBuffer dwBuffer)
	{
		int i=0;
		for(i=0;i<=m_pGridReport->GetColumnCount();i++)
		{
			//2017.8.23 if(_tcsicmp(pColName,m_pGridReport->GetColumnName(i+1))==0)break;
			if(_tcsicmp(pColName,m_pGridReport->GetColumnName(i))==0)break; 
		}
		if(i >m_pGridReport->GetColumnCount()) return false;
		//2017.8.23 if(m_pGridReport->SetItemString(row,i + 1,(LPTSTR)pData)< 0) return false;
		if(m_pGridReport->SetItemString(row,i,(LPTSTR)pData)< 0) return false;
		return true;
	}
	bool  IGridReport::SetItemString(int row,int col,LPCTSTR pData,DWBuffer dwBuffer)
	{
		if(m_pGridReport->SetItemString(row,col,(LPTSTR)pData)<0)return false;
		return true;
	}
	bool  IGridReport::SetItemStringEx(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		if(m_pGridReport->SetItemStringEx(nRow,(LPTSTR)ColumnName,(LPTSTR)lptStr)<0)return false;
		return true;
	}

	bool IGridReport::ItemChangeTo(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		event_ItemChanging(nRow,nColumn,lptStr);
		bool bret = SetItemString(nRow,nColumn,lptStr,dwBuffer);
		if(!bret) return bret;
		//2017.8.23	event_ItemChanged(nRow,nColumn - 1,lptStr);
		event_ItemChanged(nRow,nColumn,lptStr);
		return bret;
	}
	bool IGridReport::ItemChangeTo(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		int nColumn = GetColumnIndex(ColumnName);
		return ItemChangeTo(nRow,nColumn,lptStr,dwBuffer);
	}
	int IGridReport::InsertRow(int nRow)
	{
		return m_pGridReport->InsertRow(nRow);
	}
	int IGridReport::DeleteRow(int nRow)
	{
		if(nRow ==0) nRow = this->GetRow();
		if(nRow > m_pGridReport->GetRowCount()) nRow =  m_pGridReport->GetRowCount();
		return m_pGridReport->DeleteRow(nRow);
	}

	int  IGridReport::GetXml(BSTR &bstr)
	{
		return m_pGridReport->GetXml(bstr);
	}
	int  IGridReport::GetXml(BSTR &bstr,LPCTSTR pGuid)
	{
		return m_pGridReport->GetXml(bstr,(LPTSTR)pGuid);

	}
	int IGridReport::Load(IXMLDOMElement * pElement)
	{
		BSTR bstr;
		pElement->get_xml(&bstr);

		int nRet = m_pGridReport->SetXml(bstr);
		::SysFreeString(bstr);
		return nRet;
	}

	int  IGridReport::LoadXml(BSTR bstr)
	{
		return m_pGridReport->SetXml(bstr);
	}

	int IGridReport::SetReadOnly(bool bReadOnly)
	{
		return m_pGridReport->SetReadOnly(bReadOnly);
	}

	int IGridReport::SelectRow(int nRow,bool bSelect)
	{
		return m_pGridReport->SelectRow(nRow,bSelect);
	}
	int IGridReport::SelectRow(int sRow,int eRow, bool bSelect)
	{
		return m_pGridReport->SelectRow(sRow,eRow,bSelect);
	}
	int IGridReport::GetNextSelectRow(int nStartRow)
	{
		return m_pGridReport->GetNextSelectRow(nStartRow);
	}
	int IGridReport::GetPrevSelectRow(int nStartRow)
	{
		return m_pGridReport->GetPrevSelectRow(nStartRow);
	}
	bool IGridReport::IsRowSelected(int nRow)
	{
		return m_pGridReport->IsRowSelected(nRow);
	}
	int IGridReport::SetSelectBkColor(COLORREF color)
	{
		return m_pGridReport->SetSelectBkColor(color);
	}
	int IGridReport::SetSelectionMode(int nMode)
	{
		return m_pGridReport->SetSelectionMode(nMode);
	}
	int IGridReport::AddObserver(xbObserver * pObserver)
	{
		return m_pGridReport->AddObserver(pObserver);
	}

	int IGridReport::RemoveObserver(xbObserver * pObserver)
	{
		return m_pGridReport->RemoveObserver(pObserver);
	}
	int IGridReport::ClearObserver()
	{
		return m_pGridReport->ClearObserver();
	}

	IDwView * IGridReport::GetCurTable(int &nRow,int &nCol)
	{
		nRow = m_pGridReport->GetRow();
		nCol = m_pGridReport->GetCol();
		return GetCurTable();
	}
	IDwView * IGridReport::GetCurTable()
	{
		return this;
	}

	int IGridReport::DwUpdateTo(IXMLDOMDocument2 * pXmlDoc)
	{
#if 1
		int nColCount = GetColumnCount();

		LPTSTR pStr =  _tcsdup(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetXPath());
		if(!pStr) return -1;
		
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath = L"";
		LPTSTR XslPathParent =L"";

		for(int m=1;m<=m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetColCount();m++)
		{
			xslPaths.push_back(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetColumnName(m));
		}

		int pos = _tcslen(pStr) - 1;
		while(pStr[pos]!='/' && pos >=0) pos--;
		if(pos < 0)
		{
			delete pStr;
			return -1;
		}
		pStr[pos]= 0;
		XslPathParent = pStr;
		pXslPath = pStr + pos + 1;

		IXMLDOMNodeListPtr gNodes;
		IXMLDOMNodePtr gNode;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr Node;
		IXMLDOMNodePtr  currentNode;
		//IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodeListPtr pRowNodes;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodeList* pTempNodes; 
		IXMLDOMNodePtr NewNode;
		IXMLDOMElementPtr tNode;
		IXMLDOMElementPtr NewElement;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;
		wstring StrName;


		pXmlDoc->selectSingleNode(XslPathParent,&gNode);
		if(!gNode)
		{
			//if no exists gNode then create parent Node
			wstring StrPath = XslPathParent;
			StrPath =L"/" + StrPath;
			int nPosLast = -1;
			nPosLast = (int)StrPath.find_last_of(_T("/"));

			for(vector<IXMLDOMElement * >::const_iterator tr=vcElements.begin();tr!=vcElements.end();tr++)(*tr)->Release();
			vcElements.clear();

			wstring StrName;
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrName.compare(_T(""))!=0)
				{
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
					vcElements.push_back(pNewElement);

					pXmlDoc->selectSingleNode((LPTSTR)StrPath.c_str(),&gNode);
					if(gNode)break;

					nPosLast =(int) StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
				}
				for(int i=(int)vcElements.size() - 1;i>1;i--)
				{
					vcElements[i]->appendChild(vcElements[i - 1],NULL);
				}
				if(vcElements.size() > 0)
				{
					if(!gNode)
						pXmlDoc->appendChild(vcElements[vcElements.size() - 1],NULL);
					else
						gNode->appendChild(vcElements[vcElements.size() - 1],NULL);
				}
			}else
			{
				StrName = StrPath;
				if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
				if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
				StrPath = _T("");
			}
		}
		pXmlDoc->selectSingleNode(XslPathParent,&gNode);

		gNode->selectNodes(pXslPath,&pRowNodes);
		long nRowLen;
		pRowNodes->get_length(&nRowLen);
		int row = 1;
		int trow = 0;
		for(row=1;row <= GetRowCount();row++)
		{
			trow++;
			if(row <= nRowLen)
			{
				pRowNodes->get_item(row -1,&Node);
				for(int i=1 ;i<=(int)xslPaths.size();i++)
				{
					Node->selectSingleNode(xslPaths[i - 1],&currentNode);
					if(!currentNode)
					{
						//create not exits column node
						wstring path = xslPaths[i - 1];
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
						if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
						if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

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
								pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
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
									//((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									NewElement->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									
									continue;
								}else
								{
									pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
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
						//end create not exists column
					}
				}
			}else
			{
				//create data frame
				if(!tNode)
				{
					//create template data node
					wstring StrPath = pXslPath;
					int posLast = (int)StrPath.find_last_of(_T("/"));
					if(posLast>=0)
						StrPath = StrPath.substr(posLast + 1);
					else
						StrName = StrPath;
					
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&tNode);

					//end create template data node
					for(int i=1 ;i<=(int)xslPaths.size();i++)
					{
						wstring path = xslPaths[i - 1];
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
						if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
						if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
						if(StrName[0]=='@')
						{
							StrName = StrName.substr(1);
							((IXMLDOMElementPtr)tNode)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
							continue;
						}else
						{
							pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
							tNode->appendChild(NewElement,NULL);
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
							if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
							if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
							if(StrName[0]=='@')
							{
								StrName = StrName.substr(1);
								//((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
								NewElement->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
								continue;
							}else
							{
								pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
								NewElement->appendChild(NewElement1,NULL);
								NewElement = NewElement1;
							}
						}//while
					}//next
				}//end if tnod
				//end create data frame

				tNode->cloneNode(VARIANT_TRUE,&Node);
				gNode->appendChild(Node,NULL);
			}//row
			
			//set data
			//set key value
			//long ll_value;
			
			/*
			if(!pDwTable->m_guidKeys[trow - 1])
			{
				GUID guid;
				CoCreateGuid(&guid);
				LPTSTR pStrGuid = new TCHAR[48];
				StringFromGUID2(guid,pStrGuid,48);
				((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pStrGuid);
				pDwTable->m_guidKeys[trow - 1] = pStrGuid;
			}else
			{
				((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pDwTable->m_guidKeys[trow - 1]);
			}*/
			((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)m_pGridReport->GetItemString(trow,0));


			LPTSTR pStrValue;
			DOMNodeType	iNodeType;
			for(int col=1;col <= (int)xslPaths.size();col++)
			{
				pStrValue = (LPTSTR)m_pGridReport->GetItemString(trow,col);
				currentNode = NULL;
				Node->selectSingleNode(xslPaths[col - 1],&currentNode);
				if(currentNode)
				{
					currentNode->get_nodeType(&iNodeType);
					if(iNodeType == NODE_ELEMENT)
					{
						if(!pStrValue)
						{
							((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("1"));
							currentNode->put_text(L"");
						}else
						{
							((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("0"));
							((IXMLDOMElementPtr)currentNode)->removeAttribute(L"isnull");
							currentNode->put_text(pStrValue);
							//if(pItem && pItem->pStrText)
							//	((IXMLDOMElementPtr)currentNode)->setAttribute(L"_displaystring",(_variant_t)pItem->pStrText);
						}
					}
					else //attribute
					{
						if(!pStrValue)
							currentNode->put_text(L"");
						else
						{
							currentNode->put_text(pStrValue);
						}
					}//NodeType
				}//currentNode
			}//for
		}//for row

		IXMLDOMNode * pNodeItem;
		for(int k = nRowLen;k>=row;k--)
		{
			pRowNodes->get_item( k - 1,&pNodeItem);
			gNode->removeChild(pNodeItem,NULL);
			pNodeItem->Release();
		}
		//end update data to xml node
		return 1;
#endif 		
		
		
		//m_pGridReport->DwUpdate(pXmlDoc);
		BSTR	bstr;
		m_pGridReport->GetXml(bstr);
		
		VARIANT_BOOL vRet;
		pXmlDoc->loadXML(bstr,&vRet);
		::SysFreeString(bstr);
		

		return 1;
	}

	int IGridReport::DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc)
	{
		return DwUpdateTo(pXmlDoc);
	}

	int IGridReport::GetBIData(IXMLDOMDocument2 * pXmlDoc)
	{
		m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetBIData(pXmlDoc);
		return 1;
	}


	int IGridReport::DwUpdateAllToEx(IXMLDOMDocument2 * pXmlDoc)
	{
		BSTR	bstr;
		m_pGridReport->GetXml(bstr);
		
		VARIANT_BOOL vRet;
		pXmlDoc->loadXML(bstr,&vRet);
		::SysFreeString(bstr);

		return 1;
	}

	int IGridReport::Retrieve(ITrans * pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!pTrans) return -1;
		return Retrieve(pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}
	int IGridReport::Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!m_pTrans) return -1;
		return Retrieve(m_pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}

	int IGridReport::Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		TCHAR buf[512];
		
		if(_tcsstr(pDataUrl,_T("uriserver/")) == pDataUrl)
			pDataUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pDataUrl,_T("dev:xquery"))==pDataUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pDataUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pDataUrl);

		xbXml xmlRpt;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			((XXmlContentSvr *)m_pGridReport->GetGridDataSvr()->m_pContentSvr)->m_pvcTreeData = NULL;
			BSTR bstr;
			xmlRpt.m_pXmlDoc->get_xml(&bstr);
			m_pGridReport->SetXml(bstr);
			::SysFreeString(bstr); 
			m_pGridReport->Initialize();
			m_pGridReport->SetRow(1);
			m_pGridReport->SelectRow(0,false);

			if(m_pGridReport->GetVScrollBar())m_pGridReport->GetVScrollBar()->SetPosition(0);
			if(m_pGridReport->GetHScrollBar())m_pGridReport->GetHScrollBar()->SetPosition(0);
			
			m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->/*GetGridDataSvr()->m_pContentSvr->*/GetRowCount();
			if(m_pGridReport->GetVScrollBar()->nMax<=1 )
				m_pGridReport->GetVScrollBar()->nMax = 10;
		}

		return m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();
	}

	int IGridReport::Retrieve(xbXml  &xml)
	{
		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int nRet = -1;
		if(pElement)
		{
			nRet =  Retrieve(pElement);
			pElement->Release();
		}
		return nRet;
	}

	int IGridReport::Retrieve(IXMLDOMElement * pElement)
	{
		this->AcceptText();
		m_pGridReport->GetGridDataSvr()->m_pContentSvr->Load(pElement);
		m_pGridReport->Initialize();
		m_pGridReport->SetRow(1);
		m_pGridReport->SelectRow(0,false);

		if(m_pGridReport->GetVScrollBar())m_pGridReport->GetVScrollBar()->SetPosition(0);
		if(m_pGridReport->GetHScrollBar())m_pGridReport->GetHScrollBar()->SetPosition(0);
		
		m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->/*GetGridDataSvr()->m_pContentSvr->*/GetRowCount();
		if(m_pGridReport->GetVScrollBar()->nMax<=1 )
			m_pGridReport->GetVScrollBar()->nMax = 10;
		return m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();
	}

	int IGridReport::SetDataObject(ITrans * pTrans,LPCTSTR pUrl)
	{
		if(!pTrans) return -1;
		return SetDataObject(pTrans->GetServerUrl(),pUrl);
	}
	int IGridReport::SetDataObject(LPCTSTR pUrl)
	{
		if(!m_pTrans) return -1;
		return SetDataObject(m_pTrans->GetServerUrl(),pUrl);
	}

	int IGridReport::SetDataObject(LPCTSTR pServer,LPCTSTR pUrl)
	{
		TCHAR buf[512];
		
		m_pGridReport->FCurrent.y = 0;
		m_pGridReport->FCurrent.x = 0;

		if(_tcsstr(pUrl,_T("uriserver/")) == pUrl)
			pUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pUrl,_T("dev:xpage"))==pUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pUrl);

		xbXml xmlRpt;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			IXMLDOMElement * pElement;
			xmlRpt.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				m_pGridReport->m_pSheetSvr->LoadTemplate(pElement);
				pElement->Release();
				if(m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetHWND()) ::InvalidateRect(m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetHWND(),NULL,false);
				
				m_pGridReport->Initialize();
				m_pGridReport->SetRow(1);
				m_pGridReport->SelectRow(0,false);
				if(m_pGridReport->GetVScrollBar())m_pGridReport->GetVScrollBar()->SetPosition(0);
				if(m_pGridReport->GetHScrollBar())m_pGridReport->GetHScrollBar()->SetPosition(0);
				
				m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();
				if(m_pGridReport->GetVScrollBar()->nMax<=1 )
					m_pGridReport->GetVScrollBar()->nMax = 10;
				m_pGridReport->FRect.top = m_pGridReport->m_pSheetSvr->m_pColHeaderSvr->GetTitleHeight();
				//m_pGridReport
				return m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();

			}
		}else
		{

		}
		return 1;
	}

	int IGridReport::SetDataObject(IXMLDOMElement * pElement)
	{
		return m_pGridReport->m_pSheetSvr->LoadTemplate(pElement);
	}
	
	int IGridReport::SetDataObject(IXMLDOMDocument * pDoc)
	{
		IXMLDOMElement *  pElement;
		//IXMLDOMELement *  pElement;
		pDoc->get_documentElement(&pElement);
		if(pElement)
		{
			int nRet= SetDataObject(pElement);
			pElement->Release();
			return nRet;
		}
		return-1;
	}

	int IGridReport::PostRetrieve()
	{
		//reset scroll max
		m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->GetRowCount();
		if(m_pGridReport->GetVScrollBar()->nMax<=1 )
			m_pGridReport->GetVScrollBar()->nMax = 10;
		return 1;
	}

	int IGridReport::Reset()
	{
		m_pGridReport->Initialize();
		m_pGridReport->SetRow(1);
		m_pGridReport->SelectRow(0,false);

		if(m_pGridReport->GetVScrollBar())m_pGridReport->GetVScrollBar()->SetPosition(0);
		if(m_pGridReport->GetHScrollBar())m_pGridReport->GetHScrollBar()->SetPosition(0);
		
		m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->/*GetGridDataSvr()->m_pContentSvr->*/GetRowCount();
		if(m_pGridReport->GetVScrollBar()->nMax<=1 )
			m_pGridReport->GetVScrollBar()->nMax = 10;

		m_pGridReport->GetGridDataSvr()->m_pContentSvr->Reset();
		return 1;
	}

	int	IGridReport::ResetUpdateStatus()
	{
		return m_pGridReport->GetGridDataSvr()->m_pContentSvr->ResetUpdateStatus();
	}

	int IGridReport::ImportFile()
	{
		TCHAR filter[]= _T("TXT 文件(*.txt)\0*.txt\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH];
		TCHAR szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("TXT 文件(*.txt)\0*.txt\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_hWnd;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = szCustomFilter;
		ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
		ofn.lpstrTitle = _T("文件选择");
		ofn.lpstrDefExt = _T("txt");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if(::GetOpenFileName(&ofn))
		{
			return ImportFile(szPath);			
		}else
			return 0;
	}
	int IGridReport::ImportFile(LPCTSTR pStrFile,LPCTSTR pType,bool Header)
	{
		HANDLE hFile = ::CreateFile(pStrFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		DWORD  nLen = ::GetFileSize(hFile,NULL);
		DWORD   dwSize;
		BYTE* pData = new BYTE[nLen + 2];
		ReadFile(hFile,pData,nLen,&dwSize,NULL);
		CloseHandle(hFile);
		if(nLen < 2) return 1;
		pData[nLen] = 0;
		pData[nLen + 1] = 0;
		
		LPTSTR pStr;
		bool bFree = false;
		if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
		{
			pStr = ((LPTSTR)(pData + 2));
		}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
		{
			int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
			psz[_convert]='\0';
			pStr = psz;
			bFree = true;
		}else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
		{
			int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
			psz[_convert]='\0';
			pStr = psz;
			bFree = true;
		}
		int nRet = ImportString(pStr,pType,Header);

		if(bFree && pStr) delete pStr;
		if(pData)delete pData;

		return nRet;
	}

	int IGridReport::ImportString(LPCTSTR pStr,LPCTSTR pType,bool Header )
	{
		int row,col;
		int pos,start;
		int /*RowCount,*/colCount;
		int nLen ;
		TCHAR ch;
		TCHAR buffer[1024];
		BOOL  finish = false;
		finish = false;
		pos = 0;
		
		if(!pStr) return 0;
		if(Header)
		{
			pStr = _tcschr(pStr,'\n');
			if(!pStr) return 0;
			pStr++;
		}
		nLen = (int)_tcslen(pStr);
		if(nLen < 1) return 0;
		
		colCount = GetColumnCount();
		while(!finish)
		{
			//insert data row
			InsertRow(0);
			row = GetRowCount();
			for(col=1;col<=colCount;col++)
			{
				start = pos;
				do
				{
					ch = *(pStr+pos);
					buffer[pos - start]=ch;
					pos++;
				}while(pos <nLen && ch !='\0' && ch !='\t' &&ch!='\r' && ch!='\n');
				if(ch=='\t')buffer[pos-start - 1]='\0';
				if(ch=='\r')buffer[pos-start - 1]='\0';
				if(ch=='\n')buffer[pos-start - 1]='\0';
				SetItemString(row,col,buffer);
				if(pos == nLen ||ch=='\0' )
				{
					finish = true;
					break;
				}
				
				if(ch=='\t') continue;
				if(ch=='\r')
				{
					ch = *(pStr+pos);
					if(ch=='\n')pos++;
				}
				break;
			}
		}
		return 1;
	}

	int IGridReport::Modify(LPCTSTR pStr)
	{
		return -99;
	}

	int IGridReport::event_ItemFocusChanged(int nRow,int nCol)
	{
		return -99;
	}
	int IGridReport::event_RowFocusChanged(int nRow,int nOldRow)
	{
		return -99;
	}
	int IGridReport::event_GetFocus( )
	{
		return -99;
	}
	int IGridReport::event_LoseFocus()
	{
		return -99;
	}
	int IGridReport::event_ItemChanged(int nRow,int nCol,LPCTSTR pData)
	{
		return m_pGridReport->event_ItemChanged(nRow, nCol, (LPTSTR)pData);
		//return -99;
	}
	int IGridReport::event_ItemChanging(int nRow,int nCol,LPCTSTR pData)
	{
		return -99;
	}
	int IGridReport::GetDwType()
	{
		return DWT_GRID;
	}
	
	IXMLDOMElement * IGridReport::GetRowElement(int nRow)
	{
		return m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowElement(nRow);
	}

	int IGridReport::DwShareTo(IGridReport * pPrimaryReport)
	{
		m_pGridReport->DwShareTo(pPrimaryReport->m_pGridReport);
		return 1;
	}

	int IGridReport::SetAskStyle(bool bAskStyle)
	{
		return m_pGridReport->SetAskMask(bAskStyle);

	}
	int IGridReport::SetColumnProps(int nCol,LPCTSTR pXmlProp)
	{
		return m_pGridReport->SetColumnProps(nCol,pXmlProp);
	}
	int IGridReport::SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp)
	{
		int nCol;
		nCol = GetColumnIndex(pColName);
		if(nCol < 1) return NULL;
		return  SetColumnProps(nCol,pXmlProp);
	}
	LPTSTR IGridReport::GetColumnProp(int nCol,LPCTSTR pItem)
	{
		return m_pGridReport->GetColumnProp(nCol,pItem);
	}
	LPTSTR IGridReport::GetColumnProp(LPCTSTR pColName,LPCTSTR pItem)
	{
		int nCol;
		nCol = GetColumnIndex(pColName);
		if(nCol < 1) return NULL;
		//2017.8.23	return  GetColumnProp(nCol - 1,pItem);
		return  GetColumnProp(nCol,pItem);
	}
	int IGridReport::CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2)
	{
		return m_pGridReport->CreateTree(pPath,nCol, nImage1,nImage2);
	}
	LPCTSTR IGridReport::GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer)
	{
		return m_pGridReport->GetItemDisplayString(nRow,nColumn);
	}
	bool IGridReport::SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		return m_pGridReport->SetItemDisplayString(nRow,nColumn, lptStr);
	}
	LPCTSTR IGridReport::GetItemDisplayString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer)
	{
		int i=0;
		for(i=0;i<=m_pGridReport->GetColumnCount();i++)
		{
			//2017.8.14 if(_tcsicmp(ColumnName,m_pGridReport->GetColumnName(i+1))==0)break;
			if(_tcsicmp(ColumnName,m_pGridReport->GetColumnName(i))==0)break;
		}
		if(i >m_pGridReport->GetColumnCount()) return NULL;
		//2017.8.14 return m_pGridReport->GetItemDisplayString(nRow,i + 1);
		return m_pGridReport->GetItemDisplayString(nRow,i);
	}
	bool IGridReport::SetItemDisplayString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		int i=0;
		for(i=0;i<=m_pGridReport->GetColumnCount();i++)
		{
			//2017.8.14 if(_tcsicmp(ColumnName,m_pGridReport->GetColumnName(i+1))==0)break;
			if(_tcsicmp(ColumnName,m_pGridReport->GetColumnName(i))==0)break;
		}
		if(i >m_pGridReport->GetColumnCount()) return NULL;
		//2017.8.14  return m_pGridReport->SetItemDisplayString(nRow,i + 1, lptStr);
		return m_pGridReport->SetItemDisplayString(nRow,i, lptStr);
	}
	int IGridReport::AllowSort(bool sort)
	{
		return m_pGridReport->AllowSort(sort);
	}
	int IGridReport::FindByColumn(int startRow, int columnIndex, LPCTSTR columnValue)
	{
		return m_pGridReport->FindByColumn(startRow, columnIndex, columnValue);
	}
	int IGridReport::FindByColumn(int startRow, LPCTSTR column, LPCTSTR columnValue)
	{
		return m_pGridReport->FindByColumn(startRow, column, columnValue);
	}
	int IGridReport::SetReadOnlyColumn(int columnIndex, bool flag)
	{
		return m_pGridReport->SetReadOnlyColumn(columnIndex, flag);
	}
	int IGridReport::SetReadOnlyColumn(LPCTSTR column, bool flag)
	{
		return m_pGridReport->SetReadOnlyColumn(column, flag);
	}
	int IGridReport::SetColumnWidth(int columnIndex, int width)
	{
		return m_pGridReport->SetColumnWidth(columnIndex, width);
	}
	int IGridReport::SetColumnWidth(LPCTSTR column, int width)
	{
		int columnIndex = m_pGridReport->GetColumnIndex(column);
		if(columnIndex < 0)
			return -1;
		//2017.8.23	return SetColumnWidth(columnIndex - 1, width);
		return SetColumnWidth(columnIndex, width);
	}
	int IGridReport::GetColumnWidth(int columnIndex)
	{
		return m_pGridReport->GetColumnWidth(columnIndex);
	}
	int IGridReport::SetColumnState(int columnIndex, bool state)
	{
		return m_pGridReport->SetColumnState(columnIndex, state);
	}
	int IGridReport::SetColumnState(LPCTSTR column, bool state)
	{
		int columnIndex = m_pGridReport->GetColumnIndex(column);
		if(columnIndex < 0)
			return -1;
		//2017.8.23	return SetColumnState(columnIndex - 1, state);
		return SetColumnState(columnIndex , state);
	}
	int IGridReport::GetColumnWidth(LPCTSTR column)
	{
		int columnIndex = m_pGridReport->GetColumnIndex(column);
		if(columnIndex < 0)
			return -1;
		//2017.8.23	return GetColumnWidth(columnIndex - 1);
		return GetColumnWidth(columnIndex - 1);
	}
	void IGridReport::SetDataToClip(LPCTSTR DataStr)     
	{
		return m_pGridReport->SetDataToClip(DataStr);
	}
	int IGridReport::AcceptText()
	{
		m_pGridReport->HideEdit();
		return 0;
	}
	int IGridReport::ShowRowTo(int row)
	{
		return m_pGridReport->ShowRowTo(row);
	}
	int IGridReport::GetShowRow()
	{
		return m_pGridReport->GetTopRow();
	}
	int IGridReport::SaveAs(LPCTSTR pXmlArg)
	{	
		int Rows = m_pGridReport->GetRowCount();
		int Cols = m_pGridReport->GetColCount();
		int i, j;
		//std::wstring html = L"<html xmlns:x='urn:schemas-microsoft-com:office:excel'><body><table x:str border=1>";
		std::wstring html = L"<html xmlns:x='urn:schemas-microsoft-com:office:excel'><body><table  border=1>";
		//表头
		CReportBandSvr::TReportBand *  pReportBand = &(m_pGridReport->m_pDataTempl)->m_ReportBand->m_ReportBand;
		int startRow,endRow;
		startRow = pReportBand->Title.startRow;
		endRow = pReportBand->Title.endRow - 1;
		for(i=startRow; i<=endRow; i++)
		{
			html += L"<tr>";
			for(j=1; j<=Cols; j++)
			{
				int ColWidth = m_pGridReport->GetColumnWidth(j);
				if(ColWidth < 2) continue;
				PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(j, i);
				if(pItem && pItem->pStrContent)
					html += L"<td>" + (std::wstring)pItem->pStrContent + L"</td>";
				else
					html += L"<td></td>";
			}
			html += L"</tr>";
		}
		//内容
		for(i=1; i<=Rows; i++)
		{
			html += L"<tr>";
			for(j=1; j<=Cols; j++)
			{
				int ColWidth = m_pGridReport->GetColumnWidth(j);
				if(ColWidth < 2) continue;
				//PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(i, j);
				LPCTSTR pStr = m_pGridReport->GetItemString(i, j);
				LPCTSTR pDisStr = m_pGridReport->GetItemDisplayString(i, j);
				if(pDisStr)
					html += L"<td>" + (std::wstring)pDisStr + L"</td>";
				else if(pStr)
					html += L"<td>" + (std::wstring)pStr + L"</td>";
				else
					html += L"<td></td>";
			}
			html += L"</tr>";
		}
		html += L"</table></body></html>";
		int ret = IXFUtil::SaveToFile(L"",(LPTSTR)html.c_str(),L"xls",L"Excel 文件(*.xls)\0*.xls\0全部文件(*.*)\0*.*\0",(LPTSTR)pXmlArg);
		return ret;
	}
	int IGridReport::SaveAs(xbXml &xml, LPCTSTR pFileName)
	{	
		LPTSTR pStr = m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetXPath();
		if(!pStr) return -1;

		int i, j;
		int Cols = m_pGridReport->GetColCount();
		//std::wstring html = L"<html xmlns:x='urn:schemas-microsoft-com:office:excel'><body><table x:str border=1>";
		std::wstring html = L"<html xmlns:x='urn:schemas-microsoft-com:office:excel'><body><table  border=1>";
		//表头
		CReportBandSvr::TReportBand *  pReportBand = &(m_pGridReport->m_pDataTempl)->m_ReportBand->m_ReportBand;
		int startRow,endRow;
		startRow = pReportBand->Title.startRow;
		endRow = pReportBand->Title.endRow - 1;
		for(i=startRow; i<=endRow; i++)
		{
			html += L"<tr>";
			for(j=1; j<=Cols; j++)
			{
				int ColWidth = m_pGridReport->GetColumnWidth(j);
				if(ColWidth < 2) continue;
				PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(j, i);
				if(pItem && pItem->pStrContent)
					html += L"<td>" + (std::wstring)pItem->pStrContent + L"</td>";
				else
					html += L"<td></td>";
			}
			html += L"</tr>";
		}
		//数据
		IXMLDOMNodeList * pLists;
		IXMLDOMNode * pNode;
		IXMLDOMNode * pCellNode;
		long nLen;
		xml.m_pXmlDoc->selectNodes(pStr,&pLists);
		pLists->get_length(&nLen);
		BSTR sz;
		for(int i=0;i<nLen;i++)
		{
			pLists->get_item(i,&pNode);
			html += L"<tr>";
			for(j=1; j<=Cols; j++)
			{
				int ColWidth = m_pGridReport->GetColumnWidth(j );
				if(ColWidth < 2) continue;
				pNode->selectSingleNode((BSTR)m_pGridReport->GetColumnName(j), &pCellNode);
				if(pCellNode)
				{
					pCellNode->get_text(&sz);
					html += L"<td>" + (std::wstring)sz + L"</td>";
				}
				else
					html += L"<td></td>";
			}			
			html += L"</tr>";
		}
		html += L"</table></body></html>";
		int ret = IXFUtil::SaveToFile(L"",(LPTSTR)html.c_str(),L"xls",L"Excel 文件(*.xls)\0*.xls\0全部文件(*.*)\0*.*\0",(LPTSTR)pFileName);
		::SysFreeString(sz);
		return ret;
	}
	LPCTSTR IGridReport::GetTitleName(int col)
	{
		return m_pGridReport->GetTitleName(col);
	}
	int IGridReport::DataBI(LPCTSTR pSchema)
	{
		LPTSTR bstr = DataAnalysis(pSchema);
		if(bstr)
		{
			IXAServer::__CreateDirectory(L"C:/Temp");
			LPTSTR filepath = L"C:/Temp/1.htm";
			Hxsoft::XUtil::SaveToFile(filepath, bstr, _T(""),_T(""),_T(""));
			::ShellExecute(0,L"open",filepath,_T(""),_T(""),5);	
			::SysFreeString(bstr);
			return 1;
		}
		return -1;
	}
	LPTSTR IGridReport::DataAnalysis(LPCTSTR pSchema)
	{
		if(FDataBISchema) delete FDataBISchema;
		FDataBISchema = NULL;
		if(pSchema && _tcscmp(pSchema, _T("")) != 0)
		{
			FDataBISchema = (LPTSTR)pSchema;
		}
		else
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfWin* pCurWin = m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetWin();
			xfUriLoader::UrlCombineW(pCurWin->m_pXPage->m_pStrUrl,L"/business/DataAnalysis.xpage",strUrl,&dwSize,0);
			xfWin* pWin = xfWin::LoadXPage(NULL,strUrl,pCurWin->GetHWND(),xfApp::GetApp()->GetInstance(),(LPVOID)this);
		}
		xbXml xml;
		BSTR bstr = NULL;
		if(FDataBISchema && _tcscmp(FDataBISchema, _T("")) != 0 && xml.LoadXml(FDataBISchema))
		{
			xbXml xsl;
			LPTSTR pServerUrl = m_pGridReport->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl;
			if(xfUriLoader::FetchXml(pServerUrl,_T("dev:xml.xslt[exch.xslt]"),NULL, xsl)!=1)
			{
				return NULL;
			}
			//xsl.m_pXmlDoc->get_xml(&bstr);
			xsl.m_pXmlDoc->setProperty(L"AllowXsltScript",(variant_t)true);
			HRESULT hr = xml.m_pXmlDoc->transformNode(xsl.m_pXmlDoc,&bstr);

			BSTR bxslt = NULL;
			if(xfUriLoader::FetchXml(pServerUrl,_T("dev:xml.xslt[exch.data.xslt]"),NULL, xsl)!=1)
			{
				return NULL;
			}
			xml.m_pXmlDoc->transformNode(xsl.m_pXmlDoc,&bxslt);
			xsl.LoadXml(wcsstr(bxslt,L"<xsl:stylesheet"));
			BSTR x = xsl.GetXml();

			BSTR bdata = NULL;
			xbXml content;
			//this->DwUpdateAllToEx(content.m_pXmlDoc);
			if(m_pGridReport->hasBindData())
			{
				IXMLDOMNodeList* pNodes = 0;
				xml.GetXmlDoc()->selectNodes(L"//item/@column",&pNodes);
				LONG len = 0;
				pNodes->get_length(&len);
				std::set<wstring> columns;
				for(int k=0; k < len; k++)
				{
					IXMLDOMNode* pNode = NULL;
					pNodes->get_item(k,&pNode);
					BSTR column = NULL;
					pNode->get_text(&column);
					if(pNode)pNode->Release();
					if(column)
					{
						columns.insert(column);
						::SysFreeString(column);
					}
				}
				pNodes->Release();
				std::wstring columnStr;
				for(std::set<wstring>::iterator tr = columns.begin();tr != columns.end();tr++)
				{
					if(tr != columns.begin()) columnStr += L",";
					columnStr += *tr;
				}
				wchar_t* pBIData = m_pGridReport->m_pSheetSvr->m_pSheetCtrl->bindGetData(L"",-1,(wchar_t*)(L"bidata:"+columnStr).c_str());
				if(pBIData)
				{
					content.LoadXml(pBIData);
					if(pBIData) delete pBIData;
				}
			}
			else
				GetBIData(content.m_pXmlDoc);

			BSTR y = content.GetXml();
			content.m_pXmlDoc->transformNode(xsl.m_pXmlDoc, &bdata);
			content.LoadXml(bdata);
		
			xsl.LoadXml(bstr);
			/*
			IXAServerArg arg;
			arg.AddArg(_T("content"),content.GetXml());
			xml.LoadXml(L"<root/>");
			IXMLDOMElement * pDocElement;
			IXMLDOMElement * pElement =  NULL;
			xml.m_pXmlDoc->get_documentElement(&pDocElement);
			for(int i=1; i<=GetColumnCount(); i++)
			{
				LPTSTR cname = (LPTSTR)GetTitleName(i);
				LPTSTR ename = (LPTSTR)GetColumnName(i);
				if(cname)
				{
					xml.m_pXmlDoc->createElement(L"item",&pElement);
					pElement->setAttribute(L"cname", (_variant_t)cname);
					pElement->setAttribute(L"ename", (_variant_t)ename);
					pDocElement->appendChild(pElement,NULL);
				}
			}
			pDocElement->get_xml(&bstr);
			arg.AddArg(_T("column"), bstr);
			if(pElement) pElement->Release();
			if(pDocElement)pDocElement->Release();
			if(IXAServer::ExecXQuery(pServerUrl,_T("[vanalysis.xq]"),arg.GetString(),xml) != 1)
			{
				bstr = xml.GetXml();
				//::SysFreeString(bstr);
				//::MessageBox(NULL,_T("数据分析失败!"),_T("提示!"),MB_OK);
				//return -1;
			}
			else
			{
				bstr = xml.GetXml();
				xml.m_pXmlDoc->transformNode(xsl.m_pXmlDoc,&bstr);
			}
			*/
			content.m_pXmlDoc->transformNode(xsl.m_pXmlDoc,&bstr);
			//::SysFreeString(bstr);
		}
		return _tcsdup(bstr);
	}
	int IGridReport::SetRowSort(LPTSTR colName, bool colOrder)
	{
		return m_pGridReport->SetRowSort(colName, colOrder);
	}
	int IGridReport::SetClickEditFlag(bool f)
	{
		return m_pGridReport->SetClickEditFlag(f);
	}
	int IGridReport::RedrawEx()
	{
		return m_pGridReport->Redraw();
	}
	bool IGridReport::SetRowMove(int row, int movecount)
	{
		return m_pGridReport->SetRowMove(row, movecount);
	}
	int IGridReport::CreateTreeEx(LPCTSTR pPath,int nCol, int nImage1,int nImage2)
	{
		IXMLDOMElementPtr pSchemaElement;
		vector<XOffice::XXmlContentSvr::TTreeItem>* pTreeData = NULL;
		if(!pTreeData) pTreeData = new vector<XOffice::XXmlContentSvr::TTreeItem>;
		vector<XOffice::XXmlContentSvr::TTreeItem>* pTreeDataOld = NULL;
		((XXmlContentSvr *)m_pGridReport->GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeDataOld);
		XOffice::XXmlContentSvr::TTreeItem item;
		TCHAR xPath[255];
		::_stprintf_s(xPath,255,_T("//%s"), pPath);
		m_pGridReport->GetGridDataSvr()->m_pContentSvr->SetXPath(_tcsdup(xPath));
		//delete xPath;
		for(int i=0;i<GetRowCount();i++)
		{
			IXMLDOMElement * pElement;
			_variant_t val;
			pElement = GetRowElement(i + 1);
			_variant_t pGuid;
			pElement->getAttribute(L"guid", &pGuid);
			item.pGuid = NULL;
			if(pGuid.bstrVal) item.pGuid = _tcsdup((LPTSTR)pGuid.bstrVal);			

			IXMLDOMNode * pItem;
			pElement->selectSingleNode((BSTR)pPath,&pItem);
			if(pItem)
			{
				item.children = true;
				/*if(i==0)
					item.expanded = true;
				else*/
					item.expanded = false;
				item.nImage = nImage1;
				pItem->Release();
			}else
			{
				item.children =  false;
				/*if(i==0)
					item.expanded = true;
				else*/
					item.expanded = false;
				item.nImage = nImage2;
			}
			item.nContentCol = nCol;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			item.nLevel = -1;
			if(i>=0)
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
			if(pTreeDataOld && item.children && item.pGuid)
			{
				for(int j=0;j<(int)pTreeDataOld->size();j++)
				{
					if((*pTreeDataOld)[j].pGuid && _tcscmp((*pTreeDataOld)[j].pGuid, item.pGuid) == 0)
					{
						item.expanded = (*pTreeDataOld)[j].expanded;
					}
				}							
			}
			pTreeData->push_back(item);
			pElement->Release();
		}
		m_pGridReport->GetGridDataSvr()->m_pContentSvr->SetTreeData(pTreeData);
		return 1;
	}
	int IGridReport::GetTreeRow(int row)
	{
		return m_pGridReport->GetTreeRow(row);
	}
	int IGridReport::GetTreeRowCount()
	{
		return m_pGridReport->GetTreeRowCount();
	}
	int IGridReport::TreeRowExpand(int row, bool exp)
	{
		return m_pGridReport->TreeRowExpand(row, exp);
	}
	int IGridReport::RetrieveDiff(xbXml &xml)
	{
		//mark change diff to content
		((XXmlContentSvr *)m_pGridReport->GetGridDataSvr()->m_pContentSvr)->FShowOrigin = true;
		((XXmlContentSvr *)m_pGridReport->GetGridDataSvr()->m_pContentSvr)->SetXPath(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetXPath());
		return 1;
	}
	int IGridReport::Filter(LPTSTR pColumn, LPTSTR pStr)
	{
		return m_pGridReport->m_pSheetSvr->m_pColHeaderSvr->ColumnQuery(pColumn, pStr);
	}
	int IGridReport::SetHeaderText(int row, int col, LPTSTR name)
	{
		return m_pGridReport->SetHeaderText(row, col, name);
	}
	int IGridReport::MoveColumnTo(int fromcolumn, int tocolumn)
	{
		if(fromcolumn < 1 || fromcolumn > GetColumnCount()) return -1;
		if(tocolumn < 1 || tocolumn > GetColumnCount()) return -1;
		return m_pGridReport->m_pSheetSvr->m_pColHeaderSvr->MoveColumn(fromcolumn, tocolumn);
	}

	void IGridReport::SetEditUpperMode(bool upper)
	{
		m_pGridReport->SetEditUpperMode(upper);
	}
	bool IGridReport::GetEditUpperMode()
	{
		return m_pGridReport->GetEditUpperMode();
	}

}}}}
