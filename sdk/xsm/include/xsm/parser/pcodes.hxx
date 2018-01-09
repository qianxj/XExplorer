	#pragma once
	typedef int atom;
	#include "xsm.h"
	#include "../lexer/lexer.hxx"
	#include <tchar.h>
	#include <vector>
	using namespace std;
	#define  STR_BUCKET_SIZE (4096*64)
	#define  STR_POOL_SIZE (4096 *256)
	#define  DATA_POOL_SIZE (4096 *8)
	//#define  DATA_POOL_SIZE  64
   //const
	#define	__atom_version_current				0x0310

	//flag  bit[0-3]
	#define	__atom_exp_flag_var		0x0
	#define	__atom_exp_flag_arg		0x1
	#define	__atom_exp_flag_citem		0x2
	#define	__atom_exp_flag_this		0x3
	#define	__atom_exp_flag_oitem		0x4
	#define	__atom_exp_flag_pitem	  0x5
	#define	__atom_exp_flag_call		0x6
	#define	__atom_exp_flag_ptcall		0x7
	#define	__atom_exp_flag_const		0x8
	#define	__atom_exp_flag_op		0x9
	#define	__atom_exp_flag_refcall		0xA
 //token type
	#define	__atom_token_type_type	0x1024
	#define	__atom_token_type_class	0x1025
	#define	__atom_token_type_struct	0x1026
	#define	__atom_token_type_union	0x1027
	#define	__atom_token_type_interface	0x1028
	#define	__atom_token_type_field	0x1029
	#define	__atom_token_type_method	0x102a
	#define	__atom_token_type_var	0x102b
	#define	__atom_token_type_namespace	0x102c
 //token sym type
	#define	__atom_sym_typeref	0x1024
	#define	__atom_sym_type	0x1025
	#define	__atom_sym_class	0x1026
	#define	__atom_sym_field	0x1027
	#define	__atom_sym_method	0x1028
	#define	__atom_sym_memberref	0x1029
	#define	__atom_sym_param	0x102a
	#define	__atom_sym_var	0x102b
	#define	__atom_sym_classextend	0x102c
	#define	__atom_sym_namespace	0x102d
	#define	__atom_sym_pcode	0x102e
	#define	__atom_sym_ncode	0x102f
	#define	__atom_sym_expression	0x1030
	#define	__atom_sym_nativemethod	0x1031
	#define	__atom_sym_typearray	0x1032
	#define	__atom_sym_typearrayarea	0x1033
	
	#define	__atom_sym_class_none	 		0x00
	#define	__atom_sym_class_class	 		0x01
	#define	__atom_sym_class_struct	 		0x02
	#define	__atom_sym_class_union	 		0x03
	#define	__atom_sym_class_unit		 		0x04
	#define	__atom_sym_class_interface			0x05
	#define	__atom_sym_class_enum			0x06
 //token metatable class
	#define	__atom_sym_meta_typeref			0x01
	#define	__atom_sym_meta_typedef			0x02
	#define	__atom_sym_meta_fieldptr			0x03
	#define	__atom_sym_meta_field				0x04
	#define	__atom_sym_meta_methodptr			0x05
	#define	__atom_sym_meta_method			0x06
	#define	__atom_sym_meta_paramptr			0x07
	#define	__atom_sym_meta_param			0x08
	#define	__atom_sym_meta_interfaceimpl		0x09
	#define	__atom_sym_meta_memberref			0x0A
	#define	__atom_sym_meta_constant			0x0B
	
	#define	__atom_sym_meta_pcode			0x70
	#define	__atom_sym_meta_ncode			0x71
	#define	__atom_sym_meta_expression			0x72
	#define	__atom_sym_meta_nativemethod		0x73
	#define	__atom_sym_meta_typearray			0x74
	#define	__atom_sym_meta_typearrayarea		0x74
     //control code
	#define	__atom_code_if	 				0x80
	#define	__atom_code_if_condition			0x8001
	#define	__atom_code_if_true_body			0x8002
	#define	__atom_code_if_false_body			0x8003
	
	#define	__atom_code_for					0x81
	#define	__atom_code_for_initial				0x8101
	#define	__atom_code_for_condition			0x8102
	#define	__atom_code_for_loop				0x8103
	#define	__atom_code_for_body				0x8104
	
	#define	__atom_code_while				0x82
	#define	__atom_code_while_condition			0x8201
	#define	__atom_code_while_body			0x8202
	
	#define	__atom_code_do					0x83
	#define	__atom_code_do_body				0x8301
	#define	__atom_code_do_condition			0x8302
	
	#define	__atom_code_switch				0x85
	#define	__atom_code_switch_condition		0x8501
	#define	__atom_code_switch_case_condition		0x8502
	#define	__atom_code_switch_case_body		0x8503
	#define	__atom_code_switch_default			0x8504
	#define	__atom_code_switch_default_body		0x8505
	
	#define	__atom_code_try					0x86
	#define	__atom_code_try_body				0x8601
	#define	__atom_code_try_catch_condition		0x8602
	#define	__atom_code_try_catch_body			0x8603
	#define	__atom_code_try_finally				0x8604
	
	#define	__atom_code_label				0x87
	
	#define	__atom_code_break				0x90
	#define	__atom_code_goto				0x91
	#define	__atom_code_continue				0x92
	#define	__atom_code_throw				0x93
	#define	__atom_code_return				0x94
	#define	__atom_code_block_start			0x95
	#define	__atom_code_block_end				0x96

	#define	__atom_code_let_pool				0x98
	#define	__atom_code_exp_pool				0x99
	#define	__atom_code_let					0x9A
	#define	__atom_code_exp					0x9B
	#define	__atom_code_dclv					0x9C
	#define	__atom_code_dclt					0x9D
	#define	__atom_code_nop					0x9E
	
	#define	__atom_code_version				0xA0
struct XSM_API tokenref
	{	atom		reftype;
			void*		pitem;
			tokenref*	next;
	
		};
	
	struct XSM_API tokenitem
	{	int  stringPos;
			tokenitem* next;
			tokenref* ref;
	
		};

	//alloc array tokenref
	struct XSM_API tokenrefalloc
	{
	
public:
		
	 tokenrefalloc();
	 ~tokenrefalloc();
public:
		
	 tokenref* alloc();
private:
			tokenref* pitems;
	
private:
			int used;
			int size;
	};
	//alloc array token
	struct XSM_API tokenalloc
	{
	
public:
		
	 tokenalloc();
	 ~tokenalloc();
	 tokenitem* alloc();
private:
			tokenitem* pitems;
	
private:
			int used;
			int size;
	};
	
	class XSM_API StringPool
	{
	
private:
			tchar * pBuffer;
			int used;
			int alloced;
	
public:
		
	 StringPool();
	 ~StringPool();
	 inline tchar * GetStringByPos(int nPos);
	 int AppendString(const tchar * pStr);
	 int SetUsed(int val);
	 int GetUsed();
	 const lptstr GetStartString();
	 const lptstr GetBuffer();};
				
	class XSM_API ByteDataPool
	{
	
private:
			Byte * pBuffer;
			int used;
			int alloced;
	
public:
		
	 ByteDataPool();
	 ~ByteDataPool();
	 inline Byte * GetDataByPos(int nPos);
	 int EnterData(const Byte * pStr, int nlen,int pos);
	 int AppendData(const Byte * pStr, int nlen);
	 int GetUsed();
	 int SetUsed(int val);
	 int AppendData(ByteDataPool * pPool);
	 Byte * GetStartData();
	 Byte * GetBuffer();};
				
	//token hash table when match then create list
	class XSM_API  tokentab
	{

	
public:
		
	 tokentab();
	 ~tokentab();
private:
			int bucketsize;
			tokenitem ** items;
			StringPool * pStringPool;
		
	 int hash(const tchar* str);
public:
		
	 tokenitem* addtoken(const tchar* str);
	 tokenitem* findtoken(const tchar* str);
public:
		
	 bool addtokenref(tokenitem * token,atom typ,void * pitem);
	 tokenitem* addtoken(const tchar* str,atom at,void * pitem);
	 const lptstr getstring(tokenitem * token);
	 const lptstr getstring(int pos);
private:
			tokenalloc * allocer;
			tokenrefalloc * refallocer;
	};
struct sym_object
	{	atom at;                                      //atom value
			int seq;                                       //meta seq
			sym_object * next;                       //pointer next  object
			sym_object * piror;                      // ponter piror   object
			sym_object * parent;                   // pointer parent
			int row;                                      // start row
			int erow;                                    //  end row
	
		};
	
	//type
	struct sym_type : public sym_object
	{	tokenitem*	name;		//type name
		//var	int			nprimay;		//primary of type
			int  ntype;
		
	 int GetType();
	 int GetTypeSeq();
	 int GetPrimaryType();
		};

	//method param
	struct sym_param  : public sym_object
	{	sym_type*		ptype;		//type;
			dword		spec;			//arg spec;
			dword		exttype;		// type extend
			tokenitem*	name;		//argname
			bool			binit;			//initial
			union
		{
			tokenitem*	sval;
			int			nval;
			double		rval;
		};
	
			bool	bole;
			bool  bprop;
			int	propget;
			int	propput;
	
		};

	//class filed 
	struct sym_field   : public sym_param
	{
	};

	//class method
	struct sym_method  : public sym_param
	{	tokenitem*	name;		//method name;
			bool			boperator;		//method is operator
			bool			astype;		//method is type
			sym_param*	param;		//param
			sym_param*	pvar;			//pvar
			sym_param*	pprop;		//prop
		
	
		//code
			int			pcode;               //index of sym_pcode
			int			ncode;               //index of sym_ncode

			void*	        	addr;			//pointer address
			int			addrlen;		//code len;
	
			void*	        	xaddr;		//pointer xcode address

			int			native;		//index of nativate ;
	
		};

	//native method
	struct sym_nativemethod  : public sym_object
	{	int	from;		//pointer to dll file
            		int 	alias;		//dll method alias   name
	
		};

	//type  class extend
	struct sym_classextend  : public  sym_type
	{	sym_type* 	ptype;		//pointer type
			dword		spec;                //access spec
	
		} ;
	
	//type class
	struct sym_class  : public  sym_type
	{	atom				typ;       		//class type
			dword			spec;                //access spec
			sym_classextend* 	pextend;    	//class last extend
			sym_class*   		pclass;        	//point to last nest class
			sym_field*			pfield;               //pointer last field
			sym_method*		pmethod;          //pointer last method
			int				nativefrom;       //pointer to dll file
	        	bool				bdirty;              //dirty bit
			bool				bdeclare;	      //only declare	
		
		//ole
			void *			pTypeInfo;
			bool				bole;
	
		//static
			Byte*			pStatic;
	
		};
	
 	//name space
	struct sym_namespace : public sym_object
	{	tokenitem*			name;		//name
			sym_class*   			pclass;             //pointer last class
			sym_method*		 	pmethod;          //pointer last method
			vector<sym_namespace*>   pusing;	      //pointr using namespace
			sym_namespace*        	pnamespace;   //pointr namespace
	
			tokenitem*		olename;		//ole name
			void *			ptypelib;		//ITypeLib
	
		};

	//typeref
	struct sym_typeref : public sym_object
	{	sym_namespace*	pnamespace;	//pointer namespace
			sym_class*		pclass;		//pointer class
	
		};
	
	//memberref
	struct sym_memberref: public sym_object
	{	tokenitem*	name;	     //ref name
			sym_object*	pclass;	     //MemberRefParent
		//var	sym_blob*        	signature;	       //Signature blob 
	
		};

	//code
	struct sym_code: public sym_object
	{	int	pos;	     	//pos of code pool
			int	len;	     	//size of code
			int     state;	//state of expression 
	
		};
	
	//pcode
	struct sym_pcode: public sym_code
	{
	};
	
 	//ncode
	struct sym_ncode: public sym_code
	{
	};

	//expression
	struct sym_expression: public sym_code
	{	void * purl;
			int 	row;
	
		};
	
	// array type 
	struct sym_typearray : public  sym_type
	{	int  eletype;
			int area;
			int dim;
	
		};
	//array area
	struct sym_typearrayarea : public sym_object
	{	int low;
			int high;
	
		};
class  MetaTable
{
	
public:
		sym_namespace*	m_pGlobalNS;	//pointer global namespace

public:
		tokentab*		ptokentab;         //pointer   token hash pool
		std::vector<sym_object *>*  pTableSeq;        //array of meat table seq

private:
		ByteDataPool*	pdatapool;         //pointer   Byte  data pool
		ByteDataPool*	pcodepool;         //pointer   Byte  code pool
		tokentab*		pstringhash;       //pointer   string hash

public:
	//construct class
	
	 MetaTable();
	 ~MetaTable();
public:
	//get data pool
	
	 ByteDataPool* GetDatePool();
	 ByteDataPool* GetCodePool();
	 tokentab* GetStringHash();
public:
	//get meta table seq for code index
	
	 int AllocTableSeq(atom ty,sym_object* pobject);
	 sym_object * GetSymObject(atom ty,int seq);
	 sym_class * GetClass(lptstr pclass);
	 sym_method * GetClassMethod(tokenitem * pname,tokenitem * ptmethod,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetClassMethod(sym_class * pclass ,lptstr pmethodname,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetClassMethod(lptstr pClassName,lptstr pMethodName,std::vector<sym_method *> *pmethods = null);
	 sym_method * GetSymMethod(lptstr psname);
public:
	//alloc type item
	
	sym_type * buildtype()
	{
		sym_type * p = new sym_type;
		memset(p,0,sizeof(sym_type));
		p->at = __atom_sym_type;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_namespace * buildnamespace()
	{
		sym_namespace * p = new sym_namespace;
		memset(p,0,sizeof(sym_namespace));
		p->at = __atom_sym_namespace;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_method * buildmethod()
	{
		sym_method * p = new sym_method;
		memset(p,0,sizeof(sym_method));
		p->at = __atom_sym_method;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_nativemethod * buildnativemethod()
	{
		sym_nativemethod * p = new sym_nativemethod;
		memset(p,0,sizeof(sym_nativemethod));
		p->at = __atom_sym_nativemethod;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_field * buildfield()
	{
		sym_field * p = new sym_field;
		memset(p,0,sizeof(sym_field));
		p->at = __atom_sym_field;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_param * buildparam()
	{
		sym_param * p = new sym_param;
		memset(p,0,sizeof(sym_param));
		p->at = __atom_sym_param;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_classextend * buildclassextend()
	{
		sym_classextend * p = new sym_classextend;
		memset(p,0,sizeof(sym_classextend));
		p->at = __atom_sym_classextend;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_class * buildclass()
	{
		sym_class * p = new sym_class;
		memset(p,0,sizeof(sym_class));
		p->at = __atom_sym_class;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_typeref * buildtyperef()
	{
		sym_typeref * p = new sym_typeref;
		memset(p,0,sizeof(sym_typeref));
		p->at = __atom_sym_typeref;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_memberref * buildmemberref()
	{
		sym_memberref * p = new sym_memberref;
		memset(p,0,sizeof(sym_memberref));
		p->at = __atom_sym_memberref;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_pcode * buildpcode()
	{
		sym_pcode * p = new sym_pcode;
		memset(p,0,sizeof(sym_pcode));
		p->at = __atom_sym_pcode;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_expression * buildexpression()
	{
		sym_expression * p = new sym_expression;
		memset(p,0,sizeof(sym_expression));
		p->at = __atom_sym_expression;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_typearray * buildtypearray()
	{
		sym_typearray * p = new sym_typearray;
		memset(p,0,sizeof(sym_typearray));
		p->at = __atom_sym_typearray;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	sym_typearrayarea * buildtypearrayarea()
	{
		sym_typearrayarea * p = new sym_typearrayarea;
		memset(p,0,sizeof(sym_typearrayarea));
		p->at = __atom_sym_typearrayarea;
		p->seq  = -1;
		if(p->at==__atom_sym_class)
		      p->seq = AllocTableSeq(__atom_sym_meta_typedef,p);
		else if(p->at==__atom_sym_method)
		      p->seq = AllocTableSeq(__atom_sym_meta_method,p);
		else if(p->at==__atom_sym_nativemethod)
		      p->seq = AllocTableSeq(__atom_sym_meta_nativemethod,p);
		else if(p->at==__atom_sym_field)
		      p->seq = AllocTableSeq(__atom_sym_meta_field,p);
		else if(p->at==__atom_sym_param)
		      p->seq = AllocTableSeq(__atom_sym_meta_param,p);
		else if(p->at==__atom_sym_pcode)
		      p->seq = AllocTableSeq(__atom_sym_meta_pcode,p);
		else if(p->at==__atom_sym_ncode)
		      p->seq = AllocTableSeq(__atom_sym_meta_ncode,p);
		else if(p->at==__atom_sym_expression)
		      p->seq = AllocTableSeq(__atom_sym_meta_expression,p);
		else if(p->at==__atom_sym_typearray)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearray,p);
		else if(p->at==__atom_sym_typearrayarea)
		      p->seq = AllocTableSeq(__atom_sym_meta_typearrayarea,p);
		return p;
	}
	 ;
	//alloc namespace
	
	//alloc method
	
	//alloc method
	
	//alloc field
	
	//alloc arg
	
	//alloc classextend
	
	//alloc class
	
	//alloc typeref
	
	//alloc memberref
	
	//alloc pcode
	
	//alloc expression
	
	//alloc arraytype
	
	//alloc arraytypearea
	

public:
	//get token tab
	
	 tokentab* GetTokenTab();
	 tokenitem * FindToken(const tchar * pname);
	 tokenitem * FindTokenGlow(const tchar * pname);
public:
	//get global namespace
	
	 sym_namespace * GetGlobalNS();
	 int GetAlignSizeEx(int typ,int align = 4,int talign=1);
	 int GetAlignSizeStatic(int typ,int align = 4,int talign=1);
	 int GetAlignSizeStatic(int typ,int & msize, int align = 4,int talign=1);
	 int GetAlignSizeStatic(sym_class * pc, int &msize, int align=4 ,int talign=1);
	 int GetAlignSize(int typ,int align = 4,int talign=1);
	 int GetAlignSize(int typ,int & msize, int align = 4,int talign=1);
	 int  GetAlignSize(sym_typearray * parray,  int &msize, int align ,int talign);
	 int GetAlignSize(sym_class * pc, int &msize, int align ,int talign);
	 int GetOffsetStatic(int typ,  int seq , int align = 4,int talign=1);
	 int GetOffsetStatic(sym_class * pc, int &msize, int seq , int align ,int talign);
	 int GetOffset(int typ,  int seq , int align = 4,int talign=1);
	 int GetOffset(sym_typearray * parray, int &msize, int seq , int align ,int talign);
	 int GetOffset(sym_class * pc, int &msize, int seq , int align ,int talign);};
	

	 int  GetOpString(atom op,tchar buf[]);