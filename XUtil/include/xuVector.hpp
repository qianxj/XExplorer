#pragma once
namespace Hxsoft{ namespace XUtil
{
	 
	template<class T>
	class xuVector
	{
	public:
		xuVector():AlignSize(16),m_size(0),maxSize(0),m_pT(NULL){;}
		~xuVector(void){if(m_pT) delete m_pT;}
	public:
		T * m_pT;
	private:
		int AlignSize;
		int m_size ;
		int maxSize;
	public:
		int clear();
		int push_back(T item);
		T pop_back();
		T erase(int pos);
		int size();
		void setsize(int nSize);
		int insert(int pos,T item);
		T get(int pos);
		int set(int pos,T item);
		T & operator[](int pos) ;
		T operator[](int pos) const;
		T * GetBuffer(){return m_pT;};
	};
	template<class T>
	inline int xuVector<T>::clear()
	{
		m_size = 0;
		return 1;
	}
	template<class T>
	inline int xuVector<T>::insert(int pos,T item)
	{
		if(pos < 0) return -1;
		if(m_size == maxSize )
		{
			maxSize += AlignSize;
			T * Temp = new T[maxSize];
			if(m_size > 0)::memcpy((LPVOID)Temp,(LPVOID)m_pT,sizeof(T)*m_size);
			if(m_pT)delete m_pT;
			m_pT = Temp;
		}
		if(pos >= m_size)pos = m_size;
		for(int i=m_size - 1;i>=pos;i--)m_pT[i + 1] = m_pT[i];
		m_pT[pos] = item;
		m_size++;
		return pos;
	}

	template<class T>
	inline int xuVector<T>::size(){return m_size;}

	template<class T>
	inline void xuVector<T>::setsize(int nSize)
	{
		this->m_size = nSize;
	}
	template<class T>
	inline int xuVector<T>::push_back(T item){return insert(m_size,item);m_size++;}

	template<class T>
	inline T xuVector<T>::pop_back()
	{
		if(m_size <1)
			throw(-1);
		m_size--;
		return m_pT[m_size];
	}

	template<class T>
	inline T xuVector<T>::erase(int pos)
	{
		T item;
		if( pos > m_size||pos<0)
			throw(-1);
		else
		{
			item = m_pT[pos];
			for(int i=pos;i< m_size - 1 ;i++) m_pT[i] = m_pT[i + 1];
			m_size--;
			return item;
		}
	}

	template<class T>
	inline T xuVector<T>::get(int pos)
	{
		if( pos > m_size||pos<0)throw(-1);
		return m_pT[pos];
	}

	template<class T>
	inline int xuVector<T>::set(int pos,T item)
	{
		if( pos > m_size||pos<0)throw(-1);
		m_pT[pos] = item;
		return pos;
	}	
	template<class T>
	inline  T & xuVector<T>::operator[](int pos)
	{
		if( pos > m_size||pos<0)throw(-1);
		return m_pT[pos];
	}
	
	template<class T>
	inline T  xuVector<T>::operator[](int pos) const
	{
		if( pos > m_size||pos<0)throw(-1);
		return m_pT[pos];
	}
}}
