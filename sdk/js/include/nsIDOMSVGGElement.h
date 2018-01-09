/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGGElement.idl
 */

#ifndef __gen_nsIDOMSVGGElement_h__
#define __gen_nsIDOMSVGGElement_h__


#ifndef __gen_nsIDOMSVGElement_h__
#include "nsIDOMSVGElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMSVGGElement */
#define NS_IDOMSVGGELEMENT_IID_STR "86317234-cf55-4ff0-8fed-de48557ae82e"

#define NS_IDOMSVGGELEMENT_IID \
  {0x86317234, 0xcf55, 0x4ff0, \
    { 0x8f, 0xed, 0xde, 0x48, 0x55, 0x7a, 0xe8, 0x2e }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGGElement : public nsIDOMSVGElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGGELEMENT_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGGElement, NS_IDOMSVGGELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGGELEMENT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGGELEMENT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGGELEMENT(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGGElement : public nsIDOMSVGGElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGGELEMENT

  nsDOMSVGGElement();

private:
  ~nsDOMSVGGElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGGElement, nsIDOMSVGGElement)

nsDOMSVGGElement::nsDOMSVGGElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGGElement::~nsDOMSVGGElement()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGGElement_h__ */
