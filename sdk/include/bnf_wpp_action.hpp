#ifndef  WPP_BNF_WPP_ACTION_H
#define  WPP_BNF_WPP_ACTION_H

#include "ruler_bnf.hpp"
#include "ruler_path.hpp"
#include "bnf_wpp.hpp"

#include "pred.hpp"
//#include "tvm.hpp"
#include "parser_tr.hpp"
#include "errors.hpp"
#include "werr.hpp"
#include "wlexer.hpp" 
#include "mill_code.hpp"
#include "vill_asm.hpp"
#include "wpp_context.hpp"

#include <stack>
#include <map>
using namespace wpp;
using namespace wpp::how;


#define wpp_decl_action_(id)	   static bool id##(RulerAction_WPP * pAction,CWord &w,RulerActionParam * p);
#define wpp_attach_shift_(id, fn)  attach_shift(id,(fnPathAction)&##RulerAction_WPP##::fn);
#define wpp_attach_reduce_(id, fn) attach_reduce(id,(fnPathAction)&##RulerAction_WPP##::fn);

namespace wruler{

class VLANG3_API RulerAction_WPP : public RulerAction_base
{
public:
	//mill::mill_code mill_code_; 
	//mill::vill_asm vill_asm_;
public:
	mill::vill_asm * get_vill_asm(){return get_context()->get_vill_asm();}
private:
	std::vector<mill::vill_label> * ptr_suspend_break_label_;
	std::vector<mill::vill_label> * ptr_suspend_continue_label_;
	std::map<std::wstring, mill::vill_label> * ptr_named_label_;
public:
	std::vector<mill::vill_label> * get_suspend_break_label(){return ptr_suspend_break_label_;}
	std::vector<mill::vill_label> * get_suspend_continue_label(){return ptr_suspend_continue_label_;}
	std::map<std::wstring,mill::vill_label> * get_named_label(){ return ptr_named_label_;}
public:
	//TVm tvm;
	wlexer *	lexer_;
private:
	wpp_context_t*		context_;
public:
	wlexer *	get_lexer();
	wMetaTable* get_meta_table();
	void set_lexer(wlexer * lexer);
	wpp_context_t*	get_context(){return context_;}
	void	set_context(wpp_context_t* context){context_ = context;}
public:
	RulerAction_WPP();
	~RulerAction_WPP();
public:
	wchar_t token_str[256];
	void next_token();
	std::stack<wlexer *>* ptr_lexer_pool;
public:
	typedef int scope_state_t;
	struct scope_state
	{
		const static int scope_global		= 0x0;
		const static int scope_namespace	= 0x1;
		const static int scope_class		= 0x2;
		const static int scope_method		= 0x3;
		const static int scope_block		= 0x4;
	public:
		CWord scope_object;
		scope_state_t state;
	public:
		scope_state(CWord & ptr_obj,scope_state_t _state):scope_object(ptr_obj),state(_state){}
	};

#pragma warning( push )
#pragma warning( disable : 4251 )
	//处理的状态链
	std::vector<scope_state> proc_states_;
	//处理的节点链
	//std::vector<CWord> proc_nodes_;
#pragma warning( pop )
public:
	inline wlexer* getLexer(){return lexer_;}
public:
	void attach_action(RulerSym_WPP & ruler,RulerEngine_WPP & rp);

public:
	void error(wpp::how::error::err nError, const wchar_t* str,const wtoken& tk);
public:
	//int declare_class(CWord &w);
	int post_declare_class(CWord &w);
	unsigned char getOpcode(unsigned char op);
	int process_var_decl(CWord &w);
	int process_simple_expression(CWord &w);
	int process_expression(CWord &w);
	int process_stmt_comp(CWord &w);
	int process_stmt_exp(CWord &w);
	int process_stmt(CWord &w);
	int process_stmt_jump(CWord &w);
	int process_stmt_do(CWord &w);
	int process_stmt_switch(CWord &w);
	int process_stmt_while(CWord &w);
	int process_stmt_if(CWord &w);
	int process_stmt_for(CWord &w);
	int process_method_body(CWord &w);
	int process_method_param(CWord &w);
	int process_decl_list(CWord &w, int tailToken = 0);
	int process_decl(CWord &w);
	int process_name_space(CWord &w);
	int process_import(CWord &w);
	int process_using(CWord &w);
	int process_binary_operate(int op,CWord &left,CWord &right);
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<wtoken> token_buffer_;
	std::vector<int>	token_index_buffer_;
#pragma warning( pop )
public:
	void buffer_token(wtoken &tk, int tokenIndex);
	wtoken & get_buffer_token(int nIndex); //index from top
	void set_prop_string(CWord & prop, wchar_t* name, wtoken & tk);
	void set_prop_string(CWord & prop, int pred, wtoken & tk);
public:
	Token_ref  trans_token(wtoken &token,RulerActionParam * p);
	//揣测合适的符号语义
	int guess_token(wtoken &token,RulerActionParam * p);
	//揣测标志符合适的语义
	int guess_identifier(wtoken &token,RulerActionParam * p);
	//揣测合适的产生式
	int guess_production(wtoken &token,RulerActionParam * p);
public:
	//代理执行规则引擎
	int agentRulerEngine();
private:
	int register_;
	int register_count_;
public:
	int curRegister(){return register_ - 1;}
	int askRegister()
	{
		if(register_count_< register_) register_count_ = register_;
		return register_++;
	}
	void freeRegister(){
		
		if(register_ > 0) register_--;
	}
public:
	int print_node(CWord & node,RulerActionParam * p,int tb);
	int node_route_token(CWord & node, int tk);
	bool node_is_operator(CWord & node, wchar_t op,RulerActionParam * p);
public:
	typedef int gen_flag_t;
	struct gen_flag
	{
		static const int empty   = 0x0;
		static const int lvalue_ = 0x1;
		static const int const_  = 0x2;
		static const int int_    = 0x4;
		static const int static_ = 0x8;
		static const int gen_	 = 0x10;
	};

	wpp::type_t gen_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_assignment_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_conditional_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_binary_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_expression_list(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_cast_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_unary_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_postfix_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_primary_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_initializer_clause(CWord & node,RulerActionParam * p,gen_flag_t&  flag);

	wpp::type_t gen_statement_seq(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_labeled_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_expression_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_compound_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_selection_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_iteration_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_jump_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_declaration_statement(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
	wpp::type_t gen_new_expression(CWord & node,RulerActionParam * p,gen_flag_t&  flag);
private:
	int decl_specifier_seq(RulerAction_WPP * pAction,  CWord & node, RulerActionParam * p,
			unsigned int & spec, int &typ);
private:
	//context event
	wpp_decl_action_(reduce_translation_unit);

	//misc
	wpp_decl_action_(shift_identifier);

	//using
	wpp_decl_action_(reduce_using_declaration);

	//declare namespace 
	wpp_decl_action_(shift_namespace);
	wpp_decl_action_(reduce_namespace);
	wpp_decl_action_(reduce_namespace_ole_definition);

	//class native spec
	wpp_decl_action_(shift_native_spec);

	//declare
	wpp_decl_action_(reduce_simple_declaration);
	wpp_decl_action_(reduce_declaration);
	wpp_decl_action_(reduce_declaration_seq);

	//declare class
	wpp_decl_action_(reduce_class_key);
	wpp_decl_action_(reduce_class_head_name);
	wpp_decl_action_(reduce_class_head);
	wpp_decl_action_(reduce_class_spec);
	wpp_decl_action_(reduce_class_base_specifier);
	wpp_decl_action_(reduce_elaborated_type_specifier);

	//declare class member
	wpp_decl_action_(reduce_member_declarator);
	wpp_decl_action_(reduce_member_declaration);
	wpp_decl_action_(reduce_decl_specifier_seq);
	wpp_decl_action_(reduce_declarator_id);
	wpp_decl_action_(reduce_type_specifier);
	//declare field
	//declare var
	//declare method
	wpp_decl_action_(reduce_parameter_declaration);
	wpp_decl_action_(reduce_parameters_and_qualifiers);
	wpp_decl_action_(reduce_function_body);

	//declare pred
	wpp_decl_action_(reduce_pred_declaration);
	wpp_decl_action_(reduce_pred_attributes);
	wpp_decl_action_(reduce_axiom_definition);
	wpp_decl_action_(reduce_axiom_list);
	wpp_decl_action_(reduce_axiom_item);
	wpp_decl_action_(reduce_pred_expression);
	wpp_decl_action_(reduce_equality_definition);

	//expression
	wpp_decl_action_(shift_literal);
	wpp_decl_action_(shift_operator);
	wpp_decl_action_(reduce_id_expression);

	wpp_decl_action_(reduce_nested_name_specifier);

	wpp_decl_action_(reduce_primary_expression);
	wpp_decl_action_(reduce_postfix_expression);
	wpp_decl_action_(reduce_unary_expression);
	wpp_decl_action_(reduce_new_expression);
	wpp_decl_action_(reduce_cast_expression);
	wpp_decl_action_(reduce_pm_expression);
	wpp_decl_action_(reduce_multiplicative_expression);
	wpp_decl_action_(reduce_additive_expression);
	wpp_decl_action_(reduce_shift_expression);
	wpp_decl_action_(reduce_relational_expression);
	wpp_decl_action_(reduce_equality_expression);
	wpp_decl_action_(reduce_and_expression);
	wpp_decl_action_(reduce_exclusive_or_expression);
	wpp_decl_action_(reduce_inclusive_or_expression);
	wpp_decl_action_(reduce_logical_and_expression);
	wpp_decl_action_(reduce_logical_or_expression);
	wpp_decl_action_(reduce_conditional_expression);
	wpp_decl_action_(reduce_assignment_expression);
	wpp_decl_action_(reduce_expression);
	wpp_decl_action_(reduce_constant_expression);

	//stmt
	wpp_decl_action_(reduce_statement);
	wpp_decl_action_(reduce_labeled_statement);
	wpp_decl_action_(reduce_expression_statement);
	wpp_decl_action_(reduce_compound_statement);
	wpp_decl_action_(reduce_selection_statement);
	wpp_decl_action_(reduce_iteration_statement);
	wpp_decl_action_(reduce_jump_statement);
	wpp_decl_action_(reduce_declaration_statement);
	wpp_decl_action_(reduce_for_init_statement);

public:
	const wtoken * get_astnode_token(const CWord & node);
public:
	bool maybe_nodetype(CWord& nest, const CWord & node , int nodetype);
	bool maybe_nodetype(const CWord & node,int nodetype);
	bool check_nodetype(CWord& nest,  const CWord & node , int nodetype);
	bool check_nodetype(const CWord & node ,int nodetype);
public:
	bool maybe_namespace(CWord& nest, const CWord & node);
	bool maybe_namespace(const CWord & node);
	bool maybe_type(CWord& nest, const CWord &node);
	bool maybe_type(const CWord &node);
public:
	bool check_namespace(CWord& nest, const CWord & node);
	bool check_namespace(const CWord & node);
	bool check_type(CWord& nest, const CWord &node);
	bool check_type(const CWord &node);
};

} //namespace wruler

#endif //WPP_BNF_WPP_ACTION_H