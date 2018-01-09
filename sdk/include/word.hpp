#ifndef WPP_WORD_H
#define WPP_WORD_H

#include <vector>
#include <stack>
#include <deque>
#include <math.h>
#include <assert.h>

using namespace std;

#include "vlang3.h"
#include "hashString.hpp"
#include "worditerator.hpp"

namespace wpp  {
namespace how  {

#define SYSTEMPREDSIZE  0x1000

#define CACHEBITS 10
#define INDEX(x,y) (((x<<5) - x + (y<<5) + (y<<4) - y) & ((1<<CACHEBITS) -1)) //(31x+47y) % (1<<16)
#define INDEXVALUE(x,y) ((x<<4) + (x<<1) + 349*y ) //18x + 349y;
#define INDEXCALCY(h,x) ((h - (x<<4) - (x<<1))/349)

#define PAGESIZE 0x100000
#define PAGEINDEXSIZE 0x8000*0x100

struct CWordIndex
{
	int first;
	int last;
};

struct CWord;
struct VLANG3_API CWordValue
{
	const static CWordValue * nil;
	union
	{
		int val;
		double dval;
		void* ptr;
		wchar_t* sval;
		unsigned int uval;
		__int64 val64;
		unsigned __int64 uval64;
		struct
		{
			int hval;
			int lval;
		};
		CWordIndex wi;
	};

	CWordValue():hval(0),lval(0){}
	CWordValue(int v):val(v){}
	CWordValue(unsigned int v):uval(v){}
	CWordValue(__int64 v):val64(v){}
	CWordValue(unsigned __int64 v):uval64(v){}
	CWordValue(double v):dval(v){}
	CWordValue(float v):dval(v){}
	CWordValue(CWordIndex v):wi(v){}
	CWordValue(void* v):ptr(v){}
	CWordValue(wchar_t* v):sval(v){}
	CWordValue(const wchar_t* v):sval((wchar_t*)v){}
	CWordValue(CWord & v);
	bool isnil(CWordValue & v){ return &v ==nil? true:false;}
	bool isnil(){return this == nil? true:false;}
};

extern CWordValue NilWordValue;

struct CWordItem
{
	int h; //hash
	int next;
	CWordValue v;
};

class WordSvr
{
public:
	int count ;
	int count1 ;
private:
	void ** pageIndex;
	int itemCount;

private:
	int* cache;
	int* cache1;

public:
	CHashString hashString;

public:
	WordSvr();
	~WordSvr();
	void add(int x,int y, int v);
	int get(int x,int y);
	int get1(int x,int y);
	void calcP(int n,int p[]);
	int  allocItem();
	 CWordItem * get_word_item(int nIndex);
};

extern WordSvr	wordSvr;

struct VLANG3_API CWord
{
public:
	CWordIndex wIndex;
public:
	typedef WordIterator<CWordItem> iterator;
public:
	iterator begin(int pred = 0);
	iterator begin(wchar_t* pred);
	iterator end(int pred = 0);
public:
	CWord();
	CWord(int first,int last);
	CWord(wchar_t* tp);
	CWord(int tp);

	CWord(wchar_t* tp,wchar_t* val);
	CWord(CWord::iterator &iter);
	CWord(CWordIndex &wi);
	bool isNil();
	static int  allocItem();
	static CWordItem * get_word_item(int nIndex); 
	int get_index() const;
	CWord::iterator get_items(CWord::iterator iter);
	CWord::iterator get_items();
	CWord& append_child(CWord &child);
	CWord& append(wchar_t* pred,CWordValue  val);
	CWord& append(int pred,CWordValue  val,int & nIndex);
	CWord& append(int pred,CWordValue  val);
	CWord& append(int pred,CWord& w);
	CWord& append(CWord& w);

	int find_pred_index(int nFrom, int pred = 0) const;
	int next_index(wchar_t * pred = 0,int nIndex = 0) const;
	int next_index(int pred = 0,int nIndex = 0) const;
	const CWordValue & get_item(int nIndex,int pred) const;
	const CWordValue & get_item(wchar_t* pred) const;
	const CWordValue & get_item(int pred) const;
	void set_item(int nIndex,int pred,CWordValue & v);
	void set_item(wchar_t* pred,CWordValue & v);
	void set_item(int pred,CWordValue & v);
	static wchar_t* get_pred_string(int pred );
	static int get_string_pred(wchar_t* pstr );
	static int get_item_pred(CWordItem *item, int nWord);
	int get_item_pred(int h) ;
	int get_item_pred(CWordItem *item) const;
	CWord  clone();

	void print(CWord & w);
	CWord & clone(CWord & w);

	CWord &  CWord::operator << (CWord &w);
	CWord  operator + (CWord &w);
	const CWordValue&  operator[](int pred) const;
	const CWordValue&  operator[](wchar_t* pred) const;

	CWordValue& operator[](int pred);
	CWordValue& operator[](wchar_t* pred);
};

extern VLANG3_API CWord T(int pred,CWordValue v);
extern VLANG3_API CWord T(wchar_t* pred,CWordValue v);
extern VLANG3_API CWord T(CWord & w1);
extern VLANG3_API CWord T(CWord & w1,CWord & w2);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3,CWord & w4);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3,CWord & w4,CWord & w5);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3,CWord & w4,CWord & w5,CWord & w6);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3,CWord & w4,CWord & w5,CWord & w6,CWord & w7);
extern VLANG3_API CWord T(CWord & w1,CWord & w2,CWord & w3,CWord & w4,CWord & w5,CWord & w6,CWord & w7,CWord & w8);

}} //namespace wpp::how

#endif //WPP_WORD_H
