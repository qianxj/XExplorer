#include "StdAfx.h"
#include "xReportHeaderSvr.hpp"
#include "xreportdatasvr.hpp"
#include "xreportsheetsvr.hpp"
#include "xcControl.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportBandSvr::XReportBandSvr():eBand(none),m_nIndexStyle(-1),m_nHeight(0),strXPath(NULL),m_pDataSvr(NULL)
	{
	}

	XReportBandSvr::~XReportBandSvr()
	{
	}

	XReportHeaderSvr::XReportHeaderSvr(void):m_nStartCol(0), m_nRowHeight(17)
	{
	}

	XReportHeaderSvr::~XReportHeaderSvr(void)
	{
		for(int i=0;i<m_HeaderItems.size();i++)
		{
			if(m_HeaderItems[i]) delete m_HeaderItems[i];
		}
	}

	int XReportHeaderSvr::GetTotalLevel()
	{
		int nTotalLevel = 0;
		int nLevel = 0;
		for(int i=0;i<m_HeaderItems.size();i++)
		{
			 nLevel = GetTotalLevel(m_HeaderItems[i]);
			 if(nLevel > nTotalLevel) nTotalLevel = nLevel; 
		}
		return nTotalLevel;
	}
	int XReportHeaderSvr::GetTotalLevel(ColHeaderItem * pHeaderItem)
	{
		int nTotalLevel = 0;
		int nLevel = 0;
		for(int i=0;i<pHeaderItem->m_HeaderItems.size();i++)
		{
			 nLevel = GetTotalLevel(pHeaderItem->m_HeaderItems[i]);
			 if(nLevel > nTotalLevel) nTotalLevel = nLevel; 
		}
		return nTotalLevel + 1;
	}

	void XReportHeaderSvr::OnDraw(HDC hPaintDC,RECT &rect)
	{
		RECT	rc;
		rc = rect;
		rc.bottom = rc.top + GetTotalLevel()*m_nRowHeight;
		int nSaveDC = ::SaveDC(hPaintDC);
		::IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(164,128,174));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		int nBkMode = ::SetBkMode(hPaintDC,TRANSPARENT);

		const XUtil::Css::TBoxStyle * pStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(this->m_nIndexStyle);
		if(pStyle)
			XUtil::Css::DrawCssFrame(hPaintDC,&rc,pStyle);

		OnDraw(hPaintDC,rect,m_HeaderItems);

		::SetBkMode(hPaintDC,nBkMode);
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		::RestoreDC(hPaintDC,nSaveDC);
	}
	void XReportHeaderSvr::OnDraw(HDC hPaintDC,RECT &rect,XUtil::xuVector<ColHeaderItem *> & HeaderItems,int nLevel)
	{
		RECT rc= rect;
		RECT trect = rc;
		int rowHeight = m_nRowHeight;
		for(int i=0;i<HeaderItems.size();i++)
		{

			if(HeaderItems[i]->m_HeaderItems.size()>0)
				trect.bottom = trect.top + rowHeight;
			else
				trect.bottom = trect.top + rowHeight*(GetTotalLevel() - nLevel + 1);
			trect.right = trect.left + GetColWidth( hPaintDC, HeaderItems[i],m_pDataSvr->m_pStyleSvr);
			if(trect.right > rect.right) trect.right = rect.right;
			//::Rectangle(hPaintDC,trect.left,trect.top,trect.right+1,trect.bottom+1);
			const XUtil::Css::TBoxStyle * pStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(HeaderItems[i]->m_nIndexStyle);
			if(pStyle)
				XUtil::Css::DrawCssFrame(hPaintDC,&trect,pStyle);

			LOGFONT	LogFont;
			XUtil::Css::GetFont(pStyle,LogFont);
			HFONT	hFont = ::CreateFontIndirect(&LogFont);
			hFont = (HFONT)SelectObject(hPaintDC,hFont);

			RECT drect = trect;
			InflateRect(&drect,-2,-2);
			::DrawText(hPaintDC,HeaderItems[i]->pStrText,
				(int)_tcslen(HeaderItems[i]->pStrText),&drect,DT_WORDBREAK|DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			hFont = (HFONT)SelectObject(hPaintDC,hFont);
			::DeleteObject(hFont);

			RECT	srect=trect;
			srect.top = trect.bottom + 1;
			srect.bottom = rc.bottom;
			OnDraw(hPaintDC,srect,HeaderItems[i]->m_HeaderItems,nLevel + 1);

			trect.left = trect.right + 1;
		}
	}

	int XReportHeaderSvr::GetColWidth(HDC hPaintDC,ColHeaderItem * pHeaderItem,XOfficeStyleSvr * pStyleSvr)
	{
		int width=0;
		int twidth=0;
		if(pHeaderItem->m_nWidth<0)return -1;
		if(pHeaderItem->m_nWidth==0)
		{
			const XUtil::Css::TBoxStyle * pStyle = pStyleSvr->GetBoxStyle(pHeaderItem->m_nIndexStyle);  
			LOGFONT	LogFont;
			XUtil::Css::GetFont(pStyle,LogFont);
			HFONT	hFont = ::CreateFontIndirect(&LogFont);
			hFont = (HFONT)SelectObject(hPaintDC,hFont);

			SIZE sz;
			GetTextExtentPoint32(hPaintDC,pHeaderItem->pStrText,(int)_tcslen(pHeaderItem->pStrText),&sz);
			width = sz.cx + 20;
			hFont = (HFONT)SelectObject(hPaintDC,hFont);
			::DeleteObject(hFont);
		}else
			width = pHeaderItem->m_nWidth;
		for(int i=0;i<pHeaderItem->m_HeaderItems.size();i++)
		{
			twidth += GetColWidth(hPaintDC,pHeaderItem->m_HeaderItems[i],pStyleSvr);
		}
		return width>twidth?width:twidth;
	}

	XReportDetailSvr::XReportDetailSvr():m_nStartRow(1),m_nCurRow(1),m_nCurCol(1)
	{
	}

	XReportDetailSvr::~XReportDetailSvr()
	{
		for(int i=0;i<m_DetailItems.size();i++)
		{
			if(m_DetailItems[i]) delete m_DetailItems[i];
		}
	}

	int XReportDetailSvr::GetRowAtPoint(POINT pt,int &y)
	{
		int nRowCount;
		nRowCount = this->m_pDataSvr->m_pContentSvr->GetRowCount();

		y = pt.y;
		for(int i=0;i<nRowCount;i++)
		{
			if(y <= m_nHeight) return i + 1;
			y -= m_nHeight + 1;
		}
		return 0;
	}
	
	int XReportDetailSvr::GetColAtPoint(POINT pt,int &x)
	{
		
		int nColCount;
		nColCount = m_DetailItems.size();

		x = pt.x;
		for(int i=0;i<nColCount;i++)
		{
			if(x <= m_DetailItems[i]->m_nWidth)
			{
				return i + 1;
			}
			x -= m_DetailItems[i]->m_nWidth + 1;
		}
		return 0;
	}

	int XReportDetailSvr::GetCellRect(int ACol,int ARow,RECT &rc,XReportSheetSvr* pReportSvr)
	{
		int scrollx(0),scrolly(0);

		int startX = 0;
		int startY = 0;
		int nRowCount;
		nRowCount = this->m_pDataSvr->m_pContentSvr->GetRowCount();
		for(int i=pReportSvr->FTopLeft.y;i<=nRowCount;i++)
		{

		}
		return 1;
	}

	void XReportDetailSvr::OnDraw(HDC hPaintDC,RECT &rect,XReportSheetSvr* pReportSvr,int startRow )
	{
		RECT	rc;
		rc = rect;
		int nSaveDC = ::SaveDC(hPaintDC);
		::IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(164,128,174));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		int nBkMode = ::SetBkMode(hPaintDC,TRANSPARENT);

		const XUtil::Css::TBoxStyle * pStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(this->m_nIndexStyle);
		if(pStyle)
			XUtil::Css::DrawCssFrame(hPaintDC,&rc,pStyle);

		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)this->m_pDataSvr->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int startX = rc.left;
			int startY = rc.top + 1;
			int i=0,nIndex=-1;
			int ARow=0;
			int ACol=0;
			i = startRow;

			while(true)
			{
				nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,i++);
				if(nIndex < 0) break;
				ARow = (*pTreeItems)[nIndex].nContentRow;
				ACol = (*pTreeItems)[nIndex].nContentCol;

				startX = rc.left;
				RECT	trect;
				for(int j=0;j<m_DetailItems.size();j++)
				{
					trect.left = startX;
					trect.top = startY;
					trect.bottom = trect.top + m_nHeight; 
					trect.right = trect.left + m_DetailItems[j]->m_nWidth;

					const XUtil::Css::TBoxStyle * pStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(m_DetailItems[j]->m_nIndexStyle);
					if(pReportSvr->IsRowSelected(i - 1))
					{
						HBRUSH hBrush = ::CreateSolidBrush(pReportSvr->m_SelectBkColor);
						::InflateRect(&trect,-1,-1);
						::FillRect(hPaintDC,&trect,hBrush);
						DeleteObject(hBrush);
					}else
					{
						if(pStyle)
							XUtil::Css::DrawCssFrame(hPaintDC,&trect,pStyle);

						if(i % 2 == 0)
						{
							HBRUSH hRowBrush = ::CreateSolidBrush(RGB(225,229,238));
							//RECT rc;
							//::SetRect(&rc,ARect.left ,ARect.top ,ARect.right ,ARect.bottom);
							::FillRect(hPaintDC,&trect,hRowBrush);
							::DeleteObject(hRowBrush);
						}
					}

					if(m_DetailItems[j]->colid > 0 || j==0)
					{
						int nIndent=0;
						int nIndentEx=0;
						int nLevelIndent = 16;
						if(j==0)
						{
							nIndent = ((*pTreeItems)[nIndex].nLevel - 1)*nLevelIndent;
							if((*pTreeItems)[nIndex].nImage>=0)
							{
								if((*pTreeItems)[nIndex].nImageList >=0 && (*pTreeItems)[nIndex].nImageList < pReportSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->m_pResourceMgr->m_hImageLists.size())
								{
									HIMAGELIST hImageList = pReportSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->m_pResourceMgr->m_hImageLists[(*pTreeItems)[nIndex].nImageList];
									int cx=16,cy=15;
									ImageList_GetIconSize(hImageList,&cx,&cy);
									::ImageList_Draw(hImageList,(*pTreeItems)[nIndex].nImage,hPaintDC,trect.left + nIndent,trect.top + (trect.bottom - trect.top)/2 - cy/2,ILD_NORMAL);
									nIndentEx = cx + 2;
								}
							}
							int py = trect.top + (trect.bottom - trect.top)/2;
							int px = trect.left + 7 + nIndent - nLevelIndent - 3;
							if((*pTreeItems)[nIndex].children)
							{
								HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(120,152,181));
								if((trect.bottom - trect.top) %2 ==1)py ++;

								//draw border
								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								::MoveToEx(hPaintDC,px,py - 4,NULL);
								::LineTo(hPaintDC,px + 8,py - 4);
								::LineTo(hPaintDC,px + 8,py + 4);
								::LineTo(hPaintDC,px,py + 4);
								::LineTo(hPaintDC,px,py - 4);
								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								::DeleteObject(hPen);

								hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								if((*pTreeItems)[nIndex].expanded)
								{
									//draw sub
									::MoveToEx(hPaintDC,px + 2,py,NULL);
									::LineTo(hPaintDC,px + 7,py);
								}else
								{
									//draw plus
									::MoveToEx(hPaintDC,px + 2,py,NULL);
									::LineTo(hPaintDC,px + 7,py);
									::MoveToEx(hPaintDC,px + 4,py - 2,NULL);
									::LineTo(hPaintDC,px + 4,py + 3);
								}
								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								::DeleteObject(hPen);

								LOGBRUSH   lb;   
								DWORD   nPen[2];   

								lb.lbStyle   =   BS_SOLID;   
								lb.lbColor   =   RGB(0,0,0);   
								nPen[0]   =   0;                             //这两个参数的设置与msdn中不一致，msdn中说的是一个点的宽度，   
								nPen[1]   =   2;                             //后面再接一个间隔的宽度，如果这两个值都设为1，不能满足要求   
								hPen = ExtCreatePen(PS_GEOMETRIC   |   PS_USERSTYLE,   1,   &lb,   2,   nPen); 

								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								//draw hline
								::MoveToEx(hPaintDC,px + 9,py,NULL);
								::LineTo(hPaintDC,px + 18,py);

								//draw top vline
								if(nIndex!=0)
								{
									::MoveToEx(hPaintDC,px + 4,trect.top,NULL);
									::LineTo(hPaintDC,px + 4,py - 5);
								}

								//draw bottom vline
								if(XXmlContentSvr::GetTreeNextItem(pTreeItems,nIndex)>=0)
								{
									::MoveToEx(hPaintDC,px + 4,py + 5,NULL);
									::LineTo(hPaintDC,px + 4,trect.bottom + 1);
								}

								int pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,nIndex);
								int nIndent1,px1;
								while(pIndex >= 0)
								{
									if(XXmlContentSvr::GetTreeNextItem(pTreeItems,pIndex)>=0)
									{
										nIndent1 = ((*pTreeItems)[pIndex].nLevel - 1)*nLevelIndent;
										px1 = trect.left + 7 + nIndent1 - nLevelIndent - 3;
										::MoveToEx(hPaintDC,px1 + 4,trect.top,NULL);
										::LineTo(hPaintDC,px1 + 4,trect.bottom + 1);
									}
									pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,pIndex);
								}

								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								::DeleteObject(hPen);
							}else
							{
								if((trect.bottom - trect.top) %2 ==1)py ++;


								LOGBRUSH   lb;   
								DWORD   nPen[2];   

								lb.lbStyle   =   BS_SOLID;   
								lb.lbColor   =   RGB(0,0,0);   
								nPen[0]   =   0;                             //这两个参数的设置与msdn中不一致，msdn中说的是一个点的宽度，   
								nPen[1]   =   2;                             //后面再接一个间隔的宽度，如果这两个值都设为1，不能满足要求   
								hPen = ExtCreatePen(PS_GEOMETRIC   |   PS_USERSTYLE,   1,   &lb,   2,   nPen); 

								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								int nNextItem = XXmlContentSvr::GetTreeNextItem(pTreeItems,nIndex);
								if(nNextItem < 0)
								{
									if(nIndex!=0)
									{
										::MoveToEx(hPaintDC,px + 4,trect.top,NULL); 
										::LineTo(hPaintDC,px + 4,py);
									}
								}else
								{
									if(nIndex!=0)
									{
										if(nIndex!=0)
											::MoveToEx(hPaintDC,px + 4,trect.top,NULL);
										else
											::MoveToEx(hPaintDC,px + 4,py,NULL);

										::LineTo(hPaintDC,px + 4,trect.bottom + 1);
									}
								}
								::MoveToEx(hPaintDC,px + 5,py,NULL);
								::LineTo(hPaintDC,px + 5 + 10,py);

								int pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,nIndex);
								int nIndent1,px1;
								while(pIndex >= 0)
								{
									if(XXmlContentSvr::GetTreeNextItem(pTreeItems,pIndex)>=0)
									{
										nIndent1 = ((*pTreeItems)[pIndex].nLevel - 1)*nLevelIndent;
										px1 = trect.left + 7 + nIndent1 - nLevelIndent - 3;
										::MoveToEx(hPaintDC,px1 + 4,trect.top,NULL);
										::LineTo(hPaintDC,px1 + 4,trect.bottom + 1);
									}
									pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,pIndex);
								}

								hPen = (HPEN)::SelectObject(hPaintDC,hPen);
								::DeleteObject(hPen);
							}
						}

						UINT fmode = DT_CENTER;
						if(pStyle && pStyle->mask & cssMaskText && pStyle->pText )
						{
							if(pStyle->pText->mask & cssMaskTextAlign)
							{
								switch(pStyle->pText->textalign)
								{
								case XUtil::Css::_left:
									fmode = DT_LEFT;
									break;
								case XUtil::Css::_right:
									fmode = DT_RIGHT;
									break;
								case XUtil::Css::_center:
									fmode = DT_CENTER;
									break;
								case XUtil::Css::_justify:
								case XUtil::Css::_inherit:
									fmode = DT_CENTER;
									break;
								default:
									fmode = DT_CENTER;
									break;
								}
							}
						}

						if(m_DetailItems[j]->m_nIndexEditor>=0)
						{
							LPTSTR pStr = (*m_pDataSvr->m_pEditor)[m_DetailItems[j]->m_nIndexEditor];
							LPTSTR pStart = NULL;
							TCHAR buf[255];
							int npos = 0;
							int ipos = 0;
							if(_tcsstr(pStr,_T("editor:checkbox")) && m_DetailItems[j]->colid > 0)
							{
								LPCTSTR pValue = m_pDataSvr->m_pContentSvr->GetItemString(m_DetailItems[j]->colid,ARow);
								pStart = _tcsstr(pStr,_T("editor-checkbox-checked:"));
								if(pStart)
								{
									pStart += _tcslen(_T("editor-checkbox-checked:"));
									while(pStart[npos] && (pStart[npos]==' ' || pStart[npos]=='\t' ||pStart[npos]=='\r' ||pStart[npos]=='\n'))npos++;
									while(pStart[npos] && pStart[npos]!=';')
									{
										if(ipos < 253)buf[ipos++] = pStart[npos++];
									}
									buf[ipos++]=0;
									HTHEME theme = ::OpenThemeData(GetDesktopWindow(),_T("BUTTON"));
									if(pValue && _tcsicmp(buf,pValue)==0)
									{
										DrawThemeBackground (theme, hPaintDC, BP_CHECKBOX, CBS_CHECKEDNORMAL, &trect, 0);
									}else
									{
										DrawThemeBackground (theme, hPaintDC, BP_CHECKBOX, CBS_UNCHECKEDNORMAL, &trect, 0);
									}
									::CloseThemeData(theme);
								}
								startX += m_DetailItems[j]->m_nWidth + 1;
								continue;
							}
						}

						trect.left += nIndent + nIndentEx;
						LPCTSTR pStr;
						if(j==0)
							pStr = m_pDataSvr->m_pContentSvr->GetItemString((*pTreeItems)[nIndex].nContentCol,ARow);
						else
							pStr = m_pDataSvr->m_pContentSvr->GetItemString(m_DetailItems[j]->colid,ARow);
						if(pStr)
						{
							LOGFONT	LogFont;
							XUtil::Css::GetFont(pStyle,LogFont);
							HFONT	hFont = ::CreateFontIndirect(&LogFont);
							hFont = (HFONT)SelectObject(hPaintDC,hFont);

							COLORREF color  = ::GetTextColor(hPaintDC);
							COLORREF pcolor  = color;
							if(i - 1 == m_nCurRow)::SetTextColor(hPaintDC,RGB(0,0,255));
							if(pReportSvr->IsRowSelected(i - 1))
							{
								::SetTextColor(hPaintDC,RGB(255,255,255));
							}
							trect.left += 5;
							if(j==0)
							{
								::DrawText(hPaintDC,pStr,
									(int)_tcslen(pStr),&trect,DT_WORDBREAK|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
							}else
								::DrawText(hPaintDC,pStr,
									(int)_tcslen(pStr),&trect,DT_WORDBREAK|DT_VCENTER|fmode|DT_SINGLELINE|DT_END_ELLIPSIS);
							::SetTextColor(hPaintDC,pcolor);

							hFont = (HFONT)SelectObject(hPaintDC,hFont);
							::DeleteObject(hFont);

							//delete pStr;
						}
					}
					startX += m_DetailItems[j]->m_nWidth + 1;
				}
				startY += m_nHeight + 1;
				if(startY > rc.bottom )break;
			}
		}else
		{
			int nRowCount;
			nRowCount = this->m_pDataSvr->m_pContentSvr->GetRowCount();

			int startX = rc.left;
			int startY = rc.top + 1;
			RECT	trect;
			for(int i=startRow;i<=nRowCount;i++)
			{
				startX = rc.left;
				for(int j=0;j<m_DetailItems.size();j++)
				{
					trect.left = startX;
					trect.top = startY;
					trect.bottom = trect.top + m_nHeight; 
					trect.right = trect.left + m_DetailItems[j]->m_nWidth;

					const XUtil::Css::TBoxStyle * pStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(m_DetailItems[j]->m_nIndexStyle);
					if(pStyle)
						XUtil::Css::DrawCssFrame(hPaintDC,&trect,pStyle);

					if(m_DetailItems[j]->m_nIndexEditor>=0)
					{
						LPTSTR pStr = (*m_pDataSvr->m_pEditor)[m_DetailItems[j]->m_nIndexEditor];
						LPTSTR pStart = NULL;
						TCHAR buf[255];
						int npos = 0;
						int ipos = 0;
						if(_tcsstr(pStr,_T("editor:checkbox")) && m_DetailItems[j]->colid > 0)
						{
							LPCTSTR pValue = m_pDataSvr->m_pContentSvr->GetItemString(m_DetailItems[j]->colid,i);
							pStart = _tcsstr(pStr,_T("editor-checkbox-checked:"));
							if(pStart)
							{
								pStart += _tcslen(_T("editor-checkbox-checked:"));
								while(pStart[npos] && (pStart[npos]==' ' || pStart[npos]=='\t' ||pStart[npos]=='\r' ||pStart[npos]=='\n'))npos++;
								while(pStart[npos] && pStart[npos]!=';')
								{
									if(ipos < 253)buf[ipos++] = pStart[npos++];
								}
								buf[ipos++]=0;
								HTHEME theme = ::OpenThemeData(GetDesktopWindow(),_T("BUTTON"));
								if(pValue && _tcsicmp(buf,pValue)==0)
								{
									DrawThemeBackground (theme, hPaintDC, BP_CHECKBOX, CBS_CHECKEDNORMAL, &trect, 0);
								}else
								{
									DrawThemeBackground (theme, hPaintDC, BP_CHECKBOX, CBS_UNCHECKEDNORMAL, &trect, 0);
								}
								::CloseThemeData(theme);
							}
							//if(pValue)delete pValue;
							startX += m_DetailItems[j]->m_nWidth + 1;
							continue;
						}
					}

					if(m_DetailItems[j]->colid > 0)
					{
						if(i==m_nCurRow && j + 1==m_nCurCol )
						{
							if(pReportSvr->FOptions & goAllowEdit)
							{
								RECT rt = trect;
								::InflateRect(&rt,-1,-1);
								HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,255));
								::FillRect(hPaintDC,&rt,hBrush);
								::DeleteObject(hBrush);
								::DrawFocusRect(hPaintDC,&rt);
							}
						}
						if(pReportSvr->m_SelectionMode>=1)
						{
							if(pReportSvr->IsRowSelected(i))
							{
								RECT rt = trect;
								rt.top += 1;
								//rt.bottom -= 1;
								HBRUSH hBrush = ::CreateSolidBrush(pReportSvr->m_SelectBkColor);
								::FillRect(hPaintDC,&rt,hBrush);
								::DeleteObject(hBrush);
								::DrawFocusRect(hPaintDC,&rt);
							}
						}

						int nIndent=0;
						LPCTSTR pStr = m_pDataSvr->m_pContentSvr->GetItemString(m_DetailItems[j]->colid,i);
						if(pStr)
						{
							if(m_DetailItems[j]->m_bHtml && pStr[0])
							{
								IXMLDOMElement * pEle = m_pDataSvr->m_pContentSvr->GetRowElement(i);
								BSTR bStr1;
								pEle->get_xml(&bStr1);
								::SysFreeString(bStr1);

								LPTSTR pName = m_pDataSvr->m_pContentSvr->GetColumnName(m_DetailItems[j]->colid);
								IXMLDOMNode * pNode = NULL;
								IXMLDOMNode * pNodeEx = NULL;
								pEle->selectSingleNode(pName,&pNode);
								if(pNode)
								{
									pNode->selectSingleNode(L"node()[1]",&pNodeEx);
									pNode->Release();
									
									if(pNodeEx)
									{
										BSTR bStr;
										pNodeEx->get_xml(&bStr);
										xbObject * pObject;
										xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,L"xshtml",pObject );
										IXSHtml* pHtml = (IXSHtml*)pObject->GetInterface();
										RECT rt = trect;
										::InflateRect(&rt,-2,-2);
										pHtml->DrawHtml(hPaintDC,(LPTSTR)bStr,rt);
										pObject->Release();
										pNodeEx->Release();
										::SysFreeString(bStr);
									}
								}
							}else
							{
								LOGFONT	LogFont;
								XUtil::Css::GetFont(pStyle,LogFont);
								HFONT	hFont = ::CreateFontIndirect(&LogFont);
								hFont = (HFONT)SelectObject(hPaintDC,hFont);

								UINT fmode = DT_CENTER;
								if(pStyle && pStyle->mask & cssMaskText && pStyle->pText )
								{
									if(pStyle->pText->mask & cssMaskTextAlign)
									{
										switch(pStyle->pText->textalign)
										{
										case XUtil::Css::_left:
											fmode = DT_LEFT;
											break;
										case XUtil::Css::_right:
											fmode = DT_RIGHT;
											break;
										case XUtil::Css::_center:
											fmode = DT_CENTER;
											break;
										case XUtil::Css::_justify:
										case XUtil::Css::_inherit:
											fmode = DT_CENTER;
											break;
										default:
											fmode = DT_CENTER;
											break;
										}
									}
								}
		
								//set color
								COLORREF color = RGB(0,0,0);
								if(pStyle && pStyle->mask & cssMaskColor && pStyle->pColor  )
								{
									if(pStyle->pColor->colorRef[0] == XUtil::Css::_rgbcolor)
										color = pStyle->pColor->color[0];
									else if(pStyle->pColor->colorRef[0] == XUtil::Css::_colorindex)
										color = pStyle->pColor->color[0];
								}
								if(pReportSvr->m_SelectionMode>=1)
								{
									if(pReportSvr->IsRowSelected(i))
									{
										color = RGB(255,255,255);
									}
								}else if(i == m_nCurRow)
								{
									color = RGB(0,0,255);
								}
								color = ::SetTextColor(hPaintDC,color);
								color  = ::GetTextColor(hPaintDC);
								RECT rx = trect;
								rx.left += 5;
								::DrawText(hPaintDC,pStr,
									(int)_tcslen(pStr),&rx,DT_WORDBREAK|DT_VCENTER|fmode|DT_SINGLELINE|DT_END_ELLIPSIS);
								::SetTextColor(hPaintDC,color);

								hFont = (HFONT)SelectObject(hPaintDC,hFont);
								::DeleteObject(hFont);
							}

							//delete pStr;
						}
					}
					startX += m_DetailItems[j]->m_nWidth + 1;
				}
				startY += m_nHeight + 1;
				if(startY > rc.bottom )break;
			}
		}

		::SetBkMode(hPaintDC,nBkMode);
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		::RestoreDC(hPaintDC,nSaveDC);
	}
}}}}
