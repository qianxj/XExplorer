#include "stdafx.h"
#include "xwordsheetsvr.hpp"
#include "XWordSerializeSvr.hpp"

#define ErrorHandler(Error) 

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordSheetSvr::CBaseBlock::CBaseBlock():m_pParent(NULL),m_bRecalcPosition(true),m_nHeight(0)
	{
		SetRect(&m_AreaRect,0,0,0,0);
		SetRect(&m_ContentRect,0,0,0,0);
	}
	XWordSheetSvr::CBaseBlock::~CBaseBlock()
	{
		m_RectStrPos.clear();
		m_Rects.clear();
	}

	bool	XWordSheetSvr::CBaseBlock::HitedBlock(POINT pt,bool bInRects)
	{
		if(bInRects == false)
		{
			if(m_AreaRect.left<=pt.x && m_AreaRect.right >=pt.x && 
				m_AreaRect.top<=pt.y && m_AreaRect.bottom >=pt.y) return true;
			else
				return false;
		}
		else
		{
			for(int i=0;i<m_Rects.size();i++)
			{
				if(m_Rects[i].left<=pt.x && m_Rects[i].right >=pt.x && 
					m_Rects[i].top<=pt.y && m_Rects[i].bottom >=pt.y) return true;
			}
			return false;
		}
	}
	void*	XWordSheetSvr::CBaseBlock::GetParent()
	{
		return m_pParent;
	}

	void	XWordSheetSvr::CBaseBlock::SetParent(void*	pParent)
	{
		m_pParent = pParent;
	}


	bool XWordSheetSvr::CBaseBlock::AdjustExtent()
	{
		bool ret;
		HDC hdc = ::GetDC(NULL);
		ret = AdjustExtent(hdc);
		::ReleaseDC(NULL,hdc);
		return ret;

	}

	int XWordSheetSvr::CBaseBlock::GetExtent(SIZE &size)
	{
		size.cx = m_AreaRect.right - m_AreaRect.left + 1;
		size.cy = m_nHeight;
		return 1;
	}

	bool XWordSheetSvr::CBaseBlock::CalcContentRect( )
	{

		m_ContentRect.left  = m_AreaRect.left;
		m_ContentRect.right = m_AreaRect.right ;
		m_ContentRect.top  = m_AreaRect.top;
		m_ContentRect.bottom = m_AreaRect.bottom ;

		return true;
	}
	int XWordSheetSvr::CBaseBlock::GetHeight()
	{
		if(m_bRecalcPosition)
			CBaseBlock::AdjustExtent();
		return this->m_nHeight;
	}

	XWordSheetSvr::CBlock::CBlock():m_nFontSize(-9),m_nRowHeight(0),m_nCurIndex(0),m_nCurPos(0),m_pDx(NULL)
	{

	}


	XWordSheetSvr::CBlock::~CBlock()
	{
		if(m_pDx)delete[] m_pDx;
		if(m_strText) delete m_strText;
		m_strText = NULL;
	}

	XWordSheetSvr::CBlock * XWordSheetSvr::CBlock::GetObjectAtPoint(POINT pt)
	{
		LPTSTR	pStr;
		int nFit,width;
		SIZE sz;
		pStr = m_strText;
		for(int k=0;k<m_Rects.size();k++)
		{
			nFit = m_RectStrPos[k];
			if(m_Rects[k].left<=pt.x && m_Rects[k].right >=pt.x && 
				m_Rects[k].top<=pt.y && m_Rects[k].bottom >=pt.y)
			{
				m_nCurIndex =k;
				width = m_Rects[k].right - m_Rects[k].left + 1;
				HDC hDC = GetDC(NULL);
				LOGFONT	logFont;
				HFONT	hFont;
				GetFont(logFont);
				hFont = ::CreateFontIndirect(&logFont);
				hFont = (HFONT)::SelectObject(hDC,hFont);
				pt.x -=m_Rects[k].left;

				if(m_pDx)delete[] m_pDx;
				m_pDx = new int[nFit];
				GetTextExtentExPoint(hDC,pStr,nFit,width,&nFit,m_pDx,&sz);
				int l=0;
				for(l=0;l<nFit;l++)
				{
					if(m_pDx[l] > pt.x)break;
				}
				if(l>=nFit)
				{
					this->m_nCurPos = nFit;
				}
				else
				{
					if(m_pDx[l] - pt.x > pt.x - (l==0 ? 0 :m_pDx[l - 1]) )
						this->m_nCurPos = l;
					else
						this->m_nCurPos = l+1;
				}
				TEXTMETRIC tm;
				::GetTextMetrics(hDC,&tm);
				m_nCaretHeight =tm.tmHeight + tm.tmExternalLeading;

				hFont = (HFONT)::SelectObject(hDC,hFont);
				::DeleteObject(hFont);
				::ReleaseDC(NULL,hDC);
				return this;
			}
			pStr += nFit;
		}
		return NULL;
	}

	int XWordSheetSvr::CBlock::GetRect(RECT &rect)
	{
		GetParagraph()->GetRect(rect);
		rect.left += GetParagraph()->m_ContentRect.left - GetParagraph()->m_AreaRect.left;
		rect.right +=  GetParagraph()->m_AreaRect.right - GetParagraph()->m_ContentRect.right;
		rect.top += GetParagraph()->m_ContentRect.top - GetParagraph()->m_AreaRect.top;

		rect.top += this->m_AreaRect.top;
		rect.bottom = rect.top + this->m_nHeight - 1;
		return 1;
	}
	int  XWordSheetSvr::CBlock::GetIndexFromParent()
	{
		XWordSheetSvr::CParagraph * pParagraph;
		pParagraph = this->GetParagraph();
		for(int i=0;i<pParagraph->m_Blocks.size();i++)
		{
			if(this ==pParagraph->m_Blocks[i]) return i;
		}
		return -1;
	}

	void * XWordSheetSvr::CBlock::GetNextObject()
	{
		int index = 0;
		CParagraph*  pParagraph;
		index = this->GetIndexFromParent();
		pParagraph = GetParagraph();
		if(!pParagraph) return NULL;
		for(int i=index + 1;i< pParagraph ->m_Blocks.size();i++)
		{
			if(pParagraph->m_Blocks[i]) return pParagraph->m_Blocks[i];
		}
		while(true)
		{
			pParagraph = (CParagraph *)pParagraph->GetNextObject();
			if(!pParagraph) break;
			for(int i=0;i< pParagraph ->m_Blocks.size();i++)
			{
				if(pParagraph->m_Blocks[i]) return pParagraph->m_Blocks[i];
			}
		}
		return NULL;
	}

	void * XWordSheetSvr::CBlock::GetPeriorObject()
	{
		int index = 0;
		CParagraph*  pParagraph;
		index = this->GetIndexFromParent();
		pParagraph = GetParagraph();
		if(!pParagraph) return NULL;
		for(int i=index - 1;i>= 0;i--)
		{
			if(pParagraph->m_Blocks[i]) return pParagraph->m_Blocks[i];
		}
		while(true)
		{
			pParagraph = (CParagraph *)pParagraph->GetPeriorObject();
			if(!pParagraph) break;
			for(int i= pParagraph ->m_Blocks.size() - 1;i>=0; i--)
			{
				if(pParagraph->m_Blocks[i]) return pParagraph->m_Blocks[i];
			}
		}
		return NULL;
	}



	int XWordSheetSvr::CBlock::GetCaretPoint(POINT &pt)
	{
		RECT rect;
		GetParagraph()->GetRect(rect);
		rect.left += GetParagraph()->m_ContentRect.left - GetParagraph()->m_AreaRect.left;
		rect.right +=  GetParagraph()->m_AreaRect.right - GetParagraph()->m_ContentRect.right;
		rect.top += GetParagraph()->m_ContentRect.top - GetParagraph()->m_AreaRect.top;


		int nFit = m_RectStrPos[this->m_nCurIndex];
		if(!m_pDx)
		{
			HDC hDC = GetDC(NULL);
			LOGFONT	logFont;
			HFONT	hFont;
			GetFont(logFont);
			hFont = ::CreateFontIndirect(&logFont);
			hFont = (HFONT)::SelectObject(hDC,hFont);

			LPTSTR	pStr = this->m_strText;
			SIZE	sz;
			int width = m_Rects[m_nCurIndex].right - m_Rects[m_nCurIndex].left + 1;
			for(int i=0;i<m_nCurIndex;i++)
			{
				pStr += this->m_RectStrPos[i];
			}
			m_pDx = new int[nFit];
			GetTextExtentExPoint(hDC,pStr,nFit,width,&nFit,m_pDx,&sz);

			TEXTMETRIC tm;
			::GetTextMetrics(hDC,&tm);
			m_nCaretHeight =tm.tmHeight + tm.tmExternalLeading;

			hFont = (HFONT)::SelectObject(hDC,hFont);
			::DeleteObject(hFont);
			::ReleaseDC(NULL,hDC);
		}
		if(m_nCurPos==0)
			m_nCurPoint = 0;
		else
			m_nCurPoint = m_pDx[m_nCurPos - 1];

		pt.x = rect.left + this->m_Rects[this->m_nCurIndex].left + this->m_nCurPoint;
		pt.y = rect.top + this->m_Rects[this->m_nCurIndex].bottom - (this->m_nCaretHeight);

		return 1;
	}

	XWordSheetSvr::CParagraph * XWordSheetSvr::CBlock::GetParagraph()
	{
		return (CParagraph *)GetParent();
	}

	int XWordSheetSvr::CBlock::SetFont(LOGFONT &LogFont)
	{
		return 1;
	}
	int XWordSheetSvr::CBlock::SetCssStyle(LPTSTR cssStyle)
	{
		return 1;
	}
	int XWordSheetSvr::CBlock::MargeCssStyle(LPTSTR cssStyle)
	{
		return 1;
	}
	int XWordSheetSvr::CBlock::GetFont(LOGFONT &LogFont)
	{
		HDC hdc = ::GetDC(NULL);

		memset( &LogFont,0,sizeof(LOGFONT));
		LogFont.lfHeight =  MulDiv(m_nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		LogFont.lfCharSet = DEFAULT_CHARSET ;
		LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		LogFont.lfEscapement = 0;
		_tcscpy_s(LogFont.lfFaceName ,_tcslen( _T("宋体")) + 1, _T("宋体"));
		LogFont.lfItalic = 0;
		LogFont.lfOrientation = 0;
		LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		LogFont.lfPitchAndFamily = DEFAULT_PITCH;
		LogFont.lfQuality = CLEARTYPE_QUALITY;
		LogFont.lfStrikeOut = 0;
		LogFont.lfUnderline = 0;
		LogFont.lfWeight = FW_NORMAL;
		LogFont.lfWidth = 0;

		ReleaseDC(NULL,hdc);
		return 1;
	}

	int XWordSheetSvr::CBlock::DoDraw(HDC hPaintDC,POINT ptOffset)
	{

		LOGFONT	logFont;
		HFONT	hFont;
		int	nFit;
		this->GetFont(logFont);
		hFont = ::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);

		int nBkMode = ::SetBkMode(hPaintDC,TRANSPARENT);

		TEXTMETRIC tm;
		::GetTextMetrics(hPaintDC,&tm);
		int rheight=tm.tmHeight + tm.tmExternalLeading;

		LPTSTR pStr;
		pStr = m_strText;

		RECT rc = this->GetParagraph()->m_ContentRect;
		::OffsetRect(&rc,ptOffset.x,ptOffset.y);
		for(int i=0;i< m_Rects.size();i++)
		{
			nFit = this->m_RectStrPos[i];
			ExtTextOut(hPaintDC,rc.left + m_Rects[i].left,rc.top + m_Rects[i].top + m_Rects[i].bottom - m_Rects[i].top + 1 - rheight,0,NULL,pStr,nFit,NULL);
			pStr += nFit;
		}
		SetBkMode(hPaintDC,nBkMode);

		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);
		return 1;
	}

	bool XWordSheetSvr::CBlock::AdjustExtent(HDC hPaintDC)
	{
		m_bRecalcPosition = false;
		return true;
	}

	XWordSheetSvr::CParagraph::CParagraph():nPreSpaceOfFirstRow(28)
	{
	}


	XWordSheetSvr::CParagraph::~CParagraph()
	{
		for(int i=0;i<m_Blocks.size();i++)
		{
			if(m_Blocks[i]) delete m_Blocks[i];
		}
		m_Blocks.clear();
	}

	XWordSheetSvr::CBlock * XWordSheetSvr::CParagraph::GetObjectAtPoint(POINT pt)
	{
		pt.x -= m_ContentRect.left;
		pt.y -= m_ContentRect.top;
		CBlock * pBlock;
		for(int i=0;i<this->m_Blocks.size();i++)
		{
			if(pt.y < m_Blocks[i]->m_AreaRect.top) break;
			pBlock = m_Blocks[i]->GetObjectAtPoint(pt);
			if(pBlock) return pBlock;
		}
		return NULL;
	}

	int XWordSheetSvr::CParagraph::GetRect(RECT &rect)
	{
		GetChapter()->GetRect(rect);
		rect.left += GetChapter()->m_ContentRect.left - GetChapter()->m_AreaRect.left;
		rect.right +=  GetChapter()->m_AreaRect.right - GetChapter()->m_ContentRect.right;
		rect.top += GetChapter()->m_ContentRect.top - GetChapter()->m_AreaRect.top;
		for(int i=0;i< GetChapter()->m_Paragraph.size();i++)
		{
			if(GetChapter()->m_Paragraph[i] !=this)
				rect.top += GetChapter()->m_Paragraph[i]->m_nHeight;
			else
			{
				rect.bottom = rect.top + m_nHeight -1;
				return 1;
			}
		}
		return 1;
	}

	void * XWordSheetSvr::CParagraph::GetNextObject()
	{
		int index = 0;
		CChapter*  pChapter;
		index = this->GetIndexFromParent();
		pChapter = GetChapter();
		if(!pChapter) return NULL;
		for(int i=index + 1;i< pChapter ->m_Paragraph.size();i++)
		{
			if(pChapter->m_Paragraph[i]) return pChapter->m_Paragraph[i];
		}
		while(true)
		{
			pChapter = (CChapter *)pChapter->GetNextObject();
			if(!pChapter) break;
			for(int i=0;i< pChapter ->m_Paragraph.size();i++)
			{
				if(pChapter->m_Paragraph[i]) return pChapter->m_Paragraph[i];
			}
		}
		return NULL;
	}

	void * XWordSheetSvr::CParagraph::GetPeriorObject()
	{
		int index = 0;
		CChapter*  pChapter;
		index = this->GetIndexFromParent();
		pChapter = GetChapter();
		if(!pChapter) return NULL;
		for(int i=index - 1;i>= 0;i--)
		{
			if(pChapter->m_Paragraph[i]) return pChapter->m_Paragraph[i];
		}
		while(true)
		{
			pChapter = (CChapter *)pChapter->GetPeriorObject();
			if(!pChapter) break;
			for(int i= pChapter ->m_Paragraph.size() - 1;i>=0; i--)
			{
				if(pChapter->m_Paragraph[i]) return pChapter->m_Paragraph[i];
			}
		}
		return NULL;
	}


	int  XWordSheetSvr::CParagraph::GetIndexFromParent()
	{
		XWordSheetSvr::CChapter * pChapter;
		pChapter = this->GetChapter();
		for(int i=0;i<pChapter->m_Paragraph.size();i++)
		{
			if(this ==pChapter->m_Paragraph[i]) return i;
		}
		return -1;
	}

	int XWordSheetSvr::CParagraph::DoDraw(HDC hPaintDC,POINT ptOffset)
	{
		ptOffset.x += m_ContentRect.left;
		ptOffset.y += m_ContentRect.top;

		/*
		RECT	rect;
		for(int i=0;i<m_Blocks.size();i++)
		{
		for(int j=0;j<m_Blocks[i]->m_Rects.size();j++)
		{
		rect = m_Blocks[i]->m_Rects[j];
		::OffsetRect(&rect,ptOffset.x,ptOffset.y );
		::Rectangle(hPaintDC,rect.left,rect.top,rect.right,rect.bottom);  
		}
		}
		*/

		for(int i=0;i<m_Blocks.size();i++)
		{
			m_Blocks[i]->DoDraw(hPaintDC,ptOffset);
		}
		return 1;
	}

	int XWordSheetSvr::CParagraph::GetHeight()
	{
		if(m_bRecalcPosition)
			CBaseBlock::AdjustExtent();
		return m_nHeight;
	}

	bool XWordSheetSvr::CParagraph::AdjustExtent(HDC hPaintDC)
	{
		LOGFONT	logFont;
		HFONT	hFont;

		POINT	pt;
		int width;
		SIZE size;
		int	nFit = 0;
		int Strlen;
		RECT rc;
		LPTSTR pStr;
		int i;
		int count =0;
		bool	bBreakRow = false;
		int	height=0,heightex=0;
		int startY=0;
		TEXTMETRIC tm;
		int rheight = 0;

		Hxsoft::XUtil::xuVector<RECT *> rects;

		pt.x = this->nPreSpaceOfFirstRow;
		pt.y = 0;
		width = m_ContentRect.right - m_ContentRect.left + 1;
		width -= pt.x;
		for(i=0;i<this->m_Blocks.size();i++)
		{
			pStr = m_Blocks[i]->m_strText;
			if((int)_tcslen(pStr) <1)continue;
			m_Blocks[i]->GetFont(logFont);
			hFont = ::CreateFontIndirect(&logFont);
			hFont = (HFONT)::SelectObject(hPaintDC,hFont);

			::GetTextMetrics(hPaintDC,&tm);
			rheight=tm.tmHeight + tm.tmExternalLeading;
			rheight = rheight > m_Blocks[i]->m_nRowHeight?rheight:m_Blocks[i]->m_nRowHeight;

			m_Blocks[i]->m_Rects.clear();
			m_Blocks[i]->m_RectStrPos.clear();

			rc.top = rc.bottom = 0;
			nFit = 0;
			while(true)
			{
				rc.left = pt.x;
				Strlen = (int)_tcslen(pStr);
				GetTextExtentExPoint(hPaintDC,pStr,Strlen,width,&nFit,NULL,&size);
				nFit = nFit < Strlen ? nFit : Strlen;
				if(nFit < 1)
				{
					for(int k=0;k<rects.size();k++)
					{
						rects[k]->top=startY;
						rects[k]->bottom = startY + height - 1;
					}
					rects.clear();
					bBreakRow = false;
					startY += height;
					height = 0;

					pt.x = 0;
					width = m_ContentRect.right - m_ContentRect.left + 1 - pt.x;
					GetTextExtentExPoint(hPaintDC,pStr,Strlen,width,&nFit,NULL,&size);
					nFit = nFit < (int)_tcslen(pStr) ? nFit : Strlen;
					rc.left = pt.x;

					//未处理整个区域放不下一个字符
					if(nFit < 1) nFit=1;
				}
				if( nFit >= Strlen)
				{
					pt.x += size.cx;
					rc.right = pt.x - 1;
				}else
				{
					bBreakRow = true;
					pt.x = 0;
					rc.right = m_ContentRect.right - m_ContentRect.left;
				}
				width = m_ContentRect.right - m_ContentRect.left + 1 - pt.x;
				m_Blocks[i]->m_Rects.push_back(rc);
				m_Blocks[i]->m_RectStrPos.push_back(nFit);
				rects.push_back(&(m_Blocks[i]->m_Rects[m_Blocks[i]->m_Rects.size() -1]));

				heightex = rheight;
				height = height> heightex?height:heightex;
				count++;
				if(bBreakRow)
				{
					for(int k=0;k<rects.size();k++)
					{
						rects[k]->top=startY;
						rects[k]->bottom = startY + height - 1;
					}
					rects.clear();
					startY += height;
					height = rheight;
					bBreakRow = false;
				}
				pStr += nFit; 
				if(pStr[0] =='\0' || nFit >=Strlen )break;
			}
			hFont = (HFONT)::SelectObject(hPaintDC,hFont);
			DeleteObject(hFont);				
		}
		for(int k=0;k<rects.size();k++)
		{
			rects[k]->top=startY;
			rects[k]->bottom = startY + height - 1;
		}
		rects.clear();

		//adjust block AreaRect top and rect bottom
		for(int i=0;i<this->m_Blocks.size();i++)
		{
			if(m_Blocks[i]->m_Rects.size() >0)
			{
				m_Blocks[i]->m_AreaRect.left = m_ContentRect.left;
				m_Blocks[i]->m_AreaRect.right = m_ContentRect.right;
				m_Blocks[i]->m_AreaRect.top = m_Blocks[i]->m_Rects[0].top;
				m_Blocks[i]->m_AreaRect.bottom = m_Blocks[i]->m_Rects[m_Blocks[i]->m_Rects.size() - 1].bottom;
				m_Blocks[i]->m_nHeight = m_Blocks[i]->m_AreaRect.bottom - m_Blocks[i]->m_AreaRect.top + 1;
			}else
			{
				::SetRect(&m_Blocks[i]->m_AreaRect,0,0,0,0);
			}
			m_Blocks[i]->CalcContentRect();
		}

		if(this->m_Blocks.size() >0)
			m_nHeight = m_Blocks[m_Blocks.size() - 1]->m_AreaRect.bottom + 1;
		else
			m_nHeight = 0;

		m_nHeight += m_ContentRect.top - m_AreaRect.top ;
		m_nHeight += m_AreaRect.bottom - m_ContentRect.bottom;

		m_bRecalcPosition = false;
		return true;
	}

	XWordSheetSvr::CChapter * XWordSheetSvr::CParagraph::GetChapter()
	{
		return (XWordSheetSvr::CChapter *)GetParent();
	}
	XWordSheetSvr::CChapter::CChapter()
	{
	}

	XWordSheetSvr::CChapter::~CChapter()
	{

	}

	int XWordSheetSvr::CChapter::DoDraw(HDC hPaintDC, POINT ptOffset)
	{
		ptOffset.x = m_ContentRect.left;
		ptOffset.y = m_ContentRect.top;
		for(int i=0;i<m_Paragraph.size();i++)
		{
			if(m_Paragraph[i])m_Paragraph[i]->DoDraw(hPaintDC,ptOffset);
			ptOffset.y += m_Paragraph[i]->GetHeight();
		}
		return 1;
	}

	bool XWordSheetSvr::CChapter::AdjustExtent(HDC hPaintDC)
	{
		m_nHeight = 0;
		for(int i=0;i<this->m_Paragraph.size();i++)
		{
			m_Paragraph[i]->m_AreaRect.left  =  0;
			m_Paragraph[i]->m_AreaRect.right =  m_ContentRect.right - m_ContentRect.left;
			m_Paragraph[i]->m_AreaRect.top  =  0;
			m_Paragraph[i]->m_AreaRect.bottom;
			m_Paragraph[i]->CalcContentRect();
			m_Paragraph[i]->AdjustExtent(hPaintDC);
			m_nHeight += m_Paragraph[i]->m_nHeight;
		}
		m_nHeight += m_ContentRect.top - m_AreaRect.top ;
		m_nHeight += m_AreaRect.bottom - m_ContentRect.bottom;
		m_bRecalcPosition = false;
		return true;
	}

	XWordSheetSvr::CBlock * XWordSheetSvr::CChapter::GetObjectAtPoint(POINT pt)
	{
		pt.x -= m_ContentRect.left;
		pt.y -= m_ContentRect.top;
		for(int i=0;i<m_Paragraph.size();i++)
		{
			if(pt.y > m_Paragraph[i]->m_nHeight)
			{
				pt.y -= m_Paragraph[i]->m_nHeight;
			}else
				return m_Paragraph[i]->GetObjectAtPoint(pt);
		}
		return NULL;
	}

	int XWordSheetSvr::CChapter::GetRect(RECT &rect)
	{
		GetDocument()->GetRect(rect);
		rect.left += GetDocument()->m_ContentRect.left - GetDocument()->m_AreaRect.left;
		rect.right +=  GetDocument()->m_AreaRect.right - GetDocument()->m_ContentRect.right;
		rect.top += GetDocument()->m_ContentRect.top - GetDocument()->m_AreaRect.top;
		for(int i=0;i< GetDocument()->m_Chapters.size();i++)
		{
			if(GetDocument()->m_Chapters[i] !=this)
				rect.top += GetDocument()->m_Chapters[i]->m_nHeight;
			else
			{
				rect.bottom = rect.top + m_nHeight -1;
				return 1;
			}
		}
		return 1;
	}

	int  XWordSheetSvr::CChapter::GetIndexFromParent()
	{
		XWordSheetSvr::CDocument * pDocument;
		pDocument = this->GetDocument();
		for(int i=0;i<pDocument->m_Chapters.size();i++)
		{
			if(this ==pDocument->m_Chapters[i]) return i;
		}
		return -1;
	}

	void * XWordSheetSvr::CChapter::GetNextObject()
	{
		int index = 0;
		CDocument*  pDocument;
		index = this->GetIndexFromParent();
		pDocument = GetDocument();
		if(!pDocument) return NULL;
		for(int i=index + 1;i< pDocument ->m_Chapters.size();i++)
		{
			if(pDocument->m_Chapters[i]) return pDocument->m_Chapters[i];
		}
		while(true)
		{
			pDocument = (CDocument *)pDocument->GetNextObject();
			if(!pDocument) break;
			for(int i=0;i< pDocument ->m_Chapters.size();i++)
			{
				if(pDocument->m_Chapters[i]) return pDocument->m_Chapters[i];
			}
		}
		return NULL;
	}

	void * XWordSheetSvr::CChapter::GetPeriorObject()
	{
		int index = 0;
		CDocument*  pDocument;
		index = this->GetIndexFromParent();
		pDocument = GetDocument();
		if(!pDocument) return NULL;
		for(int i=index - 1;i>= 0;i--)
		{
			if(pDocument->m_Chapters[i]) return pDocument->m_Chapters[i];
		}
		while(true)
		{
			pDocument = (CDocument *)pDocument->GetPeriorObject();
			if(!pDocument) break;
			for(int i= pDocument ->m_Chapters.size() - 1;i>=0; i--)
			{
				if(pDocument->m_Chapters[i]) return pDocument->m_Chapters[i];
			}
		}
		return NULL;
	}

	XWordSheetSvr::CDocument * XWordSheetSvr::CChapter::GetDocument()
	{
		return (XWordSheetSvr::CDocument *)GetParent();
	}


	XWordSheetSvr::CDocument::CDocument():m_strFile(NULL),m_nCurPage(-1)
	{
		m_pPage = new CPage();
	}

	XWordSheetSvr::CDocument::~CDocument()
	{
		if(m_pPage) delete m_pPage;
	}


	XWordSheetSvr::CBlock * XWordSheetSvr::CDocument::GetObjectAtPoint(POINT pt)
	{
		pt.x -= m_ContentRect.left;
		pt.y -= m_ContentRect.top;
		for(int i=0;i<this->m_Chapters.size();i++)
		{
			if(pt.y > m_Chapters[i]->m_nHeight)
			{
				pt.y -= m_Chapters[i]->m_nHeight;
			}else
				return m_Chapters[i]->GetObjectAtPoint(pt);
		}
		return NULL;
	}

	int XWordSheetSvr::CDocument::GetRect(RECT &rect)
	{
		rect = m_AreaRect;
		rect.top = m_AreaRect.top;
		rect.bottom = m_AreaRect.top + this->m_nHeight - 1;

		return 1;
	}

	void * XWordSheetSvr::CDocument::GetNextObject()
	{
		return NULL;
	}

	void * XWordSheetSvr::CDocument::GetPeriorObject()
	{
		return NULL;
	}

	int  XWordSheetSvr::CDocument::GetIndexFromParent()
	{
		return 0;
	}

	bool XWordSheetSvr::CDocument::AdjustExtent(HDC hPaintDC)
	{
		m_nHeight = 0;
		for(int i=0;i<this->m_Chapters.size();i++)
		{

			m_Chapters[i]->m_AreaRect.left  =  0;
			m_Chapters[i]->m_AreaRect.right =  m_ContentRect.right - m_ContentRect.left;
			m_Chapters[i]->m_AreaRect.top  =  0;
			m_Chapters[i]->m_AreaRect.bottom  =  0;
			m_Chapters[i]->CalcContentRect();
			m_Chapters[i]->AdjustExtent(hPaintDC);
			m_nHeight += m_Chapters[i]->m_nHeight;
		}
		m_nHeight += m_ContentRect.top - m_AreaRect.top ;
		m_nHeight += m_AreaRect.bottom - m_ContentRect.bottom;
		m_bRecalcPosition = false;
		return true;
	}

	int XWordSheetSvr::CDocument::DoDraw(HDC hPaintDC,POINT ptOffset)
	{
		ptOffset.x = m_ContentRect.left;
		ptOffset.y = m_ContentRect.top;
		for(int i=0;i< m_Chapters.size();i++)
		{
			if(m_Chapters[i])
			{
				m_Chapters[i]->DoDraw(hPaintDC,ptOffset);
				ptOffset.y += m_Chapters[i]->GetHeight();
			}
		}
		return 1;
	}

	XWordSheetSvr::XWordSheetSvr():m_nCurrentDoc(-1),nCaretHeight(17),m_pWordLayer(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
		nCaretPosX = 0;
		nCaretPosY = 0;
		m_ptOffset.x = 0;
		m_ptOffset.y = 0;

		m_pSerializeSvr = new XWordSerializeSvr;
		m_pDataSvr = new XWordDataSvr;
		m_pWordLayer = new XWordLayer;

		/*
		IXMLDOMElement *	pElement;
		XWordSerializeSvr * pSerializeSvr = new XWordSerializeSvr();
		xbXml xml;
		xml.Load(_T("xdoc\\test.xml"));
		xml.m_pXmlDoc->get_documentElement(&pElement);

		if(!m_pWordData) m_pWordData = new XWordDataSvr;
		pSerializeSvr->LoadSheetData(pElement,m_pWordData);
		delete pSerializeSvr;
		pElement->Release();
		if(m_pWordData->m_pContentSvr)
		{
			m_pWordLayer->m_pWordData = m_pWordData;
			m_pWordLayer->m_pWordDocumentBlock->LoadData(m_pWordData->m_pContentSvr->m_pXWordDataDocument );
			m_pWordLayer->m_pWordDocumentBlock->AdjustExtent();
		}
		*/

		/*
		CDocument  *pDoc = new CDocument();
		CChapter   *pChapter = new CChapter();
		CParagraph *pParagraph = new CParagraph();
		CBlock	   *pBlock = new CBlock();

		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);

		pParagraph->m_pParent = pChapter; 
		pChapter->m_Paragraph.push_back(pParagraph);

		pChapter->m_pParent = pDoc;
		pDoc->m_Chapters.push_back(pChapter);
		m_Documents.push_back(pDoc);

		pBlock->m_strText = ::_tcsdup(_T("国际工作流管理**"));
		pBlock->m_nFontSize = -24;

		pBlock = new CBlock();
		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);
		pBlock->m_strText = ::_tcsdup(_T("联盟（Workflow Management Coalition，WfMC）的成立标志着工作流"));

		pBlock = new CBlock();
		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);
		pBlock->m_strText = ::_tcsdup(_T("技术开始进入相对"));
		pBlock->m_nFontSize = -32;

		pBlock = new CBlock();
		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);
		pBlock->m_strText = ::_tcsdup(_T("成熟的阶段。WfMC对工作流管理系统中的相关术语、体系结构及应用编程接口等方面制定了"));
		pBlock->m_nFontSize = -17;


		pParagraph = new CParagraph();
		pParagraph->m_pParent = pChapter;
		pChapter->m_Paragraph.push_back(pParagraph);

		pBlock = new CBlock();
		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);
		pBlock->m_strText = ::_tcsdup(_T("一系列标准，并给出了工作流定义：工作流是指整个或部分经营过程在计算机支持下的全自动或半"));
		pBlock->m_nFontSize = -9;

		pBlock = new CBlock();
		pBlock->m_pParent = pParagraph;
		pParagraph->m_Blocks.push_back(pBlock);
		pBlock->m_strText = ::_tcsdup(_T("自动化。可以更广泛地把凡是由计算机软件系统（工作流管理系统）控制其执行的过程都称为工作流"));
		pBlock->m_nFontSize = -14;
		
		this->m_nCurrentDoc = 0;*/
	}

	XWordSheetSvr::~XWordSheetSvr()
	{
		for(int i=0;i<m_Documents.size();i++)
		{
			if(m_Documents[i]) delete m_Documents[i];
		}
		m_Documents.clear();
		if(m_pWordLayer) delete m_pWordLayer;

		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
	}


	int XWordSheetSvr::HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
	{
		switch (message) 
		{ 
		case WM_CHAR:
			{
				UINT nChar;
				UINT nRepCnt;
				UINT nFlags ;

				nChar = (UINT)wParam;
				nRepCnt =(SHORT) LOWORD(lParam);
				nFlags = (UINT)lParam;
				OnChar(nChar,nRepCnt,nFlags);
				break;
			}
		case WM_SIZE:
			{
				UINT	nFlags;
				POINT	point;
				nFlags = (UINT)wParam; 
				point.x = (SHORT)LOWORD(lParam); 
				point.y = (SHORT)HIWORD(lParam);
				OnSize((UINT)wParam,point);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;
				this->OnLButtonDown((UINT)dwFlag,point);
				break;
			}
		case WM_LBUTTONUP:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;
				this->OnLButtonUp((UINT)dwFlag,point);
				break;
			}
		case WM_MOUSEMOVE:
			{
				POINT point;
				UINT_PTR	dwFlag;
				dwFlag = wParam;
				point.x = MAKEPOINTS(lParam).x;
				point.y = MAKEPOINTS(lParam).y;

				this->OnMouseMove((UINT)dwFlag,point);
				break;
			}
		case WM_KEYDOWN: 
			switch (wParam) 
			{
				/*
				case VK_HOME:       // Home 
				nCaretPosX = 0; 
				break; 

				case VK_END:        // End 
				nCaretPosX = nWindowCharsX - 1; 
				break; 

				case VK_PRIOR:      // Page Up 
				nCaretPosY = 0; 
				break; 

				case VK_NEXT:       // Page Down 
				nCaretPosY = nWindowCharsY -1; 
				break; 
				*/
			case VK_LEFT:       // Left arrow 
				if(m_pBlock)
				{
					if(m_pBlock->m_nCurPos>0)
					{
						m_pBlock->m_nCurPos--;
						this->MoveCaret();
					}else if(m_pBlock->m_nCurIndex >0)
					{
						m_pBlock->m_nCurIndex--;
						m_pBlock->m_nCurPos = this->m_pBlock->m_RectStrPos[m_pBlock->m_nCurIndex];
						if(m_pBlock->m_pDx)
						{
							delete[] m_pBlock->m_pDx;
							m_pBlock->m_pDx = NULL;
						}
						this->MoveCaret();
					}else
					{
						CBlock * pBlock;
						pBlock = (CBlock *)m_pBlock->GetPeriorObject();
						if(pBlock)
						{
							m_pBlock = pBlock;
							m_pBlock->m_nCurIndex = m_pBlock->m_Rects.size() - 1;
							m_pBlock->m_nCurPos = m_pBlock->m_RectStrPos[m_pBlock->m_nCurIndex];
							if(m_pBlock->m_pDx)
							{
								delete[] m_pBlock->m_pDx;
								m_pBlock->m_pDx = NULL;
							}
							this->MoveCaret(true);
						}
					}
				}
				break; 

			case VK_RIGHT:      // Right arrow 
				if(m_pBlock)
				{
					if(m_pBlock->m_nCurPos< m_pBlock->m_RectStrPos[m_pBlock->m_nCurIndex])
					{
						m_pBlock->m_nCurPos++;
						this->MoveCaret();
					}else if(m_pBlock->m_nCurIndex < m_pBlock->m_Rects.size() - 1)
					{
						m_pBlock->m_nCurIndex++;
						m_pBlock->m_nCurPos = 0;
						if(m_pBlock->m_pDx)
						{
							delete[] m_pBlock->m_pDx;
							m_pBlock->m_pDx = NULL;
						}
						this->MoveCaret();
					}else
					{
						CBlock * pBlock;
						pBlock = (CBlock *)m_pBlock->GetNextObject();
						if(pBlock)
						{
							m_pBlock = pBlock;
							m_pBlock->m_nCurIndex = 0;
							m_pBlock->m_nCurPos = 0;
							if(m_pBlock->m_pDx)
							{
								delete[] m_pBlock->m_pDx;
								m_pBlock->m_pDx = NULL;
							}
							this->MoveCaret(true);
						}
					}
				}
				break; 

			case VK_UP:         // Up arrow 
				if(m_pBlock)
				{
					CBlock * pBlock;
					RECT rect;
					POINT pt;
					if(m_pBlock->m_nCurIndex > 0)
						pBlock = m_pBlock;
					else
					{
						pBlock = (CBlock *)m_pBlock->GetPeriorObject();
						while(pBlock)
						{
							if(pBlock->m_Rects.size() > 1) break;
							if(pBlock->GetParagraph() != m_pBlock->GetParagraph())break;
							if(pBlock->m_AreaRect.bottom < m_pBlock->m_AreaRect.top)break;
							pBlock = (CBlock *)pBlock->GetPeriorObject();
						}
					}
					if(pBlock)
					{
						pBlock->GetRect(rect);
						if(pBlock ==m_pBlock)
						{
							rect.bottom -= m_pBlock->m_Rects[m_pBlock->m_Rects.size() - 1].bottom -
								m_pBlock->m_Rects[m_pBlock->m_nCurIndex -1].bottom; 
						}else if(pBlock->GetParagraph() == m_pBlock->GetParagraph())
						{
							if(m_pBlock->m_Rects[m_pBlock->m_nCurIndex].bottom == pBlock->m_Rects[pBlock->m_Rects.size() - 1].bottom)
							{
								rect.bottom -= pBlock->m_Rects[pBlock->m_Rects.size() - 1].bottom - pBlock->m_Rects[pBlock->m_Rects.size() - 2].bottom;
							}
						}
						pt.x = nCaretPosX;
						pt.y = rect.bottom - 5;
						CBlock * pBlockEx = m_Documents[m_nCurrentDoc]->GetObjectAtPoint(pt);
						if(pBlockEx)
						{
							m_pBlock = pBlockEx;
							this->MoveCaret(true);
						}else
						{
							if(pt.x < rect.left + (pBlock !=m_pBlock?pBlock->m_Rects[pBlock->m_Rects.size() - 1].left:m_pBlock->m_Rects[m_pBlock->m_nCurIndex - 1].left))
							{
								if(pBlock !=m_pBlock )
								{
									pBlock->m_nCurIndex = pBlock->m_Rects.size() - 1;
									pBlock->m_nCurPos = 0;
								}else
								{
									pBlock->m_nCurIndex = pBlock->m_nCurIndex - 1;
									pBlock->m_nCurPos = 0;
								}
							}else
							{
								pBlock->m_nCurIndex = pBlock->m_Rects.size() - 1;
								pBlock->m_nCurPos = pBlock->m_RectStrPos[pBlock->m_nCurIndex];
							}
							if(pBlock->m_pDx)
							{
								delete pBlock->m_pDx;
								pBlock->m_pDx = NULL;
							}
							m_pBlock = pBlock;
							this->MoveCaret(true);
							//set last
						}
					}
				}
				break; 

			case VK_DOWN:       // Down arrow 
				if(m_pBlock)
				{
					CBlock * pBlock;
					RECT rect;
					POINT pt;
					if(m_pBlock->m_nCurIndex < m_pBlock->m_Rects.size() - 1)
						pBlock = m_pBlock;
					else
					{
						pBlock = (CBlock *)m_pBlock->GetNextObject();
						while(pBlock)
						{
							if(pBlock->m_Rects.size() > 1) break;
							if(pBlock->GetParagraph() != m_pBlock->GetParagraph())break;
							if(pBlock->m_AreaRect.top > m_pBlock->m_AreaRect.top)break;
							pBlock = (CBlock *)pBlock->GetNextObject();
						}
					}
					if(pBlock)
					{
						pBlock->GetRect(rect);
						if(pBlock == m_pBlock)
						{
							rect.top += m_pBlock->m_Rects[m_pBlock->m_nCurIndex + 1].top -
								m_pBlock->m_Rects[0].top;
						}else if(pBlock->GetParagraph() == m_pBlock->GetParagraph())
						{
							if(m_pBlock->m_Rects[m_pBlock->m_nCurIndex].top == pBlock->m_Rects[0].top)
							{
								rect.top += pBlock->m_Rects[1].top - pBlock->m_Rects[0].top;
							}
						}
						pt.x = nCaretPosX;
						pt.y = rect.top + 5;
						CBlock * pBlockEx = m_Documents[m_nCurrentDoc]->GetObjectAtPoint(pt);
						if(pBlockEx)
						{
							m_pBlock = pBlockEx;
							this->MoveCaret(true);
						}else
						{
							if(pt.x < rect.left + (pBlock !=m_pBlock?pBlock->m_Rects[0].left:m_pBlock->m_Rects[m_pBlock->m_nCurIndex].left))
							{
								if(pBlock !=m_pBlock )
								{
									pBlock->m_nCurIndex = 0;
									pBlock->m_nCurPos = 0;
								}else
								{
									pBlock->m_nCurIndex = pBlock->m_nCurIndex + 1;
									pBlock->m_nCurPos = 0;
								}
							}else
							{
								pBlock->m_nCurIndex = pBlock->m_Rects.size() - 1;
								pBlock->m_nCurPos = pBlock->m_RectStrPos[pBlock->m_nCurIndex];
							}
							if(pBlock->m_pDx)
							{
								delete pBlock->m_pDx;
								pBlock->m_pDx = NULL;
							}
							m_pBlock = pBlock;
							this->MoveCaret(true);
							//set last
						}
					}
				}
				break; 

			case VK_DELETE:     // Delete 
				if(m_pBlock)
				{
					int len = (int)::_tcslen(m_pBlock->m_strText);
					int pos = 0;
					for(int i=0;i< m_pBlock->m_nCurIndex;i++)
					{
						pos += m_pBlock->m_RectStrPos[i];
					}
					pos += m_pBlock->m_nCurPos ;
					m_pBlock->m_strText[pos] = '\0';
					if(pos < len - 1)
						::_tcscat_s(m_pBlock->m_strText,len +1, m_pBlock->m_strText + pos + 1);

					int curIndex,curPos,caretHeight;
					curIndex = m_pBlock->m_nCurIndex;
					curPos   = m_pBlock->m_nCurPos;
					caretHeight = m_pBlock->m_nCaretHeight;

					int index = m_pBlock->GetIndexFromParent();
					CParagraph * pParagraph = m_pBlock->GetParagraph();
					((CBaseBlock *)m_pBlock->GetParagraph())->AdjustExtent();
					this->m_pBlock = pParagraph->m_Blocks[index];

					m_pBlock->m_nCurIndex = curIndex;
					m_pBlock->m_nCurPos = curPos;
					m_pBlock->m_nCaretHeight = caretHeight;
					if(m_pBlock->m_pDx)
					{
						delete m_pBlock->m_pDx;
						m_pBlock->m_pDx = NULL;
					}
					::InvalidateRect(m_pSheetCtrl->m_hWnd,&m_pSheetCtrl->m_pxfNode->m_ContentRect,false);;
				}
				break; 
			} 

			// Adjust the caret position based on the 
			// virtual-key processing. 
			/*
			SetCaretPos(nCaretPosX * nCharX, 
			nCaretPosY * nCharY); */

			return 0; 

		case WM_SETFOCUS: 
			// The window has the input focus. Load the 
			// application-defined caret resource. 

			/*
			hCaret = LoadBitmap(hinst, MAKEINTRESOURCE(120)); 

			// Create the caret. 
			*/
			CreateCaret(hWnd, NULL, 2, this->nCaretHeight); 


			// Adjust the caret position. 

			//SetCaretPos(nCaretPosX * nCharX, 
			//	nCaretPosY * nCharY); 
			SetCaretPos(nCaretPosX ,nCaretPosY); 

			// Display the caret. 

			ShowCaret(hWnd); 

			break; 

		case WM_KILLFOCUS: 
			// The window is losing the input focus, 
			// so destroy the caret. 

			DestroyCaret(); 

			break; 
		}

		return 0;
	}

	void XWordSheetSvr::OnLButtonDown(UINT nFlags,POINT point)
	{
		if(m_nCurrentDoc < 0) return;

		CBlock * pBlock = this->m_Documents[m_nCurrentDoc]->GetObjectAtPoint(point);
		if(pBlock)
		{
			POINT pt;
			pBlock->GetCaretPoint(pt);
			this->nCaretPosX = pt.x;
			this->nCaretPosY = pt.y;
			this->nCaretHeight = pBlock->m_nCaretHeight;
			m_pBlock = pBlock;
		}
		if(::GetFocus()!=m_pSheetCtrl->m_hWnd)
			::SetFocus(m_pSheetCtrl->m_hWnd);
		else
		{
			CreateCaret(m_pSheetCtrl->m_hWnd, NULL, 2, this->nCaretHeight);
			SetCaretPos(nCaretPosX ,nCaretPosY); 
			ShowCaret(m_pSheetCtrl->m_hWnd);
		}
	}
	void XWordSheetSvr::OnLButtonUp(UINT nFlags,POINT point)
	{
	}
	void XWordSheetSvr::OnMouseMove(UINT nFlags,POINT point)
	{
	}
	
	int XWordSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		int nSaveDC = ::SaveDC(hPaintDC);
		if(m_pWordLayer)
		{
			RECT rc;
			if(pDrawRect)
			{
				SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
				::IntersectClipRect(hPaintDC,1,1,
					pDrawRect->right - pDrawRect->left - 1 ,
					pDrawRect->bottom - pDrawRect->top -1);
				SetMetaRgn(hPaintDC);
				rc = *pDrawRect;
				::SetRect(&rc,1,1,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
			}
			else
				rc = FRect;
			if(m_pWordLayer)
				m_pWordLayer->DoDraw(hPaintDC,&rc);
		}

		if(m_nCurrentDoc < m_Documents.size() && m_nCurrentDoc >=0)
		{
			if(m_Documents[m_nCurrentDoc])
			{
				POINT	pt;
				if(pDrawRect)
				{
					pt.x = 0;
					pt.y = 0;
					
					RECT rc;
					rc = FRect;
					if(!EqualRect(&FRect,pDrawRect))
					{
						FRect = *pDrawRect;
						this->SetFRect(*pDrawRect);
					}

					SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
					::IntersectClipRect(hPaintDC,1,1,
						pDrawRect->right - pDrawRect->left - 1 ,
						pDrawRect->bottom - pDrawRect->top -1);
					SetMetaRgn(hPaintDC);

					m_Documents[m_nCurrentDoc]->DoDraw(hPaintDC,pt);
					this->SetFRect(rc);
					
				}else
				{
					pt.x = 0;
					pt.y = 0;
					::IntersectClipRect(hPaintDC,1,1,
						FRect.right - FRect.left - 1 ,
						FRect.bottom - FRect.top -1);
					if(m_pWordLayer)
					{
						RECT rc = FRect;
						RECT rcPage;
						rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_pWordLayer->m_rcPage.right - m_pWordLayer->m_rcPage.left)/2;
						rcPage.top = rc.top + 20;
						
						POINT	pt1;
						pt1.y = rcPage.top + m_pWordLayer->m_rcPageMargin.top;
						pt1.x = rcPage.left + m_pWordLayer->m_rcPageMargin.left;

						rc.left = pt1.x ;
						rc.top  = pt1.y;
						rc.right = rc.left + m_pWordLayer->m_rcPageContent.right - m_pWordLayer->m_rcPageContent.left;
						rc.bottom = rc.top + m_pWordLayer->m_rcPageContent.bottom - m_pWordLayer->m_rcPageContent.top;

						RECT FRectEx = FRect;
						this->SetFRect(rc);

						SetViewportOrgEx(hPaintDC,pt1.x,pt1.y,NULL);
						m_Documents[m_nCurrentDoc]->DoDraw(hPaintDC,pt);
						this->SetFRect(FRect);
					}else
						m_Documents[m_nCurrentDoc]->DoDraw(hPaintDC,pt);
				}
			}
		}
		::RestoreDC(hPaintDC,nSaveDC);
		return 0;
	}

	int XWordSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		if(m_nCurrentDoc < 0) return 0;
		if(m_nCurrentDoc >= m_Documents.size()) return 0;
		m_Documents[m_nCurrentDoc]->m_AreaRect.left = FRect.left;
		m_Documents[m_nCurrentDoc]->m_AreaRect.right = FRect.right - 20;
		m_Documents[m_nCurrentDoc]->m_AreaRect.top = FRect.top;
		m_Documents[m_nCurrentDoc]->m_AreaRect.bottom = FRect.bottom - 20;
		m_Documents[m_nCurrentDoc]->CalcContentRect();
		((CBaseBlock *)m_Documents[m_nCurrentDoc])->AdjustExtent();
		return 0;
	}

	int XWordSheetSvr::OnSize(UINT nFlags, POINT point)
	{
		return 0;
	}

	void XWordSheetSvr::OnChar(UINT nChar,UINT nRepCnt,UINT nFlags)
	{

		switch (nChar) 
		{
		case 0x08:          // Backspace 
			if(m_pBlock)
			{
				int len = (int)::_tcslen(m_pBlock->m_strText);
				int pos = 0;
				for(int i=0;i< m_pBlock->m_nCurIndex;i++)
				{
					pos += m_pBlock->m_RectStrPos[i];
				}
				pos += m_pBlock->m_nCurPos ;
				m_pBlock->m_strText[pos - 1] = '\0';
				if(pos < len )
					::_tcscat_s(m_pBlock->m_strText,len +1, m_pBlock->m_strText + pos);

				int curIndex,curPos,caretHeight;
				curIndex = m_pBlock->m_nCurIndex;
				curPos   = m_pBlock->m_nCurPos;
				caretHeight = m_pBlock->m_nCaretHeight;

				int index = m_pBlock->GetIndexFromParent();
				CParagraph * pParagraph = m_pBlock->GetParagraph();
				((CBaseBlock *)m_pBlock->GetParagraph())->AdjustExtent();
				this->m_pBlock = pParagraph->m_Blocks[index];

				m_pBlock->m_nCurIndex = curIndex;
				m_pBlock->m_nCurPos = curPos - 1;
				m_pBlock->m_nCaretHeight = caretHeight;
				if(m_pBlock->m_pDx)
				{
					delete m_pBlock->m_pDx;
					m_pBlock->m_pDx = NULL;
				}
				this->MoveCaret(false);
				::InvalidateRect(m_pSheetCtrl->m_hWnd,&m_pSheetCtrl->m_pxfNode->m_ContentRect,false);
			}
			break; 
		case 0x09:          // Tab 
			// Tab stops exist every four spaces, so add 
			// spaces until the user hits the next tab. 

			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 

			break; 

		case 0x0D:          // Carriage return 
			// Go to the beginning of the next line. 
			// The bottom line wraps around to the top. 
			break; 

		case 0x1B:        // Escape 
		case 0x0A:        // Linefeed 
			MessageBeep((UINT) -1); 
			break; 
		default: 
			if(m_pBlock)
			{
				LPTSTR str;
				int len = (int)::_tcslen(m_pBlock->m_strText);
				int pos = 0;

				for(int i=0;i< m_pBlock->m_nCurIndex;i++)
				{
					pos += m_pBlock->m_RectStrPos[i];
				}
				pos += m_pBlock->m_nCurPos ;
				str = new TCHAR[len + 2];
				::_tcsncpy_s(str,len +2,m_pBlock->m_strText,pos);
				str[pos] = nChar;
				str[pos + 1] = '\0';
				::_tcscat_s(str,len +2,m_pBlock->m_strText + pos);
				if(m_pBlock->m_strText)delete m_pBlock->m_strText;
				m_pBlock->m_strText = str;

				int curIndex,curPos,caretHeight;
				curIndex = m_pBlock->m_nCurIndex;
				curPos   = m_pBlock->m_nCurPos;
				caretHeight = m_pBlock->m_nCaretHeight;

				int index = m_pBlock->GetIndexFromParent();
				CParagraph * pParagraph = m_pBlock->GetParagraph();
				((CBaseBlock *)m_pBlock->GetParagraph())->AdjustExtent();
				this->m_pBlock = pParagraph->m_Blocks[index];

				m_pBlock->m_nCurIndex = curIndex;
				m_pBlock->m_nCurPos = curPos + 1;
				if(m_pBlock->m_nCurPos > m_pBlock->m_RectStrPos[curIndex])
				{
					m_pBlock->m_nCurIndex++;
					m_pBlock->m_nCurPos = 1;
				}

				m_pBlock->m_nCaretHeight = caretHeight;
				if(m_pBlock->m_pDx)
				{
					delete m_pBlock->m_pDx;
					m_pBlock->m_pDx = NULL;
				}
				this->MoveCaret(false);

				::InvalidateRect(m_pSheetCtrl->m_hWnd,&m_pSheetCtrl->m_pxfNode->m_ContentRect,false);

			}
			break; 
		} 
		return ; 
	}
	int XWordSheetSvr::MoveCaret(bool bCreate)
	{
		POINT pt;
		if(m_pBlock)
		{
			m_pBlock->GetCaretPoint(pt);
		}else
		{
			pt.x = nCaretPosX;
			pt.y = nCaretPosY;
		}
		return MoveCaret(pt,bCreate);
	}

	int XWordSheetSvr::MoveCaret(POINT pt,bool bCreate)
	{
		nCaretPosX = pt.x;
		nCaretPosY = pt.y;

		if(!bCreate)
			HideCaret(m_pSheetCtrl->m_hWnd);
		else
			::CreateCaret(m_pSheetCtrl->m_hWnd,NULL,2,m_pBlock?m_pBlock->m_nCaretHeight:13);
		SetCaretPos(nCaretPosX,nCaretPosY);
		ShowCaret(m_pSheetCtrl->m_hWnd);
		return 1;
	}

	int XWordSheetSvr::LoadTemplate(IXMLDOMElement * pElement)
	{
		XOfficeSheetSvr::LoadTemplate(pElement);
		XWordDataSvr* pWordData = dynamic_cast<XWordDataSvr* >(m_pDataSvr);
		if(pWordData->m_pContentSvr)
		{
			m_pWordLayer->m_pWordData = pWordData;
			m_pWordLayer->m_pWordDocumentBlock->LoadData(pWordData->m_pContentSvr->m_pXWordDataDocument );
			m_pWordLayer->m_pWordDocumentBlock->AdjustExtent();
		}
		return 1;
	}

}}}}