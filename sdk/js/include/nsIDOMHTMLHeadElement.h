/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLHeadElement.idl
 */

#ifndef __gen_nsIDOMHTMLHeadElement_h__
#define __gen_nsIDOMHTMLHeadElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLHeadElement */
#define NS_IDOMHTMLHEADELEMENT_IID_STR "0d91b09d-0de9-4d9b-a53d-ce9b4c462f5a"

#define NS_IDOMHTMLHEADELEMENT_IID \
  {0x0d91b09d, 0x0de9, 0x4d9b, \
    { 0xa5, 0x3d, 0xce, 0x9b, 0x4c, 0x46, 0x2f, 0x5a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLHeadElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLHEADELEMENT_IID)

  /**
 * The nsIDOMHTMLHeadElement interface is the interface to a [X]HTML
 * head element.
 *
 * This interface is trying to follow the DOM Level 2 HTML specification:
 * http://www.w3.org/TR/DOM-Level-2-HTML/
 *
 * with changes from the work-in-progress WHATWG HTML specification:
 * http://www.whatwg.org/specs/web-apps/current-work/
 */
  /* [noscript] attribute DOMString profile; */
  NS_IMETHOD GetProfile(nsAString & aProfile) = 0;
  NS_IMETHOD SetProfile(const nsAString & aProfile) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLHeadElement, NS_IDOMHTMLHEADELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLHEADELEMENT \
  NS_IMETHOD GetProfile(nsAString & aProfile); \
  NS_IMETHOD SetProfile(const nsAString & aProfile); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLHEADELEMENT(_to) \
  NS_IMETHOD GetProfile(nsAString & aProfile) { return _to GetProfile(aProfile); } \
  NS_IMETHOD SetProfile(const nsAString & aProfile) { return _to SetProfile(aProfile); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLHEADELEMENT(_to) \
  NS_IMETHOD GetProfile(nsAString & aProfile) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProfile(aProfile); } \
  NS_IMETHOD SetProfile(const nsAString & aProfile) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetProfile(aProfile); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLHeadElement : public nsIDOMHTMLHeadElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLHEADELEMENT

  nsDOMHTMLHeadElement();

private:
  ~nsDOMHTMLHeadElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLHeadElement, nsIDOMHTMLHeadElement)

nsDOMHTMLHeadElement::nsDOMHTMLHeadElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLHeadElement::~nsDOMHTMLHeadElement()
{
  /* destructor code */
}

/* [noscript] attribute DOMString profile; */
NS_IMETHODIMP nsDOMHTMLHeadElement::GetProfile(nsAString & aProfile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLHeadElement::SetProfile(const nsAString & aProfile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLHeadElement_h__ */
