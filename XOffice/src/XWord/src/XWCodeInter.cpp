#include "StdAfx.h"
#include "XWCodeInter.hpp"
#include "XWordCode.hpp"
#include "XWordPred.hpp"
#include "wpp4.h"
#include <assert.h>
#include "xuxutil.h"
#include "XWordView.hpp"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XUtil;

XWCodeInter::XWCodeInter()
{
}

XWCodeInter::~XWCodeInter()
{
}

void XWCodeInter::DefaultProcess(unsigned char* & pc)
{
	switch(*pc)
	{
		case XWCode::newrow:
			pc += 5;
			break;
		case XWCode::rowheight:
			pc +=5;
			break;
		case XWCode::rowinsert:
			pc += 9;
			break;
		case XWCode::rowdelete:
			pc +=5;
			break;
		case XWCode::torow:
			pc +=5;
			break;
		case XWCode::pushstyle:
			pc +=5;
			break;
		case XWCode::popstyle:
			pc ++;
			break;
		case XWCode::pushchapter:
			pc += 9;
			break;
		case XWCode::popchapter:
			pc ++;
			break;
		case XWCode::pushpara:
			pc += 9;
			break;
		case XWCode::poppara:
			pc ++;
			break;
		case XWCode::pushblock:
			pc += 9;
			break;
		case XWCode::popblock:
			pc ++;
			break;
		case XWCode::setxpos:
			pc += 5;
			break;
		case XWCode::setypos:
			pc += 5;
			break;
		case XWCode::rendertext:
			pc += 9;
			break;
		case XWCode::exit:
			pc += 1;
			break;
		case XWCode::jmp:
			pc += *(int *)(pc + 1);
			break;
		case XWCode::nop:
			pc++;
			break;
		default:
			assert(false);
			break;
	}
}

void XWCodeInter::Inter(XWCodeContentExtent* ctx,unsigned char* pc, unsigned char * end)
{
	int count = 0;

	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	while(!(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				ctx->rowHeights.push_back(*(int*)(pc+1));
				ctx->extent.cy += *(int*)(pc+1);
				pc += 5;
				break;
			case XWCode::rowheight:
				ctx->extent.cy += *(int*)(pc+1) - ctx->rowHeights.back();
				ctx->rowHeights.back() = *(int*)(pc+1) ;
				pc +=5;
				break;
			case XWCode::rowinsert:
				ctx->rowHeights.insert(ctx->rowHeights.begin() + *(int*)(pc+1),
					*(int*)(pc+5));
				ctx->extent.cy += *(int*)(pc+5);
				pc += 9;
				break;
			case XWCode::rowdelete:
				ctx->extent.cy -= ctx->rowHeights[*(int*)(pc+1)];
				ctx->rowHeights.erase(ctx->rowHeights.begin() + *(int*)(pc+1));
				pc +=5;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::exit:
				return;
			default:
				DefaultProcess(pc);
				break;
		}
	}
}

void XWCodeInter::Inter(XWCodePaintContext* ctx, unsigned char* pc, unsigned char * end)
{
	std::vector<int> styles;
	std::vector<vord_t*> blocks;

	ctx->piror_ext = 0;
	POINT ptPoint = ctx->ptPoint;
	int rowHeight = 0;
	int cx = 0;
	int cy = 0;
	int col = 1;

	//ensurce end
	int count = 0;
	int nRow = 0;
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		blocks.push_back((vord_t*)(pc+1));
		pc++;
		count = 1;
		pc+=8;
	}

	bool draw = false;
	while(!(pc == end || count== 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				nRow++;
				if(nRow >= ctx->GetDevice()->startRow) if(!draw) draw  = true;
				if(nRow >= ctx->GetDevice()->endRow && ctx->GetDevice()->endRow > 0)return; 
				rowHeight = *(int*)(pc+1);
				col = 1;
				if(!draw) ctx->piror_ext += rowHeight;
				if(draw)
				{
					ptPoint.y += rowHeight;
					if(ctx->GetDevice()->rcContentPage && 
						ptPoint.y > ctx->GetDevice()->rcContentPage->bottom)
					{
						ctx->fullpage = true;
						return;
					}
					//::MoveToEx(ctx->GetDevice()->GetPaintDC(),160,ptPoint.y,NULL);
					//::LineTo(ctx->GetDevice()->GetPaintDC(),160 + 700,ptPoint.y);
				}
				pc += 5;
				break;
			case XWCode::rowheight:
				ptPoint.y += *(int*)(pc+1) - rowHeight;
				rowHeight = *(int*)(pc+1);
				pc +=5;
				break;
			case XWCode::pushstyle:
				styles.push_back(*(int*)(pc+1));
				pc +=5;
				break;
			case XWCode::popstyle:
				styles.pop_back();
				pc ++;
				break;
			case XWCode::pushchapter:
				blocks.push_back((vord_t*)(pc+1));
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				blocks.pop_back();
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				blocks.push_back((vord_t*)(pc+1));
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				blocks.pop_back();
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blocks.push_back((vord_t*)(pc+1));
				pc += 9;
				break;
			case XWCode::popblock:
				blocks.pop_back();
				pc ++;
				break;
			case XWCode::setxpos:
				cx = *(int*)(pc+1);
				pc += 5;
				break;
			case XWCode::setypos:
				cy = *(int*)(pc+1);
				pc += 5;
				break;
			case XWCode::rendertext:
				{
					if(draw)
					{
						int i = (int)blocks.size() - 1;
						int cssIndex = 1;
						while(i>=0 && !(*blocks[i])[css::index])i--;
						if(i>=0) cssIndex = (*blocks[i])[css::index];
						const Css::TBoxStyle * pBoxStyle = ctx->pDataSvr->m_pStyleSvr->GetBoxStyle(cssIndex - 1);
						if(!pBoxStyle)
							pBoxStyle =  ctx->pDataSvr->m_pStyleSvr->GetBoxStyle(0);

						HDC hPaintDC = ctx->GetDevice()->GetPaintDC();
						LOGFONT LogFont;
						HFONT hFont = NULL;
						Css::GetFont(pBoxStyle,LogFont);

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
		
						//TEXTMETRIC tm;
						//::GetTextMetrics(hPaintDC,&tm);
						//int fHeight = tm.tmHeight;
						int sindex = *(int*)(pc+1);
						int scnt = *(int*)(pc+5);

						int startLen = 0;
						int endLen = 0;
						bool selection = false;
						int curRow = ctx->nRow + nRow;
						if(ctx->GetDevice()->startRow >0)
							curRow -= ctx->GetDevice()->startRow;
						else
							curRow -= 1;

						if(!(ctx->pSelection->startRow==ctx->pSelection->endRow &&
							ctx->pSelection->startCol==ctx->pSelection->endCol))
						{
							if(ctx->pSelection->startRow < curRow &&
								ctx->pSelection->endRow > curRow)
							{
								selection = true;
							}
							if(ctx->pSelection->startRow == curRow)
							{
								if(ctx->pSelection->startCol <= col + scnt)
									selection = true;
								if(ctx->pSelection->startCol >= col && 
									ctx->pSelection->startCol <= col + scnt)
								{
									startLen = ctx->pSelection->startCol - col ;
								}
							}
							if(ctx->pSelection->endRow == curRow)
							{
								if(ctx->pSelection->endCol <= col + scnt) selection = true;
								if(ctx->pSelection->endCol >= col &&
									ctx->pSelection->endCol <= col + scnt)
									endLen = col + scnt - ctx->pSelection->endCol;
							}
						}
						
						if(selection)
						{
							if(startLen)
								ExtTextOut(ctx->GetDevice()->GetPaintDC(),ptPoint.x + cx,ptPoint.y - rowHeight + rowHeight/9 ,0,NULL,(*blocks.back())[text::content] + sindex,startLen,NULL);
							if(endLen)
							{
								int rx = 0;
								if(sindex + scnt - endLen > 0)
									rx = (*blocks.back())[text::pdx][sindex + scnt - endLen - 1];
								if(sindex > 0) rx -= (*blocks.back())[text::pdx][sindex - 1]; 
								ExtTextOut(ctx->GetDevice()->GetPaintDC(),ptPoint.x + cx + rx
									,ptPoint.y - rowHeight + rowHeight/9 ,0,NULL,(*blocks.back())[text::content] + sindex + scnt - endLen,endLen,NULL);
							}

							int slen = scnt - startLen - endLen;
							if(slen > 0)
							{
								COLORREF color = ::SetTextColor(hPaintDC,RGB(255,255,255));
								COLORREF bkcolor =  ::SetBkColor(hPaintDC,RGB(0,0,0));
								int rx = 0;
								if(startLen)
								{
									rx = (*blocks.back())[text::pdx][sindex + startLen - 1];
									if(sindex > 0) rx -= (*blocks.back())[text::pdx][sindex - 1];
								}
								ExtTextOut(ctx->GetDevice()->GetPaintDC(),ptPoint.x + cx + rx,ptPoint.y - rowHeight + rowHeight/9 ,0,NULL,(*blocks.back())[text::content] + sindex + startLen,slen,NULL);
							}

						}else
						{
							if(scnt) ExtTextOut(ctx->GetDevice()->GetPaintDC(),ptPoint.x + cx,ptPoint.y - rowHeight + rowHeight/9 ,0,NULL,(*blocks.back())[text::content] + sindex,scnt,NULL);
						}
						col += scnt;

						::SetTextColor(hPaintDC,TextColor);
						::SetBkColor(hPaintDC,TextBkColor);
						::SetBkMode(hPaintDC,hbr);
						hFont = (HFONT)::SelectObject(hPaintDC,hFont);
						::DeleteObject(hFont);
					}
					pc += 9;
				}
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}	
}

void XWCodeInter::Inter(CalcRowColAtPoint& xt, XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end)
{
	int  count = 0;
	int rowheight = 0;
	int cx=0;
	int cy=0;
	xt.index_ = -1;
	bool rowfound = false;
	std::vector<int> styles;
	std::vector<vord_t*> blocks;

	int nRow = 0;
	bool calc = false;
	xt.col_ = 1;
	xt.block_ = vord_t();
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				if(!rowfound)
				{
					rowheight = *(int*)(pc+1);
					nRow++;
					if(nRow >= xt.startRow) if(!calc)calc  = true;
					if(calc)
					{
						xt.row_++;
						if( xt.pt_.y <= rowheight)
							rowfound = true;
						else
						{
							xt.pt_.y -= rowheight;
						}
					}
				}else
					exit = true;
				pc += 5;
				break;
			case XWCode::pushstyle:
				styles.push_back(*(int*)(pc+1));
				pc +=5;
				break;
			case XWCode::popstyle:
				styles.pop_back();
				pc ++;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blocks.push_back((vord_t*)(pc+1));
				pc += 9;
				break;
			case XWCode::popblock:
				blocks.pop_back();
				pc ++;
				break;
			case XWCode::setxpos:
				if(rowfound)
				{
					cx = *(int*)(pc+1);
					if(xt.pt_.x < cx)exit = true;
				}
				pc += 5;
				break;
			case XWCode::setypos:
				if(rowfound)cy = *(int*)(pc+1);
				pc += 5;
				break;
			case XWCode::rendertext:
				if(rowfound)
				{
					int sindex = *(int*)(pc+1);
					int count =  *(int*)(pc+5);
					if(count < 1)
						if(sindex > 0)
							xt.index_ = sindex;
						else
							xt.index_ = -1;
					else 
					{
						int sx = sindex > 0 ? (*blocks.back())[text::pdx][ sindex - 1]:0;
						if(xt.pt_.x > cx + (*blocks.back())[text::pdx][sindex + count - 1 ] - sx)
							xt.col_ += count;
						else
						{
							int i=0;
							for(;i< count - 1;i++)
							{
								if(xt.pt_.x > cx + (*blocks.back())[text::pdx][sindex +i] - sx )
									xt.col_++;
								else
								{
									int w  = 0;
									int wp = 0;
									if(i==0)
									{
										w = (*blocks.back())[text::pdx][sindex +i] - sx;
										wp = xt.pt_.x  - sx - cx;
									}
									else
									{
										w = (*blocks.back())[text::pdx][sindex +i] - 
												(*blocks.back())[text::pdx][sindex + i - 1];
										wp = xt.pt_.x - (cx + (*blocks.back())[text::pdx][sindex +i - 1] - sx) - cx;
									}
									if(wp >= w/2) xt.col_++;
									break;
								}
									
							}
							xt.index_ = sindex + i;
							exit = true;
						}
					}
				}
				pc += 9;
				break;
			case XWCode::exit:
				return;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	xt.rowheight_ = rowheight;
}
void XWCodeInter::Inter(CalcPointAtRowCol& xt, XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end)
{
	int  count = 0;
	bool rowfound = false;
	int rowheight = 0;
	int cx=0;
	int cy=0;
	int nRow = 0;
	int nCol = 1;
	bool calc = false;
	xt.index_ = -1;
	std::vector<int> styles;
	std::vector<vord_t*> blocks;
	xt.block_ = vord_t();

	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				if(!rowfound)
				{
					rowheight = *(int*)(pc+1);
					nRow++;
					if(nRow >= xt.startRow) if(!calc)calc  = true;
					if(calc)
					{
						xt.row_--;
						if(xt.row_==0)rowfound = true;
						if(!rowfound) xt.pt_.y += rowheight;
					}
				}else
					exit = true;
				pc += 5;
				break;
			case XWCode::pushstyle:
				styles.push_back(*(int*)(pc+1));
				pc +=5;
				break;
			case XWCode::popstyle:
				styles.pop_back();
				pc ++;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blocks.push_back((vord_t*)(pc+1));
				//xt.block_ = *(vord_t*)(pc+1);
				if(xt.block_.groupid() > 32)
				{
					 DebugBreak();
				}
				pc += 9;
				break;
			case XWCode::popblock:
				blocks.pop_back();
				pc ++;
				break;
			case XWCode::setxpos:
				if(rowfound)
				{
					cx = *(int*)(pc+1);
					xt.pt_.x = cx;
				}
				pc += 5;
				break;
			case XWCode::setypos:
				if(rowfound)cy = *(int*)(pc+1);
				pc += 5;
				break;
			case XWCode::rendertext:
				if(rowfound)
				{
					xt.block_ = *blocks.back();
					int sindex = *(int*)(pc+1);
					int count =  *(int*)(pc+5);
					if(count < 1)
						if(sindex > 0)
							xt.index_ = sindex;
						else
							xt.index_ = -1;
					else
					{
						int sx = sindex > 0 ? (*blocks.back())[text::pdx][ sindex - 1]:0;
						if(xt.col_ >= count + nCol)
						{
							nCol += count;
							xt.index_ = sindex + count - 1;
							xt.pt_.x = cx + (*blocks.back())[text::pdx][sindex + count- 1] - sx;
						}
						else
						{
							if(xt.col_ - nCol >= 1)
							{
								xt.pt_.x = cx + (*blocks.back())[text::pdx][sindex + xt.col_ - nCol- 1] - sx;
							}
							xt.index_ = sindex + xt.col_ - nCol - 1;
							exit = true;
						}
					}
				}
				pc += 9;
				break;
			case XWCode::exit:
				return;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	xt.rowheight_ = rowheight;
}

void XWCodeInter::Inter(XWCodeContextPosition* ctx, unsigned char* pc, unsigned char * end)
{
	int  count = 0;
	bool rowfound = false;
	int rowheight = 0;
	int cx=0;
	int cy=0;
	ctx->index = -1;
	std::vector<int> styles;
	std::vector<vord_t*> blocks;

	if(!ctx->calcpt)
	{
		ctx->col = 1;
	}
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				rowheight = *(int*)(pc+1);
				if(ctx->calcpt)
				{
					ctx->row--;
					if(ctx->row==0)rowfound = true;
					if(!rowfound) ctx->pt.y += rowheight;
				}
				else
				{
					ctx->row++;
					if( ctx->pt.y <= rowheight)
						rowfound = true;
					else
					{
						ctx->pt.y -= rowheight;
					}
				}
				pc += 5;
				break;
			case XWCode::rowheight:
				if(!ctx->calcpt && !rowfound)
				{
					int height = *(int*)(pc+1);
					if(height > rowheight)
					{
						if( ctx->pt.y + rowheight <= height)
						{
							rowfound = true;
							
						}else
							ctx->pt.y -= height - rowheight;
						rowheight = height;
					}
				}
				pc +=5;
				break;
			case XWCode::rowinsert:
				pc += 9;
				break;
			case XWCode::rowdelete:
				pc +=5;
				break;
			case XWCode::pushstyle:
				styles.push_back(*(int*)(pc+1));
				pc +=5;
				break;
			case XWCode::popstyle:
				styles.pop_back();
				pc ++;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blocks.push_back((vord_t*)(pc+1));
				pc += 9;
				break;
			case XWCode::popblock:
				blocks.pop_back();
				pc ++;
				break;
			case XWCode::setxpos:
				if(rowfound)
				{
					cx = *(int*)(pc+1);
					if(!ctx->calcpt && ctx->pt.x < cx)exit = true;
					if(ctx->calcpt) ctx->pt.x = cx;
				}
				pc += 5;
				break;
			case XWCode::setypos:
				if(rowfound)
				{
					cy = *(int*)(pc+1);
				}
				pc += 5;
				break;
			case XWCode::rendertext:
				if(rowfound)
				{
					int sindex = *(int*)(pc+1);
					int count =  *(int*)(pc+5);
					int sx = sindex > 0 ? (*blocks.back())[text::pdx][ sindex - 1]:0;
					if(!ctx->calcpt)
					{
						if( ctx->pt.x > cx + (*blocks.back())[text::pdx][sindex + count - 1 ] - sx)
							ctx->col += count;
						else
						{
							int i=0;
							for(;i< count - 1;i++)
							{
								if(ctx->pt.x > cx + (*blocks.back())[text::pdx][sindex +i] - sx )
									ctx->col++;
								else
								{
									int w  = 0;
									int wp = 0;
									if(i==0)
									{
										w = (*blocks.back())[text::pdx][sindex +i] - sx;
										wp = ctx->pt.x  - sx - cx;
									}
									else
									{
										w = (*blocks.back())[text::pdx][sindex +i] - 
												(*blocks.back())[text::pdx][sindex + i - 1];
										wp = ctx->pt.x - (cx + (*blocks.back())[text::pdx][sindex +i - 1] - sx) - cx;
									}
									if(wp >= w/2) ctx->col++;
									break;
								}
									
							}
							ctx->index = sindex + i;
							exit = true;
						}
					}else
					{
						if(ctx->col > count)
							ctx->pt.x = cx + (*blocks.back())[text::pdx][sindex + count- 1] - sx;
						else
						{
							if(ctx->col>1)
							{
								ctx->pt.x = cx + (*blocks.back())[text::pdx][sindex + ctx->col - 2] - sx;
							}
							ctx->index = sindex + ctx->col - 1;
							exit = true;
						}
					}
				}
				pc += 9;
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	ctx->found = rowfound;
	ctx->rowheight = rowheight;
}

void XWCodeInter::Inter(CalcBlockAtRow & blockatrow,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end)
{
	int  row = blockatrow.row_;

	int  count = 0;
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	bool found = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				if(row==1) 
					found=true;
				else
					row--;
				pc += 5;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blockatrow.block_ = *(vord_t*)(pc+1);
				if(found)exit = true;
				pc += 9;
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	if(!exit)blockatrow.block_ = vord_t();
}


void XWCodeInter::Inter(CalcBlockAtRowCol & blockatrowcol,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end )
{
	int  row = blockatrowcol.row_;
	int  col = blockatrowcol.col_;

	int  count = 0;
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	bool found = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				if(row==1) 
					found=true;
				else
					row --;
				pc += 5;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				blockatrowcol.block_ = *(vord_t*)(pc+1);
				pc += 9;
				break;
			case XWCode::rendertext:
				if(found)
				{
					int sindex = *(int*)(pc+1);
					int count =  *(int*)(pc+5);
					if(col <= count + 1)
					{
						blockatrowcol.index = sindex + col - 2;
						exit = true;
					}
					else
					{
						blockatrowcol.index = sindex + count;
						col -= count;
					}
				}
				pc += 9;
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	if(!exit)blockatrowcol.block_ = vord_t();
}

void XWCodeInter::Inter(CalcColCount &colcount,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end)
{
	int  row = colcount.row_;

	int  count = 0;
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	bool found = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				if(found)
					exit =true;
				else
				{
					if(row==1) 
						found=true;
					else
						row --;
				}
				pc += 5;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::rendertext:
				if(found)
				{
					int count =  *(int*)(pc+5);
					colcount.count_ += count;
				}
				pc += 9;
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	if(!found)colcount.count_ = 0;
}

void XWCodeInter::Inter(CalcRowColAtBlock  &rcblock,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end )
{
	vord_t block = rcblock.block_;
	int charIndex = rcblock.index_;
	
	rcblock.row_ = 0;
	rcblock.col_ = 0;
	int col = 1;

	int count = 0;
	//ensurce end
	if(!end && (*pc == XWCode::pushchapter || *pc == XWCode::pushpara))
	{
		pc++;
		count = 1;
		pc+=8;
	}
	bool exit = false;
	bool found = false;
	while(!exit && !(pc == end || count == 0))
	{
		switch(*pc)
		{
			case XWCode::newrow:
				rcblock.row_++;
				col = 1;
				pc += 5;
				break;
			case XWCode::pushchapter:
				count++;
				pc += 9;
				break;
			case XWCode::popchapter:
				count--;
				pc ++;
				break;
			case XWCode::pushpara:
				count++;
				pc += 9;
				break;
			case XWCode::poppara:
				count--;
				pc ++;
				break;
			case XWCode::pushblock:
				if(((vord_t*)(pc+1))->equal(block)) found = true;
				pc += 9;
				break;
			case XWCode::rendertext:
				{
					int sindex = *(int*)(pc+1);
					int count =  *(int*)(pc+5);
					if(!found)
					{
						col += count;
					}
					else
					{
						if(charIndex < sindex)
						{
							exit = true;
							break;
						}else if(charIndex <sindex + count)
						{
							col += charIndex - sindex + 1;
							exit = true;
						}else
						{
							col += count;
						}
					}
				}
				pc += 9;
				break;
			case XWCode::exit:
				return ;
			default:
				DefaultProcess(pc);
				break;
		}
	}
	if(found)rcblock.col_ = col;
}
