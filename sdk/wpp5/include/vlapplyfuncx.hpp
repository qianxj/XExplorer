#ifndef WPP5_VLAPPLYFUNCX_H
#define WPP5_VLAPPLYFUNCX_H

#include "vlAstAxis.hpp"
#include "vlwpp_context.hpp"
#include "vlhashcontext.hpp"
#include <map>
#include <vector>
#include <set>
#include <deque>
#include "vltrap.hpp"

namespace xlang
{
	vlAstAxis* fxSymGetContext(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetNamespaceName(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//ole process
	vlAstAxis* fxSymGetOleMethod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetOlePropGet(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetOlePropPut(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymIsOleItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeOleName(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetOleTypeInfo(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//symtable
	vlAstAxis* fxSymGetSymTable(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	//code prt
	vlAstAxis*  fxSymGetCodePc(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//add symbol term
	vlAstAxis* fxSymAddNamespace(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddType(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddField(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddMethod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddParam(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddVariant(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//comp type
	vlAstAxis* fxSymMakeTypeRef(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddCompTypeItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddCompTypeRank(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymInsertCompTypeItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymInsertCompTypeRank(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetCompTypeItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetCompTypeRank(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymStepCompType(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//method
	vlAstAxis* fxSymGetMethodHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodVariantHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	//method paramm
	vlAstAxis* fxSymGetMethodParamHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodParamHandleByIndex(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetParamSpec(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetParamInitValue(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodParamCount(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	//method pcode
	void fxSymSetMethodPCode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodPCode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxSymGetTypeHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetGlobalNamespace(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymSetTypeKind(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeKind(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetFieldHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeMethodHandle(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeAllocSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetFieldAllocSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetOffsetOfField(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodVariantSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	//term
	void fxSymSetNSOleTerm(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymAddNSOleTerm(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetOffsetOfTerm(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymAddBase(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTermNameID(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTermName(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTermType(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTermSpec(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTermInitValue(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymSetTermInitValue(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeTermFrameSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	vlAstAxis* fxSymLookupTypeItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymLookupType(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymLookupMethod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymLookupItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxSymGetParent(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymIsSuperClass(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//real const ptr
	vlAstAxis* fxSymAddReal(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetRealPtr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//dump symbol term
	vlAstAxis*  fxSymDump(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpType(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpMethod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpField(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpParam(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpVariant(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymDumpTypeParam(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
		
	vlAstAxis* fxSymGetMethodSpec(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis*  fxSymGetMethodAddr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymSetMethodAddr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//native sppec
	void fxSymSetMethodNativeFrom(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymSetMethodNativeAlias(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodNativeFrom(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetMethodNativeAlias(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymIsMethodNative(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymSetMethodNative(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymSetTypeNativeFrom(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSymSetTypeNativeAlias(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeNativeFrom(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymGetTypeNativeAlias(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxSymFindOleChild(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSymFindMatchedMethod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind); 
	
	//logic operate
	vlAstAxis* fxGt(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxGte(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxLt(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxLte(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEq(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxNeq(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxNil(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxNot(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxAnd(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxOr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxtoString(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAdd(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAdd(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSub(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxMul(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxDiv(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxMod(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxShr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxShl(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxBand(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxBor(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxBnot(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxXor(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxInc(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxDec(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxNeg(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxIsIdentifier(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsLiteral(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsNumber(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsDouble(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsInteger(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsChar(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsBool(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsString(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsCall(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxIsTerm(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	vlAstAxis* fxListTop(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListItem(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListSetSize(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListSetTop(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListPush(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxListPop(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	vlAstAxis* fxHash(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxGetHashString(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
		
	void fxSavePoint(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxRestorePoint(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAllocPages(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	//node operate
	vlAstAxis* fxMakeTypeDataNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxMakeDataBufferNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxRemoveChildNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAppendSiblingNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAppendNextNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAppendChildNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxMakeAxis(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxSibling(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxNext(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxChild(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxGetNode(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxParent (vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxLastChild(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxChildNext(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* ChildSibling(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//replace node value
	vlAstAxis* fxNodeNameID(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSetValue(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	//Ô´´úÂëÎ»ÖÃ
	vlAstAxis* fxTextUrl(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxTextRow(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxTextCol(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	//register alloc not used
	vlAstAxis* fxCurRegister(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxAskRegister(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxFreeRegister(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxClearRegister(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxGetRegisterCount(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);

	//vlContext * lookup
	void fxContextEnterScope(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxContextLeaveScope(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxContextPushWord(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxContextGetWord(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxContextFindToken(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxContextFindNextToken(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//debug
	void fxDebugBreak(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//code emit
	void fxEmitInteger(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxEmitChar(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxEmitShort(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxEmitPtr(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	//code emit util
	void fxLabel(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxLabelas(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxSureLabel(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxToLabel(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxFunction(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxCodeEval(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	//disasssemble
	vlAstAxis*  fxDisassemble(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	//apply vlContext *
	void fxEnterContextEx(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxEnterContext(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	void fxLeaveContext(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	
	vlAstAxis* fxEpSureProcessApply(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEpSureContextApply(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEpAddMatchCount(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEpAddMatchApplyName(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEpAddMatchLiteral(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind); 
	vlAstAxis* fxEpAddMatchName(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
	vlAstAxis* fxEpAddMatchAny(vlApply * apply, vlContext * context,vlAstAxis * node,std::vector<vlAstAxis *> &args,vlParser* parser,vlAstAxis * curNode,std::map<int,vlAstAxis*> & paramBind);
}

#endif //WPP5_VLAPPLYFUNCX_H