#ifndef WPP_VORDITERATOR_H
#define WPP_VORDITERATOR_H

#include "vorddef.hpp"

template<class T>
class vorditerator_t 
{
private:
	int pred_;
	int first_;
	int index_;
	int vord_;

public:
	vorditerator_t(): pred_(0),index_(0),first_(0),vord_(0)
	{
	}

#if 0
	vorditerator_t(vord& w, int index,int pred):_vord(w),_pred(pred),_index(index),_first(index),TuringIterator(CWord::get_word_item(index))
	{
	}
	
	operator bool()
	{
		return index_? true: false;
	}

	int get_pred() const
	{	
		return pred_;
	}

	int get_index() const
	{	
		return index_;
	}

	int get_item_pred()
	{
		return _word.get_item_pred(CWord::get_word_item(get_index()));
	}

	int get_ivord() const
	{
		return vord_;
	}

	vord* get_vord()
	{
		return & vord(vord_);
	}

	vorditerator items()
	{
		return _word.get_items(*this);
	}

	vorditerator items(int pred)
	{
		return CWord((*this)->v.wi).begin(pred);
	}

	vorditerator items(wchar_t* pred)
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
		return _index == ((vorditerator &)t)._index && (_index==0 || _pred == ((vorditerator &)t)._pred);
	}

	bool operator != (_Myt & t)
	{	
		return !operator ==(t);
	}
#endif

};


#endif //WPP_VORDITERATOR_H