/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/core/nsIDOMComment.idl
 */

#ifndef __gen_nsIDOMComment_h__
#define __gen_nsIDOMComment_h__


#ifndef __gen_nsIDOMCharacterData_h__
#include "nsIDOMCharacterData.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMComment */
#define NS_IDOMCOMMENT_IID_STR "eaf04950-d409-41a0-a99d-2e4e43c1e33d"

#define NS_IDOMCOMMENT_IID \
  {0xeaf04950, 0xd409, 0x41a0, \
    { 0xa9, 0x9d, 0x2e, 0x4e, 0x43, 0xc1, 0xe3, 0x3d }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMComment : public nsIDOMCharacterData {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMCOMMENT_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMComment, NS_IDOMCOMMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMCOMMENT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMCOMMENT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMCOMMENT(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMComment : public nsIDOMComment
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMCOMMENT

  nsDOMComment();

private:
  ~nsDOMComment();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMComment, nsIDOMComment)

nsDOMComment::nsDOMComment()
{
  /* member initializers and constructor code */
}

nsDOMComment::~nsDOMComment()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMComment_h__ */
