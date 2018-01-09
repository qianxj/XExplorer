#pragma once

#include "../lexer/lexer.hpp"
#include "vldef.hpp"
#include "vlutils.hpp"
#include <assert.h>
#include "vlblockmgr.hpp"
#include "vlock.hpp"

namespace vl
{
	/*
		除了得特定的内存外,所有的内存都以页为单位,按一定的大小,分割成多块内存。
	*/
	const int vPageBitSize = 13; 
	const int vPageSize = 1<< vPageBitSize;//8k

	//define page for what used
	enum vPageClass : int
	{
		pcMetaPage	= 0x00,	//page for meta page
		pcPage32	= 0x01,	//page divide to 32 byte block
		pcPage64	= 0x02,	//page divide to 64 byte block
		pcPage128	= 0x03, //page divide to 128 byte block
		pcPage256	= 0x04, //page divide to 256 byte block
		pcPage512	= 0x05, //page divide to 512 byte block
		pcPage1024	= 0x06, //page divide to 1024 byte block
		pcPage2048	= 0x07, //page divide to 2048 byte block
		pcPage4096	= 0x08, //page divide to 4096 byte block
		pcPage8192	= 0x09, //page divide to 8192 byte block
		pcPageLarge = 0x0a, //page which size > 8K 
		pcPageCount = 0x0b
	};

	//for page
	struct vlPage 
	{
		//one page have vPageSize byte data
		unsigned char data[vPageSize];
	};

	struct vPageMetaBlock;
	struct vlPageSetItem
	{
		vPageMetaBlock* pBlock;
	};

	const int vPageSetSize = 1024;
	const int vPageSetMaskCount = 25;
	const int vPageSetCountPerPage (vPageSize/vPageSetSize);
	
	struct vlPageSet;
	struct vlPageSetHeader
	{
		vlPageSet* next;		//pointer next set
		vlPageSet* prev;		//pointer prev set

		int size;		//size meta item 
		int count;		//used count
		int mask[vPageSetMaskCount];	//mask owner have used
		int maskMetaBlock[vPageSetMaskCount];//mask middle have used
		int maskContentBlock[vPageSetMaskCount];//mask last have used
	};

	const int vPageSetItemHeaderSize = sizeof(vlPageSetHeader);
	const int vPageSetItemSize = sizeof(vlPageSetItem);
	const int vPageSetItemCount = ((vPageSetSize - vPageSetItemHeaderSize)/vPageSetItemSize);
	const int vPageSetMaskBitCount =  vPageSetItemCount;
	const int vPageSetNoUsedSize = (vPageSetSize - vPageSetItemCount*vPageSetItemSize - vPageSetItemHeaderSize);
	//
	struct vlPageSet : vlPageSetHeader 
	{
		vlPageSetItem items[vPageSetItemCount]; //pointer to meta items , length by size
		//unsigned char NoUsed[vPageSetNoUsedSize];
	};

	template <class vlblock, class vlblockitem>
	class VL_API vlPageSetMgr: public vlmaskblockmgr<vlblock,vlblockitem>
	{
	public:
		//get item count in block is mask bit used now
		inline int getItemUsedCount(vlblock* pblock){return pblock->count;}
		//get item count in block now
		inline int getItemCount(vlblock* pblock){return pblock->size;}
		//get block item count
		inline int getBlockItemCount(vlblock* pblock){return vPageSetItemCount;}
	public:
		//get next block
		inline vlblock * getNextBlock(vlblock * pblock){return pblock->next;}
		
		//get block item by index
		inline vlblockitem * getItem(vlblock * pblock, int nItem){return pblock->items + nItem;}
	public:
		//push block item
		inline int pushItem(vlblock * pblock,void * item)
		{
			//查找空闲的块
			int nItem = getNotUsedItem(pblock);
			
			//使用nItem指定的块
			this->setItemUsed(pblock,nItem);
			(pblock->items + nItem)->pBlock = (vPageMetaBlock *)item;
			pblock->count++;
			if(nItem>=pblock->size) pblock->size++;

			//设置关联的标记
			this->SetItemMaskBit(pblock->maskMetaBlock,getBlockMaskBitCount(pblock),nItem,false); //有空
			this->SetItemMaskBit(pblock->maskContentBlock,getBlockMaskBitCount(pblock),nItem,true); //无空

			return nItem;
		}
	public:
		//get block mask pointer
		inline int * getBlockMask(vlblock* pblock){return pblock->mask;}
		//get block mask bit count
		inline int getBlockMaskBitCount(vlblock* pblock){return vPageSetMaskCount*vBitsPerByte*vIntSize;}
	public:
		inline int getFreeItem(vlblock* pblock,int nLevel = -1)
		{
			if(nLevel==-1)
			{
				//判断末级是否有空
				return  vlmaskblockmgr::getFirstBit(pblock->maskContentBlock,pblock->mask,getBlockMaskBitCount(pblock));
			}else if(nLevel==1)
			{
				//判断中间是否有空
				return  vlmaskblockmgr::getFirstBit(pblock->maskMetaBlock,pblock->mask,getBlockMaskBitCount(pblock));
			}else if(nLevel==0)
			{
				//判断当前是否有空
				return getNotUsedItem(pblock);
			}
			return -1;
		}
	};

	//define page meta item
	struct vPageMetaItem
	{
		void* pPage;	//page index to pointer vlPageSet page item
		int mask[2];	//mask page block is used or not used, but for page32 used first 256 byte of page for page64 used first 128 byte
	};

	const int vPageMetaItemSize = sizeof(vPageMetaItem);
	const int vPageMetaBlockItemCount = 128;

	//define page meta block
	struct vPageMetaBlock
	{
		vPageMetaBlock* next;		//pointer next meta block
		vPageMetaBlock* prev;		//pointer prev meta block
		int size;		//size meta item <=128 , include free item
		int count;		//count meta item <=128, not include free item
		int mask[4];	//mask owner have used
		int maskContentBlock[4];//mask last have used
		vPageMetaItem items[vPageMetaBlockItemCount]; //pointer to meta items , length by size
	};

	const int vPageMetaBlockSize = sizeof(vPageMetaBlock);
	const int vPageMetaBlockCountPerPage = (vPageSize/vPageMetaBlockSize);

	template <class vlblock, class vlblockitem>
	class VL_API vlPageMetaBlockMgr: public vlmaskblockmgr<vlblock,vlblockitem>
	{
	public:
		//get item count in block is mask bit used now
		inline int getItemUsedCount(vlblock* pblock){return pblock->count;}
		//get item count in block now
		inline int getItemCount(vlblock* pblock){return pblock->size;}
		//get block item count
		inline int getBlockItemCount(vlblock* pblock){return vPageMetaBlockItemCount;}
	public:
		//get next block
		inline vlblock * getNextBlock(vlblock * pblock){return pblock->next;}
		
		//get block item by index
		inline vlblockitem * getItem(vlblock * pblock, int nItem){return pblock->items + nItem;}
	public:
		//get block mask pointer
		inline int * getBlockMask(vlblock* pblock){return pblock->mask;}
		//get block mask bit count
		inline int getBlockMaskBitCount(vlblock* pblock){return (sizeof(pblock->mask)*vBitsPerByte);}
	public:
		//push block item
		inline int pushItem(vlblock * pblock,void * item)
		{
			//查找空闲的块
			int nItem = getNotUsedItem(pblock);
			
			//使用nItem指定的块
			this->setItemUsed(pblock,nItem);
			(pblock->items + nItem)->pPage = item;
			(pblock->items + nItem)->mask[0] = 0;
			(pblock->items + nItem)->mask[1] = 0;
			pblock->count++;
			if(nItem>=pblock->size) pblock->size++;

			//设置关联的标记
			this->SetItemMaskBit(pblock->maskContentBlock,getBlockMaskBitCount(pblock),nItem,false); //无空
			if(this->isBlockFullUsed(pblock))
			{
				//需要设置前级索引标记为1,但在此处没有办法操作,因此调用后需要立即设置
			}
			return nItem;
		}
	public:
		inline int getFreeItem(vlblock* pblock,int nLevel = -1)
		{
			if(nLevel==-1)
			{
				//判断末级是否有空
				return  vlmaskblockmgr::getFirstBit(pblock->maskContentBlock,pblock->mask,getBlockMaskBitCount(pblock));
			}else if(nLevel==0)
			{
				//判断当前是否有空
				return getNotUsedItem(pblock);
			}
			return -1;
		}
	};

	class spaceMgr;
	template <class T>
	struct vptr
	{
		T * pt_;
		int val_;
	};

	//define space manage
	class VL_API spaceMgr
	{
	private:
		vlock lock_;
	public:
		spaceMgr();
		~spaceMgr();
	public:
		vlPageSetMgr<vlPageSet,vlPageSetItem>* pPageSetMgr;
		vlPageMetaBlockMgr<vPageMetaBlock,vPageMetaItem>*	pPageMetaBlockMgr;
	public:
		vlPage* pPageSetSolt[32];
		vlPage* pPageMetaBlockSolt[512];

		int PageSetSoltSize; //this is should <32
		int PageMetaBlockSoltSize; //this is should < 512
		int PageMetaBlockCount;
	public:
		vlPageSet* ptPageSet[pcPageCount];
		int	nPageSetCount[pcPageCount];
	private:
		vlPageSet* ptHitedPageSet[pcPageCount];
		vPageMetaBlock* ptHitedBlock[pcPageCount];
		int	nHitedSetItem[pcPageCount];
	public:
		vlPageSet* allocRawPageSet();
		vlPageSet* allocPageSet(vPageClass pc);
		vPageMetaBlock* allocMetaBlock(vPageClass pc); //alloc meta block
	private:
		//first use last meta block , should continue data
		//when last meta block no free space , find from first meta block
		//if all meta block no free space, return null
		//but when pc = pcPageLarge then direct alloc block from system
		vPageMetaBlock* getFreeBlock(vPageClass pc ,vlPageSet* & pSet,int& nItemSet, int & item ,int nLevel = -1 );
		vlPageSet* getFreePageSet(vPageClass pc ,int & item ,int nLevel = -1 );
		
		vPageMetaBlock* getNotUsedBlock(vPageClass pc ,int & item );
		vlPageSet* getNotUsedPageSet(vPageClass pc ,int & item );
	public:
		//alloc space by size
		int allocSpace(int sz);
		//free space
		int freeSpace(int h);
	public:
		void* allocPage(vPageClass pc); //alloc one page used for spec page class
		//alloc one page and zero of it
		static void * allocRawPage();
		static void * allocRaw(int sz);
		static void freeRaw(void * p);
	private:
		int pushPageSetItem(vlPageSet * pSet,void* item); //append one page set item for new page
		int pushMetaItem(vlPageSet * pSet,int nItemSet, vPageMetaBlock* pBlock,void* item); //append one page meta item for new page
	private:
		static vPageClass getWhichPageClass(int sz);
	private:
		//判断pn中前bits指定的位是否全部为零
		bool isMetaItemNotUsed(int pn[], int bits);
	private:
		int  pageSetItemIndex(vlPageSet * pSet,vPageClass pc);
		vlPageSet *  getPageSetByIndex(vPageClass pc,int nIndex, int &nItemSet);
	private:
		void * handleToPtr(int h);
	private:
		int nHitedHandle;
		void * ptHitedPtr;
	public:
		template<class T>
		T * handleTo(int h)
		{
			return (T*)h;
			/*if(h==nHitedHandle)
				return (T*)ptHitedPtr;
			else 
				return (T *)handleToPtr(h);*/
		}
	};
}