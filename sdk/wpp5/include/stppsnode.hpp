#ifndef STPP_SNODE_H
#define STPP_SNODE_H

#include <vector>
#include "vltoken.hpp"

using namespace xlang;

namespace trap {
namespace complier {

	enum KindSNode : unsigned char
	{
		kIf = 1,
		kFor = 2,
		kWhile = 3,
		kUntil = 4,
		kReturn = 5,
		kContinue = 6,
		kBreak = 7,
		kCompStmt = 8,
		kExprStmt = 9,
		kItem =10,
		kThis=11,
		kSuper = 12,
		kValue = 13,
		kNill = 14,
		kField = 15,
		kElement = 16,
		kMethod = 17,
		kCall = 18,
		kArgList = 19,
		kType = 20,
		kInc  = 21,
		kDec = 22,
		kUInc = 23,
		kUDec = 24,
		kNew = 25,
		kDelete = 26,
		kNot = 27,
		kBNot = 28,
		kNeg = 29,
		kCast = 30,
		kBinary = 31,
		kList = 32,
		kParen = 33,

		kVar = 40,
		kParam = 40,

		kToken = 41
	};

	struct SNode
	{
		KindSNode kind;
		SNode(KindSNode k):kind(k){};
	};

	struct OpToken : public SNode
	{
		vlToken tk;
		OpToken(vlToken &tk):SNode(kToken),tk(tk){}
	};

	struct OpIf : public SNode
	{
		SNode* cond;
		SNode* tbody;
		SNode* fbody;
		OpIf(SNode* cond, SNode* tbody, SNode* fbody):SNode(kIf),
			cond(cond),tbody(tbody),fbody(fbody){};
	};

	struct OpBreak : public SNode
	{
		OpBreak():SNode(kBreak){};
	};

	struct OpContinue : public SNode
	{
		OpContinue():SNode(kContinue){};
	};

	struct OpExprStmt : public SNode
	{
		SNode* expr;
		OpExprStmt(SNode* expr):SNode(kExprStmt),expr(expr){}
	};

	struct OpCompStmt : public SNode
	{
		std::vector<SNode *> stmts;
		OpCompStmt():SNode(kCompStmt){}
	};

	struct OpFor : public SNode
	{
		SNode * init;
		SNode * cond;
		SNode * inc;
		SNode * body;

		OpFor(SNode* init, SNode* cond, SNode* inc, SNode* body):SNode(kFor),
		init(init),cond(cond),inc(inc),body(body){}
	};

	struct OpWhile : public SNode
	{
		SNode * cond;
		SNode * body;
		OpWhile(SNode * cond, SNode * body):SNode(kWhile),
		cond(cond),body(body){}
	};

	struct OpReturn : public SNode
	{
		SNode * expr;
		OpReturn():SNode(kReturn){};
		OpReturn(SNode* expr):SNode(kReturn),expr(expr){}
	};


	struct OpUntil : public SNode
	{
		SNode * cond;
		SNode * body;
		OpUntil(SNode * cond, SNode * body):SNode(kUntil),
		cond(cond),body(body){}
	};

	struct OpValue : public SNode
	{
		enum ValueType
		{
			I64 = 1,
			I32 = 2,
			I16 = 3,
			I8	= 4,
			UI64 = 5,
			UI32 = 6,
			UI16 = 7,
			UI8  = 8,
			D64 = 9,
			D32 = 10,
			STR = 11,
			ASTR = 12,
			BOOL = 13,
			PTR  = 14,
			WCHAR = 15
		};

		ValueType vt;
		union 
		{
			__int64				val_i64;
			int					val_i32;
			short				val_i16;
			char				val_i8;
			unsigned __int64	val_ui64;
			unsigned int		val_ui32;
			unsigned short		val_ui16;
			unsigned char		val_ui8;

			double				val_d64;
			float				val_d32;

			char *				val_str;
			wchar_t*			val_wstr;

			bool				val_bool;
			void *				val_ptr;
		};
		OpValue(int val):SNode(kValue),vt(I32),val_i32(val){}
		OpValue(unsigned int val):SNode(kValue),vt(UI32),val_ui32(val){}
		OpValue(short val):SNode(kValue),vt(I16),val_i16(val){}
		OpValue(unsigned short val):SNode(kValue),vt(UI16),val_ui16(val){}
		OpValue(char val):SNode(kValue),vt(I8),val_i8(val){}
		OpValue(unsigned char val):SNode(kValue),vt(UI8),val_ui8(val){}
		OpValue(__int64 val):SNode(kValue),vt(I64),val_i64(val){}
		OpValue(unsigned __int64 val):SNode(kValue),vt(UI64),val_ui64(val){}
		OpValue(double val):SNode(kValue),vt(D64),val_d64(val){}
		OpValue(float val):SNode(kValue),vt(D32),val_d32(val){}

		OpValue(char* val):SNode(kValue),vt(ASTR),val_str(val){}
		OpValue(wchar_t* val):SNode(kValue),vt(STR),val_wstr(val){}

		OpValue(void* val):SNode(kValue),vt(PTR),val_ptr(val){}
		OpValue(bool val):SNode(kValue),vt(BOOL),val_bool(val){}
	};

	struct OpItem : public SNode
	{
		int ident;
		OpItem(int ident):SNode(kItem),ident(ident){}
	};

	struct	OpThis : public SNode
	{
		OpThis(): SNode(kThis){}
	};

	struct	OpSuper : public SNode
	{
		OpSuper(): SNode(kSuper){}
	};

	struct	OpNill : public SNode
	{
		OpNill(): SNode(kNill){}
	};

	struct OpMethodPtr : public SNode
	{
		SNode* typ;
		SNode* args;
		SNode* base;
		OpMethodPtr(SNode* typ, SNode* base) : SNode(kMethod),
			typ(typ),base(base){}
	};

	struct OpCall  : public SNode
	{
		SNode * typ;
		SNode * mt;
		SNode * args;
		OpCall(SNode* typ, SNode* mt, SNode* args):SNode(kCall),
			typ(typ),mt(mt),args(args){}
	};

	struct OpArgList : public SNode
	{
		std::vector<SNode*> args;
		OpArgList():SNode(kArgList){}
	};

	struct OpField : public SNode
	{
		SNode * typ;
		SNode * base;
		SNode * hitem;
		OpField(SNode* typ, SNode* base, SNode* hitem) :SNode(kField),
			typ(typ),base(base),hitem(hitem){}
	};

	struct OpElement : public SNode
	{
		SNode * typ;
		SNode * rank;
		SNode * exp;
		SNode * idx;
		OpElement(SNode* typ, SNode* rank, SNode* exp, SNode* idx) : SNode(kElement),
			typ(typ),rank(rank),exp(exp),idx(idx){}
	};

	struct OpInc : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpInc(SNode* typ, SNode* expr) : SNode(kInc),
			typ(typ),expr(expr){}
	};
	
	struct OpDec : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpDec(SNode* typ, SNode* expr) : SNode(kDec),
			typ(typ),expr(expr){}
	};

	struct OpUInc : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpUInc(SNode* typ, SNode* expr) : SNode(kUInc),
			typ(typ),expr(expr){}
	};

	struct OpUDec : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpUDec(SNode* typ, SNode* expr) : SNode(kUDec),
			typ(typ),expr(expr){}
	};

	struct OpNeg : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpNeg(SNode* typ, SNode* expr) : SNode(kNeg),
			typ(typ),expr(expr){}
	};

	struct OpNew : public SNode
	{
		SNode * typ;
		SNode * rank;
		OpNew(SNode* typ, SNode* rank) : SNode(kNew),
			typ(typ),rank(rank){}
	};

	struct OpDelete : public SNode
	{
		SNode * exp;
		OpDelete(SNode* exp) :SNode(kDelete){}
	};

	struct OpNot : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpNot(SNode* typ, SNode* expr) : SNode(kNot),
			typ(typ),expr(expr){}
	};

	struct OpBNot : public SNode
	{
		SNode * typ;
		SNode * expr;
		OpBNot(SNode* typ, SNode* expr) : SNode(kBNot),
			typ(typ),expr(expr){}
	};
	
	struct OpCast : public SNode
	{
		SNode * typ;
		SNode * expr;
		SNode* typFrom;
		OpCast(SNode* typ, SNode* typFrom, SNode* expr) :SNode(kCast),
			typ(typ),typFrom(typFrom),expr(expr){}
	};

	struct OpBinary : public SNode
	{
		enum Op
		{
			add		= 0x1,
			sub		= 0x2,
			mul		= 0x3,
			div		= 0x4,
			shl		= 0x5,
			shr		= 0x6,
			asn		= 0x7,
			mod		= 0x8,
			or		= 0x12,
			and		= 0x13,
			band	= 0x15,
			bor		= 0x16,
			xor		= 0x18,
			eq		= 0x20,
			neq		= 0x21,
			gt		= 0x22,
			gte		= 0x23,
			lt		= 0x24,
			lte		= 0x25,

			ref		 = 0x2e,
			pointer	 = 0x3A,
			mref	 = 0x3E,
			mpointer = 0x3F,
			varrow	 = 0x40,
			arrow	 = 0x42
		};

		Op op;
		SNode* typ;
		SNode* lhs;
		SNode* rhs;
		OpBinary(Op op, SNode* typ, SNode* lhs, SNode* rhs) :SNode(kBinary),
			op(op),typ(typ),lhs(lhs),rhs(rhs){}
	};

	struct OpList : public SNode
	{
		OpList* next;
		SNode * node;
		OpList():SNode(kList),next(0),node(0){}
		OpList(SNode * node):SNode(kList),next(0),node(node){}
		OpList(OpList* next, SNode * node):SNode(kList),next(next),node(node){}
	};

	struct OpVar : public SNode
	{
		int ident;
		SNode * typ;
		SNode * expr;
		OpVar(int ident, SNode* typ, SNode * expr) : SNode(kVar),
			ident(ident),typ(typ),expr(expr){}
	};

	struct OpType : public SNode
	{
		unsigned int htype;
		OpType(int htype) :SNode(kType),htype(htype){}
	};


	inline OpIf* opIf(SNode* cond, SNode* tbody, SNode* fbody)
	{
		return new OpIf(cond,tbody,fbody); 
	}

	inline OpFor* opFor(SNode* init, SNode* cond, SNode* inc, SNode* body)
	{
		return new OpFor(init,cond,inc,body);
	}

	inline OpWhile* opWhile(SNode* cond, SNode* body)
	{
		return new OpWhile(cond,body);
	};

	inline OpUntil* opUntil(SNode* cond, SNode* body)
	{
		return new OpUntil(cond,body);
	};

	inline OpReturn* opReturn(SNode* exp)
	{
		return new OpReturn(exp);
	};

	inline OpContinue* opContinue()
	{
		return new OpContinue();
	};

	inline OpBreak* opBreak()
	{
		return new OpBreak();
	};

	inline OpCompStmt* opCompStmt()
	{
		return new OpCompStmt();
	};

	inline OpExprStmt* opExprStmt(SNode* exp)
	{
		return new OpExprStmt(exp);
	}
	
	inline OpValue* opValue(SNode* typ, SNode* lt)
	{
		//new OpValue(typ,lt);
	};

	inline OpValue* opValue(int val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(unsigned int val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(short val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(unsigned short val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(char val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(unsigned char val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(__int64 val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(unsigned __int64 val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(double val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(float val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(char* val)
	{
		return new OpValue(val);
	}
	inline OpValue* opValue(wchar_t* val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(void* val)
	{
		return new OpValue(val);
	}

	inline OpValue* opValue(bool val)
	{
		return new OpValue(val);
	}

	inline OpItem* opItem(int ident)
	{
		return new OpItem(ident);
	};

	inline OpItem* opItem(SNode* typ, SNode* hitem)
	{
		//new OpItem(typ,hitem);
	};

	inline OpThis* opThis()
	{
		return new OpThis();
	};

	inline OpSuper* opSuper()
	{
		return new OpSuper();
	};

	inline OpNill* opNill()
	{
		return new OpNill();
	};

	inline OpMethodPtr* opMethodPtr(SNode* typ, SNode* base)
	{
		return new OpMethodPtr(typ,base);
	};

	inline OpCall* opCall(SNode* typ, SNode* mt, SNode* args)
	{
		return new OpCall(typ,mt,args);
	};

	inline OpArgList* opArgList()
	{
		return new OpArgList();
	};

	inline OpField* opField(SNode* typ, SNode* base, SNode* hitem)
	{
		return new OpField(typ,base,hitem);
	};

	inline OpElement* opElement(SNode* typ, SNode* rank, SNode* exp, SNode* idx)
	{
		return new OpElement(typ,rank,exp,idx);
	};

	inline OpInc* opInc(SNode* typ, SNode* exp)
	{
		return new OpInc(typ,exp);
	};
	inline OpDec* opDec(SNode* typ, SNode* exp)
	{
		return new OpDec(typ,exp);
	};

	inline OpUInc* opUInc(SNode* typ, SNode* exp)
	{
		return new OpUInc(typ,exp);
	};

	inline OpUDec* opUDec(SNode* typ, SNode* exp)
	{
		return new OpUDec(typ,exp);
	};

	inline OpNeg* opNeg(SNode* typ, SNode* exp)
	{
		return new OpNeg(typ,exp);
	};

	inline OpNew* opNew(SNode* typ, SNode* rank)
	{
		return new OpNew(typ,rank);
	};

	inline OpDelete* opDelete(SNode* exp)
	{
		return new OpDelete(exp);
	};

	inline OpNot* opNot(SNode* typ, SNode* exp)
	{
		return new OpNot(typ,exp);
	};

	inline OpBNot* opBNot(SNode* typ, SNode* exp)
	{
		return new OpBNot(typ,exp);
	};
	
	inline OpCast* opCast(SNode* typ, SNode* typFrom, SNode* exp)
	{
		return new OpCast(typ,typFrom,exp);
	};

	inline OpVar* opVar(int ident,SNode * typ, SNode*  expr)
	{
		return new OpVar(ident,typ,expr);
	}

	inline OpBinary* opBinary(OpBinary::Op op, SNode* typ, SNode* lhs, SNode* rhs)
	{

		return new OpBinary(op,typ,lhs,rhs);
	};

	inline OpToken* opToken(vlToken &tk)
	{
		return new OpToken(tk);
	}

	inline OpType* opType(int htype)
	{
		return new OpType(htype);
	}
} // namespace complier
} // namespace trap


#endif //STPP_SNODE_H