/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLSourceElement.idl
 */

#ifndef __gen_nsIDOMHTMLSourceElement_h__
#define __gen_nsIDOMHTMLSourceElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLSourceElement */
#define NS_IDOMHTMLSOURCEELEMENT_IID_STR "ee69ccd4-5216-46cc-bfb4-b612d197ce29"

#define NS_IDOMHTMLSOURCEELEMENT_IID \
  {0xee69ccd4, 0x5216, 0x46cc, \
    { 0xbf, 0xb4, 0xb6, 0x12, 0xd1, 0x97, 0xce, 0x29 }}

/**
 * The nsIDOMHTMLVideoElement interface is the interface to a HTML
 * <source> element.
 *
 * For more information on this interface, please see
 * http://www.whatwg.org/specs/web-apps/current-work/#source
 *
 * @status UNDER_DEVELOPMENT
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLSourceElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLSOURCEELEMENT_IID)

  /* attribute DOMString src; */
  NS_SCRIPTABLE NS_IMETHOD GetSrc(nsAString & aSrc) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetSrc(const nsAString & aSrc) = 0;

  /* attribute DOMString type; */
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLSourceElement, NS_IDOMHTMLSOURCEELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLSOURCEELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetSrc(nsAString & aSrc); \
  NS_SCRIPTABLE NS_IMETHOD SetSrc(const nsAString & aSrc); \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType); \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLSOURCEELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetSrc(nsAString & aSrc) { return _to GetSrc(aSrc); } \
  NS_SCRIPTABLE NS_IMETHOD SetSrc(const nsAString & aSrc) { return _to SetSrc(aSrc); } \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return _to GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) { return _to SetType(aType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLSOURCEELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetSrc(nsAString & aSrc) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSrc(aSrc); } \
  NS_SCRIPTABLE NS_IMETHOD SetSrc(const nsAString & aSrc) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSrc(aSrc); } \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetType(aType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLSourceElement : public nsIDOMHTMLSourceElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLSOURCEELEMENT

  nsDOMHTMLSourceElement();

private:
  ~nsDOMHTMLSourceElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLSourceElement, nsIDOMHTMLSourceElement)

nsDOMHTMLSourceElement::nsDOMHTMLSourceElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLSourceElement::~nsDOMHTMLSourceElement()
{
  /* destructor code */
}

/* attribute DOMString src; */
NS_IMETHODIMP nsDOMHTMLSourceElement::GetSrc(nsAString & aSrc)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLSourceElement::SetSrc(const nsAString & aSrc)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString type; */
NS_IMETHODIMP nsDOMHTMLSourceElement::GetType(nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLSourceElement::SetType(const nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLSourceElement_h__ */
