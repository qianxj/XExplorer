/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGScriptElement.idl
 */

#ifndef __gen_nsIDOMSVGScriptElement_h__
#define __gen_nsIDOMSVGScriptElement_h__


#ifndef __gen_nsIDOMSVGElement_h__
#include "nsIDOMSVGElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMSVGScriptElement */
#define NS_IDOMSVGSCRIPTELEMENT_IID_STR "26525c48-f1fa-453b-b19a-6586b2d88e1d"

#define NS_IDOMSVGSCRIPTELEMENT_IID \
  {0x26525c48, 0xf1fa, 0x453b, \
    { 0xb1, 0x9a, 0x65, 0x86, 0xb2, 0xd8, 0x8e, 0x1d }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGScriptElement : public nsIDOMSVGElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGSCRIPTELEMENT_IID)

  /* attribute DOMString type; */
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGScriptElement, NS_IDOMSVGSCRIPTELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGSCRIPTELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType); \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGSCRIPTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return _to GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) { return _to SetType(aType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGSCRIPTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD SetType(const nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetType(aType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGScriptElement : public nsIDOMSVGScriptElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGSCRIPTELEMENT

  nsDOMSVGScriptElement();

private:
  ~nsDOMSVGScriptElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGScriptElement, nsIDOMSVGScriptElement)

nsDOMSVGScriptElement::nsDOMSVGScriptElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGScriptElement::~nsDOMSVGScriptElement()
{
  /* destructor code */
}

/* attribute DOMString type; */
NS_IMETHODIMP nsDOMSVGScriptElement::GetType(nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMSVGScriptElement::SetType(const nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGScriptElement_h__ */
