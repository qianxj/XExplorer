#ifndef WPP_HASHCONTEXT_H
#define WPP_HASHCONTEXT_H

#include "vlang3.h"
#include "hashstring.hpp"
#include "word.hpp"

#include <stack>
#include <vector>
#include <deque>
using namespace std;


namespace wpp {
namespace how {

struct VLANG3_API CHashContextItem
{
	CWord nWord;
	int ide;
	int next;
	int piror; //when <0 point to _bucket >0 point to _items
};

class VLANG3_API CHashContext
{
public:
	CHashContext(void);
	~CHashContext(void);
 
#pragma warning( push )
#pragma warning( disable : 4251 )
private:

	//indexs in current scope block
	stack<int> _currents;
	//count indexs in current scope block
	stack<int> _countCurrents;
	//words used last
	//vector<int>	_hots;

private:
	int _bucketsize;
private:
	int _count;
	vector<int> _bucket;
	deque<CHashContextItem> _items;
	//freelist
	stack<int> _freelist;
#pragma warning( pop ) 

private:
	CHashString* _hashString;
public:
	void set_hash_string(CHashString* hashString);
	CHashString* get_hash_string();
public:
	int get_count();
public:
	int hash_value(const wchar_t* str);
public:
	void enter_scope();
	//leave scope should pop up words in this scope
	void leave_scope();
public:
	int push_word(CWord & w,int ide);
	int push_word(CWord & w,const wchar_t* str);
public:
	 //return index + 1 of _bucket
	 int find_token(const wchar_t* str);
	 //return next word's index + 1 of _bucket
	 int find_next_token(int nIndex,int ide);
	 int find_next_token(int nIndex,const wchar_t* str);
public:
	//get word index;
	CWord get_word(int nIndex);
};

}}// namespace wpp::how


#endif //WPP_HASHCONTEXT_H