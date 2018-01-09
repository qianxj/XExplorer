#ifndef WPP_AST_H
#define WPP_AST_H

#include "pred.hpp"

namespace wpp {
namespace how {

namespace ast
{
	DECL_PRED(type,			"ast:type");
	DECL_PRED(id,			"ast:id");
	DECL_PRED(this,			"ast:this");
	DECL_PRED(expression,	"ast:expression");
	DECL_PRED(operator,		"ast:oprator");
}

}} //namespace wpp::how

#endif WPP_AST_H