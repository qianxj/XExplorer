#include "StdAfx.h"
#include "..\include\XCellSheet.hpp"
#include "XCellSheetSvr.hpp"
#include "GridDataSvr.hpp"
#include "dwtable.hpp"
#include "idwtable.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XCellSheet::XCellSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XCellSheetSvr;
		m_pSheetSvr->m_pSheetCtrl = this;
		m_pSheetSvr->AddRouterMessageFrom(this);

		m_pIXCell = NULL;

		/*
		m_pVScrollBar = new CXScrollBar(SB_VERT);
		m_pVScrollBar->m_pOwner = m_pSheetSvr;
		m_pHScrollBar = new CXScrollBar(SB_HORZ);
		m_pHScrollBar->m_pOwner = m_pSheetSvr;


		m_pVScrollBar->AddRouterMessageFrom((xfAreaObject *)this);
		m_pHScrollBar->AddRouterMessageFrom((xfAreaObject *)this);
		*/
	}


	XCellSheet::~XCellSheet(void)
	{
		if(m_Standalone)
		{
			m_pVScrollBar->RemoveRouterMessageFrom((xfAreaObject *)this);
			m_pHScrollBar->RemoveRouterMessageFrom((xfAreaObject *)this);
		}

		if(m_pSheetSvr) delete m_pSheetSvr;

		if(m_Standalone)
		{
			if(m_pVScrollBar)delete m_pVScrollBar;
			if(m_pHScrollBar)delete m_pHScrollBar;
		}
		if(m_pIXCell)delete m_pIXCell;
	}
	int XCellSheet::FocusedSheet()
	{
		
		xfNode* pNode = this->m_pxfNode->GetXPage()->GetAgentNode();
		if(pNode)
		{
			if(pNode->GetXPage()->m_pFocusNodeControl != dynamic_cast<xfNodeControl *>(this->m_pxfNode) ||
				!pNode->GetNodeByName(_T("xcellagent"),false))
			{
				XOfficeSheet::FocusedSheet();
			}
			((XCellSheetSvr *)m_pSheetSvr)->initialAgent();
		}
		return 1;
	}

	xbObject * XCellSheet::GetInterface()
	{
		return (xbObject *)GetDwInterFace();
	}

	void * XCellSheet::GetDwInterFace()
	{
		CGridDataSvr * pDataSvr = (CGridDataSvr *)((XCellSheetSvr *)m_pSheetSvr)->m_pDataSvr;
		if(!pDataSvr || pDataSvr->FBandDwTables.size() < 1)
		{
			IDwTable * pDwTable =  new IDwTable();
			pDwTable->m_pSheet  = this;
			pDwTable->m_pControl = this;

			return pDwTable;
		}
		CDwTable * pTable = pDataSvr->FBandDwTables[0];

		if(pTable->m_pInterFace) return pTable->m_pInterFace;
		pTable->m_pInterFace = new IDwTable();
		((IDwTable *)pTable->m_pInterFace)->m_pDwTable = pTable;
		((IDwTable *)pTable->m_pInterFace)->m_pControl = this;
		((IDwTable *)pTable->m_pInterFace)->m_pSheet = this;

		return pTable->GetInterface();
	}

	xbObject * XCellSheet::QueryItem(LPTSTR pItem)
	{
		if(_tcsicmp(pItem,L"ixcell")==0)
		{
			if(!m_pIXCell)
			{
				m_pIXCell = new IXCell;
				m_pIXCell->m_pControl = this;
			}
			return m_pIXCell;
		}
		return NULL;
	}

	 SIZE XCellSheet::GetContentExtent()
	 {
		 SIZE sz;
		 sz.cx = this->m_pSheetSvr->FRect.right -  this->m_pSheetSvr->FRect.left;
		 XCellSheetSvr * pSheetSvr = (XCellSheetSvr*)m_pSheetSvr;
		 CGridDataSvr* pDataSvr = pSheetSvr->m_pGridFrameSvrs->GetGridDataSvr();
		 int nRow = 1;		
		 nRow = pDataSvr->m_pContentSvr->GetValidRow();
		 sz.cy = pDataSvr->m_pAxisSvr->GetSumRowHeight(1,nRow);
		 
		 return sz;
	 }
}}}}
