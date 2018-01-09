/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGClipPathElement.idl
 */

#ifndef __gen_nsIDOMSVGClipPathElement_h__
#define __gen_nsIDOMSVGClipPathElement_h__


#ifndef __gen_nsIDOMSVGElement_h__
#include "nsIDOMSVGElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMSVGAnimatedEnumeration; /* forward declaration */


/* starting interface:    nsIDOMSVGClipPathElement */
#define NS_IDOMSVGCLIPPATHELEMENT_IID_STR "3dd19de9-deee-4105-9c5c-c51c68b31326"

#define NS_IDOMSVGCLIPPATHELEMENT_IID \
  {0x3dd19de9, 0xdeee, 0x4105, \
    { 0x9c, 0x5c, 0xc5, 0x1c, 0x68, 0xb3, 0x13, 0x26 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGClipPathElement : public nsIDOMSVGElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGCLIPPATHELEMENT_IID)

  /* readonly attribute nsIDOMSVGAnimatedEnumeration clipPathUnits; */
  NS_SCRIPTABLE NS_IMETHOD GetClipPathUnits(nsIDOMSVGAnimatedEnumeration **aClipPathUnits) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGClipPathElement, NS_IDOMSVGCLIPPATHELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGCLIPPATHELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetClipPathUnits(nsIDOMSVGAnimatedEnumeration **aClipPathUnits); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGCLIPPATHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetClipPathUnits(nsIDOMSVGAnimatedEnumeration **aClipPathUnits) { return _to GetClipPathUnits(aClipPathUnits); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGCLIPPATHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetClipPathUnits(nsIDOMSVGAnimatedEnumeration **aClipPathUnits) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClipPathUnits(aClipPathUnits); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGClipPathElement : public nsIDOMSVGClipPathElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGCLIPPATHELEMENT

  nsDOMSVGClipPathElement();

private:
  ~nsDOMSVGClipPathElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGClipPathElement, nsIDOMSVGClipPathElement)

nsDOMSVGClipPathElement::nsDOMSVGClipPathElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGClipPathElement::~nsDOMSVGClipPathElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMSVGAnimatedEnumeration clipPathUnits; */
NS_IMETHODIMP nsDOMSVGClipPathElement::GetClipPathUnits(nsIDOMSVGAnimatedEnumeration **aClipPathUnits)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGClipPathElement_h__ */
