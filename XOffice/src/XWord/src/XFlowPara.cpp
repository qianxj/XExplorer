#include "StdAfx.h"
#include "XFlowPara.hpp"
#include "XFlowDocument.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XFlowPara::XFlowPara():m_pContent(NULL)
	{
	}

	XFlowPara::~XFlowPara()
	{
	}

	//获取设定高度和宽度信息
	POINT XFlowPara::GetExtent()
	{
		POINT pt={0,0};
		return pt;
	}
	//获取实际占用的高度和宽度信息
	POINT XFlowPara::GetRealExtent()
	{
		POINT pt={0,0};
		return pt;
	}

	//计算
	SIZE XFlowPara::CalcBlockLayerInfo(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataTextBlock* pBlock,BlockLayerInfo * pBlockLayerInfo)
	{
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(pBlock->m_nIndexStyle);
		if(!pBoxStyle)
			pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		LOGFONT LogFont;
		HFONT hFont = NULL;
		XUtil::Css::GetFont(pBoxStyle,LogFont);
		if(hDC)
		{
			int nYPixelPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
			HDC dc = GetDC(NULL);
			int _nYPixelPerInch = GetDeviceCaps(dc, LOGPIXELSX);
			::ReleaseDC(NULL,dc);
			::MulDiv(LogFont.lfHeight, nYPixelPerInch,_nYPixelPerInch);
		}
		hFont = ::CreateFontIndirect(&LogFont);

		XWordDataTextBlock* pText = (XWordDataTextBlock*)pBlock;
		LPTSTR pStr = pText->m_pStrText;
		int nStrLen = 0;
		if(pText->m_pStrText) nStrLen = (int)_tcslen(pText->m_pStrText);
		int nFit;
		SIZE size;
		int * pDx = nStrLen >0 && pBlockLayerInfo ? new int[nStrLen] : NULL;
		if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
		GetTextExtentPoint(hDC,pStr,nStrLen,&size);
		GetTextExtentExPoint(hDC,pStr,nStrLen,size.cx + 1,&nFit,pDx,&size);
		if(nStrLen==0)
		{
			GetTextExtentPoint(hDC,_T("AFGXMafxgm"),10,&size);
			size.cx = 0;
		}
		if(hFont)hFont = (HFONT)::SelectObject(hDC,hFont);
		if(hFont) ::DeleteObject(hFont);
		
		if(pBlockLayerInfo)
		{
			pBlockLayerInfo->bDeleteBoxStyle = false;
			pBlockLayerInfo->pBoxStyle = (XUtil::Css::TBoxStyle *)pBoxStyle;
			pBlockLayerInfo->ContentExtent = size;
			pBlockLayerInfo->nLenStr = nStrLen;
			pBlockLayerInfo->pStr = pStr;
			pBlockLayerInfo->pDx = pDx;
			pBlockLayerInfo->pBlock = pText;
		}
		return size;
	}

	//清除块布局信息
	int  XFlowPara::ClearLayerInfo(BlockLayerInfo  pBlockLayerInfo[],int nCount)
	{
		for(int i=0;i<nCount;i++)
		{
			if(pBlockLayerInfo[i].bDeleteBoxStyle)
				delete pBlockLayerInfo[i].pBoxStyle;
			if(pBlockLayerInfo[i].pDx) delete pBlockLayerInfo[i].pDx; 
		}
		delete pBlockLayerInfo;
		
		return 1;
	}

	int XFlowPara::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		return 1;
	}

	int  XFlowPara::DrawContent(XFlowDocument * pDocument,BlockLayerInfo * pBlockLayerInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen)
	{
		LOGFONT LogFont;
		HFONT hFont = NULL;
		XUtil::Css::GetFont(pBlockLayerInfo->pBoxStyle,LogFont);

		int nYPixelPerInch = GetDeviceCaps(hPaintDC, LOGPIXELSY);
		HDC dc = GetDC(NULL);
		int _nYPixelPerInch = GetDeviceCaps(dc, LOGPIXELSX);
		::ReleaseDC(NULL,dc);
		::MulDiv(LogFont.lfHeight, nYPixelPerInch,_nYPixelPerInch);
		
		hFont = ::CreateFontIndirect(&LogFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);

		COLORREF	TextColor = ::GetTextColor(hPaintDC);
		int hbr = ::GetBkMode(hPaintDC);
		COLORREF	TextBkColor = ::GetBkColor(hPaintDC);
		COLORREF	crColor = RGB(0,0,0);
		XUtil::Css::TBoxStyle * pBoxStyle = pBlockLayerInfo->pBoxStyle;
		if(pBoxStyle)
		{
			if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
			{
				::SetTextColor(hPaintDC,pBoxStyle->pColor->color[0]);
				crColor = pBoxStyle->pColor->color[0];
			}
			if(pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
			{
				if(pBoxStyle->pBackGround->mask & cssMaskBackGroundColor)
				{
					if(pBoxStyle->pBackGround->color.color[0]!= RGB(255,255,255))
					{
						hbr = ::SetBkMode(hPaintDC,OPAQUE);
						::SetBkColor(hPaintDC,pBoxStyle->pBackGround->color.color[0]);
					}
				}
			}
		}

		if(pDocument->m_Selection.bSelection)
		{
			if(pDocument->BlockIsSelected((XWordDataTextBlock *)pBlockLayerInfo->pBlock,
				(XWordDataTextBlock *)pDocument->m_Selection.pStartBlock,
				(XWordDataTextBlock *)pDocument->m_Selection.pEndBlock))
			{
				//起始部分
				if(pBlockLayerInfo->pBlock==pDocument->m_Selection.pStartBlock && nStart < pDocument->m_Selection.nStartPosChar)
				{
					int nEnd = pDocument->m_Selection.nStartPosChar;
					if(nEnd > nStart + nLen) nEnd = nStart + nLen;
					ExtTextOut(hPaintDC,rt.left ,rt.top ,0,NULL,pBlockLayerInfo->pStr+nStart,nEnd - nStart ,NULL);
				}

				//中间部分
				int nStartEx= -1,nEndEx = -1;
				if(pBlockLayerInfo->pBlock==pDocument->m_Selection.pStartBlock )
				{
					if(nStart < pDocument->m_Selection.nStartPosChar)
						nStartEx =pDocument->m_Selection.nStartPosChar;
					else
						nStartEx = nStart;
				}else if(pDocument->GetIndex(pBlockLayerInfo->pBlock)>pDocument->GetIndex(pDocument->m_Selection.pStartBlock))
				{
					nStartEx = nStart;
				}
				if(pBlockLayerInfo->pBlock==pDocument->m_Selection.pEndBlock )
				{
					if(nStart + nLen > pDocument->m_Selection.nEndPosChar)
					{
						nEndEx =pDocument->m_Selection.nEndPosChar;
					}else
						nEndEx = nStart + nLen;
				}else if(pDocument->GetIndex(pBlockLayerInfo->pBlock)<pDocument->GetIndex(pDocument->m_Selection.pEndBlock))
				{
					nEndEx = nStart + nLen;
				}

				if(nEndEx!=-1 && nStartEx!=-1)
				{
					int nStartPos = 0;
					if(pBlockLayerInfo->pDx)
					{
						if(nStartEx != nStart)
						{
							nStartPos = pBlockLayerInfo->pDx[nStartEx - 1] -
								(nStart>0?pBlockLayerInfo->pDx[nStart- 1] :0);
						}
					}

					int hbr = ::SetBkMode(hPaintDC,OPAQUE);
					COLORREF bkColor = ::SetBkColor(hPaintDC,RGB(0,0,0));
					COLORREF tColor = RGB(255 - GetRValue(crColor),
							255 - GetGValue(crColor),
							255 - GetBValue(crColor));
					tColor = ::SetTextColor(hPaintDC,tColor);
					ExtTextOut(hPaintDC,rt.left + nStartPos ,rt.top ,0,NULL,pBlockLayerInfo->pStr+nStartEx,nEndEx - nStartEx ,NULL);
					tColor = ::SetTextColor(hPaintDC,tColor);
					::SetBkMode(hPaintDC,hbr);
					SetBkColor(hPaintDC,bkColor);
				}

				//结束部分
				if(pBlockLayerInfo->pBlock==pDocument->m_Selection.pEndBlock && nStart + nLen >= pDocument->m_Selection.nEndPosChar)
				{
					int nStartPos = 0;
					int nEnd = pDocument->m_Selection.nEndPosChar;
					if(nEnd < nStart)
					{
						ExtTextOut(hPaintDC,rt.left+ nStartPos,rt.top ,0,NULL,pBlockLayerInfo->pStr + nStart,nLen,NULL);
					}else
					{
						if(pBlockLayerInfo->pDx)
						{
							nStartPos = (pDocument->m_Selection.nEndPosChar>0?pBlockLayerInfo->pDx[pDocument->m_Selection.nEndPosChar - 1]:0) -
									(nStart>0?pBlockLayerInfo->pDx[nStart- 1] :0);
						}
						ExtTextOut(hPaintDC,rt.left+ nStartPos,rt.top ,0,NULL,pBlockLayerInfo->pStr + pDocument->m_Selection.nEndPosChar,nStart + nLen - pDocument->m_Selection.nEndPosChar ,NULL);
					}
				}
			}
			else
				ExtTextOut(hPaintDC,rt.left,rt.top ,0,NULL,pBlockLayerInfo->pStr+nStart,nLen,NULL);
		}else
		{
			ExtTextOut(hPaintDC,rt.left,rt.top ,0,NULL,pBlockLayerInfo->pStr+nStart,nLen,NULL);
			//::DrawText(hPaintDC,pBlockLayerInfo->pStr+nStart,nLen,&rt,DT_LEFT);
		}
		
		::SetTextColor(hPaintDC,TextColor);
		::SetBkColor(hPaintDC,TextBkColor);
		::SetBkMode(hPaintDC,hbr);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);

		return 1;
	}

	int  XFlowPara::DoDraw(XFlowDocument * pDocument,BlockLayerInfo pBlockLayerInfo[], int nLen, HDC hPaintDC)
	{
		int nFirstWordBala = 20;
		int nItem = 0;
		int nWidth = FContentRect.right - FContentRect.left;
		POINT ptOffset = {nFirstWordBala,0};
		bool bHasFited = false;
		std::vector<RECT> rRects;
		int nRight = 0;
		bool bStart = false; //start try to fit text
		int rRectIndex = 0;

		RECT rcDraw;
		this->m_RowHeights.clear();
		this->m_spRowHeights.clear();
		for(int i=0;i<nLen;i++)
		{
			int nHeight = 0;
			rRects.clear();

			//得到可填充矩阵
			if(pBlockLayerInfo[i].pDx)
			{
				rRects.clear();
				this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
				rRectIndex = 0;
				nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

				LPTSTR pStr = pBlockLayerInfo[i].pStr;
				int  nStrLen   = (int)_tcslen(pStr);
				int  nIndex	= 0;
				int	 nIndexStart = 0;
				bool bNewRow = false;
				while(true)
				{
					bNewRow = false;
					while(nIndex < nStrLen && pBlockLayerInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
					if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
					{
						//draw text;
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,ptOffset.y,
							rRects[rRectIndex].right + 1,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						DrawContent(pDocument,pBlockLayerInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);

						//前面部分的字符都可以填冲，开始新的一行
						nIndex++;
						if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight)
							nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						bNewRow = true; //开始新行
					}else if(nIndex == nIndexStart && nStrLen) //not fit one char
					{
						//如果没有填充，并且是整个宽度，则强制填充
						if(!bHasFited && nWidth - nRight <= 2 )
						{
							nIndex++; //fit it
							
							//draw text;
							SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,rRects[rRectIndex].top,
								rRects[rRectIndex].right + 1,rRects[rRectIndex].top + pBlockLayerInfo[i].ContentExtent.cy);
							::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
							DrawContent(pDocument,pBlockLayerInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);


							if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						}
						bNewRow = true;	//开始新行
					}else
					{
						if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
						{
							int nPos = nIndex;
							nIndex --;
							while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
							//如一行就只有一个词
							if(nIndex == nIndexStart) nIndex = nPos;
						}

						//draw text;
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight +1,ptOffset.y,
							rRects[rRectIndex].right + 5,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						DrawContent(pDocument,pBlockLayerInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);
						
						if(nIndex < nStrLen)
						{
							if(rRectIndex == rRects.size() - 1) 
								bNewRow = true; //开始新行
							else if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
							{
								nIndex++;
								if(pStr[nIndex]=='\r' && pStr[nIndex + 1]=='\n')nIndex++;
								bNewRow = true; //开始新行
							}
							else
							{
								rRectIndex++;
								nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1;
							}
						}else
						{
							//调整左右长度
							ptOffset.x = rRects[rRectIndex].left +
								(nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0);

							nRight -= nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0;
							bHasFited = true;
						}

						//如高度大于前面的高度，使用新高度
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
					}
					if(bNewRow)
					{
						//设置行高度
						this->m_RowHeights.push_back((int)(nHeight * 1.5));
						//this->m_RowHeights[nItem] = nHeight;

						//增加行号
						nItem++;
						
						//重置行参数
						ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
						nHeight = 0;
						bHasFited = false;
						rRects.clear();
						this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
						rRectIndex = 0;
						nRight = rRects[0].right - rRects[0].left + 1;
					}
					nIndexStart = nIndex;
					bNewRow = false;
					//如已全部填充，则退出
					if(nIndex >= nStrLen) break;
				}
			}
		}
		return 1;
	}

	int  XFlowPara::DoDraw(XWordDataSvr* pDataSvr,XFlowDocument * pDocument,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect)
	{
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(!pBoxStyle)
			pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

		int nLen = m_pContent->m_pChilds->size();
		BlockLayerInfo*  pBlockLayerInfo = NULL;
		if(nLen)
			pBlockLayerInfo = new BlockLayerInfo[nLen];

		int nIndex = 0;
		for(int i=0;i < nLen;i++)
		{
			XWordDataBase * pData = (*m_pContent->m_pChilds)[i];
			if(pData->eWordDataType ==_XWordDataSpanControlBlock)continue;
			CalcBlockLayerInfo(pDataSvr,(XUtil::Css::TBoxStyle *)pBoxStyle,hPaintDC,(XWordDataTextBlock*)(*m_pContent->m_pChilds)[i],pBlockLayerInfo + nIndex);
			nIndex++;
		}
		DoDraw(pDocument,pBlockLayerInfo,nIndex, hPaintDC);
		this->ClearLayerInfo(pBlockLayerInfo,nIndex);

		return 1;
	}

	bool XFlowPara::CalcCursorPositionAtPoint(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,POINT pt,XWordCursorInfoEx & CursorInfo)
	{
		bool ret = false;
		SIZE extent={0,0};
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(!pBoxStyle)
			pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		
		int nLen = m_pContent->m_pChilds->size();
		BlockLayerInfo*  pBlockLayerInfo;
		if(nLen)
			pBlockLayerInfo = new BlockLayerInfo[nLen];
		int nIndex = 0;
		int cHeight = 0;
		for(int i=0;i < nLen;i++)
		{
			XWordDataBase * pData = (*m_pContent->m_pChilds)[i];
			if(pData->eWordDataType ==_XWordDataSpanControlBlock)
			{
				if(cHeight < pData->y + pData->height)
					cHeight = pData->y + pData->height;
			}else
			{
				CalcBlockLayerInfo(pDataSvr,(XUtil::Css::TBoxStyle *)pBoxStyle,hDC,(XWordDataTextBlock*)(*m_pContent->m_pChilds)[i],pBlockLayerInfo + nIndex);
				nIndex++;
			}
		}
		ret = CalcCursorPositionAtPoint(pBlockLayerInfo,nIndex,pt,CursorInfo);
		this->ClearLayerInfo(pBlockLayerInfo,nIndex);

		return ret;
	}

	bool XFlowPara::CalcCursorPositionAtPoint(BlockLayerInfo pBlockLayerInfo[],int nLen,POINT pt,XWordCursorInfoEx & CursorInfo)
	{
		int nFirstWordBala = 20;
		int nItem = 0;
		int nWidth = FContentRect.right - FContentRect.left;
		POINT ptOffset = {nFirstWordBala,0};
		bool bHasFited = false;
		std::vector<RECT> rRects;
		int nRight = 0;
		bool bStart = false; //start try to fit text
		int rRectIndex = 0;

		RECT rcDraw;
		for(int i=0;i<nLen;i++)
		{
			int nHeight = 0;
			rRects.clear();

			//得到可填充矩阵
			if(pBlockLayerInfo[i].pDx || pBlockLayerInfo[i].nLenStr < 1)
			{
				rRects.clear();
				this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
				rRectIndex = 0;
				nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

				LPTSTR pStr = pBlockLayerInfo[i].pStr;
				int  nStrLen   = (int)_tcslen(pStr);
				int  nIndex	= 0;
				int	 nIndexStart = 0;
				bool bNewRow = false;
				while(true)
				{
					bNewRow = false;
					while(nIndex < nStrLen && pBlockLayerInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
					if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
					{
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,ptOffset.y,
							rRects[rRectIndex].right + 1,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						if(::PtInRect(&rcDraw,pt))
							return this->CalcCursorPositionAtPoint(pBlockLayerInfo+i,nIndexStart,nIndex,rcDraw.left,rcDraw.top,pt,CursorInfo);

						//前面部分的字符都可以填冲，开始新的一行
						nIndex++;
						if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight)
							nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						bNewRow = true; //开始新行
					}else if(nIndex == nIndexStart && nStrLen) //not fit one char
					{
						//如果没有填充，并且是整个宽度，则强制填充
						if(!bHasFited && nWidth - nRight <= 2 )
						{
							nIndex++; //fit it
							
							SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,rRects[rRectIndex].top,
								rRects[rRectIndex].right + 1,rRects[rRectIndex].top + pBlockLayerInfo[i].ContentExtent.cy);
							::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
							if(::PtInRect(&rcDraw,pt))
								return this->CalcCursorPositionAtPoint(pBlockLayerInfo+i,nIndexStart,nIndex,rcDraw.left,rcDraw.top,pt,CursorInfo);

							if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						}
						bNewRow = true;	//开始新行
					}else
					{
						if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
						{
							nIndex --;
							while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
						}

						//draw text;
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight +1,ptOffset.y,
							rRects[rRectIndex].right + 5,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						if(rcDraw.right - rcDraw.left > pBlockLayerInfo[i].ContentExtent.cx - (nIndexStart>0?pBlockLayerInfo[i].pDx[nIndexStart -1]:0))
							rcDraw.right = rcDraw.left + pBlockLayerInfo[i].ContentExtent.cx - (nIndexStart>0?pBlockLayerInfo[i].pDx[nIndexStart -1]:0);
						
						if(::PtInRect(&rcDraw,pt))
							return this->CalcCursorPositionAtPoint(pBlockLayerInfo+i,nIndexStart,nIndex,rcDraw.left,rcDraw.top,pt,CursorInfo);
						
						if(nIndex < nStrLen)
						{
							if(rRectIndex == rRects.size() - 1) 
								bNewRow = true; //开始新行
							else if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
							{
								nIndex++;
								if(pStr[nIndex]=='\r' && pStr[nIndex + 1]=='\n')nIndex++;
								bNewRow = true; //开始新行
							}
							else
							{
								rRectIndex++;
								nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1;
							}
						}else
						{
							//调整左右长度
							ptOffset.x = rRects[rRectIndex].left +
								(nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0);

							nRight -= nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0;
							bHasFited = true;
						}

						//如高度大于前面的高度，使用新高度
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
					}
					if(bNewRow)
					{
						//设置行高度
						//this->m_RowHeights.push_back((int)(nHeight * 1.5));

						//增加行号
						nItem++;
						
						//重置行参数
						ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
						nHeight = 0;
						bHasFited = false;
						rRects.clear();
						this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
						rRectIndex = 0;
						nRight = rRects[0].right - rRects[0].left + 1;
					}
					nIndexStart = nIndex;
					bNewRow = false;
					//如已全部填充，则退出
					if(nIndex >= nStrLen) break;
				}
			}
		}
		return false;
	}

	bool XFlowPara::CalcCursorPositionAtPoint(BlockLayerInfo* pBlockLayerInfo,int nStart,int nBefore,int nPosLeft,int nPosTop, POINT pt,XWordCursorInfoEx & CursorInfo)
	{
		int nStartPos = nStart > 0 ? pBlockLayerInfo->pDx[nStart -1]:0;
		int nPosX = pt.x - nPosLeft;
		for(int k = nStart;k < nBefore ;k++)
		{
			if(pBlockLayerInfo->pDx[k] - nStartPos >= nPosX )
			{
				if( nPosX - (k==0?0:pBlockLayerInfo->pDx[k - 1] - nStartPos) < pBlockLayerInfo->pDx[k] - nStartPos - nPosX) //shift to left char
				{
					CursorInfo.XPosCursur = nPosLeft + (k==0?0:pBlockLayerInfo->pDx[k - 1] - nStartPos);
					CursorInfo.nIndex = k; 
				}
				else
				{
					CursorInfo.XPosCursur = nPosLeft + pBlockLayerInfo->pDx[k] - nStartPos;
					CursorInfo.nIndex = k + 1; 
				}
				CursorInfo.pBlock = (XWordDataTextBlock*)pBlockLayerInfo->pBlock;
				CursorInfo.CursorHeight = pBlockLayerInfo->ContentExtent.cy;
				CursorInfo.YPosCursur = nPosTop;
				break;		
			}
		}		
		return true;
	}

	bool XFlowPara::CalcCursorPosition(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo)
	{
		bool ret = false;
		SIZE extent={0,0};
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(!pBoxStyle)
			pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

		int nLen = m_pContent->m_pChilds->size();
		BlockLayerInfo*  pBlockLayerInfo;
		if(nLen)
			pBlockLayerInfo = new BlockLayerInfo[nLen];
		int nIndex = 0;
		int cHeight = 0;
		for(int i=0;i < nLen;i++)
		{
			XWordDataBase * pData = (*m_pContent->m_pChilds)[i];
			if(pData->eWordDataType ==_XWordDataSpanControlBlock)
			{
				if(cHeight < pData->y + pData->height)
					cHeight = pData->y + pData->height;
			}else
			{
				CalcBlockLayerInfo(pDataSvr,(XUtil::Css::TBoxStyle *)pBoxStyle,hDC,(XWordDataTextBlock*)(*m_pContent->m_pChilds)[i],pBlockLayerInfo + nIndex);
				nIndex++;
			}
			if(pData==pBlock)break;
		}
		CursorInfo.pBlock = pBlock;
		CursorInfo.nIndex = nTextIndex;
		ret = CalcCursorPosition(pBlockLayerInfo,nIndex,pBlock,nTextIndex,CursorInfo);
		/*if(!ret)
		{
			CursorInfo.pBlock = NULL;
			CursorInfo.nIndex = nTextIndex;
		}*/

		this->ClearLayerInfo(pBlockLayerInfo,nIndex);

		return ret;
	}

	bool XFlowPara::CalcCursorPosition(BlockLayerInfo pBlockLayerInfo[],int nLen,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo)
	{
		int nFirstWordBala = 20;
		int nItem = 0;
		int nWidth = FContentRect.right - FContentRect.left;
		POINT ptOffset = {nFirstWordBala,0};
		bool bHasFited = false;
		std::vector<RECT> rRects;
		int nRight = 0;
		bool bStart = false; //start try to fit text
		int rRectIndex = 0;

		RECT rcDraw;
		for(int i=0;i<nLen;i++)
		{
			int nHeight = 0;
			rRects.clear();

			//得到可填充矩阵
			if(pBlockLayerInfo[i].pDx || pBlockLayerInfo[i].nLenStr<1 )
			{
				rRects.clear();
				this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
				rRectIndex = 0;
				nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

				LPTSTR pStr = pBlockLayerInfo[i].pStr;
				int  nStrLen   = (int)_tcslen(pStr);
				int  nIndex	= 0;
				int	 nIndexStart = 0;
				bool bNewRow = false;
				bool bFound = false;
				while(true)
				{
					bNewRow = false;
					while(nIndex < nStrLen && pBlockLayerInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B &&  !(pBlockLayerInfo[i].pBlock==pBlock && nIndex==nTextIndex))nIndex++;
					if(pBlockLayerInfo[i].pBlock==pBlock && nIndex==nTextIndex)bFound = true;
					if(bFound && nTextIndex==0)
					{
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight +1,ptOffset.y,
							rRects[rRectIndex].right + 5,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						CursorInfo.XPosCursur = rcDraw.left;
						CursorInfo.YPosCursur = rcDraw.top;
						return true;
					}
					if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
					{
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,ptOffset.y,
							rRects[rRectIndex].right + 1,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						if(bFound)
							return this->CalcCursorPosition(pBlockLayerInfo+i,nIndexStart,nTextIndex,rcDraw.left,rcDraw.top,CursorInfo);

						//前面部分的字符都可以填冲，开始新的一行
						nIndex++;
						if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight)
							nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						bNewRow = true; //开始新行
					}else if(nIndex == nIndexStart && nStrLen ) //not fit one char
					{
						//如果没有填充，并且是整个宽度，则强制填充
						if(!bHasFited && nWidth - nRight <= 2 )
						{
							nIndex++; //fit it
							
							SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,rRects[rRectIndex].top,
								rRects[rRectIndex].right + 1,rRects[rRectIndex].top + pBlockLayerInfo[i].ContentExtent.cy);
							::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
							if(bFound)
								return this->CalcCursorPosition(pBlockLayerInfo+i,nIndexStart,nTextIndex,rcDraw.left,rcDraw.top,CursorInfo);

							if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						}
						bNewRow = true;	//开始新行
					}else
					{
						if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
						{
							nIndex --;
							while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
						}

						//draw text;
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight +1,ptOffset.y,
							rRects[rRectIndex].right + 5,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						if(rcDraw.right - rcDraw.left > pBlockLayerInfo[i].ContentExtent.cx - (nIndexStart>0?pBlockLayerInfo[i].pDx[nIndexStart -1]:0))
							rcDraw.right = rcDraw.left + pBlockLayerInfo[i].ContentExtent.cx - (nIndexStart>0?pBlockLayerInfo[i].pDx[nIndexStart -1]:0);
						if(bFound)
							return this->CalcCursorPosition(pBlockLayerInfo+i,nIndexStart,nTextIndex,rcDraw.left,rcDraw.top,CursorInfo);

						
						if(nIndex < nStrLen)
						{
							if(rRectIndex == rRects.size() - 1) 
								bNewRow = true; //开始新行
							else if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
							{
								nIndex++;
								if(pStr[nIndex]=='\r' && pStr[nIndex + 1]=='\n')nIndex++;
								bNewRow = true; //开始新行
							}
							else
							{
								rRectIndex++;
								nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1;
							}
						}else
						{
							//调整左右长度
							ptOffset.x = rRects[rRectIndex].left +
								(nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0);

							nRight -= nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0;
							bHasFited = true;
						}

						//如高度大于前面的高度，使用新高度
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
					}
					if(bNewRow)
					{
						//设置行高度
						//this->m_RowHeights.push_back((int)(nHeight * 1.5));

						//增加行号
						nItem++;
						
						//重置行参数
						ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
						nHeight = 0;
						bHasFited = false;
						rRects.clear();
						this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
						rRectIndex = 0;
						nRight = rRects[0].right - rRects[0].left + 1;
					}
					nIndexStart = nIndex;
					bNewRow = false;
					//如已全部填充，则退出
					if(nIndex >= nStrLen) break;
				}
			}
			else
			{
				if(pBlockLayerInfo[i].pBlock==pBlock)
				{
					rRects.clear();
					this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
					rRectIndex = 0;
					nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;
					
					SetRect(&rcDraw,rRects[rRectIndex].right - nRight +1,ptOffset.y,
						rRects[rRectIndex].right + 5,ptOffset.y + pBlockLayerInfo[i].ContentExtent.cy);
					::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
					CursorInfo.XPosCursur = rcDraw.left;
					CursorInfo.YPosCursur = rcDraw.top;
					return true;
				}
			}
		}
		return false;
	}

	bool XFlowPara::CalcCursorPosition(BlockLayerInfo* pBlockLayerInfo,int nStart,int nTextIndex,int nPosLeft,int nPosTop, XWordCursorInfoEx & CursorInfo)
	{
		int nStartPos = nStart > 0 ? pBlockLayerInfo->pDx[nStart -1]:0;
		
		CursorInfo.XPosCursur = nPosLeft + (nTextIndex<=0?0:pBlockLayerInfo->pDx[nTextIndex - 1]) - nStartPos;
		CursorInfo.nIndex = nTextIndex; 
		CursorInfo.CursorHeight = pBlockLayerInfo->ContentExtent.cy;
		CursorInfo.YPosCursur = nPosTop;
		CursorInfo.pBlock = (XWordDataTextBlock*)pBlockLayerInfo->pBlock;

		return true;
	}

	//计算占用的高度和宽度信息
	SIZE XFlowPara::ReCalcExtent(BlockLayerInfo pBlockLayerInfo[],int nLen)
	{
		int nFirstWordBala = 20;
		int nItem = 0;
		int nWidth = FContentRect.right - FContentRect.left;
		POINT ptOffset = {nFirstWordBala,0};
		bool bHasFited = false;
		std::vector<RECT> rRects;
		int nRight = 0;
		bool bStart = false; //start try to fit text
		int rRectIndex = 0;
		int nHeight = 0;

		this->m_RowHeights.clear();
		this->m_spRowHeights.clear();
		SIZE extent={0,0};
		extent.cx = nWidth;
		for(int i=0;i<nLen;i++)
		{
			nHeight = 0;
			rRects.clear();

			//得到可填充矩阵
			if(pBlockLayerInfo[i].pDx)
			{
				rRects.clear();
				this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
				rRectIndex = 0;
				nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

				LPTSTR pStr = pBlockLayerInfo[i].pStr;
				int  nStrLen   = (int)_tcslen(pStr);
				int  nIndex	= 0;
				int	 nIndexStart = 0;
				bool bNewRow = false;
				while(true)
				{
					bNewRow = false;
					//得到可填充的字数
					while(nIndex < nStrLen && pBlockLayerInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
					if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
					{
						//前面部分的字符都可以填冲，开始新的一行
						nIndex++;
						if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight)
							nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						bNewRow = true; //开始新行
					}else if(nIndex == nIndexStart && nStrLen) //not fit one char
					{
						//如果没有填充，并且是整个宽度，则强制填充
						if(!bHasFited && nWidth - nRight <= 2 )
						{
							nIndex++; //fit it
							
							if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
						}
						bNewRow = true;	//开始新行
					}else
					{
						//如最后不够填充整个词,则退出这个词
						if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
						{
							nIndex --;
							while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
						}

						if(nIndex < nStrLen)
						{
							//如未全部填充
							if(rRectIndex == rRects.size() - 1) 
								bNewRow = true; //开始新行
							else if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
							{
								nIndex++;
								if(pStr[nIndex]=='\r' && pStr[nIndex + 1]=='\n')nIndex++;
								bNewRow = true; //开始新行
							}
							else
							{
								//如右边还有剩余空间,则读取下一空间继续填充
								rRectIndex++;
								nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1;
							}
						}else
						{
							//已全部填充
							//调整左右长度,填充下一块
							ptOffset.x = rRects[rRectIndex].left +
								(nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0);

							nRight -= nIndex>=1?pBlockLayerInfo[i].pDx[nIndex - 1] - 
								(nIndexStart >= 1?pBlockLayerInfo[i].pDx[nIndexStart - 1]:0) :0;
							bHasFited = true;
						}

						//如高度大于前面的高度，使用新高度
						if(pBlockLayerInfo[i].ContentExtent.cy > nHeight) 
								nHeight = pBlockLayerInfo[i].ContentExtent.cy; 
					}

					if(bNewRow)
					{
						//设置行高度
						this->m_RowHeights.push_back((int)(nHeight * 1.5));
						//this->m_RowHeights[nItem] = nHeight;
						extent.cy += (int)(nHeight * 1.5);

						//增加行号
						nItem++;
						
						//重置行参数
						ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
						nHeight = 0;
						bHasFited = false;
						rRects.clear();
						this->GetLayerRightRects(rRects,pBlockLayerInfo,pBlockLayerInfo[i].ContentExtent.cy,16,ptOffset);
						rRectIndex = 0;
						nRight = rRects[0].right - rRects[0].left + 1;
					}
					nIndexStart = nIndex;
					bNewRow = false;
					//如已全部填充，则退出
					if(nIndex >= nStrLen) break;
				}
			}
		}
		extent.cy += nHeight;
		return extent;
	}


	//重新计算位置信息
	SIZE XFlowPara::CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC)
	{
		SIZE extent={0,0};
		const XUtil::Css::TBoxStyle * pBoxStyle;
		pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(m_pContent->m_nIndexStyle);
		if(!pBoxStyle)
			pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

		int nLen = m_pContent->m_pChilds->size();
		BlockLayerInfo*  pBlockLayerInfo = NULL;
		if(nLen)
			pBlockLayerInfo = new BlockLayerInfo[nLen];
		int nIndex = 0;
		int cHeight = 0;
		for(int i=0;i < nLen;i++)
		{
			XWordDataBase * pData = (*m_pContent->m_pChilds)[i];
			if(pData->eWordDataType ==_XWordDataSpanControlBlock)
			{
				if(cHeight < pData->y + pData->height)
					cHeight = pData->y + pData->height;
			}else
			{
				CalcBlockLayerInfo(pDataSvr,(XUtil::Css::TBoxStyle *)pBoxStyle,hDC,(XWordDataTextBlock*)(*m_pContent->m_pChilds)[i],pBlockLayerInfo + nIndex);
				nIndex++;
			}
		}
		extent = ReCalcExtent(pBlockLayerInfo,nIndex);
		this->ClearLayerInfo(pBlockLayerInfo,nIndex);
		if(extent.cy < cHeight)
			extent.cy = cHeight + 1;
		return extent;
	}

	//得到相交的排除矩阵
	BOOL XFlowPara::GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound)
	{
		RECT rc;
		BOOL bIntersect = FALSE;
		if(m_exRects.size() < 1) return FALSE;
		vRect.clear();
		for(int i= 0;i<(int)m_exRects.size();i++)
		{
			if(IntersectRect(&rc ,&m_exRects[i],&rtBound))
			{
				if(!bIntersect)bIntersect = TRUE;
				vRect.push_back(rc);
			}
		}
		return bIntersect;
	}

	//按照从左到右排序矩形组
	int XFlowPara::SortRects(std::vector<RECT> &vRect)
	{
		RECT rt;
		int nCount = (int)vRect.size();
		if(nCount <= 1) return 1;
		if(nCount == 2)
		{
			if(vRect[0].left < vRect[1].left)return 1;
			rt = vRect[0];
			vRect[0] = vRect[1];
			vRect[1] = rt;
			return 1;
		}else if(nCount==3)
		{
			if(vRect[0].left < vRect[1].left)
			{
				if(vRect[0].left < vRect[2].left)
				{
					//sort element 2,3
					if(vRect[1].left < vRect[2].left)return 1;
					rt = vRect[1];
					vRect[1] = vRect[2];
					vRect[2] = rt;
					return 1;
				}else
				{
					rt = vRect[2];
					vRect[2] = vRect[1];
					vRect[1] = vRect[0];
					vRect[0] = rt;
					return 1;
				}
			}else
			{
				if(vRect[1].left > vRect[2].left)
				{
					rt = vRect[2];
					vRect[2] = vRect[0];
					vRect[0] = rt;
					return 1;
				}else
				{
					rt = vRect[0];
					vRect[0] = vRect[1];
					if(rt.left < vRect[2].left)
					{
						vRect[1] =  rt;
						return 1;
					}else
					{
						vRect[1] = vRect[2];
						vRect[2] = rt;
						return 1;
					}
				}
			}
		}else
		{
			//operator sort
		}
		return 1;
	}

	//计算右边可容纳矩阵的宽度，计算时需要排除ExRect
	int XFlowPara::GetLayerRight(BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset)
	{
		//如无排除矩阵，直接返回余下的宽度
		if(m_exRects.size() < 1)
			return this->FContentRect.right - this->FContentRect.left - ptOffset.x;
		
		std::vector<RECT> vRect;
		RECT rt ={ptOffset.x,ptOffset.y,
				this->FContentRect.right - this->FContentRect.left,nHeight};
		
		//如无相交，直接返回余下的宽度
		if(!GetUnionExRect(vRect,rt))
			return this->FContentRect.right - this->FContentRect.left - ptOffset.x;

		//排序排除矩形
		SortRects(vRect);

		int nRightSum = 0;
		int nRight = 0;
		int nStart = ptOffset.x;
		for(int i = 0;i<(int)vRect.size();i++)
		{
			nRight = vRect[i].left - nStart;
			if(nRight >= nMinWidth) //宽度必须大于nMinWidth
			{
				nRightSum += nRight;
				nStart = vRect[i].right + 1;
			}
		}
		return nRightSum;
	}

	//计算右边可容纳矩阵的矩阵，计算时需要排除ExRect
	int XFlowPara::GetLayerRightRects(std::vector<RECT> &rRect,BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset)
	{
		int nMergeWidth = 5;
		RECT rc;

		//如无排除矩阵，直接返回余下的宽度
		if(m_exRects.size() < 1)
		{
			int nRight = this->FContentRect.right - this->FContentRect.left - ptOffset.x;
			::SetRect(&rc,ptOffset.x,ptOffset.y,ptOffset.x + nRight,ptOffset.y + nHeight);
			rRect.push_back(rc);
			return nRight;
		}
		
		std::vector<RECT> vRect;
		RECT rt ={ptOffset.x,ptOffset.y,
				this->FContentRect.right - this->FContentRect.left,ptOffset.y+nHeight};
		
		//如无相交，直接返回余下的宽度
		if(!GetUnionExRect(vRect,rt))
		{
			int nRight = this->FContentRect.right - this->FContentRect.left - ptOffset.x;
			::SetRect(&rc,ptOffset.x,ptOffset.y,ptOffset.x + nRight,ptOffset.y + nHeight);
			rRect.push_back(rc);
			return nRight;
		}

		//排序排除矩形
		SortRects(vRect);

		int nRightSum = 0;
		int nRight = 0;
		int nStart = ptOffset.x;
		for(int i = 0;i<(int)vRect.size();i++)
		{
			nRight = vRect[i].left - nStart;
			if(nRight >= nMinWidth) //宽度必须大于nMinWidth
			{
				nRightSum += nRight;
				::SetRect(&rc,nStart,ptOffset.y,vRect[i].left - nMergeWidth,ptOffset.y + nHeight);
				rRect.push_back(rc);
			}
			nStart = vRect[i].right + nMergeWidth;
			if(i==(int)vRect.size() - 1)
			{
				nRight = this->FContentRect.right - this->FContentRect.left - nStart;
				if(nRight >= nMinWidth) //宽度必须大于nMinWidth
				{
					nRightSum += nRight;
					::SetRect(&rc,nStart,ptOffset.y,FContentRect.right - FContentRect.left,ptOffset.y + nHeight);
					rRect.push_back(rc);
				}
			}
		}
		return nRightSum;
	}
	
	int XFlowPara::GetIndexByBlock(XWordDataBase *pBlock)
	{
		for(int i=0;i<m_pContent->m_pChilds->size();i++)
		{
			if((*m_pContent->m_pChilds)[i]==pBlock)return i;
		}
		return -1;
	}

	XWordDataBase* XFlowPara::GetControlBlockAtPoint(XWordDataSvr* pDataSvr,POINT pt)
	{
		RECT rt;
		POINT p1 = pt;
		p1.x -= FRect.left;
		p1.y -= FRect.top;
		for(int k=0;k<m_pContent->m_pChilds->size();k++)
		{
			XWordDataBase * pData = (*m_pContent->m_pChilds)[k];
			if(pData->eWordDataType ==_XWordDataSpanControlBlock)
			{
				::SetRect(&rt,pData->x,pData->y,pData->x + pData->width,pData->y + pData->height);
				if(::PtInRect(&rt,p1))
					return pData;
			}
		}
		return NULL;
	}

}}}}