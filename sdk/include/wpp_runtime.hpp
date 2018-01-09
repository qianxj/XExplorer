#ifndef WPP_WPP_RUNTIME_H
#define WPP_WPP_RUNTIME_H

#include "wpp_context.hpp"

namespace wpp{

typedef class VLANG3_API wpp_runtime
{
public:
	static wchar_t* (* GetSource)(wchar_t* purl) ;
	static void (* FreeSource)(wchar_t* purl) ;
	static FILETIME  (* GetSourceDT)(wchar_t* purl) ;
	static FILETIME  (* GetSourceVER)(wchar_t* purl) ;

public:
	wpp_runtime(){}
	~wpp_runtime(){}
public:
	int find_method(wpp_context* context,wchar_t * class_name, wchar_t* method_name);
	int find_method(wpp_context* context,int cls, wchar_t* method_name);
	int call_method(wpp_context* context,CWord  fn);
	int load_class(wpp_context* ,const wchar_t * urlClass);
	int create_class(wpp_context* ,int cf);
	void set_nativePointer(wpp_context* ,int cls,void * p);
	int call_method(wpp_context* context,CWord  fn,int pthis);
	int call_method(wpp_context* context,void* ctx,CWord  fn);
public:
	void * prepare_call();
	int push_arg(void* ctx,int param);
	int push_arg(void* ctx,__int64 param);
	int push_arg(void* ctx,unsigned int param);
	int push_arg(void* ctx,unsigned __int64 param);
	int push_arg(void* ctx,float param);
	int push_arg(void* ctx,double param);
	int push_arg(void* ctx,void* param);
public:
	std::vector<void *> * get_vruntime();
	
} wpp_runtime_t;

}//namespace wpp


#endif //WPP_WPP_RUNTIME_H