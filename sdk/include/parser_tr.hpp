#ifndef PARSER_H
#define  PARSER_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include "scan_tr.hpp"
#include <stack>
#include "meta.hpp"

using namespace std;
using namespace wpp::how;

struct VLANG3_API TuringTokenTape : TuringIterator<token>
{
public:
	turingScan scan;
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	TuringTokenTape(): TuringIterator<token>(){};
	TuringTokenTape(_Tptr _Ptr) :TuringIterator<token>(_Ptr){};
#pragma warning( pop )

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
		//++_Myptr;
		scan.insymbol();
		if(!scan.getToken().ty)
			_Myptr = 0;
		else
			_Myptr = & scan.getToken();

		return (*this);
	}

	virtual _Myt operator++(int)
	{	// postincrement
		//throw L"not support operate postincrement";
		
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	virtual _Myt left()
	{	
		throw L"not support method left";
		return TuringIterator(0);
	}

	virtual _Myt right()
	{
		throw L"not support method right";
		return TuringIterator(0);
	}

	virtual _Myt& operator--()
	{	
		throw L"not support operate --";
		--_Myptr;
		return (*this);
	}
	
	virtual _Myt operator--(int)
	{	// predecrement
		throw L"not support operate predecrement";
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
		throw L"not support operate +=";
		_Myptr += _Off;
		return (*this);
	}

	virtual _Myt operator+(difference_type _Off) const
	{	
		throw L"not support operate +";
		_Myt _Tmp = *this;
		return (_Tmp += _Off);
	}

	virtual _Myt& operator-=(difference_type _Off)
	{	
		throw L"not support operate -=";
		return (*this += -_Off);
	}

	virtual _Myt operator-(difference_type _Off) const
	{	
		throw L"not support operate -";
		_Myt _Tmp = *this;
		return (_Tmp -= _Off);
	}

	virtual difference_type operator-(const _Myt& _Right) const
	{	
		throw L"not support operate -";
		return (int) (_Myptr  - _Right._Myptr);
	}

	virtual reference operator[](difference_type _Off) const
	{	
		throw L"not support operate []";
		return (*(*this + _Off));
	}
};



struct VLANG3_API turingParser : public Turing<TuringTokenTape>
{
public:
	bool isTapeEnd(TuringTape &tape);
	bool MyMatch(TuringTokenTape &tape,CWord & t, CWord::iterator iter);
	int	 MyAction(TuringTokenTape &tape,CWord & t, CWord::iterator & action,int &halt);
public:
	wMetaTable* m_pMetaTable;
	token m_tk;
public:
	turingParser();
};

#endif //PARSER_H
