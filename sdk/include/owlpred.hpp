#ifndef OWLPRED_H
#define OWLPRED_H

#include "pred.hpp"

namespace owl //http://www.w3.org/2002/07/owl#
{
	decl_pred_(AllDifferent,				"owl:AllDifferent");//ȫ������
	decl_pred_(AllDisjointClasses,		"owl:AllDisjointClasses");//ȫ������
	decl_pred_(AllDisjointProperties,	"owl:AllDisjointProperties");//ȫ������
	decl_pred_(allValuesFrom,			"owl:allValuesFrom"); //ȫ������
	decl_pred_(annotatedProperty,		"owl:annotatedProperty");
	decl_pred_(annotatedSource,			"owl:annotatedSource");
	decl_pred_(annotatedTarget,			"owl:annotatedTarget");
	decl_pred_(Annotation,				"owl:Annotation");
	decl_pred_(AnnotationProperty,		"owl:AnnotationProperty");
	decl_pred_(assertionProperty,		"owl:assertionProperty"); //����ָ��
	decl_pred_(AsymmetricProperty,		"owl:AsymmetricProperty"); //�ǶԳ�������
	decl_pred_(Axiom,					"owl:Axiom"); //����
	decl_pred_(backwardCompatibleWith,	"owl:backwardCompatibleWith"); //����
	decl_pred_(bottomDataProperty,		"owl:bottomDataProperty"); //����������
	decl_pred_(bottomObjectProperty,		"owl:bottomObjectProperty"); //�׶�������
	decl_pred_(cardinality,				"owl:cardinality"); //����
	decl_pred_(Class,					"owl:Class");			//ʵ������
	decl_pred_(complementOf,				"owl:complementOf"); //������	
	decl_pred_(DataRange,				"owl:DataRange");	//ֵ��
	decl_pred_(datatypeComplementOf,		"owl:datatypeComplementOf"); //������
	decl_pred_(DatatypeProperty,			"owl:DatatypeProperty"); //������������
	decl_pred_(DataPropertyAssertion,	"owl:DataPropertyAssertion");//��������ָ��
	decl_pred_(DataPropertyDomain,		"owl:DataPropertyDomain");//����������
	decl_pred_(DataPropertyRange,		"owl:DataPropertyRange");//����������
	decl_pred_(deprecated,				"owl:deprecated"); //����
	decl_pred_(DeprecatedClass,			"owl:DeprecatedClass"); //������
	decl_pred_(DeprecatedProperty,		"owl:DeprecatedProperty"); //��������
	decl_pred_(differentFrom,			"owl:differentFrom");
	decl_pred_(differentIndividuals,		"owl:differentIndividuals");//��ͬ����
	decl_pred_(disjointUnionOf,			"owl:disjointUnionOf"); //�����ϲ�
	decl_pred_(disjointWith,				"owl:disjointWith");	//��...����	
	decl_pred_(distinctMembers,			"owl:distinctMembers");
	decl_pred_(equivalentClass,			"owl:equivalentClass"); //�ȼ���
	decl_pred_(equivalentProperty,		"owl:equivalentProperty"); //�ȼ�����
	decl_pred_(FunctionalProperty,		"owl:FunctionalProperty"); //����������
	decl_pred_(hasKey,					"owl:hasKey");//����ֵΨһ
	decl_pred_(hasSelf,					"owl:hasSelf");//��Ӧ����ֵ���Լ�
	decl_pred_(hasValue,					"owl:hasValue");//�������ض���ֵ
	decl_pred_(imports,					"owl:imports"); //����
	decl_pred_(incompatibleWith,			"owl:incompatibleWith");
	decl_pred_(intersectionOf,			"owl:intersectionOf"); //������
	decl_pred_(InverseFunctionalProperty,"owl:InverseFunctionalProperty");
	decl_pred_(inverseOf,				"owl:inverseOf"); //���򡢷���
	decl_pred_(IrreflexiveProperty,		"owl:IrreflexiveProperty"); //���Է�����
	decl_pred_(maxCardinality,			"owl:maxCardinality"); //�������
	decl_pred_(maxQualifiedCardinality,	"owl:maxQualifiedCardinality");
	decl_pred_(members,					"owl:members");
	decl_pred_(minCardinality,			"owl:minCardinality"); //��С����
	decl_pred_(minQualifiedCardinality,	"owl:minQualifiedCardinality");
	decl_pred_(NamedIndividual,			"owl:NamedIndividual"); //Ԫ�ظ���
	decl_pred_(NegativeDataPropertyAssertion,"owl:NegativeDataPropertyAssertion");//������������ָ��
	decl_pred_(NegativePropertyAssertion,"owl:NegativePropertyAssertion"); //��������ָ��
	decl_pred_(Nothing,					"owl:Nothing");
	
	/*
	decl_pred_(ObjectProperty,			"owl:ObjectProperty"); //��������
	decl_pred_(ObjectPropertyDomain,		"owl:ObjectPropertyDomain"); //�������Զ�����
	decl_pred_(ObjectPropertyRange,		"owl:ObjectPropertyRange"); //��������ֵ��
	decl_pred_(ObjectIntersectionOf,		"owl:ObjectIntersectionOf");//��������
	decl_pred_(ObjectComplementOf,		"owl:ObjectComplementOf");//��������
	decl_pred_(ObjectUnionOf,			"owl:ObjectUnionOf");//��������
	decl_pred_(ObjectSomeValuesFrom,		"owl:ObjectSomeValuesFrom"); //��������(existential quantification)
	decl_pred_(ObjectAllValuesFrom,      "owl:ObjectAllValuesFrom"); //ȫ������(universal quantification) 
	decl_pred_(ObjectHasValue,			"owl:ObjectHasValue");	//�������ض���ֵ
	decl_pred_(ObjectHasSelf,			"owl:ObjectHasSelf"); //��Ӧ����ֵ���Լ�
	//ObjectMaxCardinality ObjectMaxCardinality
	*/

	decl_pred_(onClass,					"owl:onClass");
	decl_pred_(onDataRange,				"owl:onDataRange");
	decl_pred_(onDatatype,				"owl:onDatatype");
	decl_pred_(oneOf,					"owl:oneOf");  //oneof ���� 
	decl_pred_(onProperty,				"owl:onProperty");
	decl_pred_(onProperties,				"owl:onProperties");
	decl_pred_(Ontology,					"owl:Ontology"); //����
	decl_pred_(OntologyProperty,			"owl:OntologyProperty");
	decl_pred_(priorVersion,				"owl:priorVersion");
	decl_pred_(propertyChainAxiom,		"owl:propertyChainAxiom"); //������
	decl_pred_(propertyDisjointWith,		"owl:propertyDisjointWith");//����
	decl_pred_(qualifiedCardinality,		"owl:qualifiedCardinality");//��������
	decl_pred_(ReflexiveProperty,		"owl:ReflexiveProperty");//
	decl_pred_(Restriction,				"owl:Restriction");//�ϸ�
	decl_pred_(sameAs,					"owl:sameAs"); //��ͬ
	decl_pred_(sameIndividual,			"owl:sameIndividual");//��ͬ����
	decl_pred_(someValuesFrom,			"owl:someValuesFrom");
	decl_pred_(sourceIndividual,			"owl:sourceIndividual");
	decl_pred_(SymmetricProperty,		"owl:SymmetricProperty"); //�Գ�����
	decl_pred_(targetIndividual,			"owl:targetIndividual");
	decl_pred_(targetValue,				"owl:targetValue");
	decl_pred_(Thing,					"owl:Thing"); //������
	decl_pred_(topDataProperty,			"owl:topDataProperty"); //����������
	decl_pred_(topObjectProperty,		"owl:topObjectProperty");//�����ݶ�������
	decl_pred_(TransitiveProperty,		"owl:TransitiveProperty"); //�ɴ�������
	decl_pred_(unionOf,					"owl:unionOf"); //�ϲ�����
	decl_pred_(versionInfo,				"owl:versionInfo"); //�汾��Ϣ
	decl_pred_(versionIRI,				"owl:versionIRI");//�汾IRI
	decl_pred_(withRestrictions,			"owl:withRestrictions"); //�ϸ�
};

#endif