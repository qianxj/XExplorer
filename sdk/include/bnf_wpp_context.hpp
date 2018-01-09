#ifndef  WPP_BNF_WPP_CONTEXT_H
#define  WPP_BNF_WPP_CONTEXT_H

#include "ruler_context.hpp"
#include <vector>
using namespace std;

namespace wruler {

class bnf_wpp_context_t : public ruler_dynamic_context_t
{
public:
	bnf_wpp_context_t();
public:
	virtual void setAction(void *p);
	virtual void * getAction();
public:
	int guess_indent(CWord nest, Token_ref & tk, RulerActionParam *  param);
public:
	virtual int guess(t_maybe_path,RulerToken_base * rToken, RulerActionParam *  param);
	virtual int guess_path(vector<int> &p,	RulerToken_base * rToken, RulerActionParam *  param);
	virtual int ensure(RulerSym_base & rSym,RulerEngine_base& rEngine,int state_node,int TokenIndex,vector<path_link_t> & items);
public:
	virtual int register_maybe(t_maybe_token,int from,int to);
	virtual int register_maybe(t_maybe_state,int from,int to);
public:
	virtual bool maybe(t_maybe_token,int from ,int to,RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool maybe(t_maybe_state,int from, int to,RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool nomaybe(t_maybe_token,int from ,int to,RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool nomaybe(t_maybe_state,int from, int to,RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool ensure(t_maybe_token,int from ,int to,	RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool ensure(t_maybe_state,int from, int to,	RulerToken_base * rToken, RulerActionParam *  param);
private:
	void * action_;
};

} //namespace wruler

#endif //WPP_BNF_WPP_CONTEXT_H