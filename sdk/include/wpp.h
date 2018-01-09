#ifndef WPP_H
#define WPP_H

#ifdef VLANG3_EXPORTS
#define VLANG3_API __declspec(dllexport)
#else
#define VLANG3_API __declspec(dllimport)
#endif

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"

#include "scan.hpp"
#include "parser.hpp"
#include "gen.hpp"
#include "type.hpp"
//#include "tvm.hpp"
//#include "wparser.hpp"

#include "scan-r.hpp"
#include "parser-r.hpp"
#include "type-r.hpp"
#include "symtable-r.hpp"

#include "wpp.hpp"
#include "wlang.hpp"

#include "xthing.hpp"
#include "xinter.hpp"
#include "xform.hpp"
#include "xequality.hpp"
#include "xcontext.hpp"
#include "xaffect.hpp"

#include "logger.hpp"
#include "log.hpp"
#include "logmessage.hpp" 
#include "debuger.hpp"

#include "ruler_base.hpp"
#include "ruler_bnf.hpp"
#include "ruler_path.hpp"

#include "wpp_runtime.hpp"
#include "wpp_context.hpp"

#include "bnf_cpp.hpp"
#include "bnf_wpp.hpp"
#include "bnf_wpp_token.hpp" 
#include "bnf_wpp_action.hpp"

#endif //WPP_H