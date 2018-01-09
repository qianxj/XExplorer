/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

#pragma once

#include "symmgr.hpp"
#include "errors.hpp"
#include "../vl/atoms.hpp"

namespace vl
{


	//syntax parser object
	class  VL_API parser
	{
	public:
		//bool header;
	public:
		parser(void);
		~parser(void);
	private:
		lexer*	plexer;
		symmgr* psymmgr;
	public:
		int setsymmgr(symmgr* pmgr);
		int setlexer(lexer*	p);
	public:
		int error(error::err item,bool bprocess = false);
		int error(error::err item,tchar * ext,bool bprocess = false);
		void procerror();
	public:
		int parserunit();
		int parseStmt(unit * pUnit,int bookMarkId);
		int parseBookMarkStmt(unit * pUnit);
		int parseLogicStmt(unit * pUnit,int bookMarkId);
		int parseLogicStmtItem(unit * pUnit,int bookMarkId,bool bPred = true, bool bLine = false);
		int parseBookMarkStmt(unit * pUnit,int bookMarkId);
		int parseRulerStmt(unit * pUnit,int bookMarkId);
		int parseRulerExpr(unit * pUnit,int bookMarkId);
		int parseRulerAction(unit * pUnit,int bookMarkId);
	public:
		int  GetNextItemRow(unit * pUnit, int nTokenId,int nRow=0);
		int  MakeRoute(unit * pUnit, int nTokenId, int nPath,marray &m);
		void MakeItem(unit * pUnit, int & tId, int nPath,marray &m,int nVar,int & rIndex);
		void MakeBlock(unit * pUnit, int & tId, int nPath,marray &m,int nVar,int & rIndex);
		void MakeItems(unit * pUnit, int & tId, int nPath,marray &m,int nVar,int & rIndex);
	public:
		void MakeAllRoute(unit * pUnit, int nFrom,int nTo,marray &m);
		void PrintRoute(unit * pUnit,int Route,marray &m,std::wstring &str,bool bNumber=false);
	};
}