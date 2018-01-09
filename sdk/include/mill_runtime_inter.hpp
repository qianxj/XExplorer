#ifndef WPP_MILL_RUNTIME_INTER_H
#define WPP_MILL_RUNTIME_INTER_H

#include <vector>
#include "debuger.hpp"
#include "mill_runtime.hpp"
#include "mill_code.hpp"

namespace mill
{

struct mill_runtime_static_inter_
{
	//指令指针
	int cp;
	//堆栈指针
	int sp;
	//基址指针
	int bp;
};

struct mill_active_recorder_inter_ : public mill_active_recorder_base_
{
public:

};



struct mill_runtime_inter_ : public mill_runtime_base_
{
	mill_runtime_static_inter_ * ptr_mill_runtime_static;
};

}//namespace mill

#endif //WPP_MILL_RUNTIME_INTER_H
