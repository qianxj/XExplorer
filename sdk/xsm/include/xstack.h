#pragma once
#include "xvmdef.h"
#include "xalloc.h"
#include "xarrays.h"

namespace xcode
{	
	template class xarrays<dataitem*,dataitem>; 
	template<class T>
	class XVM_API xstack : public xarrays<T*,T>
	{
	public:
		xstack();
		~xstack();
	public:
		//esp;
		int m_nsp;
	public:
		//follow si stack operate
		void push(T &d)throw(...);
		T & push() throw(...);
		const T & pop();
		void pop(int n);
	public:
		//define array to read stack data,position as stack top is zero 
		T & operator [](int nIndex)throw(...);
		const T  operator [](int nIndex) const throw(...) ;
	};

}