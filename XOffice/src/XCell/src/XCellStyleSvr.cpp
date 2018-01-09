#include "StdAfx.h"
#include "XCellStyleSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XCellStyleSvr::XCellStyleSvr(void)
	{
		m_pCssClass = new XUtil::xuVector<LPColCssClass>;
		for (int i=0;i<255;i++)
		{
			LPColCssClass lp;
			lp= new XUtil::xuCsArray< XCROW, CssClassIndex>( MAXROW, -1);
			m_pCssClass->push_back(lp);
		}
	}

	XCellStyleSvr::~XCellStyleSvr(void)
	{
		if(m_pCssClass)delete m_pCssClass;
	}
	const XUtil::Css::TBoxStyle *	XCellStyleSvr::GetCellCssStyle(XCROW ACol,XCROW ARow)
	{
		if(ACol >=255) return NULL;
		if(ARow < 0 || ACol < 0 ) return NULL;

		int nIndex = (*m_pCssClass)[ACol]->GetValue(ARow);
		if(nIndex < 0) return NULL;

		return m_pCssStyle->GetBoxStyle(nIndex);
	}

	int XCellStyleSvr::SetCellCssStyle(XCROW ACol,XCROW ARow, XUtil::Css::TBoxStyle * pBoxStyle)
	{
		if(ACol >=255) return -1;
		if(ARow < 0 || ACol < 0 ) return -1;

		if(!pBoxStyle)
			(*m_pCssClass)[ACol]->SetValue(ARow,-1);
		else
		{
			int nIndex = m_pCssStyle->AddCssCellStyle(pBoxStyle);
			(*m_pCssClass)[ACol]->SetValue(ARow,nIndex);
		}

		return 1;
	}
	int XCellStyleSvr::SetCellCssStyle(TGridRect ARect, XUtil::Css::TBoxStyle * pBoxStyle)
	{
		if(ARect.left < 0) ARect.left = 0;
		if(ARect.top < 0) ARect.top = 0;
		
		int nIndex = -1;
		if(pBoxStyle) nIndex = m_pCssStyle->AddCssCellStyle(pBoxStyle);
		for(int i=ARect.left; i<ARect.right;i++)
		{
			(*m_pCssClass)[i]->SetValue(ARect.top,ARect.bottom, nIndex);
		}
		return 1;
	}
	int XCellStyleSvr::SetFont(TGridRect ARect, LOGFONT	&logFont)
	{
		if(ARect.left < 0) ARect.left = 0;
		if(ARect.top < 0) ARect.top = 0;
		
		int nIndex = -1;
		int nLast,nItem;
		int nRow;
		XUtil::Css::TBoxStyle * pBoxStyle;
		for(int i=ARect.left; i<ARect.right;i++)
		{
			nRow = ARect.top;
			while(nRow < ARect.bottom && nRow > 0)
			{
				nIndex = (*m_pCssClass)[i]-> GetValue(nRow,nItem,nLast);
				if(nIndex < 0)
					pBoxStyle = new XUtil::Css::TBoxStyle;
				else
					pBoxStyle =( XUtil::Css::TBoxStyle*) m_pCssStyle->GetBoxStyle(nIndex);
				XUtil::Css::SetFont(pBoxStyle,logFont);

				(*m_pCssClass)[i]->SetValue(nRow,nLast >ARect.bottom? nLast : ARect.bottom ,m_pCssStyle->AddCssCellStyle(pBoxStyle));
				if(nIndex < 0)
					delete pBoxStyle;
				if(nLast==MAXROW)break;
				nRow = nLast + 1;
			}
		}
		return 1;
	}
	int XCellStyleSvr::GetFont(XCROW ACol,XCROW ARow, LOGFONT	&logFont)
	{
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = this->GetCellCssStyle(ACol,ARow);
		if(!pBoxStyle) return 0;
		if(pBoxStyle->mask & cssMaskFont)
		{
			XUtil::Css::GetFont(pBoxStyle,logFont);
			return 1;
		}else
			return 0;
	}

	int XCellStyleSvr::SetFont(XCROW ACol,XCROW ARow, LOGFONT	&logFont)
	{
		bool	bClearStyle = false;
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle;
		if(GetCellCssStyle(ACol,ARow))
			*pBoxStyle = *GetCellCssStyle(ACol,ARow);

		XUtil::Css::SetFont(pBoxStyle,logFont);
		this->SetCellCssStyle(ACol,ARow,pBoxStyle);

		if(pBoxStyle) delete pBoxStyle;
		return 1;
	}

	int XCellStyleSvr::InsertRow(XCROW row, int nAmount)
	{
		for(int i=0;i<m_pCssClass->size();i++)
		{
			(*m_pCssClass)[i]->Insert(row,nAmount);
		}
		return 1;
	}

	int XCellStyleSvr::RemoveRow(XCROW row, int nAmount)
	{
		for(int i=0;i<m_pCssClass->size();i++)
		{
			(*m_pCssClass)[i]->Remove(row,nAmount);
		}
		return 1;
	}

	int XCellStyleSvr::InsertCol(XCCOL col, int nAmount)
	{
		for(int i=0;i<nAmount;i++)
		{
			LPColCssClass lp;
			lp= new XUtil::xuCsArray<XCROW, CssClassIndex>( MAXROW, -1);
			if(col > 1)
				lp->CopyFrom(*(*m_pCssClass)[col],1,MAXROW);
			m_pCssClass->insert(col,lp);
		}
		return 1;
	}

	int XCellStyleSvr::RemoveCol(XCCOL col, int nAmount)
	{
		for(int i=0;i<nAmount;i++)
		{
			m_pCssClass->erase(col);
		}
		return 1;
	}

}}}}
