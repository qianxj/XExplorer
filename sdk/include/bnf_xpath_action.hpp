#ifndef  XPATH_BNF_XPATH_ACTION_H
#define  XPATH_BNF_XPATH_ACTION_H

#include "ruler_bnf.hpp"
#include "ruler_path.hpp"
#include "bnf_xpath.hpp"

#include "pred.hpp"
#include "parser_tr.hpp"
#include "errors.hpp"
#include "werr.hpp"
#include "wlexer.hpp" 
#include "wpp_context.hpp"

#include <stack>
#include <map>
using namespace wpp;
using namespace wpp::how;


#define wpp_decl_action_(id)	   static bool id##(RulerAction_XPATH * pAction,CWord &w,RulerActionParam * p);
#define wpp_attach_shift_(id, fn)  attach_shift(id,(fnPathAction)&##RulerAction_XPATH##::fn);
#define wpp_attach_reduce_(id, fn) attach_reduce(id,(fnPathAction)&##RulerAction_XPATH##::fn);

namespace wruler{

class VLANG3_API RulerAction_XPATH : public RulerAction_base
{
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
	RulerAction_XPATH();
	~RulerAction_XPATH();
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
#pragma warning( pop )
public:
	inline wlexer* getLexer(){return lexer_;}
public:
	void attach_action(RulerSym_XPATH & ruler,RulerEngine_XPATH & rp);

public:
	void error(wpp::how::error::err nError, const wchar_t* str,const wtoken& tk);
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<wtoken> token_buffer_;
	std::vector<int>	token_index_buffer_;
#pragma warning( pop )
public:
	void buffer_token(wtoken &tk, int tokenIndex);
	wtoken & get_buffer_token(int nIndex); //index from top
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
public:
	int print_node(CWord & node,RulerActionParam * p,int tb);
	int node_route_token(CWord & node, int tk);
	bool node_is_operator(CWord & node, wchar_t op,RulerActionParam * p);
};

} //namespace wruler

#endif //XPATH_BNF_XPATH_ACTION_H