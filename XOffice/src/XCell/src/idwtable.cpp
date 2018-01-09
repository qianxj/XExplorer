#include "StdAfx.h"
#include "..\include\IDwTable.hpp"
#include "DwTable.hpp"
#include "XCellSheetSvr.hpp"
#include "XCellSheet.hpp"
#include "XCellSerializeSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	IDwTable::IDwTable(void):m_pDwTable(NULL),m_pSheet(NULL),m_pDataObject(NULL)
	{
	}

	IDwTable::~IDwTable(void)
	{
		if(m_pDataObject)((IXMLDOMElement*)m_pDataObject)->Release();
	}

	int IDwTable::GetRow()
	{
		return m_pDwTable->GetRow();
	}

	int IDwTable::GetColumn()
	{
		int row, col;
		m_pDwTable->getRowColumnFromCell(m_pDwTable->m_pGrid->FCurrent.x, m_pDwTable->m_pGrid->FCurrent.y, row, col);
		return col;
		//return -1;
	}
	int IDwTable::SetRow(int row)
	{
		return m_pDwTable->SetRow(row);
	}
	int IDwTable::SetColumn(int col)
	{
		if(GetColumnCount() < 1) return -1;
		if(col == 0) col = GetColumnCount() ;
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(1,col,ACol,ARow);
		m_pDwTable->m_pGrid->FCurrent.x = ACol;
		m_pDwTable->m_pGrid->FocusCell(ACol, m_pDwTable->m_pGrid->FCurrent.y,true);
		return col;
	}

	int IDwTable::GetRowCount()
	{
		return m_pDwTable->GetRowCount();
	}

	int IDwTable::GetColumnCount()
	{
		return m_pDwTable->getColumnCount();
	}

	int IDwTable::GetGroupStartRow(int nGroup)
	{
		return m_pDwTable->getGroupStartRow(nGroup);
	}

	int IDwTable::GetGroupRowCount(int nGroup)
	{
		return m_pDwTable->getGroupRowCount(nGroup);
	}
	int IDwTable::InsertGroupRow(int nGroup,int nRow)
	{
		return m_pDwTable->InsertGRow(nGroup,nRow);
	}
	int IDwTable::GetGroupFromRow(int nRow)
	{
		return m_pDwTable->getGroupFromRow(nRow);
	}

	int IDwTable::GetMaxDeep()
	{
		return m_pDwTable->GetMaxDeep();
	}

	int IDwTable::GetLevel()
	{
		return m_pDwTable->GetLevel();
	}

	LPCTSTR IDwTable::GetDwXPath()
	{
		return m_pDwTable->m_strBoundGroup;
	}

	LPCTSTR IDwTable::GetNote(int nRow,int nColumn)
	{
		return NULL;
	}
	bool IDwTable::SetNote(int nRow,int nColumn,LPCTSTR pStrNote)
	{
		return false;
	}

	int IDwTable::InsertRow()
	{
		return m_pDwTable->InsertRow();
	}

	int IDwTable::InsertRow(int nRow)
	{
		if(nRow < 0)
			return m_pDwTable->AppendRow();
		else
			return m_pDwTable->InsertRow(nRow);
	}

	int IDwTable::DeleteRow()
	{
		return m_pDwTable->DeleteRow();
	}

	int IDwTable::DeleteRow(int nRow)
	{
		return m_pDwTable->DeleteRow(nRow);
	}

	int IDwTable::InsertRow(int nGroup,int nRow)
	{
		return m_pDwTable->InsertGRow(nGroup,nRow);
	}
	int IDwTable::DeleteRow(int nGroup,int nRow)
	{
		return m_pDwTable->DeleteGRow(nGroup,nRow);
	}

	int IDwTable::GetColumnIndex(LPCTSTR pColumn)
	{
		int nIndex = 1;
		for(;nIndex <= (int)m_pDwTable->m_dbcols.size();nIndex ++)
		{
			if( m_pDwTable->m_dbcols[nIndex - 1] && _tcsicmp( m_pDwTable->m_dbcols[nIndex - 1],pColumn)==0) break;
		}
		if(nIndex <= (int)m_pDwTable->m_dbcols.size()) return nIndex;
		return -1;
	}

	LPCTSTR IDwTable::GetColumnName(int nColumn)
	{
		return m_pDwTable->getColumnName(nColumn);
	}

	LPCTSTR IDwTable::GetItemString(int nRow,int nColumn,DWBuffer dwBuffer)
	{
		return m_pDwTable->getDwItemText(nRow,nColumn);
		
	}
	LPCTSTR IDwTable::GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer)
	{
		int nIndex = GetColumnIndex(ColumnName);
		if(nIndex > 0 )
			return m_pDwTable->getDwItemText(nRow,nIndex);
		else
			return NULL;
	}
	
	LPCTSTR IDwTable::GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer)
	{
		return m_pDwTable->getDwItemDisplayText(nRow,nColumn);
		
	}
	LPCTSTR IDwTable::GetItemDisplayString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer)
	{
		int nIndex = GetColumnIndex(ColumnName);
		if(nIndex > 0 )
			return m_pDwTable->getDwItemDisplayText(nRow,nIndex);
		else
			return NULL;
	}

	bool IDwTable::ItemChangeTo(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		event_ItemChanging(nRow,nColumn,lptStr);
		bool bret = SetItemString(nRow,nColumn,lptStr,dwBuffer);
		if(!bret) return bret;
		event_ItemChanged(nRow,nColumn,lptStr);
		return bret;
	}
	bool IDwTable::ItemChangeTo(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		int nColumn = GetColumnIndex(ColumnName);
		event_ItemChanging(nRow,nColumn,lptStr);
		bool bret = SetItemString(nRow,nColumn,lptStr,dwBuffer);
		if(!bret) return bret;
		event_ItemChanged(nRow,nColumn,lptStr);
		return bret;
	}

	bool IDwTable::SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		/*if(m_pDwTable->m_pGrid->FEditorMode && m_pDwTable->m_pGrid->m_pEditSvr)
		{
			int nCellRow,nCellCol;
			m_pDwTable->getCellFromRowColumn(nRow,nColumn,nCellCol,nCellRow);
			if(m_pDwTable->m_pGrid->FInplaceCol == nCellCol &&
				m_pDwTable->m_pGrid->FInplaceRow == nCellRow)
			{
				SetWindowText(m_pDwTable->m_pGrid->m_pEditSvr->GetHWND(),lptStr);
			}
		}*/

		m_pDwTable->setDwItemText(nRow,nColumn,(LPTSTR)lptStr);

		int ACol,ARow;
		if(m_pDwTable)
		{
			m_pDwTable->getCellFromRowColumn(nRow,nColumn,ACol,ARow);
			m_pDwTable->m_pGrid->InvalidateCell(ACol,ARow);
		}
		return true;
	}
	bool IDwTable::SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		int nIndex = GetColumnIndex(ColumnName);
		if(nIndex > 0 )
		{
			int nRet = m_pDwTable->setDwItemText(nRow,nIndex,(LPTSTR)lptStr);
			int ACol,ARow;
			if(m_pDwTable)
			{
				m_pDwTable->getCellFromRowColumn(nRow,nIndex,ACol,ARow);
				m_pDwTable->m_pGrid->InvalidateCell(ACol,ARow);
			}
			if(nRet >=0) 
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool IDwTable::SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		m_pDwTable->setDwItemDisplayText(nRow,nColumn,(LPTSTR)lptStr);

		int ACol,ARow;
		if(m_pDwTable)
		{
			m_pDwTable->getCellFromRowColumn(nRow,nColumn,ACol,ARow);
			m_pDwTable->m_pGrid->InvalidateCell(ACol,ARow);
		}
		return true;
	}

	bool IDwTable::SetItemDisplayString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		int nIndex = GetColumnIndex(ColumnName);
		if(nIndex > 0 )
		{
			int nRet = m_pDwTable->setDwItemDisplayText(nRow,nIndex,(LPTSTR)lptStr);
			int ACol,ARow;
			if(m_pDwTable)
			{
				m_pDwTable->getCellFromRowColumn(nRow,nIndex,ACol,ARow);
				m_pDwTable->m_pGrid->InvalidateCell(ACol,ARow);
			}
			if(nRet >=0) 
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool IDwTable::SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer)
	{
		return m_pDwTable->SetItemStatus(nRow,nColumn,(XCell::RowStatus)status,(XCell::DWBuffer)dwBuffer);
	}

	IDwTable::RowStatus IDwTable::GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer)
	{
		return (IDwTable::RowStatus)m_pDwTable->GetItemStatus(nRow,nColumn,(XCell::DWBuffer)dwBuffer);
	}

	IDwView * IDwTable::GetParent()
	{
		if(!m_pDwTable->m_pParent) return NULL;
		if(m_pDwTable->m_pParent->GetInterface()) return (IDwTable *)m_pDwTable->m_pParent->GetInterface();

		CDwTable * pTable = m_pDwTable->m_pParent;
		pTable->m_pInterFace = new IDwTable();
		((IDwTable *)pTable->m_pInterFace)->m_pDwTable =  pTable;
		return (IDwTable *)pTable->GetInterface();
	}

	int IDwTable::GetChildCount()
	{
		return (int)m_pDwTable->m_pChilds.size();
	}
	IDwView * IDwTable::GetChild(int nIndex)
	{
		if(nIndex >= (int)m_pDwTable->m_pChilds.size()) return NULL;
		CDwTable * pTable = m_pDwTable->m_pChilds[nIndex];
		if(pTable ->GetInterface()) return (IDwTable *)pTable->GetInterface();

		pTable->m_pInterFace = new IDwTable;
		((IDwTable *)pTable->m_pInterFace)->m_pDwTable =  pTable;
		return (IDwTable *)pTable->GetInterface();
	}

	int	IDwTable::SetKeyValue(int nRow,int nKeyValue)
	{
		return m_pDwTable->SetKeyvalue(nRow,nKeyValue);
	}
	int IDwTable::GetKeyValue(int nRow)
	{
		return m_pDwTable->GetKeyvalue(nRow);
	}
	bool IDwTable::GetGuid(int nRow,TCHAR Guid[],int nMax)
	{
		return false;
	}
	LPCTSTR IDwTable::GetGuid(int nRow)
	{
		return m_pDwTable->GetGuid(nRow);
	}

	bool IDwTable::SetGuid(int nRow,LPCTSTR pStrGuid)
	{
		return m_pDwTable->SetGuid(nRow,(LPTSTR)pStrGuid);
	}


	int	IDwTable::DeletedCount()
	{
		return m_pDwTable->GetDeletedCount();
	}
	int	IDwTable::ModifiedCount()
	{
		return m_pDwTable->GetModifiedCount();
	}

	int	IDwTable::Reset()
	{
		return m_pDwTable->Reset();
	}
	int	IDwTable::ResetUpdateStatus()
	{
		int ret = m_pDwTable->ResetUpdateStatus();
		this->Redraw();
		return ret;
	}

	
	int IDwTable::DwUpdateTo(IXMLDOMDocument2 * pXmlDoc)
	{
		return m_pDwTable->m_pGrid->GetGridDataSvr()->DwUpdate(m_pDwTable,pXmlDoc);
	}
	int IDwTable::DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc)
	{
		return m_pDwTable->m_pGrid->GetGridDataSvr()->DwUpdate(pXmlDoc);
	}
	int IDwTable::DwUpdateToDiff(IXMLDOMDocument2 * pXmlDoc)
	{
		return m_pDwTable->m_pGrid->GetGridDataSvr()->DwUpdateDiff(m_pDwTable,pXmlDoc);
	}
	int IDwTable::DwUpdateAllToDiff(IXMLDOMDocument2 * pXmlDoc)
	{
		return m_pDwTable->m_pGrid->GetGridDataSvr()->DwUpdateDiff(pXmlDoc);
	}

	int IDwTable::Retrieve(IXMLDOMElement * pElement)
	{
		if(!this->m_pDataObject) return -1;
	
		if(!m_pDwTable) return -1;
		m_pDwTable->m_pInterFace = NULL;

		int FScrollBars = m_pDwTable->m_pGrid->FScrollBars;
		int FOptions = m_pDwTable->m_pGrid->FOptions;
		int FCanEditModify = m_pDwTable->m_pGrid->FCanEditModify;
		int nHeight = m_pDwTable->m_pGrid->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0);
		int nWidth = m_pDwTable->m_pGrid->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);

		SetDataObject((IXMLDOMElement *)m_pDataObject);

		m_pDwTable->m_pGrid->FScrollBars = FScrollBars;
		m_pDwTable->m_pGrid->FOptions = FOptions;
		m_pDwTable->m_pGrid->FCanEditModify = FCanEditModify;
		//m_pDwTable->m_pGrid->FShowOrigin = false;
		m_pDwTable->m_pGrid->GetGridDataSvr()->m_pAxisSvr->SetRowHeight(0, nHeight);
		m_pDwTable->m_pGrid->GetGridDataSvr()->m_pAxisSvr->SetColWidth(0, nWidth);

		BSTR bstr;
		pElement->get_xml(&bstr);
		
		//if(::IsWindowVisible(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND()))
		//	::SendMessage(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND(),WM_SETREDRAW,FALSE,0);
		int nRet = m_pDwTable->m_pGrid->GetGridDataSvr()->RetrieveXmlData(m_pDwTable,bstr);
		::SysFreeString(bstr);

		if(!this->m_pDwTable->m_pParent && m_pSheet)
		{
			int ACol=1,ARow=1;
			m_pDwTable->getCellFromRowColumn(1,1,ACol,ARow);
			CDwTable * pItem = m_pDwTable;
			while(ACol==1 && ARow==1 &&  pItem->m_pChilds.size() >0)
			{
				pItem = pItem->m_pChilds[0];
				pItem->getCellFromRowColumn(1,1,ACol,ARow);
			}

			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FCurrent.x = ACol;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FCurrent.y = ARow;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.x = ACol;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.y = ARow;

			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FTopLeft.x = 1;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FTopLeft.y = 1;


			//if(::IsWindowVisible(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND()))
			//{
			//	::SendMessage(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND(),WM_SETREDRAW,TRUE,0);
				((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->InvalidateGrid();
			//}
		}
		return nRet;
	}
	
	int IDwTable::RetrieveDiff(IXMLDOMElement * pElement)
	{
		if(!m_pDwTable) return -1;

		BSTR bstr;
		pElement->get_xml(&bstr);
		
		//if(::IsWindowVisible(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND()))
		//	::SendMessage(((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetHWND(),WM_SETREDRAW,FALSE,0);
		int nRet = m_pDwTable->m_pGrid->GetGridDataSvr()->RetrieveXmlDataDiff(m_pDwTable,bstr);
		::SysFreeString(bstr);

		return nRet;
	}

	int IDwTable::Retrieve(xbXml &xml)
	{
		IXMLDOMElement * pElement;
		int nRet = -1;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			nRet =  Retrieve(pElement);
			pElement->Release();
		}
		return nRet;
	}

	int IDwTable::RetrieveDiff(xbXml &xml)
	{
		IXMLDOMElement * pElement;
		int nRet = -1;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			nRet =  RetrieveDiff(pElement);
			pElement->Release();
		}
		return nRet;
	}

	int IDwTable::Retrieve(ITrans * pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!pTrans) return -1;
		return Retrieve(pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}
	int IDwTable::Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!m_pTrans) return -1;
		return Retrieve(m_pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}

	int IDwTable::Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		TCHAR buf[512];
		
		if(_tcsstr(pDataUrl,_T("uriserver/")) == pDataUrl)
			pDataUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pDataUrl,_T("xquery:"))==pDataUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pDataUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pDataUrl);

		xbXml xmlRpt;
		int nRet = -1;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:m_pDwTable->m_pGrid->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			BSTR bstr;
			xmlRpt.m_pXmlDoc->get_xml(&bstr);
			nRet = m_pDwTable->m_pGrid->GetGridDataSvr()->RetrieveXmlData(bstr);
			::SysFreeString(bstr);

			int ACol=1,ARow=1;
			m_pDwTable->getCellFromRowColumn(1,1,ACol,ARow);
			CDwTable * pItem = m_pDwTable;
			while(ACol==1 && ARow==1 &&  pItem->m_pChilds.size() >0)
			{
				pItem = pItem->m_pChilds[0];
				pItem->getCellFromRowColumn(1,1,ACol,ARow);
			}
			
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FCurrent.x = ACol;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FCurrent.y = ARow;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.x = ACol;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.y = ARow;

			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FTopLeft.x = 1;
			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->FTopLeft.y = 1;

			((XCellSheetSvr*) m_pSheet->m_pSheetSvr)->m_pGridFrameSvrs->InvalidateGrid();
		}

		return nRet;
	}



	BSTR IDwTable::GetXml(LPCTSTR pRoot, bool bIncludeChild )
	{
		return NULL;
	}
	int IDwTable::Load(IXMLDOMElement * pElement)
	{
		return this->Retrieve(pElement);
	}
	int  IDwTable::LoadXml(BSTR bstr)
	{
		IXMLDOMElement * pElement;
		xbXml xml;
		xml.Load(bstr);

		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			int nRet = Load(pElement);
			pElement->Release();
			return nRet;
		}else
			return -1;
	}

	int IDwTable::AddObserver(xbObserver * pObserver)
	{
		return m_pDwTable->AddObserver(pObserver);
	}

	int IDwTable::RemoveObserver(xbObserver * pObserver)
	{
		return m_pDwTable->RemoveObserver(pObserver);
	}
	int IDwTable::ClearObserver()
	{
		return m_pDwTable->ClearObserver();
	}

	LPCTSTR IDwTable::GetDwName()
	{
		 return m_pDwTable->m_strBoundGroup;
	}
	
	IDwView * IDwTable::FindDwTable(LPCTSTR pStrName,IDwView *pParent)
	{
		if(!pParent)
		{
			pParent = this;
			while(pParent->GetParent())
			{
				pParent = pParent->GetParent();
			}
		}

		IDwTable *pItem;
		if(pParent)
		{
			if(_tcsicmp(pStrName,((IDwTable *)pParent)->m_pDwTable->m_strBoundGroup)==0) return pParent;

			for(int i=0;i<pParent->GetChildCount();i++)
			{
				if(!pParent->GetChild(i)) continue;
				pItem = (IDwTable *)FindDwTable(pStrName,pParent->GetChild(i));
				if(pItem) return pItem;
			}
		}

		return NULL;
	}

	IDwView * IDwTable::GetCurTable()
	{
		int nRow,nCol;
		return GetCurTable(nRow,nCol);
	}

	IDwView * IDwTable::GetCurTable(int &nRow,int &nCol)
	{
		nRow = -1;
		nCol = -1;
		CDwTable * pTable = m_pDwTable->m_pGrid->GetCurDwTable();
		if(!pTable) return NULL;

		if(!pTable ->GetInterface())
		{
			pTable->m_pInterFace = new IDwTable;
			((IDwTable *)pTable->m_pInterFace)->m_pDwTable =  pTable;
		}
		pTable->getRowColumnFromCell(m_pDwTable->m_pGrid->FCurrent.x,
			m_pDwTable->m_pGrid->FCurrent.y,nRow,nCol);

		return (IDwTable *)pTable->GetInterface();
	}

	int IDwTable::SetDataObject(IXMLDOMDocument * pDoc)
	{
		IXMLDOMElement *  pElement;
		
		pDoc->get_documentElement(&pElement);
		if(pElement)
		{
			int nRet= SetDataObject(pElement);
			pElement->Release();
			return nRet;
		}
		return-1;
	}

	int IDwTable::SetDataObject(IXMLDOMElement * pElement)
	{
		if(!m_pSheet) return -1;
		if(m_pDataObject!=pElement)
		{
			pElement->AddRef();
			if(m_pDataObject)((IXMLDOMElement *)m_pDataObject)->Release();
			m_pDataObject = pElement;
		}
		
		if(m_pSheet->m_pSheetSvr->m_pDataSvr)
		{
			delete m_pSheet->m_pSheetSvr->m_pDataSvr;
			m_pSheet->m_pSheetSvr->m_pDataSvr = new  CGridDataSvr();
		}

		int nRet = m_pSheet->m_pSheetSvr->LoadTemplate(pElement,goRunState);
		
		CGridDataSvr * pDataSvr = (CGridDataSvr *)((XCellSheetSvr *)m_pSheet->m_pSheetSvr)->m_pDataSvr;
		if(/*!m_pDwTable &&*/ pDataSvr->FBandDwTables.size()>0)
		{
			CDwTable * pTable = pDataSvr->FBandDwTables[0];
			pTable->m_pInterFace = this;
			this->m_pDwTable = pTable;
		}else
			this->m_pDwTable = NULL;

		int ACol=1,ARow=1;
		if(pDataSvr->FBandDwTables.size()>0)
		{
			 CDwTable * pTable = pDataSvr->FBandDwTables[0];
			 pTable->getCellFromRowColumn(1,1,ACol,ARow);
			 if(!pTable->m_pParent)
			 {
				 if(ACol > 0 && ARow > 0)
				 {
					 CDwTable * pItem = pTable;
					while(ACol==1 && ARow==1 &&  pItem->m_pChilds.size() >0)
					{
						pItem = pItem->m_pChilds[0];
						pItem->getCellFromRowColumn(1,1,ACol,ARow);
					}

					 pTable->m_pGrid->FCurrent.x = ACol;
					 pTable->m_pGrid->FCurrent.y = ARow;
					 pTable->m_pGrid->FAnchor = m_pDwTable->m_pGrid->FCurrent;
				 }
				 pTable->m_pGrid->FTopLeft.x = 1;
				 pTable->m_pGrid->FTopLeft.y = 1;
			 }	
		}
		
		return nRet;
	}

	int IDwTable::SetDataObject(ITrans * pTrans,LPCTSTR pUrl)
	{
		if(!pTrans) return -1;
		return SetDataObject(pTrans->GetServerUrl(),pUrl);
	}
	int IDwTable::SetDataObject(LPCTSTR pUrl)
	{
		if(!m_pTrans) return -1;
		return SetDataObject(m_pTrans->GetServerUrl(),pUrl);
	}

	int IDwTable::SetDataObject(LPCTSTR pServer,LPCTSTR pUrl)
	{
		TCHAR buf[512];
		int nRet = -1;
		
		if(_tcsstr(pUrl,_T("uriserver/")) == pUrl)
			pUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pUrl,_T("dev:sheet"))==pUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pUrl);

		xbXml xmlRpt;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:m_pDwTable->m_pGrid->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			IXMLDOMElement * pElement;
			xmlRpt.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				nRet = this->SetDataObject(pElement);
				pElement->Release();
			}
		}
		return nRet;
	}

	int IDwTable::ImportFile()
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
		ofn.hwndOwner = m_pDwTable->m_pGrid->m_pSheetSvr->m_pSheetCtrl->GetWin()->m_hWnd;
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
	int IDwTable::ImportFile(LPCTSTR pStrFile,LPCTSTR pType,bool Header)
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

	int IDwTable::ImportString(LPCTSTR pStr,LPCTSTR pType,bool Header )
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
	
	int IDwTable::Modify(LPCTSTR pStr)
	{
		return -99;
	}

	int IDwTable::event_ItemFocusChanged(int nRow,int nCol)
	{
		return m_pDwTable->event_ItemFocusChanged(nRow,nCol);
	}
	int IDwTable::event_RowFocusChanged(int nRow,int nOldRow)
	{
		return m_pDwTable->event_RowFocusChanged(nRow,nOldRow);
	}
	int IDwTable::event_GetFocus( )
	{
		return m_pDwTable->event_GetFocus(NULL);
	}
	int IDwTable::event_LoseFocus()
	{
		return m_pDwTable->event_LoseFocus(NULL);
	}
	int IDwTable::event_ItemChanged(int nRow,int nCol,LPCTSTR pData)
	{
		return m_pDwTable->event_ItemChanged(nRow,nCol,(LPTSTR)pData);
	}
	int IDwTable::event_ItemChanging(int nRow,int nCol,LPCTSTR pData)
	{
		return m_pDwTable->event_ItemChanging(nRow,nCol,(LPTSTR)pData);
	}

	int IDwTable::GetDwType()
	{
		return DWT_CELL;
	}

	int IDwTable::SetRowSelectorWidth(int nWidth)
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->SetRowSelectorWidth(nWidth);
	}
	int IDwTable::ResetRowSelector()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->ResetRowSelector();
	}
	int IDwTable::GetRowSelectorWidth()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetRowSelectorWidth();
	}
	int IDwTable::GetRowSelectorDefaultWidth()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetRowSelectorDefaultWidth();
	}

	int IDwTable::SetColHeaderHeight(int nHeight)
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->SetColHeaderHeight(nHeight);
	}
	int IDwTable::GetColHeaderDefaultHeight()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetColHeaderDefaultHeight();
	}
	int IDwTable::ResetColHeaderHeight()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->ResetColHeaderHeight();
	}
	int IDwTable::GetColHeaderHeight()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetColHeaderHeight();
	}

	LPTSTR IDwTable::GetColumnProps(LPCTSTR pColName)
	{
		return m_pDwTable->GetColumnProps(pColName);
	}
	int IDwTable::SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp)
	{
		return m_pDwTable->SetColumnProps(pColName,pXmlProp);
	}
	LPTSTR IDwTable::GetColumnProp(LPCTSTR pColName,LPCTSTR pItem)
	{
		return m_pDwTable->GetColumnProp(pColName,pItem);
	}
	int IDwTable::SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp)
	{
		return m_pDwTable->SetColumnProp(pColName,pItem,pProp);
	}

	LPTSTR IDwTable::GetColumnProps(int nCol)
	{
		return m_pDwTable->GetColumnProps(nCol);
	}
	int IDwTable::SetColumnProps(int nCol,LPCTSTR pXmlProp)
	{
		return m_pDwTable->SetColumnProps(nCol,pXmlProp);
	}
	LPTSTR IDwTable::GetColumnProp(int nCol,LPCTSTR pItem)
	{
		return m_pDwTable->GetColumnProp(nCol,pItem);
	}
	int IDwTable::SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp)
	{
		return m_pDwTable->SetColumnProp(nCol,pItem,pProp);
	}
	int IDwTable::SetColumnProp(int nRow, int nCol,LPCTSTR pItem,LPCTSTR pProp)
	{
		return m_pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemProp(nCol,nRow,pItem,pProp);
	}
	
	int IDwTable::SetScrollState(bool both)
	{
		if(!m_pDwTable) return -1;
		if(both)
			m_pDwTable->m_pGrid->FScrollBars=ssBoth;
		else
			m_pDwTable->m_pGrid->FScrollBars=ssNone;
		m_pDwTable->m_pGrid->InvalidateGrid();

		return 1;
	}
	int IDwTable::SetHScrollState(bool bShow)
	{
		if(!m_pDwTable) return -1;
		if(bShow)
		{
			if(!(m_pDwTable->m_pGrid->FScrollBars&ssHorizontal))
			{
				m_pDwTable->m_pGrid->FScrollBars |= ssHorizontal;
				m_pDwTable->m_pGrid->InvalidateGrid();
			}
		}else
		{
			if(m_pDwTable->m_pGrid->FScrollBars & ssHorizontal)
			{
				m_pDwTable->m_pGrid->FScrollBars &= ~ssHorizontal;
				m_pDwTable->m_pGrid->InvalidateGrid();
			}
		}
		return 1;
	}
	int IDwTable::SetVScrollState(bool bShow)
	{
		if(!m_pDwTable) return -1;
		if(bShow)
		{
			if(!(m_pDwTable->m_pGrid->FScrollBars & ssVertical))
			{
				m_pDwTable->m_pGrid->FScrollBars |= ssVertical;
				m_pDwTable->m_pGrid->InvalidateGrid();
			}
		}else
		{
			if(m_pDwTable->m_pGrid->FScrollBars & ssVertical)
			{
				m_pDwTable->m_pGrid->FScrollBars &= ~ssVertical;
				m_pDwTable->m_pGrid->InvalidateGrid();
			}
		}
		return 1;
	}

	LPTSTR IDwTable::GetHtml()
	{
		return ((XCellSerializeSvr*)m_pDwTable->m_pGrid->m_pSheetSvr->m_pSerializeSvr)
			->SerializeSheetDataToHtml(m_pDwTable->m_pGrid->m_pSheetSvr->m_pDataSvr);
	}

	int IDwTable::SetReadOnly(bool bReadOnly)
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->m_pGridFrameSvrs->SetReadOnly(bReadOnly);
	}

	COLORREF IDwTable::GetDirectColor()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetDirectColor();
	}
	void	IDwTable::SetDirectColor(COLORREF cr)
	{
		m_pDwTable->m_pGrid->m_pSheetSvr->SetDirectColor(cr);
	}
	COLORREF IDwTable::GetAdvanceColor()
	{
		return m_pDwTable->m_pGrid->m_pSheetSvr->GetAdvanceColor();
	}
	void	IDwTable::SetAdvanceColor(COLORREF cr)
	{
		m_pDwTable->m_pGrid->m_pSheetSvr->SetAdvanceColor(cr);
	}

	LPTSTR	IDwTable::GetNoteString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		return _tcsdup(m_pDwTable->m_pGrid->m_pSheetSvr->GetNoteString(ACol,ARow));
	}
	LPTSTR	IDwTable::GetOriginString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		return _tcsdup(m_pDwTable->m_pGrid->m_pSheetSvr->GetOriginString(ACol,ARow));
	}
	LPTSTR	IDwTable::GetAdvanceString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		return _tcsdup(m_pDwTable->m_pGrid->m_pSheetSvr->GetAdvanceString(ACol,ARow));
	}
		
	void	IDwTable::SetNoteString(int nRow, int nCol,LPTSTR pStr)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->SetNoteString(ACol,ARow,pStr);

	}
	void	IDwTable::SetOriginString(int nRow, int nCol,LPTSTR pStr)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->SetOriginString(ACol,ARow,pStr);
	}
	void	IDwTable::SetAdvanceString(int nRow, int nCol,LPTSTR pStr)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->SetAdvanceString(ACol,ARow,pStr);
	}

	void	IDwTable::ClearNoteString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->ClearOriginString(ACol,ARow);

	}
	void	IDwTable::ClearOriginString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->ClearOriginString(ACol,ARow);

	}
	void	IDwTable::ClearAdvanceString(int nRow, int nCol)
	{
		int ACol,ARow;
		m_pDwTable->getCellFromRowColumn(nRow,nCol,ACol,ARow);
		m_pDwTable->m_pGrid->m_pSheetSvr->ClearAdvanceString(ACol,ARow);
	}
	int IDwTable::AcceptText()
	{
		m_pDwTable->AcceptText();
		return 0;
	}
	int IDwTable::getNullCell(POINT &p)
	{
		return m_pDwTable->getNullCell(p);
    }
	int IDwTable::ExpandRow()
	{
		return m_pDwTable->ExpandRow();
	}
	int IDwTable::ExpandRow(int sRow, int eRow)
	{
		return m_pDwTable->ExpandRow(sRow, eRow);
	}
	int IDwTable::ExpandCellRow(int sRow, int eRow)
	{
		return m_pDwTable->ExpandCellRow(sRow, eRow);
	}
	int IDwTable::ExpandMarginRow(int sRow, int eRow)
	{
		return m_pDwTable->ExpandMarginRow(sRow, eRow);
	}
	int IDwTable::GetCellRowFromRowColumn(int sRow, int sCol)
	{	
		int aRow, aCol;
		m_pDwTable->getCellFromRowColumn(sRow, sCol, aCol, aRow);
		return aRow;
	}
	int IDwTable::SetSelectionBorder(int Flag)
	{
		if(Flag == 0)
			m_pDwTable->m_pGrid->FOptions &= ~goVisibleSelection;
		else
			m_pDwTable->m_pGrid->FOptions |= goVisibleSelection;
		return 1;
	}
	int IDwTable::SetItemText(int row, int col, LPTSTR name)
	{
		this->m_pDwTable->m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemText(col, row, name);
		return 1;
	}

	void IDwTable::SetEditUpperMode(bool upper)
	{
		m_pDwTable->m_pGrid->SetEditUpperMode(upper);
	}
	bool IDwTable::GetEditUpperMode()
	{
		return m_pDwTable->m_pGrid->GetEditUpperMode();
	}

}}}}
