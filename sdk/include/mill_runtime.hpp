#ifndef WPP_MILL_RUNTIME_H
#define WPP_MILL_RUNTIME_H

#include "mill_space.hpp"
#include "mill_gc.hpp"

namespace mill
{

struct mill_active_recorder_base_
{
	void * ptr_arg;
	void * ptr_return_addr;
	void * ptr_bp;
	void * ptr_var;
	void * ptr_register;
	void * ptr_buffer;
};

struct mill_runtime_base_
{
	//活动记录
	mill_active_recorder_base_ * ptr_active_recorder;
	
	//空间管理
	mill_space_base_ * ptr_space;

};



} //namespace mill

#endif //WPP_MILL_RUNTIME_H