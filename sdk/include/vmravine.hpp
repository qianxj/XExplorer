#ifndef WPP_VMRAVINE_H
#define WPP_VMRAVINE_H

#include "vm.hpp"
#include "word.hpp"
#include "vmcode.hpp"
using namespace wpp::how;

namespace vm
{

//ÐéÄâ»ú
class VMRavine
{
public:
	VMRavine();
	~VMRavine();
private:
	IVMTable * vmtable_;
public:
	void setVMTable(IVMTable * metaTable);
	IVMTable * getVMTable();
public:
	int genFn(int fn, unsigned char *  pc,unsigned char *  end);
};

} //namespace vm

#endif //WPP_VMRAVINE_H