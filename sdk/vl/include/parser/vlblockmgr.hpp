#pragma once

#include "../lexer/lexer.hpp"
#include "vldef.hpp"
#include "vlutils.hpp"

namespace vl
{
	//class for block manager
	template <class vlblock, class vlblockitem>
	class vlblockmgr
	{
	public:
		//block have no free item
		virtual bool isBlockFullUsed(vlblock* pblock)=0;
		//block is used?
		virtual bool isBlockUsed(vlblock* pblock)=0;
		//get free item in block
		virtual int getNotUsedItem(vlblock* pblock)=0;
	public:
		//get item count in block is mask bit used now
		virtual int getItemUsedCount(vlblock* pblock)=0;
		//get item count in block now
		virtual int getItemCount(vlblock* pblock)=0;
		//get block item count
		virtual int getBlockItemCount(vlblock* pblock)=0;
	public:
		//clear item mask bit
		virtual void setItemNotUsed(vlblock* pblock, int nItem)=0;
	public:
		//get next block
		virtual vlblock * getNextBlock(vlblock * pblock)=0;
		//get block item by index
		virtual vlblockitem * getItem(vlblock * pblock,int nItem)=0;
	public:
		//push block item
		virtual int pushItem(vlblock * pblock,void * item)=0;
	public:
		/*
		//lock block ,general lock block mask=0;
		virtual void lockBlock(vlblock * pblock)=0;
		//free block
		virtual void unlockBlock(vlblock * pblock)=0;

		//lock item , nitem is index of item in block
		virtual void lockItem(vlblock * pblock,int nItem)=0;
		//free item
		virtual void unlockItem(vlblock * pblock,int nItem)=0;
		*/
	};

	//class for block manager
	template <class vlblock, class vlblockitem>
	class vlmaskblockmgr : public vlblockmgr<vlblock,vlblockitem>
	{
	public:
		//block have no not used item
		virtual bool isBlockFullUsed(vlblock* pblock)
		{
			return isAllUsed(this->getBlockMask(pblock),this->getBlockMaskBitCount(pblock));
		}

		//block is used?
		virtual bool isBlockUsed(vlblock* pblock)
		{
			return !isAllNotUsed(this->getBlockMask(pblock),this->getBlockMaskBitCount(pblock));
		}

		//get not used item in block
		virtual int getNotUsedItem(vlblock* pblock)
		{
			return this->getFirstBit(this->getBlockMask(pblock),this->getBlockMaskBitCount(pblock));
		}
	public:
		//set item mask bit
		virtual void setItemUsed(vlblock* pblock, int nItem)
		{
			SetItemMaskBit(this->getBlockMask(pblock),this->getBlockItemCount(pblock),nItem,true);
		}

		//clear item mask used bit
		virtual void setItemNotUsed(vlblock* pblock, int nItem)
		{
			SetItemMaskBit(this->getBlockMask(pblock),this->getBlockItemCount(pblock),nItem,false);
		}
	public:
		//get block mask bit count
		virtual int getBlockMaskBitCount(vlblock* pblock)=0;
		//get block mask used pointer
		virtual int * getBlockMask(vlblock* pblock)=0;
	public:
		//last position zero bit
		 static int getLastBit(int n)
		{
			if(n==-1)return -1;
			unsigned int m = n^- 1;
			int nbit = 31;
			#define p(x,y) if(!(m & ((1<<x) - 1))){m>>=x; nbit-=x;} y
			p(16,p(8,p(4,p(2,p(1,return nbit)))));
			#undef  p
		}
		//last position zero bit
		 static int getLastBit(int pn[], int bits)
		{
			int nIndex = bits/(vIntSize * 8);
			int noffset = bits % (vIntSize * 8);
			int nbit = -1;
			int ttl = nIndex * vIntSize * 8;
			if(noffset)
			{
				int v = pn[nIndex] | (1<<(vIntSize*8 - noffset)) - 1;
				if(v!=-1) nbit = getLastBit(v);
			}
			while(nbit==-1 && nIndex>0)
			{
				ttl -= vIntSize * 8;
				if(pn[nIndex - 1]!=-1)
					nbit = getLastBit(pn[nIndex - 1]);
				nIndex--;
			}
			return nbit + ttl;
		}

		 static int getLastBit(int pn[],int pn1[], int bits)
		{
			if(!pn1) return getFirstBit(pn,bits);

			int nIndex = bits/(vIntSize * 8);
			int noffset = bits % (vIntSize * 8);
			int nbit = -1;
			int ttl = nIndex * vIntSize * 8;
			if(noffset)
			{
				int v  = pn[nIndex] | (1<<(vIntSize*8 - noffset)) - 1;
				int v1 = pn1[nIndex] | (1<<(vIntSize*8 - noffset)) - 1;
				v |= ~v1;
				if(v!=-1) nbit = getLastBit(v);
			}
			while(nbit==-1 && nIndex>0)
			{
				ttl -= vIntSize * 8;
				if(pn[nIndex - 1]!=-1)
					nbit = getLastBit(pn[nIndex - 1]|~pn1[nIndex - 1]);
				nIndex--;
			}
			return nbit + ttl;
		}

		 //firtst position zero bit index start zero
		 static int getFirstBit(int n)
		 {
			 if(n==-1)return -1;
			 unsigned int m = n^- 1;
			 int nbit = 0;
			 #define p(x,y) if(m & (((1<<x) - 1))<<x) m>>=x; else  nbit+=x; y
				p(16,p(8,p(4,p(2,p(1,return nbit)))));
			 #undef  p
		 }

		 //firtst position zero bit index start zero
		 static int getFirstBit(int pn[],int pn1[], int bits)
		 {
			 if(!pn1) return getFirstBit(pn,bits);
			 int nIndex = bits/(vIntSize * 8);
			 int noffset = bits % (vIntSize * 8);
			 int nbit = -1;
			 int ttl = 0;
			 int i = 0;
			 while(nbit==-1 && i<nIndex)
			 {
				 if(pn[i]!=-1)
					 nbit = getFirstBit(pn[i]|~pn1[i]);
				 else
					 ttl += vIntSize * 8;
				 i++;
			 }
			 if(nbit==-1 && noffset)
			 {
				 int v  = pn[nIndex] | (1<<(vIntSize*8 - noffset)) - 1;
				 int v1 = pn1[nIndex] | (1<<(vIntSize*8 - noffset)) - 1;
				 v |=~v1;
				 if(v!=-1) nbit = getFirstBit(v);
			 }
			 if(nbit==-1) return -1;
			 return nbit + ttl;
		 }

		 //firtst position zero bit index start zero
		 static int getFirstBit(int pn[], int bits)
		 {
			 int nIndex = bits/vBitsPerInt;
			 int nOffset = bits % vBitsPerInt;
			 int nbit = -1;
			 int ttl = 0;
			 int i = 0;
			 while(nbit==-1 && i<nIndex)
			 {
				 if(pn[i]!=-1)
					 nbit = getFirstBit(pn[i]);
				 else
					 ttl += vBitsPerInt;
				 i++;
			 }
			 if(nbit==-1 && nOffset)
			 {
				 int v = pn[nIndex] | (1<<(vBitsPerInt - nOffset)) - 1;
				 if(v!=-1) nbit = getFirstBit(v);
			 }
			 if(nbit==-1) return -1;
			 return nbit + ttl;
		 }

		 //if is all used
		 static bool isAllUsed(int pn[], int bits)
		 {
			 int nIndex = bits/(vIntSize * 8);
			 int noffset = bits % (vIntSize * 8);
			 int nbit = -1;
			 int ttl = 0;
			 int i = 0;
			 while(i< nIndex)
			 {
				 if(pn[i]!=-1) return false;
				 i++;
			 }

			 if(noffset)
			 {
				 if((((unsigned char)pn[nIndex])>> (vIntSize * 8 - noffset))!= ((0x1<<noffset) - 1))return false;
			 }

			 return true;
		 }

		 //if is not use
		 static bool isAllNotUsed(int pn[], int bits)
		 {
			 int nIndex = bits/(vIntSize * 8);
			 int noffset = bits % (vIntSize * 8);
			 int nbit = -1;
			 int ttl = 0;
			 int i = 0;
			 while(i< nIndex)
			 {
				 if(pn[i]) return false;
				 i++;
			 }
			 if(noffset && pn[nIndex])return false;
			 return true;
		 }

		 static void SetItemMaskBit(int pn[],int count, int nItem,bool set)
		 {
			 assert(nItem >=0 && nItem < count);

			 int index  = nItem / (vIntSize * 8);
			 int offset = nItem % (vIntSize * 8);

			 if(set)
				 pn[index] |= 1<<(vIntSize * 8 - offset - 1);
			 else
				 pn[index] &= ~(1<<(vIntSize * 8 - offset - 1));
		 }

	};
}