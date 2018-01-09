#ifndef  XPATH_BNF_XPATH_TOKEN_H
#define  XPATH_BNF_XPATH_TOKEN_H

#include "ruler_bnf.hpp"
#include "ruler_path.hpp"

namespace wruler{

class RulerAction_XPATH;	
class VLANG3_API RulerToken_XPATH : public RulerToken_base
{
private:
	RulerAction_XPATH * action_;
public:
	inline void set_action(RulerAction_XPATH * action){action_ = action;}
public:
	//token interface
	virtual Token_ref next_token(RulerActionParam * p) ;
	virtual Token_ref current(RulerActionParam * p) ;
public:
	virtual wtoken & get_token(int tokenref, RulerActionParam * p) ;
	const wchar_t * get_source_url();
public:
	static bool match_spec(const wtoken &tk);
	static bool match_access(const wtoken & tk);
	static bool match_class_key(const wtoken & tk);
	static bool match(const wtoken &tk, int ty);
	static bool match_eof(const wtoken &tk);
	static bool match_ident(const wtoken &tk);
	static bool match_bool(const wtoken &tk);
	static bool match_string(const wtoken &tk);
	static bool match_char(const wtoken &tk);
	static bool match_number(const wtoken &tk);
	static bool match_operate(const wtoken &tk);
	static bool match_operate(const wtoken &tk,int op);
	static bool match_keyword(const wtoken &tk);
	static bool match_keyword(const wtoken &tk,const wchar_t* kw);
};

} //namespace wruler

#endif //XPATH_BNF_XPATH_PROC_H