#ifndef OWLPRED_H
#define OWLPRED_H

#include "pred.hpp"

namespace owl //http://www.w3.org/2002/07/owl#
{
	decl_pred_(AllDifferent,				"owl:AllDifferent");//全程量词
	decl_pred_(AllDisjointClasses,		"owl:AllDisjointClasses");//全程量词
	decl_pred_(AllDisjointProperties,	"owl:AllDisjointProperties");//全程量词
	decl_pred_(allValuesFrom,			"owl:allValuesFrom"); //全程量词
	decl_pred_(annotatedProperty,		"owl:annotatedProperty");
	decl_pred_(annotatedSource,			"owl:annotatedSource");
	decl_pred_(annotatedTarget,			"owl:annotatedTarget");
	decl_pred_(Annotation,				"owl:Annotation");
	decl_pred_(AnnotationProperty,		"owl:AnnotationProperty");
	decl_pred_(assertionProperty,		"owl:assertionProperty"); //属性指定
	decl_pred_(AsymmetricProperty,		"owl:AsymmetricProperty"); //非对称性属性
	decl_pred_(Axiom,					"owl:Axiom"); //公理
	decl_pred_(backwardCompatibleWith,	"owl:backwardCompatibleWith"); //兼容
	decl_pred_(bottomDataProperty,		"owl:bottomDataProperty"); //底数据属性
	decl_pred_(bottomObjectProperty,		"owl:bottomObjectProperty"); //底对象属性
	decl_pred_(cardinality,				"owl:cardinality"); //序数
	decl_pred_(Class,					"owl:Class");			//实例是类
	decl_pred_(complementOf,				"owl:complementOf"); //补运算	
	decl_pred_(DataRange,				"owl:DataRange");	//值域
	decl_pred_(datatypeComplementOf,		"owl:datatypeComplementOf"); //补运算
	decl_pred_(DatatypeProperty,			"owl:DatatypeProperty"); //数据类型属性
	decl_pred_(DataPropertyAssertion,	"owl:DataPropertyAssertion");//数据属性指定
	decl_pred_(DataPropertyDomain,		"owl:DataPropertyDomain");//数据属性域
	decl_pred_(DataPropertyRange,		"owl:DataPropertyRange");//数据属性域
	decl_pred_(deprecated,				"owl:deprecated"); //过期
	decl_pred_(DeprecatedClass,			"owl:DeprecatedClass"); //过期类
	decl_pred_(DeprecatedProperty,		"owl:DeprecatedProperty"); //过期属性
	decl_pred_(differentFrom,			"owl:differentFrom");
	decl_pred_(differentIndividuals,		"owl:differentIndividuals");//不同个体
	decl_pred_(disjointUnionOf,			"owl:disjointUnionOf"); //正交合并
	decl_pred_(disjointWith,				"owl:disjointWith");	//与...正交	
	decl_pred_(distinctMembers,			"owl:distinctMembers");
	decl_pred_(equivalentClass,			"owl:equivalentClass"); //等价类
	decl_pred_(equivalentProperty,		"owl:equivalentProperty"); //等价属性
	decl_pred_(FunctionalProperty,		"owl:FunctionalProperty"); //函数型特性
	decl_pred_(hasKey,					"owl:hasKey");//属性值唯一
	decl_pred_(hasSelf,					"owl:hasSelf");//对应属性值是自己
	decl_pred_(hasValue,					"owl:hasValue");//属性有特定的值
	decl_pred_(imports,					"owl:imports"); //并入
	decl_pred_(incompatibleWith,			"owl:incompatibleWith");
	decl_pred_(intersectionOf,			"owl:intersectionOf"); //交运算
	decl_pred_(InverseFunctionalProperty,"owl:InverseFunctionalProperty");
	decl_pred_(inverseOf,				"owl:inverseOf"); //反向、反义
	decl_pred_(IrreflexiveProperty,		"owl:IrreflexiveProperty"); //非自反属性
	decl_pred_(maxCardinality,			"owl:maxCardinality"); //最大序数
	decl_pred_(maxQualifiedCardinality,	"owl:maxQualifiedCardinality");
	decl_pred_(members,					"owl:members");
	decl_pred_(minCardinality,			"owl:minCardinality"); //最小序数
	decl_pred_(minQualifiedCardinality,	"owl:minQualifiedCardinality");
	decl_pred_(NamedIndividual,			"owl:NamedIndividual"); //元素个体
	decl_pred_(NegativeDataPropertyAssertion,"owl:NegativeDataPropertyAssertion");//反向数据属性指定
	decl_pred_(NegativePropertyAssertion,"owl:NegativePropertyAssertion"); //反向属性指定
	decl_pred_(Nothing,					"owl:Nothing");
	
	/*
	decl_pred_(ObjectProperty,			"owl:ObjectProperty"); //对象属性
	decl_pred_(ObjectPropertyDomain,		"owl:ObjectPropertyDomain"); //对象属性定义域
	decl_pred_(ObjectPropertyRange,		"owl:ObjectPropertyRange"); //对象属性值域
	decl_pred_(ObjectIntersectionOf,		"owl:ObjectIntersectionOf");//对象交运算
	decl_pred_(ObjectComplementOf,		"owl:ObjectComplementOf");//对象补运算
	decl_pred_(ObjectUnionOf,			"owl:ObjectUnionOf");//对象并运算
	decl_pred_(ObjectSomeValuesFrom,		"owl:ObjectSomeValuesFrom"); //存在量词(existential quantification)
	decl_pred_(ObjectAllValuesFrom,      "owl:ObjectAllValuesFrom"); //全程量词(universal quantification) 
	decl_pred_(ObjectHasValue,			"owl:ObjectHasValue");	//属性有特定的值
	decl_pred_(ObjectHasSelf,			"owl:ObjectHasSelf"); //对应属性值是自己
	//ObjectMaxCardinality ObjectMaxCardinality
	*/

	decl_pred_(onClass,					"owl:onClass");
	decl_pred_(onDataRange,				"owl:onDataRange");
	decl_pred_(onDatatype,				"owl:onDatatype");
	decl_pred_(oneOf,					"owl:oneOf");  //oneof 集合 
	decl_pred_(onProperty,				"owl:onProperty");
	decl_pred_(onProperties,				"owl:onProperties");
	decl_pred_(Ontology,					"owl:Ontology"); //本体
	decl_pred_(OntologyProperty,			"owl:OntologyProperty");
	decl_pred_(priorVersion,				"owl:priorVersion");
	decl_pred_(propertyChainAxiom,		"owl:propertyChainAxiom"); //属性链
	decl_pred_(propertyDisjointWith,		"owl:propertyDisjointWith");//正交
	decl_pred_(qualifiedCardinality,		"owl:qualifiedCardinality");//数量序数
	decl_pred_(ReflexiveProperty,		"owl:ReflexiveProperty");//
	decl_pred_(Restriction,				"owl:Restriction");//严格化
	decl_pred_(sameAs,					"owl:sameAs"); //相同
	decl_pred_(sameIndividual,			"owl:sameIndividual");//相同个体
	decl_pred_(someValuesFrom,			"owl:someValuesFrom");
	decl_pred_(sourceIndividual,			"owl:sourceIndividual");
	decl_pred_(SymmetricProperty,		"owl:SymmetricProperty"); //对称属性
	decl_pred_(targetIndividual,			"owl:targetIndividual");
	decl_pred_(targetValue,				"owl:targetValue");
	decl_pred_(Thing,					"owl:Thing"); //基本类
	decl_pred_(topDataProperty,			"owl:topDataProperty"); //顶数据属性
	decl_pred_(topObjectProperty,		"owl:topObjectProperty");//顶数据对象属性
	decl_pred_(TransitiveProperty,		"owl:TransitiveProperty"); //可传递属性
	decl_pred_(unionOf,					"owl:unionOf"); //合并操作
	decl_pred_(versionInfo,				"owl:versionInfo"); //版本信息
	decl_pred_(versionIRI,				"owl:versionIRI");//版本IRI
	decl_pred_(withRestrictions,			"owl:withRestrictions"); //严格化
};

#endif