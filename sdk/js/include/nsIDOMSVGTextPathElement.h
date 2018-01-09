/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGTextPathElement.idl
 */

#ifndef __gen_nsIDOMSVGTextPathElement_h__
#define __gen_nsIDOMSVGTextPathElement_h__


#ifndef __gen_nsIDOMSVGTextContentElement_h__
#include "nsIDOMSVGTextContentElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMSVGAnimatedLength; /* forward declaration */

class nsIDOMSVGAnimatedEnumeration; /* forward declaration */


/* starting interface:    nsIDOMSVGTextPathElement */
#define NS_IDOMSVGTEXTPATHELEMENT_IID_STR "55c2dbe7-c3e2-4e45-aeb1-b81802ceec48"

#define NS_IDOMSVGTEXTPATHELEMENT_IID \
  {0x55c2dbe7, 0xc3e2, 0x4e45, \
    { 0xae, 0xb1, 0xb8, 0x18, 0x02, 0xce, 0xec, 0x48 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGTextPathElement : public nsIDOMSVGTextContentElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGTEXTPATHELEMENT_IID)

  enum { TEXTPATH_METHODTYPE_UNKNOWN = 0U };

  enum { TEXTPATH_METHODTYPE_ALIGN = 1U };

  enum { TEXTPATH_METHODTYPE_STRETCH = 2U };

  enum { TEXTPATH_SPACINGTYPE_UNKNOWN = 0U };

  enum { TEXTPATH_SPACINGTYPE_AUTO = 1U };

  enum { TEXTPATH_SPACINGTYPE_EXACT = 2U };

  /* readonly attribute nsIDOMSVGAnimatedLength startOffset; */
  NS_SCRIPTABLE NS_IMETHOD GetStartOffset(nsIDOMSVGAnimatedLength **aStartOffset) = 0;

  /* readonly attribute nsIDOMSVGAnimatedEnumeration method; */
  NS_SCRIPTABLE NS_IMETHOD GetMethod(nsIDOMSVGAnimatedEnumeration **aMethod) = 0;

  /* readonly attribute nsIDOMSVGAnimatedEnumeration spacing; */
  NS_SCRIPTABLE NS_IMETHOD GetSpacing(nsIDOMSVGAnimatedEnumeration **aSpacing) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGTextPathElement, NS_IDOMSVGTEXTPATHELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGTEXTPATHELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetStartOffset(nsIDOMSVGAnimatedLength **aStartOffset); \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(nsIDOMSVGAnimatedEnumeration **aMethod); \
  NS_SCRIPTABLE NS_IMETHOD GetSpacing(nsIDOMSVGAnimatedEnumeration **aSpacing); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGTEXTPATHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStartOffset(nsIDOMSVGAnimatedLength **aStartOffset) { return _to GetStartOffset(aStartOffset); } \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(nsIDOMSVGAnimatedEnumeration **aMethod) { return _to GetMethod(aMethod); } \
  NS_SCRIPTABLE NS_IMETHOD GetSpacing(nsIDOMSVGAnimatedEnumeration **aSpacing) { return _to GetSpacing(aSpacing); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGTEXTPATHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStartOffset(nsIDOMSVGAnimatedLength **aStartOffset) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStartOffset(aStartOffset); } \
  NS_SCRIPTABLE NS_IMETHOD GetMethod(nsIDOMSVGAnimatedEnumeration **aMethod) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethod(aMethod); } \
  NS_SCRIPTABLE NS_IMETHOD GetSpacing(nsIDOMSVGAnimatedEnumeration **aSpacing) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSpacing(aSpacing); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGTextPathElement : public nsIDOMSVGTextPathElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGTEXTPATHELEMENT

  nsDOMSVGTextPathElement();

private:
  ~nsDOMSVGTextPathElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGTextPathElement, nsIDOMSVGTextPathElement)

nsDOMSVGTextPathElement::nsDOMSVGTextPathElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGTextPathElement::~nsDOMSVGTextPathElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMSVGAnimatedLength startOffset; */
NS_IMETHODIMP nsDOMSVGTextPathElement::GetStartOffset(nsIDOMSVGAnimatedLength **aStartOffset)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedEnumeration method; */
NS_IMETHODIMP nsDOMSVGTextPathElement::GetMethod(nsIDOMSVGAnimatedEnumeration **aMethod)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedEnumeration spacing; */
NS_IMETHODIMP nsDOMSVGTextPathElement::GetSpacing(nsIDOMSVGAnimatedEnumeration **aSpacing)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGTextPathElement_h__ */
