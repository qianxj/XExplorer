#include "StdAfx.h"
#include "xuSpArray.hpp"
namespace Hxsoft{ namespace XUtil
{
	template<class T>
	xuSpArray<T>::xuSpArray(void):FSecDirSize(4096),TSecDir(NULL)
	{
		Create();
	}

	template<class T>
	xuSpArray<T>::~xuSpArray(void)
	{
		Clear();
	}

	const int SPAIndexMask[2] = {15, 255};
	const int SPASecShift[2] = {4, 8};

	template<class T>
	void xuSpArray<T>::Create(TSPAQuantum Quantum)
	{
		Clear();
		TSecDir = new TSlot[FSecDirSize];
		::ZeroMemory(TSecDir,sizeof(TSlot)*FSecDirSize);

		FHighBound = -1;
		FSectionSize = WORD(SPAIndexMask[Quantum]) + 1;
		indexMask = WORD(SPAIndexMask[Quantum]);
		secShift = WORD(SPASecShift[Quantum]);
	}

	template<class T>
	void xuSpArray<T>::Clear()
	{
		if(TSecDir)
		{
			for(int i=0;i<this->FSecDirSize -1;i++)
			{
				if(TSecDir[i])delete[] TSecDir[i];
			}
			delete[] TSecDir;
			TSecDir = NULL;
		}
	}

	template<class T>
	const T xuSpArray<T>::GetAt(int Index) const
	{
		int secIndex;

		secIndex = (Index >> secShift);
		if(secIndex >= this->FSecDirSize) return NULL;
		if(!TSecDir[secIndex])return NULL;
		return TSecDir[secIndex][Index & indexMask]; 
	}

	template<class T>
	T xuSpArray<T>::GetAt(int Index)
	{
		int secIndex;

		secIndex = (Index >> secShift);
		if(secIndex >= this->FSecDirSize) return NULL;
		if(!TSecDir[secIndex]) return NULL;
		return TSecDir[secIndex][Index & indexMask]; 
	}

	template<class T>
	T&  xuSpArray<T>::MakeAt(int Index)
	{
		WORD secIndex;
		T*	 pSlot;

		//get dir
		secIndex = Index >> secShift;  
		if(secIndex >= FSecDirSize)
		{
			TSlot * pTemp;
			pTemp = new TSlot[FSecDirSize + 64];
			::CopyMemory(pTemp,TSecDir,FSecDirSize * sizeof(TSlot));
			if(TSecDir)delete[] TSecDir;
			TSecDir = pTemp;
			FSecDirSize += 64;
		}

		if(!TSecDir[secIndex])
		{
			pSlot = (T *) new T[FSectionSize];
			::ZeroMemory(pSlot,sizeof(T)*FSectionSize);
			TSecDir[secIndex] = pSlot;
		}
		if(Index > FHighBound)FHighBound = Index;
		return TSecDir[secIndex][Index & indexMask];
	}

	template<class T>
	void xuSpArray<T>::PutAt(int Index, T Item)
	{
		this->MakeAt(Index)  = Item;
		if(!Item && FHighBound == Index)ResetHighBound();
	}

	template<class T>
	void xuSpArray<T>::ResetHighBound()
	{
		int i;
		for(i=FHighBound;i>=0;i--)
		{
			if(this->GetAt(i))break;
		}
		FHighBound = i;
	}

	template<class T>
	xuSpList<T>::xuSpList()
	{
	}
	template<class T>
	xuSpList<T>::~xuSpList()
	{
	}

	template<class T>
	void xuSpList<T>::NewList(TSPAQuantum Quantum)
	{
		FQuantum = Quantum;
		FList.Create(Quantum);
	}

	template<class T>
	void xuSpList<T>::Error() const
	{
		throw _T("InvalidArgException");
	}

	template<class T>
	void xuSpList<T>::Create(TSPAQuantum Quantum)
	{
		FList.Clear();
		NewList(FQuantum);
	}

	template<class T>
	const T   xuSpList<T>::Get(int Index) const
	{
		if(Index < 0) Error();
		return FList[Index];
	}

	template<class T>
	T   xuSpList<T>::Get(int Index)
	{
		if(Index < 0) Error();
		return FList[Index];
	}

	template<class T>
	void xuSpList<T>::Put(int Index, T Item)
	{
		if(Index < 0) Error();
		FList.PutAt(Index,Item);
	}


	template<class T>
	void xuSpList<T>::Delete(int Index)
	{
		int cnt = this->GetHighBound();
		if((Index < 0) || (Index > cnt ))return;
		for(int i = Index; i<= cnt - 1;i++)
			FList.PutAt(i,FList[i + 1]);
		FList.PutAt(cnt,NULL); //slow
	}

	template<class T>
	void xuSpList<T>::Exchange(int Index1, int Index2)
	{
		T temp;

		temp = Get(Index1);
		Put(Index1, Get(Index2));
		Put(Index2, temp);
	}

	template<class T>
	void xuSpList<T>::Insert(int Index, T Item)
	{
		if(Index < 0) Error();
		int i = this->GetHighBound();
		while(i >= Index)
		{
			FList.PutAt(i + 1,FList[i]);
			i--;
		}
		FList.PutAt(Index,Item);
	}

	template<class T>
	void xuSpList<T>::Move(int CurIndex, int NewIndex)
	{
		T Item;

		if(CurIndex != NewIndex)
		{
			Item = Get(CurIndex);
			Delete(CurIndex);
			Insert(NewIndex, Item);
		}
	}

	template class xuSpArray<void *>;
	template class xuSpList<void *>;
	template class xuSpList<xuSpList<void *>*>;
}}
