#include "HtTableStyle.hpp"
#include "xframe.h"
#include "xofbase.h"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	#define MAXROW 0x01<<30
	CHtTableStyle::CHtTableStyle(void)
	{
		m_pCssClass = new std::vector<LPColCssClass>;
		for (int i=0;i<255;i++)
		{
			LPColCssClass lp;
			lp= new XUtil::xuCsArray< INT32, CssClassIndex>(MAXROW, -1);
			m_pCssClass->push_back(lp);
		}
	}

	CHtTableStyle::~CHtTableStyle(void)
	{
		if(m_pCssClass)delete m_pCssClass;
	}

	const XUtil::Css::TBoxStyle *	CHtTableStyle::GetCellCssStyle(INT32 ACol,INT32 ARow)
	{
		if(ACol >=255) return NULL;
		if(ARow < 0 || ACol < 0 ) return NULL;

		int nIndex = (*m_pCssClass)[ACol]->GetValue(ARow);
		if(nIndex < 0) return NULL;

		return ((XOffice::XOfficeStyleSvr *)m_pStyleSvr)->m_pCssStyle->GetBoxStyle(nIndex);
	}

	int CHtTableStyle::SetCellCssStyle(INT32 ACol,INT32 ARow, XUtil::Css::TBoxStyle * pBoxStyle)
	{
		if(ACol >=255) return -1;
		if(ARow < 0 || ACol < 0 ) return -1;

		if(!pBoxStyle)
			(*m_pCssClass)[ACol]->SetValue(ARow,-1);
		else
		{
			int nIndex = ((XOffice::XOfficeStyleSvr *)m_pStyleSvr)->m_pCssStyle->AddCssCellStyle(pBoxStyle);
			(*m_pCssClass)[ACol]->SetValue(ARow,nIndex);
		}

		return 1;
	}

	int CHtTableStyle::SetCellCssStyle(RECT ARect, XUtil::Css::TBoxStyle * pBoxStyle)
	{
		if(ARect.left < 0) ARect.left = 0;
		if(ARect.top < 0) ARect.top = 0;
		
		int nIndex = -1;
		if(pBoxStyle) nIndex = ((XOffice::XOfficeStyleSvr *)m_pStyleSvr)->m_pCssStyle->AddCssCellStyle(pBoxStyle);
		for(int i=ARect.left; i<ARect.right;i++)
		{
			(*m_pCssClass)[i]->SetValue(ARect.top,ARect.bottom, nIndex);
		}
		return 1;
	}

	int CHtTableStyle::SetFont(RECT ARect, LOGFONT	&logFont)
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
					pBoxStyle =( XUtil::Css::TBoxStyle*) ((XOffice::XOfficeStyleSvr *)m_pStyleSvr)->m_pCssStyle->GetBoxStyle(nIndex);
				XUtil::Css::SetFont(pBoxStyle,logFont);

				(*m_pCssClass)[i]->SetValue(nRow,nLast >ARect.bottom? nLast : ARect.bottom ,((XOffice::XOfficeStyleSvr *)m_pStyleSvr)->m_pCssStyle->AddCssCellStyle(pBoxStyle));
				if(nIndex < 0)
					delete pBoxStyle;
				if(nLast==MAXROW)break;
				nRow = nLast + 1;
			}
		}
		return 1;
	}

	int CHtTableStyle::GetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont)
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

	int CHtTableStyle::SetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont)
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

	int CHtTableStyle::InsertRow(INT32 row, int nAmount)
	{
		for(int i=0;i<(int)m_pCssClass->size();i++)
		{
			(*m_pCssClass)[i]->Insert(row,nAmount);
		}
		return 1;
	}

	int CHtTableStyle::RemoveRow(INT32 row, int nAmount)
	{
		for(int i=0;i<(int)m_pCssClass->size();i++)
		{
			(*m_pCssClass)[i]->Remove(row,nAmount);
		}
		return 1;
	}

	int CHtTableStyle::InsertCol(INT32 col, int nAmount)
	{
		for(int i=0;i<nAmount;i++)
		{
			LPColCssClass lp;
			lp= new XUtil::xuCsArray<INT32, CssClassIndex>( MAXROW, -1);
			if(col > 1)
				lp->CopyFrom(*(*m_pCssClass)[col],1,MAXROW);
			m_pCssClass->insert(m_pCssClass->begin() + col,lp);
		}
		return 1;
	}

	int CHtTableStyle::RemoveCol(INT32 col, int nAmount)
	{
		for(int i=0;i<nAmount;i++)
		{
			m_pCssClass->erase(m_pCssClass->begin() + col);
		}
		return 1;
	}

	void * CHtTableStyle::GetClassStyle()
	{
		return m_pStyleSvr;
	}

	void CHtTableStyle::SetClassStyle(void * pClassStyle)
	{
		m_pStyleSvr = pClassStyle;
	}

}}}
