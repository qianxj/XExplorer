#ifndef WPP_VMRUNTIME_H
#define WPP_VMRUNTIME_H

#include "word.hpp"
using namespace wpp::how;

namespace vm
{

struct RegItem
{
	int type;
	CWordValue val;
};

struct VMRuntime
{
	//default stack size;
	static const int cStackSize = 0x400 * 0x256;
	//maximal register size
	static const int cRegisterCount = 0xff;

	//register pool
	RegItem* registers;

	//runtime stack
	unsigned char* stacks;
	int*	stacksType;
	//stack pointer
	unsigned char* sp_;
	//stack type pointer
	int* spt_; 
	//base addr pointer
	unsigned char* bp_;
	//stack type pointer
	int* bpt_; 
	
	VMRuntime();
	~VMRuntime();
};

} //namespace vm

#endif //WPP_VMRUNTIME_H
