#include "StdAfx.h"
#include "..\include\XWordLayer.hpp"
#include "xwordsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	#define  goThumbTracking         0x8000

	extern XUtil::Css::tagCssData 	_CssData;
	XWordLayer::XWordLayer(void):m_pWordData(NULL),m_pSheetSvr(NULL),m_State(TWordState_Normal),m_nPage(1),FOptions(goThumbTracking)
	{
		::SetRect(&FRect,0,0,0,0);
		m_ptScroll.x =0;
		m_ptScroll.y =0;
		::SetRect(&m_rcPage,0,0, MulDiv(210,100*_CssData.nXPixelPerInch,2540),
			MulDiv(297,100*_CssData.nYPixelPerInch,2540));

		::SetRect(&m_rcPageMargin,60,60,60,60);
		m_rcPageContent.left = m_rcPage.left + m_rcPageMargin.left;
		m_rcPageContent.right = m_rcPage.right - m_rcPageMargin.right;
		m_rcPageContent.top = m_rcPage.top + m_rcPageMargin.top;
		m_rcPageContent.bottom = m_rcPage.bottom - m_rcPageMargin.bottom;

		m_pWordDocumentBlock = new XWordDocumentBlock(this);
		InitCursorInfo(m_CursorInfo);
		InitSelection(m_Selection);
	}

	XWordLayer::~XWordLayer(void)
	{
		if(m_pWordDocumentBlock)delete m_pWordDocumentBlock;
	}

	int XWordLayer::CalcContentPageRect(int nPage,RECT * pDrawRect,RECT &rc)
	{
		if(!pDrawRect) return 0;
		RECT rect = *pDrawRect;

		RECT rcPage;
		if( rect.right - rect.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rect.left + 5;
		else
			rcPage.left = rect.left + (rect.right - rect.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rect.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;

		rc = rcPage;
		rc.top += (rcPage.bottom - rcPage.top + 1)* nPage + nPage * 20;
		rc.bottom = rc.top + rcPage.bottom - rcPage.top;

		rc.left += m_rcPageMargin.left;
		rc.top += m_rcPageMargin.top;
		rc.right -= m_rcPageMargin.right;
		rc.bottom -= m_rcPageMargin.bottom;

		return 1;
	}

	CXScrollBar * XWordLayer::GetHScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pHScrollBar;
	}
	CXScrollBar * XWordLayer::GetVScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pVScrollBar;
	}

	int DrawPageMarker(HDC hPaintDC,RECT &rcPage,RECT &rcPageMargin)
	{
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(192,192,192));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		
		//topleft
		::MoveToEx(hPaintDC,rcPage.left + rcPageMargin.left - 20,
			rcPage.top + rcPageMargin.top - 1,NULL);
		::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
			rcPage.top + rcPageMargin.top - 1);
		::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
			rcPage.top + rcPageMargin.top - 20); 
		
		//bottomleft
		::MoveToEx(hPaintDC,rcPage.left + rcPageMargin.left - 20,
			rcPage.bottom - rcPageMargin.bottom - 1,NULL);
		::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
			rcPage.bottom - rcPageMargin.bottom - 1);
		::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
			rcPage.bottom - rcPageMargin.bottom + 20); 

		//topright
		::MoveToEx(hPaintDC,rcPage.right - rcPageMargin.right + 20,
			rcPage.top + rcPageMargin.top - 1,NULL);
		::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
			rcPage.top + rcPageMargin.top - 1);
		::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
			rcPage.top + rcPageMargin.top - 20); 

		//bottomright
		::MoveToEx(hPaintDC,rcPage.right - rcPageMargin.right + 20,
			rcPage.bottom - rcPageMargin.bottom - 1,NULL);
		::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
			rcPage.bottom - rcPageMargin.bottom - 1);
		::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
			rcPage.bottom - rcPageMargin.bottom + 20); 

		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		return 1;
	}

	int XWordLayer::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		RECT	rc;
		if(!pDrawRect) 
			rc = FRect;
		else
			rc = *pDrawRect;

		int nSave = ::SaveDC(hPaintDC);
		IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(144,153,174));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		OffsetViewportOrgEx(hPaintDC,- m_ptScroll.x, - m_ptScroll.y,NULL);

		RECT rcPage;
		if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rc.left + 5;
		else
			rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rc.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;

		::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
		DrawPageMarker(hPaintDC,rcPage,m_rcPageMargin);

		for(int i=1;i<=m_nPage;i++)
		{
			POINT	pt;
			pt.y = rcPage.top + m_rcPageMargin.top;
			pt.x = rcPage.left + m_rcPageMargin.left;
			DoDrawPage(hPaintDC,i,pt);

			::OffsetRect(&rcPage, 0,rcPage.bottom - rcPage.top + 1 + 20);
			::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
			DrawPageMarker(hPaintDC,rcPage,m_rcPageMargin);
		}

		OffsetViewportOrgEx(hPaintDC,m_ptScroll.x,m_ptScroll.y,NULL);
		::RestoreDC(hPaintDC,nSave);
		return 0;
	}


	XWordLayer::XWordBlock * XWordLayer::XWordBlock::CreateWordBlock(XWordDataBase * pWordDataBase)
	{
		switch(pWordDataBase->eWordDataType)
		{
		case _XWordDataSpanControlBlock:
			return new XWordSpanControlBlock;
		case _XWordDataSpanNullBlock:
			return new XWordSpanNullBlock;
		case _XWordDataTextBlock:
			return new XWordTextBlock;
		case _XWordDataControlParagraph:
			return new XWordControlParagraph;
		case _XWordDataParagraph:
			return new XWordParagraphBlock;
		case _XWordDataChapter:
			return new XWordChapterBlock;
		case _XWordDataDocument:
			return new XWordDocumentBlock;
		}
		return NULL;
	}

	int XWordLayer::XWordBlock::GetIndexFromParent()
	{
		for(int i=0;i<(int)m_pBlockParent->m_pChilds->size();i++)
		{
			if((*m_pBlockParent->m_pChilds)[i]==this) return i;
		}
		return -1;
	}

	XWordLayer::XWordBlock * XWordLayer::XWordBlock::GetPirorTextBlock()
	{
		XWordTextBlock * pTextBlock;
		int index = this->GetIndexFromParent();
		for(int i=index - 1;i>=0;i--)
		{
			pTextBlock = dynamic_cast<XWordTextBlock *>((*m_pBlockParent->m_pChilds)[i]);
			if(pTextBlock) return pTextBlock;
		}
		index = m_pBlockParent->GetIndexFromParent();
		XWordParagraphBlock * pParagraph;
		XWordChapterBlock * pChapter;
		for(int i=index - 1;i>=0;i--)
		{
			pParagraph = dynamic_cast<XWordParagraphBlock *>((*m_pBlockParent->m_pBlockParent->m_pChilds)[i]);
			if(!pParagraph) continue;
			for(int k=(int)pParagraph->m_pChilds->size() - 1 ;k>=0;k--)
			{
				pTextBlock = dynamic_cast<XWordTextBlock *>((*pParagraph->m_pChilds)[k]);
				if(pTextBlock) return pTextBlock;
			}
		}
		index = m_pBlockParent->m_pBlockParent->GetIndexFromParent();
		for(int i=index - 1;i>=0;i--)
		{
			pChapter = dynamic_cast<XWordChapterBlock *>((*m_pBlockParent->m_pBlockParent->m_pBlockParent->m_pChilds)[i]);
			for(int c=(int)pChapter->m_pChilds->size() - 1;c>=0;c--)
			{
				pParagraph = dynamic_cast<XWordParagraphBlock *>((*pChapter->m_pChilds)[c]);
				if(!pParagraph) continue;
				for(int k=(int)pParagraph->m_pChilds->size() - 1 ;k>=0;k--)
				{
					pTextBlock = dynamic_cast<XWordTextBlock *>((*pParagraph->m_pChilds)[k]);
					if(pTextBlock) return pTextBlock;
				}
			}
		}

		return NULL;
	}

	XWordLayer::XWordBlock * XWordLayer::XWordBlock::GetNextTextBlock()
	{
		XWordTextBlock * pTextBlock;
		int index = this->GetIndexFromParent();
		for(int i=index + 1;i<(int)m_pBlockParent->m_pChilds->size();i++)
		{
			pTextBlock = dynamic_cast<XWordTextBlock *>((*m_pBlockParent->m_pChilds)[i]);
			if(pTextBlock) return pTextBlock;
		}
		index = m_pBlockParent->GetIndexFromParent();
		XWordParagraphBlock * pParagraph;
		XWordChapterBlock * pChapter;
		for(int i=index+1;i<(int)m_pBlockParent->m_pBlockParent->m_pChilds->size();i++)
		{
			pParagraph = dynamic_cast<XWordParagraphBlock *>((*m_pBlockParent->m_pBlockParent->m_pChilds)[i]);
			if(!pParagraph) continue;
			for(int k=0 ;k<(int)pParagraph->m_pChilds->size();k++)
			{
				pTextBlock = dynamic_cast<XWordTextBlock *>((*pParagraph->m_pChilds)[k]);
				if(pTextBlock) return pTextBlock;
			}
		}
		index = m_pBlockParent->m_pBlockParent->GetIndexFromParent();
		for(int i=index+1;i<(int)m_pBlockParent->m_pBlockParent->m_pBlockParent->m_pChilds->size();i++)
		{
			pChapter = dynamic_cast<XWordChapterBlock *>((*m_pBlockParent->m_pBlockParent->m_pBlockParent->m_pChilds)[i]);
			for(int c=0;c<(int)pChapter->m_pChilds->size() ;c++)
			{
				pParagraph = dynamic_cast<XWordParagraphBlock *>((*pChapter->m_pChilds)[c]);
				if(!pParagraph) continue;
				for(int k=0;k<(int)pParagraph->m_pChilds->size();k++)
				{
					pTextBlock = dynamic_cast<XWordTextBlock *>((*pParagraph->m_pChilds)[k]);
					if(pTextBlock) return pTextBlock;
				}
			}
		}

		return NULL;
	}

	bool XWordLayer::XWordBlock::BlockIsSelected(XWordBlock * pStartBlock,XWordBlock * pEndBlock)
	{
		return GetDocumentBlock()->m_pLayer->BlockIsSelected(this,pStartBlock,pEndBlock);
	}

	int XWordLayer::XWordBlock::CalcContentRect()
	{
		m_ContentRect = m_AreaRect;

		const XUtil::Css::TBoxStyle * pCssBoxStyle = GetDocumentBlock()->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(pCssBoxStyle)
		{
			if(pCssBoxStyle->mask & cssMaskMargin)
			{
				if(pCssBoxStyle->pMargin[0])
				{
					int valuex =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0]);
					int valuey =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0],false);
					m_ContentRect.left += valuex;
					m_ContentRect.right -= valuey;
					m_ContentRect.top += valuex;
					m_ContentRect.bottom -= valuey;
				}
				if(pCssBoxStyle->pMargin[1])m_ContentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[1]);
				if(pCssBoxStyle->pMargin[2])m_ContentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[2],false);
				if(pCssBoxStyle->pMargin[3])m_ContentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[3]);
				if(pCssBoxStyle->pMargin[4])m_ContentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[4],false);

			}
			if(pCssBoxStyle->mask & cssMaskPadding)
			{
				if(pCssBoxStyle->pPadding[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[0]);
					m_ContentRect.left += value;
					m_ContentRect.right -= value;
					m_ContentRect.top += value;
					m_ContentRect.bottom -= value;
				}
				if(pCssBoxStyle->pPadding[1])m_ContentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[1]);
				if(pCssBoxStyle->pPadding[2])m_ContentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[2],false);
				if(pCssBoxStyle->pPadding[3])m_ContentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[3]);
				if(pCssBoxStyle->pPadding[4])m_ContentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[4],false);

			}
			if(pCssBoxStyle->mask & cssMaskBorder)
			{
				if(pCssBoxStyle->pBorder[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(&pCssBoxStyle->pBorder[0]->widthmesure);
					m_ContentRect.left += value;
					m_ContentRect.right -= value;
					m_ContentRect.top += value;
					m_ContentRect.bottom -= value;
				}
				if(pCssBoxStyle->pBorder[1])m_ContentRect.left += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[1]);
				if(pCssBoxStyle->pBorder[2])m_ContentRect.top += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[2],false);
				if(pCssBoxStyle->pBorder[3])m_ContentRect.right -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[3]);
				if(pCssBoxStyle->pBorder[4])m_ContentRect.bottom -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[4],false);
			}
		}
		return 1;
	}

	int XWordLayer::XWordBlock::AdjustExtent()
	{
		return 0;
	}

	XWordLayer::XWordDocumentBlock * XWordLayer::XWordBlock::GetDocumentBlock() 
	{
		XWordBlock * pParentWordBlock = this;
		while(pParentWordBlock->m_pBlockParent)
		{
			pParentWordBlock = pParentWordBlock->m_pBlockParent;
		}
		return (XWordDocumentBlock *)pParentWordBlock;
	}

	int XWordLayer::XWordBlock::LoadData(XWordDataBase * pContent,bool bLoadChild)
	{
		m_pContent = pContent;
		m_nCssStyle = pContent->m_nIndexStyle;
		if(bLoadChild)
		{
			for(int i=0;i<pContent->m_pChilds->size();i++)
			{
				XWordBlock * pWordBlock =this->CreateWordBlock((*pContent->m_pChilds)[i]);
				pWordBlock->m_pBlockParent = this;
				pWordBlock->m_pContent = (*pContent->m_pChilds)[i];
				m_pChilds->push_back(pWordBlock);
				pWordBlock->LoadData(pWordBlock->m_pContent,bLoadChild);
			}
		}
		return 0;
	}

	bool XWordLayer::XWordBlock::CalcExtent(XWordBlockExtentInfo &extentInfo)
	{
		return true;
	}

	int XWordLayer::XWordTextBlock::GetFitedWidth(int startChar,int nFited)
	{
		if(!((XWordDataTextBlock *)m_pContent)->m_pStrText) return 0;
		if(startChar<0) startChar = 0;
		if(startChar + nFited>= (int)::_tcslen(((XWordDataTextBlock *)m_pContent)->m_pStrText)) return 0;
		if(startChar > 0)
			return m_pDx[startChar + nFited - 1] - m_pDx[startChar];
		else
			return m_pDx[startChar + nFited - 1];
	}

	bool XWordLayer::XWordTextBlock::CalcExtent(XWordBlockExtentInfo &extentInfo)
	{
		extentInfo.nFits.clear();
		extentInfo.nLens.clear();
		extentInfo.nZWFits.clear();
		extentInfo.pStr = ((XWordDataTextBlock *)m_pContent)->m_pStrText;

		if(extentInfo.pStr)
			extentInfo.nStrLen = (int)_tcslen(extentInfo.pStr);
		else
			extentInfo.nStrLen=0;

		if(!extentInfo.hPaintDC && m_pDx)
		{
			extentInfo.pDx = m_pDx;
			extentInfo.rowheight = m_nRowHeight;
		}else
		{
			int nFit;
			SIZE size;
			HDC	hDC;
			int* pDx;
			if(extentInfo.nStrLen >0)
				pDx = new int[extentInfo.nStrLen];
			else
				pDx = NULL;
			const XUtil::Css::TBoxStyle * pBoxStyle;
			pBoxStyle = ((XWordDocumentBlock *)GetDocumentBlock())->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(this->m_nCssStyle);
			if(!pBoxStyle)
				pBoxStyle =  ((XWordDocumentBlock *)GetDocumentBlock())->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(0);
			LOGFONT LogFont;
			HFONT hFont = NULL;
			if(pBoxStyle /*&& pBoxStyle->mask & cssMaskFont*/)
			{
				XUtil::Css::GetFont(pBoxStyle,LogFont);
				if(extentInfo.hPaintDC)
				{
					int nYPixelPerInch = GetDeviceCaps(extentInfo.hPaintDC, LOGPIXELSY);
					::MulDiv(LogFont.lfHeight, nYPixelPerInch,_CssData.nYPixelPerInch);
				}
				hFont = ::CreateFontIndirect(&LogFont);
			}
			if(extentInfo.hPaintDC)
			{
				hDC = extentInfo.hPaintDC;
				if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
				GetTextExtentPoint(hDC,extentInfo.pStr,extentInfo.nStrLen,&size);
				GetTextExtentExPoint(hDC,extentInfo.pStr,extentInfo.nStrLen,/*extentInfo.endx - extentInfo.startx*/size.cx + 1,&nFit,pDx,&size);
				if(extentInfo.nStrLen==0)
				{
					GetTextExtentPoint(hDC,_T("AFGXMafxgm"),10,&size);
					size.cx = 0;
				}
				if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
			}else
			{
				hDC = ::GetDC(NULL);
				if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
				GetTextExtentPoint(hDC,extentInfo.pStr,extentInfo.nStrLen,&size);
				GetTextExtentExPoint(hDC,extentInfo.pStr,extentInfo.nStrLen,/*extentInfo.endx - extentInfo.startx*/size.cx + 1,&nFit,pDx,&size);
				if(extentInfo.nStrLen==0)
				{
					GetTextExtentPoint(hDC,_T("AFGXMafxgm"),10,&size);
					size.cx = 0;
				}
				if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
				m_pDx = pDx;
				::ReleaseDC(NULL,hDC);
			}
			extentInfo.pDx =pDx;
			extentInfo.rowheight =(int)( size.cy * 1.5);		//??? should calc rowheight by style row-height
			m_nRowHeight = extentInfo.rowheight;
			if(hFont) ::DeleteObject(hFont);
		}

		if(extentInfo.nStrLen==0)
		{
			extentInfo.nFitFirstRow=0;
			extentInfo.rows=1;
			extentInfo.nFits.push_back(0);
			extentInfo.nZWFits.push_back(0);
			extentInfo.nLens.push_back(0);
			extentInfo.xPosLast = extentInfo.startx;
			extentInfo.cx = 0;
			extentInfo.cy = extentInfo.rowheight;
			return true;
		}

		int nFited = 0;
		int nPrevFited = 0;
		int nFitedX = 0;
		int nLen=0;
		extentInfo.rows = 0;
		int i=0;
		//TCHAR a='\r';
		if(extentInfo.nStrLen==0)
		{
			extentInfo.nFitFirstRow=0;
			extentInfo.rows=1;
			extentInfo.nFits.push_back(0);
			extentInfo.nZWFits.push_back(0);
			extentInfo.nLens.push_back(0);
			extentInfo.xPosLast = extentInfo.startx;
			extentInfo.cx = 0;
			extentInfo.cy = extentInfo.rowheight;
			return true;
		}else
		{
			while(i<extentInfo.nStrLen && extentInfo.pDx[i] <= extentInfo.endx -extentInfo.startx + 1&&extentInfo.pStr[i]!='\r' &&extentInfo.pStr[i]!='\n' &&extentInfo.pStr[i]!=0x0B)i++;
			if(i>0 || extentInfo.pStr[i]=='\r' || extentInfo.pStr[i]!='\n' || extentInfo.pStr[i]!=0x0B)
			{
				int z=i;
				if(extentInfo.pStr[i]==0x0B)
					i++;
				else
				{
					if(extentInfo.pStr[i]=='\r' ||extentInfo.pStr[i]=='\n')
					{
						i++;
						if(extentInfo.pStr[i - 1]=='\r' && extentInfo.pStr[i]=='\n')i++;
					}
				}

				nFited = i;
				extentInfo.nFitFirstRow = i;
				extentInfo.rows = 1;
				extentInfo.nFits.push_back(nFited);
				nLen = extentInfo.pDx[i - 1];
				extentInfo.nLens.push_back(nLen);
				extentInfo.nZWFits.push_back(i - z);
			}
			else
			{   //can't fit one letter
				if(extentInfo.startx==0)
				{
					i++;
					nFited = i;
					extentInfo.nFitFirstRow = i;
					extentInfo.rows = 1;
					extentInfo.nFits.push_back(nFited);
					nLen = extentInfo.pDx[i - 1];
					extentInfo.nLens.push_back(nLen);
					extentInfo.nZWFits.push_back(0);
				}else
				{
					extentInfo.nFitFirstRow = 0;
					extentInfo.rows = 0;
				}
			}
			if(i>=extentInfo.nStrLen)
			{
				if(extentInfo.nStrLen >=1 &&( extentInfo.pStr[extentInfo.nStrLen -1]=='\r'||extentInfo.pStr[extentInfo.nStrLen -1]=='\n' ||extentInfo.pStr[extentInfo.nStrLen -1]==0x0B))
				{
					extentInfo.rows ++;
					extentInfo.nFits.push_back(0);
					extentInfo.nLens.push_back(0);
					extentInfo.nZWFits.push_back(0);
					extentInfo.xPosLast = 0;
				}else
				{
					//get last xpos
					extentInfo.xPosLast = (extentInfo.rows==1?extentInfo.startx:0) +  extentInfo.nLens[extentInfo.nLens.size() - 1];
				}
			}
			else
			{
				nPrevFited =  extentInfo.nFitFirstRow;
				while(true)
				{
					if(nFited > 0) nFitedX = extentInfo.pDx[nFited - 1];
					int i = nFited;
					while(i<extentInfo.nStrLen && extentInfo.pDx[i] - nFitedX <= extentInfo.endx + 1 &&extentInfo.pStr[i]!='\r'&&extentInfo.pStr[i]!='\n'&&extentInfo.pStr[i]!=0x0B)i++;
					int z=i;
					if(i==nFited || extentInfo.pStr[i]=='\r' || extentInfo.pStr[i]!='\n' || extentInfo.pStr[i]!=0x0B)		//can't include one char
					{
						if(extentInfo.pStr[i]==0x0B)
							i++;
						else
						{
							if(extentInfo.pStr[i]=='\r' ||extentInfo.pStr[i]=='\n')
							{
								i++;
								if(extentInfo.pStr[i - 1]=='\r' && extentInfo.pStr[i]=='\n')i++;
							}
						}
					}else
					{
						if(extentInfo.pStr[i]==0x0B)
							i++;
						else
						{
							if(extentInfo.pStr[i]=='\r' ||extentInfo.pStr[i]=='\n')
							{
								i++;
								if(extentInfo.pStr[i - 1]=='\r' && extentInfo.pStr[i]=='\n')i++;
							}
						}
					}
					nFited = i;
					extentInfo.rows++;
					if(extentInfo.rows==1)
						extentInfo.nFits.push_back(nFited );
					else
						extentInfo.nFits.push_back(nFited - nPrevFited );

					extentInfo.nZWFits.push_back(i - z);
					nLen = extentInfo.pDx[i - 1] - nFitedX;
					extentInfo.nLens.push_back(nLen);

					if(nFited >= extentInfo.nStrLen)
					{
						if(extentInfo.nStrLen >=1 &&( extentInfo.pStr[extentInfo.nStrLen -1]=='\r'||extentInfo.pStr[extentInfo.nStrLen -1]=='\n' ||extentInfo.pStr[extentInfo.nStrLen -1]==0x0B))
						{
							extentInfo.rows ++;
							extentInfo.nFits.push_back(0);
							extentInfo.nLens.push_back(0);
							extentInfo.nZWFits.push_back(0);
							extentInfo.xPosLast = 0;
						}else
						{
							//get last xpos
							extentInfo.xPosLast = (extentInfo.rows==1?extentInfo.startx:0) + nLen;
						}
						break;
					}
					nPrevFited = nFited;
				}
			}
			if(extentInfo.rows <=1)
			{
				extentInfo.cx = extentInfo.xPosLast - extentInfo.startx + 1;
				extentInfo.cy = extentInfo.rowheight;
			}else
			{
				extentInfo.cx = extentInfo.endx;
				extentInfo.cy = extentInfo.rowheight * extentInfo.rows;
			}
		}
		return true;
	}

	bool XWordLayer::XWordSpanControlBlock::CalcExtent(XWordBlockExtentInfo &extentInfo)
	{
		extentInfo.rowheight = m_pContent->height;
		if(extentInfo.endx - extentInfo.startx + 1> m_pContent->width)
		{
			extentInfo.nFitFirstRow = 1;
			extentInfo.xPosLast = extentInfo.startx + m_pContent->width - 1;
		}else
		{
			extentInfo.nFitFirstRow = 0;
			extentInfo.xPosLast = m_pContent->width - 1;
		}

		extentInfo.rows = 1;
		if(m_pContent->width > 0) extentInfo.cx = m_pContent->width;
		if(m_pContent->height > 0) extentInfo.cy = m_pContent->height;

		return true;
	}


	int CalcRowInfoBala(XWordLayer::XWordParagraphRowInfo &rowInfo,int width)
	{
		int cx = width;
		for(int m=0;m<(int)rowInfo.m_rowBlocks.size();m++)
		{
			cx -= rowInfo.m_rowBlocks[m].nLen;
		}
		return cx>=10?cx:0;
	}

	int XWordLayer::XWordParagraphBlock::AdjustExtent()
	{
		int width,height = 0;
		int startx,endx;
		int rowheight = 0;

		//get width
		if(m_pContent->width > 0) 
			width = m_pContent->width;
		else
			width = GetDocumentBlock()->m_pLayer->m_rcPageContent.right -
			GetDocumentBlock()->m_pLayer->m_rcPageContent.left;

		startx = 0;
		endx = startx + width - 1;
		XWordBlockExtentInfo extentInfo;
		XWordParagraphRowInfo rowInfo;
		XWordParagraphRowInfo::RowBlock  rowBlockInfo;
		m_rowInfos.clear(); 
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			extentInfo.startx = startx;
			extentInfo.endx = endx;
			(*m_pChilds)[i]->CalcExtent(extentInfo);
			int nStartCharPos = 0;  
			if(extentInfo.nFitFirstRow < 1 && extentInfo.nStrLen !=0 )
			{
				height += rowheight;
				if(rowInfo.m_rowBlocks.size()>0)
				{
					rowInfo.nBala = CalcRowInfoBala(rowInfo,width);
					m_rowInfos.push_back(rowInfo);
					rowInfo.m_rowBlocks.clear();
				}
				rowheight = extentInfo.rowheight;
				height += extentInfo.cy - extentInfo.rowheight;
			}else
			{
				if(rowheight < extentInfo.rowheight) rowheight = extentInfo.rowheight;
				if(	extentInfo.rows > 1)
				{
					height += rowheight;
					rowheight = extentInfo.rowheight;
					height += extentInfo.cy - 2 * extentInfo.rowheight;
				}
			}
			//create rowsinfo
			nStartCharPos=0;
			for(int j=0;j<extentInfo.rows;j++)
			{
				rowBlockInfo.pBlock = (*m_pChilds)[i];
				if(j==0 && extentInfo.nFitFirstRow>0)
					rowBlockInfo.nStartX = startx;
				else
					rowBlockInfo.nStartX = 0;

				rowBlockInfo.nLen = extentInfo.nLens[j];
				rowBlockInfo.nRowheight = extentInfo.rowheight;
				rowBlockInfo.nStartCharPos = nStartCharPos;
				rowBlockInfo.nFited = extentInfo.nFits[j];
				rowBlockInfo.nZWFited = extentInfo.nZWFits[j];
				nStartCharPos += rowBlockInfo.nFited;
				rowInfo.m_rowBlocks.push_back(rowBlockInfo);
				if(rowInfo.nRowHeight < rowheight)rowInfo.nRowHeight = rowheight;
				if(j< extentInfo.rows - 1)
				{
					rowInfo.nBala = CalcRowInfoBala(rowInfo,width);
					m_rowInfos.push_back(rowInfo);
					rowInfo.m_rowBlocks.clear();
					rowInfo.nRowHeight = 0;
				}
			}

			startx = extentInfo.xPosLast + 1;
			if(startx >= width - 1)
			{
				startx = 0;
				height += rowheight;
				rowheight = 0;
				rowInfo.nBala = CalcRowInfoBala(rowInfo,width);
				m_rowInfos.push_back(rowInfo);
				rowInfo.m_rowBlocks.clear();
			}
			if(rowheight > 0 && i==m_pChilds->size() -1)
			{
				height += rowheight;
				rowInfo.nBala = CalcRowInfoBala(rowInfo,width);
				m_rowInfos.push_back(rowInfo);
				rowInfo.m_rowBlocks.clear();
			}
		}
		m_AreaRect.bottom = m_AreaRect.top + height - 1;
		m_AreaRect.right = m_AreaRect.left + width -1;

		CalcContentRect();
		return 0;
	}

	int XWordLayer::XWordContentParagraph::AdjustExtent()
	{
		if(m_pContent->x > 0) 
			m_AreaRect.left =  m_pContent->x -1;  

		if(m_pContent->y > 0) 
			m_AreaRect.top =  m_pContent->y -1;  

		if(m_pContent->height > 0) 
			m_AreaRect.bottom = m_AreaRect.top + m_pContent->height -1;  
		else
			m_AreaRect.bottom = 200;

		if(m_pContent->width > 0) 
			m_AreaRect.right = m_AreaRect.left + m_pContent->width -1;
		else
			m_AreaRect.right = m_AreaRect.left + GetDocumentBlock()->m_pLayer->m_rcPageContent.right -
			GetDocumentBlock()->m_pLayer->m_rcPageContent.left;

		CalcContentRect();
		return 0;
	}

	int XWordLayer::XWordChapterBlock::AdjustExtent()
	{	
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->AdjustExtent();
		}

		int height = 0;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			height += (*m_pChilds)[i]->m_AreaRect.bottom ;/*- (*m_pChilds)[i]->m_AreaRect.top*/;
		}

		m_AreaRect.bottom = m_AreaRect.top + height ;
		m_AreaRect.right = m_AreaRect.left + GetDocumentBlock()->m_pLayer->m_rcPageContent.right -
			GetDocumentBlock()->m_pLayer->m_rcPageContent.left;

		return 0;
	}
	int XWordLayer::XWordDocumentBlock::AdjustExtent()
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->AdjustExtent();
		}

		int height = 0;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(i==0)
				height = (*m_pChilds)[i]->m_AreaRect.bottom - (*m_pChilds)[i]->m_AreaRect.top;
			else
				height += (*m_pChilds)[i]->m_AreaRect.bottom - (*m_pChilds)[i]->m_AreaRect.top;
		}

		m_AreaRect.bottom = m_AreaRect.top + height ;
		m_AreaRect.right = m_AreaRect.left + GetDocumentBlock()->m_pLayer->m_rcPageContent.right -
			GetDocumentBlock()->m_pLayer->m_rcPageContent.left;
		m_pLayer->m_nPage  = m_pLayer->CalcPages();
		return 0;
	}

	int XWordLayer::XWordParagraphBlock::DoDraw(HDC hPaintDC,POINT ptOffset,int &nStartRow, int & nHeight)
	{
		//XWordBlock::DoDraw(hPaintDC,ptOffset);
		RECT rect = this->m_ContentRect;

		const XUtil::Css::TBoxStyle * pBoxStyle; 
		LOGFONT LogFont;
		HFONT hFont = NULL;

		int nBkMode = ::SetBkMode(hPaintDC,TRANSPARENT);
		int rowheight;
		XWordBlock * pBlock;
		XWordTextBlock * pTextBlock;
		LPTSTR pStr;
		int nFit;
		int startx0 = ptOffset.x + rect.left;
		int starty = ptOffset.y + rect.top;
		bool bSelection = false;
		int startx;
		int i=0;
		for(i=0;i<(int)this->m_rowInfos.size();i++)
		{
			startx = startx0;
			if(this->m_pContent->align&DT_CENTER && m_rowInfos[i].nBala >0)
				startx += m_rowInfos[i].nBala/2;
			if(this->m_pContent->align&DT_RIGHT && m_rowInfos[i].nBala >0)
				startx += m_rowInfos[i].nBala;
			if(i<nStartRow) continue;
			rowheight = m_rowInfos[i].nRowHeight;
			if(rowheight > nHeight && nHeight!=0)
				break;
			else
				nHeight -= rowheight;
			for(int j=0;j<(int)m_rowInfos[i].m_rowBlocks.size();j++)
			{
				pBlock = m_rowInfos[i].m_rowBlocks[j].pBlock;
				pBoxStyle = ((XWordDocumentBlock *)pBlock->GetDocumentBlock())->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(pBlock->m_nCssStyle);
				if(!pBoxStyle)
					pBoxStyle = ((XWordDocumentBlock *)pBlock->GetDocumentBlock())->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(0);

				XWordBlock * pStartBlock,* pEndBlock;
				int nStartBlock,nEndBlock;
				int nStartPosChar,nEndPosChar;
				
				if(pBlock->m_pContent->eWordDataType==_XWordDataTextBlock)
				{
					if(this->GetDocumentBlock()->m_pLayer->m_Selection.bSelection)
					{
						XWordParagraphBlock* pPara;
						int nParaRow;
						this->GetDocumentBlock()->m_pLayer->CalcParagraphRow(this->GetDocumentBlock()->m_pLayer->m_Selection.nStartRow,pPara,nParaRow);
						this->GetDocumentBlock()->m_pLayer->CalcBlockCol(pPara,nParaRow,this->GetDocumentBlock()->m_pLayer->m_Selection.nStartCol,nStartBlock,nStartPosChar);
						pStartBlock = pPara->m_rowInfos[nParaRow].m_rowBlocks[nStartBlock].pBlock;
						this->GetDocumentBlock()->m_pLayer->CalcParagraphRow(this->GetDocumentBlock()->m_pLayer->m_Selection.nEndRow,pPara,nParaRow);
						this->GetDocumentBlock()->m_pLayer->CalcBlockCol(pPara,nParaRow,this->GetDocumentBlock()->m_pLayer->m_Selection.nEndCol,nEndBlock,nEndPosChar);
						pEndBlock = pPara->m_rowInfos[nParaRow].m_rowBlocks[nEndBlock].pBlock;

						if(pBlock->BlockIsSelected(pStartBlock,pEndBlock) &&  
							(pStartBlock !=  pEndBlock ||
							nStartPosChar !=  nEndPosChar))
						{
							bSelection = true;
						}else
							bSelection = false;
					}else
						bSelection = false;

					pTextBlock = (XWordTextBlock*)pBlock;
					XUtil::Css::GetFont(pBoxStyle,LogFont);
					hFont = ::CreateFontIndirect(&LogFont);
					hFont = (HFONT)::SelectObject(hPaintDC,hFont);

					pStr = ((XWordDataTextBlock*)pTextBlock->m_pContent)->m_pStrText;
					pStr += m_rowInfos[i].m_rowBlocks[j].nStartCharPos;
					nFit = m_rowInfos[i].m_rowBlocks[j].nFited;

					if(bSelection)
					{
						int nLenUnSelectionFirst=0;
						int nLenSelection=0;
						int nLenUnSelectionLast = 0;
						if(pBlock==pStartBlock)
							nLenUnSelectionFirst = nStartPosChar - m_rowInfos[i].m_rowBlocks[j].nStartCharPos;
						else
							nLenUnSelectionFirst=0;
						if(pBlock==pEndBlock)
							nLenUnSelectionLast = m_rowInfos[i].m_rowBlocks[j].nStartCharPos + nFit - nEndPosChar;
						else
							nLenUnSelectionLast=0;
						if(nLenUnSelectionFirst<0)nLenUnSelectionFirst = 0;
						if(nLenUnSelectionLast <0) nLenUnSelectionLast = 0;
						if(nLenUnSelectionFirst>nFit)nLenUnSelectionFirst = nFit;
						if(nLenUnSelectionLast>nFit)nLenUnSelectionLast = nFit;
						nLenSelection = nFit - nLenUnSelectionFirst- nLenUnSelectionLast;

						if(nLenSelection <=0)
							ExtTextOut(hPaintDC,startx + m_rowInfos[i].m_rowBlocks[j].nStartX,starty +  rowheight - pTextBlock->m_nRowHeight ,0,NULL,pStr,nFit,NULL);
						else
						{
							if(nLenUnSelectionFirst>0)
							{
								COLORREF	TextColor = ::GetTextColor(hPaintDC);
								int hbr = ::GetBkMode(hPaintDC);
								COLORREF	TextBkColor = ::GetBkColor(hPaintDC);

								if(pBoxStyle)
								{
									if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
										::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
									if(pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
									{
										if(pBoxStyle->pBackGround->mask & cssMaskBackGroundColor)
										{
											if(pBoxStyle->pBackGround->color.color[0]!= RGB(255,255,255))
											{
												hbr = ::SetBkMode(hPaintDC,OPAQUE);
												TextBkColor = ::SetBkColor(hPaintDC,pBoxStyle->pBackGround->color.color[0]);
											}
										}
									}
								}
								ExtTextOut(hPaintDC,startx + m_rowInfos[i].m_rowBlocks[j].nStartX,starty +  rowheight - pTextBlock->m_nRowHeight ,0,NULL,pStr,nLenUnSelectionFirst,NULL);
								::SetTextColor(hPaintDC,TextColor);
								::SetBkColor(hPaintDC,TextBkColor);
								::SetBkMode(hPaintDC,hbr);
							}
							if(nLenUnSelectionLast>0)
							{
								int rx;
								COLORREF	TextColor = ::GetTextColor(hPaintDC);
								int hbr = ::GetBkMode(hPaintDC);
								COLORREF	TextBkColor = ::GetBkColor(hPaintDC);

								if(pBoxStyle)
								{
									if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
										::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
									if(pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
									{
										if(pBoxStyle->pBackGround->mask & cssMaskBackGroundColor)
										{
											if(pBoxStyle->pBackGround->color.color[0]!= RGB(255,255,255))
											{
												hbr = ::SetBkMode(hPaintDC,OPAQUE);
												TextBkColor = ::SetBkColor(hPaintDC,pBoxStyle->pBackGround->color.color[0]);
											}
										}
									}
								}
								rx = pTextBlock->m_pDx[m_rowInfos[i].m_rowBlocks[j].nStartCharPos + nFit - nLenUnSelectionLast - 1];
								rx -= m_rowInfos[i].m_rowBlocks[j].nStartCharPos >0 ? pTextBlock->m_pDx[m_rowInfos[i].m_rowBlocks[j].nStartCharPos - 1]:0;
								ExtTextOut(hPaintDC,startx + m_rowInfos[i].m_rowBlocks[j].nStartX + rx
									,starty +   rowheight - pTextBlock->m_nRowHeight ,0,NULL,pStr + nFit - nLenUnSelectionLast ,nLenUnSelectionLast,NULL);
								::SetTextColor(hPaintDC,TextColor);
								::SetBkColor(hPaintDC,TextBkColor);
								::SetBkMode(hPaintDC,hbr);
							}
							if(nLenSelection >0)
							{
								COLORREF color = ::SetTextColor(hPaintDC,RGB(255,255,255));
								COLORREF bkcolor =  ::SetBkColor(hPaintDC,RGB(0,0,0));
								int hbr = ::SetBkMode(hPaintDC,OPAQUE);
								int rx = 0;
								if(nLenUnSelectionFirst>0)
								{
									rx = pTextBlock->m_pDx[m_rowInfos[i].m_rowBlocks[j].nStartCharPos + nLenUnSelectionFirst - 1];
									rx -= m_rowInfos[i].m_rowBlocks[j].nStartCharPos >0 ? pTextBlock->m_pDx[m_rowInfos[i].m_rowBlocks[j].nStartCharPos - 1]:0;
								}
								ExtTextOut(hPaintDC,startx + m_rowInfos[i].m_rowBlocks[j].nStartX + rx ,starty +   rowheight - pTextBlock->m_nRowHeight ,0,NULL,pStr + nLenUnSelectionFirst ,nLenSelection,NULL);
								::SetTextColor(hPaintDC,color);
								::SetBkColor(hPaintDC,bkcolor);
								SetBkMode(hPaintDC,hbr);
							}
						}
					}else
					{
						COLORREF	TextColor = ::GetTextColor(hPaintDC);
						int hbr = ::GetBkMode(hPaintDC);
						COLORREF	TextBkColor = ::GetBkColor(hPaintDC);

						if(pBoxStyle)
						{
							if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
								::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
							if(pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
							{
								if(pBoxStyle->pBackGround->mask & cssMaskBackGroundColor)
								{
									if(pBoxStyle->pBackGround->color.color[0]!= RGB(255,255,255))
									{
										hbr = ::SetBkMode(hPaintDC,OPAQUE);
										TextBkColor = ::SetBkColor(hPaintDC,pBoxStyle->pBackGround->color.color[0]);
									}
								}
							}
						}


						ExtTextOut(hPaintDC,startx + m_rowInfos[i].m_rowBlocks[j].nStartX,starty +  rowheight - pTextBlock->m_nRowHeight ,0,NULL,pStr,nFit,NULL);
						::SetTextColor(hPaintDC,TextColor);
						::SetBkColor(hPaintDC,TextBkColor);
						::SetBkMode(hPaintDC,hbr);

					}
					hFont = (HFONT)::SelectObject(hPaintDC,hFont);
					::DeleteObject(hFont);
					
					if(i==(int)this->m_rowInfos.size() - 1 && j== m_rowInfos[i].m_rowBlocks.size() -1)
					{
						int x,y;
						x = startx + m_rowInfos[i].m_rowBlocks[j].nStartX + m_rowInfos[i].m_rowBlocks[j].nLen + 2;
						y = starty +  rowheight - 7 ;

						HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(192,192,192));
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						POINT Points[10];
						DWORD nPoints[5];
						Points[0].x = x;Points[0].y = y-1;Points[1].x = x;Points[1].y = y ;nPoints[0] = 2;
						Points[2].x = x+1;Points[2].y = y-2;Points[3].x = x+1;Points[3].y = y+1 ;nPoints[1] = 2;
						Points[4].x = x+2;Points[4].y = y-3;Points[5].x= x+2;Points[5].y = y+2 ;nPoints[2] = 2;
						Points[6].x = x+3;Points[6].y = y - 1;Points[7].x = x+6;Points[7].y = y - 1 ;nPoints[3] = 2;
						Points[8].x = x+6;Points[8].y = y-5;Points[9].x = x+6;Points[9].y = y-1 ;nPoints[4] = 2;

						PolyPolyline(hPaintDC,Points,nPoints,5);
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);
					}else if(i<(int)this->m_rowInfos.size() - 1 && j== m_rowInfos[i].m_rowBlocks.size() -1)
					{
						int x,y;
						x = startx + m_rowInfos[i].m_rowBlocks[j].nStartX + m_rowInfos[i].m_rowBlocks[j].nLen + 2 + 6;
						y = starty +  rowheight - 5 ;

						HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(192,192,192));
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						POINT Points[8];
						DWORD nPoints[4];
						Points[0].x = x - 1;Points[0].y = y;     Points[1].x = x;    Points[1].y = y ;    nPoints[0] = 2;
						Points[2].x = x - 2;Points[2].y = y - 1; Points[3].x = x+1;  Points[3].y = y - 1 ; nPoints[1] = 2;
						Points[4].x = x - 3;Points[4].y = y - 2; Points[5].x = x+2;  Points[5].y = y - 2 ;nPoints[2] = 2;
						Points[6].x = x - 1;Points[6].y = y - 8; Points[7].x = x-1;  Points[7].y = y - 2 ;nPoints[3] = 2;

						PolyPolyline(hPaintDC,Points,nPoints,4);
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);
					}
				}else if(pBlock->m_pContent->eWordDataType==_XWordDataSpanControlBlock)
				{
					int sx = startx + m_rowInfos[i].m_rowBlocks[j].nStartX;
					int sy = starty ;
					RECT rt;
					::SetRect(&rt,sx,sy,sx+pBlock->width,sy+pBlock->height);

					//if(((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass)
					/*
					xbObject *	pObject;
					xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass,pObject);
					xfControl*  pxfControl = (xfControl *)pObject;
					if(pxfControl)
					{
						RECT rect;
						::SetRect(&rect,0,0,0,0);
						pxfControl->m_pxfNode= NULL;
						pxfControl->m_dwStyle &= ~WS_VISIBLE;
						pxfControl->CreateControl(NULL,rect,GetDesktopWindow(),0,xfApp::GetApp()->GetInstance());
						::SetProp(pxfControl->m_hWnd,_T("this"),pxfControl);
						if(_tcsicmp(((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass,L"ximage")==0)
						{
							pxfControl->Initial((IXMLDOMElement*)m_pContent->param);
						}
						else
							pxfControl->Initial();

						RECT rc = m_AreaRect;
						::OffsetRect(&rc,((XWordDataControlParagraph*)m_pContent)->x,
							((XWordDataControlParagraph*)m_pContent)->y);
						::OffsetRect(&rc,ptOffset.x,ptOffset.y);

						pxfControl->DoDraw(hPaintDC,&rc);
					}*/
				}
			}
			starty +=  rowheight;
		}
		SetBkMode(hPaintDC,nBkMode);
		if(i==(int)this->m_rowInfos.size())
			nStartRow = 0;
		else
			nStartRow = i;
		return 0;
	}

	int XWordLayer::XWordParagraphBlock::DoDraw(HDC hPaintDC,POINT ptOffset)
	{
		XWordBlock::DoDraw(hPaintDC,ptOffset);

		int nStartRow=0;
		int nHeight = 0;
		return DoDraw(hPaintDC,ptOffset,nStartRow,nHeight);
	}

	int XWordLayer::XWordControlParagraph::DoDraw(HDC hPaintDC,POINT ptOffset)
	{
		XWordBlock::DoDraw(hPaintDC,ptOffset);
		if(((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass)
		{
			xbObject *	pObject;
			xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass,pObject);
			xfControl*  pxfControl = (xfControl *)pObject;
			if(pxfControl)
			{
				RECT rect;
				::SetRect(&rect,0,0,0,0);
				pxfControl->m_pxfNode= NULL;
				pxfControl->m_dwStyle &= ~WS_VISIBLE;
				pxfControl->CreateControl(NULL,rect,GetDesktopWindow(),0,xfApp::GetApp()->GetInstance());
				::SetProp(pxfControl->m_hWnd,_T("this"),pxfControl);
				if(_tcsicmp(((XWordDataControlParagraph*)m_pContent)->m_pStrControlClass,L"ximage")==0)
				{
					pxfControl->Initial((IXMLDOMElement*)m_pContent->param);
				}
				else
					pxfControl->Initial();

				RECT rc = m_AreaRect;
				::OffsetRect(&rc,((XWordDataControlParagraph*)m_pContent)->x,
					((XWordDataControlParagraph*)m_pContent)->y);
				::OffsetRect(&rc,ptOffset.x,ptOffset.y);

				pxfControl->DoDraw(hPaintDC,&rc);
			}
		}

		return 0;
	}

	int XWordLayer::XWordBlock::DoDrawBackGround(HDC hPaintDC,POINT ptOffset)
	{
		RECT	rect;
		rect = m_AreaRect;
		::OffsetRect(&rect,ptOffset.x,ptOffset.y);

		const XUtil::Css::TBoxStyle * pCssStyle = GetDocumentBlock()->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(pCssStyle)
		{
			if(pCssStyle->mask & cssMaskMargin)
			{
				if(pCssStyle->pMargin[0])
				{
					int valuex =(int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[0]);
					int valuey =(int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[0],false);
					rect.left += valuex;
					rect.right -= valuey;
					rect.top += valuex;
					rect.bottom -= valuey;
				}
				if(pCssStyle->pMargin[1])rect.left += (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[1]);
				if(pCssStyle->pMargin[2])rect.top += (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[2],false);
				if(pCssStyle->pMargin[3])rect.right -= (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[3]);
				if(pCssStyle->pMargin[4])rect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[4],false);
			}

			//draw backgound
			if(pCssStyle->mask & cssMaskBackGround)
			{
				if(pCssStyle->pBackGround->mask & cssMaskBackGroundColor)
				{
					if(pCssStyle->pBackGround->color.mask & cssMaskColor)
					{
						COLORREF color = pCssStyle->pBackGround->color.color[0];
						HBRUSH hBrush = ::CreateSolidBrush(color);
						::FillRect(hPaintDC,&rect,hBrush);
						::DeleteObject(hBrush);
					}
					if(pCssStyle->pBackGround->color.mask & cssMaskLeft && pCssStyle->pBackGround->color.colorRef[1]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskRight && pCssStyle->pBackGround->color.colorRef[3]!=XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
					}else if(pCssStyle->pBackGround->color.mask & cssMaskTop && pCssStyle->pBackGround->color.colorRef[2]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskBottom && pCssStyle->pBackGround->color.colorRef[4]!=XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
					}
				}
			}
		}		
		return 0;
	}
	int XWordLayer::XWordBlock::DoDraw(HDC hPaintDC,POINT ptOffset)
	{
		RECT	rect;
		rect = m_AreaRect;
		::OffsetRect(&rect,ptOffset.x,ptOffset.y);

		const XUtil::Css::TBoxStyle * pCssStyle = GetDocumentBlock()->m_pLayer->m_pWordData->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(pCssStyle)
		{
			if(pCssStyle->mask & cssMaskMargin)
			{
				if(pCssStyle->pMargin[0])
				{
					int valuex =(int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[0]);
					int valuey =(int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[0],false);
					rect.left += valuex;
					rect.right -= valuey;
					rect.top += valuex;
					rect.bottom -= valuey;
				}
				if(pCssStyle->pMargin[1])rect.left += (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[1]);
				if(pCssStyle->pMargin[2])rect.top += (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[2],false);
				if(pCssStyle->pMargin[3])rect.right -= (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[3]);
				if(pCssStyle->pMargin[4])rect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssStyle->pMargin[4],false);
			}

			//draw backgound
			this->DoDrawBackGround(hPaintDC,ptOffset);
		}

		POINT pt;
		pt.x = rect.left;
		pt.y = rect.top;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->DoDraw(hPaintDC,pt);
			pt.y += (*m_pChilds)[i]->m_AreaRect.bottom - (*m_pChilds)[i]->m_AreaRect.top;
		}
		return 0;
	}

	XWordLayer::XWordBlock * XWordLayer::GetObjectAtPoint(POINT pt,XWordParagraphRowInfo::RowBlock * &pRowBlock)
	{
		RECT rcContent;
		CalcContentPageRect(0,&FRect,rcContent);
		pt.x -= rcContent.left;
		pt.y -= rcContent.top;

		pRowBlock = NULL;

		if(m_pWordDocumentBlock)
		{
			//chater
			for(int i=0;i<(int)m_pWordDocumentBlock->m_pChilds->size();i++)
			{
				if(pt.y > (*m_pWordDocumentBlock->m_pChilds)[i]->m_AreaRect.bottom - (*m_pWordDocumentBlock->m_pChilds)[i]->m_AreaRect.top)
				{
					pt.y -= (*m_pWordDocumentBlock->m_pChilds)[i]->m_AreaRect.bottom - (*m_pWordDocumentBlock->m_pChilds)[i]->m_AreaRect.top;
					continue;
				}else
				{
					XWordBlock * pChater;
					pChater = (*m_pWordDocumentBlock->m_pChilds)[i];

					//Paragraph
					for(int j=0;j<(int)pChater->m_pChilds->size();j++)
					{
						if(pt.y > (*pChater->m_pChilds)[i]->m_AreaRect.bottom - (*pChater->m_pChilds)[i]->m_AreaRect.top)
						{
							pt.y -= (*pChater->m_pChilds)[i]->m_AreaRect.bottom - (*pChater->m_pChilds)[i]->m_AreaRect.top;
							continue;
						}else
						{
							XWordBlock * pParagraph;
							pParagraph = (*pChater->m_pChilds)[i];
							if(pParagraph->m_pContent->eWordDataType == _XWordDataControlParagraph)
								return pParagraph;
							else
							{
								XWordParagraphBlock * pTextParagraphBlock = dynamic_cast<XWordParagraphBlock *>(pParagraph);
								if(pTextParagraphBlock)
								{
									for(int k=0;k<(int)pTextParagraphBlock->m_rowInfos.size();k++)
									{
										if(pt.y > pTextParagraphBlock->m_rowInfos[k].nRowHeight)
										{
											pt.y -= pTextParagraphBlock->m_rowInfos[k].nRowHeight;
											continue;
										}else
										{
											int l;
											for(l=0;l<(int)pTextParagraphBlock->m_rowInfos[k].m_rowBlocks.size();l++)
											{
												if(pt.x < pTextParagraphBlock->m_rowInfos[k].m_rowBlocks[l].nStartX) break;
											}

											if(l - 1 >= 0)
											{
												pRowBlock = &pTextParagraphBlock->m_rowInfos[k].m_rowBlocks[l - 1];
												return pTextParagraphBlock->m_rowInfos[k].m_rowBlocks[l - 1].pBlock;
											}	
											else
											{
												//none object find
												return pParagraph;
											}
										}
										return pParagraph;
									}
								}else
									return pParagraph;
							}
							return pParagraph;
						}
						return pChater;
					}
				}
			}
		}
		return NULL;
	}
	
	int XWordLayer::PosBlockIndex(int &y,std::vector<XWordBlock * > * pItems)
	{
		int i;
		for(i=0;i<(int)pItems->size();i++)
		{
			if(y > (*pItems)[i]->m_AreaRect.bottom - (*pItems)[i]->m_AreaRect.top)
				y -= (*pItems)[i]->m_AreaRect.bottom - (*pItems)[i]->m_AreaRect.top;
			else
				break;
		}
		return i;
	}

	int XWordLayer::PosRowIndex(int &y,std::vector<XWordParagraphRowInfo> * pItems,int nStartRow)
	{
		int i;
		for(i=nStartRow;i<(int)pItems->size();i++)
		{
			if(y > (*pItems)[i].nRowHeight)
				y -= (*pItems)[i].nRowHeight;
			else
				break;
		}
		return i;
	}
	
	int XWordLayer::PosCharIndexEx(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int & PosX,int &nCol)
	{
		int i = 0;
		int CharLeftX = 0;
		int CharRightX = 0;
		nCol = 1;
		for(i=0;i<(int)pRowInfo->m_rowBlocks.size() - 1 ;i++)
		{
			if(x < pRowInfo->m_rowBlocks[i].nStartX + pRowInfo->m_rowBlocks[i].nLen)break;
			nCol += pRowInfo->m_rowBlocks[i].nFited;
		}

		int firstX = 0;
		int j;
		if(pRowInfo->m_rowBlocks[i].nStartCharPos > 0)
			firstX = ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[ pRowInfo->m_rowBlocks[i].nStartCharPos -1];
		x -= pRowInfo->m_rowBlocks[i].nStartX;
		for(j=0;j<pRowInfo->m_rowBlocks[i].nFited;j++)
		{
			if(x <((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j + pRowInfo->m_rowBlocks[i].nStartCharPos] - firstX)break;
		}
		if(j==pRowInfo->m_rowBlocks[i].nFited)
		{
			nPosChar = pRowInfo->m_rowBlocks[i].nStartCharPos + pRowInfo->m_rowBlocks[i].nFited;
			PosX = pRowInfo->m_rowBlocks[i].nStartX;
			if(j>0)PosX +=((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j +  pRowInfo->m_rowBlocks[i].nStartCharPos - 1] - firstX;
			nCol += j;
			nCol -= pRowInfo->m_rowBlocks[i].nZWFited;
			return i;
		}
		CharRightX = ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j +  pRowInfo->m_rowBlocks[i].nStartCharPos] - firstX;
		if(j<=0) 
			CharLeftX = 0;
		else
			CharLeftX = ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j + pRowInfo->m_rowBlocks[i].nStartCharPos - 1] - firstX;
		nPosChar = j + pRowInfo->m_rowBlocks[i].nStartCharPos;
		if(x - CharLeftX > CharRightX - x)
		{
			nPosChar ++;
			PosX = CharRightX + pRowInfo->m_rowBlocks[i].nStartX;
		}else
		{
			PosX = CharLeftX + pRowInfo->m_rowBlocks[i].nStartX;
		}
		nCol += nPosChar - pRowInfo->m_rowBlocks[i].nStartCharPos;

		return i;
	}
	int XWordLayer::PosCharIndex(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int & PosX)
	{
		int i = 0;
		int CharLeftX = 0;
		int CharRightX = 0;
		for(i=0;i<(int)pRowInfo->m_rowBlocks.size() - 1 ;i++)
		{
			if(x < pRowInfo->m_rowBlocks[i+1].nStartX)break;
		}

		int firstX = 0;
		int j;
		if(pRowInfo->m_rowBlocks[i].nStartCharPos > 0)
			firstX = ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[ pRowInfo->m_rowBlocks[i].nStartCharPos -1];
		x -= pRowInfo->m_rowBlocks[i].nStartX;
		for(j=0;j<pRowInfo->m_rowBlocks[i].nFited;j++)
		{
			if(x <((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j + pRowInfo->m_rowBlocks[i].nStartCharPos] - firstX)break;
		}
		CharRightX = pRowInfo->m_rowBlocks[i].nStartX + ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j +  pRowInfo->m_rowBlocks[i].nStartCharPos] - firstX;
		if(j<=0) 
			CharLeftX = pRowInfo->m_rowBlocks[i].nStartX;
		else
			CharLeftX = pRowInfo->m_rowBlocks[i].nStartX + ((XWordLayer::XWordTextBlock *)pRowInfo->m_rowBlocks[i].pBlock)->m_pDx[j + pRowInfo->m_rowBlocks[i].nStartCharPos - 1] - firstX;
		nPosChar = j + pRowInfo->m_rowBlocks[i].nStartCharPos;
		if(x - CharLeftX > CharRightX - x)
		{
			nPosChar ++;
			PosX = CharRightX;
		}else
		{
			PosX = CharLeftX;
		}

		return i;
	}


	bool XWordLayer::CalcCursorInfoAtPoint(POINT pt,XWordCursorInfo & CursorInfo)
	{
		pt.x += this->m_ptScroll.x;
		pt.y += this->m_ptScroll.y;

		RECT rcContent;
		int i=0,nPage=0;
		for(i=0;i<this->m_nPage;i++)
		{
			CalcContentPageRect(i,&FRect,rcContent);
			InflateRect(&rcContent,1,1);
			if(PtInRect(&rcContent,pt))break;
		}
		if(i==m_nPage)return false;
		InflateRect(&rcContent,-1,-1); //restore rcContent
		nPage = i+1;

		this->InitCursorInfo(CursorInfo);

		int nIndex;
		int x = pt.x - rcContent.left;
		int y = pt.y - rcContent.top;


		if(x > rcContent.right - rcContent.left) return false;
		if(x<0) return false;

		int nParaRow;
		XWordContentParagraph * pParaBlock;
		if(!GetFirstRowOfPage(nPage,nParaRow,pParaBlock)) return false;
		XWordBlock * pChater = pParaBlock->m_pBlockParent;
		XWordParagraphBlock * pTextParagraphBlock;
		while(true)
		{
			if(pParaBlock->m_AreaRect.bottom - pParaBlock->m_AreaRect.top >= y)
			{
				pTextParagraphBlock = dynamic_cast<XWordParagraphBlock *>(pParaBlock);
				if(!pTextParagraphBlock)
					return false;
				else
				{
					nIndex = PosRowIndex(y,&pTextParagraphBlock->m_rowInfos,nParaRow);
					if(nIndex >= (int)pTextParagraphBlock->m_rowInfos.size())
					{
						//pos cursor at last row end
						return false; //should modify;
					}else
					{
						CursorInfo.nRow = GetParahraphRow(pTextParagraphBlock) + nIndex + 1;

						int dx = 0;
						if(pTextParagraphBlock->m_pContent->align & DT_CENTER)
							dx = pTextParagraphBlock->m_rowInfos[nIndex].nBala/2;
						if(pTextParagraphBlock->m_pContent->align & DT_RIGHT)
							dx = pTextParagraphBlock->m_rowInfos[nIndex].nBala;
						if(x <dx) dx = x;

						int posX;
						int nPosChar;
						int nBlock;
						nBlock = PosCharIndexEx(x - dx,&pTextParagraphBlock->m_rowInfos[nIndex],nPosChar,posX,CursorInfo.nCol);
						if(pt.y - y + pTextParagraphBlock->m_rowInfos[nIndex].m_rowBlocks[nBlock].nRowheight >rcContent.bottom)
						{
							return this->CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol);
						}
						
						

						CursorInfo.CursorHeight = pTextParagraphBlock->m_rowInfos[nIndex].m_rowBlocks[nBlock].nRowheight;
						CursorInfo.XPosCursur = rcContent.left + posX + dx;
						CursorInfo.YPosCursur = pt.y - y + pTextParagraphBlock->m_rowInfos[nIndex].nRowHeight - CursorInfo.CursorHeight;
						return true;
					}
				}
			}else
				y -= (pParaBlock->m_AreaRect.bottom - pParaBlock->m_AreaRect.top + 1);

			nIndex = pParaBlock->GetIndexFromParent();
			if(nIndex < (int)pChater->m_pChilds->size() - 1)
			{
				pParaBlock =  dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[nIndex+1]);
				if(pParaBlock->m_pContent->m_bBreakPage) return false;
			}else
			{
FindNextChater:
				nIndex = pChater->GetIndexFromParent();
				if(nIndex < (int)pChater->m_pBlockParent->m_pChilds->size() - 1)
				{
					pChater = (*pChater->m_pBlockParent->m_pChilds)[nIndex+1];
					if(pChater->m_pContent->m_bBreakPage) return false;
					if(pChater->m_pChilds->size() < 1) goto FindNextChater;
					pParaBlock =  dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[0]);
					if(pParaBlock->m_pContent->m_bBreakPage) return false;
				}else
				{
					//should document last
					return false;
				}
			}
		}
		return false;
	}

	int  XWordLayer::CalcBlockCol(XWordParagraphBlock * pPara,int nParaRow,int nCol,int &nBlock,int &nCharPos,bool bRight)
	{
		XWordTextBlock * pBlock;
		if(nParaRow <0) return -1;
		for(int k=0;k<(int)pPara->m_rowInfos[nParaRow].m_rowBlocks.size();k++)
		{
			pBlock = dynamic_cast<XWordTextBlock *>(pPara->m_rowInfos[nParaRow].m_rowBlocks[k].pBlock);
			if(!pBlock)continue;
			if(nCol <= pPara->m_rowInfos[nParaRow].m_rowBlocks[k].nFited + 1)
			{
				if(bRight && nCol ==pPara->m_rowInfos[nParaRow].m_rowBlocks[k].nFited + 1 && k<(int)pPara->m_rowInfos[nParaRow].m_rowBlocks.size() - 1 )
				{
					nCharPos = 0;
					nBlock = k + 1;
					return 1;

				}else
				{
					nBlock = k;
					nCharPos = nCol - 1 + pPara->m_rowInfos[nParaRow].m_rowBlocks[k].nStartCharPos;
					return 1;
				}
			}
			else
				nCol -= pPara->m_rowInfos[nParaRow].m_rowBlocks[k].nFited;
		}
		if(nCol > 0)
		{
			return 0;
		}
		return -1;
	}

	int  XWordLayer::CalcParagraphRow(int nRow,XWordParagraphBlock * &pPara,int &nParaRow)
	{
		XWordBlock * pChater;
		XWordParagraphBlock * pParagraph;
		int Row = nRow;
		for(int p=0;p<(int)m_pWordDocumentBlock->m_pChilds->size();p++)
		{
			pChater = dynamic_cast<XWordChapterBlock *>((*m_pWordDocumentBlock->m_pChilds)[p]);
			for(int i=0;i<(int)pChater->m_pChilds->size();i++)
			{
				pParagraph = dynamic_cast<XWordParagraphBlock *>((*pChater->m_pChilds)[i]);
				if(!pParagraph) continue;
				if(Row <= (int)pParagraph->m_rowInfos.size())
				{
					pPara = pParagraph;
					nParaRow = Row - 1;
					return 1;
				}else
				{
					pPara = pParagraph;
					nParaRow = (int)pParagraph->m_rowInfos.size() - 1;
					Row -= (int)pParagraph->m_rowInfos.size();
				}
			}
		}
		pPara = NULL;
		nParaRow = 0;
		//DebugBreak();
		return 0;
	}

	bool XWordLayer::CalcCursorInfo(XWordCursorInfo & CursorInfo,int nRow,int nCol)
	{
		XWordParagraphBlock * pParagraph;
		int nParaRowEx;
		this->CalcParagraphRow(nRow,pParagraph,nParaRowEx);
		if(!pParagraph)
		{
			//should last;
			return false;
		}
		RECT rcContent;
		int nPage = this->GetPageOfParagraph( pParagraph,nParaRowEx);
		CalcContentPageRect(nPage - 1,&FRect,rcContent);

		int y=0;
		int x=0;
		
		int nParaRow = 0;
		XWordContentParagraph * pParaBlock;
		if(!GetFirstRowOfPage(nPage,nParaRow,pParaBlock)) return false;
		if(!pParaBlock) return false;
		XWordBlock * pChater = pParaBlock->m_pBlockParent;

		while(pParagraph != pParaBlock) 
		{
			y += pParaBlock->m_AreaRect.bottom - pParaBlock->m_AreaRect.top +1;
			nParaRow = 0;
			int nIndex = pParaBlock->GetIndexFromParent();
			if( nIndex < (int)pChater->m_pChilds->size() - 1)
				pParaBlock = dynamic_cast<XWordContentParagraph*>((*pChater->m_pChilds)[nIndex + 1]);
			else
			{
FindNextChater:
				nIndex = pChater->GetIndexFromParent();
				if(nIndex < (int)pChater->m_pBlockParent->m_pChilds->size() - 1)
				{
					pChater = (*pChater->m_pBlockParent->m_pChilds)[nIndex+1];
					if(pChater->m_pChilds->size() < 1) goto FindNextChater;
					pParaBlock =  dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[0]);
				}else
					break; //should error
			}
		}

		XWordTextBlock * pBlock;
		for(int j = nParaRow;j<=nParaRowEx;j++)
		{
			if(j==nParaRowEx)
			{
				int rCol = 0 ;
				x = 0;
				CursorInfo.XPosCursur = x;
				CursorInfo.CursorHeight = pParagraph->m_rowInfos[j].nRowHeight;
				CursorInfo.YPosCursur = y;
				CursorInfo.nRow = nRow;
				CursorInfo.nCol = nCol;
				for(int k=0;k<(int)pParagraph->m_rowInfos[j].m_rowBlocks.size();k++)
				{
					pBlock = dynamic_cast<XWordTextBlock *>(pParagraph->m_rowInfos[j].m_rowBlocks[k].pBlock);
					if(!pBlock)continue;
					int pCol = rCol;
					rCol += pParagraph->m_rowInfos[j].m_rowBlocks[k].nFited;
					rCol -= pParagraph->m_rowInfos[j].m_rowBlocks[k].nZWFited;
					if(rCol + 1 >= nCol && nCol > 0)
					{

						int nBlockPrevX = 0;
						if(pParagraph->m_rowInfos[j].m_rowBlocks[k].nStartCharPos > 1)
							nBlockPrevX = pBlock->m_pDx[pParagraph->m_rowInfos[j].m_rowBlocks[k].nStartCharPos - 1];
						if(nCol  - 2 - pCol >= 0)
							CursorInfo.XPosCursur = x + pBlock->m_pDx[pParagraph->m_rowInfos[j].m_rowBlocks[k].nStartCharPos + (nCol - pCol - 2)] - nBlockPrevX;
						CursorInfo.CursorHeight = pParagraph->m_rowInfos[j].m_rowBlocks[k].nRowheight;
						CursorInfo.YPosCursur = y + pParagraph->m_rowInfos[j].nRowHeight - pParagraph->m_rowInfos[j].m_rowBlocks[k].nRowheight;

						break;
					}else
					{
						x += pParagraph->m_rowInfos[j].m_rowBlocks[k].nLen;
						CursorInfo.XPosCursur = x;
						CursorInfo.CursorHeight = pParagraph->m_rowInfos[j].m_rowBlocks[k].nRowheight;
						CursorInfo.YPosCursur = y + pParagraph->m_rowInfos[j].nRowHeight - pParagraph->m_rowInfos[j].m_rowBlocks[k].nRowheight;
					}
				}
				if(nCol < 0)
					CursorInfo.nCol = rCol + 1;
				
				CursorInfo.XPosCursur += rcContent.left;
				CursorInfo.YPosCursur += rcContent.top;

				int dx = 0;
				if(pParagraph->m_pContent->align & DT_CENTER)
					dx = pParagraph->m_rowInfos[j].nBala/2;
				if(pParagraph->m_pContent->align & DT_RIGHT)
					dx = pParagraph->m_rowInfos[j].nBala;
				CursorInfo.XPosCursur += dx;
				return true;
			}
			y += pParagraph->m_rowInfos[j].nRowHeight;
		}
		return false;
	}

	int  XWordLayer::GetPageOfParagraph(XWordParagraphBlock * pParagraph,int nParaRow)
	{
		int nPage=1;
		XWordContentParagraph*	pParaBlock;
		int nStartParaRow;
		int i=0;
		for(i=0;i<this->m_nPage;i++)
		{
			if(!GetFirstRowOfPage(i+1,nStartParaRow,pParaBlock))continue;
			if(pParagraph==pParaBlock && nParaRow < nStartParaRow) return i;
			if(pParaBlock && pParaBlock->m_pBlockParent->GetIndexFromParent() > pParagraph->m_pBlockParent->GetIndexFromParent()) return i;
			if(pParaBlock && pParaBlock->m_pBlockParent->GetIndexFromParent() == pParagraph->m_pBlockParent->GetIndexFromParent() &&
				pParaBlock->GetIndexFromParent() > pParagraph->GetIndexFromParent()) return i;
		}
		return i;
	}

	int  XWordLayer::GetPageOfBlock(XWordBlock * pBlock,int nPosChar)
	{
		int nPage=1;
		XWordContentParagraph*	pParaBlock;
		XWordParagraphBlock*	pParaText;
		int nStartRow;
		int i=0;
		for(i=0;i<this->m_nPage;i++)
		{
			if(!GetFirstRowOfPage(i+1,nStartRow,pParaBlock))continue;
			pParaText = dynamic_cast<XWordParagraphBlock*>(pParaBlock);
			if(pParaText)
			{
				int nRet = BlockCmp(pBlock,pParaText->m_rowInfos[nStartRow].m_rowBlocks[0].pBlock);
				if(nRet < 0) 
				{
					nPage = i ;
					break;
				} else if(nRet==0)
				{
					if(pParaText->m_rowInfos[nStartRow].m_rowBlocks[0].nStartCharPos  > nPosChar)
					{
						nPage = i ;
						break;
					}else if(pParaText->m_rowInfos[nStartRow].m_rowBlocks[0].nStartCharPos + pParaText->m_rowInfos[nStartRow].m_rowBlocks[0].nFited > nPosChar)
					{
						nPage = i + 1;
						break;
					}
				}
			}else
			{
				if(pParaBlock->m_pBlockParent->GetIndexFromParent() > pBlock->m_pBlockParent->m_pBlockParent->GetIndexFromParent())
				{
					nPage = i;
					break;
				}else if(pParaBlock->m_pBlockParent->GetIndexFromParent() == pBlock->m_pBlockParent->m_pBlockParent->GetIndexFromParent())
				{
					if(pParaBlock->GetIndexFromParent() > pBlock->m_pBlockParent->GetIndexFromParent())
					{
						nPage = i;
						break;
					}
				}
			}
		}
		if(i>=m_nPage) nPage = i;

		return nPage;
	}
	bool XWordLayer::CalcCursorInfoEx(XWordCursorInfo & CursorInfo,XWordBlock * pBlock,int nPosChar)
	{
		/*
		XWordParagraphBlock * pParagraph = dynamic_cast<XWordParagraphBlock *>(pBlock->m_pBlockParent);
		if(!pParagraph) return false;
		if(nPosChar <0) return false;

		RECT rcContent;
		int nPage = GetPageOfBlock( pBlock,nPosChar);
		CalcContentPageRect(nPage - 1,&FRect,rcContent);

		int y=0;
		int x=0;
		bool bFound=false;
		
		int nParaRow = 0;
		XWordContentParagraph * pParaBlock;
		if(!GetFirstRowOfPage(nPage,nParaRow,pParaBlock)) return false;
		XWordBlock * pChater = pParaBlock->m_pBlockParent;

		while(pParagraph != pParaBlock) 
		{
			y += pParaBlock->m_AreaRect.bottom - pParaBlock->m_AreaRect.top +1;
			nParaRow = 0;
			int nIndex = pParaBlock->GetIndexFromParent();
			if( nIndex < (int)pChater->m_pChilds->size() - 1)
				pParaBlock = dynamic_cast<XWordContentParagraph*>((*pChater->m_pChilds)[nIndex + 1]);
			else
			{
FindNextChater:
				nIndex = pChater->GetIndexFromParent();
				if(nIndex < (int)pChater->m_pBlockParent->m_pChilds->size() - 1)
				{
					pChater = (*pChater->m_pBlockParent->m_pChilds)[nIndex+1];
					if(pChater->m_pChilds->size() < 1) goto FindNextChater;
					pParaBlock =  dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[0]);
				}else
					break; //should error
			}
		}

		for(int row=nParaRow;row < (int)pParagraph->m_rowInfos.size();row++)
		{
			if(bFound)
			{
				x = 0;
				if((int)pParagraph->m_rowInfos[row].m_rowBlocks.size() >0 && pParagraph->m_rowInfos[row].m_rowBlocks[0].pBlock==pBlock)
				{
					if(nPosChar <= pParagraph->m_rowInfos[row].m_rowBlocks[0].nStartCharPos + pParagraph->m_rowInfos[row].m_rowBlocks[0].nFited)
					{
						CursorInfo.m_pParagraphBlock = pParagraph;
						CursorInfo.bControlParagraph = false;
						CursorInfo.nPosChar = nPosChar;
						CursorInfo.nRow = row;
						CursorInfo.nBlock = 0;
						CursorInfo.CursorHeight = pParagraph->m_rowInfos[row].m_rowBlocks[0].nRowheight;
						CursorInfo.YPosCursur = y + pParagraph->m_rowInfos[row].nRowHeight - pParagraph->m_rowInfos[row].m_rowBlocks[0].nRowheight;

						CursorInfo.XPosCursur = x + (nPosChar >0 ?dynamic_cast<XWordTextBlock *>(pBlock)->m_pDx[nPosChar - 1]:0);
						if(pParagraph->m_rowInfos[row].m_rowBlocks[0].nStartCharPos > 0)
							CursorInfo.XPosCursur -= dynamic_cast<XWordTextBlock *>(pBlock)->m_pDx[pParagraph->m_rowInfos[row].m_rowBlocks[0].nStartCharPos - 1];

						CursorInfo.XPosCursur += rcContent.left;
						CursorInfo.YPosCursur += rcContent.top;

						int dx = 0;
						if(pParagraph->m_pContent->align & DT_CENTER)
							dx = pParagraph->m_rowInfos[row].nBala/2;
						if(pParagraph->m_pContent->align & DT_RIGHT)
							dx = pParagraph->m_rowInfos[row].nBala;
						CursorInfo.XPosCursur += dx;

						return true;
					}
				}else
					break;
			}
			x = 0;
			for(int i=0;i<(int)pParagraph->m_rowInfos[row].m_rowBlocks.size();i++)
			{
				if(pParagraph->m_rowInfos[row].m_rowBlocks[i].pBlock != pBlock)
				{
					x += pParagraph->m_rowInfos[row].m_rowBlocks[i].nLen;
					continue;
				}else
				{
					bFound = true;
					if(nPosChar <= pParagraph->m_rowInfos[row].m_rowBlocks[i].nFited)
					{
						CursorInfo.m_pParagraphBlock = pParagraph;
						CursorInfo.bControlParagraph = false;
						CursorInfo.nPosChar = nPosChar;
						CursorInfo.nRow = row;
						CursorInfo.nBlock = i;
						CursorInfo.CursorHeight = pParagraph->m_rowInfos[row].m_rowBlocks[i].nRowheight;

						CursorInfo.XPosCursur = nPosChar>0? x + dynamic_cast<XWordTextBlock *>(pBlock)->m_pDx[nPosChar - 1]:x;
						CursorInfo.YPosCursur = y + pParagraph->m_rowInfos[row].nRowHeight - pParagraph->m_rowInfos[row].m_rowBlocks[i].nRowheight;

						CursorInfo.XPosCursur += rcContent.left;
						CursorInfo.YPosCursur += rcContent.top;

						int dx = 0;
						if(pParagraph->m_pContent->align & DT_CENTER)
							dx = pParagraph->m_rowInfos[row].nBala/2;
						if(pParagraph->m_pContent->align & DT_RIGHT)
							dx = pParagraph->m_rowInfos[row].nBala;
						CursorInfo.XPosCursur += dx;

						return true;
					}else
						break;
				}
			}
			y += pParagraph->m_rowInfos[row].nRowHeight;
		}*/
		return false;
	}
	int XWordLayer::GetBlockAndCharPos(int nRow,int nCol,XWordBlock * &pBlock,int &nCharPos,bool bRight)
	{
		XWordParagraphBlock * pPara;
		int nParaRow;
		int nBlock;

		this->CalcParagraphRow(nRow,pPara,nParaRow);
		if(!pPara) return -1;
		this->CalcBlockCol(pPara,nParaRow,nCol,nBlock,nCharPos,bRight);
		pBlock = pPara->m_rowInfos[nParaRow].m_rowBlocks[nBlock].pBlock;
		return 1;
	}
	
	XWordLayer::XWordParagraphBlock * XWordLayer::GetParagraphByRow(int nRow)
	{
		XWordParagraphBlock * pPara;
		int nParaRow;
		this->CalcParagraphRow(nRow,pPara,nParaRow);
		return pPara;
	}

	XWordLayer::XWordBlock * XWordLayer::GetBlock(int nRow,int nCol)
	{
		XWordParagraphBlock * pPara;
		int nParaRow;
		int nBlock;
		int nCharPos;

		if(this->CalcParagraphRow(nRow,pPara,nParaRow)<0) return NULL;
		if(this->CalcBlockCol(pPara,nParaRow,nCol,nBlock,nCharPos)<0) return NULL;
		return pPara->m_rowInfos[nParaRow].m_rowBlocks[nBlock].pBlock;
	}
	
	XWordLayer::XWordParagraphRowInfo::RowBlock * XWordLayer::GetRowBlock(int nRow,int nCol)
	{
		XWordParagraphBlock * pPara;
		int nParaRow;
		int nBlock;
		int nCharPos;

		this->CalcParagraphRow(nRow,pPara,nParaRow);
		
		if(!pPara) return NULL;
		
		this->CalcBlockCol(pPara,nParaRow,nCol,nBlock,nCharPos);
		return &pPara->m_rowInfos[nParaRow].m_rowBlocks[nBlock];
	}


	int  XWordLayer::GetChaterRow(XWordBlock * pChater)
	{
		int nRow = 0;
		for(int i=0;i<(int)this->m_pWordDocumentBlock->m_pChilds->size();i++)
		{
			if( (*m_pWordDocumentBlock->m_pChilds)[i]==pChater)break;
			nRow += this->GetChaterRowCount((*m_pWordDocumentBlock->m_pChilds)[i]);
		}
		return nRow + 1;
	}
	int  XWordLayer::GetChaterRowCount(XWordBlock * pChater)
	{
		int tRow = 0;
		for(int i=0;i<(int)pChater->m_pChilds->size();i++)
		{
			tRow += this->GetParahraphRowCount((*pChater->m_pChilds)[i]);
		}
		return tRow;
	}
	int  XWordLayer::GetParahraphRow(XWordBlock * pParahraph)
	{
		int tRow = 0;
		int nIndex = pParahraph->m_pBlockParent->GetIndexFromParent();
		for(int i=0;i<nIndex;i++)
		{
			tRow += this->GetChaterRowCount((*m_pWordDocumentBlock->m_pChilds)[i]);
		}
		nIndex = pParahraph->GetIndexFromParent();
		XWordParagraphBlock* pPara;
		for(int i=0;i<nIndex;i++)
		{
			pPara = dynamic_cast<XWordParagraphBlock*>((*pParahraph->m_pBlockParent->m_pChilds)[i]);
			if(pPara)tRow += (int)pPara->m_rowInfos.size();
		}
		return tRow++;
	}
	int  XWordLayer::GetParahraphRowCount(XWordBlock * pParahraph)
	{
		XWordParagraphBlock * pPara = dynamic_cast<XWordParagraphBlock *>(pParahraph);
		return (int)pPara->m_rowInfos.size();
	}

	int  XWordLayer::GetBlockRowCol(XWordBlock * pBlock,int &nStartRow,int &nStartCol,int &nEndRow,int &nEndCol)
	{
		XWordParagraphBlock * pParagraph;
		int nRow;
		int nCol = 1;
		int pCol;
		bool bFind = false;

		pParagraph = dynamic_cast<XWordParagraphBlock *>(pBlock->m_pBlockParent);
		nRow = this->GetParahraphRow(pParagraph);
		for(int k=0;k<(int)pParagraph->m_rowInfos.size();k++)
		{
			pCol = nCol;
			nCol = 1;
			for(int m=0;m<(int)pParagraph->m_rowInfos[k].m_rowBlocks.size();m++)
			{
				if(pParagraph->m_rowInfos[k].m_rowBlocks[m].pBlock==pBlock && !bFind)
				{
					nStartRow = nRow;
					nStartCol = nCol;
					bFind = true;
				}else
				{
					nEndRow = nRow - 1;
					nEndCol = pCol ;
					return 1;
				}
				nCol += pParagraph->m_rowInfos[k].m_rowBlocks[m].nFited;
			}
			nRow++;
		}
		nEndRow = nRow - 1;
		nEndCol = pCol ;
		return 1;
	}

	void XWordLayer::ShowCurrentCaret(bool bShow)
	{
		if(!bShow)
			::HideCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd);
		else
		{
			CreateCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd, NULL, 2, m_CursorInfo.CursorHeight);
			SetCaretPos(m_CursorInfo.XPosCursur - m_ptScroll.x,m_CursorInfo.YPosCursur - m_ptScroll.y);  
			ShowCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd);
		}
	}

	int XWordLayer::GetColCount(int nRow)
	{
		XWordBlock * pChater;
		XWordParagraphBlock * pParagraph;
		XWordTextBlock * pBlock ;
		for(int p=0;p<(int)m_pWordDocumentBlock->m_pChilds->size();p++)
		{
			pChater = dynamic_cast<XWordChapterBlock *>((*m_pWordDocumentBlock->m_pChilds)[p]);
			for(int i=0;i<(int)pChater->m_pChilds->size();i++)
			{
				pParagraph = dynamic_cast<XWordParagraphBlock *>((*pChater->m_pChilds)[i]);
				if(!pParagraph) continue;
				if(nRow <= (int)pParagraph->m_rowInfos.size())
				{
					int rCol = 0 ;
					int j = nRow - 1;
					for(int k=0;k<(int)pParagraph->m_rowInfos[j].m_rowBlocks.size();k++)
					{
						pBlock = dynamic_cast<XWordTextBlock *>(pParagraph->m_rowInfos[j].m_rowBlocks[k].pBlock);
						if(!pBlock)continue;
						rCol += pParagraph->m_rowInfos[j].m_rowBlocks[k].nFited - pParagraph->m_rowInfos[j].m_rowBlocks[k].nZWFited ;
					}
					return rCol + 1;
				}else
					nRow -= (int)pParagraph->m_rowInfos.size();
			}
		}
		return  1;
	}

	int XWordLayer::GetRowCount()
	{
		XWordBlock * pChater;
		XWordParagraphBlock * pParagraph;
		int nRow = 0;
		for(int p=0;p<(int)m_pWordDocumentBlock->m_pChilds->size();p++)
		{
			pChater = dynamic_cast<XWordChapterBlock *>((*m_pWordDocumentBlock->m_pChilds)[p]);
			for(int i=0;i<(int)pChater->m_pChilds->size();i++)
			{
				pParagraph = dynamic_cast<XWordParagraphBlock *>((*pChater->m_pChilds)[i]);
				if(!pParagraph) continue;
				nRow += (int)pParagraph->m_rowInfos.size();
			}
		}
		return  nRow;
	}

	bool XWordLayer::CalcLeftCursorInfo(XWordCursorInfo & CursorInfo)
	{
		if(CursorInfo.nCol > 1)
		{
			CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol - 1);
			return true;
		}
		else
		{
			if(CursorInfo.nRow > 1)
			{
				CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,-1);
				return true;
			}
			else
				return false;
		}
	}

	bool XWordLayer::CalcRightCursorInfo(XWordCursorInfo & CursorInfo)
	{
		if(CursorInfo.nCol < this->GetColCount(CursorInfo.nRow) )
		{
			CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol + 1);
			return true;
		}
		else
		{
			if(CursorInfo.nRow < GetRowCount())
			{
				CalcCursorInfo(CursorInfo,CursorInfo.nRow + 1,1);
				return true;
			}
			else
				return false;
		}
		return false;
	}
	bool XWordLayer::CalcUpCursorInfo(XWordCursorInfo & CursorInfo)
	{
		XWordParagraphBlock * pTextParagraphBlock ;
		int nParaRow;
		if(CursorInfo.nRow < 2) return false;
		CalcParagraphRow(CursorInfo.nRow - 1,pTextParagraphBlock,nParaRow);

		int dx = 0;
		if(pTextParagraphBlock->m_pContent->align & DT_CENTER)
			dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala/2;
		if(pTextParagraphBlock->m_pContent->align & DT_RIGHT)
			dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala;

		RECT rcContent;
		int x;
		int nPosChar,posX, nCol;
		this->CalcContentPageRect(0,&this->FRect,rcContent);
		x = CursorInfo.XPosCursur + m_ptScroll.x - rcContent.left - dx;
		this->PosCharIndexEx(x,&pTextParagraphBlock->m_rowInfos[nParaRow],nPosChar,posX, nCol);
		return this->CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,nCol);
	}
	bool XWordLayer::CalcDownCursorInfo(XWordCursorInfo & CursorInfo)
	{
		XWordParagraphBlock * pTextParagraphBlock ;
		int nParaRow;
		if(CursorInfo.nRow >= this->GetRowCount()) return false;
		CalcParagraphRow(CursorInfo.nRow + 1,pTextParagraphBlock,nParaRow);

		int dx = 0;
		if(pTextParagraphBlock->m_pContent->align & DT_CENTER)
			dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala/2;
		if(pTextParagraphBlock->m_pContent->align & DT_RIGHT)
			dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala;

		RECT rcContent;
		int x;
		int nPosChar,posX, nCol;
		this->CalcContentPageRect(0,&this->FRect,rcContent);
		x = CursorInfo.XPosCursur + m_ptScroll.x - rcContent.left - dx;
		this->PosCharIndexEx(x,&pTextParagraphBlock->m_rowInfos[nParaRow],nPosChar,posX, nCol);
		return this->CalcCursorInfo(CursorInfo,CursorInfo.nRow + 1,nCol);
	}

	bool XWordLayer::XWordBlock::GetRects(std::vector<RECT> &rects)
	{
		return false;
	}

	bool XWordLayer::XWordSpanBlock::GetRects(std::vector<RECT> &rects)
	{
		XWordParagraphBlock *pParagraph = dynamic_cast<XWordParagraphBlock *>(m_pBlockParent);
		if(!pParagraph) return false;

		int y=0;
		int x=0;
		bool bFound=false;
		std::vector<RECT> pRects;
		m_pBlockParent->GetRects(pRects);
		for(int row=0;row < (int)pParagraph->m_rowInfos.size();row++)
		{
			if(bFound)
			{
				if((int)pParagraph->m_rowInfos[row].m_rowBlocks.size() >0 && pParagraph->m_rowInfos[row].m_rowBlocks[0].pBlock==this)
				{
					RECT rc;
					rc.left = x;
					rc.top = y;
					rc.right = rc.left + pParagraph->m_rowInfos[row].m_rowBlocks[0].nLen - 1;
					rc.bottom = rc.top + pParagraph->m_rowInfos[row].nRowHeight - 1;
					::OffsetRect(&rc,pRects[0].left,pRects[0].top);
					rects.push_back(rc);
				}else
					break;
			}

			for(int i=0;i<(int)pParagraph->m_rowInfos[row].m_rowBlocks.size();i++)
			{
				if(!bFound && pParagraph->m_rowInfos[row].m_rowBlocks[i].pBlock != this)
				{
					x += pParagraph->m_rowInfos[row].m_rowBlocks[i].nLen;
					continue;
				}else
				{
					RECT rc;
					rc.left = x;
					rc.top = y;
					rc.right = rc.left + pParagraph->m_rowInfos[row].m_rowBlocks[i].nLen - 1;
					rc.bottom = rc.top + pParagraph->m_rowInfos[row].nRowHeight - 1;
					::OffsetRect(&rc,pRects[0].left,pRects[0].top);
					rects.push_back(rc);
					bFound = true;
					break;
				}
			}
			y += pParagraph->m_rowInfos[row].nRowHeight;
		}
		if(bFound) return true;
		return false;
	}

	bool XWordLayer::XWordContentParagraph::GetRects(std::vector<RECT> &rects)
	{
		int x=0,y = 0;
		rects.clear();
		for(int i=0;i<(int)m_pBlockParent->m_pChilds->size();i++)
		{
			if((*m_pBlockParent->m_pChilds)[i]==this)break;
			y += (*m_pBlockParent->m_pChilds)[i]->m_AreaRect.bottom - (*m_pBlockParent->m_pChilds)[i]->m_AreaRect.top + 1;
		}
		RECT	rect;
		rect = this->m_AreaRect;
		::OffsetRect(&rect,x,y);

		std::vector<RECT> pRects;
		m_pBlockParent->GetRects(pRects);
		::OffsetRect(&rect,pRects[0].left,pRects[0].top);

		rects.push_back(rect);

		return true;
	}

	bool XWordLayer::XWordChapterBlock::GetRects(std::vector<RECT> &rects)
	{
		int x=0,y = 0;
		rects.clear();
		for(int i=0;i<(int)m_pBlockParent->m_pChilds->size();i++)
		{
			if((*m_pBlockParent->m_pChilds)[i]==this)break;
			y += (*m_pBlockParent->m_pChilds)[i]->m_AreaRect.bottom - (*m_pBlockParent->m_pChilds)[i]->m_AreaRect.top + 1;
		}
		RECT	rect;
		rect = this->m_AreaRect;
		::OffsetRect(&rect,x,y);

		rects.push_back(rect);
		return true;
	}

	bool XWordLayer::BlockIsSelected(XWordBlock * pBlock,XWordBlock * pStartBlock,XWordBlock * pEndBlock)
	{
		if(!m_Selection.bSelection)return false;
		if(BlockCmp(pBlock,pStartBlock)<0)return false;
		if(BlockCmp(pBlock,pEndBlock)>0)return false;
		return true;
	}

	int XWordLayer::BlockCmp(XWordBlock * pFirst,XWordBlock* pTwo)
	{
		if(pFirst==pTwo) return 0;
		if(pFirst->m_pBlockParent->m_pBlockParent->GetIndexFromParent() < pTwo->m_pBlockParent->m_pBlockParent->GetIndexFromParent()) return -1;
		if(pFirst->m_pBlockParent->m_pBlockParent->GetIndexFromParent() == pTwo->m_pBlockParent->m_pBlockParent->GetIndexFromParent())
		{
			if(pFirst->m_pBlockParent->GetIndexFromParent() < pTwo->m_pBlockParent->GetIndexFromParent()) return -1;
			if(pFirst->m_pBlockParent->GetIndexFromParent() == pTwo->m_pBlockParent->GetIndexFromParent())
			{
				if(pFirst->GetIndexFromParent() < pTwo->GetIndexFromParent()) return -1;
			}
		}
		return 1;
	}

	bool XWordLayer::AdjustSelection(XWordCursorInfo & CursorInfo,TShiftState state)
	{
		bool bRedraw = false;
		if(state & ssShift)
		{
			m_Selection.bSelection = true;
			int nResult = -1;
			if(!m_Selection.blast)
			{
				if(m_Selection.nStartRow > CursorInfo.nRow || (m_Selection.nStartRow == CursorInfo.nRow && m_Selection.nStartCol > CursorInfo.nCol))
					nResult = 1;
			}else
			{
				if(m_Selection.nEndRow > CursorInfo.nRow || (m_Selection.nEndRow == CursorInfo.nRow && m_Selection.nEndCol > CursorInfo.nCol))
					nResult = 1;
			}
			if(nResult < 0)
			{
				if(m_Selection.blast)
				{
					m_Selection.nStartRow = m_Selection.nEndRow;
					m_Selection.nStartCol = m_Selection.nEndCol;
					 m_Selection.blast = false;
				}
				m_Selection.nEndRow =  CursorInfo.nRow;
				m_Selection.nEndCol = CursorInfo.nCol;
			}
			if(nResult > 0)
			{
				if(!m_Selection.blast)
				{
					m_Selection.nEndRow = m_Selection.nStartRow;
					m_Selection.nEndCol = m_Selection.nStartCol;
					m_Selection.blast = true;
				}
				m_Selection.nStartRow =  CursorInfo.nRow;
				m_Selection.nStartCol = CursorInfo.nCol;
			}
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		}else
		{
			if(m_Selection.bSelection)bRedraw =true;
			m_Selection.nStartRow = CursorInfo.nRow;
			m_Selection.nStartCol = CursorInfo.nCol;
			m_Selection.nEndRow = m_Selection.nStartRow;
			m_Selection.nEndCol = m_Selection.nStartCol;
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		}

		if(m_Selection.nStartRow == m_Selection.nEndRow && m_Selection.nStartCol == m_Selection.nEndCol)m_Selection.bSelection = false;

		return true;
	}

	bool XWordLayer::CursorInfoInSelection(XWordCursorInfo & CursorInfo)
	{
		bool bInSelection = false;
		/**if(CursorInfo.bControlParagraph) return false;
		XWordBlock * pBlock = ((XWordParagraphBlock *)CursorInfo.m_pParagraphBlock)->m_rowInfos[CursorInfo.nRow].m_rowBlocks[CursorInfo.nBlock].pBlock;
		if(m_Selection.bSelection && BlockIsSelected(pBlock))
		{
			bInSelection = true;
			if(pBlock==m_Selection.pStartBlock && CursorInfo.nPosChar < m_Selection.nStartPosChar) bInSelection = false;
			if(pBlock==m_Selection.pEndBlock && CursorInfo.nPosChar > m_Selection.nEndPosChar) bInSelection = false;
		}
		**/
		return bInSelection;
	}
	
	bool XWordLayer::StandOnlySelection(XWordSelection &Selection)
	{
		XWordBlock * pStartBlock ,* pEndBlock;
		return StandOnlySelection(Selection,pStartBlock,pEndBlock);
	}
	bool XWordLayer::StandOnlySelection(XWordSelection &Selection,XWordBlock * &pStartBlockEx,XWordBlock * &pEndBlockEx)
	{
		//sparse start block;
		int nStartPosChar,nEndPosChar;
		XWordBlock * pStartBlock ,* pEndBlock;

		this->GetBlockAndCharPos(Selection.nStartRow,Selection.nStartCol, pStartBlock,nStartPosChar,true);
		if(nStartPosChar !=0)
		{
			XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
			XWordDataTextBlock * pContentBlock = (XWordDataTextBlock *)(pTextBlock->m_pContent);

			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1,pContentBlock->m_pStrText + nStartPosChar);
			XWordTextBlock * pNewTextBlock = new XWordTextBlock();
			pNewTextBlock->m_pContent = pNewDataTextBlock;
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewTextBlock->m_pBlockParent = pTextBlock->m_pBlockParent;
			pNewTextBlock->m_nCssStyle = pTextBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nStartPosChar;
			pContentBlock->m_pStrText[nStartPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}
			std::vector<XWordBlock *>::iterator it = pTextBlock->m_pBlockParent->m_pChilds->begin() +
				pTextBlock->GetIndexFromParent();
			pTextBlock->m_pBlockParent->m_pChilds->insert(it + 1,pNewTextBlock);
			pTextBlock->m_pBlockParent->m_pContent->m_pChilds->insert(pTextBlock->GetIndexFromParent() + 1,pNewTextBlock->m_pContent);

			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			pTextBlock->m_pBlockParent->AdjustExtent();
			pStartBlockEx = pNewTextBlock;
		}else
			pStartBlockEx = pStartBlock;

		//sparse end block;
		this->GetBlockAndCharPos(Selection.nEndRow,Selection.nEndCol, pEndBlock,nEndPosChar);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);
		XWordDataTextBlock * pContentBlock = (XWordDataTextBlock *)(pTextBlock->m_pContent);
		if(nEndPosChar != ::_tcslen(pContentBlock->m_pStrText))
		{
			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nEndPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nEndPosChar) + 1,pContentBlock->m_pStrText + nEndPosChar);
			XWordTextBlock * pNewTextBlock = new XWordTextBlock();
			pNewTextBlock->m_pContent = pNewDataTextBlock;
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewTextBlock->m_pBlockParent = pTextBlock->m_pBlockParent; 
			pNewTextBlock->m_nCssStyle = pTextBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nEndPosChar;
			pContentBlock->m_pStrText[nEndPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}
			std::vector<XWordBlock *>::iterator it = pTextBlock->m_pBlockParent->m_pChilds->begin() +
				pTextBlock->GetIndexFromParent();
			pTextBlock->m_pBlockParent->m_pChilds->insert(it + 1,pNewTextBlock);
			pTextBlock->m_pBlockParent->m_pContent->m_pChilds->insert(pTextBlock->GetIndexFromParent() + 1,pNewTextBlock->m_pContent);
			
			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			pTextBlock->m_pBlockParent->AdjustExtent();
		}
		pEndBlockEx = pEndBlock;
		return true;
	}

	bool XWordLayer::StandOnlyToPara(int nRow,int nCol)
	{
		int nStartPosChar;
		XWordBlock * pStartBlock;

		this->GetBlockAndCharPos(nRow,nCol, pStartBlock,nStartPosChar,true);
		if(nStartPosChar !=0)
		{
			XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
			XWordDataTextBlock * pContentBlock = (XWordDataTextBlock *)(pTextBlock->m_pContent);

			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1,pContentBlock->m_pStrText + nStartPosChar);
			XWordTextBlock * pNewTextBlock = new XWordTextBlock();
			pNewTextBlock->m_pContent = pNewDataTextBlock;
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewTextBlock->m_pBlockParent = pTextBlock->m_pBlockParent;
			pNewTextBlock->m_nCssStyle = pTextBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nStartPosChar;
			pContentBlock->m_pStrText[nStartPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}
			std::vector<XWordBlock *>::iterator it = pTextBlock->m_pBlockParent->m_pChilds->begin() +
				pTextBlock->GetIndexFromParent();
			pTextBlock->m_pBlockParent->m_pChilds->insert(it + 1,pNewTextBlock);
			pTextBlock->m_pBlockParent->m_pContent->m_pChilds->insert(pTextBlock->GetIndexFromParent() + 1,pNewTextBlock->m_pContent);

			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			pStartBlock = pNewTextBlock;
		}
		int nIndex = pStartBlock->GetIndexFromParent();
		if(nIndex==0)
		{

			XWordTextBlock* pBlock = new XWordTextBlock();
			XWordDataTextBlock * pTextBlock = new XWordDataTextBlock();

			pBlock->m_nCssStyle = pStartBlock->m_nCssStyle;

			XWordParagraphBlock * pPara = new XWordParagraphBlock();
			XWordDataParagraph * pParaData = new XWordDataParagraph();

			pBlock->m_pBlockParent = pPara;
			pBlock->m_pContent = pTextBlock;
			pTextBlock->m_pDataParent = pParaData;
			pPara->m_pChilds->push_back(pBlock);
			pParaData->m_pChilds->push_back(pTextBlock);

			pPara->m_pContent = pParaData;
			pPara->m_pBlockParent = pStartBlock->m_pBlockParent->m_pBlockParent;
			pParaData->m_pDataParent = pPara->m_pBlockParent->m_pContent;

			int nParaIndex = pStartBlock->m_pBlockParent->GetIndexFromParent();
			pStartBlock->m_pBlockParent->m_pBlockParent->m_pChilds->insert(pStartBlock->m_pBlockParent->m_pBlockParent->m_pChilds->begin() + nParaIndex,pPara);
			pStartBlock->m_pBlockParent->m_pBlockParent->m_pContent->m_pChilds->insert(nParaIndex,pParaData);
			pStartBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
		}
		else if(nIndex>0)
		{
			XWordContentParagraph * pPrevPara = (XWordContentParagraph *)pStartBlock->m_pBlockParent;
			XWordParagraphBlock * pPara = new XWordParagraphBlock();
			XWordDataParagraph * pParaData = new XWordDataParagraph();
			pPara->m_pContent = pParaData;
			pPara->m_pBlockParent = pPrevPara->m_pBlockParent;
			pParaData->m_pDataParent = pPrevPara->m_pContent->m_pDataParent;
			for(int i=nIndex;i<(int)pPrevPara->m_pChilds->size();i++)
			{
				pParaData->m_pChilds->push_back((*pPrevPara->m_pContent->m_pChilds)[i]);
				(*pPrevPara->m_pContent->m_pChilds)[i]->m_pDataParent = pParaData;
				pPara->m_pChilds->push_back((*pPrevPara->m_pChilds)[i]);
				(*pPrevPara->m_pChilds)[i]->m_pBlockParent = pPara;
			}

			while((int)pPrevPara->m_pChilds->size()> nIndex)
			{
				pPrevPara->m_pChilds->erase(pPrevPara->m_pChilds->begin() + nIndex);
				pPrevPara->m_pContent->m_pChilds->erase(nIndex);
			}
			
			int nParaIndex = pPrevPara->GetIndexFromParent();
			pPrevPara->m_pBlockParent->m_pChilds->insert(pPrevPara->m_pBlockParent->m_pChilds->begin() + nParaIndex + 1,pPara);
			pPrevPara->m_pContent->m_pDataParent->m_pChilds->insert(nParaIndex + 1,pParaData);
			pPrevPara->m_pBlockParent->AdjustExtent();
		}

		return true;
	}

	bool XWordLayer::MerginPara(XWordParagraphBlock * pDestPara,XWordParagraphBlock * pSrcPara)
	{
		for(int i=0;i<(int)pSrcPara->m_pChilds->size();i++)
		{
			(*pSrcPara->m_pChilds)[i]->m_pBlockParent = pDestPara;
			(*pSrcPara->m_pChilds)[i]->m_pContent->m_pDataParent = pDestPara->m_pContent;
			pDestPara->m_pChilds->push_back((*pSrcPara->m_pChilds)[i]);
			pDestPara->m_pContent->m_pChilds->push_back((*pSrcPara->m_pContent->m_pChilds)[i]);
		}
		
		int nIndex = pSrcPara->GetIndexFromParent();
		pSrcPara->m_pBlockParent->m_pChilds->erase(pSrcPara->m_pBlockParent->m_pChilds->begin()+nIndex);
		pSrcPara->m_pBlockParent->m_pContent->m_pChilds->erase(	nIndex);	
		
		pSrcPara->m_pChilds->clear();
		delete pSrcPara;
		
		return true;
	}

	void XWordLayer::SetFontBold()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);

		LOGFONT logFont;
		bool bFirst = true;
		bool bBold = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
			XUtil::Css::GetFont(pBoxStyle,logFont);
			if(bFirst)
			{
				if(logFont.lfWeight == FW_BOLD)
				{
					logFont.lfWeight = FW_NORMAL;
				}else
				{
					logFont.lfWeight = FW_BOLD;
					bBold = true;
				}
			}else
			{
				if(bBold)
					logFont.lfWeight = FW_BOLD;
				else
					logFont.lfWeight = FW_NORMAL;
			}
			
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle) delete pBoxStyle;
			pTextBlock->m_pContent->m_nIndexStyle = nStyleIndex;
			pTextBlock->m_nCssStyle = nStyleIndex;
			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}

			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}
		pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);

		if(m_Selection.blast)
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XWordLayer::SetFontItalic()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);
	
		LOGFONT logFont;
		bool bFirst = true;
		bool bItalic = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
			XUtil::Css::GetFont(pBoxStyle,logFont);
			if(bFirst)
			{
				if(logFont.lfItalic == TRUE)
				{
					logFont.lfItalic = FALSE;
				}else
				{
					logFont.lfItalic = TRUE;
					bItalic = true;
				}
			}else
			{
				if(bItalic)
					logFont.lfItalic = TRUE;
				else
					logFont.lfItalic = FALSE;
			}
			
			if(!pBoxStyle) pBoxStyle = new XUtil::Css::TBoxStyle();
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_pContent->m_nIndexStyle = nStyleIndex;
			pTextBlock->m_nCssStyle = nStyleIndex;
			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}
		pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);


		if(m_Selection.blast)
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XWordLayer::SetFontUnderline()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);
	
		LOGFONT logFont;
		bool bFirst = true;
		bool bUnderline = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
			XUtil::Css::GetFont(pBoxStyle,logFont);
			if(bFirst)
			{
				if(logFont.lfUnderline == TRUE)
				{
					logFont.lfUnderline = FALSE;
				}else
				{
					logFont.lfUnderline = TRUE;
					bUnderline = true;
				}
			}else
			{
				if(bUnderline)
					logFont.lfUnderline = TRUE;
				else
					logFont.lfUnderline = FALSE;
			}
			
			if(!pBoxStyle) pBoxStyle = new XUtil::Css::TBoxStyle();
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_pContent->m_nIndexStyle = nStyleIndex;
			pTextBlock->m_nCssStyle = nStyleIndex;
			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}
		pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);

		if(m_Selection.blast)
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	bool XWordLayer::ChooseFontDlg()
	{
		CHOOSEFONT cf; 
		LOGFONT	   logFont;
		COLORREF	Color = -1;

		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(this->GetBlock(m_Selection.nStartRow,m_Selection.nStartCol));
		const XUtil::Css::TBoxStyle * pBoxStyle = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle);
		ZeroMemory(&cf,sizeof(CHOOSEFONT));

		XUtil::Css::GetFont(pBoxStyle,logFont);
		if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
		{
			Color = pBoxStyle->pColor->color[0];
		}

		cf.lStructSize = sizeof(CHOOSEFONT); 
		cf.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->m_hWnd); 
		cf.hDC = 0;  
		cf.lpLogFont = &logFont; 
		cf.iPointSize = 18; 
		cf.Flags = CF_BOTH|CF_EFFECTS|CF_INITTOLOGFONTSTRUCT|CF_EFFECTS; 
		cf.rgbColors = Color; 
		cf.lCustData = 0; 
		cf.lpfnHook = NULL; 
		cf.lpTemplateName = NULL; 
		cf.hInstance = NULL; 
		cf.lpszStyle = NULL; 
		cf.nFontType = 200; 
		cf.___MISSING_ALIGNMENT__ = 0; 
		cf.nSizeMin = 8; 
		cf.nSizeMax = 32;

		if(::ChooseFont(&cf))
		{
			if(cf.rgbColors !=Color && Color != -1 ||
				(Color == -1 && cf.rgbColors != RGB(0,0,0)))
				this->SetFont(*cf.lpLogFont,cf.rgbColors);
			else
				this->SetFont(*cf.lpLogFont,-1);

			return TRUE;
		}
		return FALSE;
	}

	void XWordLayer::SetFont(LOGFONT &logFont,COLORREF color)
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);
		
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_pContent->m_nIndexStyle = nStyleIndex;
			pTextBlock->m_nCssStyle = nStyleIndex;
			if(pTextBlock->m_pDx)
			{
				delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
			}
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}
		pTextBlock->GetDocumentBlock()->AdjustExtent();
		m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);

		if(m_Selection.blast)
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	bool XWordLayer::ChooseColorDlg(DWORD flag)
	{
		CHOOSECOLOR cc;
		COLORREF colors[16];

		colors[0] = RGB(255,255,255);
		colors[1] = RGB(255,204,153);
		colors[2] = RGB(255,255,153);
		colors[3] = RGB(204,255,204);
		colors[4] = RGB(204,255,255);
		colors[5] = RGB(255,153,204);
		colors[6] = RGB(255,153,0);
		colors[7] = RGB(153,204,0);
		colors[8] = RGB(153,51,0);
		colors[9] = RGB(51,153,102);

		colors[10] = 0xdda0dd;
		colors[11] = 0xee82ee;
		colors[12] = 0xe6e6fa;
		colors[13] = 0x778899;
		colors[14] = 0x00ced1;
		colors[15] = 0x7fffd4;

		::memset(&cc,0,sizeof(CHOOSECOLOR));
		cc.lStructSize = sizeof(CHOOSECOLOR); 
		cc.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->m_hWnd); 
		cc.hInstance = 0; 
		cc.rgbResult = 0; 
		cc.lpCustColors = colors; 
		cc.Flags = CC_RGBINIT; 
		cc.lCustData = 0; 
		cc.lpfnHook = NULL; 
		cc.lpTemplateName = NULL;

		if(::ChooseColor(&cc))
		{
			this->SetColor(cc.rgbResult,flag);
			return TRUE;
		}
		return FALSE;
	}

	void XWordLayer::SetAlign(DWORD flag)
	{
		if(!m_Selection.bSelection)
		{
			GetParagraphByRow(m_CursorInfo.nRow)->m_pContent->align=flag;
		}else
		{
			GetParagraphByRow(m_Selection.nStartRow)->m_pContent->align = flag;
			if(GetBlock(m_Selection.nStartRow,m_Selection.nStartCol)!=GetBlock(m_Selection.nEndRow,m_Selection.nEndCol))
			{
				XWordBlock * pBlock = GetBlock(m_Selection.nStartRow,m_Selection.nStartCol); 
				pBlock = pBlock->GetNextTextBlock();
				while(pBlock!= GetBlock(m_Selection.nEndRow,m_Selection.nEndCol) && pBlock)
				{
					pBlock->m_pBlockParent->m_pContent->align = flag;
					pBlock = pBlock->GetNextTextBlock();
				}
				GetParagraphByRow(m_Selection.nEndRow)->m_pContent->align = flag;
			}
		}

		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		if(m_Selection.blast)
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XWordLayer::SetColor(COLORREF color, DWORD flag)
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);
	
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_pContent->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);

			if(flag & STI_TEXTCOLOR)
			{
				if(!(pBoxStyle->pColor))
				{
					pBoxStyle->mask |= cssMaskColor;
					pBoxStyle->pColor = new XUtil::Css::TColor;
				}
				pBoxStyle->pColor->colorRef[0] = XUtil::Css::_rgbcolor;
				pBoxStyle->pColor->color[0] = color;
			}else if(flag & STI_BKCOLOR)
			{
				if(!(pBoxStyle->pBackGround))
				{
					pBoxStyle->mask |= cssMaskBackGround;
					pBoxStyle->pBackGround = new XUtil::Css::TBackGround;
				}
				pBoxStyle->pBackGround->mask = cssMaskBackGroundColor;
				pBoxStyle->pBackGround->color.colorRef[0] = XUtil::Css::_rgbcolor;
				pBoxStyle->pBackGround->color.color[0] = color;
			}

			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_pContent->m_nIndexStyle = nStyleIndex;
			pTextBlock->m_nCssStyle = nStyleIndex;
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}

		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		ShowCurrentCaret(true);
	}

	int XWordLayer::RemoveBlock(XWordBlock * pBlock)
	{
		if(m_pWordDocumentBlock->m_pChilds->size()==1 &&
			(*m_pWordDocumentBlock->m_pChilds)[0]->m_pChilds->size()==1 && 
			(*(*m_pWordDocumentBlock->m_pChilds)[0]->m_pChilds)[0]->m_pChilds->size()==1) return 1;
		int nIndex = pBlock->GetIndexFromParent();
		XWordContentParagraph * pPara;
		XWordChapterBlock * pChater;
		pPara = (XWordContentParagraph *)pBlock->m_pBlockParent;

		delete pBlock->m_pContent;
		delete pBlock;
		pPara->m_pChilds->erase(pPara->m_pChilds->begin() + nIndex);
		pPara->m_pContent->m_pChilds->erase(nIndex);
		pChater = (XWordChapterBlock *)pPara->m_pBlockParent;
		if(pPara->m_pChilds->size()==0)
		{
			nIndex = pPara->GetIndexFromParent();
			delete pPara->m_pContent;
			delete pPara;
			pChater->m_pChilds->erase(pChater->m_pChilds->begin() + nIndex);
			pChater->m_pContent->m_pChilds->erase(nIndex);
		}
		if(pChater->m_pChilds->size()==0)
		{
			nIndex = pChater->GetIndexFromParent();
			delete pChater->m_pContent;
			delete pChater;
			m_pWordDocumentBlock->m_pChilds->erase(m_pWordDocumentBlock->m_pChilds->begin()+nIndex);
		}else
			pChater->AdjustExtent();
		return 1;
	}

	bool XWordLayer::BackDeleteChar(XWordCursorInfo & CursorInfo)
	{
		m_Selection.bSelection = false;
		int nPosChar;
		XWordBlock * pBlock ;
		XWordBlock * pBlockEx ;
		this->GetBlockAndCharPos(CursorInfo.nRow,CursorInfo.nCol,pBlock ,nPosChar);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
		if(!pTextBlock) return false;
		
		int nCol = CursorInfo.nCol; 
		this->CalcLeftCursorInfo(CursorInfo);
		LPTSTR pStr = ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;

		XWordTextBlock * pTextBlockEx;
		if(nPosChar == 0)
		{
			if(pBlock->GetIndexFromParent()==0)
			{
				pBlockEx = pBlock->GetPirorTextBlock();
				if(!pBlockEx) return false;

				pTextBlockEx = dynamic_cast<XWordTextBlock *>(pBlockEx);
				if(!((XWordDataTextBlock *)pTextBlockEx->m_pContent)->m_pStrText||((XWordDataTextBlock *)pTextBlockEx->m_pContent)->m_pStrText[0]=='\0')
				{
					this->RemoveBlock(pBlockEx);
				}else
					this->MerginPara(dynamic_cast<XWordParagraphBlock *>(pBlockEx->m_pBlockParent),dynamic_cast<XWordParagraphBlock *>(pBlock->m_pBlockParent));
				pBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
				m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				ShowCurrentCaret(true);

				return true;
			}
		}

		while(nPosChar==0)
		{
			pBlock = pBlock->GetPirorTextBlock();
			if(!pBlock) return false;
			if(!pStr ||pStr[0]=='\0')
			{
				this->RemoveBlock(pTextBlock);
			}
			pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
			pStr = ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;
			nPosChar = (int)::_tcslen(pStr);
		}

		if(pStr[nPosChar - 1]=='\n' && nPosChar - 2 >=0 && pStr[nPosChar - 2]=='\r')
		{
			::_tcscpy_s(pStr + nPosChar - 2, _tcslen(pStr) - nPosChar + 1, pStr + nPosChar );
			nPosChar -= 2;
		}else
		{
			::_tcscpy_s(pStr + nPosChar - 1, _tcslen(pStr) - nPosChar + 1, pStr + nPosChar );
			nPosChar--;
		}
		
		if(pStr[0]=='\0' && nCol - 1>1)
		{
			this->RemoveBlock(pTextBlock);
			pTextBlock = NULL;
		}else
		{
			if(pTextBlock)
			{
				if(pTextBlock->m_pDx)
				{
					delete pTextBlock->m_pDx;
					pTextBlock->m_pDx = NULL;
				}
			}
			((XWordContentParagraph *)pTextBlock->m_pBlockParent)->AdjustExtent();
		}
		m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		ShowCurrentCaret(true);
		return true;
	}

	bool XWordLayer::DeleteChar(XWordCursorInfo & CursorInfo)
	{
		m_Selection.bSelection = false;
		int nPosChar;
		XWordBlock * pBlock;
		this->GetBlockAndCharPos(CursorInfo.nRow,CursorInfo.nCol,pBlock ,nPosChar);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
		if(!pTextBlock) return false;
		
		int nCol = CursorInfo.nCol; 
		int nRow = CursorInfo.nRow;
		int nColCount = this->GetColCount(nRow);

		XWordBlock * pBlockEx ;
		XWordTextBlock * pTextBlockEx;
		LPTSTR pStr = ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;
		if(!pStr ||(int)::_tcslen(pStr)== nPosChar)
		{
			if(pBlock->GetIndexFromParent()==pBlock->m_pBlockParent->m_pChilds->size() - 1)
			{
				pBlockEx = pBlock->GetNextTextBlock();
				if(!pBlockEx) return false;
				pTextBlockEx = dynamic_cast<XWordTextBlock *>(pBlockEx);
				if(!((XWordDataTextBlock *)pTextBlockEx->m_pContent)->m_pStrText||((XWordDataTextBlock *)pTextBlockEx->m_pContent)->m_pStrText[0]=='\0')
				{
					this->RemoveBlock(pBlockEx);
				}else
					this->MerginPara(dynamic_cast<XWordParagraphBlock *>(pBlock->m_pBlockParent),dynamic_cast<XWordParagraphBlock *>(pBlockEx->m_pBlockParent));
				pBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
				m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				ShowCurrentCaret(true);
				return true;
			}
		}

		if(pStr && (int)::_tcslen(pStr)> nPosChar)
		{
			if(pStr[nPosChar]=='\r' && (int)::_tcslen(pStr)> nPosChar + 1 && pStr[nPosChar + 1]=='\n')
				::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 2 );
			else
				::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 1);
		}else
		{
			while(!pStr || (int)::_tcslen(pStr)<=nPosChar)
			{
				pBlock = pBlock->GetNextTextBlock();
				if(!pBlock) return false;
				pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
				pStr = ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;
				nPosChar = 0;
			}
			::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 1 );
		}

		if(pStr[0]=='\0' && nCol < nColCount - 1)
		{
			this->RemoveBlock(pTextBlock);
			pTextBlock = NULL;
		}else
		{
			if(pTextBlock)
			{
				if(pTextBlock->m_pDx)
				{
					delete pTextBlock->m_pDx;
					pTextBlock->m_pDx = NULL;
				}
			}
			((XWordContentParagraph *)pTextBlock->m_pBlockParent)->AdjustExtent();
		}

		m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);

		return true;
	}


	bool XWordLayer::InsertChar(XWordCursorInfo & CursorInfo,TCHAR ch)
	{
		m_Selection.bSelection = false;

		if(ch=='\r')
		{
			StandOnlyToPara(CursorInfo.nRow,CursorInfo.nCol);
			m_nPage = CalcPages();
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
			this->CalcRightCursorInfo(CursorInfo);
			ShowCurrentCaret(true);
			return true;
		}

		int nPosChar;
		XWordBlock * pBlock;
		this->GetBlockAndCharPos(CursorInfo.nRow,CursorInfo.nCol,pBlock ,nPosChar);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
		if(!pTextBlock) return false;
		
		LPTSTR pStr = ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;
		int nLimit = ((XWordDataTextBlock *)pTextBlock->m_pContent)->nStrLimit;

		LPTSTR pStrTemp = NULL;
		int nStrLen = (int)(pStr?_tcslen(pStr):0);
		if(!pStr)
		{
			nLimit = 35;
			pStr = new TCHAR[nLimit];
			pStr[0]='\0';
			((XWordDataTextBlock *)pTextBlock->m_pContent)->nStrLimit = nLimit;
			((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText = pStr;
		}else
		{
			if(nStrLen + 3 > nLimit)
			{
				if(nLimit == 0)
					nLimit =(int) nStrLen + 3 + 32;
				else
					nLimit += 32;
				pStrTemp = new TCHAR[nLimit];
				((XWordDataTextBlock *)pTextBlock->m_pContent)->nStrLimit = nLimit;
			}
			
			if(pStrTemp)
			{
				::_tcscpy_s(pStrTemp,nLimit,pStr);
				delete pStr;
				pStr = pStrTemp;
				((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText = pStr;
			}
		}
		if(ch!=0x0B)
		{
			int nLen = (int)::_tcslen(pStr + nPosChar) + 1;
			while(nLen> 0)
			{
				pStr[nPosChar + nLen] = pStr[nPosChar + nLen - 1];
				nLen--;
			}
			pStr[nPosChar] = ch;
		}
		else
		{
			int nLen = (int)::_tcslen(pStr + nPosChar) + 2;
			while(nLen> 0)
			{
				pStr[nPosChar + nLen + 1] = pStr[nPosChar + nLen - 1];
				nLen--;
			}
			pStr[nPosChar] = '\r';
			pStr[nPosChar + 1] = '\n';
		}
		if(pTextBlock->m_pDx)
		{
			delete pTextBlock->m_pDx;
			pTextBlock->m_pDx = NULL;
		}
		pTextBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
		m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		this->CalcRightCursorInfo(CursorInfo);
		ShowCurrentCaret(true);
		return true;
	}
	
	void XWordLayer::WMVScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
	{
		ModifyScrollBar(SB_VERT, nSBCode, nPos);
	}

	void XWordLayer::WMHScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
	{
		ModifyScrollBar(SB_HORZ, nSBCode, nPos);
	}

	void XWordLayer::WMWheel(POINT point,UINT_PTR	fwKeys,	short zDelta)
	{
		if(zDelta > 0)
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
		else
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
	}

	int XWordLayer::CalcScrollBar(int Value, int ScrollCode, int ScrollBar,int Pos)
	{
		int Result;
		Result = Value;
		switch(ScrollCode) 
		{
		case SB_LINEUP:
			Result = Value - 15;
			break;
		case SB_LINEDOWN:
			Result = Value + 15;
			break;
		case SB_PAGEUP:
			Result = Value - PageUp(ScrollBar);
			break;
		case SB_PAGEDOWN:
			Result = Value + PageDown(ScrollBar);
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if((goThumbTracking & FOptions) || (ScrollCode == SB_THUMBPOSITION))
			{
				Result = ScrollBarMin(ScrollBar) + 
					MulDiv(Pos, ScrollBarMax(ScrollBar)
					- ScrollBarMin(ScrollBar), 100);
				break;
			}
		case SB_BOTTOM:
			Result = ScrollBarMax(ScrollBar);
			break;
		case SB_TOP:
			Result = ScrollBarMin(ScrollBar);
			break;
		}
		return Result;
	}

	void XWordLayer::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
	{
		if(ScrollBar==SB_VERT)
		{
			m_ptScroll.y = this->CalcScrollBar(m_ptScroll.y,ScrollCode,ScrollBar,Pos);
			if(m_ptScroll.y < ScrollBarMin(ScrollBar)) m_ptScroll.y = ScrollBarMin(ScrollBar);
			if(m_ptScroll.y > ScrollBarMax(ScrollBar)) m_ptScroll.y = ScrollBarMax(ScrollBar);
		}else
		{
			m_ptScroll.x = this->CalcScrollBar(m_ptScroll.x,ScrollCode,ScrollBar,Pos);
			if(m_ptScroll.x < ScrollBarMin(ScrollBar)) m_ptScroll.x = ScrollBarMin(ScrollBar);
			if(m_ptScroll.x > ScrollBarMax(ScrollBar)) m_ptScroll.x = ScrollBarMax(ScrollBar);
		}
		this->UpdateScrollPos(ScrollBar);
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		ShowCurrentCaret(true);
	}

	void XWordLayer::UpdateScrollPos(int ScrollBar)
	{
		if(ScrollBar==SB_VERT)
		{
			if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(m_ptScroll.y , 100 , GetMaxScrollExtent(ScrollBar)));
		}else
			if(GetHScrollBar())GetHScrollBar()->SetPosition(MulDiv(m_ptScroll.x, 100 , GetMaxScrollExtent(ScrollBar)));
	}

	int XWordLayer::PageUp(int ScrollBar)
	{
		if(ScrollBar==SB_VERT)
		{
			if(m_ptScroll.y > FRect.bottom - FRect.top + 1)
				return FRect.bottom - FRect.top + 1;
			else
				return m_ptScroll.y;
		}else
		{
			if(m_ptScroll.x > FRect.right - FRect.left + 1)
				return FRect.right - FRect.left + 1;
			else
				return m_ptScroll.x;

		}
		return 0;
		
	}

	int XWordLayer::PageDown(int ScrollBar)
	{
		if(ScrollBar==SB_VERT)
		{
			int nMaxScrollExtent = GetMaxScrollExtent(ScrollBar);
			if(m_ptScroll.y +( FRect.bottom - FRect.top + 1) > nMaxScrollExtent)
				return nMaxScrollExtent;
			else
				return m_ptScroll.y + FRect.bottom - FRect.top + 1;
		}else
		{
			int nMaxScrollExtent = GetMaxScrollExtent(ScrollBar);
			if(m_ptScroll.x +( FRect.right - FRect.left + 1) > nMaxScrollExtent)
				return nMaxScrollExtent;
			else
				return m_ptScroll.x + FRect.right - FRect.left + 1;
		}
		return 0;
	}

	int XWordLayer::ScrollBarMin(int ScrollBar)
	{
		return 0;
	}

	int XWordLayer::ScrollBarMax(int ScrollBar)
	{
		return GetMaxScrollExtent( ScrollBar);
	}

	int XWordLayer::GetTotalHeight()
	{
		return  m_nPage *(m_rcPage.bottom - m_rcPage.top + 1) + (m_nPage +1) * 20;
	}

	int XWordLayer::GetMaxScrollExtent(int ScrollBar)
	{
		if(ScrollBar==SB_VERT)
		{
			return GetTotalHeight() - (FRect.bottom - FRect.top + 1);
			
		}else
		{
			RECT rc = FRect;
			if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
				return (m_rcPage.right - m_rcPage.left) - (rc.right - rc.left) + 10;
			else
				return 0; //not need scroll
		}
	}

	bool XWordLayer::ClampInView(const XWordCursorInfo & CursorInfo)
	{
		//if(CursorInfo.bControlParagraph) return false;

		XWordParagraphRowInfo::RowBlock * pRowBlock;
		XWordParagraphBlock * pTextParagraphBlock = dynamic_cast<XWordParagraphBlock *>(this->GetParagraphByRow(CursorInfo.nRow));
		pRowBlock = this->GetRowBlock(CursorInfo.nRow,CursorInfo.nCol);

		if(CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y < pRowBlock->nRowheight + 40 )
		{
			this->ShowCurrentCaret(false);
			m_ptScroll.y -= pRowBlock->nRowheight - (CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y) + 40;
			if(m_ptScroll.y <= 40)m_ptScroll.y = 0;
			UpdateScrollPos(SB_VERT);
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
			this->ShowCurrentCaret(true);
			return true;
		}else if(CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y > FRect.bottom - FRect.top + 1 - 40)
		{
			this->ShowCurrentCaret(false);
			m_ptScroll.y += (CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y) - (FRect.bottom - FRect.top + 1) + 40;
			if(m_ptScroll.y <= 40)m_ptScroll.y = 0;
			UpdateScrollPos(SB_VERT);
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
			this->ShowCurrentCaret(true);
			return true;
		}else
		{
			this->ShowCurrentCaret(true);
			return false;
		}
	}
	
	bool XWordLayer::GetFirstRowOfPage(int nPage,int & nParaRow,XWordContentParagraph * & pParaBlockEx)
	{
		if(nPage > m_nPage) return false;
		if(nPage < 1) return false;

		int nPages = 1;
		int height = 0;
		int rtHeight = m_rcPageContent.bottom - m_rcPageContent.top;
		if(m_pWordDocumentBlock)
		{
			XWordBlock * pChater;
			XWordContentParagraph * pContentParaBlock;
			XWordParagraphBlock * pParaBlock;
			for(int i=0;i<(int)m_pWordDocumentBlock->m_pChilds->size();i++)
			{
				pChater = (*m_pWordDocumentBlock->m_pChilds)[i];
				if(pChater->m_pContent->m_bBreakPage)
				{
					nPages++;
					height = 0;
					if(nPages > nPage) return false;
					if(nPages == nPage && (int)pChater->m_pChilds->size() > 0)
					{
						pParaBlockEx = dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[0]);
						nParaRow = 0;
						return true;
					}
				}
				for(int j=0;j<(int)pChater->m_pChilds->size();j++)
				{
					pContentParaBlock = dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[j]);
					if(pContentParaBlock->m_pContent->m_bBreakPage)
					{
						nPages++;
						height = 0;
						if(nPages > nPage) return false;
						if(nPages == nPage)
						{
							pParaBlockEx = pContentParaBlock;
							nParaRow = 0;
							return true;
						}
					}

					pParaBlock = dynamic_cast<XWordParagraphBlock *>(pContentParaBlock);
					if(pParaBlock)
					{
						for(int m=0;m<(int)pParaBlock->m_rowInfos.size();m++)
						{
							if(pParaBlock->m_rowInfos[m].nRowHeight + height > rtHeight)
							{
								nPages++;
								height = 0;
								if(nPages > nPage) return false;
								if(nPages == nPage)
								{
									pParaBlockEx = pParaBlock;
									nParaRow = m;
									return true;
								}
							}else
							{
								height += pParaBlock->m_rowInfos[m].nRowHeight;
								if(nPage==1)
								{
									pParaBlockEx = pParaBlock;
									nParaRow = m;
									return true;
								}
							}
						}
					}else
					{
						//should control ParaBlock
						if(pContentParaBlock->m_pContent->height > rtHeight - height)
						{
							nPages++;
							height = pContentParaBlock->m_pContent->height;
							if(nPages > nPage) return false;
							if(nPages == nPage)
							{
								pParaBlockEx = pParaBlock;
								nParaRow = -1;
								return true;
							}
						}
						else
						{
							height += pContentParaBlock->m_pContent->height;
							if(nPage==1)
							{
								pParaBlockEx = pContentParaBlock;
								nParaRow = -1;
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	int XWordLayer::CalcPages()
	{
		int nPages = 1;
		int height = 0;
		int rtHeight = m_rcPageContent.bottom - m_rcPageContent.top;
		if(m_pWordDocumentBlock)
		{
			XWordBlock * pChater;
			XWordContentParagraph * pContentParaBlock;
			XWordParagraphBlock * pParaBlock;
			for(int i=0;i<(int)m_pWordDocumentBlock->m_pChilds->size();i++)
			{
				pChater = (*m_pWordDocumentBlock->m_pChilds)[i];
				if(pChater->m_pContent->m_bBreakPage)
				{
					nPages++;
					height = 0;
				}
				for(int j=0;j<(int)pChater->m_pChilds->size();j++)
				{
					pContentParaBlock = dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[j]);
					if(pContentParaBlock->m_pContent->m_bBreakPage)
					{
						nPages++;
						height = 0;
					}

					pParaBlock = dynamic_cast<XWordParagraphBlock *>(pContentParaBlock);
					if(pParaBlock)
					{
						for(int m=0;m<(int)pParaBlock->m_rowInfos.size();m++)
						{
							if(pParaBlock->m_rowInfos[m].nRowHeight + height > rtHeight)
							{
								nPages++;
								height = 0;
							}else
							{
								height += pParaBlock->m_rowInfos[m].nRowHeight;
							}
						}
					}else
					{
						//should control ParaBlock
						if(pContentParaBlock->m_pContent->height > rtHeight - height)
						{
							nPages++;
							height = pContentParaBlock->m_pContent->height;
						}
						else
							height += pContentParaBlock->m_pContent->height;
					}
				}
			}
		}
		return nPages;
	}

	bool XWordLayer::DoDrawPage(HDC hPaintDC,int nPage,POINT ptOffset)
	{
		int rtHeight = m_rcPageContent.bottom - m_rcPageContent.top;
		int nHeight = rtHeight;
		if(m_pWordDocumentBlock)
		{
			XWordBlock * pChater;
			XWordContentParagraph * pContentParaBlock;
			XWordParagraphBlock * pParaBlock;
			int nStartRow=0;
			int nIndex=0;
			bool bFirst = true;
			if(!GetFirstRowOfPage(nPage,nStartRow,pContentParaBlock)) return false;
			if(!pContentParaBlock) return false;

			pChater = pContentParaBlock->m_pBlockParent;
			nIndex = pContentParaBlock->GetIndexFromParent();
			while(pChater)
			{
				for(int j=nIndex;j<(int)pChater->m_pChilds->size();j++)
				{
					if(!bFirst && (*pChater->m_pChilds)[j]->m_pContent->m_bBreakPage) return true;
					pParaBlock = dynamic_cast<XWordParagraphBlock *>((*pChater->m_pChilds)[j]);
					if(pParaBlock)
					{
						int nHeightOld = nHeight;
						pParaBlock->DoDraw(hPaintDC,ptOffset,nStartRow,nHeight);
						ptOffset.y += nHeightOld - nHeight;
						if(nHeightOld == nHeight || nStartRow!=0)
							return true;
					}else
					{
						//should control ParaBlock
						pContentParaBlock = dynamic_cast<XWordContentParagraph *>((*pChater->m_pChilds)[j]);
						if(pContentParaBlock->m_pContent->height > nHeight)
						{
							return true;
						}
						else
						{
							pContentParaBlock->DoDraw(hPaintDC,ptOffset);
							nHeight -= pContentParaBlock->m_pContent->height;
							ptOffset.y += pContentParaBlock->m_pContent->height;
						}
					}
					bFirst = false;
				}

				if(nIndex >= (int)pChater->m_pBlockParent->m_pChilds->size() - 1)
					return true;
				else
				{
					nIndex++;
					pChater = (*pChater->m_pBlockParent->m_pChilds)[nIndex];
				}
			}
		}
		return false;
	}

	int XWordLayer::SetFRect(RECT &rc)
	{
		FRect = rc;
		return 1;
	}
	
	int XWordLayer::DoCopy()
	{
		if(!m_Selection.bSelection) return 0; //should todo add new block

		TCHAR* lpdata;
		HGLOBAL        hClipGlobal;
		hClipGlobal = GlobalAlloc(0,128000);
		lpdata = (TCHAR *)GlobalLock(hClipGlobal);
		ZeroMemory(lpdata,128000);


		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
		XWordTextBlock * pEndTextBlock = dynamic_cast<XWordTextBlock *>(pEndBlock);

		bool bFirst = true;
		bool bBold = false;

		XWordBlock* pPrevParentBlock = NULL;
		while(pTextBlock)
		{
			if(!pPrevParentBlock)pPrevParentBlock = pTextBlock->m_pBlockParent;
			if(pPrevParentBlock != pTextBlock->m_pBlockParent)
				_tcscat_s(lpdata,64000,_T("\r\n"));
			if(((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText)
				_tcscat_s(lpdata,64000,((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText);

			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = dynamic_cast<XWordTextBlock *>(pTextBlock->GetNextTextBlock());
		}

		GlobalUnlock(hClipGlobal);
		
		if(OpenClipboard(m_pSheetSvr->GetHWND()))
		{
			SetClipboardData(CF_UNICODETEXT,hClipGlobal);
			CloseClipboard();
		}
		return 1;
	}

	int XWordLayer::DoCut()
	{
		DoCopy();
		if(!m_Selection.bSelection) return 0; //should todo add new block

		XWordBlock * pStartBlock;
		XWordBlock * pEndBlock;
		XWordBlock * pBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);

		if(m_pWordDocumentBlock->m_pChilds->size()==1 &&
			(*m_pWordDocumentBlock->m_pChilds)[0]->m_pChilds->size()==1 && 
			(*(*m_pWordDocumentBlock->m_pChilds)[0]->m_pChilds)[0]->m_pChilds->size()==1)
		{
			XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pStartBlock);
			if(pTextBlock)
			{
				if(((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText)
					delete ((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText;
				((XWordDataTextBlock *)pTextBlock->m_pContent)->m_pStrText = NULL;
				if(pTextBlock->m_pDx) delete pTextBlock->m_pDx;
				pTextBlock->m_pDx = NULL;
				pTextBlock->AdjustExtent();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
				CalcCursorInfo(m_CursorInfo,1,1);
				ShowCurrentCaret(true);
			}
			return 1;
		}

		bool bbreak = true;
		while(bbreak && pStartBlock)
		{
			if(pStartBlock == pEndBlock)bbreak = false;
			pBlock = pStartBlock;
			int nIndex = pBlock->GetIndexFromParent();
			XWordContentParagraph * pPara;
			XWordChapterBlock * pChater;
			pPara = (XWordContentParagraph *)pBlock->m_pBlockParent;
			delete pBlock->m_pContent;
			delete pBlock;
			pPara->m_pChilds->erase(pPara->m_pChilds->begin() + nIndex);
			pPara->m_pContent->m_pChilds->erase(nIndex);
			pChater = (XWordChapterBlock *)pPara->m_pBlockParent;

			int nPara = pPara->GetIndexFromParent();
			bool bPara = true;
			if(pPara->m_pChilds->size()==0)
			{
				nIndex = pPara->GetIndexFromParent();
				delete pPara->m_pContent;
				delete pPara;
				pChater->m_pChilds->erase(pChater->m_pChilds->begin() + nIndex);
				pChater->m_pContent->m_pChilds->erase(nIndex);
				bPara  = false;
			}else if(nIndex == pPara->m_pChilds->size())
				bPara  = false;
			int nChater = pChater->GetIndexFromParent();
			bool bChater = true;
			if(pChater->m_pChilds->size()==0)
			{
				nIndex = pChater->GetIndexFromParent();
				delete pChater->m_pContent;
				delete pChater;
				m_pWordDocumentBlock->m_pChilds->erase(m_pWordDocumentBlock->m_pChilds->begin()+nIndex);
				bChater =false;
			}else if(nPara == pChater->m_pChilds->size())
			{
				bChater =false;
			}

			if(!pEndBlock) break;
			if(bPara)
				pStartBlock = (*pPara->m_pChilds)[nIndex];
			else if(bChater)
			{
				pStartBlock =  (*((*pChater->m_pChilds)[nPara]->m_pChilds))[0];
			}else
				(*((*(*m_pWordDocumentBlock->m_pChilds)[0]->m_pChilds)[0]->m_pChilds))[0];
		}
		m_Selection.bSelection = false;
		m_pWordDocumentBlock->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->GetHWND(),&FRect,false);
		CalcCursorInfo(m_CursorInfo,1,1);
		ShowCurrentCaret(true);

		return 1;
	}

	int XWordLayer::DoPaste()
	{
		HANDLE        hClipGlobal;
		void *       lpbi;
		int         length;

		OpenClipboard(this->m_pSheetSvr->m_pSheetCtrl->m_hWnd);
		if( (hClipGlobal = GetClipboardData( CF_UNICODETEXT )) != NULL )
		{
			length = (int)GlobalSize(hClipGlobal);                         
			// Lock it down
			if( (lpbi=GlobalLock(hClipGlobal)) != NULL )
			{
				HANDLE hData;
				TCHAR* lpdata;
				hData = GlobalAlloc(0,length);
				lpdata = (TCHAR *)GlobalLock(hData);
				CopyMemory(lpdata,lpbi,length);
				
				int nCharPos;
				XWordBlock* pBlock ;
				this->GetBlockAndCharPos(m_CursorInfo.nRow,m_CursorInfo.nCol,pBlock,nCharPos);

				int nLimit;
				int nStrLen;
				LPTSTR pStrTemp;
				LPTSTR pStr = ((XWordDataTextBlock *)pBlock->m_pContent)->m_pStrText;
				if(!pStr)
				{
					pStr = new TCHAR[length/2];
					::_tcscpy_s(pStr,length/2,(LPTSTR)lpdata);
					((XWordDataTextBlock *)pBlock->m_pContent)->nStrLimit = length/2 + 1;
					((XWordDataTextBlock *)pBlock->m_pContent)->m_pStrText = pStr;
				}else
				{
					nStrLen = (int)::_tcslen(pStr) + length/2;
					nLimit = ((XWordDataTextBlock *)pBlock->m_pContent)->nStrLimit;
					if(nLimit == 0)
						nLimit =(int) nStrLen + 3 + 32;
					else
						nLimit += length/2 + 32;
					pStrTemp = new TCHAR[nLimit];
					((XWordDataTextBlock *)pBlock->m_pContent)->nStrLimit = nLimit;

					if(pStrTemp)
					{
						::_tcsncpy_s(pStrTemp,nLimit,pStr,nCharPos);
						::_tcscpy_s(pStrTemp + nCharPos,length/2 ,(LPTSTR)lpdata);

						int nLen = (int)::_tcslen(pStr);
						::_tcscpy_s(pStrTemp + nCharPos + length/2 - 1,::_tcslen(pStr) - nCharPos + 1,pStr + nCharPos);
						delete pStr;
						pStr = pStrTemp;
						((XWordDataTextBlock *)pBlock->m_pContent)->m_pStrText = pStr;
					}
				}
				XWordTextBlock * pTextBlock = dynamic_cast<XWordTextBlock *>(pBlock);
				if(pTextBlock->m_pDx)
				{
					delete pTextBlock->m_pDx;
					pTextBlock->m_pDx = NULL;
				}
				

				GlobalUnlock(hData);
				GlobalFree(hData);

				GlobalUnlock( hClipGlobal );
				
				pTextBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
				m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				this->CalcRightCursorInfo(m_CursorInfo);
				ShowCurrentCaret(true);
			}
		}
		CloseClipboard();

		return 1;
	}
}}}}
