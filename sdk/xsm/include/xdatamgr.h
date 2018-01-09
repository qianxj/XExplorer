#pragma once
#include "xvmdef.h"
#include "xalloc.h"
#include "xthrows.h"
#include "xcolumn.h"

namespace xcode
{
	struct xardataitem
	{
		int  sdim; //start dim 
		int  edim; //end dim
		int	 elesize;
		int  size;
		bool direct;
		int  bound;
		xarrayinfo* pInfo;
	};
	struct xardata
	{
		int dims;
		int count; //is array
		int hitem; //is array tuple item
		xardataitem* pItems;
		void * pdirectData; 
		union
		{
			xarrays<byte*,byte>* pData;
			xarraysp<void**,void*>* pDataEx;
		};
	};

	class xdatamgr
	{
	public:
		xdatamgr();
		~xdatamgr();
	public:
		class xvmcore * m_pxvm;
	public:
		XVM_API xardata * initxardata(int hItem);
		XVM_API xardata * initxardata(xcolinfo &info);
	public:
		void * getArrDataPtr(xardata * pardata, int cindex,int * pIndexs);

		template<class T>
		inline T getArrData(xardata * pardata, int cindex,int * pIndexs,bool tupledata )
		{
			void * p = getArrDataPtr(pardata,cindex,pIndexs);
			if(p)
			{
				if(tupledata)
					return p;
				else
					return *(T*)p;
			}
			return 0;
		}

		template<class T>
		inline T getArrData(xardata * pardata, int cindex,int * pIndexs)
		{
			void * p = getArrDataPtr(pardata,cindex,pIndexs);
			if(p) return *(T*)p;
			return 0;
		}


		template<class T>
		inline void setArrData(xardata * pardata,  int cindex,int * pIndexs ,T data)
		{
			assert(cindex==pardata->dims );
			void *p = null;
			int sindex = 1;
			int index = -1;
			xarrayinfo* pa;

			for(int i=pardata->count - 1;i>=0;i--)
			{
				sindex = 1;
				index = pIndexs[pardata->pItems[i].sdim];
				if(pardata->pItems[i].bound < index) pardata->pItems[i].bound = index;
				if(pardata->pItems[i].sdim != pardata->pItems[i].edim)
				{
					pa = pardata->pItems[i].pInfo;
					int k= 1;
					while(k<= pardata->pItems[i].edim - pardata->pItems[i].sdim)
					{
						index += sindex * (pIndexs[k + pardata->pItems[i].sdim] + 1); 
						sindex *= pa->high - pa->low + 1;
						pa = (xarrayinfo*)&m_pxvm->m_colmgr->m_parrays->elementAt(pa->next);
						k++;
					}
				}
				if(i==pardata->count - 1)
				{
					if(pardata->pdirectData)
						p = i==0?((byte*)pardata->pdirectData)+ index * pardata->pItems[i].elesize:
								((void**)pardata->pdirectData)[index];
					else
						p = i==0?(void *)&pardata->pData->makeAt(index):
							(void *)pardata->pData->makeAt(index);
					if(i==0)
					{
						*(T *)p = data;
						return ;
					}
				}else
				{
					if(i==0)
					{
						if(pardata->pItems[i].direct)
						{
							void * p1 = new byte[pardata->pItems[i].size*pardata->pItems[i].elesize];
							*(void **)p = p1;
							::memset(p1,0,pardata->pItems[i].size*pardata->pItems[i].elesize);
							((T*)p1)[index] = data;
						}else
						{
							void * p1 = new xarrays<byte*,byte>(pardata->pItems[i].elesize);
							*(void **)p = p1;
							(T &)((xarrays<byte*,byte> *)p1)[index] = data;
						}
					}else
					{
						if(pardata->pItems[i].direct)
						{
							void * p1 = new byte[pardata->pItems[i].size*pardata->pItems[i].elesize];
							*(void **)p = p1;
						}else
						{
							void * p1 = new xarraysp<void**,void*>();
							*(void **)p = p1;
						}
					}
				}
			}
			return ;
		}

		template<class T>
		inline void setArrData(xardata * pardata,  int cindex,int * pIndexs ,T &data,bool tupledata)
		{
			assert(cindex==pardata->dims );
			void *p = null;
			int sindex = 1;
			int index = -1;
			xarrayinfo* pa;

			for(int i=pardata->count - 1;i>=0;i--)
			{
				sindex = 1;
				index = pIndexs[pardata->pItems[i].sdim];
				if(pardata->pItems[i].bound < index) pardata->pItems[i].bound = index;
				if(pardata->pItems[i].sdim != pardata->pItems[i].edim)
				{
					pa = pardata->pItems[i].pInfo;
					int k= 1;
					while(k<= pardata->pItems[i].edim - pardata->pItems[i].sdim)
					{
						index += sindex * (pIndexs[k + pardata->pItems[i].sdim] + 1); 
						sindex *= pa->high - pa->low + 1;
						pa = (xarrayinfo*)&m_pxvm->m_colmgr->m_parrays->elementAt(pa->next);
						k++;
					}
				}
				if(i==pardata->count - 1)
				{
					if(pardata->pdirectData)
						p = i==0?((byte*)pardata->pdirectData)+ index * pardata->pItems[i].elesize:
								((void**)pardata->pdirectData)[index];
					else
						p = i==0?(void *)&pardata->pData->makeAt(index):
							(void *)pardata->pData->makeAt(index);
					if(i==0)
					{
						if(tupledata)
							::memcpy_s(p,
								pardata->pItems[i].elesize,
								data,pardata->pItems[i].elesize);
						else
							*(T *)p = data;
						return ;
					}
				}else
				{
					if(i==0)
					{
						if(pardata->pItems[i].direct)
						{
							void * p1 = new byte[pardata->pItems[i].size*pardata->pItems[i].elesize];
							*(void **)p = p1;
							::memset(p1,0,pardata->pItems[i].size*pardata->pItems[i].elesize);
							if(tupledata)
								::memcpy_s(((byte*)p1) + index * pardata->pItems[i].elesize,
									 pardata->pItems[i].elesize,
									data,pardata->pItems[i].elesize);
							else
								((T*)p1)[index] = data;
						}else
						{
							void * p1 = new xarrays<byte*,byte>(pardata->pItems[i].elesize);
							*(void **)p = p1;
							if(tupledata)
								::memcpy_s(&((xarrays<byte*,byte> *)p1)[index],pardata->pItems[i].elesize,
								data,pardata->pItems[i].elesize);
							else
								(T &)((xarrays<byte*,byte> *)p1)[index] = data;
						}
					}else
					{
						if(pardata->pItems[i].direct)
						{
							void * p1 = new byte[pardata->pItems[i].size*pardata->pItems[i].elesize];
							*(void **)p = p1;
						}else
						{
							void * p1 = new xarraysp<void**,void*>();
							*(void **)p = p1;
						}
					}
				}
			}
			return ;
		}
	};
}