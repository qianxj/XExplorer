/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XOffice\src\XOFObject\include\IBoundSvr.idl
 */

#ifndef __gen_IBoundSvr_h__
#define __gen_IBoundSvr_h__
/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif

/* starting interface:    IBoundSvr */
#define IBOUNDSVR_IID_STR "f2df3213-0274-45d3-91ad-8fa310ccf010"

#define IBOUNDSVR_IID \
  {0xf2df3213, 0x0274, 0x45d3, \
    { 0x91, 0xad, 0x8f, 0xa3, 0x10, 0xcc, 0xf0, 0x10 }}

class XNO_VTABLE XSCRIPTABLE IBoundSvr {
 public: 

  XDECLARE_STATIC_IID_ACCESSOR(IBOUNDSVR_IID)

  /**
		 * Bound Rect
		 */
/**
		 * redraw rect
		 */
  /* [noscript] attribute long Test; */
  XIMETHOD GetTest(PRInt32 *aTest) = 0;
  XIMETHOD SetTest(PRInt32 aTest) = 0;

};

  XDEFINE_STATIC_IID_ACCESSOR(IBoundSvr, IBOUNDSVR_IID)

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_IBOUNDSVR \
  XIMETHOD GetTest(PRInt32 *aTest); \
  XIMETHOD SetTest(PRInt32 aTest); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_IBOUNDSVR(_to) \
  XIMETHOD GetTest(PRInt32 *aTest) { return _to GetTest(aTest); } \
  XIMETHOD SetTest(PRInt32 aTest) { return _to SetTest(aTest); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_IBOUNDSVR(_to) \
  XIMETHOD GetTest(PRInt32 *aTest) { return !_to ? XERROR_NULL_POINTER : _to->GetTest(aTest); } \
  XIMETHOD SetTest(PRInt32 aTest) { return !_to ? XERROR_NULL_POINTER : _to->SetTest(aTest); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IBoundSvr
{
public:
  XDECL_ISUPPORTS
  XDECL_IBOUNDSVR

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
XIMPL_ISUPPORTS1(_MYCLASS_, IBoundSvr)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* [noscript] attribute long Test; */
XIMETHODIMP _MYCLASS_::GetTest(PRInt32 *aTest)
{
    return XERROR_NOT_IMPLEMENTED;
}
XIMETHODIMP _MYCLASS_::SetTest(PRInt32 aTest)
{
    return XERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_IBoundSvr_h__ */
