#ifndef WPP_MILL_CONTEXT_H
#define WPP_MILL_CONTEXT_H

#include "mill_metatable.hpp"
#include "mill_type.hpp"
#include "mill_runtime.hpp"
#include "mill_space.hpp"
#include "mill_code.hpp"

namespace mill
{

class mill_context_base_
{
public:
	mill_meta_data_ * get_mata_data() {return ptr_meta_data_;}
	void set_meta_data(mill_meta_data_ * ptr_meta_data){ptr_meta_data_ = ptr_meta_data;}
public:
	mill_type_base_ * get_type(){return ptr_type_;}
	void set_type(mill_type_base_ * ptr_type) { ptr_type_ = ptr_type;}
public:
	mill_runtime_base_ * get_runtime(){return ptr_runtime_;}
	void set_runtime(mill_runtime_base_ * ptr_runtime){ptr_runtime_ = ptr_runtime;}
protected:
	//�����ķ��ű�
	 mill_meta_data_ * ptr_meta_data_;

	//ִ�л���
	mill_runtime_base_ * ptr_runtime_;

	//����
	mill_type_base_ * ptr_type_;
};

}

#endif //namespace mill