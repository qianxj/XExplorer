#pragma once
#include "xvmdef.h"
#include "xalloc.h"

namespace xcode
{	
	template<class T>
	class XVM_API xsoltvc 
	{
	public:
		xsoltvc();
		~xsoltvc();
	private:
		T** m_pSlot;
		//size of slots;
		int m_nSizeSlot;
	private:
		//check solt is alloc and soltitem is alloc  alloc it if not
		bool ensureSoltItem(int nsolt);
		//alloc slot 
		inline T** allocSlot();
		//alloc slot item
		inline T* allocSlotItem();
	public:	
		//get data
		T getdata(int nIndex);
		//set data
		T setdata(int nIndex, T& data);
	public:
		//define array to read stack data,position as stack top is zero 
		T & operator [](int nIndex)throw(...);
		const T & operator [](int nIndex) const throw(...) ;
	};

}