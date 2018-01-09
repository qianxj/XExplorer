#include "StdAfx.h"
#include "DwTable.hpp"
#include "Idwtable.hpp"
#include "IDwTableObServer.hpp"
#include "optionsvr.hpp"
#include "XCellSheetSvr.hpp"
#include "XCellSheet.hpp"
#include "XGridComplier.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CDwTable::CDwTable(void)
		: m_pGrid(NULL)
		, m_pTemplGridData(NULL)
		, m_RowGroupStartTempl(-1)
		, m_RowGroupEndTempl(-1)
		, m_bDestroyTemplData(false)
		, m_preRows(1),
		m_strBoundGroup(NULL)
	{
	}

	CDwTable::~CDwTable(void)
	{
		if(m_strBoundGroup) delete m_strBoundGroup;
		int size =  (int)m_pChilds.size();
		for(int i = size - 1 ; i >=0; i --)
		{
			if(m_pChilds[i])
			{
				if(m_bDestroyTemplData)
				{
					delete m_pTemplGridData;
					m_pTemplGridData = NULL;
				}
				delete  m_pChilds[i];
			}
		}
		int index;
		for(index=0;index < (int)m_dbcols.size();index++)
		{
			if(m_dbcols[index]) delete m_dbcols[index];
		}

		for(index=0;index < (int)m_StrKeys.size();index++)
		{
			if(m_StrKeys[index]) delete m_StrKeys[index];
			if(m_guidKeys[index]) delete m_guidKeys[index];
		}

		for(int i=0;i<(int)m_guidKeys.size();i++)
		{
			if(m_guidKeys[i])delete m_guidKeys[i];
		}
		for(int i=0;i<(int)m_StrKeys.size();i++)
		{
			if(m_StrKeys[i])delete m_StrKeys[i];
		}

		m_rowsIndex.clear();
		m_delrowsIndex.clear();
		m_delXml.clear();
		m_delGroup.clear();

		m_StrKeys.clear() ;
		m_Keys.clear() ;
		m_guidKeys.clear() ;
		m_XmlRowNos.clear() ;
		m_RowsStatus.clear() ;

		m_dbcols.clear();
		m_groups.clear();
		m_groupstart.clear();
	}

	int CDwTable::getColumns(vector<LPTSTR> &dataColumns,vector<POINT> &ColPoses) //得到数据列
	{
		int ACol;
		for(int index=0;index < (int)dataColumns.size();index++)
		{
			if(dataColumns[index]) delete dataColumns[index];
		}
		dataColumns.clear();
		ColPoses.clear();

		for(int i=m_RowGroupStartTempl;i<=m_RowGroupEndTempl;i++)
		{
			//adjust cell item
			ACol = m_pTemplGridData->m_pContentSvr->GetValidCol(i);
			for(int j=1;j<=ACol;j++)
			{
				//if is spaned cell then continue
				TGridRect srect;
				m_pTemplGridData->m_pSpanSvr->isSpaned(j,i,srect);
				if(srect.left !=j ||srect.top!= i)continue;

				PStrItem pItem = m_pTemplGridData->m_pContentSvr->GetItemData(j,i);

				if(pItem)
				{
					LPTSTR pStrGroupStr = pItem->pStrGroup;
					LPTSTR pStrColumn = pItem->pStrColumn;

					if(pStrGroupStr &&::_tcsicmp(m_strBoundGroup,pStrGroupStr)==0 && pStrColumn &&pStrColumn[0]!='\0')
					{
						LPTSTR ptStrCol = ::_tcsdup(pStrColumn);
						dataColumns.push_back(ptStrCol);
						POINT pt;
						pt.y = i - m_RowGroupStartTempl;
						pt.x = j;
						ColPoses.push_back(pt);
					}
				}
			}
		}
		return (int)dataColumns.size();
	}

	int CDwTable::getRowColumnFromCell(int ACol,int ARow,int &row,int &col)
	{
		int dRow = this->GetRowFromCell(ARow);
		if(dRow < 0) return -1;
		int sCell,eCell;
		this->GetRowArea(dRow,sCell,eCell);
		for(int i =0;i<(int)this->m_dbcols.size();i++)
		{
			if(m_dbcolsPos[i].y == ARow - sCell  && m_dbcolsPos[i].x == ACol)
			{
				row = dRow;
				col = i + 1;
				return row;
			}
		}
		return -1;
	}
	int CDwTable::getCellFromRowColumn(int row,int col,int &ACol,int &ARow)
	{
		int sCell,eCell;
		if(this->GetRowArea(row,sCell,eCell)<0) return -1;
		if(col < 1) return -1;
		if(col > (int)m_dbcols.size()) return -1;
		ACol = this->m_dbcolsPos[col -1].x ;
		ARow = this->m_dbcolsPos[col -1].y + sCell;
		return  ARow;
	}

	PStrItem CDwTable::getDwItem(int row,int col) //得到行列项
	{
		int ACol,ARow;
		if(getCellFromRowColumn(row,col,ACol,ARow)<0) return NULL;

		return m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
	}
	int CDwTable::setDwItem(int row,int col,PStrItem pItem) //设置行列项
	{
		int ACol,ARow;
		if(getCellFromRowColumn(row,col,ACol,ARow)<0) return -1;

		m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
		return 1;
	}

	int	CDwTable::SetItemModified(int row,int col)
	{
		if((this->m_RowsStatus[this->m_rowsIndex[row - 1]]&0x0fff) ==rs_new)
		{
			this->m_RowsStatus[this->m_rowsIndex[row - 1]] =
				(RowStatus)((this->m_RowsStatus[this->m_rowsIndex[row - 1]]&0xf000)|rs_newmodified);
		}
		if((this->m_RowsStatus[this->m_rowsIndex[row - 1]]&0x0fff)==rs_datanotmodified)
		{
			this->m_RowsStatus[this->m_rowsIndex[row - 1]]=
				(RowStatus)((this->m_RowsStatus[this->m_rowsIndex[row - 1]]&0xf000)|rs_datamodified);
		}
		CDwTable* pParent = this->m_pParent;
		int group = this->getGroupFromRow(row);
		while(pParent)
		{
			pParent->m_RowsStatus[pParent->m_rowsIndex[group - 1]] =
				(RowStatus)(pParent->m_RowsStatus[pParent->m_rowsIndex[group - 1]]|rs_modified);

			group = pParent->getGroupFromRow(group);
			pParent=pParent->m_pParent;
		}

		PStrItem pItem;
		pItem = getDwItem(row,col);
		if(pItem)
		{
			pItem->bDirect = true;
			if(pItem->pStrContent && !pItem->pOrigin)
			{
				if(!pItem->pOrigin)pItem->pOrigin = _tcsdup(pItem->pStrContent);
			}
			if(pItem->pStrText && !pItem->pOriginStr)
			{
				pItem->pOriginStr = _tcsdup(pItem->pStrText);
			}
		}

		return 1;
	}

	LPTSTR CDwTable::getDwItemText(int row,int col) //得到行列项
	{
		PStrItem pItem;
		pItem = getDwItem(row,col);
		if(pItem && pItem->pStrXml)
		{
			IXMLDOMElement * pElement1 = NULL;
			IXMLDOMNode * pNode;
			_variant_t var;

			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
				pElement1->getAttribute(L"cellsource",&var);
				LPTSTR pItemStr = NULL;
				if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
				{
					pItemStr = _tcsdup(var.bstrVal);
					if(pItemStr[0]=='=')
					{
						int ACol,ARow;
						if(getCellFromRowColumn(row,col,ACol,ARow) >= 0)
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this->m_pGrid, 1, ARow, ACol))
								pItemStr = x.GetString();
						}
					}
				}
				pNode->Release();
				pElement1->Release();
				if(pItemStr) return _tcsdup(pItemStr);
			}
		}

		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = this->m_strBoundGroup;
			CDwTable * pDwParent = this->m_pParent;
			while(pDwParent)
			{
				path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
				pDwParent = pDwParent->m_pParent;
			}
			return m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindGetData((wchar_t*)path.c_str(),row,(wchar_t*)getColumnName(col));
		}
		else
		{
			if(!pItem) return NULL;
			return pItem->pStrContent;
		}
	}

	int CDwTable::setDwItemText(int row,int col,LPTSTR strValue) //设置行列项
	{
		int ACol,ARow;
		if(getCellFromRowColumn(row,col,ACol,ARow)<0) return -1;
		SetItemModified(row,col);
		m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemString(ACol,ARow,strValue);
		PStrItem pItem;
		pItem = getDwItem(row,col);

		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = this->m_strBoundGroup;
			CDwTable * pDwParent = this->m_pParent;
			while(pDwParent)
			{
				path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
				pDwParent = pDwParent->m_pParent;
			}
			m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path.c_str(),row,getColumnName(col),strValue);
		}
		return 1;
	}

	LPTSTR CDwTable::getDwItemDisplayText(int row,int col) //得到行列项
	{
		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = this->m_strBoundGroup;
			CDwTable * pDwParent = this->m_pParent;
			while(pDwParent)
			{
				path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
				pDwParent = pDwParent->m_pParent;
			}
			wstring colname = (wstring)getColumnName(col) + (wstring)_T("/@_displaystring");
			return m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindGetData((wchar_t*)path.c_str(),row,(wchar_t*)colname.c_str());
		}
		else
		{
			PStrItem pItem;
			pItem = getDwItem(row,col);
			if(!pItem) return NULL;
			return pItem->pStrText;
		}
	}

	int CDwTable::setDwItemDisplayText(int row,int col,LPTSTR strValue) //设置行列项
	{
		int ACol,ARow;
		PStrItem pItem;
		if(getCellFromRowColumn(row,col,ACol,ARow)<0) return -1;
		pItem = getDwItem(row,col);
		if(!pItem) return -1;
		pItem->pStrText = _tcsdup(strValue);

		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = this->m_strBoundGroup;
			CDwTable * pDwParent = this->m_pParent;
			while(pDwParent)
			{
				path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
				pDwParent = pDwParent->m_pParent;
			}
			wstring colname = (wstring)getColumnName(col);
			colname += (wstring)_T("/@_displaystring");
			m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path.c_str(),row,(wchar_t*)colname.c_str(),strValue);
		}

		return 1;
	}

	int CDwTable::getColumnCount() //得到列数
	{
		return (int)this->m_dbcols.size();
	}
	LPTSTR CDwTable::getColumnName(int col) //得到列名
	{
		if(col > (int)m_dbcols.size()) return NULL;
		if(col < 1) return NULL;
		return m_dbcols[col - 1];
	}
	int CDwTable::getColumnIndex(LPTSTR pColName) //得到列名
	{
		for(int i=0;i<(int)m_dbcols.size();i++)
		{
			if(_tcsicmp(m_dbcols[i],pColName)==0)
				return i+1;
		}
		return -1;
	}


	//执行组预留行
	int CDwTable::DoSetGroupPreRow(int group)
	{
		if(group<1) return -1;
		if(group > (int)m_groups.size()) return -1;
		if(m_groups[group -1] >0) return -2;	//只能在数据行新增加时调用

		int sRow;
		int cellRows = this->m_RowGroupEndTempl - this->m_RowGroupStartTempl + 1;
		sRow = this->getGroupStartRow(group);
		for(int i=0;i<this->m_preRows;i++)
		{
			if(sRow - 1 >=(int)this->m_pRowCells.size())
				this->m_pRowCells.push_back(cellRows);
			else
				this->m_pRowCells.insert(m_pRowCells.begin() + sRow - 1,cellRows); 
			if((int)m_rowsIndex.size() < sRow)
				m_rowsIndex.push_back((int)m_Keys.size());
			else
				this->m_rowsIndex.insert(m_rowsIndex.begin() + sRow - 1,(int)m_Keys.size());
			m_StrKeys.push_back(NULL);
			m_Keys.push_back(-1);
			m_guidKeys.push_back(NULL);
			m_XmlRowNos.push_back(NULL);
			m_RowsStatus.push_back(rs_new);
		}
		this->m_groups[group - 1] = m_preRows;

		for(int i=0;i<this->m_preRows;i++)
		{
			for(int k=0;k<(int)this->m_pChilds.size();k++)
			{
				m_pChilds[k]->insertGroup(sRow + i);
			}
		}

		return -1;
	}


	//设置预留的行
	int CDwTable::DoSetPreRows()
	{
		return DoSetGroupPreRow(1);
	}

	//得到行数
	int CDwTable::GetRowCount()
	{
		return (int)m_pRowCells.size();
	}

	int CDwTable::DeleteRow()
	{
		return DeleteRow(0);
	}

	int CDwTable::DeleteRow(int row)
	{
		wstring path = this->m_strBoundGroup;
		CDwTable * pDwParent = this->m_pParent;
		while(pDwParent)
		{
			path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
			pDwParent = pDwParent->m_pParent;
		}
		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl)
			m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyDeleteRow((wchar_t*)path.c_str(),row);

		int group =1;
		if(row==0)
		{
			row = this->GetRow();
			if(row < 1) return 0;
		}

		group = this->getGroupFromRow(row);
		if(group < 1) return 0;
		int sRow,gRow;
		sRow = this->getGroupStartRow(group);
		gRow = row - sRow  + 1;
		return DeleteGRow(group,gRow);
	}

	int CDwTable::setDWTableDelElement(CDwTable * pCDwTable, IXMLDOMElement * Ele)
	{
		int i;
		for(i=0; i<(int)pCDwTable->m_pChilds.size(); i++)
		{
			LPTSTR path = pCDwTable->m_pChilds[i]->m_strBoundGroup;

			IXMLDOMElement* pElement = NULL; 
			IXMLDOMNode* pNode = NULL;
			IXMLDOMNodeList* pNodeList = NULL;
			Ele->selectNodes(path, &pNodeList);
			if(pNodeList)
			{
				long nLen;
				pNodeList->get_length(&nLen);
				int j;
				for(j=(int)nLen - 1; j>=0; j--)
				{
					pNodeList->get_item(j, &pNode);	
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)
					{
						_variant_t isnew;
						pElement->getAttribute(L"update.new",&isnew);
						if(isnew.bstrVal && ::_tcscmp(isnew.bstrVal,L"1")==0)
						{
							IXMLDOMNode* pNode2 = NULL;
							pElement->get_parentNode(&pNode2);
							if(pNode2)
							{
								pNode2->removeChild(pNode, NULL);
								pNode2->Release();
							}
						}
						else
						{
							pElement->setAttribute(L"update.delete",(_variant_t)_T("1"));
						}
						//if(isnew.bstrVal) ::SysFreeString(isnew);
						pElement->AddRef();
						setDWTableDelElement(pCDwTable->m_pChilds[i], pElement);
						pElement->Release();
					}
				}
				pNodeList->Release();
			}
		}

		return 1;
	}

	//删除行
	int CDwTable::DeleteGRow(int group,int grow)
	{
		int nDelPGroup = 0;

		int sCellRow,eCellRow;
		int arow = 0;

		arow = this->getGroupStartRow(group) + grow - 1;
		int rowid = this->m_rowsIndex[arow - 1];

		//删除所有子分组的行
/*		int i,j;
		for(i=0; i<(int)this->m_pChilds.size(); i++)
		{
			int cgroup = 1;
			int ii, crow = 0;
			for(ii=0; ii<=(int)this->m_pChilds[i]->m_groups.size(); ii++)
			{
				if(ii >= arow) break;
				crow += this->m_pChilds[i]->m_groups[ii];
			}
			cgroup = this->m_pChilds[i]->getGroupFromRow(crow);
			int crowcount = this->m_pChilds[i]->m_groups[cgroup - 1];
			for(j=0; j<crowcount; j++)
				this->m_pChilds[i]->DeleteGRow(cgroup, 1);
		}
*/
		if(this->m_groups[group - 1]==this->m_preRows)
			this->InsertGRow(group,0);

		if((m_RowsStatus[rowid]&0x0fff)!=rs_new && (m_RowsStatus[rowid]&0x0fff)!=rs_newmodified)
		{
			xbXml xml;
			IXMLDOMElement* pElement = NULL; 
			IXMLDOMNode* pNode = NULL;
			IXMLDOMNodeList* pNodeList = NULL;
			m_pGrid->GetGridDataSvr()->DwUpdate(xml.m_pXmlDoc);
			wstring XslPathParent = this->m_strBoundGroup;
			CDwTable * pDwParent = NULL;
			pDwParent = this->m_pParent;
			while(pDwParent)
			{
				XslPathParent = pDwParent->m_strBoundGroup + (wstring)_T("/") + XslPathParent;
				pDwParent = pDwParent->m_pParent;
			}
			xml.m_pXmlDoc->selectNodes((LPTSTR)XslPathParent.c_str(),&pNodeList);
			if(pNodeList)
			{
				long nLen;
				pNodeList->get_length(&nLen);
				if(int(nLen) >= grow)
					pNodeList->get_item(arow - 1, &pNode);
				pNodeList->Release();
			}
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();
			}
			if(pElement)
			{
				pElement->setAttribute(L"update.delete",(_variant_t)_T("1"));
				setDWTableDelElement(this, pElement);
				pElement->AddRef();
				m_delXml.push_back(pElement);
				pElement->Release();
			}
		}

		if((m_RowsStatus[rowid]&0x0fff)==rs_new ||(m_RowsStatus[rowid]&0x0fff)==rs_newmodified)
		{
			m_rowsIndex.erase(m_rowsIndex.begin() + arow -1);
		}else
		{
			m_rowsIndex.erase(m_rowsIndex.begin() + arow -1);
			this->m_delrowsIndex.push_back(rowid);
			
			CDwTable* pParent = this->m_pParent;
			int group = this->getGroupFromRow(arow);
			while(pParent)
			{
				pParent->m_RowsStatus[pParent->m_rowsIndex[group - 1]] =
					(RowStatus)(pParent->m_RowsStatus[pParent->m_rowsIndex[group - 1]]|rs_modified);

				group = pParent->getGroupFromRow(group);
				pParent=pParent->m_pParent;
			}
		}

		this->GetRowArea(arow,sCellRow,eCellRow);
		m_pGrid->DeleteRow(sCellRow, eCellRow - sCellRow + 1);
		this->m_pRowCells.erase(m_pRowCells.begin() + arow - 1);
		this->m_groups[group -1] -= 1;

		//adjust other dwtable
		CDwTable * pParent = this->m_pParent;

		int pRow;
		int scRow=1,ecRow;
		pRow = group;
		while(pParent)
		{
			pParent->m_pRowCells[pRow - 1] -= eCellRow - sCellRow + 1;
			pRow = pParent->getGroupFromRow(pRow);
			//调整跨区间数据
			pParent->GetRowArea(pRow,scRow,ecRow);
			for(int i=1;i<(int)pParent->m_dbcolsPos.size();i++)
			{
				if(pParent->m_dbcolsPos[i].y >= eCellRow - scRow)
				{
					if(pParent->m_dbcolsPos[i].y >= (eCellRow - sCellRow + 1))
						pParent->m_dbcolsPos[i].y -= eCellRow - sCellRow + 1;
				}
			}
			pParent = pParent->m_pParent;
		}

		/*while(pParent)
		{
			pParent->m_pRowCells[pRow - 1] -= eCellRow - sCellRow + 1;
			pRow = pParent->getGroupFromRow(pRow);
			pParent = pParent->m_pParent;
			//调整跨区间数据
			if(pParent && !pParent->m_pParent)
			{
				int scRow,ecRow;
				pParent->GetRowArea(pRow,scRow,ecRow);
				for(int i=1;i<(int)pParent->m_dbcolsPos.size();i++)
				{
					if(pParent->m_dbcolsPos[i].y >= eCellRow - scRow)
					{
						pParent->m_dbcolsPos[i].y -= eCellRow - sCellRow + 1;
					}
				}
			}
		}*/

		for(int n=0;n<(int)this->m_pChilds.size();n++)
		{
			m_pChilds[n]->deleteGroup(arow);
		}

		//AdjustDwTables(sCellRow,- (eCellRow - sCellRow + 1));

		m_pGrid->InvalidateGrid();
		return 0;
	}/*end delete grow */

	int CDwTable::AdjustDwTables(CGridFrameSvr* pGrid, int sCellPos,int Number)
	{
		for(int m=0;m<(int)pGrid->GetGridDataSvr()->/*m_pContentSvr->*/m_pDwTable.size();m++)
		{
			AdjustDwTables(pGrid->GetGridDataSvr()->/*m_pContentSvr->*/m_pDwTable[m],sCellPos,Number);
		}
		return 1;
	}

	int CDwTable::AdjustDwTables(CDwTable * pDwTable,int sCellPos,int Number)
	{
		if(pDwTable->getGroupStartCellRow(1) > sCellPos)
		{
			if(pDwTable->m_groupstart[0] + Number > 0)
				pDwTable->m_groupstart[0] += Number;
		}else
		{
			for(int i=0;i<(int)pDwTable->m_pChilds.size();i++)
			{
				AdjustDwTables(pDwTable->m_pChilds[i],sCellPos,Number);
			}
		}
		return 1;
	}

	int CDwTable::AdjustDwTables(int sCellPos,int Number)
	{
		return AdjustDwTables(m_pGrid,sCellPos,Number);
	}

	int CDwTable::InsertRow()
	{
		int row;
		row = this->GetRow();
		if(row < 1)
			return InsertRow(0);
		else
			return InsertRow(row);
	}

	int CDwTable::InsertRow(int row)
	{
		int group =1;
		if(row==0)
		{
			group=(int)this->m_groups.size();
			return InsertGRow(group,0); //append last;
		}else
		{
			group = this->getGroupFromRow(row);
			int sRow,gRow;
			sRow = this->getGroupStartRow(group);
			gRow = row - sRow  + 1;
			return InsertGRow(group,gRow);
		}
	}

	int bindNotifyInsertRow(CDwTable * pDwTable,int group, int grow,bool child)
	{
		wstring path = pDwTable->m_strBoundGroup;
		CDwTable * pDwParent = pDwTable->m_pParent;
		TCHAR buf[20];
		swprintf_s(buf, 20, L"%d", group);
		//if(pDwParent) 
		//	path = pDwParent->m_strBoundGroup + (wstring)_T("[position()='")+buf+L"']/" + path;
		//pDwParent = pDwParent->m_pParent;
		wstring mpath = L"";
		while(pDwParent)
		{
			if(mpath !=L"")
				mpath = pDwParent->m_strBoundGroup + (wstring)_T("/") + mpath;
			else
				mpath = pDwParent->m_strBoundGroup;
			pDwParent = pDwParent->m_pParent;
		}
		if(mpath != L"")
		{
			if((int)mpath.find(L"/") > 0)
				path = (wstring)L"(" + mpath + (wstring)L")" + (wstring)_T("[position()=")+buf+L"]/"+path;
			else
				path = mpath + (wstring)_T("[position()=")+buf+L"]/"+path;
		}
		pDwTable->m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyInsertRow((wchar_t*)path.c_str(),grow);
		if(child)
		{
			int arow = pDwTable->getGroupStartRow(group) + pDwTable->m_groups[group -1] - 1;
			for(int i=0;i<(int)pDwTable->m_pChilds.size();i++)
			{
				bindNotifyInsertRow(pDwTable->m_pChilds[i],arow,0,true);
			}
		}
		return 1;
	}

	int CDwTable::InsertGRow(int group,int grow)
	{
		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl)
		{
			bindNotifyInsertRow(this,group,grow,false);
		}
		int row = InsertGRow(group,grow,1);
		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl)
		{
			int arow = group==0?
				arow = this->getGroupStartRow(group) + grow - 1:
					getGroupStartRow(group) + m_groups[group -1] - 1;
			for(int i=0;i<(int)m_pChilds.size();i++)
			{
				bindNotifyInsertRow(m_pChilds[i],arow,0,true);
			}
		}
		return row;
	}

	// 插入新行
	int CDwTable::InsertGRow(int group,int grow,int amt,bool redraw)
	{
		int sCellRow,eCellRow;
		int arow = 0;

		if(grow==0) //append group row
		{
			this->GetRowArea(this->getGroupStartRow(group) + this->m_groups[group -1] - 1,sCellRow,eCellRow);
			m_pGrid->InsertRow(eCellRow + 1, (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*amt ,false);
			//调整式样
			for(int k=0;k<amt;k++)
			{
				for(int i=m_RowGroupStartTempl;i<=m_RowGroupEndTempl;i++)
				{
					int ARow = eCellRow + (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*k + i - m_RowGroupStartTempl + 1;
					int ACol = m_pTemplGridData->m_pContentSvr->GetValidCol(i);
					for(int l=1;l<=ACol;l++)
					{
						m_pGrid->GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(l,ARow,
							(Hxsoft::XUtil::Css::TBoxStyle *)m_pTemplGridData->GetStyleSvr()->GetCellCssStyle(l,i));
					}
				}
			}

			if(group==m_groups.size())
			{
				for(int i=0;i<amt;i++)
				{
					m_pRowCells.push_back(m_RowGroupEndTempl - m_RowGroupStartTempl + 1);
				}
				this->m_groups[group -1] += 1*amt;
				arow = this->GetRowCount() - amt + 1;
			}else
			{
				for(int i=0;i<amt;i++)
				{
					m_pRowCells.insert(m_pRowCells.begin() + this->getGroupStartRow(group + 1) - 1,(m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*amt);
				}
				this->m_groups[group -1] += 1*amt;
				arow = getGroupStartRow(group + 1) - 1 - amt + 1; 
			}

			sCellRow = eCellRow + 1;
			eCellRow = sCellRow + (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*amt - 1;
		}else
		{
			arow = this->getGroupStartRow(group) + grow - 1;
			this->GetRowArea(arow,sCellRow,eCellRow);

			for(int i=0;i<amt;i++)
			{
				m_pGrid->InsertRow(sCellRow, (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*amt ,false);
			}

			this->m_pRowCells.insert(m_pRowCells.begin() + arow - 1,m_RowGroupEndTempl - m_RowGroupStartTempl + 1);
			this->m_groups[group -1] += 1;

			eCellRow = sCellRow + (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*amt - 1;
		}

		for(int n=0;n<amt;n++)
		{
			//if(arow + n >= (int)this->m_rowsIndex.size())
			if(arow + n > (int)this->m_rowsIndex.size())
				this->m_rowsIndex.push_back((int)m_Keys.size());
			else
				this->m_rowsIndex.insert(m_rowsIndex.begin() + arow -1 + n,(int)m_Keys.size());

			m_StrKeys.push_back(NULL);
			m_Keys.push_back(-1);
			m_guidKeys.push_back(NULL);
			m_XmlRowNos.push_back(NULL);
			m_RowsStatus.push_back(rs_new);
		}

		//adjust other dwtable
		CDwTable * pParent = this->m_pParent;

		int pRow;
		pRow = group;
		while(pParent)
		{
			pParent->m_pRowCells[pRow - 1] += eCellRow - sCellRow + 1;
			pRow = pParent->getGroupFromRow(pRow);
			//调整跨区间数据
			int scRow,ecRow;
			pParent->GetRowArea(pRow,scRow,ecRow);
			for(int i=1;i<(int)pParent->m_dbcolsPos.size();i++)
			{
				if(pParent->m_dbcolsPos[i].y >= sCellRow - scRow)
				{
					pParent->m_dbcolsPos[i].y += eCellRow - sCellRow + 1;
				}
			}
			pParent = pParent->m_pParent;
			//pRow = pParent->getGroupFromRow(pRow);
		}
		/*while(pParent)
		{
			pParent->m_pRowCells[pRow - 1] += eCellRow - sCellRow + 1;
			pRow = pParent->getGroupFromRow(pRow);
			pParent = pParent->m_pParent;
			//调整跨区间数据
			if(pParent && !pParent->m_pParent)
			{
				int scRow,ecRow;
				pParent->GetRowArea(pRow,scRow,ecRow);
				for(int i=1;i<(int)pParent->m_dbcolsPos.size();i++)
				{
					//if(pParent->m_dbcolsPos[i].y>=sCellRow - ecRow)
					if(pParent->m_dbcolsPos[i].y >= sCellRow - scRow)
					{
						pParent->m_dbcolsPos[i].y += eCellRow - sCellRow + 1;
					}
				}
			}
		}*/

		for(int n=0;n<amt;n++)
		{
			for(int j=0;j<(int)this->m_pChilds.size();j++)
			{
					m_pChilds[j]->insertGroup(arow + n);
			}
		}

		//AdjustDwTables(sCellRow,eCellRow - sCellRow + 1);

		int ACol;
		PStrItem pItem;

		int eCellRowEx = eCellRow;
		eCellRow = sCellRow + m_RowGroupEndTempl - m_RowGroupStartTempl;
		for(int i=sCellRow;i<=eCellRow ;i++)
		{
			//adjust cell item
			ACol = m_pTemplGridData->m_pContentSvr->GetValidCol(m_RowGroupStartTempl + i - sCellRow);
			for(int j=1;j<=ACol;j++)
			{
				//if is spaned cell then continue
				TGridRect srect;
				m_pTemplGridData->m_pSpanSvr->isSpaned(j,m_RowGroupStartTempl + i - sCellRow,srect);
				//if(srect.left !=j ||srect.top != m_RowGroupStartTempl + i - sCellRow)continue;

				//pItem = m_pTemplGridData->m_pContentSvr->GetItemData(j,m_RowGroupStartTempl + i - sCellRow);
				if(srect.left !=j ||srect.top > m_RowGroupStartTempl + i - sCellRow)continue;
				pItem = m_pTemplGridData->m_pContentSvr->GetItemData(j,srect.top);

				LPTSTR groupStr,columnStr;
				bool	bPGroup = false;

				if(pItem )
				{
					groupStr = pItem->pStrGroup;
					columnStr = pItem->pStrColumn;
					if(!(groupStr && columnStr) && pItem->pStrContent)
					{
						PStrItem pNewItem;
						for(int m=0;m<amt;m++)
						{
							pNewItem = new StrItem;
							pNewItem->CopyFrom(pItem);
							m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemData(j,i + (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*m,pNewItem);
						}
						continue;
					}
					if(!groupStr) continue;
					if(!columnStr) continue;

					if(columnStr && columnStr[0]!='\0')
					{
						if(pItem->pStrContent)
						{
							delete pItem->pStrContent; pItem->pStrContent= NULL;
						}
					}
					pParent = this->m_pParent;
					while(pParent)
					{
						if(_tcsicmp(pParent->m_strBoundGroup,groupStr)==0)break;
						pParent = pParent->m_pParent;
					}
					if(pParent)
					{
						bPGroup = true;
						if(srect.bottom== m_RowGroupStartTempl + eCellRow - sCellRow)
						{
							int spRow,epRow;
							CDwTable * pParent1;
							int pgroup = group;
							pParent1 = this->m_pParent;
							while(pParent1 != pParent)
							{
								pgroup = pParent1->getGroupFromRow(pgroup);
								pParent1 = pParent1->m_pParent;
							}
							pParent->GetRowArea(pgroup,spRow,epRow);
							int cnt = m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects.size();
							int m=0;
							for(m=0;m<cnt;m++)
							{
								if(m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects[m].top == spRow + i - sCellRow && m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects[m].left ==j)break;

							}
							if(m<cnt)
							{
								m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects[m].bottom = epRow ;
							}else
							{
								RECT rect;
								rect.left = srect.left;rect.top = srect.top;
								rect.right = srect.right;rect.bottom = srect.bottom;
								rect.top = spRow + i - sCellRow;
								rect.bottom = epRow ;
								m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(rect);
							}
						}
					}
				}
				if(pItem && !bPGroup)
				{
					PStrItem pNewItem;
					for(int m=0;m<amt;m++)
					{
						pNewItem = new StrItem;
						pNewItem->CopyFrom(pItem);
						m_pGrid->GetGridDataSvr()->m_pContentSvr->SetItemData(j,i + (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*m,pNewItem);
					}
				}
			}
			//adjust row heights
			for(int m=0;m<amt;m++)
			{
				m_pGrid->SetRowHeight(i +  (m_RowGroupEndTempl - m_RowGroupStartTempl + 1)*m,m_pTemplGridData->m_pAxisSvr->GetRowHeight(m_RowGroupStartTempl + i - sCellRow));
			}
		}
		
		//adjust span rect;
		int cnt = m_pTemplGridData->m_pSpanSvr->SpanRects.size();
		int dcnt=this->m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects.size();
		RECT mrect;
		for(int i=0;i< cnt;i++)
		{
			//if(m_pTemplGridData->m_pSpanSvr->SpanRects[i].top < m_RowGroupStartTempl ||m_pTemplGridData->m_pSpanSvr->SpanRects[i].bottom > m_RowGroupEndTempl) continue;
			if(m_pTemplGridData->m_pSpanSvr->SpanRects[i].bottom < m_RowGroupStartTempl ||m_pTemplGridData->m_pSpanSvr->SpanRects[i].top > m_RowGroupEndTempl) continue;
			RECT rect = m_pTemplGridData->m_pSpanSvr->SpanRects[i];
			if(rect.top < m_RowGroupStartTempl) rect.top =  m_RowGroupStartTempl;
			if(rect.bottom > m_RowGroupEndTempl) rect.bottom =  m_RowGroupEndTempl;
			if(rect.bottom==rect.top && rect.left==rect.right)continue;
			rect.top = rect.top - m_RowGroupStartTempl + sCellRow;
			rect.bottom = rect.bottom - m_RowGroupStartTempl + sCellRow;

			int j;
			for(j=0;j<dcnt;j++)
			{
				mrect = m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects[j];
				if(mrect.left <=rect.left && mrect.top <=rect.top && mrect.right>=rect.right && mrect.bottom >=rect.bottom)break;
			}
			if(j >= dcnt) 
			{
				for(int m=0;m<amt;m++)
				{
					m_pGrid->GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(rect);
					rect.top += m_RowGroupEndTempl - m_RowGroupStartTempl + 1;
					rect.bottom += m_RowGroupEndTempl - m_RowGroupStartTempl + 1;
				}
			}
		}

		if(redraw)m_pGrid->InvalidateGrid();

		return arow;
	}

	// 增加行
	int CDwTable::AppendRow(void)
	{
		int row;
		row = this->GetRow();
		if(row < 1) 
			return this->InsertRow(0);
		else
		{
			int group;
			group = this->getGroupFromRow(row);
			if(group <0)
				return this->InsertRow(0);
			else
				return this->InsertGRow(group,0);
		}
	}

	// 增加行
	int CDwTable::AppendGRow(int group)
	{
		return InsertGRow(group,0);
	}

	//判断是否匹配组
	LPTSTR CDwTable::GetMatchGroupTempl(LPCTSTR szGroup,LPTSTR szGroupStr)
	{
		int pos=0;
		int gpos=0;
		int gspos=0;
		int spos=0;

		if(!szGroupStr) return NULL;
		if(!szGroup) return NULL;

		//trim left space for szGroup
		while(szGroup[gspos]==' '||szGroup[gspos]=='\t'||szGroup[gspos]=='\r'||szGroup[gspos]=='\n'){gspos++;}
		if(szGroup[gspos]=='\0') return NULL; //not is valid szGroup Argument

		//trim left space for szGroupStr
		while(szGroupStr[pos]==' '||szGroupStr[pos]=='\t'||szGroupStr[pos]=='\r'||szGroupStr[pos]=='\n'){spos++;}
		if(szGroupStr[pos]=='\0') return NULL; //not is valid szGroupStr Argument

		//trim sperate char
		while(szGroupStr[pos]==';'){spos++;}

		//now start match
		while(true)
		{
			//try match string
			spos = pos;
			gpos = gspos;
			while(szGroupStr[pos]==szGroup[gpos] && szGroupStr[pos]!='\0' && szGroup[gpos]){pos++;gpos++;}

			//continue to trim szgroup right space;
			while(szGroup[gpos]==' '||szGroup[gpos]=='\t'||szGroup[gpos]=='\r'||szGroup[gpos]=='\n'){gpos++;}

			//continue to trim szGroupStr right space
			while(szGroupStr[pos]==' '||szGroupStr[pos]=='\t'||szGroupStr[pos]=='\r'||szGroupStr[pos]=='\n'){pos++;}

			//matched
			if(szGroup[gpos]=='\0' && (szGroupStr[pos]=='\0' || szGroupStr[pos]==';')) return szGroupStr + spos;

			//trim balanace char
			while(szGroupStr[pos] !='\0' && szGroupStr[pos] !=';'){pos++;}

			//trim sperate char
			while(szGroupStr[pos]==';'){pos++;}
			if(szGroupStr[pos]=='\0') return NULL;
		}
		return NULL;
	}

	//去除左边的第一个组
	LPTSTR CDwTable::TrimLeftGroupTempl(LPTSTR szGroupStr)
	{
		int pos=0;

		if(!szGroupStr) return NULL;
		while(szGroupStr[pos]!='\0' && szGroupStr[pos]!=';')pos++;
		if(szGroupStr[pos]=='\0')return NULL;
		pos++;
		if(szGroupStr[pos]=='\0')return NULL;
		return szGroupStr + pos;
	}

	bool CDwTable::LoadTemplData(int row, LPTSTR szGroupStr,CGridDataSvr * m_pData)
	{
		int ARow;
		int ACol;
		PStrItem	pItem;
		int sRow,eRow;
		int spos=0,pos=0;
		LPTSTR szGroup = NULL;
		LPTSTR	str;
		TCHAR ch;


		if(!szGroupStr) return false;
		//get szGroup;
		//trim space of szGroupStr
		while(szGroupStr[spos]==' '||szGroupStr[spos]=='\t'||szGroupStr[spos]=='\r'||szGroupStr[spos]=='\n'){spos++;}
		pos = spos;
		while(szGroupStr[pos] != ';'&& szGroupStr[pos] != '\0' && szGroupStr[pos] != '\r' && szGroupStr[pos] != '\n' &&szGroupStr[pos] != ' ' && szGroupStr[pos] != '\t')pos++;
		ch = szGroupStr[pos];
		szGroupStr[pos]='\0';
		szGroup = szGroupStr + spos;

		m_pData->m_pContentSvr->GetValidRowCol(ACol,ARow);
		sRow = row;
		eRow = ARow;

		if(m_strBoundGroup) delete m_strBoundGroup;
		if(::_tcschr(szGroup,'{')>0)
		{
			m_strBoundGroup = new TCHAR[_tcschr(szGroup,'{') + 1 - szGroup];
			::_tcsncpy_s(m_strBoundGroup ,_tcschr(szGroup,'{') + 1 - szGroup,szGroup,_tcschr(szGroup,'{') - szGroup);
			szGroup = _tcschr(szGroup,'{') + 1;
			if(_tcschr(szGroup,'}') > 0)
			{
				LPTSTR pStr = _tcschr(szGroup,'}');
				pStr[0] ='\0';
				this->m_preRows = _ttol(szGroup);
				pStr[0]='}';
			}
			else
				this->m_preRows = _ttol(szGroup);
		}else
			this->m_strBoundGroup = _tcsdup(szGroup);
		szGroup = this->m_strBoundGroup;
		szGroupStr[pos] =ch;

		int k = 0;
		for(k=sRow; k<=ARow; k++)
		{
			pItem = m_pData->m_pContentSvr->GetItemData(0,k);
			if(!pItem)continue;
			if(pItem->pStrPreLast && GetMatchGroupTempl(szGroup,pItem->pStrPreLast))
			{
				eRow = k;
				break;
			}
		}

		m_pChilds.clear();
		m_RowGroupStartTempl = sRow;
		m_RowGroupEndTempl  = eRow;

		//start add Child dwtable

		//clear child dwtable
		int size =  (int)m_pChilds.size();
		for(int i = size - 1 ; i >=0; i --)delete  m_pChilds[i];
		//end clear child dwtable

		str = this->TrimLeftGroupTempl(szGroupStr);
		if(str)
		{
			CDwTable*	pDwChild = new CDwTable();
			pDwChild->m_pParent = this;
			pDwChild->m_pTemplGridData = this->m_pTemplGridData;
			m_pChilds.push_back(pDwChild);
			pDwChild->LoadTemplData(row,str,m_pData);
			sRow =pDwChild->m_RowGroupEndTempl + 1;
		}else
			sRow = row + 1;

		for(k=sRow;k<=eRow;k++)
		{
			pItem = m_pData->m_pContentSvr->GetItemData(0,k);
			if(!pItem || !pItem->pStrPre)continue;
			spos = 0;
			str = pItem->pStrPre;
			while(str[spos]==' '||str[spos]=='\t'||str[spos]=='\r'||str[spos]=='\n'||str[spos]==';'){spos++;}
			if(str[spos]=='\0')continue;
			str += spos;

			CDwTable*	pDwChild = new CDwTable();
			pDwChild->m_pParent = this;
			pDwChild->m_pTemplGridData = this->m_pTemplGridData;
			m_pChilds.push_back(pDwChild);
			pDwChild->LoadTemplData(k,str,m_pData);
			k =pDwChild->m_RowGroupEndTempl;
		}
		//end add child dwtable;
		if(this->m_RowGroupStartTempl < 1) this->m_RowGroupStartTempl =1;

		this->getColumns(m_dbcols,m_dbcolsPos);

		return true;
	}


	//获取组在模板中的起始行
	int CDwTable::GetRowGroupStartTempl(LPCTSTR szGroup,CGridDataSvr * m_pData)
	{
		int ARow;
		int ACol;
		PStrItem	pItem;

		m_pData->m_pContentSvr->GetValidRowCol(ACol,ARow);
		for(int row=0; row<=ARow; row++)
		{
			pItem = m_pData->m_pContentSvr->GetItemData(0,row);
			if(!pItem)continue;
			if(pItem->pStrPre && GetMatchGroupTempl(szGroup,pItem->pStrPre))
			{
				return row == 0 ? 1:row;
			}
		}
		return -1;
	}

	//获取组在模板中的结束行
	int CDwTable::GetRowGroupEndTempl(LPCTSTR szGroup,CGridDataSvr * m_pData)
	{
		int ARow;
		int ACol;
		PStrItem	pItem;

		m_pData->m_pContentSvr->GetValidRowCol(ACol,ARow);
		for(int row=0; row<=ARow; row++)
		{
			pItem = m_pData->m_pContentSvr->GetItemData(0,row);
			if(!pItem)continue;
			if(pItem->pStrPreLast && GetMatchGroupTempl(szGroup,pItem->pStrPreLast)) return row;
		}
		return ARow;
	}

	int CDwTable::GetMaxDeep()
	{
		int dl = 0;
		int mdl = 0;
		for(int i=0;i< (int)this->m_pChilds.size();i++)
		{
			dl = m_pChilds[i]->GetMaxDeep();
			if(dl > mdl ) mdl = dl;
		}
		return mdl + 1;
	}

	int CDwTable::GetLevel()
	{
		int level = 0;
		CDwTable * pParent = this;
		while(pParent)
		{
			level++;
			pParent = pParent->m_pParent;
		}
		return level;
	}

	int CDwTable::GetBandInfo(vector<POINT> &pts,vector<int> &levels, vector<wstring> &Bands,vector<CDwTable *> &pDws)
	{
		int		level;
		POINT	pt;
		level = this->GetLevel();
		levels.push_back(level);
		pt.x = this->m_RowGroupStartTempl;
		pt.y = this->m_RowGroupEndTempl;
		pts.push_back (pt);
		Bands.push_back(_T("明细"));
		pDws.push_back(this);
		for(int i=0;i<(int)this->m_pChilds.size();i++)
		{
			m_pChilds[i]->GetBandInfo(pts,levels,Bands,pDws);
		}

		return 1;
	}

	int CDwTable::getGroupRowCount(int group)
	{
		if(group > (int)this->m_groups.size()) return 0;
		return this->m_groups[group - 1];
	}

	//增加组
	int CDwTable::insertGroup(int group)
	{
		//first group is 1
		if(group<0) return -1;
		if(!this->m_pParent && group==1)
		{
			this->m_groups.push_back( 0);
			this->m_groupstart.push_back(this->m_RowGroupStartTempl);
			this->DoSetGroupPreRow((int)m_groups.size());
			return (int)this->m_groups.size(); 
		}
		if(group > (int)this->m_groups.size())group = 0;
		if(group == 0)
		{
			this->m_groups.push_back(0); 
			this->m_groupstart.push_back(this->m_RowGroupStartTempl - this->m_pParent->m_RowGroupStartTempl);//参照父对象
			this->DoSetGroupPreRow((int)m_groups.size());
			return (int)this->m_groups.size(); 
		}else
		{
			this->m_groups.insert(m_groups.begin( ) + group - 1,0);
			this->m_groupstart.insert(m_groupstart.begin( ) + group - 1,this->m_RowGroupStartTempl - this->m_pParent->m_RowGroupStartTempl);
			this->DoSetGroupPreRow(group);
			return group;
		}
		return 0;
	}

	//删除组
	int CDwTable::deleteGroup(int group)
	{
		if(group < 1) return -1;
		if(group > (int)this->m_groups.size()) return -1;

		int row;
		row = this->getGroupStartRow(group);
		for(int i=row + this->m_groups[group -1] - 1;i>=row;i--)
		{
			for(int j=0; j < (int)this->m_pChilds.size();j++)
			{
				m_pChilds[j]->deleteGroup(row);
			}
			this->m_pRowCells.erase(m_pRowCells.begin() + i - 1);
		}
		this->m_groups.erase(this->m_groups.begin() + group - 1);
		return 1;
	}


	//得到组的起始行
	int CDwTable::getGroupStartRow(int group)
	{
		int rows=0;
		if(group < 1) return -1;
		if(group==1) return 1;
		if(group > (int)this->m_groups.size()) return -1;
		rows = 1;
		for(int i=1;i<group;i++)
		{
			rows += m_groups[i - 1];
		}
		return rows;
	}

	//得到组的起始表格行
	int CDwTable::getGroupStartCellRow(int group)
	{
		int cellRow;
		if(group < 1) return -1;

		if(group > (int)m_groups.size()) return -1;

		if(!this->m_pParent) return this->m_groupstart[0];
		int pGroup = m_pParent->getGroupFromRow(group);
		if(pGroup < 1) return -1;

		int psRow;
		cellRow = m_pParent->getGroupStartCellRow(pGroup);
		psRow = m_pParent->getGroupStartRow(pGroup);
		for(int i=psRow ;i< group;i++)
		{
			cellRow += m_pParent->m_pRowCells[i -1];
		}
		cellRow += this->m_groupstart[group - 1];

		return cellRow;
	}

	//得到组的起始表格行
	int CDwTable::getGroupFromRow(int row)
	{
		if(row < 1) return -1;
		if(row > (int)this->m_pRowCells.size()) return -1;
		if(this->m_groups.size()<1) return 1;

		int i;
		int rows=0;
		for(i=1;i<(int)m_groups.size();i++)
		{
			rows += m_groups[i - 1];
			if(rows >= row) return i;
		}
		return i;
	}

	//得到组的表格行区间
	bool CDwTable::getGroupCellArea(int group, int &sCellRow,int &eCellRow)
	{
		if(group < 1) return false;
		if(group > 1 && !this->m_pParent) return false;

		if(group ==1 && m_groups.size()==1)
		{
			return this->GetTableCellArea(sCellRow,eCellRow);
		}

		int sRow;
		sCellRow = this->getGroupStartCellRow(group);
		sRow = this->getGroupStartRow(group);

		eCellRow = sCellRow;
		for(int i=sRow;i<= sRow + this->m_groups[group -1] - 1;i++)
		{
			eCellRow += this->m_pRowCells[i - 1];
		}
		eCellRow --;
		return true;
	}

	//得到组的数据行区间
	bool CDwTable::getGroupRowArea(int group, int &sRow,int &eRow)
	{
		if(group > 1 && group > (int)m_groups.size()) return false;
		sRow = this->getGroupStartRow(group);
		eRow = sRow + this->m_groups[group -1] - 1 ;

		return true;
	}

	//从表格行得到组
	int CDwTable::getGroupFromCellRow(int cellRow)
	{
		int sCellRow;
		int i=0;
		for(i=0;i<(int)this->m_groups.size();i++)
		{
			sCellRow = this->getGroupStartCellRow(i + 1);
			if(cellRow < sCellRow) return i ;
		}
		return i;
	}

	//得到数据表的表格行区间
	bool CDwTable::GetTableCellArea(int &sCellRow, int &eCellRow) 
	{
		if(this->GetRowCount() < 1)
		{
			sCellRow = this->getGroupStartCellRow(1);
			eCellRow = this->getGroupStartCellRow(1) + this->m_RowGroupEndTempl - this->m_RowGroupStartTempl;
		}else
		{
			int tCellRow;
			this->GetRowArea(1,sCellRow,tCellRow);
			this->GetRowArea(this->GetRowCount(),tCellRow,eCellRow);
		}
		return true;
	}


	//得到行的表格区域
	int CDwTable::GetRowArea(int row, int& sCellRow,int& eCellRow)
	{
		if(row > (int)m_pRowCells.size()) return -1;

		int group;
		int sgCellRow;
		int sgRow;

		group = getGroupFromRow(row);
		sgCellRow = getGroupStartCellRow(group);
		sgRow = this->getGroupStartRow(group);

		if(sgRow < 0) return -1;

		sCellRow = sgCellRow;
		for(int i=sgRow;i<=row;i++)
		{
			if(i>sgRow)
				sCellRow = eCellRow + 1;
			eCellRow = sCellRow +  m_pRowCells[i - 1] - 1;
		}
		return 1;
	}

	// 设置行焦点
	int CDwTable::SetRow(int row)
	{
		if(GetRowCount() < 1) return -1;
		if(row == 0) row = GetRowCount() ;
		int ACol,ARow;
		getCellFromRowColumn(row,1,ACol,ARow);
		int nOldRow = m_pGrid->FCurrent.y;
		m_pGrid->FCurrent.y = ARow;
		m_pGrid->FocusCell(m_pGrid->FCurrent.x,ARow,true);
		event_RowFocusChanged(ARow,nOldRow);
		
		return row;
	}

	// 得到当前行
	int CDwTable::GetRow()
	{
		return GetRowFromCell(m_pGrid->FCurrent.y);
	}

	// 从表格行中得到当前行
	int CDwTable::GetRowFromCell(int cellRow)
	{
		if(cellRow < this->getGroupStartCellRow(1)) return -1;
		if(this->GetRowCount() < 1) return -1;

		int sCellRow,eCellRow;
		this->GetTableCellArea(sCellRow,eCellRow);
		if(cellRow < sCellRow || cellRow > eCellRow) return -1;

		int startCellRow ;
		if(!m_pParent)
		{
			startCellRow = this->m_groupstart[0];
			for(int i=0;i<(int)this->m_pRowCells.size();i++)
			{
				if(cellRow < startCellRow + m_pRowCells[i]) return i + 1;
				startCellRow += m_pRowCells[i];
			}
		}else
		{
			int group;
			int sRow;
			group = getGroupFromCellRow(cellRow);
			startCellRow = this->getGroupStartCellRow(group);
			sRow = this->getGroupStartRow(group);
			for(int i=sRow;i<=sRow + this->m_groups[group -1] - 1;i++)
			{
				if(cellRow < startCellRow + m_pRowCells[i - 1]) return i;
				startCellRow += m_pRowCells[i - 1];
			}
		}
		return -1;
	}

	bool CDwTable::SetGuid(int row,LPTSTR guid,DWBuffer dwBuffer/*=dbuf_normal*/)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return false;
			this->m_guidKeys[m_delrowsIndex[row -1]] = ::_tcsdup(guid);
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return false;
			this->m_guidKeys[m_rowsIndex[row -1]] = ::_tcsdup(guid);
		}
		return true;
	}
	LPCTSTR	CDwTable::GetGuid(int row,DWBuffer dwBuffer/*=dbuf_normal*/)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return NULL;
			return this->m_guidKeys[m_delrowsIndex[row -1]];
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return NULL;
			return this->m_guidKeys[m_rowsIndex[row -1]];
		}
	}

	bool	CDwTable::SetStringKeyValue(int row,LPTSTR keyValue,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return false;
			this->m_StrKeys[m_delrowsIndex[row -1]] = ::_tcsdup(keyValue);
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return false;
			this->m_StrKeys[m_rowsIndex[row -1]] = ::_tcsdup(keyValue);
		}
		return true;
	}
	bool	CDwTable::SetKeyvalue(int row,int keyValue,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return false;
			this->m_Keys[m_delrowsIndex[row -1]] = keyValue;
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return false;
			this->m_Keys[m_rowsIndex[row -1]] = keyValue;
		}
		return true;
	}
	LPCTSTR	CDwTable::GetStringKeyvalue(int row,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return NULL;
			return this->m_StrKeys[m_delrowsIndex[row -1]];
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return NULL;
			return this->m_StrKeys[m_rowsIndex[row -1]];
		}
	}
	int		CDwTable::GetKeyvalue(int row,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return NULL;
			return this->m_Keys[m_delrowsIndex[row -1]];
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return NULL;
			return this->m_Keys[m_rowsIndex[row -1]];
		}
	}
	RowStatus	CDwTable::GetItemStatus(int row,int col,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return rs_new;
			return (RowStatus)(this->m_RowsStatus[m_delrowsIndex[row -1]]&0x0fff);
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return rs_new;
			return (RowStatus)(this->m_RowsStatus[m_rowsIndex[row -1]]&0x0fff);
		}
	}

	bool	CDwTable::SetItemStatus(int row,int col,RowStatus status,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return false;
			this->m_RowsStatus[m_delrowsIndex[row -1]] = 
				(RowStatus)((this->m_RowsStatus[m_delrowsIndex[row -1]]&0xf000)|status);
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return false;
			this->m_RowsStatus[m_rowsIndex[row -1]] = 
				(RowStatus)((this->m_RowsStatus[m_rowsIndex[row -1]]&0xf000)|status);
		}
		return true;
	}


	bool	CDwTable::SetXmlRowId(int row,int xmlRowId,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return false;
			this->m_XmlRowNos[m_delrowsIndex[row -1]] = xmlRowId;
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return false;
			this->m_XmlRowNos[m_rowsIndex[row -1]] = xmlRowId;
		}
		return true;
	}
	int 	CDwTable::GetXmlRowId(int row,DWBuffer dwBuffer)
	{
		if(dwBuffer==dbuf_delete)
		{
			if(row > (int)this->m_delrowsIndex.size()) return -1;
			return this->m_XmlRowNos[m_delrowsIndex[row -1]];
		}else
		{
			if(row > (int)this->m_rowsIndex.size()) return -1;
			return this->m_XmlRowNos[m_rowsIndex[row -1]];
		}
	}

	int		CDwTable::GetDeletedCount()
	{
		return (int)this->m_delrowsIndex.size();
	}
	int		CDwTable::GetModifiedCount()
	{
		int count=0;
		for(int i=0;i<(int)this->m_rowsIndex.size();i++)
			if( (this->m_RowsStatus[m_rowsIndex[i]]&0x0fff) == rs_newmodified || (this->m_RowsStatus[m_rowsIndex[i]]&0x0fff)== rs_datamodified)count++;
		return count;
	}

	int CDwTable::Reset()
	{
		int Count = GetRowCount();
		while(Count > 0)
		{
			this->DeleteRow(Count);
			Count--;
		}

		int index;
		for(index=0;index < (int)m_StrKeys.size();index++)
		{
			if(m_StrKeys[index]) delete m_StrKeys[index];
			if(m_guidKeys[index]) delete m_guidKeys[index];
		}

		m_rowsIndex.clear();
		m_delrowsIndex.clear();

		m_StrKeys.clear() ;
		m_Keys.clear() ;
		m_guidKeys.clear() ;
		m_XmlRowNos.clear() ;
		m_RowsStatus.clear() ;

		return 1;
	}

	int	CDwTable::ResetUpdateStatus()
	{
		int i=0;
		for(int i=0;i<(int)this->m_rowsIndex.size();i++)
		{
			int colCount = this->getColumnCount();
			if((m_RowsStatus[m_rowsIndex[i]]&0x0fff)==rs_newmodified || (m_RowsStatus[m_rowsIndex[i]]&0x0fff)==rs_datamodified)
			{
				int row = i+1;
				PStrItem pItem;
				for(int j=1;j<=colCount; j++)
				{
					pItem = getDwItem(row,j);
					if(pItem && pItem->bDirect)
					{
						pItem->bDirect = false;
						if(pItem->pOrigin)delete pItem->pOrigin;
						pItem->pOrigin = NULL;
					}
				}
			}
			if((m_RowsStatus[m_rowsIndex[i]]&0x0fff)!=rs_new)
			{
				m_RowsStatus[m_rowsIndex[i]] =
					(RowStatus)((m_RowsStatus[m_rowsIndex[i]] &0xf000)|rs_datanotmodified);
			}
		}
		this->m_delrowsIndex.clear();
		this->m_delXml.clear();
		this->m_delGroup.clear();

		for(int j=0;j< (int)m_pChilds.size();j++)
		{
			m_pChilds[j]->ResetUpdateStatus();
		}
		return 1;
	}

	int CDwTable::AddObserver(xbObserver * pObserver)
	{
		m_pObServers.push_back(pObserver);
		return 1;
	}
	int CDwTable::RemoveObserver(xbObserver * pObserver)
	{
		for(int i= 0;i < (int)m_pObServers.size();i++)
		{
			if(m_pObServers[i] == pObserver)
			{
				m_pObServers.erase(m_pObServers.begin()+i);
				break;
			}
		}
		return 1;
	}
	int CDwTable::ClearObserver()
	{
		m_pObServers.clear();
		return 1;
	}

	xbObject*  CDwTable::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IDwTable();
		((IDwTable *)m_pInterFace)->m_pDwTable = this;
		return m_pInterFace;
	}

	int CDwTable::event_ItemFocusChanged(int nRow,int nCol)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMFOCUSCHANGED;
		hdr.DwTable = (IDwTable *)this->GetInterface();
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = getColumnName(nCol);

		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		for(int i=0;i<(int)m_pObServers.size();i++)
			((IDwTableObserver *)m_pObServers[i])->ItemFocueChanged((IDwTable *)GetInterface(),nRow,nCol);
		return 1;
	}
	int CDwTable::event_RowFocusChanged(int nRow,int nOldRow)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ROWFOCUSCHANGED;
		hdr.DwTable = (IDwTable *)this->GetInterface();
		hdr.row = nRow;
		hdr.oldrow = nOldRow;
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IDwTableObserver *)m_pObServers[i])->RowFoucsChanged((IDwTable *)GetInterface(),nRow,nOldRow);
		return 1;
	}
	int CDwTable::event_GetFocus( CDwTable * pLoseDwTable)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_GETFOCUS;
		if(pLoseDwTable)
			hdr.DwTable = (IDwTable *)pLoseDwTable->GetInterface();
		else
			hdr.DwTable = 0;
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			if(pLoseDwTable)
				((IDwTableObserver *)m_pObServers[i])->GetFocus((IDwTable *)pLoseDwTable->GetInterface());
			else
				((IDwTableObserver *)m_pObServers[i])->GetFocus(NULL);

		}
		return 1;
	}
	int CDwTable::event_LoseFocus(CDwTable * pFocusedDwTable)
	{
		this->AcceptText();
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_LOSEFOCUS;
		if(pFocusedDwTable)
			hdr.DwTable = (IDwTable *)pFocusedDwTable->GetInterface();
		else
			hdr.DwTable = 0;
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			if(pFocusedDwTable)
				((IDwTableObserver *)m_pObServers[i])->LoseFocus((IDwTable *)pFocusedDwTable->GetInterface());
			else
				((IDwTableObserver *)m_pObServers[i])->LoseFocus(NULL);
				
		}
		return 1;
	}
	int CDwTable::event_ItemChanged(int nRow,int nCol,LPTSTR pData,LPTSTR pDisplayData)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMCHANGED;
		hdr.DwTable = (IDwTable *)this->GetInterface();
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = getColumnName(nCol);
		hdr.data = pData;
		if(m_pGrid->GetHWND() && ::GetParent(m_pGrid->GetHWND()))
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		for(int i=0;i<(int)m_pObServers.size();i++)
			((IDwTableObserver *)m_pObServers[i])->ItemChanged((IDwTable *)GetInterface(),nRow,nCol,pData);


		wstring path = this->m_strBoundGroup;
		CDwTable * pDwParent = this->m_pParent;
		while(pDwParent)
		{
			path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
			pDwParent = pDwParent->m_pParent;
		}
		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData())
			m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path.c_str(),nRow,hdr.colname,hdr.data);

		if(m_pGrid->m_pSheetSvr && m_pGrid->m_pSheetSvr->m_pSheetCtrl->hasBindData() && pDisplayData)
		{
			wstring colname = (wstring)hdr.colname;
			colname += (wstring)_T("/@_displaystring");
			m_pGrid->m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path.c_str(),nRow,(wchar_t*)colname.c_str(),pDisplayData);
		}
		
		return 1;
	}
	int CDwTable::event_ItemChanging(int nRow,int nCol,LPTSTR pData)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMCHANGING;
		hdr.DwTable = (IDwTable *)this->GetInterface();
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = getColumnName(nCol);
		hdr.data = pData;
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IDwTableObserver *)m_pObServers[i])->ItemChanging((IDwTable *)GetInterface(),nRow,nCol,pData);
		return hdr.idFrom;
	}
	int CDwTable::event_EditorLoseFocus()
	{
		if(!this->m_pGrid->m_pEditSvr->m_pEdit) return 1;
		LPTSTR pStr1 = this->m_pGrid->m_pEditSvr->m_pEdit->GetText();
		int nRow, nCol;
		getRowColumnFromCell(this->m_pGrid->FCurrent.x,this->m_pGrid->FCurrent.y,nRow,nCol);
		if(nRow > GetRowCount() || nCol > getColumnCount()) return 1;

		LPTSTR pStr2 = (LPTSTR)this->getDwItem(nRow, nCol);
		if(pStr1 && pStr2 && _tcscmp(pStr1, pStr2) == 0) return 1;

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom =m_pGrid->GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_EDITORLOSEFOCUS;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)getColumnName(nCol);
		hdr.data = pStr1;
		::SendMessage(::GetParent(m_pGrid->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		if(hdr.idFrom < 0)
			::SetFocus(this->m_pGrid->m_pEditSvr->m_pEdit->GetHWND());
		return hdr.idFrom;
	}
	int CDwTable::GetTemplateColumnCell(int nCol,int &tCol,int &tRow)
	{
		if(nCol < 1) return  -1;
		if(nCol > (int)m_dbcolsPos.size()) return -1;
		tRow = m_dbcolsPos[nCol - 1].y + this->getGroupStartCellRow(1);
		tCol = m_dbcolsPos[nCol - 1].x;
		return 1;
	}
	int CDwTable::GetTemplateColumnCell(LPTSTR pColName,int &tCol,int &tRow)
	{
		int nCol = this->getColumnIndex(pColName);
		return GetTemplateColumnCell(nCol,tCol,tRow);
	}
	
	LPTSTR CDwTable::GetColumnProp(LPCTSTR pColName,LPCTSTR pAttribute)
	{
		int nCol;
		nCol = this->getColumnIndex((LPTSTR)pColName);
		if(nCol < 1) return NULL;
		return  GetColumnProp(nCol,pAttribute);
	}

	LPTSTR CDwTable::GetColumnProp(int nCol,LPCTSTR pAttribute)
	{
		int ACol,ARow;
		if(GetTemplateColumnCell(nCol,ACol,ARow)<0) return NULL;

		//PStrItem pStrItem = m_pTemplGridData->m_pContentSvr->GetItemData(ACol,ARow);
		PStrItem pStrItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
	
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				LPTSTR pItemStr = NULL;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute((BSTR)pAttribute,&var);
					pElement->Release();
					if(var.bstrVal) //return _tcsdup(var.bstrVal);
					{
						pItemStr = _tcsdup(var.bstrVal);
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this->m_pGrid, 1, ARow, ACol))
								pItemStr = x.GetString();							
						}
					}
				}
				pNode->Release();
				return _tcsdup(pItemStr);
			}
		}
		return NULL;
	}
	
	LPTSTR CDwTable::GetColumnProps(LPCTSTR pcolName)
	{
		int nCol =  this->getColumnIndex((LPTSTR)pcolName);
		if(nCol < 1) return NULL;
		return  GetColumnProps(nCol);
	}

	LPTSTR CDwTable::GetColumnProps(int nCol)
	{
		int ACol,ARow;
		if(GetTemplateColumnCell(nCol,ACol,ARow)<0) return NULL;

		PStrItem pStrItem = m_pTemplGridData->m_pContentSvr->GetItemData(ACol,ARow);
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				BSTR bstr;
				LPTSTR pStr = NULL;
				pNode->get_xml(&bstr);
				if(bstr)
				{
					pStr = _tcsdup(bstr);
					::SysFreeString(bstr);
				}
				pNode->Release();
				if(pStr) return pStr;
			}
		}
		return NULL;
	}

	int CDwTable::SetColumnProp(int nCol,LPCTSTR pAttribute,LPCTSTR pProp)
	{
		int ACol,ARow;
		if(GetTemplateColumnCell(nCol,ACol,ARow)<0) return NULL;

		//PStrItem pStrItem = m_pTemplGridData->m_pContentSvr->GetItemData(ACol,ARow);
		PStrItem pStrItem = m_pGrid->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pStrItem) pStrItem = new StrItem;
		if(!pStrItem->pStrXml)
		{
			pStrItem->pStrXml = _tcsdup(L"<cell><cellprop/></cell>");
			//m_pTemplGridData->m_pContentSvr->SetItemData(ACol,ARow,pStrItem);
		}
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);
			if(!pNode)
			{
				xml.m_pXmlDoc->selectSingleNode(L"cell",&pNode);
				if(!pNode)
				{
					IXMLDOMElement * pRoot;
					xml.m_pXmlDoc->createElement(L"cell",&pRoot);
					xml.m_pXmlDoc->appendChild(pRoot,NULL);
					xml.m_pXmlDoc->createElement(L"cellprop",&pElement);
					pRoot->appendChild(pElement,NULL);
					pElement->Release();
					pRoot->Release();
				}else
				{
					xml.m_pXmlDoc->createElement(L"cellprop",&pElement);
					pNode->appendChild(pElement,NULL);
					pElement->Release();
					pNode->Release();
				}
				xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);
			}
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					var = pProp;
					pElement->setAttribute((BSTR)pAttribute,var);
					pElement->Release();
					//return 1;
				}
				pNode->Release();
			}
			pStrItem->pStrXml = _tcsdup(xml.GetXml());
		}
		return -1;
	}

	int CDwTable::SetColumnProp(LPCTSTR pcolName,LPCTSTR pAttribute,LPCTSTR pProp)
	{
		int nCol;
		nCol = this->getColumnIndex((LPTSTR)pcolName);
		if(nCol < 1) return NULL;
		return  SetColumnProp(nCol,pAttribute,pProp);
	}

	int CDwTable::SetColumnProps(LPCTSTR pcolName,LPCTSTR pProps)
	{
		int nCol;
		nCol = this->getColumnIndex((LPTSTR)pcolName);
		if(nCol < 1) return NULL;
		return  SetColumnProps(nCol,pProps);
	}

	int CDwTable::SetColumnProps(int nCol,LPCTSTR pProps)
	{
		return -1;
	}
	int CDwTable::AcceptText()
	{
		this->m_pGrid->m_pEditSvr->HiddenEditor();
		return 0;
	}

	int CDwTable::getNextAllowEditColumn(int nRow,int nCol)
	{
		int i = 0;
		for(i=nCol + 1;i<=this->getColumnCount();i++)
		{
			int cRow,cCol;
			getCellFromRowColumn(nRow,i,cCol,cRow);

			bool bProtect = false;
			bool allowEdit = (m_pGrid->FOptions & goAllowEdit)?true:false;
			bool bVisible = true;
			LPTSTR protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellprotect"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					bProtect = true;
				}
				delete protect;
			}
			protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellvisible"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("不可见"))==0 )bVisible = false;
				delete protect;
			}
			if(!bVisible)continue;
			if(bProtect) continue;
			break;
		}
		if(i<=getColumnCount()) return i;
		return -1;
	}
	int CDwTable::getPirorAllowEditColumn(int nRow,int nCol)
	{
		int i = 0;
		for(i=nCol - 1;i>=1;i--)
		{
			int cRow,cCol;
			getCellFromRowColumn(nRow,i,cCol,cRow);

			bool bProtect = false;
			bool allowEdit = (m_pGrid->FOptions & goAllowEdit)?true:false;
			bool bVisible = true;
			LPTSTR protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellprotect"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					bProtect = true;
				}
				delete protect;
			}
			protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellvisible"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("不可见"))==0 )bVisible = false;
				delete protect;
			}
			if(!bVisible)continue;
			if(bProtect) continue;
			break;
		}
		if(i>0) return i;
		return -1;
	}

	int CDwTable::getDownAllowEditColumn(int nRow,int nCol)
	{
		int i = 0;
		int cRow0,cCol0;
		int sRow0,sCol0;
		this->getCellFromRowColumn(nRow,nCol,cCol0,cRow0);
		TGridRect	rect;
		m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(cCol0,cRow0,rect);
		sCol0 = rect.right;
		sRow0 = rect.bottom;

		for(i=nCol + 1;i<=this->getColumnCount();i++)
		{
			int cRow,cCol;
			getCellFromRowColumn(nRow,i,cCol,cRow);

			bool bProtect = false;
			bool allowEdit = (m_pGrid->FOptions & goAllowEdit)?true:false;
			bool bVisible = true;
			LPTSTR protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellprotect"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					bProtect = true;
				}
				delete protect;
			}
			protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellvisible"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("不可见"))==0 )bVisible = false;
				delete protect;
			}
			if(!bVisible)continue;
			if(bProtect) continue;

			int sRow,sCol;
			m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(cCol,cRow,rect);
			sCol = rect.right;
			sRow = rect.bottom;
			if(cCol == cCol0 && sRow == cRow0 +1) return i;
			if(cCol <= cCol0 && sCol>=cCol0 && sRow == cRow0 +1) return i;
			if(cCol0 <= cCol && sCol0>=cCol && sRow == cRow0 +1) return i;
		}
		return -1;
	}
	int CDwTable::getUpAllowEditColumn(int nRow,int nCol)
	{
		int i = 0;
		int cRow0,cCol0;
		int sRow0,sCol0;
		this->getCellFromRowColumn(nRow,nCol,cCol0,cRow0);
		TGridRect	rect;
		m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(cCol0,cRow0,rect);
		sCol0 = rect.right;
		sRow0 = rect.bottom;

		for(i=nCol - 1;i>=1;i--)
		{
			int cRow,cCol;
			getCellFromRowColumn(nRow,i,cCol,cRow);

			bool bProtect = false;
			bool allowEdit = (m_pGrid->FOptions & goAllowEdit)?true:false;
			bool bVisible = true;
			LPTSTR protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellprotect"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					bProtect = true;
				}
				delete protect;
			}
			protect = m_pGrid->GetItemProp(cCol,cRow,_T("cellvisible"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("不可见"))==0 )bVisible = false;
				delete protect;
			}
			if(!bVisible)continue;
			if(bProtect) continue;

			int sRow,sCol;
			m_pGrid->GetGridDataSvr()->m_pSpanSvr->isSpaned(cCol,cRow,rect);
			sCol = rect.right;
			sRow = rect.bottom;
			if(cCol == cCol0 && sRow == cRow0 -1) return i;
			if(cCol <= cCol0 && sCol>=cCol0 && sRow == cRow0 -1) return i;
			if(cCol0 <= cCol && sCol0>=cCol && sRow == cRow0 -1) return i;
		}
		return -1;
	}
	int CDwTable::getNullCell(POINT &p)
	{
		//POINT p;
		int aRow = 1;
		int aCol = 1;
		
		PStrItem pItem = this->getDwItem(aRow, aCol);
		//while(aRow <= m_pGrid->GetVisibleRowCount())
		while(aRow <= GetRowCount())
		{
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMNode * pNode;
				IXMLDOMElement * pElement;
				_variant_t var;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pElement->getAttribute(L"cellneeddata",&var);
					if(var.bstrVal && _tcscmp(var.bstrVal,_T("是"))==0)
					{
						if(!pItem->pStrContent || _tcscmp(_tcstrim(pItem->pStrContent), L"") == 0)
						{
							p.x = aRow;
							p.y = aCol;
							return (int)this->GetInterface();
						}
					}
				}
			}
			//if(aCol <= m_pGrid->GetVisibleColCount()) 
			if(aCol <= (int)this->m_dbcols.size()) 
				aCol++;
			else
			{
				aRow++;
				aCol = 1;
			}
			if(pItem) pItem = NULL;
			pItem = this->getDwItem(aRow, aCol);
		}
		int i;
		for(i=0; i<(int)this->m_pChilds.size();i++)
		{
			int ret = this->m_pChilds[i]->getNullCell(p);
			if(ret>0)
				return ret;
		}
		return -1;
	}
	LPTSTR CDwTable::_tcstrim(const LPTSTR lpszStr)
	{
		//assert(lpszStr != NULL);

		// trim right
		LPTSTR lpStrEnd;
		lpStrEnd = lpszStr + _tcslen( lpszStr );
		while (lpStrEnd > lpszStr) {
			lpStrEnd--;
			if ( (*lpStrEnd) == _T(' ') )
				(*lpStrEnd) = 0;
			else
				break;
		}
		lpStrEnd = lpszStr + _tcslen( lpszStr );

		// trim left
		LPTSTR lpszDest = lpszStr;
		LPTSTR lpszSrc = lpszStr;
		while (lpszSrc < lpStrEnd && (*lpszSrc) == _T(' ')) {
			lpszSrc++;
		}
		if (lpszDest != lpszSrc) {
			while (lpszSrc <= lpStrEnd) {
				(*lpszDest) = (*lpszSrc);
				lpszDest++;
				lpszSrc++;
			}
		}

		return lpszStr;
	}
	int CDwTable::ExpandRow()
	{
		return ExpandMarginRow(0, 0);
	}
	int CDwTable::ExpandMarginRow(int sRow, int eRow)
	{
		int startRow;
		int endRow;
		this->GetTableCellArea(startRow, endRow);
		return ExpandCellRow(startRow + sRow, endRow + eRow);
	}
	int CDwTable::ExpandRow(int sRow, int eRow)
	{
		int startRow;
		int endRow;
		this->GetTableCellArea(startRow, endRow);
		sRow += startRow - 1;
		eRow += startRow - 1;
		startRow = startRow>sRow?startRow:sRow;
		endRow = endRow>eRow?eRow:endRow;
		return ExpandCellRow(startRow, endRow);
	}
	int CDwTable::ExpandCellRow(int sRow, int eRow)
	{
		sRow = sRow<1?1:sRow;
		int i;
		for(i=sRow; i<=eRow; i++)
		{
			int height = m_pGrid->GetRowHeight(i);
			m_pGrid->GetGridDataSvr()->m_pAxisSvr->SetRowHeight(i, height*-1);
		}
		return 1;
	}

}}}}

