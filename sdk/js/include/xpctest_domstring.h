/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_domstring.idl
 */

#ifndef __gen_xpctest_domstring_h__
#define __gen_xpctest_domstring_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestDOMString */
#define NS_IXPCTESTDOMSTRING_IID_STR "646d0b6b-6872-43b9-aa73-3c6b89ac3080"

#define NS_IXPCTESTDOMSTRING_IID \
  {0x646d0b6b, 0x6872, 0x43b9, \
    { 0xaa, 0x73, 0x3c, 0x6b, 0x89, 0xac, 0x30, 0x80 }}

/**
 * Interface for testing the DOMString-conversion infrastructure.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestDOMString : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTDOMSTRING_IID)

  /* void hereHaveADOMString (in DOMString str); */
  NS_SCRIPTABLE NS_IMETHOD HereHaveADOMString(const nsAString & str) = 0;

  /* void dontKeepThisOne (in DOMString str); */
  NS_SCRIPTABLE NS_IMETHOD DontKeepThisOne(const nsAString & str) = 0;

  /* void giveDOMStringTo (in nsIXPCTestDOMString recv); */
  NS_SCRIPTABLE NS_IMETHOD GiveDOMStringTo(nsIXPCTestDOMString *recv) = 0;

  /* void passDOMStringThroughTo (in DOMString str, in nsIXPCTestDOMString recv); */
  NS_SCRIPTABLE NS_IMETHOD PassDOMStringThroughTo(const nsAString & str, nsIXPCTestDOMString *recv) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestDOMString, NS_IXPCTESTDOMSTRING_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTDOMSTRING \
  NS_SCRIPTABLE NS_IMETHOD HereHaveADOMString(const nsAString & str); \
  NS_SCRIPTABLE NS_IMETHOD DontKeepThisOne(const nsAString & str); \
  NS_SCRIPTABLE NS_IMETHOD GiveDOMStringTo(nsIXPCTestDOMString *recv); \
  NS_SCRIPTABLE NS_IMETHOD PassDOMStringThroughTo(const nsAString & str, nsIXPCTestDOMString *recv); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTDOMSTRING(_to) \
  NS_SCRIPTABLE NS_IMETHOD HereHaveADOMString(const nsAString & str) { return _to HereHaveADOMString(str); } \
  NS_SCRIPTABLE NS_IMETHOD DontKeepThisOne(const nsAString & str) { return _to DontKeepThisOne(str); } \
  NS_SCRIPTABLE NS_IMETHOD GiveDOMStringTo(nsIXPCTestDOMString *recv) { return _to GiveDOMStringTo(recv); } \
  NS_SCRIPTABLE NS_IMETHOD PassDOMStringThroughTo(const nsAString & str, nsIXPCTestDOMString *recv) { return _to PassDOMStringThroughTo(str, recv); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTDOMSTRING(_to) \
  NS_SCRIPTABLE NS_IMETHOD HereHaveADOMString(const nsAString & str) { return !_to ? NS_ERROR_NULL_POINTER : _to->HereHaveADOMString(str); } \
  NS_SCRIPTABLE NS_IMETHOD DontKeepThisOne(const nsAString & str) { return !_to ? NS_ERROR_NULL_POINTER : _to->DontKeepThisOne(str); } \
  NS_SCRIPTABLE NS_IMETHOD GiveDOMStringTo(nsIXPCTestDOMString *recv) { return !_to ? NS_ERROR_NULL_POINTER : _to->GiveDOMStringTo(recv); } \
  NS_SCRIPTABLE NS_IMETHOD PassDOMStringThroughTo(const nsAString & str, nsIXPCTestDOMString *recv) { return !_to ? NS_ERROR_NULL_POINTER : _to->PassDOMStringThroughTo(str, recv); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestDOMString : public nsIXPCTestDOMString
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTDOMSTRING

  nsXPCTestDOMString();

private:
  ~nsXPCTestDOMString();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestDOMString, nsIXPCTestDOMString)

nsXPCTestDOMString::nsXPCTestDOMString()
{
  /* member initializers and constructor code */
}

nsXPCTestDOMString::~nsXPCTestDOMString()
{
  /* destructor code */
}

/* void hereHaveADOMString (in DOMString str); */
NS_IMETHODIMP nsXPCTestDOMString::HereHaveADOMString(const nsAString & str)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void dontKeepThisOne (in DOMString str); */
NS_IMETHODIMP nsXPCTestDOMString::DontKeepThisOne(const nsAString & str)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void giveDOMStringTo (in nsIXPCTestDOMString recv); */
NS_IMETHODIMP nsXPCTestDOMString::GiveDOMStringTo(nsIXPCTestDOMString *recv)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void passDOMStringThroughTo (in DOMString str, in nsIXPCTestDOMString recv); */
NS_IMETHODIMP nsXPCTestDOMString::PassDOMStringThroughTo(const nsAString & str, nsIXPCTestDOMString *recv)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_domstring_h__ */
