#include "stdafx.h"
#include "XDataView.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	bool SortDoubleLess(FSortItem & item,int pos1,int pos2)
	{
		int nSize = (int)item.pVals->size();
		int nret;
		for(int i=0;i<nSize;i++)
		{
			if((*(*item.pVals)[i])[pos1] && (*(*item.pVals)[i])[pos2])
			{
				nret = wcstod((*(*item.pVals)[i])[pos1],0) > wcstod((*(*item.pVals)[i])[pos2],0) ? 1 : -1;
				if(wcstod((*(*item.pVals)[i])[pos1],0) == wcstod((*(*item.pVals)[i])[pos2],0)) nret = 0;
			}
			else if(!(*(*item.pVals)[i])[pos1] && !(*(*item.pVals)[i])[pos2])
				nret = 0;
			else if(!(*(*item.pVals)[i])[pos1])
				nret = -1;
			else if(!(*(*item.pVals)[i])[pos2])
				nret = 1;
			else 
				nret = -1;
			if((*item.pOrders)[i])
			{
				if(nret > 0) return false;
				if(nret < 0) return true;
			}else
			{
				if(nret < 0) return false;
				if(nret > 0) return true;
			}
		}
		return false;
	}

	bool SortIntLess(FSortItem & item,int pos1,int pos2)
	{
		int nSize = (int)item.pVals->size();
		int nret;
		for(int i=0;i<nSize;i++)
		{
			if((*(*item.pVals)[i])[pos1] && (*(*item.pVals)[i])[pos2])
			{
				nret = wcstol((*(*item.pVals)[i])[pos1],0,10) > wcstol((*(*item.pVals)[i])[pos2],0,10) ? 1 : -1;
				if(wcstod((*(*item.pVals)[i])[pos1],0) == wcstod((*(*item.pVals)[i])[pos2],0)) nret = 0;
			}
			else if(!(*(*item.pVals)[i])[pos1] && !(*(*item.pVals)[i])[pos2])
				nret = 0;
			else if(!(*(*item.pVals)[i])[pos1])
				nret = -1;
			else if(!(*(*item.pVals)[i])[pos2])
				nret = 1;
			else 
				nret = -1;
			if((*item.pOrders)[i])
			{
				if(nret > 0) return false;
				if(nret < 0) return true;
			}else
			{
				if(nret < 0) return false;
				if(nret > 0) return true;
			}
		}
		return false;
	}

	bool SortLess(FSortItem & item,int pos1,int pos2)
	{
		int nSize = (int)item.pVals->size();
		int nret;
		for(int i=0;i<nSize;i++)
		{
			if((*(*item.pVals)[i])[pos1] && (*(*item.pVals)[i])[pos2])
			{
				if((*item.pColTypes)[i]==L"xs:decimal")
				{
					nret = wcstod((*(*item.pVals)[i])[pos1],0) > wcstod((*(*item.pVals)[i])[pos2],0) ? 1 : -1;
					if(wcstod((*(*item.pVals)[i])[pos1],0) == wcstod((*(*item.pVals)[i])[pos2],0)) nret = 0;
				}else if((*item.pColTypes)[i]==L"xs:integer")
				{
					nret = wcstol((*(*item.pVals)[i])[pos1],0,10) > wcstol((*(*item.pVals)[i])[pos2],0,10) ? 1 : -1;
					if(wcstod((*(*item.pVals)[i])[pos1],0) == wcstod((*(*item.pVals)[i])[pos2],0)) nret = 0;
				}
				else nret = _tcsicoll((*(*item.pVals)[i])[pos1],(*(*item.pVals)[i])[pos2]);
			}
			else if(!(*(*item.pVals)[i])[pos1] && !(*(*item.pVals)[i])[pos2])
				nret = 0;
			else if(!(*(*item.pVals)[i])[pos1])
				nret = -1;
			else if(!(*(*item.pVals)[i])[pos2])
				nret = 1;
			else 
				nret = -1;
			if((*item.pOrders)[i])
			{
				if(nret > 0) return false;
				if(nret < 0) return true;
			}else
			{
				if(nret < 0) return false;
				if(nret > 0) return true;
			}
		}
		return false;
	}

	bool SortExch(FSortItem & item,int pos1,int pos2)
	{
		int nSize = (int)item.pVals->size();
		LPCTSTR pStr ;
		for(int i=0;i<nSize;i++)
		{
			pStr = (*(*item.pVals)[i])[pos1];
			(*(*item.pVals)[i])[pos1] = (*(*item.pVals)[i])[pos2];
			(*(*item.pVals)[i])[pos2] = pStr;
		}
		int pos ;
		pos = (*item.pPos)[pos1];
		(*item.pPos)[pos1] = (*item.pPos)[pos2];
		(*item.pPos)[pos2] = pos;

		return true;
	}

	int Partition(FSortItem & item, int nBeging, int nEnd)
	{
		int i = nBeging + rand()%(nBeging - nEnd);
		SortExch(item,i,nBeging);
		int x = nBeging;
		--nEnd;

		while (nBeging < nEnd)
		{
			//�Ӻ���ǰ���ҵ���XС��Ԫ��λ��
			while(SortLess(item, x,nEnd))
			{
				--nEnd;
				if(x > nEnd) break;
			}
			//��xС��Ԫ��λ����ǰ��nBegin���պ��ܱ����xС��Ԫ��
			if (nBeging < nEnd)
			{
				SortExch(item,nBeging,nEnd);
				x = nEnd;
				++nBeging;
			}

			//С���������ӣ��ҵ�һ������xС��Ԫ�ء�
			while (SortLess(item,nBeging,x))
			{
				++nBeging;
				if(nBeging > x) break;
			}

			//�Ѳ���xС��Ԫ�ش���ڴ�������ڡ�nEnd�պ�Ԥ���˴�λ�á�
			if (nBeging < nEnd)
			{
				SortExch(item,nBeging,nEnd);
				x = nBeging;
				--nEnd;
			}
		}
		return nBeging;  
	}

	int QuickSortRecursion(FSortItem & item, int nBeging, int nEnd)
	{
		if (nBeging >= nEnd -1)       
		{
			return 1;
		}

		int i = Partition(item, nBeging, nEnd);      

		QuickSortRecursion(item, nBeging, i);       
		QuickSortRecursion(item, i + 1, nEnd);            
		return 1;
	}

	int QuickSort(FSortItem & item, int nLen)
	{
		QuickSortRecursion(item,  0, nLen);
		return 1;
	}

}}}
