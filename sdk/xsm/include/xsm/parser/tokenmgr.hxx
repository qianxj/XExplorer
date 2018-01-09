	#pragma once
	
	#include <vector>
	#include <string>
	#include <set>
	#include "types.hxx"
	#include "pcodes.hxx"
 //token metatable class
	#define	__atom_AstNode_indentifier			0x01
	#define	__atom_AstNode_class				0x02
	#define	__atom_AstNode_method			0x03
	#define	__atom_AstNode_type				0x04
	#define	__atom_AstNode_declare			0x05
	#define	__atom_AstNode_expression			0x06
	#define	__atom_AstNode_namespace			0x07
	#define	__atom_AstNode_pcode				0x08
class xsourcemgr
{

private:
	//array 用于存放代码文件
		std::vector<wstring> m_sources;

public:
	//增加源代码文件
	
	 int AddSource(lptstr pSource);
	 int GetCount();
public:
	//判断代码文件是否过时
	 bool IsDirect(lptstr pSource); lptstr Read(lptstr pSource);
	//读取代码文件
	

public:
	//清空代码文件
	
	 void Reset();};

/*源代码信息*/
struct xsourceinfo
{	int source;	//源代码文件索引
		int row;     //源代码文件行

	};

class AstNode
	{
	
public:
			sym_object * pobj;
			MetaTable * pMetaTable;
		
	 AstNode();
	 virtual void Release();};
	/*
        class AstNode_namespace : public AstNode
	{
	
	};
	
	class AstNode_type : public AstNode
	{
	};
	
	class AstNode_declare  : public AstNode_type
	{
	};

	class AstNode_method : public AstNode_type
	{
	
	};
	
	class AstNode_class : public AstNode_type
	{
	};
	*/
	class AstNode_indentifier : public AstNode
	{
	
public:
			tokenitem   * name;
			sym_object * paxis;
			vector<sym_object*> objs;
			bool bref;
			bool bdeconstruct;
		
	 AstNode_indentifier();
	 const lptstr GetName();};
	
	class AstNode_code  : public AstNode
	{
	
public:
			ByteDataPool * pool;      //this is data pool
			sym_code    	 * pobj;      //this is sym pcodes 
			bool typetest; 	//only type test not product code;
	
public:
		
	  AstNode_code();
	 ~AstNode_code();
	 virtual void Release();			
	
public:
		
	 bool SetTypeTest(bool bTypeTest);
	 bool GetTypeTest();
	 void enter(Byte by,int pos = -1);
	 void enter(Byte* pdata ,int nlen,int pos=-1);
	 void enter_sub(short by);
	 void enter_index(int nindex);
	 int get_index(int &cc);
	 static int get_index(Byte* pdata, int &cc);
	 void enter_short(short val,int pos=-1);
	 void enter_int(int val,int pos=-1);
	 void save();
	 int SetUsed(int used);
	 int GetUsed();};
			
	class AstNode_pcode  : public AstNode_code
	{
	
public:
		
	 int AllocSymObject();
	 int GetSymSeq(bool bensure = false);};
	
	class AstNode_expression  : public AstNode_code 
	{
	
public:
		 	xsourceinfo xinfo; //源代码文件信息
	
public:
		
	 void enter_op(Byte op);
	 void enter_type(int ty);
	 void enter_string_literal(tchar * pname);
	 void enter_char_literal(tchar ch);
	 void enter_double_literal(double v);
	 void enter_bool_literal(bool v);
	 void enter_int_literal(int v);
	 void enter_nil();
	 void enter_this(int seq);
	 void enter_object(sym_param * pobj,int paracount = 0,bool ptcall = false,bool ref = false);
	 int AllocSymObject();
	 int GetSymSeq(bool bensure = false);};
class XSM_API CompileUnit
	{
	
private:
			MetaTable * m_pMetaTable;
	
private:
		//name of compile unit;
			lptstr m_pname;
	
public:
			const lptstr GetName()
		{
			return m_pname;
		}
	 void SetName(lptstr pname);
	 MetaTable * GetMetaTable();
public:
		
	 CompileUnit(lptstr pname);
	 ~CompileUnit();};
	
	//symmgr object
	class XSM_API CompileEnv
	{
	
public:
			std::vector<CompileUnit *> m_units; 
	
private:
			tokentab* ptokentab;
			std::vector<sym_type *>* m_pPrimaryTypes;   //used for initial primary type
	
public:
			vector<sym_object *>    m_pregions;
		//var vector<sym_object *> 	 m_pscopes;
	
public:
			int m_hCompileUnit;
	
public:
			sym_method * m_pmethod;
			void * m_pgc;
         
public:
			xsourcemgr  * m_pSourceMgr;
	
public:
		
	 CompileEnv(void);
	 ~CompileEnv(void);
public:
		
	 int SetCompileUnit(int hCompileUnit);
public:
		//alloc new compile unit use name ptoken
		
	  int AllocCompileUnit(lptstr ptoken);
	 sym_class * GetPrimaryClass(int ty);
	 AstNode * AllocAstNode(atom at , lptstr pname = null);
	 AstNode* AllocAstNode(int hCompileUnit,atom at , lptstr pname = null);
	 void FreeAstNode(AstNode* pnode );
	 void FreeAstNode(int hCompileUnit, AstNode * pnode);
	 sym_object * AllocObject(atom at,const tchar * pname, atom subat = 0);
	 sym_object * AllocObject(int hCompileUnit, atom at,const tchar * pname, atom subat = 0);
	 int FreeObject(sym_object * pobj);
	 sym_object * LookupNameEx(sym_object * pobject, tchar * pname, vector<sym_object *>* pobjects = null);
	 sym_object * LookupNameEx(int hCompileUnit,sym_object * pobject, tchar * pname, vector<sym_object *>* pobjects = null);
	 sym_object * LookupName( tchar * pname,sym_object * pNest, vector<sym_object *>* pobjects = null);
	 sym_object * LookupName(int hCompileUnit,  tchar * pname,sym_object * pNest, vector<sym_object *>* pobjects = null);
	 sym_object * LookupName( tchar * pname, vector<sym_object *>* pobjects = null);
	 sym_object * LookupName(int hCompileUnit, tchar * pname, vector<sym_object *>* pobjects = null);
	 MetaTable * GetMetaTable();
	 MetaTable * GetMetaTable(int hCompileUnit);
	 sym_object * GetSymObject(atom ty, int seq);
	 sym_object * GetSymObject(int hCompileUnit, atom ty, int seq);
	 sym_namespace * GetGlobalNS();
	 sym_namespace * GetGlobalNS(int hCompileUnit);
	  ByteDataPool* GetDataPool(int hCompileUnit);
	  MetaTable *   GetCurMetaTable();
	 sym_class * GetThisClass();
	 sym_class * GetThisClass(int hCompileUnit);
	 int GetType(sym_object* pobj);
	 int GetType(int hCompileUnit,sym_object* pobj);
	 const lptstr GetTypeName(int ntype);
	 atom GetTypeCat(int ntype);
	 atom GetTypeCat(int hCompileUnit ,int ntype);
	 const lptstr GetTypeName(int hCompileUnit, int ntype);
	 const lptstr GetTypeName(sym_type * ptype);
	 const lptstr GetTypeName(int hCompileUnit, sym_type * ptype);
	 sym_object * GetPrimaryType(int seq);
	 const lptstr GetPrimaryName(int seq);
	 const lptstr GetTokenName(tokenitem * pname);
	 const lptstr GetTokenName(int hCompileUnit, tokenitem * pname);
	 const lptstr GetTokenName(int stringPos);
	 const lptstr GetTokenName(int hCompileUnit,int stringPos);
	 const lptstr GetConstString(int stringPos);
	 const lptstr GetConstString(int hCompileUnit,int stringPos);
	 sym_method * GetClassMethod(tokenitem * pname,tokenitem * ptmethod,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetClassMethod(sym_class * pclass ,lptstr pmethodname,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetClassMethod(lptstr pclassname,lptstr pmethodname,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetSymMethod(lptstr pmethod);
	 sym_class * GetClass(lptstr pclassname);
	 void GetClassMethodString(sym_class * pclass, lptstr fct, std::set<std::wstring> * & pSet);
	 void GetNamespaceAutoString(sym_namespace * pns, std::set<std::wstring> * & pSet);
	 void GetClassAutoString(sym_class * pclass, std::set<std::wstring> * & pSet);
	 LPTSTR getVarAutoString(LPTSTR v);
	 LPTSTR getFctTipString(LPTSTR v);
	 void Free(LPTSTR p);
	 void * AllocGC();
	 void  FreeGC(void * p);};
 