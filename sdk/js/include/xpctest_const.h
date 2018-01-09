/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_const.idl
 */

#ifndef __gen_xpctest_const_h__
#define __gen_xpctest_const_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsrootidl_h__
#include "nsrootidl.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestConst */
#define NS_IXPCTESTCONST_IID_STR "83f57a56-4f55-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCONST_IID \
  {0x83f57a56, 0x4f55, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

/**
 *	Only consts of type short and long are respected.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestConst : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCONST_IID)

  enum { shortConst = 254 };

  enum { longConst = 1234567890 };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestConst, NS_IXPCTESTCONST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCONST \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCONST(_to) \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCONST(_to) \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestConst : public nsIXPCTestConst
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCONST

  nsXPCTestConst();

private:
  ~nsXPCTestConst();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestConst, nsIXPCTestConst)

nsXPCTestConst::nsXPCTestConst()
{
  /* member initializers and constructor code */
}

nsXPCTestConst::~nsXPCTestConst()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_const_h__ */
