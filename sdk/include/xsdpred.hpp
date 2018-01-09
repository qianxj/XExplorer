#ifndef XSDPRED_H
#define XSDPRED_H

#include "pred.hpp"

namespace xsd
{
	decl_pred_(anyUrI ,			"xsd:anyUrI");
	decl_pred_(base64Binary,		"xsd:base64Binary");
	decl_pred_(boolean,			"xsd:boolean");
	decl_pred_(byte_,				"xsd:byte");
	decl_pred_(dateTime,			"xsd:dateTime");
	decl_pred_(dateTimeStamp,	"xsd:dateTimeStamp");
	decl_pred_(decimal,			"xsd:decimal");
	decl_pred_(double_,			"xsd:double");
	decl_pred_(float_,			"xsd:float");
	decl_pred_(hexBinary,		"xsd:hexBinary");
	decl_pred_(int_,				"xsd:int");
	decl_pred_(integer,			"xsd:integer");
	decl_pred_(language,			"xsd:language");
	decl_pred_(long_,			"xsd:long");
	decl_pred_(Name,				"xsd:Name");
	decl_pred_(NCName,			"xsd:NCName");
	decl_pred_(negativeInteger,	"xsd:negativeInteger");
	decl_pred_(NMTOKEN,			"xsd:NMTOKEN");
	decl_pred_(nonNegativeInteger,"xsd:nonNegativeInteger");
	decl_pred_(nonPositiveInteger,"xsd:nonPositiveInteger");
	decl_pred_(normalizedString,	 "xsd:normalizedString");
	decl_pred_(PlainLiteral,		 "rdf:PlainLiteral");
	decl_pred_(positiveInteger,	 "xsd:positiveInteger");
	decl_pred_(rational,			 "owl:rational");
	decl_pred_(real,				 "owl:real");
	decl_pred_(short_,			 "xsd:short");
	decl_pred_(string,			 "xsd:string");
	decl_pred_(token,			 "xsd:token");
	decl_pred_(unsignedByte,		 "xsd:unsignedByte");
	decl_pred_(unsignedInt,		 "xsd:unsignedInt");
	decl_pred_(unsignedLong,		 "xsd:unsignedLong");
	decl_pred_(unsignedShort,	 "xsd:unsignedShort");
	decl_pred_(XMLLiteral,		 "rdf:XMLLiteral");
};

#endif //XSDPRED_H
