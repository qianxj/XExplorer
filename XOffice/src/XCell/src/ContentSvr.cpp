#include "stdafx.h"
#include "xofbase.h"
#include "contentsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CContentSvr::CContentSvr(void)
	{
		m_pData = new XUtil::xuSpList<XUtil::xuSpList<void *>*>();
	}
	CContentSvr::~CContentSvr(void)
	{
		for(int row = 0;row <= m_pData->GetHighBound();row++)
		{
			if((*m_pData)[row])
			{
				for(int col=0;col <= (*m_pData)[row]->GetHighBound();col++)
				{
					if((*(*m_pData)[row])[col])delete (*(*m_pData)[row])[col];
				}
				delete (*m_pData)[row];
			}
		}
		if(m_pData)delete m_pData;
	}

	PStrItem CContentSvr::GetSysItemData(eSysColumn ASysCol,int ARow)
	{
		if(m_pData->Get(ARow))
			return (LPContentItem)m_pData->Get(ARow)->Get(ASysCol);
		else
			return NULL;
	}
	int CContentSvr::SetSysItemData(eSysColumn ASysCol,int ARow, LPContentItem pData)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);
		(*(*m_pData)[ARow])[ASysCol] = pData;
		
		return 1;
	}

	PStrItem CContentSvr::GetItemData(int ACol,int ARow)
	{
		if(!m_pData) return NULL;
		if(ARow < 0) return NULL;
		if(m_pData->Get(ARow))
			return (LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count];
		else
			return NULL;
	}
	int  CContentSvr::SetItemData(int ACol,int ARow,LPContentItem pData)
	{
		if(!m_pData->Get(ARow))
		{
			if(pData ==NULL) return 1;
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);
		}
		if((*(*m_pData)[ARow])[ACol + CContentSvr::count])
		{
			PStrItem pItem = (LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count];
			delete (*(*m_pData)[ARow])[ACol + CContentSvr::count];
		}
		(*(*m_pData)[ARow])[ACol + CContentSvr::count] = pData;
		return 1;
	}
	TCHAR * CContentSvr::GetItemString(int ACol,int ARow)
	{
		if(m_pData->Get(ARow) && ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count]))
			return ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent ;
		else
			return NULL;
	}
	int CContentSvr::SetItemString(int ACol,int ARow, TCHAR * pData)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);

		if((*(*m_pData)[ARow])[ACol + CContentSvr::count])
		{
			if(((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent &&
					((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent == pData) return 1;

			if(((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent )
				delete ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent;
		}else
		{
			LPContentItem pItem = new TContentItem();
			(*(*m_pData)[ARow])[ACol + CContentSvr::count] = pItem;
		}
		((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent = _tcsdup(pData);
		return 1;
	}
	TCHAR * CContentSvr::GetItemText(int ACol,int ARow)
	{
		if(m_pData->Get(ARow) && ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count]))
			return ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrText ;
		else
			return NULL;
	}
	int CContentSvr::SetItemText(int ACol,int ARow, TCHAR * pData)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);

		if((*(*m_pData)[ARow])[ACol + CContentSvr::count])
		{
			if(((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrText &&
					((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrContent == pData) return 1;

			if(((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrText)
				delete ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrText;
		}else
		{
			LPContentItem pItem = new TContentItem();
			(*(*m_pData)[ARow])[ACol + CContentSvr::count] = pItem;
		}
		((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->pStrText = _tcsdup(pData);
		return 1;
	}
	int CContentSvr::CopyTo(int FromRow,int ToRow)
	{
		if(!m_pData->Get(FromRow))
		{
			if(!m_pData->Get(ToRow))
			{
				delete m_pData->Get(ToRow);
				m_pData->Put(ToRow,NULL);
			}
		}
		if(!m_pData->Get(FromRow))
			m_pData->Put(ToRow,new XUtil::xuSpList<void *>);
		
		XUtil::xuSpList<void *> * pFrom = m_pData->Get(FromRow);
		XUtil::xuSpList<void *> * pTo = m_pData->Get(FromRow);

		int vCol = GetValidCol(FromRow);
		for(int i=1;i<vCol;i++)
		{
			if((*(*m_pData)[FromRow])[i + CContentSvr::count])
			{
				LPContentItem pItem = new TContentItem;
				(*(*m_pData)[ToRow])[i + CContentSvr::count] = pItem;
				pItem->CopyFrom((LPContentItem)(*(*m_pData)[FromRow])[i + CContentSvr::count]);
			}else
			{
				if((*(*m_pData)[ToRow])[i + CContentSvr::count])
					delete (*(*m_pData)[ToRow])[i + CContentSvr::count];
			}
		}
		return 1;
	}

	CContentSvr::eDataType CContentSvr::GetItemType(int ACol,int ARow)
	{
		if(m_pData->Get(ARow))
			return ((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->eDTItem;
		else
			return CContentSvr::none;
	}

	int CContentSvr::SetItemType(int ACol,int ARow, CContentSvr::eDataType dt)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);
		if((*(*m_pData)[ARow])[ACol + CContentSvr::count])
			(*(*m_pData)[ARow])[ACol + CContentSvr::count] = new TContentItem;
		((LPContentItem)(*(*m_pData)[ARow])[ACol + CContentSvr::count])->eDTItem = dt;
		
		return 1;
	}
	int CContentSvr::Reset()
	{
		for(int row = 0;row <= m_pData->GetHighBound();row++)
		{
			if((*m_pData)[row])
			{
				for(int col=0;col <= (*m_pData)[row]->GetHighBound();col++)
				{
					if((*(*m_pData)[row])[col])delete (*(*m_pData)[row])[col];
				}
				delete (*m_pData)[row];
			}
		}
		if(m_pData)delete m_pData;
		m_pData = new XUtil::xuSpList<XUtil::xuSpList<void *>*>();

		return 1;
	}

	int CContentSvr::GetValidRow()
	{
		return m_pData->GetHighBound();
	}

	int CContentSvr::GetValidCol(int ARow)
	{	
		if(ARow > GetValidRow()) return -1;
		if(!m_pData->Get(ARow)) return -1;
		return m_pData->Get(ARow)->GetHighBound() - CContentSvr::count;

	}
	int CContentSvr::GetValidCol()
	{
		int count = GetValidRow();
		int Result = -1;
		int VCol;
		for(int i=1;i<=count;i++)
		{
			if(!m_pData->Get(i))continue;
			VCol = m_pData->Get(i)->GetHighBound() - CContentSvr::count;
			if(VCol > Result) Result = VCol;
		}
		return Result;
	}
	
	int CContentSvr::MoveRow(int FromRow,int ToRow)
	{
		m_pData->Move(FromRow, ToRow);
		return 1;
	}

	int CContentSvr::MoveCol(int FromIndex,int ToIndex)
	{
		int nRows = GetValidRow();
		for(int i=0;i<=nRows;i++)
		{
			if((*m_pData)[i])
				(*m_pData)[i]->Move(CContentSvr::count + FromIndex,CContentSvr::count + ToIndex);
		}
		return 1;
	}

	int CContentSvr::GetValidRowCol(int &ACol,int &ARow)
	{
		ARow = this->GetValidRow();
		ACol = this->GetValidCol();
		return 1;
	}

	void CContentSvr::ClearRowData(int ARow)
	{
		if(m_pData)
		{
			if(ARow >m_pData->GetHighBound()) return ;
			if(m_pData->Get(ARow))
			{
				delete m_pData->Get(ARow);
			}
			m_pData->Put(ARow,NULL);
		}
	}

	int CContentSvr::MoveRow(int FromRow,int ToRow,int Number)
	{
		if(FromRow < ToRow)
		{
			for(int i=1;i<=Number;i++)
			{
				m_pData->Move(FromRow , ToRow);
			}
		}else
		{
			for(int i=1;i<=Number;i++)
			{
				m_pData->Move(FromRow , ToRow + i -1);
			}
		}
		return 1;
	}

	int CContentSvr::SetItemProp(int ACol,int ARow, LPCTSTR pAttribute, LPCTSTR pProp)
	{
		PStrItem pStrItem = GetItemData(ACol,ARow);
		if(!pStrItem) pStrItem = new StrItem;
		if(!pStrItem->pStrXml)
		{
			pStrItem->pStrXml = _tcsdup(L"<cell><cellprop/></cell>");
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
		return 1;	
	}
}}}}
