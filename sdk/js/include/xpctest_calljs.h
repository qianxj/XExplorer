/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_calljs.idl
 */

#ifndef __gen_xpctest_calljs_h__
#define __gen_xpctest_calljs_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestCallJS */
#define NS_IXPCTESTCALLJS_IID_STR "38ba7d98-5a54-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCALLJS_IID \
  {0x38ba7d98, 0x5a54, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestCallJS : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCALLJS_IID)

  /* boolean CallMethodNoArgs (); */
  NS_SCRIPTABLE NS_IMETHOD CallMethodNoArgs(PRBool *_retval NS_OUTPARAM) = 0;

  /* void Evaluate (in string s); */
  NS_SCRIPTABLE NS_IMETHOD Evaluate(const char *s) = 0;

  /* nsresult EvaluateAndReturnError (in nsresult s); */
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndReturnError(nsresult s, nsresult *_retval NS_OUTPARAM) = 0;

  /* void SetJSObject (in nsIXPCTestCallJS o); */
  NS_SCRIPTABLE NS_IMETHOD SetJSObject(nsIXPCTestCallJS *o) = 0;

  /* void EvaluateAndEatErrors (in string s); */
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndEatErrors(const char *s) = 0;

  /* [noscript] void UnscriptableMethod (); */
  NS_IMETHOD UnscriptableMethod(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestCallJS, NS_IXPCTESTCALLJS_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCALLJS \
  NS_SCRIPTABLE NS_IMETHOD CallMethodNoArgs(PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Evaluate(const char *s); \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndReturnError(nsresult s, nsresult *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetJSObject(nsIXPCTestCallJS *o); \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndEatErrors(const char *s); \
  NS_IMETHOD UnscriptableMethod(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCALLJS(_to) \
  NS_SCRIPTABLE NS_IMETHOD CallMethodNoArgs(PRBool *_retval NS_OUTPARAM) { return _to CallMethodNoArgs(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD Evaluate(const char *s) { return _to Evaluate(s); } \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndReturnError(nsresult s, nsresult *_retval NS_OUTPARAM) { return _to EvaluateAndReturnError(s, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetJSObject(nsIXPCTestCallJS *o) { return _to SetJSObject(o); } \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndEatErrors(const char *s) { return _to EvaluateAndEatErrors(s); } \
  NS_IMETHOD UnscriptableMethod(void) { return _to UnscriptableMethod(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCALLJS(_to) \
  NS_SCRIPTABLE NS_IMETHOD CallMethodNoArgs(PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallMethodNoArgs(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD Evaluate(const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->Evaluate(s); } \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndReturnError(nsresult s, nsresult *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EvaluateAndReturnError(s, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetJSObject(nsIXPCTestCallJS *o) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetJSObject(o); } \
  NS_SCRIPTABLE NS_IMETHOD EvaluateAndEatErrors(const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->EvaluateAndEatErrors(s); } \
  NS_IMETHOD UnscriptableMethod(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->UnscriptableMethod(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestCallJS : public nsIXPCTestCallJS
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCALLJS

  nsXPCTestCallJS();

private:
  ~nsXPCTestCallJS();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestCallJS, nsIXPCTestCallJS)

nsXPCTestCallJS::nsXPCTestCallJS()
{
  /* member initializers and constructor code */
}

nsXPCTestCallJS::~nsXPCTestCallJS()
{
  /* destructor code */
}

/* boolean CallMethodNoArgs (); */
NS_IMETHODIMP nsXPCTestCallJS::CallMethodNoArgs(PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Evaluate (in string s); */
NS_IMETHODIMP nsXPCTestCallJS::Evaluate(const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsresult EvaluateAndReturnError (in nsresult s); */
NS_IMETHODIMP nsXPCTestCallJS::EvaluateAndReturnError(nsresult s, nsresult *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetJSObject (in nsIXPCTestCallJS o); */
NS_IMETHODIMP nsXPCTestCallJS::SetJSObject(nsIXPCTestCallJS *o)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EvaluateAndEatErrors (in string s); */
NS_IMETHODIMP nsXPCTestCallJS::EvaluateAndEatErrors(const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void UnscriptableMethod (); */
NS_IMETHODIMP nsXPCTestCallJS::UnscriptableMethod()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_calljs_h__ */
