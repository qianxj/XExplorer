#ifndef  WPP_BNF_XPATH_H
#define  WPP_BNF_XPATH_H

#include "ruler_bnf.hpp"
#include "ruler_path.hpp"

namespace wruler{

class VLANG3_API RulerEngine_XPATH : public RulerEngine_BNF
{
};

class VLANG3_API  RulerSym_XPATH : public RulerSym_BNF
{
public:
	RulerSym_XPATH();
	void initial();
public:
	CWord rPath();
public:
	CPath XPath;
	CPath Expr;
	CPath ExprSingle;
	CPath ForExpr;
	CPath QuantifiedExpr;
	CPath IfExpr;
	CPath OrExpr;
	CPath SimpleForClause;
	CPath AndExpr;
	CPath ComparisonExpr;
	CPath RangeExpr;
	CPath AdditiveExpr;
	CPath MultiplicativeExpr;
	CPath UnionExpr;
	CPath IntersectExceptExpr;
	CPath InstanceofExpr;
	CPath TreatExpr;
	CPath CastableExpr;
	CPath CastExpr;
	CPath UnaryExpr;
	CPath ValueExpr;
	CPath GeneralComp;
	CPath ValueComp;
	CPath NodeComp;
	CPath PathExpr;
	CPath RelativePathExpr;
	CPath StepExpr;
	CPath AxisStep;
	CPath ForwardStep;
	CPath ForwardAxis;
	CPath AbbrevForwardStep;
	CPath ReverseStep;
	CPath ReverseAxis;
	CPath AbbrevReverseStep;
	CPath NodeTest;
	CPath NameTest;
	CPath Wildcard;
	CPath FilterExpr;
	CPath PredicateList;
	CPath Predicate;
	CPath PrimaryExpr;
	CPath Literal;
	CPath NumericLiteral;
	CPath VarRef;
	CPath VarName;
	CPath ParenthesizedExpr;
	CPath ContextItemExpr;
	CPath FunctionCall;
	CPath SingleType;
	CPath SequenceType;
	CPath OccurrenceIndicator;
	CPath ItemType;
	CPath AtomicType;
	CPath KindTest;
	CPath AnyKindTest;
	CPath DocumentTest;
	CPath TextTest;
	CPath CommentTest;
	CPath PITest;
	CPath AttributeTest;
	CPath AttribNameOrWildcard;
	CPath SchemaAttributeTest;
	CPath AttributeDeclaration;
	CPath ElementTest;
	CPath ElementNameOrWildcard;
	CPath SchemaElementTest;
	CPath ElementDeclaration;
	CPath AttributeName;
	CPath ElementName;
	CPath TypeName;

	CPath QName;
	CPath NCName;
	CPath StringLiteral;
	CPath IntegerLiteral;
	CPath DecimalLiteral;
	CPath DoubleLiteral;
};


} //namepsace wruler

#endif //WPP_BNF_XPATH_H