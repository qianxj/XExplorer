#ifndef META_H
#define META_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include <stack>
#include "hashcontext.hpp"
#include "wpppred.hpp"
#include "type.hpp"

using namespace std;
using namespace wpp::how;


struct VLANG3_API WContext
{
	CWord global;
#pragma warning( push )
#pragma warning( disable : 4251 )
	stack<CWord> scope;
#pragma warning( pop ) 
	CHashContext hashContext;

	const static int eAny	 = 0x00;
	const static int eType   = 0x01;
	const static int eClass  = 0x02;
	const static int eMethod = 0x04;
	const static int eField  = 0x08;
	const static int eParam  = 0x10;
	const static int eVar	 = 0x20;
	const static int eItem = eField|eParam|eVar;

	int id_undef;
	WContext();
	CWord get_scope_top();
	CWord & push_item(CWord & w);
	CWord & enter_scope(CWord & w);
	void leave_scope();
	CWord lookup(const wchar_t* name,int what = eAny);
	CWord lookup_item(const wchar_t * name);
	CWord lookup_method(const wchar_t * name);
	CWord lookup_type(const wchar_t * name);
	CWord lookup_class(const wchar_t * name);
	CWord lookup_in(CWord& w,const wchar_t* name);
	CWord lookup(CWord axis, const wchar_t* name);
	CWord lookup(CWord axis, const wchar_t* name,int what);
	CWord lookup_item(CWord axis,const wchar_t * name);
	CWord lookup_method(CWord axis,const wchar_t * name);
	CWord lookup_type(CWord axis,const wchar_t * name);
	CWord lookup_class(CWord axis,const wchar_t * name);
};

class VLANG3_API wMetaTable
{
private:
	WContext wcontext_;
public:
	WContext * get_context();
#if 0
public:
	struct wMethod;
	struct VLANG3_API wClass
	{
		int ntype;
		int nField;
		int nParam;
		wMetaTable* metaTable;
		wClass(){}
		wClass(wMetaTable* metaTable_):ntype(0),nField(0),nParam(0), metaTable(metaTable_){}
		wClass& append_super(unsigned spec, const wchar_t* name);
		wClass& append_super(unsigned spec, CWord & pSuper);
		wClass& append_field(unsigned spec, const wchar_t* name, int ntype, int dval=0);
		wMethod* append_method(unsigned spec, const wchar_t* name, int ntype);
		wClass& append_param(unsigned spec, const wchar_t* name, int ntype, int dval=0);
		void leave_scope();
		CWord get_word();
	};
	struct VLANG3_API wMethod
	{
		int nMethod;
		int nParam;
		int nArg;
		int nVar;
		wClass* wclass;
		wMetaTable* metaTable;
		wMethod(wClass* wclass_):nVar(0),nArg(0),nParam(0),nMethod(0),wclass(wclass_),metaTable(wclass_->metaTable){}
		wMethod(wMetaTable* metaTable_):nMethod(0),nVar(0),nArg(0),nParam(0),wclass(0),metaTable(metaTable_){}
		wMethod& append_param(unsigned spec, const wchar_t* name, int ntype, int dval=0);
		wMethod& append_arg(unsigned spec, const wchar_t* name, int ntype, int dval=0);
		wMethod& append_var(unsigned spec, const wchar_t* name, int ntype);
		wMethod& append_type(unsigned spec, const wchar_t* name, int ntype);
		wMethod& append_code(vector<unsigned char> & code, int from);
		wClass&  leave_scope();
		wClass&  get_class() { return *wclass;}
		int get_method_addr();
		
		CWord get_word();

		int make_var_frame();
		void* get_var_frame();
		int make_param_frame();
		void* get_param_frame();
		int get_param_count();
		int get_var_count();
		CWord::iterator get_var_begin();
		CWord::iterator get_param_begin();
		CWord::iterator get_param(int nIndex);
		CWord::iterator get_var(int nIndex);
		int get_param_offset(int nIndex);
		int get_var_offset(int nIndex);
		int get_param_type(int nIndex);
		int get_var_type(int nIndex);
		int get_item_index(CWord item,int & nItemType);
	};
#endif
public:
	wMetaTable();
public:
	void init_context(WContext& context);
public:
#if 0
	wClass* declare_class(const wchar_t* name, const wchar_t* area);
	wClass* declare_class(CWord w);
	wMethod* append_method(unsigned spec, const wchar_t* name, int ntype);
	//wMethod* append_method(unsigned spec, const wchar_t* name, int ntype);
public:
	unsigned int get_spec(const wchar_t* spec);
	int get_type(const wchar_t* name);
#endif
public:
	//method
#if 0
	int make_var_frame(CWord fn);
	void* get_var_frame(CWord fn);
	int make_param_frame(CWord fn);
	void* get_param_frame(CWord fn);
#endif
	int get_method_addr(CWord fn);
	int get_param_count(CWord fn);
	int get_var_count(CWord fn);
	CWord::iterator get_var_begin(CWord fn);
	CWord::iterator get_param_begin(CWord fn);
#if 0
	int get_param_offset(CWord fn,int nIndex);
	int get_var_offset(CWord fn,int nIndex);
	int get_param_type(CWord fn,int nIndex);
	int get_var_type(CWord fn,int nIndex);
#endif
	CWord::iterator get_param(CWord fn,int nIndex);
	CWord::iterator get_var(CWord fn,int nIndex);
#if 0
	int get_tvm_type(const wchar_t* typname);
	int get_item_index(CWord fn,CWord item,int & nItemType);
#endif
public:
	const CWordValue & get_node_type(CWord& node);
	void set_node_type(CWord & node, wpp_ast_t nodetype);
	//get node by word
	bool is_class_node(CWord& node);
	bool is_namespace_node(CWord& node);
	bool is_param_node(CWord& node);
	bool is_arg_node(CWord& node);
	bool is_var_node(CWord& node);
	bool is_field_node(CWord& node);
	bool is_method_node(CWord& node);
	//get node by word value
	bool is_class_node(const CWordValue & v);
	bool is_namespace_node(const CWordValue & v);
	bool is_param_node(const CWordValue & v);
	bool is_var_node(const CWordValue & v);
	bool is_arg_node(const CWordValue & v);
	bool is_field_node(const CWordValue & v);
	bool is_method_node(const CWordValue & v);
public:
	bool is_static_spec(int spec) {return spec & wpp::type_spec::static_?true:false;}
	bool is_byref_spec(int spec){ return spec & wpp::type_spec::byref_?true:false;}
public:
	bool is_class_type(wpp::type_t typ){return typ == wpp::type::class_ || typ == wpp::type::struct_ ||typ == wpp::type::interface_;}
	bool is_struct_type(wpp::type_t typ){return typ == wpp::type::struct_;}

public:
	int calc_arg_offset(CWord fn,CWord & arg,bool owner = false);
	int calc_var_offset(CWord fn,CWord & var,bool owner = false);
	int calc_var_offset(CWord fn);
	int calc_arg_offset(CWord fn);

public:
	int get_size(CWord & typ);
	int get_static_size(CWord & typ);
public:
	int calc_class_align_size(CWord & wclass,int & msize, int align = 4);
	int calc_field_offset(CWord & wclass, CWord & field, int align = 4);
	int calc_field_offset(CWord & wclass, CWord & field,bool & bfound, int align = 4);
public:
	bool type_can_imp_conv(wpp::type_t ty1,wpp::type_t ty2);
	static wpp::type_t node_to_type(CWord & ty);
public:
	int find_matched_method( CWord * nest, const wchar_t* ident, int nargs,const std::vector<wpp::type_t> &typs);
	bool method_arg_matched(CWord & fn, int nargs,const std::vector<wpp::type_t> &typs);
public:
	CWord  find_method(wchar_t * class_name, wchar_t* method_name);
	CWord  findOleChild(CWord &item,const wchar_t * tk); 
public:
	int get_method_args( CWord fn, std::vector<wpp::type_t> &typs);
	int get_method_args_spec( CWord fn, std::vector<wpp::type_t> &spec);
	CWord get_method_arg( CWord fn, int index);

public:
	CWord  declare_class(const wchar_t* name,bool enter_scope=true);
	CWord  declare_class(CWord& ns, const wchar_t* name,bool enter_scope=true);
	CWord append_super(CWord  &wc, unsigned spec, const wchar_t* name);
	void  append_super(CWord  &wc, unsigned spec, CWord & pSuper);
	CWord append_field(CWord  &wc, unsigned spec, const wchar_t* name, int ntype, int dval=0);
	CWord append_method(CWord &wc, unsigned spec, const wchar_t* name, int ntype);
	CWord append_param(CWord  &wc, unsigned spec, const wchar_t* name, int ntype, int dval=0);
	CWord append_pred(CWord &wc, const wchar_t* name);
	CWord append_preditem(CWord &wc, const wchar_t* name,int ns);
	
	CWord append_arg(CWord  &wm,unsigned spec, const wchar_t* name, int ntype, int dval=0);
	CWord append_var(CWord  &wm,unsigned spec, const wchar_t* name, int ntype);
	CWord append_type(CWord &wm,unsigned spec,const wchar_t* name, int ntype);
public:
	bool is_super(CWord &super, CWord &w);
public:
	const static int native_ptr_offset = 4; 
	int calc_class_vtable(CWord &wc,std::vector<int> &items);
	static int find_match_as_method(CWord &wc, CWord &wm);
};

#endif //META_H