#include "HtTableSpan.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	BOOL HtPointInGridRect(int Col, int Row,  RECT Rect)
	{
		return  (Col >= Rect.left) && (Col <= Rect.right) && (Row >= Rect.top)
			&& (Row <= Rect.bottom);
	};

	BOOL HtPtInRect(int X, int Y ,RECT Rect)
	{
		return  (X >= Rect.left) && (X <= Rect.right) && (Y >= Rect.top) &&
			(Y <= Rect.bottom);
	};

	BOOL HtInflateRect(LPRECT pRect,int dx0,int dy0,int dx1,int dy1)
	{
		pRect->left -= dx0;
		pRect->top  -= dy0;
		pRect->right  += dx1;
		pRect->bottom += dy1;
		return TRUE;
	}

	RECT HtNormalizeRect(RECT &rect)
	{
		int nTemp;
		if (rect.left > rect.right)
		{
			nTemp = rect.left;
			rect.left = rect.right;
			rect.right = nTemp;
		}
		if (rect.top > rect.bottom)
		{
			nTemp = rect.top;
			rect.top = rect.bottom;
			rect.bottom = nTemp;
		}
		return rect;
	};

	CHtTableSpan::CHtTableSpan(void)
	{
		m_pSpanRects = new std::vector<RECT>;
	}

	CHtTableSpan::~CHtTableSpan(void)
	{
		if(m_pSpanRects)delete m_pSpanRects;
	}

	bool CHtTableSpan::InsertRow(int ARow,int Number,bool bCopyMode)
	{
		int cnt = m_pSpanRects->size();
		for(int i=0;i< cnt;i++)
		{
			if((*m_pSpanRects)[i].bottom == ARow - 1 &&  (*m_pSpanRects)[i].top== ARow - 1 )
			{
				RECT	rect;
				if(bCopyMode)
				{
					for(int k=1;k<=Number;k++)
					{
						rect = (*m_pSpanRects)[i];
						rect.top = rect.bottom = ARow + k - 1 ;
						m_pSpanRects->push_back(rect);
					}
				}
			}
			else if((*m_pSpanRects)[i].top < ARow && (*m_pSpanRects)[i].bottom >= ARow)
			{
				(*m_pSpanRects)[i].bottom += Number;
			}
			else if((*m_pSpanRects)[i].top >= ARow)
			{
				(*m_pSpanRects)[i].top += Number; (*m_pSpanRects)[i].bottom+=Number;
			}
		}		
		return true;
	}

	bool CHtTableSpan::InsertCol(int ACol,int Number,bool bCopyMode)
	{
		int cnt = m_pSpanRects->size();
		for(int i=0;i< cnt;i++)
		{
			if((*m_pSpanRects)[i].right == ACol - 1 &&  (*m_pSpanRects)[i].left== ACol - 1 )
			{
				RECT	rect;
				if(bCopyMode)
				{
					for(int k=1;k<=Number;k++)
					{
						rect = (*m_pSpanRects)[i];
						rect.left = rect.right = ACol + k - 1 ;
						m_pSpanRects->push_back(rect);
					}
				}
			}
			else if((*m_pSpanRects)[i].left < ACol && (*m_pSpanRects)[i].right >= ACol)
			{
				(*m_pSpanRects)[i].right += Number;
			}
			else if((*m_pSpanRects)[i].top >= ACol)
			{
				(*m_pSpanRects)[i].left += Number; (*m_pSpanRects)[i].right+=Number;
			}
		}		
		return true;
	}

	bool CHtTableSpan::InsertRow(int ARow)
	{
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			if((*m_pSpanRects)[i].bottom <ARow)
				continue;
			else if((*m_pSpanRects)[i].top >= ARow)
			{
				(*m_pSpanRects)[i].top++;
				(*m_pSpanRects)[i].bottom++;
			}
			else
				(*m_pSpanRects)[i].bottom++;
		}
		return true;
	}

	bool CHtTableSpan::MoveRow(int FromIndex,int ToIndex)
	{
		int cnt = m_pSpanRects->size();
		RECT rect;
		for(int i=0;i<cnt;i++)
		{
			rect = (*m_pSpanRects)[i];
			if(ToIndex > FromIndex)
			{
				if(rect.top > FromIndex && rect.bottom <= ToIndex)
				{
					rect.top--;
					rect.bottom--;
					continue;
				}
			}else
			{
				if(rect.top >= ToIndex && rect.bottom < FromIndex)
				{
					rect.top++;
					rect.bottom++;
					continue;
				}
			}
			if(rect.top == FromIndex &&  rect.bottom == FromIndex)
			{
				rect.top = rect.bottom = ToIndex;
			}
		}
		return true;
	}


	bool CHtTableSpan::DeleteRow(int ARow)
	{
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			if((*m_pSpanRects)[i].bottom <ARow)
				continue;
			else if((*m_pSpanRects)[i].top == (*m_pSpanRects)[i].bottom && (*m_pSpanRects)[i].top ==ARow)
			{
				::SetRect(&(*m_pSpanRects)[i],0,0,0,0);
			}
			else if((*m_pSpanRects)[i].top == ARow)
			{
				(*m_pSpanRects)[i].bottom--;
			}
			else if((*m_pSpanRects)[i].top > ARow)
			{
				(*m_pSpanRects)[i].top--;
				(*m_pSpanRects)[i].bottom--;
			}
			else
				(*m_pSpanRects)[i].bottom--;
		}
		return true;
	}

	bool CHtTableSpan::InsertCol(int ACol)
	{
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			if((*m_pSpanRects)[i].right <ACol)
				continue;
			else if((*m_pSpanRects)[i].left >= ACol)
			{
				(*m_pSpanRects)[i].left++;
				(*m_pSpanRects)[i].right++;
			}
			else
				(*m_pSpanRects)[i].left++;
		}
		return true;
	}

	bool CHtTableSpan::DeleteCol(int ACol)
	{
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			if((*m_pSpanRects)[i].right <ACol)
				continue;
			else if((*m_pSpanRects)[i].left == (*m_pSpanRects)[i].right && (*m_pSpanRects)[i].left ==ACol)
			{
				::SetRect(&(*m_pSpanRects)[i],0,0,0,0);
			}
			else if((*m_pSpanRects)[i].left == ACol)
			{
				(*m_pSpanRects)[i].right--;
			}
			else if((*m_pSpanRects)[i].left > ACol)
			{
				(*m_pSpanRects)[i].left--;
				(*m_pSpanRects)[i].right--;
			}
			else
				(*m_pSpanRects)[i].right--;
		}
		return true;
	}
	bool CHtTableSpan::MoveCol(int FromIndex,int ToIndex)
	{
		int cnt = this->m_pSpanRects->size();
		RECT rect;
		for(int i=0;i<cnt;i++)
		{
			rect = (*m_pSpanRects)[i];
			if(ToIndex > FromIndex)
			{
				if(rect.left > FromIndex && rect.right <= ToIndex)
				{
					rect.left--;
					rect.right--;
					continue;
				}
			}else
			{
				if(rect.left >= ToIndex && rect.right < FromIndex)
				{
					rect.left++;
					rect.right++;
					continue;
				}
			}
			if(rect.left == FromIndex &&  rect.right == FromIndex)
			{
				rect.left = rect.right = ToIndex;
			}
		}
		return true;
	}


	bool CHtTableSpan::Merge(RECT ARect)
	{
		RECT rect = *(RECT *)&GetFullRect(ARect);

		//merge
		RECT arect,interRect;
		HtInflateRect(&rect,0,0,1,1);
		for(int i = 0;i<m_pSpanRects->size();i++)
		{
			arect = (*m_pSpanRects)[i];
			HtInflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				(*m_pSpanRects)[i] = arect;
			}
		}
		HtInflateRect(&rect,0,0,-1,-1);
		m_pSpanRects->push_back(rect);
		return true;
	}

	bool CHtTableSpan::UnMerge(RECT ARect)
	{
		RECT rect = *(RECT *)&GetFullRect(ARect);

		//merge
		RECT arect,interRect;
		HtInflateRect(&rect,0,0,1,1);
		for(int i = 0;i<m_pSpanRects->size();i++)
		{
			arect = (*m_pSpanRects)[i];
			HtInflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				(*m_pSpanRects)[i] = arect;
			}
		}
		return true;
	}

	RECT CHtTableSpan::GetFullRect(RECT ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect;
		HtNormalizeRect(rect);
		HtInflateRect(&rect,0,0,1,1);
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			arect = (*m_pSpanRects)[i];
			if(arect.left == arect.right && arect.left==0)continue;
			HtInflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				UnionRect(&rect,&rect,&arect);
			}
		}
		HtInflateRect(&rect,0,0,-1,-1);
		return *(RECT *)&rect;
	}
	bool CHtTableSpan::RMerge(RECT ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect,Rrect;
		HtNormalizeRect(rect);
		HtInflateRect(&rect,0,0,1,1);

		int i;
		for(i = 0;i<m_pSpanRects->size();i++)
		{
			arect = (*m_pSpanRects)[i];
			HtInflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				(*m_pSpanRects)[i] = arect;
			}
		}
		HtInflateRect(&rect,0,0,-1,-1);
		for(i=rect.top;i<=rect.bottom;i++)
		{
			SetRect(&Rrect,rect.left,i,rect.right,i);
			m_pSpanRects->push_back(Rrect);
		}
		return true;
	}
	bool CHtTableSpan::CMerge(RECT ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect,Rrect;
		HtNormalizeRect(rect);
		HtInflateRect(&rect,0,0,1,1);

		int i;
		for(i = 0;i<m_pSpanRects->size();i++)
		{
			arect = (*m_pSpanRects)[i];
			HtInflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				(*m_pSpanRects)[i] = arect;
			}
		}
		HtInflateRect(&rect,0,0,-1,-1);
		for(i=rect.left;i<=rect.right;i++)
		{
			SetRect(&Rrect,rect.top,i,rect.bottom,i);
			m_pSpanRects->push_back(Rrect);
		}
		return true;
	}

	bool CHtTableSpan::GetSpanedRowCol(int ACol,int ARow,int &SCol,int &SRow)
	{
		RECT	rect;
		SCol = ACol;
		SRow = ARow;
		if(isSpaned(ACol,ARow,rect))
		{
			SCol = rect.left;
			SRow = rect.top;
			return true;
		}
		return false;
	}

	bool CHtTableSpan::isSpaned(int ACol,int ARow)
	{
		RECT	rect;
		return isSpaned(ACol,ARow,rect);
	}

	bool CHtTableSpan::isSpaned(int ACol, int ARow, RECT &ARect)
	{
		int index;
		return isSpaned(ACol,ARow,ARect,index);
	}

	bool CHtTableSpan::isSpaned(int ACol, int ARow, RECT &ARect,int &index)
	{
		RECT	rect;
		ARect.left = ARect.right = ACol;
		ARect.top = ARect.bottom = ARow;
		index  = -1;
		for(int i=0;i<m_pSpanRects->size();i++)
		{
			rect = (*m_pSpanRects)[i];
			if(rect.left == rect.right && rect.left==0)continue;

			if((ACol>=rect.left) && (ACol<=rect.right) && (ARow>=rect.top) && (ARow<=rect.bottom))
			{
				ARect = *(RECT *)&(*m_pSpanRects)[i];
				index = i;
				return true;
			}
		}
		return false;
	}

	void CHtTableSpan::RefreshSpans()
	{
		int index = -1;
		RECT rect;
		for(int i = 0;i<m_pSpanRects->size();i++)
		{
			rect = (*m_pSpanRects)[i];
			if(rect.bottom == 0 || rect.top ==0)continue;
			if(rect.right == 0 || rect.left ==0)continue;
			if(rect.bottom > rect.top || rect.right > rect.left)
			{
				index ++ ;
				(*m_pSpanRects)[index].top = rect.top;
				(*m_pSpanRects)[index].bottom = rect.bottom;
				(*m_pSpanRects)[index].left = rect.left;
				(*m_pSpanRects)[index].right = rect.right;
			}
		}
		m_pSpanRects->resize(index + 1);
	}
}}}
