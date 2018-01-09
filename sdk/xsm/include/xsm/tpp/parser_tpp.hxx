	#pragma once
	#include 	"../parser/tokenmgr.hxx"
	#include "../parser/parser.hxx"
	#include "../package/xpackage.h"
class XSM_API parser_cpp : public parse 
 {
	 bool TypeCanImpConv(int From, int To);
	 bool TypeCanExpConv(int From, int To);
	 inline atom GetTypeCat(int ty);
	  bool type_check(int from , int to);
	 int type_result(int t1, int t2,atom op,bool convert=false,int t3=0);
	
public:
         	dword		m_state;        //parse state
         	CompileEnv*	m_pEnv;
	 	vector<dword>	m_states;
	 	xsm::xPackageMgr * 	m_pPackageMgr;
         	bool 			m_parsedecl;
	
	 parser_cpp();
	 ~parser_cpp();
	 void SetParseDecl(bool bParseDecl);
	 virtual atom parse_trans_unit();
	 atom  declaration_seq(atom tk);
	 AstNode_indentifier* identifier(sym_object * & pobj);
	 AstNode_indentifier*  id_expression(sym_object * & pobj);
	 AstNode_indentifier*  unqualified_id(sym_object * & pobj);
	 AstNode_indentifier* qualified_id(sym_object * & pobj);
	 atom nested_name_specifier(sym_object * & pNest);
	 atom primary_expression(AstNode_expression *exp,int &typ,sym_object * &  pobj, AstNode_indentifier * & pindentifier, bool asknode = false );
	 atom primary_expression(AstNode_expression *exp,int &typ,sym_object * &  pobj);
	  atom postfix_expression(AstNode_expression *exp , int &typ);
	 atom unary_expression(AstNode_expression *exp,int & typ);
	 atom cast_expression(AstNode_expression *exp , int &typ);
	 atom pm_expression(AstNode_expression *exp , int &typ);
	 atom  multiplicative_expression(AstNode_expression *exp , int &typ);
	 atom  additive_expression(AstNode_expression *exp, int & typ);
	 atom  shift_expression(AstNode_expression *exp, int & typ);
	 atom  relational_expression(AstNode_expression *exp, int & typ);
	 atom  equality_expression(AstNode_expression *exp, int & typ);
	 atom  and_expression(AstNode_expression *exp, int & typ);
	 atom  exclusive_or_expression(AstNode_expression *exp, int & typ);
	 atom  inclusive_or_expression(AstNode_expression *exp, int & typ);
	 atom  logical_and_expression(AstNode_expression *exp, int & typ);
	 atom  logical_or_expression(AstNode_expression *exp, int & typ);
	 atom  conditional_expression(AstNode_expression *exp, int & typ);
	 atom assignment_expression(AstNode_expression *exp, int & typ);
	 atom expression(AstNode_expression *exp,int & typ);
	 atom constant_expression(AstNode_expression *exp,int & typ);
	 atom statement(AstNode_pcode *pcode);
	 atom labeled_statement(AstNode_pcode *pcode);
	 atom expression_statement(AstNode_pcode *pcode);
	 atom compound_statement(AstNode_pcode *pcode,bool bEnterBlock = true);
	 atom statement_seq(AstNode_pcode *pcode);
	 atom selection_statement(AstNode_pcode *pcode);
	 atom condition(AstNode_pcode *pcode);
	 atom iteration_statement(AstNode_pcode *pcode);
	 atom for_init_statement(AstNode_pcode *pcode);
	 atom jump_statement(AstNode_pcode *pcode);
	 atom declaration_statement(AstNode_pcode *pcode);
	 atom declaration(AstNode_pcode *pcode = null);
	 atom block_declaration(AstNode_pcode *pcode=null);
	 atom simple_declaration(AstNode_pcode *pcode=null);
	 atom decl_specifier_seq(dword & spec);
	 atom type_specifier(int &  typ);
	 atom type_primary_specifier(int &  typ);
	 atom  simple_type_specifier(int &  typ);
	 atom type_derived_specifier(int & typ);
	 atom namespace_definition();
	 atom using_declaration();
	 atom using_directive();
	 atom init_declarator_list(dword spec,int &  ty,AstNode_pcode *pcode=null);
	 atom declarator(dword spec,int &  ty,AstNode_pcode *pcode=null);
	 atom direct_declarator(dword spec,int &  ty,AstNode_pcode *pcode=null);
	 atom cv_qualifier_seq(dword spec);
	 AstNode_indentifier*  declarator_id( int &  ty);
	 atom type_id(dword & spec,int &  typ);
	 atom parameter_declaration_clause(AstNode_pcode *pcode=null);
	 atom parameter_declaration(AstNode_pcode *pcode=null);
	 atom function_body(sym_method * pmethod);
	 atom initializer(AstNode_pcode *pcode=null);
	 atom initializer_clause(AstNode_pcode *pcode=null);
	 atom initializer_list(AstNode_pcode *pcode=null);
	 atom class_specifier();
	 atom class_head(sym_class * & pclass);
	 atom class_key(atom & sat);
	 atom member_specification(sym_class * pclass);
	 atom base_clause(sym_class * pclass);
	 atom ProcessInclude();
	 atom ProcessFile(lptstr file);};