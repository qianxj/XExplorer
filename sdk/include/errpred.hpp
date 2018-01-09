#ifndef WPP_ERRPRED_H
#define WPP_ERRPRED_H

#include "pred.hpp"

namespace wpp {
namespace how {

namespace wpp_error
{
	decl_pred_(needln,			"err:needln");
	decl_pred_(needsemicolon,    "err:needsemicolon");
	decl_pred_(needrightbracket, "err:needrightbracket");
	decl_pred_(needrightbrace,   "err:needrightbrace");
	decl_pred_(needleftbracket,  "err:needleftbracket");
	decl_pred_(needleftbrace,    "err:needleftbrace");
	decl_pred_(muchdefine,		"err:muchdefine");
	decl_pred_(needoperator,		"err:needoperator");
	decl_pred_(needleftparen,    "err:needleftparen");
	decl_pred_(needrightparen,   "err:needrightparen");
	decl_pred_(needcomma,		"err:needcomma");
	decl_pred_(needgt,			"err:needgt");
	decl_pred_(needcolon,		"err:needcolon");

	decl_pred_(token,			"err:token");
	decl_pred_(doublescale,		"err:doublescale");
	decl_pred_(scannumber,		"err:scannumber");
	decl_pred_(string,			"err:string");
	decl_pred_(undefinedtype,    "err:undefinedtype");
	decl_pred_(needvarname,		"err:needvarname");
	decl_pred_(unvalidtype,		"err:unvalidtype");
	decl_pred_(existtype,		"err:existtype");
	decl_pred_(needname,			"err:needname");
	decl_pred_(needindent,		"err:needindent");
	decl_pred_(undefinedvar,		"err:undefinedvar");
	decl_pred_(undefinedlang,    "err:undefinedlang");

	decl_pred_(needcommenttrail, "err:needcommenttrail");
	decl_pred_(stringtrail,		"err:stringtrail");
	decl_pred_(unvaliddataitem,  "err:unvaliddataitem");
	decl_pred_(unvalidcommand,   "err:unvalidcommand");
	decl_pred_(unkownconst,		"err:unkownconst");
	decl_pred_(unkownstmt,		"err:unkownstmt");
	decl_pred_(needkeyword,		"err:needkeyword");
	decl_pred_(needtype,			"err:needtype");
	decl_pred_(notneedkeyword,   "err:notneedkeyword");
	decl_pred_(needdeclarestmt,  "err:needdeclarestmt");
	decl_pred_(needclassornamespace, "err:needclassornamespace");
	decl_pred_(undeftype,			"err:undeftype");
	decl_pred_(needclassorinteface,  "err:needclassorinteface");
	decl_pred_(unknownstmt,			"err:unknownstmt");
	decl_pred_(unknownmethod,		"err:unknownmethod");
	decl_pred_(needstring,			"err:needstring");
	decl_pred_(needclassorstruct,    "err:needclassorstruct");

	decl_pred_(type_cast,			"err:type_cast");
	decl_pred_(type_op,				"err:type_op");
	decl_pred_(method_arg,			"err:method_arg");
	decl_pred_(needcategoryint,		"err:needcategoryint");
	decl_pred_(needleftval,			"err:needleftval");

	//type group
	decl_pred_(needtypegroupindent,	"err:needtypegroupindent");
	decl_pred_(typegroup_needitem,   "err:typegroup_needitem");
	decl_pred_(typegroup_needitemval,"err:typegroup_needitemval");

	decl_pred_(typegroupval_ref,		"err:typegroupval_ref");
	decl_pred_(typegroupval_copy,    "err:typegroupval_copy");
	decl_pred_(typegroupval_array,   "err:typegroupval_array");
	decl_pred_(typegroupval_fctpure, "err:typegroupval_fctpure");


	//type list
	decl_pred_(needtypeindent,   "err:needtypeindent");

}
namespace worn
{
	//warning	type group list
	decl_pred_(typegroup_unvaditem, "worn:typegroup_unvaditem");
}

}} //namespace wpp::how

#endif WPP_ERRPRED_H