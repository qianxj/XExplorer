/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/core/nsIDOMDocumentFragment.idl
 */

#ifndef __gen_nsIDOMDocumentFragment_h__
#define __gen_nsIDOMDocumentFragment_h__


#ifndef __gen_nsIDOMNode_h__
#include "nsIDOMNode.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMDocumentFragment */
#define NS_IDOMDOCUMENTFRAGMENT_IID_STR "06376ec5-7c91-45ad-a346-30a06a125935"

#define NS_IDOMDOCUMENTFRAGMENT_IID \
  {0x06376ec5, 0x7c91, 0x45ad, \
    { 0xa3, 0x46, 0x30, 0xa0, 0x6a, 0x12, 0x59, 0x35 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMDocumentFragment : public nsIDOMNode {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMDOCUMENTFRAGMENT_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMDocumentFragment, NS_IDOMDOCUMENTFRAGMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMDOCUMENTFRAGMENT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMDOCUMENTFRAGMENT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMDOCUMENTFRAGMENT(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMDocumentFragment : public nsIDOMDocumentFragment
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMDOCUMENTFRAGMENT

  nsDOMDocumentFragment();

private:
  ~nsDOMDocumentFragment();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMDocumentFragment, nsIDOMDocumentFragment)

nsDOMDocumentFragment::nsDOMDocumentFragment()
{
  /* member initializers and constructor code */
}

nsDOMDocumentFragment::~nsDOMDocumentFragment()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMDocumentFragment_h__ */
