#ifndef WPP_VM_H
#define WPP_VM_H

#include <stack>
#include "vmdef.hpp"

//#include "pred.hpp"
//#include "word.hpp"

//using namespace wpp::how;

//类分为虚拟部分和本地部分,本地部份可以认为是虚拟部分的解释.
namespace vm {


//namespace
struct  DArea
{
	int no;
	int name;
	int parent;
};

//data type
struct DType
{
	int no;
	int name;
	int size;	//alloc size
	int staticsize; 
	int area;	
	int nclass; 
	int parent; 
	int param;
};

//array define
struct DArray
{
	int type;
	int lower;
	int high;
	int next;
};

//class define
struct DClass
{
	int firstfield;
	int lastfield;
	int firstmethod;
	int lastmethod;
	int firstattribute;
	int lastattribute;
	int firstprop;
	int lastprop;
	int super;
};

//super class
struct DSuper
{
	int access;
	int type;
	int next;
};

//method define
struct DMethod
{
	int no;
	int name;
	int spec;
	int type;
	int size;
	int next;	
	int firstParam;
	int lastParam;
	int firstVar;
	int lastVar;
	int class_;
	int area;
};

//var define
struct DVar
{
	int no;
	int name;
	int spec;
	int type;
	int size;
	int next;
	int offset;
};

struct DField : public DVar{};
struct DParam : public DVar{
	bool init;
};

} //namespace vm

namespace vm {
	class IItemNil
	{
	};
	class IItemIndex
	{
		virtual int get_index() const = 0;
	};
	class IItemName 
	{
		virtual int getNameID() const = 0;
		virtual const wchar_t * getName() const = 0;
		virtual int getNameID(int index) const = 0;
		virtual const wchar_t * getName(int index) const = 0;
	};
	class IItemWord 
	{
		virtual int get_word() const = 0;
		virtual int getWord(int index) const = 0;
	};
	class IItemSpec  
	{
		virtual int getSpec() const = 0;
	};

	class IArea 
	{
		
	};
	class IType 
	{
	public:
		virtual void* InitialAtomType();
	public:
		virtual int getSize(int typ)=0;
		virtual int getStaticSize(int typ)=0;
	public:
		virtual int calcClassAlignSize(int nclass,int & msize, int align = 4,int talign=1)=0;
		virtual int calcFieldOffset(int nclass, int nfield,  int align = 4,int talign=1)=0;
	};

	class IArray 
	{
	};

	class IClass 
	{
		virtual int GetClassField(int fc,wchar_t* pfield)=0;
	};

	class ISuper 
	{
	};
	class IMethod 
	{
	};
	class IField
	{
	};
	class IParam 
	{
	};
	class IVar 
	{
	};
} //namespace vm

namespace vm {
	class TSymItem {};
	class TArea : public TSymItem{};
	class TType : public TSymItem{}; 
	class TArray: public TSymItem{};  
	class TClass: public TSymItem{};  
	class TSuper: public TSymItem{};  
	class TMethod:public TSymItem{};  
	class TField: public TSymItem{}; 
	class TParam: public TSymItem{};  
	class TVar:	  public TSymItem{};  
} //namespace vm

namespace vm {
	class Area : public IArea, public DArea, public TArea {};
	class Type : public IType, public DType, public TType
	{
	public:
		const DType*   getDType(int typ) const{	return 0;}
		const DClass*  getDClass(int cls) const{	return 0;}
		const DSuper*  getDSuper(int sc) const{	return 0;}
		const DField*  getDField(int field) const{	return 0;}
		const DMethod* getDMethod(int f) const{	return 0;}
		const DVar*		getDVar(int f) const{	return 0;}
		const DParam*  getDParam(int f) const{	return 0;}
	public:
		bool specStatic(int spec){return spec & spec::static_?true:false;}
		bool specByref(int spec){return spec & spec::byref_?true:false;}
	public:
		bool isClassType(int typ){return typ == type::class_ || typ == type::struct_ ||typ == type::interface_;}
		bool isStructType(int typ){return typ == type::struct_;}
	public:
		bool typeCanImpConv(int ty1,int ty2){return true;}

	//interface of IType
	public:
		virtual int getSize(int typ);
		virtual int getStaticSize(int typ);
	public:
		virtual int calcClassAlignSize(int nclass,int & msize, int align = 4);
		virtual int calcFieldOffset(int nclass, int nfield,  int align = 4);

	public:
		virtual int findMatchedMethod(int pl ,int fc , int indent, int nargs,int typs[],int nest=-1);
		virtual bool isMatchedMethod(int f, int nargs,int  typ[]);

	public:
		virtual void* initialAtomType();
	}; 

	class Array : public IArray, public DArray, public TArray {};  
	class Class : public IClass, public DClass, public TClass{};  
	class Super : public ISuper, public DSuper, public TSuper{};  
	class Method: public IMethod, public DMethod, public TMethod{};  
	class Field : public IField, public DField, public TField{}; 
	class Param : public IParam, public DParam, public TParam{};  
	class Var   : public IVar, public DVar, public TVar{};  
} //namespace vm

namespace vm
{
	struct DVMTable
	{
		Area*	areaPtr;
		Type*	typePtr;
		Array*	arrayPtr;
		Class*	classPtr;
		Super*	superPtr;
		Method* methodPtr;
		Field*	fieldPtr;
		Param*	paramPtr; 
		Var*	varPtr;
	};

	struct IVMTable
	{
		virtual const Area*   getAreaPtr()   const = 0;
		virtual const Area*   getPtr(TArea)  const = 0;
		virtual const Type*   getTypePtr()   const = 0;
		virtual const Type*   getPtr(TType)  const = 0;
		virtual const Array*  getAraryPtr()  const = 0;
		virtual const Array*  getPtr(TArray) const = 0;
		virtual const Class*  getClassPtr()  const = 0;
		virtual const Class*  getPtr(TClass) const = 0;
		virtual const Super*  getSuperPtr()  const = 0;
		virtual const Super*  getPtr(TSuper) const = 0;
		virtual const Method* getMethodPtr() const = 0;
		virtual const Method* getPtr(TMethod)const = 0;
		virtual const Field*  getFieldPtr()	 const = 0;
		virtual const Field*  getPtr(TField) const = 0;
		virtual const Param*  getParamPtr()	 const = 0;
		virtual const Param*  getPtr(TParam) const = 0;
		virtual const Var*	  getVarPtr()	 const = 0;
		virtual const Var*	  getPtr(TVar)	 const = 0;
	};

	class VMTable : public IVMTable,public DVMTable
	{
		//struct 
	};
}

#endif //WPP_VM_H