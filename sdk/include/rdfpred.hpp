#ifndef RDFPRED_H
#define RDFPRED_H

#include "pred.hpp"

namespace rdf //http://www.w3.org/1999/02/22-rdf-syntax-ns#
{
	decl_pred_(Property,		"rdf:Property"); 
	decl_pred_(XMLLiteral,	"rdf:XMLLiteral"); 
	decl_pred_(List,			"rdf:List");
	decl_pred_(Statement,	"rdf:Statement");
	decl_pred_(Seq,			"rdf:Seq"); 
	decl_pred_(Bag,			"rdf:Bag"); 
	decl_pred_(Alt,			"rdf:Alt");
	decl_pred_(type,			"rdf:type"); 

	decl_pred_(nil,			"rdf:nil"); 
	decl_pred_(subject,		"rdf:subject");
	decl_pred_(predicate,	"rdf:predicate"); 
	decl_pred_(object,		"rdf:object");
	decl_pred_(first,		"rdf:first");
	decl_pred_(rest,			"rdf:rest");
	decl_pred_(value,		"rdf:value");
};

namespace rdfs //http://www.w3.org/2000/01/rdf-schema#
{
	decl_pred_(Resource,						"rdfs:Resource");
	decl_pred_(Class,						"rdfs:Class");
	decl_pred_(Datatype,						"rdfs:Datatype");
	decl_pred_(Literal,						"rdfs:Literal");
	decl_pred_(ContainerMembershipProperty,	"rdfs:ContainerMembershipProperty");
	decl_pred_(Container,					"rdfs:Container");

	decl_pred_(subClassOf,					"rdfs:subClassOf");
	decl_pred_(subPropertyOf,				"rdfs:subPropertyOf");
	decl_pred_(comment,						"rdfs:comment");
	decl_pred_(label,						"rdfs:label");
	decl_pred_(domain,						"rdfs:domain");
	decl_pred_(range,						"rdfs:range");
	decl_pred_(seeAlso,						"rdfs:seeAlso");
	decl_pred_(isDefinedBy,					"rdfs:isDefinedBy");
	decl_pred_(member,						"rdfs:member");
	decl_pred_(value,						"rdfs:value");
};

#endif //RDFPRED_H
