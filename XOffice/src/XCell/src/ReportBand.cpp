#include "stdafx.h"
#include "xofbase.h"
#include "reportband.hpp"
#include "axissvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CReportBandSvr::CReportBandSvr(void)
	{

	}
	CReportBandSvr::~CReportBandSvr(void)
	{
	}
	
	bool CReportBandSvr::GetReportBand(int nRow,TReportBandItem* & pBandItem, int & nGroup)
	{
		if(!m_ReportBand.bValid) return false;
		nGroup = -1;
		pBandItem = NULL;

		if(m_ReportBand.PageHeader.bValid && m_ReportBand.PageHeader.startRow <= nRow && m_ReportBand.PageHeader.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.PageHeader;
			return true;
		}
		if(m_ReportBand.PageFooter.bValid && m_ReportBand.PageFooter.startRow <= nRow && m_ReportBand.PageFooter.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.PageFooter;
			return true;
		}
		if(m_ReportBand.Header.bValid && m_ReportBand.Header.startRow <= nRow && m_ReportBand.Header.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.Header;
			return true;
		}
		if(m_ReportBand.Title.bValid && m_ReportBand.Title.startRow <= nRow && m_ReportBand.Title.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.Title;
			return true;
		}
		if(m_ReportBand.Detail.bValid && m_ReportBand.Detail.startRow <= nRow && m_ReportBand.Detail.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.Detail;
			return true;
		}
		if(m_ReportBand.Summury.bValid && m_ReportBand.Summury.startRow <= nRow && m_ReportBand.Summury.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.Summury;
			return true;
		}
		if(m_ReportBand.Footer.bValid && m_ReportBand.Footer.startRow <= nRow && m_ReportBand.Footer.endRow >= nRow)
		{
			pBandItem = &m_ReportBand.Footer;
			return true;
		}

		for(int i=0;i<(int)m_ReportBand.m_pGroups->size();i++)
		{
			if((*m_ReportBand.m_pGroups)[i].Header.bValid && (*m_ReportBand.m_pGroups)[i].Header.startRow <= nRow && (*m_ReportBand.m_pGroups)[i].Header.endRow >= nRow)
			{
				pBandItem = &(*m_ReportBand.m_pGroups)[i].Header ;
				return true;
			}
			if((*m_ReportBand.m_pGroups)[i].Title.bValid && (*m_ReportBand.m_pGroups)[i].Title.startRow <= nRow && (*m_ReportBand.m_pGroups)[i].Title.endRow >= nRow)
			{
				pBandItem = &(*m_ReportBand.m_pGroups)[i].Title;
				return true;
			}
			if((*m_ReportBand.m_pGroups)[i].Detail.bValid && (*m_ReportBand.m_pGroups)[i].Detail.startRow <= nRow && (*m_ReportBand.m_pGroups)[i].Detail.endRow >= nRow)
			{
				pBandItem = &(*m_ReportBand.m_pGroups)[i].Detail;
				return true;
			}
			if((*m_ReportBand.m_pGroups)[i].Summury.bValid && (*m_ReportBand.m_pGroups)[i].Summury.startRow <= nRow && (*m_ReportBand.m_pGroups)[i].Summury.endRow >= nRow)
			{
				pBandItem = &(*m_ReportBand.m_pGroups)[i].Summury;
				return true;
			}
			if((*m_ReportBand.m_pGroups)[i].Footer.bValid && (*m_ReportBand.m_pGroups)[i].Footer.startRow <= nRow && (*m_ReportBand.m_pGroups)[i].Footer.endRow >= nRow)
			{
				pBandItem = &(*m_ReportBand.m_pGroups)[i].Footer;
				return true;
			}
		}
		return false;
	}

	LPTSTR CReportBandSvr::GetReportBandTitle(TReportBandItem* pBandItem) 
	{
		if(!m_ReportBand.bValid) return NULL;

		if(&m_ReportBand.PageHeader == pBandItem) return _tcsdup(L"页头");
		if(&m_ReportBand.PageFooter == pBandItem) return _tcsdup(L"页尾");
		if(&m_ReportBand.Header == pBandItem) return _tcsdup(L"头项");
		if(&m_ReportBand.Title == pBandItem) return _tcsdup(L"标题");
		if(&m_ReportBand.Detail == pBandItem) return _tcsdup(L"明细");
		if(&m_ReportBand.Summury == pBandItem) return _tcsdup(L"汇总");
		if(&m_ReportBand.Footer == pBandItem) return _tcsdup(L"尾项");

		for(int i=0;i<(int)(*m_ReportBand.m_pGroups).size();i++)
		{
			TCHAR buf[225];
			if(&(*m_ReportBand.m_pGroups)[i].Header == pBandItem)
			{
				_stprintf_s(buf,255,_T("组(%s).%i.%s"),(*m_ReportBand.m_pGroups)[i].StrName,i, L"头项");
			}
			if(&(*m_ReportBand.m_pGroups)[i].Title == pBandItem)
			{
				_stprintf_s(buf,255,_T("组(%s).%i.%s"),(*m_ReportBand.m_pGroups)[i].StrName,i, L"标题");
			}
			if(&(*m_ReportBand.m_pGroups)[i].Detail == pBandItem)
			{
				_stprintf_s(buf,255,_T("组(%s).%i.%s"),(*m_ReportBand.m_pGroups)[i].StrName,i, L"明细");
			}
			if(&(*m_ReportBand.m_pGroups)[i].Summury == pBandItem)
			{
				_stprintf_s(buf,255,_T("组(%s).%i.%s"),(*m_ReportBand.m_pGroups)[i].StrName,i, L"汇总");
			}
			if(&(*m_ReportBand.m_pGroups)[i].Footer == pBandItem)
			{
				_stprintf_s(buf,255,_T("组(%s).%i.%s"),(*m_ReportBand.m_pGroups)[i].StrName,i, L"尾项");
			}
		}
		return NULL;
	}
	bool CReportBandSvr::InsertRow(int ARow)
	{
		TReportBandItem* pBandItem;
		int nGroup;

		std::vector<TReportBandItem *> Items;
		if(m_ReportBand.PageHeader.bValid)Items.push_back(&m_ReportBand.PageHeader);
		if(m_ReportBand.Header.bValid)Items.push_back(&m_ReportBand.Header);
		if(m_ReportBand.Title.bValid)Items.push_back(&m_ReportBand.Title);
		if(m_ReportBand.Detail.bValid)Items.push_back(&m_ReportBand.Detail);
		if(m_ReportBand.Summury.bValid)Items.push_back(&m_ReportBand.Summury);
		if(m_ReportBand.Footer.bValid)Items.push_back(&m_ReportBand.Footer);
		if(m_ReportBand.PageFooter.bValid)Items.push_back(&m_ReportBand.PageFooter);

		if(GetReportBand(ARow,pBandItem,nGroup))
		{
			int i;
			for(i=0;i<(int)Items.size();i++)
			{
				if(pBandItem==Items[i]) break;
			}
			if(i<(int)Items.size()) Items[i]->endRow++;
			for(i=i+1;i<(int)Items.size();i++)
			{
				Items[i]->startRow++;
				Items[i]->endRow++;
			}

		}
		return true;
	}
	bool CReportBandSvr::DeleteRow(int ARow)
	{
		TReportBandItem* pBandItem;
		int nGroup;

		std::vector<TReportBandItem *> Items;
		if(m_ReportBand.PageHeader.bValid)Items.push_back(&m_ReportBand.PageHeader);
		if(m_ReportBand.Header.bValid)Items.push_back(&m_ReportBand.Header);
		if(m_ReportBand.Title.bValid)Items.push_back(&m_ReportBand.Title);
		if(m_ReportBand.Detail.bValid)Items.push_back(&m_ReportBand.Detail);
		if(m_ReportBand.Summury.bValid)Items.push_back(&m_ReportBand.Summury);
		if(m_ReportBand.Footer.bValid)Items.push_back(&m_ReportBand.Footer);
		if(m_ReportBand.PageFooter.bValid)Items.push_back(&m_ReportBand.PageFooter);

		if(GetReportBand(ARow,pBandItem,nGroup))
		{
			int i;
			for(i=0;i<(int)Items.size();i++)
			{
				if(pBandItem==Items[i]) break;
			}
			if(i<(int)Items.size() && Items[i]->endRow == ARow) return false;
			if(i<(int)Items.size()) Items[i]->endRow--;
			for(i=i+1;i<(int)Items.size();i++)
			{
				Items[i]->startRow--;
				Items[i]->endRow--;
			}

		}

		return true;
	}

	int CReportBandSvr::GetTitleHeight(CAxisSvr * pAxisSvr)
	{
		if(!m_ReportBand.Title.bValid) return 0;
		
		int height = 0;
		int startRow,endRow;
		startRow = m_ReportBand.Title.startRow;
		endRow = m_ReportBand.Title.endRow - 1;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			height += pAxisSvr->GetRowHeight(ARow) + 1;
		}
		return height + 1;

	}
	int CReportBandSvr::GetDetailHeight(CAxisSvr * pAxisSvr)
	{
		if(!m_ReportBand.Detail.bValid) return 0;
		
		int height = 0;
		int startRow,endRow;
		startRow = m_ReportBand.Detail.startRow;
		endRow = m_ReportBand.Detail.endRow - 1;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			height += pAxisSvr->GetRowHeight(ARow) + 1;
		}
		return height + 1;
	}

}}}}