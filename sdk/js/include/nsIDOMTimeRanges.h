/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMTimeRanges.idl
 */

#ifndef __gen_nsIDOMTimeRanges_h__
#define __gen_nsIDOMTimeRanges_h__


#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMTimeRanges */
#define NS_IDOMTIMERANGES_IID_STR "c43448db-0bab-461d-b648-1ca14a967f7e"

#define NS_IDOMTIMERANGES_IID \
  {0xc43448db, 0x0bab, 0x461d, \
    { 0xb6, 0x48, 0x1c, 0xa1, 0x4a, 0x96, 0x7f, 0x7e }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMTimeRanges : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMTIMERANGES_IID)

  /* readonly attribute unsigned long length; */
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) = 0;

  /* double start (in unsigned long index); */
  NS_SCRIPTABLE NS_IMETHOD Start(PRUint32 index, double *_retval NS_OUTPARAM) = 0;

  /* double end (in unsigned long index); */
  NS_SCRIPTABLE NS_IMETHOD End(PRUint32 index, double *_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMTimeRanges, NS_IDOMTIMERANGES_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMTIMERANGES \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength); \
  NS_SCRIPTABLE NS_IMETHOD Start(PRUint32 index, double *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD End(PRUint32 index, double *_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMTIMERANGES(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) { return _to GetLength(aLength); } \
  NS_SCRIPTABLE NS_IMETHOD Start(PRUint32 index, double *_retval NS_OUTPARAM) { return _to Start(index, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD End(PRUint32 index, double *_retval NS_OUTPARAM) { return _to End(index, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMTIMERANGES(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetLength(PRUint32 *aLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLength(aLength); } \
  NS_SCRIPTABLE NS_IMETHOD Start(PRUint32 index, double *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Start(index, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD End(PRUint32 index, double *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->End(index, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMTimeRanges : public nsIDOMTimeRanges
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMTIMERANGES

  nsDOMTimeRanges();

private:
  ~nsDOMTimeRanges();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMTimeRanges, nsIDOMTimeRanges)

nsDOMTimeRanges::nsDOMTimeRanges()
{
  /* member initializers and constructor code */
}

nsDOMTimeRanges::~nsDOMTimeRanges()
{
  /* destructor code */
}

/* readonly attribute unsigned long length; */
NS_IMETHODIMP nsDOMTimeRanges::GetLength(PRUint32 *aLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* double start (in unsigned long index); */
NS_IMETHODIMP nsDOMTimeRanges::Start(PRUint32 index, double *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* double end (in unsigned long index); */
NS_IMETHODIMP nsDOMTimeRanges::End(PRUint32 index, double *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMTimeRanges_h__ */
