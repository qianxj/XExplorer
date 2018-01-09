#include "StdAfx.h"
#include "XWordLayerPara.hpp"
#include "XWordBlock.hpp"
#include "xtree.hpp"
#include "XWordLayerDocument.hpp"
#include "XWordPred.hpp"
#include "XWordDevice.hpp"
#include "XWCodeAsm.hpp"
#include "XWCodeInter.hpp"
#include "XWCodeContext.hpp"

XWordLayerPara::XWordLayerPara(void)
{
}

XWordLayerPara::~XWordLayerPara(void)
{
}

int XWordLayerPara::DoDraw(HDC hPaintDC,RECT * pDrawRect)
{
	return 1;
}

int XWordLayerPara::DrawContent(XWordLayerDocument * pDocument, BlockMetaInfo * pBlockMetaInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen)
{
#if 0
	LOGFONT LogFont;
	HFONT hFont = NULL;
	Css::GetFont(pBlockMetaInfo->pBoxStyle,LogFont);

	int nYPixelPerInch = GetDeviceCaps(hPaintDC, LOGPIXELSY);
	HDC dc = GetDC(NULL);
	int _nYPixelPerInch = GetDeviceCaps(dc, LOGPIXELSY);
	::ReleaseDC(NULL,dc);
	::MulDiv(LogFont.lfHeight, nYPixelPerInch,_nYPixelPerInch);
	
	hFont = ::CreateFontIndirect(&LogFont);
	hFont = (HFONT)::SelectObject(hPaintDC,hFont);

	COLORREF	TextColor = ::GetTextColor(hPaintDC);
	int hbr = ::GetBkMode(hPaintDC);
	COLORREF	TextBkColor = ::GetBkColor(hPaintDC);
	COLORREF	crColor = RGB(0,0,0);
	Css::TBoxStyle * pBoxStyle = pBlockMetaInfo->pBoxStyle;
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

/*	if(pDocument->m_Selection.bSelection)
	{
		if(pDocument->BlockIsSelected((XWordDataTextBlock *)pBlockMetaInfo->pBlock,
			(XWordDataTextBlock *)pDocument->m_Selection.pStartBlock,
			(XWordDataTextBlock *)pDocument->m_Selection.pEndBlock))
		{
			//起始部分
			if(pBlockMetaInfo->pBlock==pDocument->m_Selection.pStartBlock && nStart < pDocument->m_Selection.nStartPosChar)
			{
				int nEnd = pDocument->m_Selection.nStartPosChar;
				if(nEnd > nStart + nLen) nEnd = nStart + nLen;
				ExtTextOut(hPaintDC,rt.left ,rt.top ,0,NULL,pBlockMetaInfo->pStr+nStart,nEnd - nStart ,NULL);
			}

			//中间部分
			int nStartEx= -1,nEndEx = -1;
			if(pBlockMetaInfo->pBlock==pDocument->m_Selection.pStartBlock )
			{
				if(nStart < pDocument->m_Selection.nStartPosChar)
					nStartEx =pDocument->m_Selection.nStartPosChar;
				else
					nStartEx = nStart;
			}else if(pDocument->GetIndex(pBlockMetaInfo->pBlock)>pDocument->GetIndex(pDocument->m_Selection.pStartBlock))
			{
				nStartEx = nStart;
			}
			if(pBlockMetaInfo->pBlock==pDocument->m_Selection.pEndBlock )
			{
				if(nStart + nLen > pDocument->m_Selection.nEndPosChar)
				{
					nEndEx =pDocument->m_Selection.nEndPosChar;
				}else
					nEndEx = nStart + nLen;
			}else if(pDocument->GetIndex(pBlockMetaInfo->pBlock)<pDocument->GetIndex(pDocument->m_Selection.pEndBlock))
			{
				nEndEx = nStart + nLen;
			}

			if(nEndEx!=-1 && nStartEx!=-1)
			{
				int nStartPos = 0;
				if(pBlockMetaInfo->pDx)
				{
					if(nStartEx != nStart)
					{
						nStartPos = pBlockMetaInfo->pDx[nStartEx - 1] -
							(nStart>0?pBlockMetaInfo->pDx[nStart- 1] :0);
					}
				}

				int hbr = ::SetBkMode(hPaintDC,OPAQUE);
				COLORREF bkColor = ::SetBkColor(hPaintDC,RGB(0,0,0));
				COLORREF tColor = RGB(255 - GetRValue(crColor),
						255 - GetGValue(crColor),
						255 - GetBValue(crColor));
				tColor = ::SetTextColor(hPaintDC,tColor);
				ExtTextOut(hPaintDC,rt.left + nStartPos ,rt.top ,0,NULL,pBlockMetaInfo->pStr+nStartEx,nEndEx - nStartEx ,NULL);
				tColor = ::SetTextColor(hPaintDC,tColor);
				::SetBkMode(hPaintDC,hbr);
				SetBkColor(hPaintDC,bkColor);
			}

			//结束部分
			if(pBlockMetaInfo->pBlock==pDocument->m_Selection.pEndBlock && nStart + nLen >= pDocument->m_Selection.nEndPosChar)
			{
				int nStartPos = 0;
				int nEnd = pDocument->m_Selection.nEndPosChar;
				if(nEnd < nStart)
				{
					ExtTextOut(hPaintDC,rt.left+ nStartPos,rt.top ,0,NULL,pBlockMetaInfo->pStr + nStart,nLen,NULL);
				}else
				{
					if(pBlockMetaInfo->pDx)
					{
						nStartPos = (pDocument->m_Selection.nEndPosChar>0?pBlockMetaInfo->pDx[pDocument->m_Selection.nEndPosChar - 1]:0) -
								(nStart>0?pBlockMetaInfo->pDx[nStart- 1] :0);
					}
					ExtTextOut(hPaintDC,rt.left+ nStartPos,rt.top ,0,NULL,pBlockMetaInfo->pStr + pDocument->m_Selection.nEndPosChar,nStart + nLen - pDocument->m_Selection.nEndPosChar ,NULL);
				}
			}
		}
		else
			ExtTextOut(hPaintDC,rt.left,rt.top ,0,NULL,pBlockMetaInfo->pStr+nStart,nLen,NULL);
	}else
	{*/
		ExtTextOut(hPaintDC,rt.left,rt.top ,0,NULL,pBlockMetaInfo->pStr+nStart,nLen,NULL);
		//::DrawText(hPaintDC,pBlockMetaInfo->pStr+nStart,nLen,&rt,DT_LEFT);
	//}
	
	::SetTextColor(hPaintDC,TextColor);
	::SetBkColor(hPaintDC,TextBkColor);
	::SetBkMode(hPaintDC,hbr);
	hFont = (HFONT)::SelectObject(hPaintDC,hFont);
	::DeleteObject(hFont);
#endif
	return 1;
}

int  XWordLayerPara::DoDraw(XWordLayerDocument * pDocument,BlockMetaInfo pBlockMetaInfo[], int nLen, HDC hPaintDC,POINT & ptOffset)
{
#if 0
	int nFirstWordBala = 20;
	int nItem = 0;
	int nWidth = FContentRect.right - FContentRect.left;
	//POINT ptOffset = {nFirstWordBala,0};
	ptOffset.x += nFirstWordBala;

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
		if(pBlockMetaInfo[i].pDx)
		{
			rRects.clear();
			this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
			rRectIndex = 0;
			nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

			LPTSTR pStr = pBlockMetaInfo[i].pStr;
			int  nStrLen   = (int)_tcslen(pStr);
			int  nIndex	= 0;
			int	 nIndexStart = 0;
			bool bNewRow = false;
			while(true)
			{
				bNewRow = false;
				while(nIndex < nStrLen && pBlockMetaInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
				if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
				{
					//draw text;
					SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,ptOffset.y,
						rRects[rRectIndex].right + 1,ptOffset.y + pBlockMetaInfo[i].ContentExtent.cy);
					::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
					DrawContent(pDocument,pBlockMetaInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);

					//前面部分的字符都可以填冲，开始新的一行
					nIndex++;
					if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight)
						nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
					bNewRow = true; //开始新行
				}else if(nIndex == nIndexStart && nStrLen) //not fit one char
				{
					//如果没有填充，并且是整个宽度，则强制填充
					if(!bHasFited && nWidth - nRight <= 2 )
					{
						nIndex++; //fit it

						//draw text;
						SetRect(&rcDraw,rRects[rRectIndex].right - nRight + 1,rRects[rRectIndex].top,
							rRects[rRectIndex].right + 1,rRects[rRectIndex].top + pBlockMetaInfo[i].ContentExtent.cy);
						::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
						DrawContent(pDocument,pBlockMetaInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);


						if(pBlockMetaInfo[i].ContentExtent.cy > nHeight) 
							nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
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
						rRects[rRectIndex].right + 5,ptOffset.y + pBlockMetaInfo[i].ContentExtent.cy);
					::OffsetRect(&rcDraw,FContentRect.left,FContentRect.top);
					DrawContent(pDocument,pBlockMetaInfo+i,rcDraw,hPaintDC,nIndexStart,nIndex - nIndexStart);

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
							(nIndex>=1?pBlockMetaInfo[i].pDx[nIndex - 1] - 
							(nIndexStart >= 1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) :0);

						nRight -= nIndex>=1?pBlockMetaInfo[i].pDx[nIndex - 1] - 
							(nIndexStart >= 1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) :0;
						bHasFited = true;
					}

					//如高度大于前面的高度，使用新高度
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight) 
						nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
				}
				if(bNewRow)
				{
					//设置行高度

					//增加行号
					nItem++;

					//重置行参数
					ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
					nHeight = 0;
					bHasFited = false;
					rRects.clear();
					this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
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
#endif
	return 1;
}


int  XWordLayerPara::DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect)
{
#if 0
	const Css::TBoxStyle * pBoxStyle = NULL;
	vord_t block = get_block();
	if(block[css::index]>=0)pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	if(!pBoxStyle) pBoxStyle = pParentBoxStyle;
	if(!pBoxStyle) 
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);
	
	std::vector<BlockMetaInfo> MetaInfoes; 
	BlockMetaInfo MetaInfo;
	for(vord_t item =  block.get_child();item;item = item.get_next())
	{
		if(item[block::type] == blocktype::text)
		{
			CalcBlockMetaInfo(pDataSvr,(Css::TBoxStyle *)pBoxStyle,hPaintDC,item,&MetaInfo);
			MetaInfoes.push_back(MetaInfo);
		}
	}
	if(MetaInfoes.size())
	{
		DoDraw(pDocument,&*MetaInfoes.begin(),(int)MetaInfoes.size(), hPaintDC,ptOffset);
		ClearMetaInfo(&*MetaInfoes.begin(),(int)MetaInfoes.size());
	}
#endif
	return 1;
}

SIZE XWordLayerPara::CalcBlockMetaInfo(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle, HDC hDC,vord_t&  pBlock,BlockMetaInfo * pBlockMetaInfo)
{
	const Css::TBoxStyle * pBoxStyle = NULL;
	vord_t& block = pBlock;
	if(block[css::index] >= 1)pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	if(!pBoxStyle) pBoxStyle = pParentBoxStyle;
	if(!pBoxStyle) 
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	LOGFONT LogFont;
	HFONT hFont = NULL;
	Css::GetFont(pBoxStyle,LogFont);
	if(hDC)
	{
		int nYPixelPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
		HDC dc = GetDC(NULL);
		int _nYPixelPerInch = GetDeviceCaps(dc, LOGPIXELSY);
		::ReleaseDC(NULL,dc);
		::MulDiv(LogFont.lfHeight, nYPixelPerInch,_nYPixelPerInch);
	}
	hFont = ::CreateFontIndirect(&LogFont);

	LPTSTR pStr = block[text::content];
	int nStrLen =  (int)wcslen(pStr);
	int nFit;
	SIZE size;
	int * pDx = nStrLen >0 && pBlockMetaInfo ? new int[nStrLen] : NULL;
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
	
	if(pBlockMetaInfo)
	{
		pBlockMetaInfo->bDeleteBoxStyle = false;
		pBlockMetaInfo->pBoxStyle = (Css::TBoxStyle *)pBoxStyle;
		pBlockMetaInfo->ContentExtent = size;
		pBlockMetaInfo->nLenStr = nStrLen;
		pBlockMetaInfo->pStr = pStr;
		pBlockMetaInfo->pDx = pDx;
		pBlockMetaInfo->pBlock = pBlock;
	}
	if(pBlock[text::pdx])delete pBlock[text::pdx];
	pBlock[text::pdx] = pDx;

	return size;
}

int  XWordLayerPara::ClearMetaInfo(BlockMetaInfo pBlockMetaInfo[],int nCount)
{
	for(int i=0;i<nCount;i++)
	{
		if(pBlockMetaInfo[i].bDeleteBoxStyle)
			delete pBlockMetaInfo[i].pBoxStyle;
		//if(pBlockMetaInfo[i].pDx) delete pBlockMetaInfo[i].pDx; 
	}
	
	return 1;
}

SIZE XWordLayerPara::ReCalcExtent(BlockMetaInfo pBlockMetaInfo[],int nLen)
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

	SIZE extent={0,0};
	extent.cx = nWidth;
	nHeight = 0;
	for(int i=0;i<nLen;i++)
	{
		rRects.clear();

		//得到可填充矩阵
		if(pBlockMetaInfo[i].pDx)
		{
			rRects.clear();
			this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
			rRectIndex = 0;
			nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;

			LPTSTR pStr = pBlockMetaInfo[i].pStr;
			int  nStrLen   = (int)_tcslen(pStr);
			int  nIndex	= 0;
			int	 nIndexStart = 0;
			bool bNewRow = false;
			while(true)
			{
				bNewRow = false;
				//得到可填充的字数
				while(nIndex < nStrLen && pBlockMetaInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) <= nRight && pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
				if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
				{
					//前面部分的字符都可以填冲，开始新的一行
					nIndex++;
					if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight)
						nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
					bNewRow = true; //开始新行
				}else if(nIndex == nIndexStart && nStrLen) //not fit one char
				{
					//如果没有填充，并且是整个宽度，则强制填充
					if(!bHasFited && nWidth - nRight <= 2 )
					{
						nIndex++; //fit it

						if(pBlockMetaInfo[i].ContentExtent.cy > nHeight) 
							nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
					}
					bNewRow = true;	//开始新行
				}else
				{
					//如最后不够填充整个词,则退出这个词
					if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
					{
						int nPos = nIndex;
						nIndex --;
						while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
						//如一行就只有一个词
						if(nIndex == nIndexStart) nIndex = nPos;
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
							(nIndex>=1?pBlockMetaInfo[i].pDx[nIndex - 1] - 
							(nIndexStart >= 1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) :0);

						nRight -= nIndex>=1?pBlockMetaInfo[i].pDx[nIndex - 1] - 
							(nIndexStart >= 1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) :0;
						bHasFited = true;
					}

					//如高度大于前面的高度，使用新高度
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight) 
						nHeight = pBlockMetaInfo[i].ContentExtent.cy; 
				}

				if(bNewRow)
				{
					//设置行高度
					extent.cy += (int)(nHeight * 1.5);

					//增加行号
					nItem++;

					//重置行参数
					ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
					nHeight = 0;
					bHasFited = false;
					rRects.clear();
					this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
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

SIZE XWordLayerPara::GetExtent()
{
	SIZE sz = {0,0};
	vord_t block = get_block();
	if(block)
	{
		sz.cx = block[extent_c::cx];
		sz.cy = block[extent_c::cy];
	}
	return sz;
}

SIZE XWordLayerPara::GetRealExtent()
{
	SIZE sz = {0,0};
	vord_t block = get_block();
	if(block)
	{
		sz.cx = block[extent_c::cx];
		sz.cy = block[extent_c::cy];
	}
	return sz;
}

SIZE XWordLayerPara::CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC)
{
	SIZE extent={0,0};
	XWordDevice device;
	XWCodeInter machine;
	XWCodeContentExtent context;
	XWCodeAsm wasm(pBlock.get_parent()[render::code],
		pBlock.get_parent()[render::code] + 
		pBlock.get_parent()[render::pc],
		pBlock.get_parent()[render::codelimit]);

	if(!wasm.code)
	{
		printf("%S",L"rander code is null\r\n");
		return extent;
	}

	int prev_pc = pBlock[render::pc];
	int prev_epc = pBlock[render::epc];

	device.SetDC(hDC);
	device.SetPaintDC(hDC);

	int pc = (int)(wasm.pc - wasm.code);
	CalcLayer(pDataSvr,(Css::TBoxStyle *)pParentBoxStyle,&wasm,&device);
	int epc = (int) (wasm.pc - wasm.code);

	int cy1 = pBlock[extent_c::cy];
	int cx1 = pBlock[extent_c::cx];

	context.extent.cx = 0;
	context.extent.cy = 0;
	machine.Inter(&context,wasm.code + pc);
	extent = context.extent;

	//判断代码覆盖原来的空间是否足够
	if(epc - pc <= prev_epc - prev_pc)
	{
		//move code;
		::memmove(wasm.code + prev_pc, wasm.code + pc, epc - pc);
		if((prev_epc - prev_pc) - (epc - pc) > 0)
			::memset(wasm.code + prev_pc + (epc - pc) ,XWCode::nop, (prev_epc - prev_pc) - (epc - pc));
		pBlock[render::epc] = prev_pc + epc - pc;
		wasm.pc = wasm.code + pc;
	}else
	{
		pBlock[render::pc]  = pc;
		pBlock[render::epc] = epc;
	}
	
	//update para extent
	pBlock[extent_c::cy] = extent.cy;
	pBlock[extent_c::cx] = extent.cx;
	pBlock[render::rowcount] = (int)context.rowHeights.size();

	//update chapter extent
	pBlock.get_parent()[extent_c::cy] += extent.cy - cy1; 
	pBlock.get_parent()[extent_c::cx] += extent.cx - cx1;

	//update doc
	pBlock.get_parent().get_parent()[extent_c::cy] += extent.cy - cy1; 
	pBlock.get_parent().get_parent()[extent_c::cx] += extent.cx - cx1;

	pBlock.get_parent()[render::code] = wasm.code; 
	pBlock.get_parent()[render::codelimit] = wasm.limit_;
	pBlock.get_parent()[render::pc] = (int)(wasm.pc - wasm.code);

	return extent;

#if 0
	SIZE extent={0,0};
	const Css::TBoxStyle * pBoxStyle;
	pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(pBlock[css::index] - 1);

	if(!pBoxStyle)
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);

	//para zorder
	//para rect
	//para content rect
	//para before
	//para first word indent
	//para align
	//para row height
	//para after

	int cHeight = 0;
	std::vector<BlockMetaInfo> MetaInfoes;
	BlockMetaInfo MetaInfo;
	vord_t item = pBlock->get_child();
	while(item)
	{
		if(item[block::type] == blocktype::text)
		{
			CalcBlockMetaInfo(pDataSvr,(Css::TBoxStyle *)pBoxStyle,hDC,item,&MetaInfo);
			MetaInfoes.push_back(MetaInfo);
		}
		item = item.get_next();
	}

	if(MetaInfoes.size())
	{
		extent = ReCalcExtent(&*MetaInfoes.begin(),(int)MetaInfoes.size());
		this->ClearMetaInfo(&*MetaInfoes.begin(),(int)MetaInfoes.size());
	}
	if(extent.cy < cHeight)
		extent.cy = cHeight + 1;
	(*get_block())[extent_c::cy] = extent.cy;
	return extent;
#endif

}

//计算右边可容纳矩阵的宽度，计算时需要排除ExRect
int XWordLayerPara::GetLayerRight(BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset)
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
int XWordLayerPara::GetLayerRightRects(std::vector<RECT> &rRect,BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset)
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

//得到相交的排除矩阵
BOOL XWordLayerPara::GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound)
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
int XWordLayerPara::SortRects(std::vector<RECT> &vRect)
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

const Css::TBoxStyle *  XWordLayerPara::GetBoxStyle(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,vord_t pBlock)
{
	const Css::TBoxStyle * pBoxStyle = NULL;
	if(pBlock[css::index]>0)pBoxStyle = pDataSvr->m_pStyleSvr->GetBoxStyle(pBlock[css::index] - 1);
	if(!pBoxStyle) pBoxStyle = pParentBoxStyle;
	if(!pBoxStyle) 
		pBoxStyle =  pDataSvr->m_pStyleSvr->GetBoxStyle(0);
	return pBoxStyle;
	
}

int XWordLayerPara::CalcLayer(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,
							  XWCodeAsm  * pAsm, XWordDevice * pDevice)
{
	vord_t block = get_block();
	const Css::TBoxStyle * pBoxStyle = GetBoxStyle(pDataSvr, pParentBoxStyle,block);

	std::vector<BlockMetaInfo> MetaInfoes;
	BlockMetaInfo MetaInfo;
	for(vord_t item =  block.get_child();item;item = item.get_next())
	{
		if(item[block::type] == blocktype::text)
		{
			CalcBlockMetaInfo(pDataSvr,(Css::TBoxStyle *)pBoxStyle,pDevice->GetDC(),item,&MetaInfo);
			MetaInfoes.push_back(MetaInfo);
		}
	}
	if(MetaInfoes.size())
	{
		CalcLayer(pAsm,&*MetaInfoes.begin(),(int)MetaInfoes.size());
		ClearMetaInfo(&*MetaInfoes.begin(),(int)MetaInfoes.size());
	}

	return 1;
}

int  XWordLayerPara::CalcLayer(XWCodeAsm  * pAsm, BlockMetaInfo pBlockMetaInfo[], int nLen)
{
	int nFirstWordBala = 20;
	int nWidth = FContentRect.right - FContentRect.left;
	POINT ptOffset = {nFirstWordBala,0};

	bool bHasFited = false;
	std::vector<RECT> rRects;
	int nRight = 0;
	bool bStart = false; //start try to fit text
	int rRectIndex = 0;
	int nColumn = 0;
	int nHeight = 0;

	//enter para
	pAsm->pushpara(get_block()); 
	for(int i=0;i<nLen;i++)
	{
		//得到可填充矩阵
		if(pBlockMetaInfo[i].pDx || (pBlockMetaInfo[i].nLenStr == 0 && nLen==1))
		{
			pAsm->pushblock(pBlockMetaInfo[i].pBlock);
			//if((*pBlockMetaInfo[i].pBlock)[css::index])
			//	pAsm->pushstyle((*pBlockMetaInfo[i].pBlock)[css::index]);

			//if bHasFited should fill continue;
			if(!bHasFited)
			{
				//initial column no
				nColumn = 0;

				//new row to fill text
				nHeight = pBlockMetaInfo[i].ContentExtent.cy;

				//new row
				pAsm->newrow(nHeight * 1.5);

				//calc right rect that can be fited ;
				rRects.clear();
				this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
				rRectIndex = 0;
				nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1 ;
			}

			//get text which to fited
			LPTSTR pStr = pBlockMetaInfo[i].pStr;
			int  nStrLen   = (int)_tcslen(pStr);
			
			//fited text to right rect
			int  nIndex	= 0;
			int	 nIndexStart = 0;
			bool bNewRow = false;
			
			while(true)
			{
				bNewRow = false;
				while(nIndex < nStrLen 
					&& pBlockMetaInfo[i].pDx[nIndex] - (nIndexStart>=1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) <= nRight 
					&& pStr[nIndex]!='\r' && pStr[nIndex]!='\n' && pStr[nIndex]!=0x0B )nIndex++;
				if(pStr[nIndex]=='\r'||pStr[nIndex]=='\n'||pStr[nIndex]==0x0B)
				{
					//ensure row height
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight)
					{
						nHeight = pBlockMetaInfo[i].ContentExtent.cy;
						pAsm->rowheight(nHeight * 1.5);
					}

					//render text;
					pAsm->setxpos(rRects[rRectIndex].right - nRight + 1);
					pAsm->rendertext(nIndexStart,nIndex - nIndexStart);

					//前面部分的字符都可以填充，开始新的一行
					nIndex++;
					//trim one ln;
					if(pStr[nIndex - 1]=='\r' && pStr[nIndex]=='\n')nIndex++;

					bNewRow = true; //开始新行
				}else if(nIndex == nIndexStart && nStrLen) //not fit one char
				{
					//如果没有填充，并且是整个宽度，则强制填充
					if(!bHasFited && nWidth - nRight <= 2 )
					{
						nIndex++; //fit it

						//ensure row height
						if(pBlockMetaInfo[i].ContentExtent.cy > nHeight)
						{
							nHeight = pBlockMetaInfo[i].ContentExtent.cy;
							pAsm->rowheight(nHeight * 1.5);
						}

						//render text;
						pAsm->setxpos(rRects[rRectIndex].right - nRight + 1);
						pAsm->rendertext(nIndexStart,nIndex  - nIndexStart);

					}
					bNewRow = true;	//开始新行
				}else
				{
					//如果最后一个是英语单词,并且不能全部填充,则换行填充
					if(pStr[nIndex]>='0' && pStr[nIndex]<='z')
					{
						int nPos = nIndex;
						nIndex --;
						while(nIndex > nIndexStart && ((pStr[nIndex]>='0' && pStr[nIndex]<='z') || pStr[nIndex]=='_'))nIndex--;
						//如一行就只有一个词,则硬性填充
						if(nIndex == nIndexStart) nIndex = nPos;
					}

					//ensure row height
					if(pBlockMetaInfo[i].ContentExtent.cy > nHeight)
					{
						nHeight = pBlockMetaInfo[i].ContentExtent.cy;
						pAsm->rowheight(nHeight * 1.5);
					}

					//render text;
					pAsm->setxpos(rRects[rRectIndex].right - nRight + 1);
					pAsm->rendertext(nIndexStart,nIndex  - nIndexStart);

					if(nIndex < nStrLen)
					{
						//如果右边空间已填充完成,则开始新行
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
							//填充下一矩形
							rRectIndex++;
							nRight = rRects[rRectIndex].right - rRects[rRectIndex].left + 1;
						}
					}else
					{//全部文本已填充完成,填充下一文本
						nRight -= nIndex>=1?pBlockMetaInfo[i].pDx[nIndex - 1] - 
							(nIndexStart >= 1?pBlockMetaInfo[i].pDx[nIndexStart - 1]:0) :0;
						bHasFited = true;
						break;
					}
				}
				if(bNewRow)
				{
					//intital row param
					ptOffset.x = 0 , ptOffset.y += (int)(nHeight * 1.5);
					bHasFited = true;

					//设置行高度
					nHeight = pBlockMetaInfo[i].ContentExtent.cy;

					//add row
					pAsm->newrow(nHeight * 1.5);

					//get fill rect
					rRects.clear();
					this->GetLayerRightRects(rRects,pBlockMetaInfo,pBlockMetaInfo[i].ContentExtent.cy,16,ptOffset);
					rRectIndex = 0;
					nRight = rRects[0].right - rRects[0].left + 1;
				}
				nIndexStart = nIndex;
				bNewRow = false;
				//如已全部填充，则退出
				if(nIndex == nStrLen && pStr && nIndex > 0 && (pStr[nIndex - 1]=='\n'||pStr[nIndex - 1]=='\r'))
				{
					pAsm->setxpos(rRects[0].left);
					pAsm->rendertext(nIndex,0);
				}
				if(nIndex >= nStrLen) break;
			}

			//if((*pBlockMetaInfo[i].pBlock)[css::index])
			//	pAsm->popstyle();
			pAsm->popblock();
		}else
		{
			pAsm->pushblock(pBlockMetaInfo[i].pBlock);
			if(!bHasFited)
			{
				//new row to fill text
				nHeight = pBlockMetaInfo[i].ContentExtent.cy;
				//new row
				pAsm->newrow(nHeight * 1.5);
				bHasFited = true;
			}
			pAsm->popblock();
		}
	}
	//leave para
	pAsm->poppara();

	return 1;
}
