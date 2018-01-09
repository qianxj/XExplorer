	#pragma once
	#include "gen.hxx"
	#include "../parser/pcodes.hxx"
class pcode
{
};

class XString 
{

public:
	
	 static lptstr stringDup(lptstr pstr);
	 static int stringcmp(lptstr pstr1,lptstr pstr2);
	 static lptstr stringCat(lptstr pstr1,lptstr  pstr2);
	 inline static int find(lptstr pstr,lptstr pfind,int pos = 0);
	 inline static tchar * left(lptstr pstr, int len);
	 inline static tchar * right(lptstr pstr, int len);
	 inline static tchar * space(lptstr pstr, int len);
	 inline static tchar * mid(lptstr pstr, int pos,int len);
	 inline static int toInt(lptstr pstr);
	 inline static double toDouble(lptstr pstr);
	 inline static lptstr lower(lptstr pstr);
	 inline static lptstr upper(lptstr pstr);
	 inline static tchar at(lptstr pstr, int pos);
	 inline static int length(lptstr pstr);
	 inline static lptstr castFromNumber(int number,int radix);
	 inline static lptstr castFromDouble(double number,int round );};
class xint
{

public:
	
	 static lptstr toString(int v);};
class xdouble
{

public:
	
	 static lptstr toString(double v);
	 static lptstr toString1(float v);
	 static int toInt(double v);
	 static int toInt1(float v);
	 static double abs(double v);
	 static float abs1(float v);};

class xVariant
{

public:
	
	 void ConvertVarTypeToString(VARTYPE vt, WCHAR *pwszType, ULONG cchType );
	 static const lptstr  CastVariantToString(VARIANT &v);
	 static const int  CastVariantToInt(VARIANT &v);
	 static const double  CastVariantToDouble(VARIANT &v);} ;
	
class XSM_API gener_tpp : public gener
{	struct   FAllocItem
	{
		void* p;       //point to memory
		int m_ref;
		int ntype;
		int dims;
		dword dwFlag;
		FAllocItem():ntype(0),dims(0),m_ref(0),dwFlag(0),p(null)
		{
		}
	};

	//var static VARIANT	m_var;
	
public:
	
	  gener_tpp();
	 ~gener_tpp();
private:
			void* m_ExecPool;

private:
	
	 wstring adjust(wstring src, tchar sp[] );
public:
		
	
	  virtual BSTR  DeComplier();
	 virtual BSTR  DeComplier(int hCompileUnit);
	  virtual int DeComplier(int hCompileUnit,sym_object * pobj, tchar sp[] ,wstring & str);  int disasmexpitem(MetaTable * pMetaTable, sym_method  *pcall,sym_expression* pexp, Byte * pdata,atom typ,int & cc, std::wstring & disstr);  int disasmexp(MetaTable * pMetaTable, sym_method  *pcall, atom typ,int index, std::wstring & disstr,int &label,byte* pdata = null,int cz=0,int rx = 0);  int disasmexp_pcode(MetaTable * pMetaTable, sym_method  *pcall, atom typ,int index, std::wstring & disstr);  int diasmitem_pcode( MetaTable * pMetaTable, sym_method  *pcall,sym_pcode * pcode, Byte * pdata, atom typ,int & cc, std::wstring & disstr,int &label);  int diasmitem( MetaTable * pMetaTable, sym_method  *pcall,sym_pcode * pcode, Byte * pdata, atom typ,int & cc, std::wstring & disstr,int &label); wstring diasm(int hCompileUnit,  sym_method  *pcall,atom typ);  wstring DumpCodes(int hCompileUnit,  sym_method  *pcall);
	 static IDispatch * OleCreateInstance(lptstr pitem);
	 static int OleAddRef(IUnknown * pobj);
	 static int OleRelease(IUnknown * pobj);
	 static int OleGetPropertyByName(IDispatch * pDispath,tchar *pvar,VARIANT &val);
	  static void  FreeObject( gener_tpp * pg, void * p);
	 static void * AllocType(int size, int ntype,int dims);
	 int disasmexp_itemAddr(void * pMasm, MetaTable* pMetaTable, int level, int regr,int cc, int &rx, int drx, int &count, Byte * pdata,int & ty,std::vector<int> & typs,std::vector<int> & stack );
	 int disasmexp_itemSimple(void * pMasm, MetaTable* pMetaTable, int level, int regr,int cc, int &rx, int drx, int &count, Byte * pdata,int & ty,std::vector<int> & typs,std::vector<int> & stack );
	 int disasmexp_itemType(void * pMasm, MetaTable* pMetaTable, int level,int count,std::vector<int> & stack,Byte * pdata,std::vector<int> & typs );
	 int disasmexp_item(void * pMasm, MetaTable* pMetaTable, int level, int regr,int cc, int &rx, int drx, int &count, Byte * pdata,int & ty,int &drxm, std::vector<int> & typs,std::vector<int> & stack );
	 int disasmexp_itemStore(void * pMasm, MetaTable* pMetaTable, int level, int regr,int cc, int &rx, int drx, int &count, Byte * pdata,int & ty,std::vector<int> & typs,std::vector<int> & stack );
	 int disasmexp_itemPush(void * pMasm, MetaTable* pMetaTable, sym_param * pa, int level, int regr,int cc, int &rx, int drx, int &count, Byte * pdata,int & ty,std::vector<int> & typs,std::vector<int> & stack,bool bnative=false ,bool byref=false,int vt = 0);
	 int disasmexp_itemFinishEx(int cx, int drx, Byte * pdata,	int &rx,	int &crx,	int &count,	std::vector<int> & stack);
	 int disasmexp_itemFinish(void * pMasm,	int cx, int drx, Byte * pdata,	int &rx, int & ty,	int &rxm,	int	&drxm,	int &crx,int &crxm,	int &count,	std::vector<int> & stack,std::vector<int> & typs);
	 int disasmexp_itemEat(Byte * pdata, int &cc, int crxm);
	 int disasmexp_ncode(void * pMasm, MetaTable * pMetaTable, sym_method  *pcall, atom typ,int index, int &label,byte* pdata = null,int cz=0,int rx = 0,int rxm=0);
	 int diasmitem_ncode(void * pMasm, MetaTable * pMetaTable, sym_method  *pcall,sym_pcode * pcode, Byte * pdata, atom typ,int & cc, int &label,void * jcontinue,  void * jbreak,void *jreturn);
	 int diasm_ncode(int hCompileUnit,  sym_method  *pcall,atom typ,bool bdefault=true);
	 int Filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
	 void initialPool();
	 virtual BSTR Exec(lptstr pclass,lptstr pfunction,void* pnative = null);
	 virtual BSTR Exec();
	
	
	
	
	
	
	
	};