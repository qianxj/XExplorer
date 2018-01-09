#ifndef VLANG_VARRAY_H_
#define VLANG_VARRAY_H_

namespace vl
{
	struct varrayblock
	{
		int next;	//pointer next block
		int prev;	//pointer prev block
		int large;	//pointer large next block
		int size;	//size of element in block
		int	data[1];//data array size is nBlockSize of vArrayMetaItem
	};

	//define mtable data block,this block used for insert or delete
	struct varraymeta
	{
		void*	ptStart;		//pointer start block
		int		size;			//count of element
		int		blockSize;		//data block size
		void*	ptLast;			//pointer end block
		int		nElement;		
		varrayblock ptHited;		//pointer hited block
		int ptHitedIndex;			//index of hited index
	};

	template<class T> 
	class varray
	{
	public:
		inline varray();
		inline ~varray();
	public:
		//int allocArray(int flag  = vArrayFlagInt,int nElement = 1,int blockSize = 32); //alloc array
	public:
		inline varraymeta * getMetaBlock();
	private:
		inline varrayblock* getBlock(int nIndex);
		inline varrayblock* allocDataBlock();
	private:
		int hitedId;
		varraymeta * hitedMetaBlock;
	public:
		//get array size
		inline int getSize() { return getMetaBlock()->size;}
		//set array size
		inline void setSize(int cz) { getMetaBlock()->size = cz;}
		//popup back
		inline int pop();
		//push array value
		inline int push();
		inline int push(T  d);

		inline T & getPtr(int nIndex);
		//get array value
		inline T getValue(int nIndex);
		//set array value
		inline int setValue(int nIndex,T d);
	};
}

#endif