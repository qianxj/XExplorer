#ifndef SKOSPRED_H
#define SKOSPRED_H

#include "pred.hpp"

namespace skos //http://www.w3.org/2002/07/owl#
{
	//����֮��Ĺ�����ϵ
	decl_pred_(Concept,				"skos:Concept"); //����
	decl_pred_(ConceptScheme,		"skos:ConceptScheme"); //����ģʽ
	decl_pred_(inScheme,				"skos:inScheme"); //����ģʽ
	decl_pred_(hasTopConcept,		"skos:hasTopConcept"); //��������
	decl_pred_(topConceptOf,			"skos:topConceptOf"); //������������
	decl_pred_(altLabel,				"skos:altLabel"); //��ѡ����
	decl_pred_(hiddenLabel,			"skos:hiddenLabel");//����ʾ������
	decl_pred_(prefLabel,			"skos:prefLabel");//����ѡ������
	decl_pred_(notation,				"skos:notation"); //���
	decl_pred_(changeNote,			"skos:changeNote");//�����޸ĵı�ע
	decl_pred_(definition,			"skos:definition"); //����
	decl_pred_(editorialNote,		"skos:editorialNote");//�༭�ı�ע
	decl_pred_(example,				"skos:example"); //����
	decl_pred_(historyNote,			"skos:historyNote");//������ʷ�ı�ע
	decl_pred_(note,					"skos:note"); //��ע
	decl_pred_(scopeNote,			"skos:scopeNote"); //��Χ��ע
	decl_pred_(broader,				"skos:broader"); //�� broader in meaning (i.e. more general)
	decl_pred_(broaderTransitive,	"skos:broaderTransitive");//�ɴ��ݿ�
	decl_pred_(narrower,				"skos:narrower"); //խ��
	decl_pred_(narrowerTransitive,	"skos:narrowerTransitive"); //�ɴ���խ��
	decl_pred_(related,				"skos:related"); //�����������ǶԳƵ�(symmetric),�Ǵ���(transitivity)
	decl_pred_(Collection,			"skos:Collection"); //����
	decl_pred_(OrderedCollection,	"skos:OrderedCollection"); //���򼯺ϣ�������
	decl_pred_(member,				"skos:member"); //��Ա
	decl_pred_(memberList,			"skos:memberList"); //��Ա��,�������򼯺�
	decl_pred_(broadMatch,			"skos:broadMatch"); //��ƥ��
	decl_pred_(closeMatch,			"skos:closeMatch"); //��ƥ��
	decl_pred_(exactMatch,			"skos:exactMatch"); //�ɴ��ݽ�ƥ��
	decl_pred_(narrowMatch,			"skos:narrowMatch"); //խƥ��
	decl_pred_(relatedMatch,			"skos:relatedMatch");//����ƥ��
};

#endif //SKOSPRED_H