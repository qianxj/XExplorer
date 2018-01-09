#ifndef WPP_AST_H
#define WPP_AST_H

#include "pred.hpp"

namespace wpp {
namespace how {

typedef int wpp_ast_t; 

namespace wpp_ast
{
	decl_pred_(nodetype,		"ast:nodetype");
	decl_pred_(childnodetype,	"ast:childnodetype");
	decl_pred_(type,			"ast:type");
	decl_pred_(id,				"ast:id");
	decl_pred_(this_,			"ast:this");
	decl_pred_(operator_,		"ast:operator");
	decl_pred_(class_,			"ast:class");
	decl_pred_(method,			"ast:method");
	decl_pred_(field,			"ast:field");
	decl_pred_(param,			"ast:param");
	decl_pred_(pred,			"ast:pred");
	decl_pred_(preditem,		"ast:preditem");
	decl_pred_(preditem_ns,		"ast:preditem_ns");
	decl_pred_(pred_expr,		"ast:pred_expr");
	decl_pred_(equality,		"ast:equality");
	decl_pred_(axiom,			"ast:axiom");
	decl_pred_(arg,				"ast:argument");
	decl_pred_(var,				"ast:var");
	decl_pred_(expr,			"ast:expr");
	decl_pred_(stmt,			"ast:stmt");
	decl_pred_(val,				"ast:val");
	decl_pred_(child,			"ast:child");
	decl_pred_(parent,			"ast:parent");
	decl_pred_(const_,			"ast:const");
	decl_pred_(namespace_,		"ast:namespace");
	decl_pred_(class_super,		"ast:class-super");
	decl_pred_(val_default,		"ast:val-default");
	decl_pred_(field_no,		"ast:field-no");
	decl_pred_(param_no,		"ast:param-no");
	decl_pred_(seq_no,			"ast:field-no");
	decl_pred_(data_type,		"ast:data-type");
	decl_pred_(type_id,			"ast:type-id");
	decl_pred_(code,			"ast:code");
	decl_pred_(var_frame,		"ast:var-frame");
	decl_pred_(param_frame,		"ast:param-frame");
	decl_pred_(ident,			"ast:ident");
	decl_pred_(atype,			"ast:atype"); //atom type
	decl_pred_(typepart,		"ast:typepart");
	decl_pred_(identpart,		"ast:identpart");
	decl_pred_(compnode,		"ast:compnode");
	decl_pred_(termination,		"ast:termination");
	decl_pred_(token,			"ast:token");
	decl_pred_(token_ref_,		"ast:token_ref");
	decl_pred_(token_last,		"ast:token_last");
	decl_pred_(token_count,		"ast:token_count");
	decl_pred_(token_top,		"ast:token_top");
	decl_pred_(child_count,		"ast:child_count");
	decl_pred_(method_registeres, "ast:method_registeres");
	decl_pred_(method_ttloffset,  "ast:method_ttloffset");
	decl_pred_(spec,			  "ast:spec");
	decl_pred_(init,			  "ast:init");
	decl_pred_(hasinit,			  "ast:hasinit");
	decl_pred_(method_addr,		  "ast:method_addr");
	decl_pred_(method_from,		  "ast:method_from");
	decl_pred_(method_alias,	  "ast:method_alias");
	decl_pred_(forward,			  "ast:forward");
	decl_pred_(ole_name,		  "ast:ole_name");
	decl_pred_(ole_ptlib,		  "ast:ole_ptlib");
	decl_pred_(isole,			  "ast:isole");
	decl_pred_(ole_typeinfo,	  "ast:ole_typeinfo");
	decl_pred_(ole_propget,		  "ast:ole_propget");
	decl_pred_(ole_propput,		  "ast:ole_propput");
	decl_pred_(nest_node,		  "ast:nest_node");
	decl_pred_(obj_defined,		  "ast:obj_defined");
	decl_pred_(obj_isdirty,		  "ast:obj_dirty");
	decl_pred_(voc,				  "ast:voc");
	decl_pred_(interface_,		  "ast:interface");
	decl_pred_(enum_,			  "ast:enum");
	decl_pred_(union_,			  "ast:union");
	decl_pred_(struct_,			  "ast:struct");
}

namespace wpp_parse
{	
	//match
	decl_pred_(match_token,	"parse:match_token");
	decl_pred_(match_string,	"parse:match_string");
	decl_pred_(match_char,	"parse:match_char");
	decl_pred_(match_number,	"parse:match_number");
	decl_pred_(match_keyword,"parse:match_keyword");
	decl_pred_(match_ide,	"parse:match_ide");
	decl_pred_(match_operate,"parse:match_operate");
	decl_pred_(match_spec,	"parse:match_spec");

	//action
	decl_pred_(lang_method,		"parse:lang_method");
	decl_pred_(expr,				"parse:expr");
	decl_pred_(lang_var,			"parse:lang_var");
	decl_pred_(stmt_if,			"parse:stmt_if");
	decl_pred_(stmt_while,		"parse:stmt_while");
	decl_pred_(lang_class,		"parse:lang_class");
	decl_pred_(lang_super,		"parse:lang_super");
	decl_pred_(lang_namespace,	"parse:lang_namespace");
	decl_pred_(exp_id,			"parse:exp_id");
	decl_pred_(lang_enterscope,	"parse:lang_enterscope");
	decl_pred_(lang_leavescope,	"parse:lang_leavescope");
}

}} //namespace wpp::how

#endif WPP_AST_H