#include "stdafx.h"
#include "griddatasvr.hpp"
#include "dwtable.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CGridDataSvr::CGridDataSvr(void):m_enView(envNormal),m_pChilds(new std::vector<TChildSvr * >),m_pSchemaSource(NULL),m_pSchemaStr(NULL)
	{
		m_pAxisSvr = new CAxisSvr;
		m_pContentSvr = new CContentSvr;
		m_pSpanSvr = new CSpanRectSvr;
		m_pFormatSvr = new CFormatSvr;
		m_pNameSvr = new CNameSvr;
		m_pNoteSvr = new CNoteSvr;
		m_pTemplDefSvr = new CTemplDefSvr;
		m_pAttribSvr = new CAttribSvr;
		delete m_pStyleSvr;
		m_pStyleSvr = new XCellStyleSvr;
		m_ReportBand = new CReportBandSvr;
	}
	
	CGridDataSvr::~CGridDataSvr(void)
	{
		if(m_pAxisSvr)delete m_pAxisSvr;
		if(m_pContentSvr)delete m_pContentSvr;
		if(m_pSpanSvr)delete m_pSpanSvr;
		if(m_pFormatSvr)delete m_pFormatSvr;
		if(m_pNameSvr)delete m_pNameSvr;
		if(m_pNoteSvr)delete m_pNoteSvr;
		if(m_pTemplDefSvr)delete m_pTemplDefSvr;
		if(m_pAttribSvr)delete m_pAttribSvr;
		if(m_ReportBand) delete m_ReportBand ;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if((*m_pChilds)[i])delete (*m_pChilds)[i] ; 
		}
		if(m_pChilds) delete m_pChilds;
		if(m_pSchemaSource)delete m_pSchemaSource;
		if(m_pSchemaStr)delete m_pSchemaStr;
	}
	XCellStyleSvr*		CGridDataSvr::GetStyleSvr()
	{
		return dynamic_cast<XCellStyleSvr*>(m_pStyleSvr);
	}
	
	void	CGridDataSvr::SetSchmaSource(LPCTSTR pSchema)
	{
		if(m_pSchemaSource)delete m_pSchemaSource;
		m_pSchemaSource = _tcsdup(pSchema);
	}
	LPCTSTR	CGridDataSvr::GetSchemaSource()
	{
		return m_pSchemaSource;
	}

	void	CGridDataSvr::SetSchmaString(LPCTSTR pSchemaStr)
	{
		if(m_pSchemaStr)delete m_pSchemaStr;
		m_pSchemaStr = _tcsdup(pSchemaStr);
	}
	LPCTSTR	CGridDataSvr::GetSchemaString()
	{
		return m_pSchemaStr;
	}


	bool CGridDataSvr::AddTemplData(CGridDataSvr* m_pTemplData,bool bDestroyTemplData)
	{
		int ARow,ACol;
		PStrItem	pItem;
		this->m_pContentSvr->GetValidRowCol(ACol,ARow);
		int i;
		for( i=0;i<=ARow;i++)
		{
			pItem = this->m_pContentSvr->GetItemData(0,i);
			if(!pItem || !pItem->pStrPre || pItem->pStrPre[0]=='\0')continue;
			break;
		}
		if(i<=ARow)
		{
			CDwTable* pTable = new  CDwTable();
			pTable->m_bDestroyTemplData = bDestroyTemplData;
			pTable->m_pTemplGridData = m_pTemplData;
			pTable->m_pParent = NULL;

			if(pTable->LoadTemplData(i,pItem->pStrPre,this))
				this->m_pDwTable.push_back(pTable);
			else
				delete pTable;
		}
		return true;
	}
	bool CGridDataSvr::ResetTemplData()
	{
		int size = (int)m_pDwTable.size();
		for(int i = size - 1 ; i >=0; i --)
			if(m_pDwTable[i]) delete  m_pDwTable[i];
		m_pDwTable.clear();
		this->CalcRptBands();
		return true;
	}

	CDwTable * CGridDataSvr::GetDwTableFromCell(int ACol,int ARow)
	{
		int row,col;
		for(int i=0;i<(int)this->FBandDwTables.size();i++)
		{
			if(FBandDwTables[i]->getRowColumnFromCell(ACol,ARow,row,col) >0) return FBandDwTables[i];
		}
		return NULL;
	}

	void CGridDataSvr::CalcRptBands()
	{
		FRowBands.clear();
		FRowBandRects.clear();
		FBandDwTables.clear();
		FBandLevels.clear();

		int deep,maxDeep = 0;
		for(int i=0;i<(int)this->m_pDwTable.size();i++)
		{
			m_pDwTable[i]->GetBandInfo(FRowBandRects,FBandLevels,FRowBands,FBandDwTables);
			deep = m_pDwTable[i]->GetMaxDeep();
			if(deep > maxDeep)maxDeep = deep;
		}
		if(maxDeep > 0)
		{
			//FRptBandWidth = 0;
			//FGroupWidth = 20 * maxDeep;
		}else
		{
			//FRptBandWidth = 0;
			//FGroupWidth = 0;
		}
	}

	int CGridDataSvr::RetrieveXmlData(LPTSTR pStrXml)
	{
		xbXml xml;
		if(!xml.LoadXml(pStrXml)) return 0;
		for(int i=0;i<(int)this->FBandDwTables.size();i++)
		{
			if(!FBandDwTables[i]->m_pParent)
				DwRetrieve(FBandDwTables[i],NULL,1,xml.m_pXmlDoc);
		}
		return 1;
	}

	int CGridDataSvr::RetrieveXmlData(CDwTable * pDwtable,LPTSTR pStrXml)
	{
		xbXml xml;
		if(!xml.LoadXml(pStrXml)) return 0;
		return DwRetrieve(pDwtable,NULL,1,xml.m_pXmlDoc);
	}

	int CGridDataSvr::DwRetrieve(CDwTable * pDwTable,IXMLDOMElement * pElement,int nRow,IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = pDwTable->getColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;
		//2017.8.23 for(int nCol = 0;nCol <nColCount;nCol++)
		for(int nCol = 1;nCol <= nColCount;nCol++)
		{
			//2017.8.23 xslPaths.push_back(pDwTable->getColumnName(nCol + 1));
			xslPaths.push_back(pDwTable->getColumnName(nCol));
		}
		pXslPath = pDwTable->m_strBoundGroup;

		IXMLDOMNodeList * pNodes;
		IXMLDOMNode*	  pNode;
		IXMLDOMNode*	  pCurrentNode;
		IXMLDOMElement*	  pItemElement;
		IXMLDOMElement*	  pColElement;
		long	nLen;
		int		group;
		int		ttlrow;
		_variant_t	val;

		wstring wpath = pXslPath;
		CDwTable* pParent = pDwTable;
		while(pParent->m_pParent && !pElement)
		{
			pParent = pParent->m_pParent;
			wpath = pParent->m_strBoundGroup +(L"/"+ wpath);
		}
		if(pElement)
			pElement->selectNodes((BSTR)wpath.c_str(),&pNodes);
		else
			pXmlDoc->selectNodes((BSTR)wpath.c_str(),&pNodes);
		pNodes->get_length(&nLen);
		for(int row = 1;row <= nLen; row++)
		{
			group = nRow;
			if(row > pDwTable->getGroupRowCount(group))
			{
				//ttlrow = pDwTable->InsertGRow(group,0,nLen - row + 1 ,false);
				ttlrow = pDwTable->InsertGRow(group,0,1 ,false);
			}
			else
				ttlrow = pDwTable->getGroupStartRow(group) + row - 1;

			pNodes->get_item(row - 1,&pNode);
			pItemElement = xbXml::GetElement(pNode);

			pItemElement->getAttribute(L"keyvalue",&val);
			if(val.bstrVal)pDwTable->SetKeyvalue(ttlrow,(int)val);
			pItemElement->getAttribute(L"guid",&val);
			if(val.bstrVal)
			{
				if((int)pDwTable->m_guidKeys.size() >= ttlrow)
					pDwTable->m_guidKeys[ttlrow - 1] = _tcsdup(val.bstrVal);
				else
					pDwTable->m_guidKeys.push_back(_tcsdup(val.bstrVal));
			}else
			{
				if((int)pDwTable->m_guidKeys.size() >= ttlrow)
					pDwTable->m_guidKeys[ttlrow - 1] = NULL;
				else
					pDwTable->m_guidKeys.push_back(NULL);
			}

			DOMNodeType	iNodeType;
			LPTSTR	pStrText;
			LPTSTR pStrDisplay ;
			_variant_t vDisplay;
			for(int col=0;col < nColCount;col++)
			{
				pStrText = NULL;
				pCurrentNode = NULL;
				pStrDisplay = NULL;
				pNode->selectSingleNode(xslPaths[col],&pCurrentNode);
				if(!pCurrentNode)continue;
				pCurrentNode->get_nodeType(&iNodeType);
				if(iNodeType == NODE_ELEMENT)
				{
					pColElement = xbXml::GetElement(pCurrentNode);
					pColElement->getAttribute(L"isnull",&val);
					if(val.bstrVal && (int)val == 1)
					{
						pStrText = NULL;
					}else
					{
						BSTR bstr;
						pColElement->get_text(&bstr);
						pColElement->getAttribute(_T("_displaystring"),&vDisplay);
						if(vDisplay.bstrVal)
							pStrDisplay = ::_tcsdup(vDisplay.bstrVal);
						if(bstr)pStrText = ::_tcsdup(bstr);
						::SysFreeString(bstr);
					}
					pColElement->Release();
				}else
				{
					BSTR bstr;
					//pColElement->get_text(&bstr);
					pCurrentNode->get_text(&bstr);
					if(bstr)pStrText = ::_tcsdup(bstr);
					::SysFreeString(bstr);
				}
				pCurrentNode->Release();


				int ACol,ARow;
				if(pDwTable->getCellFromRowColumn(ttlrow,col + 1,ACol,ARow)>=0)
				pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemString(ACol,ARow,pStrText);

				PStrItem pItem = pDwTable->getDwItem(ttlrow,col + 1);
				if(pStrDisplay)pItem->pStrText = pStrDisplay;
			}
			pDwTable->SetItemStatus(ttlrow,0,rs_datanotmodified);

			for(int k=0;k<(int)pDwTable->m_pChilds.size();k++)
			{
				DwRetrieve(pDwTable->m_pChilds[k],pItemElement,ttlrow);
			}

			bool bNew = false;
			bool bDelete = false;
			bool bModify = false;
			pItemElement->getAttribute(L"update.new",&val);
			if(val.bstrVal && _tcscmp(val.bstrVal, L"1") == 0)
				bNew = true;
			pItemElement->getAttribute(L"update.modify",&val);
			if(val.bstrVal && _tcscmp(val.bstrVal, L"1") == 0)
				bModify = true;
			pItemElement->getAttribute(L"update.delete",&val);
			if(val.bstrVal && _tcscmp(val.bstrVal, L"1") == 0)
				bDelete = true;

			if(bDelete)
				pDwTable->DeleteGRow(group,ttlrow);
			else if(bNew && bModify)
				pDwTable->SetItemStatus(ttlrow,0,rs_newmodified);
			else if(bNew)
				pDwTable->SetItemStatus(ttlrow,0,rs_new);
			else if(bModify)
				pDwTable->SetItemStatus(ttlrow,0,rs_datamodified);

			pNode->Release();
			pItemElement->Release();
		}
		pNodes->Release();
		//pDwTable->ResetUpdateStatus();
		return 1;
	}

	int CGridDataSvr::RetrieveXmlDataDiff(LPTSTR pStrXml)
	{
		xbXml xml;
		if(!xml.LoadXml(pStrXml)) return 0;
		for(int i=0;i<(int)this->FBandDwTables.size();i++)
		{
			if(!FBandDwTables[i]->m_pParent)
				DwRetrieveDiff(FBandDwTables[i],NULL,1,xml.m_pXmlDoc);
		}
		return 1;
	}

	int CGridDataSvr::RetrieveXmlDataDiff(CDwTable * pDwtable,LPTSTR pStrXml)
	{
		xbXml xml;
		if(!xml.LoadXml(pStrXml)) return 0;
		
		int ret =  DwRetrieveDiff(pDwtable,NULL,1,xml.m_pXmlDoc);
		pDwtable->m_pGrid->FShowOrigin = true;
		return ret;
	}

	int CGridDataSvr::DwRetrieveDiff(CDwTable * pDwTable,IXMLDOMElement * pElement,int nRow,IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = pDwTable->getColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;
		//2017.8.23 for(int nCol = 0;nCol <nColCount;nCol++)
		for(int nCol = 1;nCol <=nColCount;nCol++)
		{
			//2017.8.23  xslPaths.push_back(pDwTable->getColumnName(nCol + 1));
			xslPaths.push_back(pDwTable->getColumnName(nCol));
		}
		pXslPath = pDwTable->m_strBoundGroup;

		IXMLDOMNodeList * pNodes;
		IXMLDOMNode*	  pNode;
		IXMLDOMNode*	  pCurrentNode;
		IXMLDOMElement*	  pItemElement;
		IXMLDOMElement*	  pColElement;
		long	nLen;
		int		group;
		int		ttlrow;
		_variant_t	val;

		wstring wpath = pXslPath;
		CDwTable* pParent = pDwTable;
		while(pParent->m_pParent && !pElement)
		{
			pParent = pParent->m_pParent;
			wpath = pParent->m_strBoundGroup +(L"/"+ wpath);
		}
		if(pElement)
			pElement->selectNodes((BSTR)wpath.c_str(),&pNodes);
		else
			pXmlDoc->selectNodes((BSTR)wpath.c_str(),&pNodes);
		pNodes->get_length(&nLen);
		for(int row = 1;row <= nLen; row++)
		{
			group = nRow;
			pNodes->get_item(row - 1,&pNode);
			
			bool bNew = false;
			bool bModify = false;
			bool bDelete = false;

			_variant_t vs;

			((IXMLDOMElementPtr)pNode)->getAttribute(L"update.new",&vs);
			if(vs.bstrVal && vs.bstrVal[0]=='1') bNew = true;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"update.modify",&vs);
			if(vs.bstrVal && vs.bstrVal[0]=='1') bModify = true;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"update.delete",&vs);
			if(vs.bstrVal && vs.bstrVal[0]=='1') bDelete = true;

			pItemElement = xbXml::GetElement(pNode);
			pItemElement->getAttribute(L"guid",&val);
			if(val.bstrVal)
			{
				//modified or deleted, find row
				int i = 0;
				for(i=0;i<(int)pDwTable->m_rowsIndex.size();i++)
				{
					if(pDwTable->m_guidKeys[pDwTable->m_rowsIndex[i]] && 
						pDwTable->m_guidKeys[pDwTable->m_rowsIndex[i]][0] &&
						_tcsstr(val.bstrVal,pDwTable->m_guidKeys[pDwTable->m_rowsIndex[i]]))break;
				}
				if(i>=(int)pDwTable->m_rowsIndex.size())
				{
					if(bNew || bDelete)
					{
						if((int)pDwTable->m_guidKeys.size() >= ttlrow && ttlrow > 0)
							pDwTable->m_guidKeys[ttlrow - 1] = _tcsdup(val.bstrVal);
						else
							pDwTable->m_guidKeys.push_back(_tcsdup(val.bstrVal));
						if(pDwTable->GetRowCount()==1)
						{
							if(pDwTable->GetItemStatus(1,0)!=rs_new)
								ttlrow = pDwTable->InsertGRow(group,0,1 ,false);
							else
								ttlrow = 1;
						}else
							ttlrow = pDwTable->InsertGRow(group,0,1 ,false);
					}else
					{
						for(int k=0;k<(int)pDwTable->m_pChilds.size();k++)
						{
							DwRetrieveDiff(pDwTable->m_pChilds[k],pItemElement,1);
						}
						pNode->Release();
						pItemElement->Release();
						break;
					}
				}else
					ttlrow = pDwTable->m_rowsIndex[i] + 1;
			}

			pItemElement->getAttribute(L"keyvalue",&val);
			if(val.bstrVal && (bDelete || bNew))pDwTable->SetKeyvalue(ttlrow,(int)val);
			
			if(bDelete)
				pDwTable->SetItemStatus(ttlrow,0,rs_deleted);
			else if(bNew)
				pDwTable->SetItemStatus(ttlrow,0,rs_newmodified);
			else
				pDwTable->SetItemStatus(ttlrow,0,rs_datamodified);

			DOMNodeType	iNodeType;
			LPTSTR pStrText = NULL;
			LPTSTR pStrDisplay = NULL;
			LPTSTR pOrigin = NULL;
			_variant_t vDisplay;
			for(int col=0;col < nColCount;col++)
			{
				pStrText = NULL;
				pCurrentNode = NULL;
				pStrDisplay = NULL;
				pOrigin = NULL;
				pNode->selectSingleNode(xslPaths[col],&pCurrentNode);
				if(!pCurrentNode)continue;
				pCurrentNode->get_nodeType(&iNodeType);

				if(iNodeType == NODE_ELEMENT)
				{
					pColElement = xbXml::GetElement(pCurrentNode);
					pColElement->getAttribute(L"isnull",&val);
					if(val.bstrVal && (int)val == 1)
					{
						pStrText = NULL;
					}else
					{
						BSTR bstr;
						pColElement->get_text(&bstr);
						pColElement->getAttribute(_T("_displaystring"),&vDisplay);
						if(vDisplay.bstrVal)
							pStrDisplay = ::_tcsdup(vDisplay.bstrVal);
						if(bstr)pStrText = ::_tcsdup(bstr);
						::SysFreeString(bstr);
						pColElement->getAttribute(_T("update.origin"),&vDisplay);
						if(vDisplay.bstrVal)
							pOrigin = ::_tcsdup(vDisplay.bstrVal);
						pColElement->getAttribute(_T("update.origindisplaystring"),&vDisplay);
						if(vDisplay.bstrVal)
							pOrigin = ::_tcsdup(vDisplay.bstrVal);
					}
					pColElement->Release();
				}else
				{
					BSTR bstr;
					//pColElement->get_text(&bstr);
					pCurrentNode->get_text(&bstr);
					if(bstr)pStrText = ::_tcsdup(bstr);
					::SysFreeString(bstr);
				}
				pCurrentNode->Release();

				int ACol,ARow;
				LPTSTR pStr = NULL;
				if(pDwTable->getCellFromRowColumn(ttlrow,col + 1,ACol,ARow)>=0)
				{
					/*
					if(pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(ACol,ARow))
						pStr = _tcsdup(pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemString(ACol,ARow));
					else
						pStr = NULL;

					pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemString(ACol,ARow,pStrText);
					
					PStrItem pItem = pDwTable->getDwItem(ttlrow,col + 1);
					if(pStr)pItem->pOrigin = pStr;
					pItem->bDirect = true;

					if(pStrDisplay)pItem->pStrText = pStrDisplay;*/
					PStrItem pItem = pDwTable->getDwItem(ttlrow,col + 1);
					if(!pItem)
					{
						pItem = new StrItem();
						pDwTable->setDwItem(ttlrow,col + 1,pItem);
					}
					pItem->bDirect = true;
					if(!bNew && pOrigin)pItem->pOrigin = pOrigin;
					if(pStrText)pItem->pStrContent = pStrText;
					if(pStrDisplay)pItem->pStrText = pStrDisplay;
				}
			}

			for(int k=0;k<(int)pDwTable->m_pChilds.size();k++)
			{
				DwRetrieveDiff(pDwTable->m_pChilds[k],pItemElement,ttlrow);
			}
			pNode->Release();
			pItemElement->Release();
		}
		pNodes->Release();
		return 1;
	}


	int CGridDataSvr::DwUpdate(IXMLDOMDocument2 * pXmlDoc)
	{
		for(int i=0;i<(int)this->FBandDwTables.size();i++)
		{
			DwUpdate(FBandDwTables[i],pXmlDoc);
		}
		return 1;
	}

	int CGridDataSvr::DwUpdateDiff(IXMLDOMDocument2 * pXmlDoc)
	{
		for(int i=0;i<(int)this->FBandDwTables.size();i++)
		{
			DwUpdateDiff(FBandDwTables[i],pXmlDoc);
		}
		return 1;
	}

	int CGridDataSvr::DwUpdate(CDwTable * pDwTable,IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = pDwTable->getColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;
		wstring XslPathParent;
		//2017.8.23 for(int nCol = 0;nCol <nColCount;nCol++)
		for(int nCol = 1;nCol <=nColCount;nCol++)
		{
			//2017.8.23 xslPaths.push_back(pDwTable->getColumnName(nCol + 1));
			xslPaths.push_back(pDwTable->getColumnName(nCol));
		}
		pXslPath = pDwTable->m_strBoundGroup;

		CDwTable * pDwParent = NULL;
		pDwParent = pDwTable->m_pParent;
		if(pDwParent)
		{
			//get full xslpath
			XslPathParent = pDwParent->m_strBoundGroup;
			CDwTable * pParent;
			pParent = pDwParent->m_pParent;
			while(pParent)
			{
				XslPathParent = pParent->m_strBoundGroup + (wstring)_T("/") + XslPathParent;
				pParent = pParent->m_pParent;
			}
		}

		long gcnt = 1;
		IXMLDOMNodeListPtr gNodes;
		IXMLDOMNodePtr gNode;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr Node;
		IXMLDOMNodePtr  currentNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodeListPtr pRowNodes;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodeList* pTempNodes; 
		IXMLDOMNodePtr NewNode;
		IXMLDOMElementPtr tNode;
		IXMLDOMElementPtr NewElement;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;
		wstring StrName;
		if(pDwParent)
		{
			pXmlDoc->selectNodes((LPTSTR)XslPathParent.c_str(),&gNodes);
			gNodes->get_length(&gcnt);
		}else
			gcnt = 1;
		
		int trow =0;
		for(int group = 1;group <=gcnt;group++)
		{
			if(pDwParent)
				gNodes->get_item(group - 1,&gNode);
			else
				gNode = pXmlDoc;

			//if no exists pNode then create parent Node
			wstring StrPath = pXslPath;
			int nPosLast = -1;
			nPosLast = (int)StrPath.find_last_of(_T("/"));

			for(vector<IXMLDOMElement * >::const_iterator tr=vcElements.begin();tr!=vcElements.end();tr++)(*tr)->Release();
			vcElements.clear();

			wstring StrName;
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrPath.compare(_T(""))!=0)
				{
					pNode = NULL;
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
					if(!pNode)break;
					nPosLast =(int) StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
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
				StrName = StrPath;
				if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
				if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
				StrPath = _T("");
				pNodeEx = gNode;
			}
			//end for if no exists pNode then create parent node

			gNode->selectNodes(pXslPath,&pRowNodes);
			long nRowLen;
			pRowNodes->get_length(&nRowLen);
			int row = 1;
			for(row=1;row <= pDwTable->getGroupRowCount(group);row++)
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
										((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
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
							StrPath = StrPath.substr(nPosLast + 1);
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
									((IXMLDOMElementPtr)tNode)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
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
					pNodeEx->appendChild(Node,NULL);
				}//row
				
				//set data
				//set key value
				//long ll_value;
				/*ll_value = dwcell.GetKeyValue(ll_trow,primarybuf)
				if ll_value < 1 then
					ll_value = f_getindent('*')
					dwcell.SetKeyValue(ll_trow,ll_value,primarybuf)
				end if
				node.SetAttribute("keyvalue",ll_value)*/

				if((trow > (int)pDwTable->m_rowsIndex.size()) || !pDwTable->m_guidKeys[pDwTable->m_rowsIndex[trow -1]])
				//if(!pDwTable->m_guidKeys[trow - 1])
				{
					_variant_t val;
					((IXMLDOMElementPtr)Node)->getAttribute(L"guid",&val);
					if(!val.bstrVal)
					{
						GUID guid;
						CoCreateGuid(&guid);
						LPTSTR pStrGuid = new TCHAR[48];
						StringFromGUID2(guid,pStrGuid,48);
						((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pStrGuid);
						pDwTable->m_guidKeys[pDwTable->m_rowsIndex[trow -1]] = pStrGuid;
					}
				}else
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pDwTable->m_guidKeys[pDwTable->m_rowsIndex[trow -1]]);
				}
				if(pDwTable->GetItemStatus(trow,0) == rs_new)
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.new",(_variant_t)_T("1"));
				if(pDwTable->GetItemStatus(trow,0) == rs_newmodified)
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.new",(_variant_t)_T("1"));
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.modify",(_variant_t)_T("1"));
				}
				if(pDwTable->GetItemStatus(trow,0) == rs_datamodified)
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.modify",(_variant_t)_T("1"));

				LPTSTR pStrValue;
				DOMNodeType	iNodeType;
				for(int col=1;col <= (int)xslPaths.size();col++)
				{
					pStrValue = pDwTable->getDwItemText(trow,col);
					PStrItem pItem = pDwTable->getDwItem(trow,col);
					currentNode = NULL;
					Node->selectSingleNode(xslPaths[col - 1],&currentNode);
					if(currentNode)
					{
						LPTSTR pNull = pDwTable->GetColumnProp(col,L"cellneeddata");
						if(pNull && _tcscmp(pNull, L"ÊÇ")==0)
							((IXMLDOMElementPtr)currentNode)->setAttribute(L"notnull",(_variant_t)_T("1"));

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
								LPTSTR pStrType = pDwTable->GetColumnProp(col, L"celltype");
								if(pStrType && _tcscmp(pStrType, L"xs:xml") == 0)
								{
									xbXml mStrXml;
									if(mStrXml.LoadXml(pStrValue))
									{
										IXMLDOMElement * pStrElement;
										mStrXml.GetXmlDoc()->get_documentElement(&pStrElement);
										((IXMLDOMElementPtr)currentNode)->appendChild(pStrElement, NULL);
									}
									else
										currentNode->put_text(pStrValue);
								}
								else
									currentNode->put_text(pStrValue);
								if(pItem && pItem->pStrText)
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"_displaystring",(_variant_t)pItem->pStrText);
							
							}
							if(pItem && pItem->bDirect)
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.modify",(_variant_t)_T("1"));
								if(pItem && pItem->pOrigin)
								{
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origin",(_variant_t)pItem->pOrigin);
									if(pItem->pOriginStr)
										((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origindisplaystring",(_variant_t)pItem->pOriginStr);
								}else
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origin",(_variant_t)_T(""));
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
				IXMLDOMElement * pElementEx;
				pNodeItem->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
				_variant_t vv;
				pElementEx->getAttribute(L"update.delete", &vv);
				if(vv.bstrVal)
				{
					if(_tcscmp(vv.bstrVal, L"1") == 0)
					{
						pElementEx->Release();
						pNodeItem->Release();
						continue;
					}
				}
				pNodeEx->removeChild(pNodeItem,NULL);
				pElementEx->Release();
				pNodeItem->Release();
			}
		}
		for(int k=0; k<(int)pDwTable->m_delXml.size();k++)
		{
			IXMLDOMElement * pDelElement = pDwTable->m_delXml[k];
			if(!pDelElement) continue;
			IXMLDOMNode * pDelNode;
			pDelElement->QueryInterface(IID_IXMLDOMNode,(void **)&pDelNode);
			if(!pDelNode) continue;
			pNodeEx->appendChild(pDelNode, NULL);
			//pDelElement->Release();
			pDelNode->Release();
		}
		//end update data to xml node
		return 1;
	}

	int CGridDataSvr::DwUpdateDiff(CDwTable * pDwTable,IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = pDwTable->getColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;
		wstring XslPathParent;
		//2017.8.23 for(int nCol = 0;nCol <nColCount;nCol++)
		for(int nCol = 1;nCol <=nColCount;nCol++)
		{
			//2017.8.23 xslPaths.push_back(pDwTable->getColumnName(nCol + 1));
			xslPaths.push_back(pDwTable->getColumnName(nCol));
		}
		pXslPath = pDwTable->m_strBoundGroup;

		CDwTable * pDwParent = NULL;
		pDwParent = pDwTable->m_pParent;
		if(pDwParent)
		{
			//get full xslpath
			XslPathParent = pDwParent->m_strBoundGroup;
			CDwTable * pParent;
			pParent = pDwParent->m_pParent;
			while(pParent)
			{
				XslPathParent = pParent->m_strBoundGroup + (wstring)_T("/") + XslPathParent;
				pParent = pParent->m_pParent;
			}
		}

		long gcnt = 1;
		IXMLDOMNodeListPtr gNodes;
		IXMLDOMNodePtr gNode;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr Node;
		IXMLDOMNodePtr  currentNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodeListPtr pRowNodes;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodePtr NewNode;
		IXMLDOMElementPtr NewElement;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;
		wstring StrName;
		if(pDwParent)
		{
			pXmlDoc->selectNodes((LPTSTR)XslPathParent.c_str(),&gNodes);
			gNodes->get_length(&gcnt);
		}else
			gcnt = 1;
		
		int trow =0;
		for(int group = 1;group <=gcnt;group++)
		{
			if(pDwParent)
				gNodes->get_item(group - 1,&gNode);
			else
				gNode = pXmlDoc;

			//if no exists pNode then create parent Node
			wstring StrPath = pXslPath;
			int nPosLast = -1;
			nPosLast = (int)StrPath.find_last_of(_T("/"));

			for(vector<IXMLDOMElement * >::const_iterator tr=vcElements.begin();tr!=vcElements.end();tr++)(*tr)->Release();
			vcElements.clear();

			wstring StrName;
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrPath.compare(_T(""))!=0)
				{
					pNode = NULL;
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
					if(!pNode)break;
					nPosLast =(int) StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
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
				StrName = StrPath;
				if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
				if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
				StrPath = _T("");
				pNodeEx = gNode;
			}
			//end for if no exists pNode then create parent node

			gNode->selectNodes(pXslPath,&pRowNodes);
			long nRowLen;
			pRowNodes->get_length(&nRowLen);
			int row = 1;

			IXMLDOMNode * pNodeItem;
			for(int k = nRowLen;k>=1;k--)
			{
				pRowNodes->get_item( k - 1,&pNodeItem);
				pNodeEx->removeChild(pNodeItem,NULL);
				pNodeItem->Release();
			}

			for(row=1;row <= pDwTable->getGroupRowCount(group);row++)
			{
				trow++;
				if(!(pDwTable->m_RowsStatus[pDwTable->m_rowsIndex[trow -1]]&0xf000) &&
					pDwTable->GetItemStatus(trow,0) != rs_new &&
					pDwTable->GetItemStatus(trow,0) != rs_newmodified &&
					pDwTable->GetItemStatus(trow,0) != rs_datamodified)continue;

				//create data frame
				IXMLDOMElementPtr tNode;
				if(!tNode)
				{
					//create template data node
					wstring StrPath = pXslPath;
					int posLast = (int)StrPath.find_last_of(_T("/"));
					if(posLast>=0)
						StrPath = StrPath.substr(nPosLast + 1);
					else
						StrName = StrPath;
					
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&tNode);

					//end create template data node
					for(int i=1 ;i<=(int)xslPaths.size();i++)
					{
						PStrItem pItem = pDwTable->getDwItem(trow,i);
						if(!pItem || !pItem->bDirect) continue;

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
								((IXMLDOMElementPtr)tNode)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
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
				pNodeEx->appendChild(Node,NULL);
				
				//set data
				//set key value
				//long ll_value;
				/*ll_value = dwcell.GetKeyValue(ll_trow,primarybuf)
				if ll_value < 1 then
					ll_value = f_getindent('*')
					dwcell.SetKeyValue(ll_trow,ll_value,primarybuf)
				end if
				node.SetAttribute("keyvalue",ll_value)*/

				if((trow > (int)pDwTable->m_rowsIndex.size()) || !pDwTable->m_guidKeys[pDwTable->m_rowsIndex[trow -1]])
				//if(!pDwTable->m_guidKeys[trow - 1])
				{
					_variant_t val;
					((IXMLDOMElementPtr)Node)->getAttribute(L"guid",&val);
					if(!val.bstrVal)
					{
						GUID guid;
						CoCreateGuid(&guid);
						LPTSTR pStrGuid = new TCHAR[48];
						StringFromGUID2(guid,pStrGuid,48);
						((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pStrGuid);
						pDwTable->m_guidKeys[trow - 1] = pStrGuid;
					}
				}else
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pDwTable->m_guidKeys[pDwTable->m_rowsIndex[trow -1]]);
				}
				if(pDwTable->GetItemStatus(trow,0) == rs_new)
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.new",(_variant_t)_T("1"));
				if(pDwTable->GetItemStatus(trow,0) == rs_newmodified)
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.new",(_variant_t)_T("1"));
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.modify",(_variant_t)_T("1"));
				}
				if(pDwTable->GetItemStatus(trow,0) == rs_datamodified)
					((IXMLDOMElementPtr)Node)->setAttribute(L"update.modify",(_variant_t)_T("1"));

				LPTSTR pStrValue;
				DOMNodeType	iNodeType;
				for(int col=1;col <= (int)xslPaths.size();col++)
				{
					pStrValue = pDwTable->getDwItemText(trow,col);
					PStrItem pItem = pDwTable->getDwItem(trow,col);
					if(!pItem || !pItem->bDirect) continue;
					currentNode = NULL;
					Node->selectSingleNode(xslPaths[col - 1],&currentNode);
					if(currentNode)
					{
						currentNode->get_nodeType(&iNodeType);

						LPTSTR pNull = pDwTable->GetColumnProp(col,L"cellneeddata");
						if(pNull && _tcscmp(pNull, L"ÊÇ")==0)
							((IXMLDOMElementPtr)currentNode)->setAttribute(L"notnull",(_variant_t)_T("1"));
						
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
								if(pItem && pItem->pStrText)
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"_displaystring",(_variant_t)pItem->pStrText);
							}
							if(pItem && pItem->bDirect)
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.modify",(_variant_t)_T("1"));
								if(pItem && pItem->pOrigin)
								{
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origin",(_variant_t)pItem->pOrigin);
									if(pItem->pOriginStr)
										((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origindisplaystring",(_variant_t)pItem->pOriginStr);
								}else
									((IXMLDOMElementPtr)currentNode)->setAttribute(L"update.origin",(_variant_t)_T(""));
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

			for(int k=0; k<(int)pDwTable->m_delXml.size();k++)
			{
				IXMLDOMElement * pDelElement = pDwTable->m_delXml[k];
				if(!pDelElement) continue;
				BSTR bDelXml;
				pDelElement->get_xml(&bDelXml);
				IXMLDOMNode * pDelNode;
				pDelElement->QueryInterface(IID_IXMLDOMNode,(void **)&pDelNode);
				if(!pDelNode) continue;
				pNodeEx->appendChild(pDelNode, NULL);
				//pDelElement->Release();
				pDelNode->Release();
			}
		}
		//end update data to xml node
		return 1;
	}

	int CGridDataSvr::GetChildIndex(int ARow)
	{
		int i = 0;
		for( i=0;i<(int)m_pChilds->size();i++)
		{
			if(!(*m_pChilds)[i])continue;
			if(ARow >= (*m_pChilds)[i]->startRow && ARow <= (*m_pChilds)[i]->endRow) break;
		}
		return i>=(int)m_pChilds->size()? -1 : i;
	}

	CAxisSvr *  CGridDataSvr::GetAxisSvr(int ARow)
	{
		int i = 0;
		for(i=0;i<(int)m_pChilds->size();i++)
		{
			if(!(*m_pChilds)[i])continue;
			if(ARow >= (*m_pChilds)[i]->startRow && ARow <= (*m_pChilds)[i]->endRow) break;
		}
		CAxisSvr * pAxisSvr;

		int nSize = (int)m_pChilds->size();
		if(i < (int)nSize)
		{
			pAxisSvr = (*m_pChilds)[i]->pAxis;
		}else
			pAxisSvr = m_pAxisSvr;

		return pAxisSvr;
	}

	int CGridDataSvr::AddChild(int nStartRow,int nEndRow, CAxisSvr* pAxis)
	{
		TChildSvr * pChildSvr = new TChildSvr();
		pChildSvr->startRow = nStartRow;
		pChildSvr->endRow = nEndRow;
		if(pAxis)
			pChildSvr->pAxis = pAxis;
		else
		{
			pChildSvr->pAxis = new CAxisSvr();
			pChildSvr->pAxis->m_pColWidths->CopyFrom(*m_pAxisSvr->m_pColWidths,0, MAXCOL);
			if(pChildSvr->pAxis->m_pRowHeights)delete pChildSvr->pAxis->m_pRowHeights;
			pChildSvr->pAxis->m_pRowHeights = m_pAxisSvr->m_pRowHeights;
		}
		m_pChilds->push_back(pChildSvr);

		return 1;
	}
	int CGridDataSvr::RemoveChild(int ARow)
	{
		return -1; //ready to finishh
	}

	int CGridDataSvr::GetChildIndexFollow(int ARow)
	{
		int nRow = -1;
		int nIndex = -1;

		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(!(*m_pChilds)[i])continue;
			if(ARow >(*m_pChilds)[i]->startRow)continue;
			if(nRow==-1 || nRow > (*m_pChilds)[i]->startRow)
			{
				nRow = (*m_pChilds)[i]->startRow;
				nIndex = i;
			}
		}
		return nIndex;
	}

}}}}
