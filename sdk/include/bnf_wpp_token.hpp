#ifndef  WPP_BNF_WPP_TOKEN_H
#define  WPP_BNF_WPP_TOKEN_H

#include "ruler_bnf.hpp"
#include "ruler_path.hpp"

namespace wruler{

class RulerAction_WPP;	
class VLANG3_API RulerToken_WPP : public RulerToken_base
{
private:
	RulerAction_WPP * action_;
public:
	inline void set_action(RulerAction_WPP * action){action_ = action;}
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

#endif //WPP_BNF_WPP_PROC_H