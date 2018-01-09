#ifndef WPP_TURINGITERATOR_H
#define WPP_TURINGITERATOR_H

#include <vector>
using namespace std;

namespace wpp  {
namespace how  {

template<class _Ty>
struct TuringIterator
{
public:
	typedef	TuringIterator _Myt;
	typedef int difference_type;
	typedef _Ty* _Tptr;
	typedef _Ty& reference;
	typedef _Ty* pointer;
private:
	vector<_Tptr>* marks;
public:
	TuringIterator(): _Myptr (0),marks(new vector<_Tptr>) {};
	TuringIterator(_Tptr _Ptr) : _Myptr (_Ptr),marks(new vector<_Tptr>){};

	virtual reference operator*() const
	{
		return (*_Myptr);
	}

	virtual pointer operator->() const
	{	
		return (&**this);
	}

	virtual _Myt& operator++()
	{	
		++_Myptr;
		return (*this);
	}

	virtual _Myt operator++(int)
	{	// postincrement
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	virtual _Myt left()
	{	
		return TuringIterator(0);
	}

	virtual _Myt right()
	{
		return TuringIterator(0);
	}

	virtual _Myt& operator--()
	{	
		--_Myptr;
		return (*this);
	}
	
	virtual _Myt operator--(int)
	{	// predecrement
		_Myt _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	virtual bool operator==(const _Myt& _Right) const
	{	
		return (_Myptr == _Right._Myptr);
	}

	virtual bool operator!=(const _Myt& _Right) const
	{	
		return (!(*this == _Right));
	}

	virtual _Myt& operator+=(difference_type _Off)
	{	
		_Myptr += _Off;
		return (*this);
	}

	virtual _Myt operator+(difference_type _Off) const
	{	
		_Myt _Tmp = *this;
		return (_Tmp += _Off);
	}

	virtual _Myt& operator-=(difference_type _Off)
	{	
		return (*this += -_Off);
	}

	virtual _Myt operator-(difference_type _Off) const
	{	
		_Myt _Tmp = *this;
		return (_Tmp -= _Off);
	}

	virtual difference_type operator-(const _Myt& _Right) const
	{	
		return (int) (_Myptr  - _Right._Myptr);
	}

	virtual reference operator[](difference_type _Off) const
	{	
		return (*(*this + _Off));
	}

	virtual void mark()
	{
		this->marks->push_back(_Myptr);
	}

	virtual void popmark()
	{
		if((int)marks->size()>0)this->marks->pop_back();
	}

	virtual _Myt tomark(int index = 0)
	{
		if((int)marks->size()>= index + 1)
			return _Myt((*marks)[marks->size() - index - 1]);
		return _Myt();
	}

	_Tptr _Myptr;	
};

}} //namespace wpp::how

#endif //WPP_TURINGITERATOR_H
