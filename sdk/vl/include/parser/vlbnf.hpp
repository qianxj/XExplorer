#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

namespace vl
{
	class vArrayMgr;
	class vlang3;
	//bnf manager
	class vlbnf
	{
	public:
		vlbnf(vlang3 * lv);
		~vlbnf();
	public:
		enum Field {arNameID , arTokenID, arKeywordID,arStmtID, arObjectID, arCount};
		//lang array ,every element have fields nameid, tokenid,keywordid,stmtid,objectid
		//row 0 for save fields arrayid
		int m_arlang;
		//current parse lang index in m_arlang
		int m_langid; 
	public:
		vArrayMgr* getArrayMgr();
		int AddLang(tchar * p);
	public:
		int pushToken(tchar *p); //start from 1
		const tchar* getToken(int id); //start from 1
		int getToken(tchar *p);
	public:
		//get array id by item
		int getArName(bool make=true);
		int getArToken(bool make=true);
		int getArKeyword(bool make=true);
		int getArStmt(bool make=true);
		int getArObject(bool make=true);
		int getArItem(Field item, bool make=true);
	public:
		//make stmt id;
		int makeStmt();
		//set stmt subject
		int pushStmtSubject(int stmt,int subjectId);
		//set stmt pred
		int pushStmtPred(int stmt,int predId);
		//set stmt object
		int pushStmtObject(int stmt,int objectId);
		int pushStmtObjectItemEof(int stmt);
		int pushStmtObjectStmtEof(int stmt);
	public:
		int AddTokenRef(int tokenid,int stmt,int objectid);
		int getTokenRef(int tokenid,int index,int &objectid);
		int getTokenRefCount(int tokenid,int &objectref);
	private:
		vlang3 * vlang_;
	public:
		vlang3 * getVlang();
		void setVlang(vlang3 * v);
	public:
		int determine(int token,void * param,fnFetchData fetchData ,int arCalced);
		int determine(tchar* token,void * param,fnFetchData fetchData,int arCalced);
	};
	
}