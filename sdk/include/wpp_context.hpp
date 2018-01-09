#ifndef WPP_WPP_CONTEXT_H
#define WPP_WPP_CONTEXT_H
#include "type.hpp"
#include "vill_asm.hpp"
#include "meta.hpp"
#include "wpp.hpp"
#include "werr.hpp"

namespace wpp{

class wpp_runtime; 
typedef class VLANG3_API wpp_context
{
public:
	wpp_context(wpp_runtime* runtime);
	~wpp_context();
public:
	mill::vill_asm * get_vill_asm();
	
	wMetaTable * get_metatable();
	void set_vill_asm(mill::vill_asm * ptr_code);
	void get_metatable(wMetaTable * ptr_metatable);
	Wpp* get_wpp_ptr();
	void set_wpp_ptr(Wpp* ptr_wpp);
	wpp_runtime* get_runtime();
	void set_runtime(wpp_runtime* ptr_runtime);
public:
	struct sourcetime_t
	{
		FILETIME dt;
		wstring  text;
	};
	static map<wstring,sourcetime_t>* sources;
	map<wstring,FILETIME>* files;
public:
	void set_error(wErr* errPtr_); 
	wErr* get_rrror(); 
public:
	bool parse(wchar_t *src); 
public:
	int process_file(const wchar_t* file);
	wchar_t* fetch_file(const wchar_t* file);

private:
	Wpp * ptr_wpp_;
	mill::vill_asm * ptr_vill_asm_;
	wMetaTable * ptr_metatable_;
	wpp_runtime* ptr_runtime_;
	wErr*			errPtr;
public:
	int	get_current_class();
	int get_current_method();
public:
	void GetClassMethodString(int pclass, wchar_t* fct, std::set<std::wstring> * & pSet);
	void wpp_context::GetClassAutoString(int pclass, std::set<std::wstring> * & pSet);
	wchar_t* wpp_context::getVarAutoString(wchar_t* v);
	wchar_t * wpp_context::getFctTipString(wchar_t* v);
	void wpp_context::freeAutoString(wchar_t * p);

} wpp_context_t;

}//namespace wpp

#endif //WPP_WPP_CONTEXT_H