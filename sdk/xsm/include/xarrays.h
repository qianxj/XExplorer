#pragma once
#include "xvmdef.h"
#include "xalloc.h"
#include "xthrows.h"

namespace xcode
{	
	template<class TSec,class T>
	class  xarrays 
	{
	public:
		xarrays();
		xarrays(int nEleSize); //每单元T个数
		~xarrays();

		typedef union{TSec t; T* p; }FSec;
	private:
		int eleSize;
	private:
		//follow are 
		unsigned short	indexMask, secShift;
		unsigned short	sectionSize;
		int				secDirSize;
	public:
		int				m_highBound;
	protected:
		FSec*			m_pSecDir;
		int				m_secDirSize;
	public:
		int				getHighBound(){return m_highBound;} ;
	public:
		//create 
		void create(int SecDirSize, unsigned short SectionSize);
		void clear();
	private:
		//check secitem is alloc  alloc it if not
		bool ensureSecItem(int nSecIndex);
	public:	
		//follow are get or set data item
		const T  getAt(int nIndex) const throw(...);
		T getAt(int nIndex) throw(...);
		const T &  elementAt(int nIndex) const throw(...);
		T&  makeAt(int nIndex) throw(...);
		virtual void putAt(int nIndex, T& item) throw(...);
	public:
		virtual int addItem(T& item) throw(...);

	public:
		virtual void resetHighBound();
	public:
		T & operator [](int nIndex)throw(...);
		const T  operator [](int nIndex) const throw(...) ;
	};

	template<class TSec,class T>
	class  xarraysp : public xarrays<TSec,T>
	{
	public:
		virtual void putAt(int nIndex, T& item) throw(...);
		virtual void resetHighBound();
	};

	template<class TSec, class T>
	inline xarrays<TSec,T>::xarrays(void):m_secDirSize(0),sectionSize(32),secDirSize(256),
				indexMask(31), secShift(5),m_pSecDir(null),m_highBound(-1),eleSize(1)
	{
	}
	template<class TSec, class T>
	inline xarrays<TSec,T>::xarrays(int nEleSize):eleSize(nEleSize),m_secDirSize(0),sectionSize(32),secDirSize(256),
				indexMask(31), secShift(5),m_pSecDir(null),m_highBound(-1)
	{
	}

	template<class TSec, class T>
	inline xarrays<TSec,T>::~xarrays(void)
	{
		if(m_pSecDir)
		{
			for(int i=0;i<m_secDirSize;i++)
			{
				if(m_pSecDir[i].p)xfree<T>(m_pSecDir[i].p);
			}
			xfree<FSec>(m_pSecDir);
		}
	}

	//initiat array
	template<class TSec, class T>
	inline void xarrays<TSec,T>::create(int SecDirSize, unsigned short SectionSize)
	{
		this->clear();

		this->secDirSize = SecDirSize;
		this->sectionSize = SectionSize;

		m_highBound = -1;
		indexMask = 0;
		secShift = 0;
		while(SectionSize != 1)
		{
			SectionSize >>= 1;
			(indexMask <<= 1)++;
			secShift++;
		}
	}

	template<class TSec, class T>
	inline void xarrays<TSec,T>::clear()
	{
		if(m_pSecDir)
		{
			for(int i=0;i<m_secDirSize;i++)
			{
				if(m_pSecDir[i].p)xfree<T>(m_pSecDir[i].p);
			}
			xfree<FSec>(m_pSecDir);
			m_pSecDir = null;
		}
	}

	//check solt is alloc and soltitem is alloc  alloc it if not
	template<class TSec, class T>
	inline bool  xarrays<TSec,T>::ensureSecItem(int nSecIndex)
	{
		if(nSecIndex >= m_secDirSize)
		{
			m_secDirSize += secDirSize;
			FSec* pTemp = (FSec*)xalloc<FSec>(m_secDirSize);
			if(!pTemp)return false;
			if(m_pSecDir)
				xmemcpy<FSec*>(m_pSecDir,pTemp,m_secDirSize - secDirSize);
			xfree<FSec>(m_pSecDir);
			m_pSecDir = pTemp ;
		}
		if(!m_pSecDir[nSecIndex].p)
		{
			m_pSecDir[nSecIndex].p = xalloc<T>(sectionSize * eleSize);
			if(!m_pSecDir[nSecIndex].p)return false;
		}
		return true;
	}

	template<class TSec, class T>
	inline const  T  xarrays<TSec,T>::getAt(int nIndex) const throw(...)
	{
		assert(nIndex >=0);
		if(nIndex >this->m_highBound) throw thr_outarraybound;
		
		int secIndex;
		secIndex = (nIndex >> secShift );
		if(secIndex >= this->m_secDirSize) throw thr_outarraybound;
		if(!m_pSecDir[secIndex].p)throw thr_unvalidindex;
		return m_pSecDir[secIndex].p[(nIndex & indexMask) * eleSize ]; 
	}

	template<class TSec, class T>
	inline const  T&  xarrays<TSec,T>::elementAt(int nIndex) const throw(...)
	{
		assert(nIndex >=0);
		if(nIndex >this->m_highBound) throw thr_outarraybound;
		
		int secIndex;
		secIndex = (nIndex >> secShift );
		if(secIndex >= this->m_secDirSize) throw thr_outarraybound;
		if(!m_pSecDir[secIndex].p)throw thr_unvalidindex;
		return m_pSecDir[secIndex].p[(nIndex & indexMask)* eleSize]; 
	}


	template<class TSec, class T>
	inline T xarrays<TSec,T>::getAt(int nIndex)  throw(...)
	{
		assert(nIndex >=0);
		if(nIndex >this->m_highBound) throw thr_outarraybound;
		
		int secIndex;

		secIndex = (nIndex >> secShift );
		if(secIndex >= this->m_secDirSize) throw thr_outarraybound;
		if(!m_pSecDir[secIndex].p)throw thr_unvalidindex;
		return m_pSecDir[secIndex].p[(nIndex & indexMask)* eleSize]; 
	}

	template<class TSec, class T>
	inline T&  xarrays<TSec,T>::makeAt(int nIndex) throw(...)
	{
		assert(nIndex >=0);
		int secIndex;
		secIndex = (nIndex >> secShift );

		if(!ensureSecItem(secIndex))
		{
			MessageBox(NULL,_T("out of memeory"),_T("错误"),MB_OK);
			throw thr_outmemory;
		}
		if(nIndex > m_highBound)m_highBound = nIndex;

		return m_pSecDir[secIndex].p[(nIndex & indexMask)* eleSize]; 
	}

	template<class TSec, class T>
	inline void xarrays<TSec,T>::putAt(int nIndex, T& item) throw(...)
	{
		makeAt(nIndex) = item;
		if(nIndex > m_highBound)m_highBound = nIndex;
	}

	template<class TSec, class T>
	inline int xarrays<TSec,T>::addItem(T& item) throw(...)
	{
		putAt(m_highBound + 1,item);
		return m_highBound ;
	}


	template<class TSec, class T>
	inline T & xarrays<TSec,T>::operator [](int nIndex)throw(...)
	{
		return this->makeAt(nIndex);
	}
	
	template<class TSec, class T>
	inline const T  xarrays<TSec,T>::operator [](int nIndex) const throw(...)
	{
		return this->getAt(nIndex);
	}

	template<class TSec, class T>
	inline void xarrays<TSec,T>::resetHighBound()
	{
	}

	template<class TSec, class T>
	inline void xarraysp<TSec,T>::putAt(int nIndex, T& item) throw(...)
	{
		xarrays<TSec,T>::putAt(nIndex,item);
		if(!item)resetHighBound(); 
	}
	template<class TSec, class T>
	inline void xarraysp<TSec,T>::resetHighBound()
	{
		int i;
		for(i=m_highBound;i>=0;i--)
		{
			if(this->getAt(i))break;
		}
		m_highBound = i;
	}
}