#ifndef  WPP_BNF_CPP_H
#define  WPP_BNF_CPP_H

#include "ruler_base.hpp"
#include "ruler_bnf.hpp"
#include "ruler_path.hpp"

namespace wruler{

class VLANG3_API  RulerEngine_CPP : public RulerEngine_BNF
{
};

class VLANG3_API  RulerSym_CPP : public RulerSym_BNF
{
public:
	RulerSym_CPP();
	void initial();
public:
	//
	CPath literal;

	CPath integer_literal;
	CPath character_literal;
	CPath floating_literal;
	CPath string_literal;
	CPath boolean_literal;

	//basic concepts
	CPath translation_unit;
	CPath declaration_seq;

	//expressions
	CPath primary_expression;
	CPath expression;
	CPath id_expression;
	CPath lambda_expression;
	CPath unqualified_id;
	CPath qualified_id;
	CPath operator_function_id;
	CPath literal_operator_id;
	CPath template_id;
	CPath type_name;
	CPath namespace_name;
	CPath lambda_introducer;
	CPath compound_statement;
	CPath lambda_declarator;
	CPath lambda_capture;
	CPath capture_default;
	CPath capture_list;
	CPath capture;
	CPath trailing_return_type;
	CPath exception_specification;
	CPath parameter_declaration_clause;
	CPath postfix_expression;
	CPath simple_type_specifier;
	CPath typename_specifier;
	CPath pseudo_destructor_name;
	CPath type_id;
	CPath initializer_list;
	CPath unary_expression;
	CPath cast_expression;
	CPath unary_operator;
	CPath alignof;
	CPath noexcept_expression;
	CPath new_expression;
	CPath delete_expression;
	CPath new_placement;
	CPath new_type_id;
	CPath new_initializer;
	CPath noptr_new_declarator;
	CPath new_declarator;
	CPath pm_expression;
	CPath multiplicative_expression;
	CPath additive_expression;
	CPath shift_expression;
	CPath relational_expression;
	CPath equality_expression;
	CPath and_expression;
	CPath exclusive_or_expression;
	CPath inclusive_or_expression;
	CPath logical_and_expression;
	CPath logical_or_expression;
	CPath assignment_expression;
	CPath initializer_clause;
	CPath throw_expression;
	CPath conditional_expression;
	CPath assignment_operator;

	//statement
	CPath statement;
	CPath labeled_statement;
	CPath expression_statement;
	CPath selection_statement;
	CPath iteration_statement;
	CPath jump_statement;
	CPath declaration_statement;
	CPath try_block;
	CPath statement_seq;
	CPath condition;
	CPath for_init_statement;
	CPath for_range_declaration;
	CPath for_range_initializer;
	CPath simple_declaration;
	CPath braced_init_list;
	CPath block_declaration;
	
	//Declarations
	CPath declaration;
	CPath explicit_instantiation ;
	CPath explicit_specialization ; 
	CPath linkage_specification ; 
	CPath namespace_definition ; 
	CPath empty_declaration ;
	CPath attribute_declaration ; 
	CPath asm_definition ;
	CPath namespace_alias_definition ; 
	CPath using_directive ;
	CPath opaque_enum_declaration ;
	CPath init_declarator_list;
	CPath constexpr;
	CPath decl_specifier;
	CPath storage_class_specifier;
	CPath type_specifier;
	CPath function_specifier;
	CPath thread_local;
	CPath trailing_type_specifier;
	CPath enum_specifier;
	CPath elaborated_type_specifier;
	CPath cv_qualifier;
	CPath trailing_type_specifier_seq;
	CPath decltype;
	CPath enum_name;
	CPath typedef_name;
	CPath enumerator_list;
	CPath enum_head;
	CPath enum_key;
	CPath enum_base;
	CPath enumerator_definition;
	CPath enumerator;
	CPath original_namespace_name;
	CPath namespace_alias;
	CPath named_namespace_definition;
	CPath unnamed_namespace_definition;
	CPath original_namespace_definition;
	CPath extension_namespace_definition;
	CPath qualified_namespace_specifier;
	CPath namespace_body;
	CPath attribute_specifier;
	CPath attribute_list;
	CPath alignment_specifier;
	CPath alignment_expression;
	CPath attribute;
	CPath attribute_token;
	CPath attribute_scoped_token;
	CPath attribute_namespace;
	CPath balanced_token_seq;
	CPath balanced_token;
	CPath attribute_argument_clause;

	//Declarators
	CPath init_declarator;
	CPath initializer;
	CPath ptr_declarator;
	CPath noptr_declarator;
	CPath parameters_and_qualifiers;
	CPath declarator_id;
	CPath attribute_specifier_seq;
	CPath cv_qualifier_seq;
	CPath ref_qualifier;
	CPath abstract_declarator;
	CPath ptr_abstract_declarator;
	CPath noptr_abstract_declarator;
	CPath parameter_declaration_list;
	CPath parameter_declaration;
	CPath function_body;
	CPath function_try_block;
	CPath template_parameter_list;
	//class
	CPath identifier;
	CPath class_head_name;
	CPath class_virt_specifier_seq;
	CPath class_key;
	CPath class_head;
	CPath virt_specifier_seq;
	CPath virt_specifier;
	CPath base_clause;
	CPath class_name;
	CPath nested_name_specifier;
	CPath access_specifier;
	CPath simple_template_id;
	CPath member_specification;
	CPath class_specifier;
	CPath class_virt_specifier;

	//class member specification
	CPath member_declaration;
	CPath member_declarator_list;
	CPath member_declarator;
	CPath function_definition;
	CPath pure_specifier;
	CPath decl_specifier_seq;
	CPath declarator;
	CPath brace_or_equal_initializer;
	CPath constant_expression;
	CPath using_declaration;
	CPath static_assert_declaration;
	CPath template_declaration;
	CPath alias_declaration;
	
	//Derived
	CPath base_specifier_list;
	CPath base_specifier;
	CPath base_type_specifier;
	CPath class_or_decltype;
	CPath decltype_specifier;

	//Special member functions
	CPath conversion_function_id;
	CPath conversion_type_id;
	CPath type_specifier_seq;
	CPath conversion_declarator;
	CPath ptr_operator;
	CPath ctor_initializer;
	CPath mem_initializer_list;
	CPath mem_initializer;
	CPath mem_initializer_id;
	CPath expression_list;

	//Overloading
	CPath operator_token;

	//template
	CPath template_parameter;
	CPath type_parameter;
	CPath template_name;
	CPath template_argument_list;
	CPath template_argument;

	//Exception handling 
	CPath handler_seq;
	CPath handler;
	CPath dynamic_exception_specification;
	CPath type_id_list;
	CPath exception_declaration;
	CPath noexcept_specification;

	//Preprocessing directives
	CPath preprocessing_file;
	CPath group;
	CPath group_part;
	CPath if_section;
	CPath control_line;
	CPath text_line;
	CPath non_directive;
	CPath if_group;
	CPath elif_group;
	CPath new_line;
	CPath lparen;
	CPath pp_tokens;
	CPath identifier_list;
	CPath preprocessing_token;
	CPath endif_line;
	CPath elif_groups;
	CPath else_group;
	CPath replacement_list;

	//method
	CWord rKeywords();
	CWord rLex();
	CWord rBasic();
	CWord rExpressions();
	CWord rStatements();
	CWord rDeclarations();
	CWord rDeclarators();
	CWord rClass();
	CWord rDerived();
	CWord rSpecial();
	CWord rOverloading();
	CWord rTemplates();
	CWord rExcept();
	CWord rPreprocessing();

};

} //namespace wruler

#endif //WPP_BNF_CPP_H

