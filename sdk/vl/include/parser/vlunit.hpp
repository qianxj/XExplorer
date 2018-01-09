#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

namespace vl
{
	class vlang;
	class vUnit
	{
	public:
		vUnit();
		~vUnit();
	private:
		int unitId_;
	public:
		void setUnitId(int u);
		int getUnitId();
	public:
		//set unit name
		void SetUnitName(tchar* pUntName);
	public:
		bool isPredWord(tchar* pToken);
	public:
		//get token id;
		int getTokenId(tchar * pToken);
		//get token ref;
		int getTokenRef(int tokenId);
		//get token parent
		int getTokenParent(int tokenId);
	public:
		int pushToken(tchar *p); 
		const tchar* getToken(int id);
		int getToken(tchar *p);
	public:
		//make stmt id;
		int makeStmt();
		//set stmt subject
		int pushStmtSubject(int stmt,int subjectId);
		//set stmt pred
		int pushStmtPred(int stmt,int predId);
		//set stmt object
		int pushStmtObject(int stmt,int objectId);
	public:
		//get subject index by stmt index
		int getIndexOfSubject(int stmt);
		//get pred index by stmt index
		int getIndexOfPred(int stmt);
		//get object index by stmt index
		int getIndexOfObject(int stmt);
	public:
		int getArObjTable();
	public:
		int addObjTable(tchar *p ,tchar * parent = null, int unitId=-1);
		int addObjTable(int id ,int pid = null, int unitId=-1);
		int getObjTableId(int tokenid, int parentId=-1,int unitId=-1);
	public:
		int checkObject(int tokenid,int unitId=-1);
	private:
		vlang * vlang_;
	public:
		vlang * getVlang();
		void setVlang(vlang * v);
	};
}