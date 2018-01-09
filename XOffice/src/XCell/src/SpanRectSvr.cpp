#include "stdafx.h"
#include "xofbase.h"
#include "spanrectsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CSpanRectSvr::CSpanRectSvr()
	{
	}
	CSpanRectSvr::~CSpanRectSvr()
	{
		SpanRects.clear();
	}

	bool CSpanRectSvr::InsertRow(int ARow,int Number,bool bCopyMode)
	{
		int cnt = SpanRects.size();
		for(int i=0;i< cnt;i++)
		{
			if(SpanRects[i].bottom == ARow - 1 &&  SpanRects[i].top== ARow - 1 )
			{
				RECT	rect;
				if(bCopyMode)
				{
					for(int k=1;k<=Number;k++)
					{
						rect = SpanRects[i];
						rect.top = rect.bottom = ARow + k - 1 ;
						SpanRects.push_back(rect);
					}
				}
			}
			else if(SpanRects[i].top < ARow && SpanRects[i].bottom >= ARow)
			{
				SpanRects[i].bottom += Number;
			}
			else if(SpanRects[i].top >= ARow)
			{
				SpanRects[i].top += Number; SpanRects[i].bottom+=Number;
			}
		}		
		return true;
	}

	bool CSpanRectSvr::InsertCol(int ACol,int Number,bool bCopyMode)
	{
		int cnt = SpanRects.size();
		for(int i=0;i< cnt;i++)
		{
			if(SpanRects[i].right == ACol - 1 &&  SpanRects[i].left== ACol - 1 )
			{
				RECT	rect;
				if(bCopyMode)
				{
					for(int k=1;k<=Number;k++)
					{
						rect = SpanRects[i];
						rect.left = rect.right = ACol + k - 1 ;
						SpanRects.push_back(rect);
					}
				}
			}
			else if(SpanRects[i].left < ACol && SpanRects[i].right >= ACol)
			{
				SpanRects[i].right += Number;
			}
			else if(SpanRects[i].top >= ACol)
			{
				SpanRects[i].left += Number; SpanRects[i].right+=Number;
			}
		}		
		return true;
	}

	bool CSpanRectSvr::InsertRow(int ARow)
	{
		for(int i=0;i<SpanRects.size();i++)
		{
			if(SpanRects[i].bottom <ARow)
				continue;
			else if(SpanRects[i].top >= ARow)
			{
				SpanRects[i].top++;
				SpanRects[i].bottom++;
			}
			else
				SpanRects[i].bottom++;
		}
		return true;
	}

	bool CSpanRectSvr::MoveRow(int FromIndex,int ToIndex)
	{
		int cnt = SpanRects.size();
		RECT rect;
		for(int i=0;i<cnt;i++)
		{
			rect = SpanRects[i];
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


	bool CSpanRectSvr::DeleteRow(int ARow)
	{
		for(int i=0;i<SpanRects.size();i++)
		{
			if(SpanRects[i].bottom <ARow)
				continue;
			else if(SpanRects[i].top == SpanRects[i].bottom && SpanRects[i].top ==ARow)
			{
				::SetRect(&SpanRects[i],0,0,0,0);
			}
			else if(SpanRects[i].top == ARow)
			{
				SpanRects[i].bottom--;
			}
			else if(SpanRects[i].top > ARow)
			{
				SpanRects[i].top--;
				SpanRects[i].bottom--;
			}
			else
				SpanRects[i].bottom--;
		}
		return true;
	}

	bool CSpanRectSvr::InsertCol(int ACol)
	{
		for(int i=0;i<SpanRects.size();i++)
		{
			if(SpanRects[i].right <ACol)
				continue;
			else if(SpanRects[i].left >= ACol)
			{
				SpanRects[i].left++;
				SpanRects[i].right++;
			}
			else
				SpanRects[i].right++;
		}
		return true;
	}

	bool CSpanRectSvr::DeleteCol(int ACol)
	{
		for(int i=0;i<SpanRects.size();i++)
		{
			if(SpanRects[i].right <ACol)
				continue;
			else if(SpanRects[i].left == SpanRects[i].right && SpanRects[i].left ==ACol)
			{
				::SetRect(&SpanRects[i],0,0,0,0);
			}
			else if(SpanRects[i].left == ACol)
			{
				SpanRects[i].right--;
			}
			else if(SpanRects[i].left > ACol)
			{
				SpanRects[i].left--;
				SpanRects[i].right--;
			}
			else
				SpanRects[i].right--;
		}
		return true;
	}
	bool CSpanRectSvr::MoveCol(int FromIndex,int ToIndex)
	{
		int cnt = this->SpanRects.size();
		RECT rect;
		for(int i=0;i<cnt;i++)
		{
			rect = this->SpanRects[i];
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


	bool CSpanRectSvr::Merge(TGridRect ARect)
	{
		RECT rect = *(RECT *)&GetFullRect(ARect);

		//merge
		RECT arect,interRect;
		InflateRect(&rect,0,0,1,1);
		for(int i = 0;i<SpanRects.size();i++)
		{
			arect = SpanRects[i];
			InflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				SpanRects[i] = arect;
			}
		}
		InflateRect(&rect,0,0,-1,-1);
		SpanRects.push_back(rect);
		return true;
	}

	bool CSpanRectSvr::UnMerge(TGridRect ARect)
	{
		RECT rect = *(RECT *)&GetFullRect(ARect);

		//merge
		RECT arect,interRect;
		InflateRect(&rect,0,0,1,1);
		for(int i = 0;i<SpanRects.size();i++)
		{
			arect = SpanRects[i];
			InflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				SpanRects[i] = arect;
			}
		}
		return true;
	}

	TGridRect CSpanRectSvr::GetFullRect(TGridRect ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect;
		NormalizeRect(rect);
		InflateRect(&rect,0,0,1,1);
		for(int i=0;i<SpanRects.size();i++)
		{
			arect = SpanRects[i];
			if(arect.left == arect.right && arect.left==0)continue;
			InflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				UnionRect(&rect,&rect,&arect);
			}
		}
		InflateRect(&rect,0,0,-1,-1);
		return *(TGridRect *)&rect;
	}
	bool CSpanRectSvr::RMerge(TGridRect ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect,Rrect;
		NormalizeRect(rect);
		InflateRect(&rect,0,0,1,1);

		int i;
		for(i = 0;i<SpanRects.size();i++)
		{
			arect = SpanRects[i];
			InflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				SpanRects[i] = arect;
			}
		}
		InflateRect(&rect,0,0,-1,-1);
		for(i=rect.top;i<=rect.bottom;i++)
		{
			SetRect(&Rrect,rect.left,i,rect.right,i);
			SpanRects.push_back(Rrect);
		}
		return true;
	}
	bool CSpanRectSvr::CMerge(TGridRect ARect)
	{
		RECT	rect = *(RECT *)&ARect;
		RECT arect,interRect,Rrect;
		NormalizeRect(rect);
		InflateRect(&rect,0,0,1,1);

		int i;
		for(i = 0;i<SpanRects.size();i++)
		{
			arect = SpanRects[i];
			InflateRect(&arect,0,0,1,1);
			if(IntersectRect(&interRect,&rect,&arect))
			{
				SetRect(&arect,0,0,0,0);
				SpanRects[i] = arect;
			}
		}
		InflateRect(&rect,0,0,-1,-1);
		for(i=rect.left;i<=rect.right;i++)
		{
			SetRect(&Rrect,i,rect.top,i,rect.bottom);
			SpanRects.push_back(Rrect);
		}
		return true;
	}

	bool CSpanRectSvr::GetSpanedRowCol(int ACol,int ARow,int &SCol,int &SRow)
	{
		TGridRect	rect;
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

	bool CSpanRectSvr::isSpaned(int ACol,int ARow)
	{
		TGridRect	rect;
		return isSpaned(ACol,ARow,rect);
	}

	bool CSpanRectSvr::isSpaned(int ACol, int ARow, TGridRect &ARect)
	{
		int index;
		return isSpaned(ACol,ARow,ARect,index);
	}

	bool CSpanRectSvr::isSpaned(int ACol, int ARow, TGridRect &ARect,int &index)
	{
		RECT	rect;
		ARect.left = ARect.right = ACol;
		ARect.top = ARect.bottom = ARow;
		index  = -1;
		for(int i=0;i<SpanRects.size();i++)
		{
			rect = SpanRects[i];
			if(rect.left == rect.right && rect.left==0)continue;

			if((ACol>=rect.left) && (ACol<=rect.right) && (ARow>=rect.top) && (ARow<=rect.bottom))
			{
				ARect = *(TGridRect *)&SpanRects[i];
				index = i;
				return true;
			}
		}
		return false;
	}

	void CSpanRectSvr::RefreshSpans()
	{
		int index = -1;
		RECT rect;
		for(int i = 0;i<SpanRects.size();i++)
		{
			rect = SpanRects[i];
			if(rect.bottom == 0 || rect.top ==0)continue;
			if(rect.right == 0 || rect.left ==0)continue;
			if(rect.bottom > rect.top || rect.right > rect.left)
			{
				index ++ ;
				SpanRects[index].top = rect.top;
				SpanRects[index].bottom = rect.bottom;
				SpanRects[index].left = rect.left;
				SpanRects[index].right = rect.right;
			}
		}
		SpanRects.setsize(index + 1);
	}
}}}}
