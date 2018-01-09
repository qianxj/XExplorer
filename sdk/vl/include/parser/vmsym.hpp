#ifndef VLANG_VMSYM_H_
#define VLANG_VMSYM_H_

#include "../vl.h"
#include "vldef.hpp"
#include <vector>
#include <stack>
#include "vmcode.hpp"

namespace vl
{
	/*
	//hash token
	struct vmtoken
	{
		tchar * name;
		int		first;
		int		last;
	};

	//hash token ref
	struct vmtokenref
	{
		int tokenid;
		int eitem;	//type of item
		int itemid;	//id of item
		int parentid; //parent of item
		int dirty;
		//double list
		int piror;
		int next;
	};

	//extern type
	struct vmexttype
	{
		
	};

	//type object
	struct vmtype
	{
		int tokenid;
		int etype; //what
		int spec;
		int native;
		int ole;
		int firstitem;
		int lastitem;
	};

	//super type
	struct vmsuper
	{
		int tokenid;
		int superid;
		int next;
	};

	//type item
	struct vmitem
	{
		int tokenid;
		int etype;
		int refid;
	};

	//type field item
	struct vmfield
	{
		int typ;
		int spec;
		int val; //default val
	};

	//method item
	struct vmmethod
	{
		int typ;
		int spec;
		int firstparam;
		int lastparam;
		int firstvar;
		int lastvar;
	};

	//item native feature
	struct vmnative
	{
		int from;
		int alias;
	};

	//method param
	struct vmparam
	{
		int tokenid;
		int typ;
		int spec;
		int val;
		int piror;
		int next;
	};

	//method var
	struct vmvar
	{
		int tokenid;
		int typ;
		int spec;
		int val;
		int piror;
		int next;
	};*/
	
	//define meta base item
	struct vmetaItem
	{
		int etype;
		int tokenid;
		int native;	//link native table
		int ole;	//link ole object
		//all index from 1
		int parent; //bit0-bit23->index bit24-bit31->vm::vmtype
		int next;
		int piror;
	};

	//define meta type
	struct vmetaType
	{
		int type;
		int spec;
	};

	//struct vmarea
	struct vmetaArea : vmetaItem
	{
		//all index from 1
		int firstMethod;
		int firstArea; 
		int firstClass;
		int firstField;
		int lastMethod;
		int lastArea;
		int lastClass;
		int lastField;
	};

	//define meta vmMethod
	struct vmetaMethod : vmetaItem , vmetaType
	{
		int prop;
		//all index from 1
		int firstParam;
		int firstVar;
		int lastParam;
		int lastVar;
	};


	//define meta class 
	struct vmetaClass : vmetaItem ,vmetaType
	{
		//all index from 1
		int super;
		int firstParam;
		int firstField;
		int firstMethod;
		int firstClass;
		int firstSuper;
		int lastParam;
		int lastField;
		int lastMethod;
		int lastClass;
		int lastSuper;
	};

	//define meta field ,param ,var
	struct vmetaField : vmetaItem ,vmetaType
	{
	};

	//define supper
	struct vmetaSuper : vmetaItem ,vmetaType
	{
	};

	//item native feature
	struct vmetaNative
	{
		int from;
		int alias;
	};
	
	//method ole
	struct vmetaOleMethod
	{
		int put;
		int get;
	};
	//class ole
	struct vmetaOleClass
	{
		void* ptClass;
	};
	//class olelib
	struct vmetaOleLib
	{
		void* ptLib;
	};

	//define extern ref type
	struct vmetaExttype
	{
		int etype;
		int tokenid;
		int moduleid;
		int object;
	};

	//struct meta table
	struct vmetaSym
	{
		std::vector<vmetaArea>		areaColl;
		std::vector<vmetaClass>		classColl;
		std::vector<vmetaMethod>	methodColl;
		std::vector<vmetaField>		fieldColl;
		std::vector<vmetaSuper>		superColl;
		std::vector<vmetaNative>	nativeColl;
		std::vector<vmetaOleLib>	oleLibColl;
		std::vector<vmetaOleClass>	oleClassColl;
		std::vector<vmetaOleMethod>	oleMethodColl;
		std::vector<vmetaExttype>	exttypeColl;
	};

	//define run stack item
	struct vmetaRunStackItem
	{
		vm::mtype eType;
		int object;
	};

	struct vmVisItemStack
	{
		int object;	//bit0-bit23->index bit24-bit31->vm::vmtype
		int hashlinkpiror;	//hash link next, index from zero
		int hashindex;		//hash index , index from one
	};

	struct vmVisItemHash
	{
		int tokenid;
		int indexStack;
	};

	class vlang3;
	class VL_API vmsym
	{
	public:
		vmsym();
		~vmsym();
	private:
		//符号表
		vmetaSym * vmSym_;
	private:
		//元语言执行栈
		std::stack<vmetaRunStackItem>* vmRunStack_;
	private:
		//初始化
		int initial();
		//初始命名空间
		int initArea();
		//初始执行栈
		int initRunStack(int area);
	private:
		std::stack<vmVisItemStack>* vmVisStack_;
		int visItemHash_;
		std::stack<int> * vmFrameStack_;
	public:
		int popLastFrame();
	private:
		vlang3 * vlang_;
	public:
		//exec make type
		int execMakeType(vm::mtype eType);
		//exec set object prop
		int execSetProp(vm::mItem nItem,int nVal);
		//exec enter
		int execEnter(vm::mtype eType,int object);
		//exec leave
		int execLeave();
		//exec use
		int execUse(int nArea);
		//exec get object
		int execGetObject(int tokenid);
		//exec find object
		int execFindObject(int tokenid);
	};
}
#endif