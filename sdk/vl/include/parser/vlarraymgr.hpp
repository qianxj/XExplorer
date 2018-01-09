#pragma once

//#include "vlspacemgr.hpp"
#include "../vl/vlmemory.hpp"
#include "../vl.h"
#include "vldef.hpp"
#include "vlutils.hpp"
#include <assert.h>

namespace vl
{
	//define array data block , array is array of vArrayDataBlock
	struct vArrayDataBlock
	{
		int next;	//pointer next block
		int prev;	//pointer prev block
		int large;	//pointer large next block
		int size;	//size of element in block
		int	data[1];//data array size is nBlockSize of vArrayMetaItem
	};

	const int vArrayDataBlockSize = (sizeof(vArrayDataBlock) - sizeof(int));

	//define mtable data block,this block used for insert or delete
	struct vArrayMTableBlock
	{
		int ptStart;		//pointer start block
		int size;			//count of element
		int blockSize;		//data block size
		int arFlag;			//array flag
		int nElement;		//size of element for int
		int nColumn;		//column size of one element
		int ptLast;			//pointer end block
		int largeRefNumber;	//number of vBlockLargeNumber
		int typeRef;		//ref of typedef,when free array should free typedef 
		int colType;		//when typeref=0 use this for type define or array col use typedef

		vArrayDataBlock* ptHited;		//pointer hited block
		int ptHitedIndex;				//index of hited index
	};

	const int  vArrayMTableBlockSize = sizeof(vArrayMTableBlock);

	const int	vArrayFlagInt	   =	0x00000000;	//int array
	const int	vArrayFlagDouble   =	0x00000001; //double array
	const int	vArrayFlagString   =	0x00000002; //string array
	const int	vArrayFlagBlock	   =	0x00000003; //block array
	const int	vArrayFlagPointer  =	0x00000004; //pointer array

	const int	vArrayFlagIndex	   =	0x00000010; //array data list have index array 
	const int	vArrayFlagHash	   =	0x00000020; //hash array
	const int	vArrayFlagSparse   =	0x00000040; //sparse array
	const int	vArrayFlagCompress =	0x00000080; //compress array

	class VL_API vArrayMgr
	{
	public:
		vArrayMgr();
		~vArrayMgr();
	public:
		int allocArray(int flag  = vArrayFlagInt,int nElement = 1,int blockSize = 32); //alloc array
	public:
		vArrayMTableBlock * getMetaBlock(int cid);
	private:
		vArrayDataBlock* getBlock(int cid,int nIndex);
		int getColOffset(int cid,int nCol);
		vArrayDataBlock* allocDataBlock(int cid);
	public:
		int allocString(const tchar *p);
	public:
		const tchar* getString(int h);
	private:
		int hitedId;
		vArrayMTableBlock * hitedMetaBlock;
	public:
		//get array size
		int getSize(int cid);
		//set array size
		void setSize(int cid,int cz);
		//popup back
		int pop(int cid);
		//find item
		int find(int cid,int item);
		int find(int cid,int col,int item);
		int find(int cid,int srow, int col,int item);
		
		//find char item
		int find(int cid,const tchar* item);
		int find(int cid,int col,const tchar* item);
		int find(int cid,int srow, int col,const tchar* item);

		//get column count
		int getColumnCount(int cid);
		//get element type
		int getType(int cid);
		//get elementsize
		int getElementSize(int cid);
		//get large size
		int getLargeSize(int cid);

		//push array value
		int push(int cid,int val);

		int * getPtr(int cid, int nIndex);
		//get array value
		int getValue(int cid,int nIndex);
		//get array value by index and col
		int getValue(int cid,int nIndex,int nCol);

		//set array value
		int setValue(int cid,int nIndex,int val);
		//set array value by index and col
		int setValue(int cid,int nIndex,int nCol, int val);

	public:
		//string operator
		int push(int cid,const tchar * val);
		const tchar * getString(int cid, int nIndex);
		const tchar * getString(int cid,int nCol, int nIndex);
		int setString(int cid, int nIndex,const tchar * val);
		int setString(int cid, int nIndex,int nCol,const tchar * val);

	private:
		int hash(const tchar* val,int bucketsize);
	public:
		//hash string operator
		int allocHashArray(int bucketSize,int nElement=1);
		int hashPush(int cid,const tchar * val);
		int hashFind(int cid,const tchar * val);
	public:
		int setArrayType(int cid,int ntype,int nref = 0 );
		int getAarryType(int cid);
		int getArrayTypeRef(int cid);
	};
}