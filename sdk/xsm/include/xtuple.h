#pragma once
#include "xvmdef.h"
#include "xcolumn.h"

namespace xcode
{


	class  xtuplemgr
	{
	public:
		xtuplemgr();
		~xtuplemgr();

	public:
		class xvmcore * m_pxvm;
	public:
		XVM_API void * allocData(xtuple& t);
		XVM_API void * allocData(int htupleid);
		XVM_API int getAllocSize(xtuple& t);
		XVM_API int getAllocSize(int htupleid);
		XVM_API int getPosition(xtuple& t,int nIndex);
		XVM_API int getPosition(xtuple& t,void * pname);
		XVM_API int getPosition(xtuple& t,void * pname, int &hItem,xcolinfo &colinfo);
		XVM_API int getPosition(xtuple& t,int nIndex, int &hItem,xcolinfo &colinfo);

		template<class T>
		void setData(xtuple& t, int nColIndex,T &data)
		{
			assert(t.data);
			int nPos = getPosition(t,nColIndex);
			assert(nPos >= 0);
			if(t.data) *(T*)((BYTE *)t.data + nPos) = data; 
		}

		template<class T>
		T getData(xtuple& t, int nColIndex)
		{
			assert(t.data);
			int nPos = getPosition(t,nColIndex);
			assert(nPos >= 0);
			return *(T*)((BYTE *)t.data + nPos);
		}

		template<class T>
		T getDataPtr(xtuple& t, int nColIndex)
		{
			assert(t.data);
			int nPos = getPosition(t,nColIndex);
			assert(nPos >= 0);
			return T ((BYTE *)t.data + nPos);
		}
	};
}
