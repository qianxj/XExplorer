/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGMpathElement.idl
 */

#ifndef __gen_nsIDOMSVGMpathElement_h__
#define __gen_nsIDOMSVGMpathElement_h__


#ifndef __gen_nsIDOMSVGElement_h__
#include "nsIDOMSVGElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMSVGMpathElement */
#define NS_IDOMSVGMPATHELEMENT_IID_STR "39b7550c-26f7-475b-9ac5-a013935d0e60"

#define NS_IDOMSVGMPATHELEMENT_IID \
  {0x39b7550c, 0x26f7, 0x475b, \
    { 0x9a, 0xc5, 0xa0, 0x13, 0x93, 0x5d, 0x0e, 0x60 }}

/**
 * For more information on this interface please see
 * http://www.w3.org/TR/SVG/animate.html#mpathElement
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGMpathElement : public nsIDOMSVGElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGMPATHELEMENT_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGMpathElement, NS_IDOMSVGMPATHELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGMPATHELEMENT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGMPATHELEMENT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGMPATHELEMENT(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGMpathElement : public nsIDOMSVGMpathElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGMPATHELEMENT

  nsDOMSVGMpathElement();

private:
  ~nsDOMSVGMpathElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGMpathElement, nsIDOMSVGMpathElement)

nsDOMSVGMpathElement::nsDOMSVGMpathElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGMpathElement::~nsDOMSVGMpathElement()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGMpathElement_h__ */
