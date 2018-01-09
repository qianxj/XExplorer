#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"
#include <set>
#include <stack>
#include <map>

namespace vl
{
	class vArrayMgr;
	class vlang3;
	class VL_API vmlang3
	{
	public:
		vmlang3();
		~vmlang3();
	private:
		vArrayMgr* array_;
	public:
		std::vector<std::stack<unsigned char>> codes_;
		std::stack<unsigned char> code_;
		std::stack<unsigned char> meta_;
	public:
		vArrayMgr* getArrayMgr();
		void setArrayMgr(vArrayMgr* ar);
	private:
		vlang3 * vlang_;
	public:
		vlang3* setVLang(vlang3 * p);
		vlang3* getVLang();
	private:
		vlErr* error_;
	public:
		vlErr* getError();
		void setError(vlErr* p);
	public:
		int hashToken(const tchar * p);
		const tchar * getHashToken(int tk);
	public:
		bool parser(tchar * src,int pl,int row = 0);
	public:
		//int vmeval(tchar * src,fnFetchData fn= null,void* param=null);
	public:
		//int unAsm(int pl,fnFetchData fn= null,void* param=null);
	public:
		int emit(int pcode,	unsigned char code,unsigned char ntyp ,unsigned char flag);
		int emit(int pcode,	unsigned char code);
		int emit(int pcode,	unsigned char code,unsigned char ntyp ,unsigned char flag,short d);
		int emitu(int pcode, int index, unsigned char code,unsigned char ntyp ,unsigned char flag,short d);
		int emit1(int pcode, int item);
		int emit1(int pcode, const tchar * item);
		int emit1(int pcode, double d);
		int emitx(int pcode,	unsigned char code,unsigned char ntyp ,unsigned char flag);
		int emitx(int pcode,	unsigned char code);
		int emitx(int pcode,	unsigned char code,unsigned char ntyp ,unsigned char flag,short d);
		int emitx1(int pcode, int item);
		int emitx1(int pcode, const tchar * item);
		int emitx1(int pcode, double d);
	public:
		void enter(int pv,unsigned char by,int pos = -1     );
		void enter(int pv,unsigned char* pdata ,int nlen,int pos = -1  );
		void enter_sub(int pv,short by);
		void enter_index(int pv,int nindex);

		int get_index(int pv,int &cc);
		int get_index(int pv,unsigned char* pdata, int &cc);
		void enter_short(int pv,short val,int pos = -1  );
		void enter_int(int pv,int val,int pos = -1  );
	public:
		enum pv{ pv_declcode,pv_env,pv_globalns,pv_namespace,pv_class,pv_method,pv_methodex,pv_classex,pv_indentprocessed,
				 pv_nest,pv_tkindent,pv_indent,pv_expcode,pv_methodcode, pv_type,pv_stypackage,pv_styitem,pv_stystep,pv_testtype,pv_codestart=32};
		int emitd(int pv,unsigned char code, const tchar * item = null,int typ=0,int spec=0);
		int emitd(int pv,unsigned char code, int tk = -1,int typ=0,int spec=0);
	public:
		int execMetaCode(int nStart,int nEnd = -1);
	public:
		int vdeval(int f, int code,int tr,bool bthis, fnFetchData fn= null,void* param=null);
		int vdeval1(int code,int tp,int tv,fnFetchData fn= null,void* param=null);
	public:
		int unAsm(int pl,fnFetchData fn= null,void* param=null);
		int unAsm(int pl,int start,int len, fnFetchData fn= null,void* param=null);
		int unAsmMethod(int f,fnFetchData fn= null,void* param=null);
		int unAsmMethod(int f,int exp,unsigned char* pdata, int &cc, fnFetchData fn= null,void* param=null);
	public:
		int	 getCurAddr(int pl);
		bool setLinkAddr(int pl,int link,int addr);
		bool setLinkHear(int pl,int link);
	public:
		int gen(int f,int pl,fnFetchData fn= null,void* param=null);
		int gen(int f,int pl,int exp,unsigned char* pdata, int &cc,vector<int> & vbreak, vector<int> & vcontinue, vector<int> & vreturn,
				vector<int> & vgoto, vector<int> & vname, fnFetchData fn= null,void* param=null);
	private:	
		int appendCode(int pl,int ps,int start,int nlen);
	private:
		bool _supportole;
	public:
		void setSupportOle(bool bSupport=true);
		bool getSupportOle();
	public:
		int getArItem(int tk);
		int getArClass();
		int getArArea();
		int getArMethod();
		int getArTypeSpec();
	public:
		int call(int f ,int nargs, int tr,fnFetchData fn= null,void* param=null);
		int nativeCall(int f ,int nargs, int tr,fnFetchData fnd= null,void* param=null);
		int oleCall(int f ,int nargs, int tr,fnFetchData fnd= null,void* param=null);
		int standCall(int f ,int nargs, int tr,fnFetchData fnd= null,void* param=null);
		int methodCall(int f ,int nargs, int tr,bool bthis,fnFetchData fn= null,void* param=null);
	public:
		bool isItemPred(int nitem, int tkpred );
		bool isItemField(int nitem);
		bool isItemMethod(int nitem);
		bool isItemClass(int nitem);
		bool isItemEnum(int nitem);
		bool isItemArea(int nitem);
		bool isItemVar(int nitem);
		bool isItemParam(int nitem);
	public:
		int setParamItem(int aritem, int nitem,tchar* col,int v);
		int getParamItem(int aritem, int nitem,tchar* col);
		int getClassOfField(int field);
	public:
		int GetAlignSizeEx(int typ,int align = 4,int talign=1);
		int GetAlignSizeStatic(int typ,int align = 4,int talign=1);
		int GetAlignSizeStatic(int typ,int & msize, int align = 4,int talign=1);
		int GetAlignSizeStaticEx(int pc, int &msize,  int align=4 ,int talign=1);
		int GetAlignSize(int typ,int align = 4,int talign=1);
		int GetAlignSize(int typ,int & msize, int align = 4,int talign=1);
		int GetAlignSizeEx(int pc, int &msize, int align ,int talign);
		int GetOffsetStatic(int typ,  int seq , int align = 4,int talign=1);
		int GetOffsetStatic(int pc, int &msize, int seq , int align ,int talign);
		int GetOffset(int typ,  int seq , int align = 4,int talign=1);
		int GetOffsetEx(int pc, int &msize, int seq , int align ,int talign);
	public:
		int GetClassAlignSize(int fc, int align = 4,int talign=1);
		int GetClassAlignSize(int fc, int &msize, int align = 4,int talign=1);
		int GetFieldOffset(int field,  int align = 4,int talign=1);
	public:
		int GetClassField(int fc,tchar * pfield);
	public:
		int FindMatchedMethod(int pl ,int fc , int indent, int nargs,vector<int> & typs,int nest=-1);
		bool IsMatchedMethod(int f, int nargs,vector<int> & typs);
	public:
		int findOleChild(int isolation,int item,int tk);
		int findOleObject(int isolation,int tk);
	public:
		int findClass(const tchar * pclass);
		int findClassMethod(int fc, const tchar * mt);
		int findClassMethod(const tchar * pclass, const tchar * mt);
	public:
		int loadClass(const tchar * urlClass);
		int createClass(int fc);
		void setNativePointer(int cls, void * p);
		void * getNativePointer(int cls);
	public:
		int doEvent(int fnEvent, int nc, void * evt, int p,fnFetchData fn=null,void* param=null);
	private:
		int _pl;
	public:
		int getPl();
		void setPl(int pl);
	private:
		int _tr; //Ö´ÐÐÕ»
	public:
		int getTr();
		void setTr(int tr);
	public:
		enum eLmPred { emlang , emtype, emarea, emglobal, emclass, emfield, emmethod,
			empara,emvar,emsuper , emcount };
		int m_LangPreds[emcount];
	public:
		int initLang();
	public:
		 void GetClassMethodString(int pclass, tchar* fct, std::set<std::wstring> * & pSet);
		 //void GetNamespaceAutoString(sym_namespace * pns, std::set<std::wstring> * & pSet);
		 void GetClassAutoString(int pclass, std::set<std::wstring> * & pSet);
		 tchar * getVarAutoString(tchar * v);
		 tchar * getFctTipString(tchar * v);
		 void freeAutoString(tchar * p);
	 public:
		 int EnterMetaCode(unsigned char code,unsigned char item = 0,int val=-1);
	 public:
		 std::map<int,int> m_pClassMap;
		 int GetClassByPointer(int pt);
	};

	typedef tchar * lptstr;

	class XString 
	{
	public:
		static lptstr stringDup(lptstr pstr);
		static int stringcmp(lptstr pstr1,lptstr pstr2);
		static lptstr stringCat(lptstr pstr1,lptstr  pstr2);
		static int find(lptstr pstr,lptstr pfind,int pos = 0);
		static tchar * left(lptstr pstr, int len);
		static tchar * right(lptstr pstr, int len);
		static tchar * space(lptstr pstr, int len);
		static tchar * mid(lptstr pstr, int pos,int len);
		static int toInt(lptstr pstr);
		static double toDouble(lptstr pstr);
		static lptstr lower(lptstr pstr);
		static lptstr trim(lptstr pstr);
		static lptstr ltrim(lptstr pstr);
		static lptstr rtrim(lptstr pstr);
		static lptstr upper(lptstr pstr);
		static tchar at(lptstr pstr, int pos);
		static int length(lptstr pstr);
		static lptstr castFromNumber(int number,int radix);
		static lptstr castFromDouble(double number,int round );
	};

	class xint
	{

	public:

		static lptstr toString(int v);
	};

	class xdouble
	{
	public:
		static lptstr toString(double v);
		static lptstr toString1(float v);
		static int toInt(double v);
		static int toInt1(float v);
		static double abs(double v);
		static float abs1(float v);
	};

	/*
	class xVariant
	{
	public:
		void ConvertVarTypeToString(VARTYPE vt, WCHAR *pwszType, ULONG cchType );
		static const lptstr  CastVariantToString(VARIANT &v);
		static const int  CastVariantToInt(VARIANT &v);
		static const double  CastVariantToDouble(VARIANT &v);
	};*/
}