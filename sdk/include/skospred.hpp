#ifndef SKOSPRED_H
#define SKOSPRED_H

#include "pred.hpp"

namespace skos //http://www.w3.org/2002/07/owl#
{
	//概念之间的关联关系
	decl_pred_(Concept,				"skos:Concept"); //概念
	decl_pred_(ConceptScheme,		"skos:ConceptScheme"); //概念模式
	decl_pred_(inScheme,				"skos:inScheme"); //关联模式
	decl_pred_(hasTopConcept,		"skos:hasTopConcept"); //顶级概念
	decl_pred_(topConceptOf,			"skos:topConceptOf"); //关联顶级概念
	decl_pred_(altLabel,				"skos:altLabel"); //可选名称
	decl_pred_(hiddenLabel,			"skos:hiddenLabel");//不显示的名称
	decl_pred_(prefLabel,			"skos:prefLabel");//优先选用名称
	decl_pred_(notation,				"skos:notation"); //标记
	decl_pred_(changeNote,			"skos:changeNote");//关于修改的备注
	decl_pred_(definition,			"skos:definition"); //定义
	decl_pred_(editorialNote,		"skos:editorialNote");//编辑的备注
	decl_pred_(example,				"skos:example"); //例子
	decl_pred_(historyNote,			"skos:historyNote");//关于历史的备注
	decl_pred_(note,					"skos:note"); //备注
	decl_pred_(scopeNote,			"skos:scopeNote"); //范围备注
	decl_pred_(broader,				"skos:broader"); //宽化 broader in meaning (i.e. more general)
	decl_pred_(broaderTransitive,	"skos:broaderTransitive");//可传递宽化
	decl_pred_(narrower,				"skos:narrower"); //窄化
	decl_pred_(narrowerTransitive,	"skos:narrowerTransitive"); //可传递窄化
	decl_pred_(related,				"skos:related"); //关联，关联是对称得(symmetric),非传递(transitivity)
	decl_pred_(Collection,			"skos:Collection"); //集合
	decl_pred_(OrderedCollection,	"skos:OrderedCollection"); //有序集合，如链表
	decl_pred_(member,				"skos:member"); //成员
	decl_pred_(memberList,			"skos:memberList"); //成员列,用于有序集合
	decl_pred_(broadMatch,			"skos:broadMatch"); //宽匹配
	decl_pred_(closeMatch,			"skos:closeMatch"); //紧匹配
	decl_pred_(exactMatch,			"skos:exactMatch"); //可传递紧匹配
	decl_pred_(narrowMatch,			"skos:narrowMatch"); //窄匹配
	decl_pred_(relatedMatch,			"skos:relatedMatch");//关联匹配
};

#endif //SKOSPRED_H