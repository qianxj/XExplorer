#pragma once

#include "../lexer/lexer.hpp"
#include "vldef.hpp"
#include "vlutils.hpp"
#include "errors.hpp"
#include "../vl/atoms.hpp"
#include "vlsym.hpp"

namespace vl
{
	#define _cp_parse_skip 2
	#define _cp_parse_processed  1
	#define _cp_parse_unprocessed  0
	#define _cp_parse_processerror  -1

	class vmlexer;
	class vmlang3;
	class VL_API vmparser3
	{
	public:
		vmparser3();
		~vmparser3();
	protected:
		vmlexer*	pLexer;
		vmlang3 *	pVlang;
	public:
		void setLexer(vmlexer* p);
		vmlexer* getLexer();
	public:
		void setVlang(vmlang3 * pv);
		vmlang3 * getVlang();
	public:
		//parse vlang
		bool parse(int pl);
	public:
		int class_specifier(int pv);
		int class_key(int & sat);
		int class_head(int pv);
		int base_clause(int pv);
		int member_specification(int pv);
	public:
		int nested_name_specifier(int pv);
		int qualified_id(int pv);
		int unqualified_id(int pv);
		int id_expression(int pv);
		int identifier(int pv);
		int declarator_id( int pv);
		int function_body(int pv);
	public:
		int pm_expression(int pl,int &ty,int &flag,int &spec);
		int expression(int pl,int & ty,int & flag  );
		int expressionex(int pv,bool blet = false);
		int expression_list(int pl, int &count);
	public:
		int declaration_seq(int pv);
		int declaration(int pv);
		int block_declaration(int pv);
		int simple_declaration(int pv );
		int type_id(unsigned int & spec,unsigned int &  typ,int pv);
		int decl_specifier_seq(unsigned int & spec);
		int type_specifier(unsigned int &  typ,int pv);
		int type_primary_specifier(unsigned int &  typ,int pv);
		int simple_type_specifier(unsigned int &  typ,int pv);
		int type_derived_specifier(unsigned int & typ);
	public:
		int init_declarator_list(unsigned int spec,unsigned int &ty,int pv);
		int declarator(unsigned int spec,unsigned int &  ty,int pv);
		int direct_declarator(unsigned int spec,unsigned int &ty,int pv);
		int cv_qualifier_seq(unsigned int spec);
		int parameter_declaration_clause(int pv);
		int parameter_declaration(int pv);
		int initializer_list(int pv);
		int initializer_clause(int pv);
		int initializer(int pv);
	public:
		int statement(int pv);
		int compound_statement(int pv);
		int selection_statement(int pv);
		int iteration_statement(int pv);
		int jump_statement(int pv);
		int labeled_statement(int pv);
		int declaration_statement(int pv) ;
		int expression_statement(int pv) ;
		int statement_seq(int pv);
		int condition(int pv);
		int for_init_statement(int pv);
	public:
		int namespace_definition(int pv);
	public:
		bool parseTuple(int& tp);
	public:
		int EnsureTokenItem(int pl);
		int CheckLeftItem(int pl);
		int getItemTypeSpec(int nitem,int &typ,int &spec);
		bool itemHaveProcessed(int pl);
		void setItemProcessed(int pl,bool bproc=true);

		enum opItemEm {opItemVal = 0,opItemAddr = 1, opItemAssignVal = 2};
		void consumeItem(int pl,int & ty,int & spec,opItemEm opEm = opItemVal);
		void clearItem(int pl);
	public:
		//error processed
		int error(error::err item,bool bprocess = false);
		int error(error::err item,const tchar * ext,bool bprocess = false);
		void procerror();
		int errorAndSkipLn(error::err item,tchar * ext);
	public:
		bool EnterCode(int code);
		bool EnterCode(unsigned int code,int ext);
		bool EnterCode(unsigned int code,double ext);
		bool EnterCode(unsigned int code,__int64 ext);
	};

}