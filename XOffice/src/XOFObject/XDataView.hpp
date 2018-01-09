#pragma once

#include <vector>
#include <string>

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	typedef struct tagSortItem
	{
		std::vector<int> * pPos;
		std::vector<std::vector<LPCTSTR>*>* pVals;
		std::vector<bool> * pOrders;
		std::vector<std::wstring>* pColTypes;
	} FSortItem;

	bool SortDoubleLess(FSortItem & item,int pos1,int pos2);
	bool SortIntLess(FSortItem & item,int pos1,int pos2);
	bool SortLess(FSortItem & item,int pos1,int pos2);
	bool SortExch(FSortItem & item,int pos1,int pos2);
	int Partition(FSortItem & item, int nBeging, int nEnd);
	int QuickSortRecursion(FSortItem & item, int nBeging, int nEnd);
	int QuickSort(FSortItem & item, int nLen);


}}}