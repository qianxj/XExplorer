/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/xpath/nsIDOMXPathNamespace.idl
 */

#ifndef __gen_nsIDOMXPathNamespace_h__
#define __gen_nsIDOMXPathNamespace_h__


#ifndef __gen_nsIDOMNode_h__
#include "nsIDOMNode.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMXPathNamespace */
#define NS_IDOMXPATHNAMESPACE_IID_STR "978f4c11-3b29-40af-8c29-a4e0689f2776"

#define NS_IDOMXPATHNAMESPACE_IID \
  {0x978f4c11, 0x3b29, 0x40af, \
    { 0x8c, 0x29, 0xa4, 0xe0, 0x68, 0x9f, 0x27, 0x76 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMXPathNamespace : public nsIDOMNode {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMXPATHNAMESPACE_IID)

  enum { XPATH_NAMESPACE_NODE = 13U };

  /* readonly attribute nsIDOMElement ownerElement; */
  NS_SCRIPTABLE NS_IMETHOD GetOwnerElement(nsIDOMElement **aOwnerElement) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMXPathNamespace, NS_IDOMXPATHNAMESPACE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMXPATHNAMESPACE \
  NS_SCRIPTABLE NS_IMETHOD GetOwnerElement(nsIDOMElement **aOwnerElement); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMXPATHNAMESPACE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetOwnerElement(nsIDOMElement **aOwnerElement) { return _to GetOwnerElement(aOwnerElement); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMXPATHNAMESPACE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetOwnerElement(nsIDOMElement **aOwnerElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOwnerElement(aOwnerElement); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMXPathNamespace : public nsIDOMXPathNamespace
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMXPATHNAMESPACE

  nsDOMXPathNamespace();

private:
  ~nsDOMXPathNamespace();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMXPathNamespace, nsIDOMXPathNamespace)

nsDOMXPathNamespace::nsDOMXPathNamespace()
{
  /* member initializers and constructor code */
}

nsDOMXPathNamespace::~nsDOMXPathNamespace()
{
  /* destructor code */
}

/* readonly attribute nsIDOMElement ownerElement; */
NS_IMETHODIMP nsDOMXPathNamespace::GetOwnerElement(nsIDOMElement **aOwnerElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMXPathNamespace_h__ */
