#pragma once

#include <deque>
using namespace std;

template<class _Ty>
struct xtreeNode
{
	_Ty	 		_v;
	xtreeNode* _next;
	xtreeNode* _piror;
	xtreeNode* _fchild;
	xtreeNode* _lchild;
	xtreeNode* _parent;

	xtreeNode(const _Ty& v):_v(v),_next(NULL),_piror(NULL),_fchild(NULL),_lchild(NULL),_parent(NULL){}
	xtreeNode():_next(NULL),_piror(NULL),_fchild(NULL),_lchild(NULL),_parent(NULL){}
};

template<class _Ty>
class xtreeIterator
{
public:
	typedef	xtreeIterator _Myt;
	typedef int difference_type;
	typedef _Ty* _Tptr;
	typedef _Ty & reference;
	typedef _Ty* pointer;
	typedef xtreeNode<_Ty>* _Nptr;
public:
	xtreeIterator()
	{	
	}
	xtreeIterator(_Tptr _Ptr):_Myptr(_Ptr)
	{	
	}

	reference operator*() const
	{
		return (*_Myptr);
	}

	pointer operator->() const
	{	
		return (&**this);
	}

	_Myt& operator++()
	{	
		_Myptr = (_Tptr)((_Nptr)_Myptr)->_next;
		return (*this);
	}

	_Myt operator++(int)
	{	// postincrement
		_Myt _Tmp = *this;
		_Myptr = (_Tptr)((_Nptr)_Myptr)->_next;
		return (_Tmp);
	}

	_Myt left()
	{	
		return xtreeIterator((_Tptr)((_Nptr)_Myptr)->_fchild);
	}

	_Myt leftlast()
	{	
		return xtreeIterator((_Tptr)((_Nptr)_Myptr)->_lchild);
	}

	_Myt right()
	{
		return xtreeIterator((_Tptr)((_Nptr)_Myptr)->_parent);
	}

	_Myt& operator--()
	{	
		_Myptr = (_Tptr)((_Nptr)_Myptr)->_piror;
		return (*this);
	}
	
	_Myt operator--(int)
	{	// predecrement
		_Myt _Tmp = *this;
		_Myptr = (_Tptr)((_Nptr)_Myptr)->_piror;
		return (_Tmp);
	}

	bool operator==(const _Myt& _Right) const
	{	
		return (_Myptr == _Right._Myptr);
	}

	bool operator!=(const _Myt& _Right) const
	{	
		return (!(*this == _Right));
	}

	_Tptr _Myptr;	
};

template<class _Ty>
class xtree
{
public:
	typedef	xtreeIterator<_Ty> Iterator;
public:
	deque<xtreeNode<_Ty>> _items;
public:
	Iterator begin()
	{
		Iterator tr((_Ty*)&_items[0]);
		return tr;
	}
	Iterator end()
	{
		Iterator tr((_Ty*)_end);
		return tr;
	}
public:
	xtree():_end(NULL){};
public:
	const _Ty& push( const _Ty& t)
	{
		xtreeNode<_Ty> v(t);
		if(_items.size()==0)
			_items.push_back(v);
		else
		{
			xtreeNode<_Ty> *p = &_items[0];
			while(p->_next)p = p->_next;
			_items.push_back(v);
			p->_next = &_items[_items.size()-1];
			_items[_items.size()-1]._piror = p;
		}
		return (const _Ty&)_items[_items.size()-1];
	}
	const _Ty& push(Iterator &tr, const _Ty& t)
	{
		xtreeNode<_Ty> v(t);
		xtreeNode<_Ty> *p = (xtreeNode<_Ty> *)tr._Myptr;
		
		_items.push_back(v);
		_items[_items.size()-1]._parent = p;
		if(!p->_lchild)
		{
			p->_fchild = &_items[_items.size()-1];
			p->_lchild = &_items[_items.size()-1];
		}else
		{
			p->_lchild->_next = &_items[_items.size()-1];
			_items[_items.size()-1]._piror = p->_lchild;
			p->_lchild = &_items[_items.size()-1];
		}
		return (const _Ty&)_items[_items.size()-1];
	}
private:
	xtreeNode<_Ty>* _end;
};