#ifndef WPP_VILL_INTER_H
#define WPP_VILL_INTER_H

#include "vlang3.h"
#include "type.hpp"
#include <string>
#include "vill_code.hpp"
#include "vill_context.hpp"
#include "werr.hpp"

namespace mill{

class VLANG3_API vill_inter
{
public:
	unsigned char *  code;
	unsigned char *  pc;
public:
	vill_inter(){};
	~vill_inter(){};
public:
	int call(struct vill_context * ptr_context, unsigned char * pc);
	int call(struct vill_context * ptr_context, unsigned char * pc, unsigned char * last);
	int eval(struct vill_context * ptr_context, unsigned char * pc,unsigned char * last);
public:
	int unasm(std::wstring & str, unsigned char *  pc,unsigned char *  end);
public:
	int call_method(vill_context * ptr_context,int fn,int args,bool virtual_call = false);
	int ole_call(vill_context * ptr_context,   int fn,int args,bool virtual_call = false);
	int native_call(vill_context * ptr_context,int fn,int args,bool virtual_call = false);
	int method_call(vill_context * ptr_context,int fn,int args,bool virtual_call = false);
	int stand_call(vill_context * ptr_context, int fn,int args,bool virtual_call = false);
};


} //namespace mill

#endif //WPP_VILL_INTER_H