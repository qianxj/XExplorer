#ifndef WPP_WPARSER_H
#define WPP_WPARSER_H

#include "pred.hpp"
//#include "tvm.hpp"
#include "parser_tr.hpp"
#include "errors.hpp"
#include "werr.hpp"
#include "wlexer.hpp"

namespace wpp {
namespace how {

struct VLANG3_API XInterParam
{
	TVm tvm;
	wMetaTable* m_pMetaTable;
	wlexer * tp;
#pragma warning( push )
#pragma warning( disable : 4251 )
	stack<void *> scanPtr;
#pragma warning( pop )
	wtoken tk;
	int inc;
	int nReg;
	wErr* errPtr;
	wtoken & getToken()
	{
		return tk;
	}
	XInterParam(wlexer & _tp):tp(&_tp),inc(0),m_pMetaTable(0),nReg(0),errPtr(0){}
	~XInterParam()
	{
		//delete m_pMetaTable;
	}
	void step(bool savetoken = false)
	{
		//if(savetoken) tokens.push(tk);
		tp->insymbol();
		if(eof())
			tk.ty = sop::undef;
		else
		{
			tk = tp->m_token;
			inc++;
		}
	}
	bool eof()
	{
		return tp->m_token.ty ? false : true;
		//return (*tp)._Myptr ? false : true;
	}

	void save()
	{
		tp->save();
		//scanPtr.push(tp->scan.getTapePtr()->_Myptr);
		//tps.push(*tp);
	}
	void restore()
	{
		tp->restore();
		/*
		*tp = tps.top();
		tps.pop();
		tk = *(*tp);

		tp->scan.getTapePtr()->_Myptr = (wchar_t*)scanPtr.top();
		scanPtr.pop();*/
	}

	void consume()
	{
		tp->pop();
		//tps.pop();
	}

	void setMetaTable(wMetaTable* meta)
	{
		 m_pMetaTable = meta;
	}

	wMetaTable* get_meta_table()
	{
		return m_pMetaTable;
	}

	wchar_t * getTokenString(wtoken &tk,wchar_t str[], int nlen)
	{
		str[0]=0;
		nlen = min(nlen - 1,tk.slen );
		if(nlen >= 1)
		{
#pragma warning( push )
#pragma warning( disable : 4996 )
			wcsncpy(str,tk.snum,nlen);
#pragma warning( pop )
			str[nlen] = 0;
		}
		return str;
	}
	int askRegister()
	{
		nReg++;
		return nReg - 1;
	}
	void releaseRegister(int r)
	{
		nReg--;
	}
};

struct AccessType
{
	const static int _public	= 0;
	const static int _protected	= 1;
	const static int _private	= 2;
};

struct ClassType
{
	const static int _undef		= 0;
	const static int _class		= 1;
	const static int _struct	= 2;
	const static int _interface	= 3;
	const static int _enum		= 4;
	const static int _union		= 5;
};

struct ProcessType
{
	const static int _unprocess   = 0;
	const static int _processed   = 1;
	const static int _processerr  = -1;
	const static int _processpart = 2;
};


struct VLANG3_API WParser
{
	static int getOpLevel(int op);
	static void error(XInterParam & p,error::err nError, wchar_t* str, wtoken& tk);

	static int getClassType(wtoken & tk);
	static int AccessType(wtoken & tk);

	static int processVarDecl(XInterParam & p);
	static int processVarDecl(XInterParam & p,wMetaTable::wMethod &m);
	static unsigned char getOpcode(unsigned char op);
	static int processSimpleExpression(XInterParam & p,wMetaTable::wMethod &m);
	static int processExpression(XInterParam & p,wMetaTable::wMethod &m);
	static int processCompStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processExpStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processJumpStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processDoStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processSwitchStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processWhileStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processIfStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processForStmt(XInterParam & p,wMetaTable::wMethod &m);
	static int processMethodBody(XInterParam & p,wMetaTable::wMethod &m);
	static int processParam(XInterParam & p,wMetaTable::wMethod &m);
	static int DeclareClass(XInterParam & p);
	static int processUnit(XInterParam & p);
	static int processDeclList(XInterParam & p, int tailToken = 0);
	static int processDecl(XInterParam & p);
	static int processNamespace(XInterParam & p);
	static int processImport(XInterParam & p);
	static int processUsing(XInterParam & p);

	static bool matchSpec(wtoken &tk);
	static bool matchType(wtoken & tk);
	static bool matchType(XInterParam & p);
	static bool matchTypeid(XInterParam & p);
	static bool matchAccess(wtoken & tk);
	static bool matchAccess(XInterParam & p);
	static bool matchClassKey(wtoken & tk);
	static bool matchClassKey(XInterParam & p);
	static bool matchClassClause(XInterParam & p);
	static bool matchClassHead(XInterParam & p);
	static bool matchClass(XInterParam & p);
	static bool matchExprStmt(XInterParam & p);
	static bool matchExpr(XInterParam & p);
	static bool matchPrimary(XInterParam & p);
	static bool matchOp1(XInterParam & p);
	static bool matchOp(wtoken &tk);
	static bool matchOp(XInterParam & p);
	static bool matchOp2(XInterParam & p);
	static bool match(wtoken &tk, int ty);
	static bool matchEof(wtoken &tk);
	static bool matchIdent(wtoken &tk);
	static bool matchIdent(XInterParam & p);
	static bool matchString(wtoken &tk);
	static bool matchString(XInterParam & p);
	static bool matchChar(wtoken &tk);
	static bool matchChar(XInterParam & p);
	static bool matchNumber(wtoken &tk);
	static bool matchNumber(XInterParam & p);
	static bool matchOperate(wtoken &tk);
	static bool matchOperate(wtoken &tk,int op);
	static bool matchOperate(XInterParam & p);
	static bool matchOperate(XInterParam & p,int op);
	static bool matchKeyword(wtoken &tk);
	static bool matchKeyword(wtoken &tk,wchar_t* kw);
	static bool matchKeyword(XInterParam & p);
	static bool matchKeyword(XInterParam & p,wchar_t* kw);
};

}} //namespace wpp::how

#endif //WPP_WPARSER_H