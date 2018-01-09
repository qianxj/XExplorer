/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMNSHTMLHRElement.idl
 */

#ifndef __gen_nsIDOMNSHTMLHRElement_h__
#define __gen_nsIDOMNSHTMLHRElement_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMNSHTMLHRElement */
#define NS_IDOMNSHTMLHRELEMENT_IID_STR "63c0ae1b-8aa7-4e72-82a1-aff486bfdaf5"

#define NS_IDOMNSHTMLHRELEMENT_IID \
  {0x63c0ae1b, 0x8aa7, 0x4e72, \
    { 0x82, 0xa1, 0xaf, 0xf4, 0x86, 0xbf, 0xda, 0xf5 }}

/**
 * The nsIDOMNSHTMLHRElement interface contains extensions to the
 * interface for [X]HTML hr elements, for compatibility with IE.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMNSHTMLHRElement : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMNSHTMLHRELEMENT_IID)

  /* attribute DOMString color; */
  NS_SCRIPTABLE NS_IMETHOD GetColor(nsAString & aColor) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetColor(const nsAString & aColor) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMNSHTMLHRElement, NS_IDOMNSHTMLHRELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMNSHTMLHRELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetColor(nsAString & aColor); \
  NS_SCRIPTABLE NS_IMETHOD SetColor(const nsAString & aColor); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMNSHTMLHRELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetColor(nsAString & aColor) { return _to GetColor(aColor); } \
  NS_SCRIPTABLE NS_IMETHOD SetColor(const nsAString & aColor) { return _to SetColor(aColor); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMNSHTMLHRELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetColor(nsAString & aColor) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetColor(aColor); } \
  NS_SCRIPTABLE NS_IMETHOD SetColor(const nsAString & aColor) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetColor(aColor); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMNSHTMLHRElement : public nsIDOMNSHTMLHRElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMNSHTMLHRELEMENT

  nsDOMNSHTMLHRElement();

private:
  ~nsDOMNSHTMLHRElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMNSHTMLHRElement, nsIDOMNSHTMLHRElement)

nsDOMNSHTMLHRElement::nsDOMNSHTMLHRElement()
{
  /* member initializers and constructor code */
}

nsDOMNSHTMLHRElement::~nsDOMNSHTMLHRElement()
{
  /* destructor code */
}

/* attribute DOMString color; */
NS_IMETHODIMP nsDOMNSHTMLHRElement::GetColor(nsAString & aColor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMNSHTMLHRElement::SetColor(const nsAString & aColor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMNSHTMLHRElement_h__ */
