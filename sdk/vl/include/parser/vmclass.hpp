#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

namespace vl
{
	class vArrayMgr;
	class vmclass
	{
	public:
		static int makeType(vArrayMgr* v,const tchar* val=null);
		static int getTypeSize(vArrayMgr* v,int artype);
		static int getColCount(vArrayMgr* v,int artype);
		static int getColOffset(vArrayMgr* v,int artype,int ncol);
		static int getColType(vArrayMgr* v,int artype,int ncol);
	public:
		static int freeType(vArrayMgr* v,int artype);
	public:
		static int makeClass(vArrayMgr* v ,int ty);
		static void freeClass(vArrayMgr* v, int nc);
	public:
		static int makeArrayClass(vArrayMgr* v ,int ty);
	public:
		static int pushTypeItem(vArrayMgr* v,int artype,int ntype,const tchar* val=null);
	public:
		static int setValue(vArrayMgr* v,int nc,int ncol, int val);
		static int setValue(vArrayMgr* v,int nc,int ncol, const tchar * val);
		static int setValue(vArrayMgr* v,int nc,int ncol, double val);
		static int setValue(vArrayMgr* v,int nc,int nrow,int ncol, int val);
		static int setValue(vArrayMgr* v,int nc,int nrow,int ncol, const tchar * val);
		static int setValue(vArrayMgr* v,int nc,int nrow,int ncol, double val);

		static int setValue(vArrayMgr* v,int nc,const tchar * col, int val);
		static int setValue(vArrayMgr* v,int nc,const tchar * col, const tchar * val);
		static int setValue(vArrayMgr* v,int nc,const tchar * col, double val);
		static int setValue(vArrayMgr* v,int nc, int nrow, const tchar * col, int val);
		static int setValue(vArrayMgr* v,int nc, int nrow, const tchar * col, const tchar * val);
		static int setValue(vArrayMgr* v,int nc, int nrow, const tchar * col, double val);
	public:
		static int getValue(vArrayMgr* v,int nc,int ncol);
		static const tchar * getString(vArrayMgr* v,int nc,int ncol);
		static double getDouble(vArrayMgr* v,int nc,int ncol);
		static int getValue(vArrayMgr* v,int nrow, int nc,int ncol);
		static const tchar * getString(vArrayMgr* v,int nrow, int nc,int ncol);
		static double getDouble(vArrayMgr* v,int nrow, int nc,int ncol);

		static int getValue(vArrayMgr* v,int nc,const tchar * col);
		static const tchar * getString(vArrayMgr* v,int nc,const tchar * col);
		static double getDouble(vArrayMgr* v,int nc,const tchar * col);
		static int getValue(vArrayMgr* v, int nc,int nrow,const tchar * col);
		static const tchar * getString(vArrayMgr* v, int nc,int nrow,const tchar * col);
		static double getDouble(vArrayMgr* v, int nc,int nrow,const tchar * col);
	public:
		static int getColIndex(vArrayMgr* v,int typ,const tchar* colname);
	public:
		static int trace(vArrayMgr* v,int tr, fnFetchData fn,void* param);
		static int trace(vArrayMgr* v,int tr,int nrow , fnFetchData fn,void* param);
	public:
		static int test(vArrayMgr* v,fnFetchData fn,void* param);
	};
}