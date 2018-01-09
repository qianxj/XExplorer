#include "stdafx.h"
#include "IXSHtml.hpp"
#include "HtTableView.hpp"

namespace Hxsoft{ namespace XFrame
{
	LPCTSTR IXSHtmlItem::GetUrl(xcSHtml::htBlock* pBlock)
	{
		if(pBlock && pBlock->ht==xcSHtml::ehtA)
			return ((xcSHtml::htA *)pBlock)->pStrUrl;
		return NULL;
	}

	LPCTSTR IXSHtmlItem::GetData(xcSHtml::htBlock* pBlock, int nRow, int nCol)
	{
		if(pBlock && pBlock->ht == xcSHtml::ehtTable)
		{
			if(nRow < 1 && nCol < 1) return NULL;
			XControl::CHtTableView * pView = (XControl::CHtTableView *)((xcSHtml::htTable*)pBlock)->pView;
			if(pView->GetColCount() >= nCol && pView->GetRowCount() >= nRow)
			{
				return pView->GetItemString(nCol,nRow);
			}
		}
		return NULL;
	}

	LPCTSTR IXSHtmlItem::GetData(xcSHtml::htBlock* pBlock)
	{
		if(pBlock && pBlock->ht == xcSHtml::ehtTable)
		{
			XControl::CHtTableView * pView = (XControl::CHtTableView *)((xcSHtml::htTable*)pBlock)->pView;
			if(((xcSHtml::htTable*)pBlock)->FRow < 0) return NULL;
			if(((xcSHtml::htTable*)pBlock)->FColumn < 0) return NULL;
			return pView->GetItemString(((xcSHtml::htTable*)pBlock)->FColumn,
				((xcSHtml::htTable*)pBlock)->FRow);
		}
		if(pBlock && pBlock->pData)return pBlock->pData;
		return NULL;
	}

	int IXSHtmlItem::GetRowColumn(xcSHtml::htBlock* pBlock, int &Row, int &Col)
	{
		Row = 0;
		Col = 0;
		if(pBlock && pBlock->ht == xcSHtml::ehtTable)
		{
			Row = ((xcSHtml::htTable*)pBlock)->FRow;
			Col = ((xcSHtml::htTable*)pBlock)->FColumn;
			return 1;
		}
		return -1;
	}
	
	LPCTSTR IXSHtmlItem::GetID(xcSHtml::htBlock* pBlock)
	{
		return pBlock->pStrID;
	}

	int IXSHtmlItem::SetID(xcSHtml::htBlock* pBlock,LPTSTR newID)
	{
		if(pBlock->pStrID) delete pBlock->pStrID;
		pBlock->pStrID = _wcsdup(newID);
		return 1;
	}

	int IXSHtmlItem::RenameID(xcSHtml::htBlock* pBlock,LPTSTR ID)
	{
		if(pBlock->pStrID) delete pBlock->pStrID;
		pBlock->pStrID = _wcsdup(ID);
		return 1;
	}
	
	int IXSHtmlItem::SetData(xcSHtml::htBlock* pBlock,LPTSTR strText)
	{
		if(pBlock->pData) delete pBlock->pData;
		pBlock->pData = _wcsdup(strText);
		return 1;
	}
	
	int IXSHtmlItem::SetText(xcSHtml::htBlock* pBlock,LPTSTR strText)
	{
		if(pBlock->ht==xcSHtml::ehtText)
		{
			xcSHtml::htText* pText = (xcSHtml::htText*)pBlock;
			if(pText->text)delete pText->text;
			pText->text = strText;
			return 1;
		}
		if(pBlock->pChilds->size())return SetText((*pBlock->pChilds)[0],strText);
		return 0;
	}

	LPCTSTR  IXSHtmlItem::GetText(xcSHtml::htBlock* pBlock)
	{
		if(pBlock->ht==xcSHtml::ehtText)
		{
			xcSHtml::htText* pText = (xcSHtml::htText*)pBlock;
			return pText->text;
		}
		if(pBlock->pChilds->size())return GetText((*pBlock->pChilds)[0]);
		return 0;
	}

	int IXSHtmlItem::SetStyle(xcSHtml::htBlock* pBlock,LPTSTR cssStyle)
	{
		if(pBlock->pBoxStyle) pBlock->pBoxStyle = 0;
		if(pBlock->pCssStyle) delete pBlock->pCssStyle;
		pBlock->pCssStyle = _wcsdup(cssStyle);
		return 1;
	}
	int IXSHtmlItem::SetStyleClass(xcSHtml::htBlock* pBlock,LPTSTR cssStyleClass)
	{
		if(pBlock->pBoxStyle) pBlock->pBoxStyle = 0;
		if(pBlock->pCssClass ) delete pBlock->pCssClass;
		pBlock->pCssClass = _wcsdup(cssStyleClass);
		return 1;
	}


	IXSHtml::IXSHtml(void)
	{

	}

	IXSHtml::~IXSHtml(void)
	{

	}

	int IXSHtml::SetContent(LPCTSTR pStr)
	{
		if(((xcSHtml *)m_pControl)->m_pBlock)delete ((xcSHtml *)m_pControl)->m_pBlock;
		((xcSHtml *)m_pControl)->m_pBlock = ((xcSHtml *)m_pControl)->ParseXml((LPTSTR)pStr);
		return 1;
	}

	int IXSHtml::DrawHtml(HDC hPaintDC,LPTSTR pHtml, RECT &rt)
	{
		xcSHtml::htBlock* pBlock = ((xcSHtml *)m_pControl)->ParseXml((LPTSTR)pHtml);
		if(!pBlock) return 1;

		POINT ptOffset={0,0};
		int rowHeight;
		((xcSHtml *)m_pControl)->DoDrawBlock(hPaintDC,pBlock,rt,ptOffset,rowHeight);
		return 1;
	}

	xcSHtml::htBlock* IXSHtml::FoundBlockByID(LPTSTR ID)
	{
		xcSHtml::htBlock* pBlock = ((xcSHtml *)m_pControl)->m_pBlock;
		if(!pBlock) return 0;
		if(((xcSHtml *)m_pControl)->m_pBlock->ht==((xcSHtml *)m_pControl)->ehtHtml)
		{
			pBlock = NULL;
			if(((xcSHtml *)m_pControl)->m_pBlock->pChilds)
			{
				for(int i=0;i<(int)((xcSHtml *)m_pControl)->m_pBlock->pChilds->size();i++)
				{
					if((*((xcSHtml *)m_pControl)->m_pBlock->pChilds)[i] && (*((xcSHtml *)m_pControl)->m_pBlock->pChilds)[i]->bVisible && (*((xcSHtml *)m_pControl)->m_pBlock->pChilds)[i]->ht == ((xcSHtml *)m_pControl)->ehtBody)
					{
						pBlock = (*((xcSHtml *)m_pControl)->m_pBlock->pChilds)[i];
						break;
					}
				}
			}
		}
		if(!pBlock) return 0;
		if(pBlock->pStrID &&_tcscmp(pBlock->pStrID, ID) == 0) return pBlock;
		return FoundBlockByID(pBlock,ID);
	}

	xcSHtml::htBlock* IXSHtml::FoundBlockByID(xcSHtml::htBlock* pBlock, LPTSTR ID)
	{
		if(!pBlock) return 0;
		if(ID && ID[0] && pBlock->pChilds)
		{
			for(int i=0;i<(int)pBlock->pChilds->size();i++)
			{
				if((*pBlock->pChilds)[i] && (*pBlock->pChilds)[i]->pStrID &&_tcscmp((*pBlock->pChilds)[i]->pStrID, ID) == 0)
				{
					return (*pBlock->pChilds)[i];
				}else
				{
					xcSHtml::htBlock* p = FoundBlockByID((*pBlock->pChilds)[i],ID);
					if(p) return p;
				}
			}
		}
		return 0;
	}

	int IXSHtml::SetElementVisible(LPTSTR ID, bool bVisible)
	{
		xcSHtml::htBlock* pBlock = FoundBlockByID(ID);
		if(pBlock) pBlock->bVisible = bVisible;
		return 1;
	}

	bool IXSHtml::SwitchState(LPTSTR ID)
	{
		xcSHtml::htBlock* pBlock = FoundBlockByID(ID);
		if(pBlock) pBlock->bVisible = !pBlock->bVisible;
		return false;
	}

	bool IXSHtml::IsVisible(LPTSTR ID)
	{
		xcSHtml::htBlock* pBlock = FoundBlockByID(ID);
		if(pBlock) return pBlock->bVisible;

		return false;
	}
}}
