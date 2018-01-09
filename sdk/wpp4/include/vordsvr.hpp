#ifndef  WPP_VORDSVR_H
#define  WPP_VORDSVR_H

#include <vector>
#include <map>
#include "global.hpp"
#include "vorddef.hpp"
#include "vlang4.h"

#define SYSTEMPREDSIZE  0x1000

#define CACHEBITS 10
#define INDEX(x,y) (((x<<5) - x + (y<<5) + (y<<4) - y) & ((1<<CACHEBITS) -1)) //(31x+47y) % (1<<16)
#define INDEXVALUE(x,y) ((x<<4) + (x<<1) + 349*y ) //18x + 349y;
#define INDEXCALCY(h,x) ((h - (x<<4) - (x<<1))/349)

#define PAGESIZE 0x100000
#define PAGEINDEXSIZE 0x8000

using namespace vord;

class vordgroup_t
{
public:
	vordgroup_t();
	~vordgroup_t();
private:
	void ** pageIndex_;
	int_t						freelist_ ;
	std::vector<info_t>*	list_;
	int							itemfreelist_ ;
public:
	int  alloc_vord();
	info_t* get_vordinfo(int vord);
	void init_vordinfo(info_t* info);
	void free_vord(int  vord);
	void free_item(int  item);
	int  alloc_item();
	item_t * get_vorditem(int index);
private:
	int item_count_;
	int vord_count_;
	//未使用项的起始项
	int item_unused_; 
	int vord_unused_;
//type manager
private:
	std::map<int,int>* _pred_types_;
	void set_predtype(int pred, int ty);
	int  get_predtype(int pred);
//cache manager
public:
	void cache_add(int x,int y, int v);
	void cache_remove(int x,int y);
	int	 cache_get(int x,int y);
	int  cache_get1(int x,int y);
	//calc prime
	void calcP(int n,int p[]);
private:
	int count_ ;
	int count1_ ;
private:
	int* cache_;
	int* cache1_;
};

class VLANG4_API vordsvr_t
{
public:
	vordsvr_t();
	~vordsvr_t();
private:
	std::vector<vordgroup_t*>*		grouplist_ ;
private:
	std::vector<int_t>* freelist_;
public:
	int askgroup();
	int freegroup(int groupid);
public:
	vordgroup_t* group(int groupid);
};

#endif
