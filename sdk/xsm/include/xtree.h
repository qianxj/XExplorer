#pragma once
#include "xvmdef.h"
#include "xalloc.h"
#include "xarrays.h"

namespace xcode
{
	template<class T>
	class xtree 
	{
	public:
		xtree(void);
		~xtree(void);
	public:
		struct treeitem
		{
			T data;
			int _piror;
			int _next;
			int _child;
			int _parent;
		};
		template class xarrays<treeitem*,treeitem>; 
		template class xarrays<int*,int>; 
	public:
		int m_nIndex;
	private:
		xarrays<int*,int> * m_indexs;
		xarrays<treeitem*,treeitem> * m_pitems;
	public:
		int newTree();
	public:
		int addItem(int nTree,int nItemParent,T &item);
		int insertItem(int nTree,int nItemParent,int nItemPiror,T &item);
		int removeItem(int nTree,int nItem);
	public:
		T & getItem(int nTree,int nItem) ;
		void setItem(int nTree,int nItem,T& data) ;
		T & getItem(int nItem) ;
		void setItem(int nItem,T& data) ;
	public:
		int getNext(int nTree,int nIndex);
		int getParent(int nTree,int nIndex);
		int getPiror(int nTree,int nIndex);
		int getChild(int nTree,int nIndex);
	public:
		int getRoot(int nTree);
	};

	template<class T>
	inline xtree<T>::xtree(void):m_nIndex(0)
	{
		m_indexs = new xarrays<int*,int>;
		m_pitems = new xarrays<treeitem*,treeitem>;
		m_indexs->create(16,16);
		m_pitems->create(32,256);
	}

	template<class T>
	inline xtree<T>::~xtree(void)
	{
		if(m_indexs)delete m_indexs;
		if(m_pitems)delete m_pitems;
	}

	template<class T>
	inline int xtree<T>::newTree()
	{
		return ++m_nIndex;
	}

	template<class T>
	inline int xtree<T>::addItem(int nTree,int nItemParent,T &item)
	{
		assert(nTree >=0);

		treeitem ti;
		ti.data = item;
		ti._parent = nItemParent;
		ti._piror = -1;
		ti._next = -1;
		ti._child = -1;
		int nIndex = m_pitems->addItem(ti);

		int nChild = -1;
		if(nItemParent < 0) 
		{
			if((*m_indexs)[nTree]<=0)
			{
				(*m_indexs)[nTree] = nIndex + 1; //add to indexs from 1
				return nIndex;
			}else
			{
				nChild = (*m_indexs)[nTree] - 1;
			}
		}else
			nChild = (*m_pitems)[nItemParent ]._child;
		
		while(nChild >=0)
		{
			if((*m_pitems)[nChild ]._next>=0)
				nChild = (*m_pitems)[nChild]._next;
			else
				break;
		}
		if(nChild>=0)
		{
			(*m_pitems)[nChild ]._next = nIndex;
			(*m_pitems)[nIndex]._piror = nChild;
		}else
		{
			if(nItemParent>=0)
				(*m_pitems)[nItemParent ]._child = nIndex;
		}
		return nIndex;
	}

	template<class T>
	inline int xtree<T>::insertItem(int nTree,int nItemParent,int nItemPiror,T &item)
	{
		assert(nTree >=0);

		treeitem ti;
		ti.data = item;
		ti._parent = nItemParent;
		ti._piror = -1;
		ti._next = -1;
		ti._child = -1;
		int nIndex = m_pitems->addItem(ti);

		int nChild = -1;
		if(nItemParent < 0) 
		{
			if((*m_indexs)[nTree]<=0)
			{
				(*m_indexs)[nTree] = nIndex + 1; //add to indexs from 1
				return nIndex;
			}else
			{
				nChild = (*m_indexs)[nTree] - 1;
				if(nItemPiror <0)
				{
					int it = (*m_indexs)[nTree];
					(*m_indexs)[nTree] = nIndex + 1; //add to indexs from 1
					(*m_pitems)[nIndex]._next = (*m_indexs)[nTree];
					(*m_pitems)[it]._piror = nIndex;
					return nIndex;
				}
			}
		}else
		{
			if(nItemPiror <0)
				nChild = -1;
			else
				nChild = (*m_pitems)[nItemParent ]._child;
		}
		
		while(nChild >=0 && nItemPiror != nChild)
		{
			if((*m_pitems)[nChild ]._next>=0)nChild = (*m_pitems)[nChild]._next;
		}

		if(nChild)
		{
			(*m_pitems)[nChild ]._next = nIndex;
			(*m_pitems)[nIndex]._piror = nChild;
		}else
		{
			if(nItemParent>=0)
			{
				(*m_pitems)[nItemParent ]._child = nIndex;
			}
		}
		return nIndex;
	}

	template<class T>
	inline int xtree<T>::removeItem(int nTree,int nItem)
	{
		assert(nTree >=0);
		assert(nItem >=0);
		if((*m_indexs)[nTree]<=0) return -1;
		if((*m_indexs)[nTree]=nItem + 1) 
		{
			(*m_indexs)[nTree] = 0;
			return -1;
		}
		int piror;
		int next;
		int parent;
		parent = (*m_pitems)[nItem]._parent;
		piror = (*m_pitems)[nItem]._piror;
		next = (*m_pitems)[nItem]._next;
		
		if(piror < 0)
		{
			(*m_pitems)[parent]._child = next;
			(*m_pitems)[next]._piror = piror;
		}
		else
		{
			(*m_pitems)[piror]._next = next;
			(*m_pitems)[next]._piror = piror;
		}
		return -1;
	}

	template<class T>
	inline T & xtree<T>::getItem(int nTree,int nItem)
	{
		return (*m_pitems)[nItem ].data;
	}
	
	template<class T>
	inline void xtree<T>::setItem(int nTree,int nItem,T& data)
	{
		(*m_pitems)[nItem ].data = data;
	}

	template<class T>
	inline T & xtree<T>::getItem(int nItem)
	{
		return (*m_pitems)[nItem ].data;
	}
	
	template<class T>
	inline void xtree<T>::setItem(int nItem,T& data)
	{
		(*m_pitems)[nItem ].data = data;
	}

	template<class T>
	inline int xtree<T>::getNext(int nTree,int nIndex)
	{
		return (*m_pitems)[nIndex ]._next;
	}

	template<class T>
	inline int xtree<T>::getParent(int nTree,int nIndex)
	{
		return (*m_pitems)[nIndex ]._parent;

	}

	template<class T>
	inline int xtree<T>::getPiror(int nTree,int nIndex)
	{
		return (*m_pitems)[nIndex ]._piror;
	}

	template<class T>
	inline int xtree<T>::getChild(int nTree,int nIndex)
	{
		return (*m_pitems)[nIndex ]._child;
	}

	template<class T>
	inline int xtree<T>::getRoot(int nTree)
	{
		return (*m_indexs)[nTree] - 1;
	}

}