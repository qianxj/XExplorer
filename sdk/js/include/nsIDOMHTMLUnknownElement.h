/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLUnknownElement.idl
 */

#ifndef __gen_nsIDOMHTMLUnknownElement_h__
#define __gen_nsIDOMHTMLUnknownElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLUnknownElement */
#define NS_IDOMHTMLUNKNOWNELEMENT_IID_STR "9d094117-9937-4ae4-a325-1761d7a9f3bc"

#define NS_IDOMHTMLUNKNOWNELEMENT_IID \
  {0x9d094117, 0x9937, 0x4ae4, \
    { 0xa3, 0x25, 0x17, 0x61, 0xd7, 0xa9, 0xf3, 0xbc }}

/**
 * The nsIDOMHTMLUnknownElement interface is the interface to an unknown HTML
 * element.
 *
 * @see <http://www.whatwg.org/html/#htmlunknownelement>
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLUnknownElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLUNKNOWNELEMENT_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLUnknownElement, NS_IDOMHTMLUNKNOWNELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLUNKNOWNELEMENT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLUNKNOWNELEMENT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLUNKNOWNELEMENT(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLUnknownElement : public nsIDOMHTMLUnknownElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLUNKNOWNELEMENT

  nsDOMHTMLUnknownElement();

private:
  ~nsDOMHTMLUnknownElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLUnknownElement, nsIDOMHTMLUnknownElement)

nsDOMHTMLUnknownElement::nsDOMHTMLUnknownElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLUnknownElement::~nsDOMHTMLUnknownElement()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLUnknownElement_h__ */
