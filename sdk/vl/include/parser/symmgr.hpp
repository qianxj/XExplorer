/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

#pragma once

#include "../lexer/lexer.hpp"
#include <vector>
#include <string>
#include <map>
#include "vldef.hpp"
//#include "vlspacemgr.hpp"

using namespace std;

#define STR_BUCKET_SIZE 4096
namespace vl
{
#define	Itemnull -7
#define	ItemSeqEnd -6
#define	ItemSeqStart -5
#define	ItemEnd -4
#define	ItemStart -3
#define	ItemSp	-2
#define ItemStop -1

	//token item
	struct tokenitem
	{
		tchar* text;
		tokenitem * next;
	};


	//alloc array token
	struct tokenalloc
	{
	public:
		tokenalloc();
		~tokenalloc();
	public:
		tokenitem* alloc();
	private:
		tokenitem* pitems;
	private:
		int used;
		int size;
	};

	//token hash table when match then create list
	class tokentab
	{
	public:
		tokentab();
		~tokentab();
	private:
		int bucketsize;
		tokenitem ** items;
	private:
		tokenalloc * allocer;
	private:
		int hash(tchar* str);
	public:
		tokenitem* addtoken(tchar* str);
		tokenitem* findtoken(tchar* str);
	};

#define SpacePageSize 0x2000
#define SpacePageLinkPointerSize (SpacePageSize/sizeof(void*))
#define SpacePagePointerSize (SpacePageSize/sizeof(pageMeta))
#define SpaceMetaArrayBlockSize 0x200
#define SpaceMetaArrayListBlockSize 0x20

#define SpacePageLinkIndex(x)	((x - 1)/SpacePagePointerSize + 64/sizeof(void*))
#define SpacePageLinkOffset(x)	((x - 1)/x % SpacePagePointerSize)

#define SpacePageLinkPointer(x)		 m_pAllocList[SpacePageLinkIndex(x - 1)]
#define SpacePageLinkPagePointer(x) ((pageMeta *)(m_pAllocList[SpacePageLinkIndex(x - 1)])) + SpacePageLinkOffset(x - 1)

#define	SpaceArrayFlagInt		0x00000000
#define	SpaceArrayFlagDouble	0x00000001
#define	SpaceArrayFlagString	0x00000002
#define	SpaceArrayFlagBlock		0x00000003
#define	SpaceArrayFlagPointer	0x00000004

#define SpaceArrayFlagIndex		0x00000010
#define SpaceArrayFlagHash		0x00000020
#define SpaceArrayFlagSparse	0x00000040
#define SpaceArrayFlagCompress	0x00000080

	class VL_API SpaceMgr
	{
	private:
		//alloc list of page pointer index
		void*** m_pAllocList;
		//length of alloc list
		int	m_nAllocCount;
		//count of page
		int m_nPageCount;
	private:
		//alloc memory page
		void* allocPage(int nPage = 1 );
		//calc addr by index
		void * GetPointerByIndex(int nPage, int nIndex);
	private:
		bool InitMetaArray();
		void* AskSpace(int nSize,int &nPage,int &nOffset);
	private:
		struct ArrayMeta 
		{
			int nSize;
			int nBlockSize;
			int nElementSize;
			int nFlag;
			int nPtList;
			int nPage;
			int nOffset;
		};


		enum ePageClass : int
		{
			pcMetaPage	= 0x00,
			pcPage32	= 0x01,
			pcPage64	= 0x02,
			pcPage128	= 0x03,
			pcPage256	= 0x04,
			pcPage512	= 0x05,
			pcPage1024	= 0x06,
			pcPage2048	= 0x07,
			pcPage4096	= 0x08,
			pcPage8192	= 0x09,
			pcPageLarge = 0x0a
		};
		
		struct allocHeader
		{
			int  next;	//this should zero
			int  ptMetaPage;
			int  ptPage32;
			int  ptPage64;
			int  ptPage128;
			int  ptPage256;
			int  ptPage512;
			int  ptPage1024;
			int  ptPage2048;
			int  ptPage4096;
			int  ptPage8192;
			int  ptPageLarge;
			int  notUsed[4];
		};

		struct pageMeta
		{
			void * p;
			ePageClass pc;
			int size;
			int	next;
			int last;
			unsigned char mask[8];
			unsigned char v[8 - sizeof(void*)];
		};

	private:
		//alloc memory page
		unsigned int allocPage(ePageClass pc); //index from one
		//alloc size space
		unsigned int allocSpace(int size);//index from one
		//free block
		unsigned int freeSpace(int block);//index from one
	public:
		SpaceMgr();
		~SpaceMgr();
	private:
		void * getAddr(int cid,int nIndex);
	public:
		int askCID(int flag = SpaceArrayFlagInt,int nElement = 1,int blockSize = 32);
		int getSize(int cid);
		
		int push(int cid,int val);
		
		int getValue(int cid,int nIndex);
		int getValue(int cid,int nIndex,int nCol);

		int setValue(int cid,int nIndex,int val);
		int setValue(int cid,int nIndex,int nCol, int val);

		int askStringCID(int blockSize = 32);
		int push(int cid,tchar * val);
		const tchar * getString(int cid, int nIndex);
		int setString(int cid, int nIndex,tchar * val);
	};


#define MArrayInc	5
#define MArrayMask	0x0003
#define MArrayShift	0x0002
	class marray
	{
	private:
		vector<int>    data;
	public:
		int askCID();	//分配并返回类别ID
		int push(int cid,int val);
		int getID(int cid,int nIndex);
		int setID(int cid,int nIndex,int val);
		int getSize(int cid);
	};

	namespace vnodeType {
		enum _
		{
			#define vtnode(x,y) x,
			#include "../vl/nodetype.hpp"
			#undef	vtnode(x,y)
			vtCount
		};
	};

	class unit
	{

	public:
		// int      Unitid;
		tchar * Unitname;
		int     level_id;

		//map<tchar *, int> m_tokens;

		vector<tchar *> token_name;
		vector<int>     token_pid;
		vector<int>		token_cid;
		marray			token_ref;

		vector<int>  prep_id; //所有的谓语
		vector<int>  subob_id; //subjects_objects_id[] 1,2,5,-1,6,8,9,-1   //主语 \宾语从表


		vector<int>     row_subjects_id;
		vector<int>     row_prep_id;   //指向 prep_id
		vector<int>     row_objects_id;

	public:
		unit(void);
		~unit(void);
	public:
		int SetUnit(tchar* name);
		int FindToken(tchar* tk_name);
		int AddToken(tchar* tk_name);
		int FindPrepid(int prepid);
		int FindPrepStr(tchar* prep_name);
		int AddPrepid(int prepid);
		int AddPrepstr( tchar* prep_name);
		int GetStartIndex();
		int AddSubObj(tchar* name);
		int AddSubItem(int index);
		int AddSubObjEnd();
		int AddStmt(int subject,int pred,int object,bool addRef=false);

	public:
		int AddTokenRef(int tokenId,int rowid);
		int AddTokenRef(tchar* name,int rowid);
		int AddRowTokenRef(int nRow);
	public:

	public:
		//get token name
		tchar * GetToken(int tokenid);
	public:
		//get subject index by stmt index
		int GetIndexOfSubject(int nIndex);
		//get pred index by stmt index
		int GetIndexOfPred(int nIndex);
		//get object index by stmt index
		int GetIndexOfObject(int nIndex);
	public:
		//根据句型索引得到tokenid
		int GetTokenId(int nIndex, bool bPred);
	public:
		bool IsBookMark(tchar * name);
	public:
		int	GetTokenUsedIndex(int nTokenId);
		int GetTokenUsedNextIndex(int nTokenId,int nIndex);
	public:
		int GetTokenUsedRow(int nTokenId,int nIndex);
	public:
		int GetBookMarkStmtRow(int nBookMark,int nRowAfter = -1);
	public:
		int GetHaveSeqRow(int nTokenId,int nRowAfter = -1);
		int GetPredRow(int nTokenId,int nPred,int nRowAfter = -1);
	};

	class VL_API symmgr
	{
	public:
		symmgr();
	public:
		vector<unit *> m_pSysUnit;
		vector<unit *> m_pUnit;
	public:
		int FindUnit(tchar *name,bool bsystem = false);
		int AddUnit(tchar *name, bool bsystem = false);
		unit * GetUnit(int index,bool bsystem = false);
	public:
		int initial();
	public:
		int GetTokenId(vnodeType::_ t);
	public:
		int print(wstring &ws);
	};
};