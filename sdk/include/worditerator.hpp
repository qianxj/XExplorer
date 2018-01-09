#ifndef WPP_WORDITERATOR_H
#define WPP_WORDITERATOR_H

#include "turingIterator.hpp"

namespace wpp {
namespace how {

struct CWord;
struct CWordItem;
struct CWordIndex;
template<class T>
class WordIterator : public  TuringIterator<T>
{
private:
	int _pred;
	int _index;
	int _first;
	CWord _word;

public:
	WordIterator():_pred(0),_index(0),_first(0),_word(0)
	{
	}
	WordIterator(CWord& w, int index,int pred):_word(w),_pred(pred),_index(index),_first(index),TuringIterator(CWord::get_word_item(index))
	{
	}
	
	operator bool()
	{
		return _index? true: false;
	}

	int getPred() const
	{	
		return _pred;
	}

	int get_index() const
	{	
		return _index;
	}

	int get_item_pred()
	{
		return _word.get_item_pred(CWord::get_word_item(get_index()));
	}

	int getWordIndex() const
	{
		return _word.wIndex.first;
	}

	CWord* get_word()
	{
		return &_word;
	}

	WordIterator items()
	{
		return _word.get_items(*this);
	}

	WordIterator items(int pred)
	{
		return CWord((*this)->v.wi).begin(pred);
	}

	WordIterator items(wchar_t* pred)
	{
		return CWord((*this)->v.wi).begin(CWord::get_string_pred(pred));
	}

	_Myt& first() 
	{
		_index = _first;
		return *this;
	}

	_Myt& last() 
	{
		/*
		TPredLink::Link* p = TWordIndex::getLink(_index);
		int pindex = _index;
		while(p)
		{
			if(!p->next)break;
			pindex = p->next;
			p = TWordIndex::getLink(pindex);
		}
		_index = pindex;*/
		return *this;
	}

	_Myt& operator--()
	{	
		/*
		if(_index == 0) return (*this);
		if(_index == _first)
		{
			_index = 0;
			return (*this);
		}
		if(_pred)
		{
			TPredLink::Link* p = TWordIndex::getLink(_first);
			if(p)
			{
				int index = _Ty::getPredIndex(p->next,_pred);
				int pindex = _first;
				while(_index != index)
				{
					TPredLink::Link* p = TWordIndex::getLink(index);
					if(!p)
					{
						_index = 0;
						return (*this);
					}else
					{
						pindex = index;
						index = _Ty::getPredIndex(p->next,_pred);
					}
				}
				_index = pindex;
			}
			else 
				_index = 0;
		}else
		{
			TPredLink::Link* p = TWordIndex::getLink(_first);
			if(p)
			{
				int index = p->next;
				int pindex = _first;
				while(_index != index)
				{
					TPredLink::Link* p = TWordIndex::getLink(index);
					if(!p)
					{
						_index = 0;
						return (*this);
					}else
					{
						pindex = index;
						index = p->next;
					}
				}
				_index = pindex;
			}else
				_index = 0;
		}*/
		return (*this);
	}

	_Myt& operator++()
	{	
		if(_index == 0) return (*this);

		CWordItem* p = get_word()->get_word_item(_index);
		if(p)
			_index = get_word()->find_pred_index(p->next,_pred);
		else 
			_index = 0;
		_Myptr = _index ? get_word()->get_word_item(_index): 0;
		
		return (*this);
	}

	_Myt operator++(int)
	{	
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Myt operator--(int)
	{	
		_Myt _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	bool operator == (_Myt & t)
	{	
		return _index == ((WordIterator &)t)._index && (_index==0 || _pred == ((WordIterator &)t)._pred);
	}

	bool operator != (_Myt & t)
	{	
		return !operator ==(t);
	}
};

}} //namespace wpp::how

#endif //WPP_WORDITERATOR_H
