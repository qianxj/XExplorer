#pragma once
#include "xvmdef.h"
#include "xcolumn.h"

namespace xcode
{


	struct XVM_API xtuple
	{
		int colId;
		void * data;
	};

	class  xtuplemgr
	{
	public:
		xtuplemgr();
		~xtuplemgr();

	public:
		class xvmcore * m_pxvm;
	public:
		XVM_API void * allocData(xtuple& t);
		XVM_API int getAllocSize(xtuple& t);
		XVM_API int getPosition(xtuple& t,int nIndex);
		XVM_API int getPosition(xtuple& t,void * pname);

		template<class T>
		void setData(xtuple& t, int nColIndex,T &data)
		{
			int nPos = getPosition(t,nColIndex);
			if(nPos < 0) return ;
			if(t.data) *(T*)((BYTE *)t.data + nPos) = data; 
		}

		template<class T>
		T getData(xtuple& t, int nColIndex)
		{
			int nPos = getPosition(t,nColIndex);
			if(nPos < 0) return 0 ;
			if(t.data)
				return *(T*)((BYTE *)t.data + nPos);
			else return 0;
		}
	};
}
