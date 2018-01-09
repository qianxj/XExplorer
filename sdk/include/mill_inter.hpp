#ifndef WPP_MILL_INTER_H
#define WPP_MILL_INTER_H

#include <vector>
#include "mill_context_inter.hpp"
#include "vill_asm.hpp"
#include "vill_context.hpp"

namespace mill
{


//mill interpretation
class mill_inter_
{
public:
	mill_context_inter_ * ptr_context;
public:
	int throw_error(unsigned int  error);
	int eval(int pc = 0);
	int call(int pc= 0);
public:
	unsigned char * get_code_ptr(){return 0;}
public:

public:
	void * vill_make_class(mill_context_inter_* ptr_context,int ty);
	void vill_destroy_class(mill_context_inter_* ptr_context, void * p);
public:
	int vill_call(mill_context_inter_* ptr_context, unsigned char * pc);
public:
	int vill_call_method(mill_context_inter_* ptr_context,int fn,int args,bool virtual_call = false);
	int vill_ole_call(mill_context_inter_* ptr_context,int fn,int args,bool virtual_call = false);
	int vill_native_call(mill_context_inter_* ptr_context,int fn,int args,bool virtual_call = false);
	int vill_method_call(mill_context_inter_* ptr_context,int fn,int args,bool virtual_call = false);
	int vill_stand_call(mill_context_inter_* ptr_context,int fn,int args,bool virtual_call = false);
public:
	int vill_emit(unsigned char * pc, unsigned char buf[],int len);
};

} //namespace mill

#endif //WPP_MILL_CONTEXT_INTER_H