#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

namespace vl
{
	class vArrayMgr;
	class VL_API vmtuple
	{
	public:
		static int makeTuple(vArrayMgr* v);
		static int freeTuple(vArrayMgr* v,int tuple);
	public:
		static int makeArrayType(vArrayMgr* v);
		static int makeArrayByType(vArrayMgr* v,int arType);
	public:
		static int pushType(vArrayMgr* v,int artype,int ntype,const tchar* val=null); //push array struct type
	public:
		//expand tuple
		static int push(vArrayMgr* v,int tuple, int ntype, int val);
		static int push(vArrayMgr* v,int tuple, int ntype, const tchar* val);
		static int push(vArrayMgr* v,int tuple, int ntype, void * val);
		static int push(vArrayMgr* v,int tuple, int ntype, bool val);
		static int push(vArrayMgr* v,int tuple, int ntype, double val);
		//pop
		static int pop(vArrayMgr* v,int tuple);

		//pop header
		static int pop_header(vArrayMgr* v,int tuple);

		//get item value
		static int getItem(vArrayMgr* v,int tuple,int index ,int & ntype);
		static int getItem(vArrayMgr* v,int tuple,int index);
		static double getItemDouble(vArrayMgr* v,int tuple,int index);
		static const tchar* getItemString(vArrayMgr* v,int tuple,int index);
		static int getItemType(vArrayMgr* v,int tuple,int index);
		static int setItemType(vArrayMgr* v,int tuple,int index,int ntype);

		//set item value
		static int setItem(vArrayMgr* v,int tuple, int index, int ntype, int val);
		static int setItem(vArrayMgr* v,int tuple, int index, int ntype, const tchar* val);
		static int setItem(vArrayMgr* v,int tuple, int index, int ntype, void * val);
		static int setItem(vArrayMgr* v,int tuple, int index, int ntype, bool val);
		static int setItem(vArrayMgr* v,int tuple, int index, int ntype, double val);

		//insert item
		static int insertItem(vArrayMgr* v,int tuple, int index, int ntype, int val);
		static int insertItem(vArrayMgr* v,int tuple, int index, int ntype, const tchar* val);
		static int insertItem(vArrayMgr* v,int tuple, int index, int ntype, void * val);
		static int insertItem(vArrayMgr* v,int tuple, int index, int ntype, bool val);
		static int insertItem(vArrayMgr* v,int tuple, int index, int ntype, double val);

		//erase item
		static int eraseItem(vArrayMgr* v,int tuple, int index);
	public:
		static int setItemName(vArrayMgr* v,int tuple, int index, const tchar * p);
		static int setItemName(vArrayMgr* v,int tuple, int index, int p);
		static const tchar * getItemName(vArrayMgr* v,int tuple, int index);
		static int findItemName(vArrayMgr* v,int tuple, const tchar * p);
	public:
		static int getLength(vArrayMgr* v,int tuple);
	public:
		static int trace(vArrayMgr* v,int tuple, fnFetchData fn,void* param);
	};
}