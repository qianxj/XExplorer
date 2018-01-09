#ifndef WPP_DATA_POOL_H
#define WPP_DATA_POOL_H

#include <vector>

namespace wpp  {

typedef class data_pool
{
public:
	data_pool();
	~data_pool();
public:
	void* append_data(const void * ptr_data, int size);

private:
	typedef struct ptr_data_size
	{
		void * ptr_;
		int    size_;
		ptr_data_size(void * ptr,int size):ptr_(ptr),size_(size){}
	} ptr_data_size_t;
private:
	std::vector<ptr_data_size_t>* space_alloc_;
	std::vector<ptr_data_size_t>* space_free_;
private:
	void * ensure_space(int size);
	void * alloc_space(int size);
private:
	//统计数据
	int data_count_; //append data count
	int data_used_;	//total data used
	int alloc_size_;	//total alloc size
	int free_size_;	//total free size 
} data_pool_t;

}

#endif //WPP_DATA_POOL_H