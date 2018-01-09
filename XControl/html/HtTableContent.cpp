#include "HtTableContent.hpp"
#include "tchar.h"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	CHtTableContent::CHtTableContent(void):FSysCount(4), FRowCount(0),FColCount(0)
	{
		m_pData = new XUtil::xuSpList<XUtil::xuSpList<void *>*>();
	}

	CHtTableContent::~CHtTableContent(void)
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

	LPIHtContentItem CHtTableContent::GetItemData(int ACol,int ARow)
	{
		if(m_pData->Get(ARow))
			return (LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount];
		else
			return NULL;
	}

	int  CHtTableContent::SetItemData(int ACol,int ARow,LPIHtContentItem pData)
	{
		if(!m_pData->Get(ARow))
		{
			if(pData ==NULL) return 1;
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);
		}
		if((*(*m_pData)[ARow])[ACol + FSysCount])
		{
			LPIHtContentItem pItem = (LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount];
			delete (*(*m_pData)[ARow])[ACol + FSysCount];
		}
		(*(*m_pData)[ARow])[ACol + FSysCount] = pData;
		return 1;
	}

	TCHAR * CHtTableContent::GetItemString(int ACol,int ARow)
	{
		if(m_pData->Get(ARow) && ((LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount]))
			return ((LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount])->text;
		else
			return NULL;
	}
	int CHtTableContent::SetItemString(int ACol,int ARow, TCHAR * pData)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);

		if((*(*m_pData)[ARow])[ACol + FSysCount])
		{
			if(((LPIHtContentItem)(*(*m_pData)[ARow])[ACol +FSysCount])->text)
				delete ((LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount])->text;
		}else
		{
			LPIHtContentItem pItem = new IHtContentItem();
			(*(*m_pData)[ARow])[ACol + FSysCount] = pItem;
		}
		((LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount])->text = _tcsdup(pData);
		return 1;
	}

	int CHtTableContent::CopyTo(int FromRow,int ToRow)
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
			if((*(*m_pData)[FromRow])[i + FSysCount])
			{
				LPIHtContentItem pItem = new IHtContentItem;
				(*(*m_pData)[ToRow])[i + FSysCount] = pItem;
				pItem->CopyFrom((LPIHtContentItem)(*(*m_pData)[FromRow])[i + FSysCount]);
			}else
			{
				if((*(*m_pData)[ToRow])[i + FSysCount])
					delete (*(*m_pData)[ToRow])[i + FSysCount];
			}
		}
		return 1;
	}

	int CHtTableContent::MoveRow(int FromRow,int ToRow)
	{
		m_pData->Move(FromRow, ToRow);
		return 1;
	}

	int CHtTableContent::MoveRow(int FromRow,int ToRow,int Number)
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

	int CHtTableContent::MoveCol(int FromIndex,int ToIndex)
	{
		int nRows = GetValidRow();
		for(int i=0;i<=nRows;i++)
		{
			if((*m_pData)[i])
				(*m_pData)[i]->Move( FSysCount + FromIndex, FSysCount + ToIndex);
		}
		return 1;
	}

	void CHtTableContent::ClearRowData(int ARow)
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

	IHtContentItem::IHtContentItemType CHtTableContent::GetItemType(int ACol,int ARow)
	{
		if(m_pData->Get(ARow))
			return ((LPIHtContentItem)(*(*m_pData)[ARow])[ACol + FSysCount])->et;
		else
			return IHtContentItem::eNone;
	}

	int CHtTableContent::SetItemType(int ACol,int ARow, IHtContentItem::IHtContentItemType dt)
	{
		if(!m_pData->Get(ARow))
			m_pData->Put(ARow,new XUtil::xuSpList<void *>);
		if((*(*m_pData)[ARow])[ACol +  FSysCount])
			(*(*m_pData)[ARow])[ACol +  FSysCount] = new IHtContentItem;
		((LPIHtContentItem)(*(*m_pData)[ARow])[ACol +  FSysCount])->et = dt;

		return 1;
	}

	int CHtTableContent::GetValidRow()
	{
		return m_pData->GetHighBound();
	}

	int CHtTableContent::GetValidCol(int ARow)
	{
		if(ARow > GetValidRow()) return -1;
		if(!m_pData->Get(ARow)) return -1;
		return m_pData->Get(ARow)->GetHighBound() - FSysCount;
	}

	int CHtTableContent::GetValidRowCol(int &ACol,int &ARow)
	{
		ARow = this->GetValidRow();
		ACol = this->GetValidCol();
		return 1;
	}

	int CHtTableContent::GetValidCol()
	{
		int count = GetValidRow();
		int Result = -1;
		int VCol;
		for(int i=1;i<=count;i++)
		{
			if(!m_pData->Get(i))continue;
			VCol = m_pData->Get(i)->GetHighBound() - FSysCount;
			if(VCol > Result) Result = VCol;
		}
		return Result;
	}
	//clear data and data type
	int CHtTableContent::Reset()
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

	int CHtTableContent::SetRowCount(int nRowCount)
	{
		return FRowCount = nRowCount;
	}

	int CHtTableContent::SetColCount(int nColCount)
	{
		return FColCount = nColCount;
	}

	int CHtTableContent::GetRowCount()
	{
		if(FRowCount< 1) 
			return GetValidRow();
		return FRowCount;
	}

	int CHtTableContent::GetColCount()
	{
		if(FColCount < 1) return GetValidCol();
		return FColCount;
	}
}}}
