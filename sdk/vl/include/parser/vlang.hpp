#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

#include <windows.h>
#include <set>
#include <map>
#include <string>
#include "varray.hpp"

namespace vl
{
	class vArrayMgr;
	class vlbnf;
	class vUnit;

	//执行环境
	struct vlEnv
	{
		//array system isolation
		int arSystemIsolation;

		//array lib isolation
		int arLibIsolation;

		//array isolation
		int arIsolation;
	};
	
	//isolation
	struct vlisolation
	{
		//array unit
		int arUnit;
		//token for word
		int arHashToken;
		//string pool
		int arStringPool;
		//array stmt element
		int arElement;
		//array files
		int arHashFile;
	};

	enum filestate
	{
		fs_normal = 0x00,
		fs_dirty = 0x01,
		fs_test = 0x02
	};

	struct vlhashfile
	{
		int name;
		//file datetime
		FILETIME dt;
		//file flag
		int state;
		//array child
		int archild;
	};

	//struct vl unit
	struct vlunit
	{
		//unit name
		int name;
		//array stmt
		int arStmt; 
		//array stmt Element
		int arElementList;
		//array 
		int arBlock;
	};

	//struct object
	struct vlelement
	{ 
		//pointer to hashtoken row
		int tokenid;
		//pointer to arobject row
		int	parentid;	
		//pointer to arobject row
		int	predid;	
		//pointer to arunit row
		int refid;
		//array child Element
		int unitid;
		//array child Element
		int arChild;
		//array unit stmt
		int arUnitStmt;
		//spec
		int spec; //指定是项还是谓词
		//param
		int param;
		//use file list id
		int fileid;
	};

	struct vlunitstmt
	{
		//pointer unit row
		int unitid;
		//array stmt row 
		int arStmt;
	};

	//struct 
	struct vlstmt
	{
		int subject; //当末两位为零时,指向vlElement,否则指向unit中的arElementList
		int pred1;
		int pred2;
		int object; //当末两位为零时,指向vlElement,否则指向unit中的arElementList
		int spec;	//spec
	};

	//struct token hash table
	struct vlhashToken
	{
		int stringid;
		//array object ref
		int arElementRef;
	};
	
	
	class VL_API vlang3
	{
	public:
		vlang3();
		~vlang3();
	private:
		vArrayMgr* array_;
	public:
		vArrayMgr* getArrayMgr();
	private:
		vlErr* error_;
	public:
		inline vlErr* getError(){return error_;}
		inline void setError(vlErr* p){error_ = p;}
	private:
		int	vlEnv_;
	public:
		int getVlEnv();
		int makeVlEnv();
	public:
		int initial();
		int initlang(tchar * purl );
	private:
		int resid_;
	public:
		int askResId();
	private:
		//follow enum and struct for pcode;
		enum eMakeItem
		{
			eUnit = 1,
			eStmt = 2,
			eBlock = 3
		};
		struct trStack
		{
			int item;
			eMakeItem make;
		};
	public:
		struct interpretEnv
		{
			int vlEnv;
			int isolation;
			int tr;
		};
	public:
		bool parse(tchar *src);
		bool parse(interpretEnv * pEnv,tchar *src);
	public:
		int emit(int tcode,unsigned char code,const tchar* t=null);
		bool parseAsm(tchar *src);
	public:
		int processFile(interpretEnv * pEnv,tchar* file);
	public:
		interpretEnv * makeInterpretEnv();
		interpretEnv * makeInterpretEnv(int env);
	public:
		//interpretEnv * getEnv();
		int getIsolation();
	public:
		//interpret code
		int interpret(interpretEnv * pEnv, int pl);
		int interpretStep(interpretEnv * pEnv, int pl,int pc);
		int interpretLast(interpretEnv * pEnv, int pl);
	public:
		int EnsureArUnit (int isolation);
		int EnsureHashToken(int isolation);
		int EnsureArElement(int isolation);
	public:
		int EnsureArHashTokenElementRef(int arhashToken,int index);
	public:
		int EnsureArStmt (int isolation, int unit);
		int EnsureArBlock(int isolation, int unit);
		int EnsureArElementList(int isolation, int unit);
	public:
		int EnsureArElementUnitStmt(int arElement,int element);
		int EnsureArElementChild(int arElement,int element);
	private:
		int EnsureArElementUnitStmtArStmt(int arUnitStmt,int index);
	public:
		int EnsureHashFile(int isolation);
	private:
		int findHashFile(int isolation,const tchar * pfile);
	private:
		int FindUnitIndex(int arUnit ,int name);
		int GetItemForStackIndex(int tr,eMakeItem it);
	private:
		int TransTokenItemToElement(int isolation,int unit,int arstmt,int stmt,int col);
		int FindTokenItemElementAndProcessIt(int isolation,int unit,int arstmt,int stmt,int col);
		int TransTokenItemToElement(int isolation,int unit,int arstmt,int stmt,int col,int itemid);
		int SetStmtItemParent(int isolation,int unit,int arstmt,int stmt,int col, int pelement);
	public:
		int FindTokenItemElement(int isolation,int unit,int itemid,int parent=-1);
		int SetElementParent(int isolation,int element, int pelement);
	public:
		int doCmdUse	(interpretEnv * pEnv, int arg);
		int doCmdUnit	(interpretEnv * pEnv, int arg);
		int doCmdStmt	(interpretEnv * pEnv, int arg);
		int doCmdBlock	(interpretEnv * pEnv, int arg);
		int doCmdSubject(interpretEnv * pEnv, int arg);
		int doCmdPred	(interpretEnv * pEnv, int arg);
		int doCmdObject	(interpretEnv * pEnv, int arg, bool bref = false);
		int doCmdItem	(interpretEnv * pEnv, int arg);
		int doCmdPop	(interpretEnv * pEnv, int arg);
		int doCmdParent	(interpretEnv * pEnv, int arg);
	public:
		int  CheckTokenDefine(interpretEnv * pEnv,int tk);
		int  getStmtSubject(interpretEnv * pEnv,int stmt,int index);
		int  getStmtObject(interpretEnv * pEnv,int stmt,int index);
		int  getStmtPred(interpretEnv * pEnv,int stmt,int index);
		int  getCurStmt(interpretEnv * pEnv);
	public:
		const tchar * getHashToken(interpretEnv * pEnv,int tokenid);
		int hashToken(interpretEnv * pEnv,const tchar * p);
	public:
	    static tchar* (* GetSource)(tchar* purl) ;
    	static void (* FreeSource)(tchar* purl) ;
     	static FILETIME  (* GetSourceDT)(tchar* purl) ;
    	static FILETIME  (* GetSourceVER)(tchar* purl) ;
	public:
		int traceTable(fnFetchData fetchData,void* param);
	public:
		//find tk element at child
		int findChild(int isolation,int item,int tk);
		int findChild(int isolation,int item,int tk,int pred = -1);
		int findChild(int isolation,int item,int tk,int pred = -1, int lastbit=-1);
		int findChildEx(int isolation,int item,int tk,int tkpred, int lastbit=-1);
		int findChildPred(int isolation,int item,int pred);
		//find tk element at descendant
		int findDescendant(int isolation,int item,int tk);
		int findDescendant(int isolation,int item,int tk,int pred = -1);
		int findDescendant(int isolation,int item,int tk,int pred = -1, int lastbit=-1);
		int findDescendantEx(int isolation,int item,int tk,int tkpred, int lastbit=-1);
		int findDescendantPred(int isolation,int item,int pred);
	public:
		int getPred(int isolation,int tkpred,int parent);
		int getPred(int isolation,const tchar * p,int parent);
	public:
		int getItemPredToken(int isolation,int item);
		int getSourcePred(int isolation,int pred);
	public:
		//lang method
		int getDefaultLang(int isolation);
		int getLangElement(int isolation);
		int getLangElement(int isolation,int tklang);
		int getLangPred(int isolation,int tkpred,int lang=-1);
		int getGlobalNs(int isolation,int lang = -1);
		int findToken(int isolation, int item, int tk,bool bfield = false, int lang=-1);
		int findTokenPrimaryType(int isolation,int tk,int lang=-1);
		int findTokenFollowType(int isolation, int item, int tk, int lang=-1);
	private:
		void * vmlang_;
		void * stylang_;
		void * flowlang_;
	private:
		int fileid_; //current parse fileid
	private:
		int fileidDirty_; //cache firty fileid
		int fileChildId_;	//cache file child id
	public:
		int getFildId();
		int setFileId(int fileid); //set current fileid and return old fileid
	public:
		int addChildOfFile(int fileid,int ele);
		bool bFileDirty(int fileid);
		bool bElementDirty(int element);
		void setFileDirty(int fileid);
	public:
		int recompileFile(tchar * file); //recompile
	public:
		void * getLang(const tchar * plang,bool bcreate = false);
	public:
		bool query(tchar *src,void* param,fnFetchData fetchData,int typ=0);
	public:
		int test();
	public:
		int dwTimeCompFile;
		int dwTimeGetFile;
		int dwTimeFounded;
		int dwTimeUnFound;
		std::vector<std::wstring> dwTimeFiles;
		std::vector<int> dwTimeComplier;
	};
}
