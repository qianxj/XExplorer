/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_out.idl
 */

#ifndef __gen_xpctest_out_h__
#define __gen_xpctest_out_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestOut */
#define NS_IXPCTESTOUT_IID_STR "4105ae88-5599-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTOUT_IID \
  {0x4105ae88, 0x5599, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestOut : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTOUT_IID)

  /* void GetLong (out long l); */
  NS_SCRIPTABLE NS_IMETHOD GetLong(PRInt32 *l NS_OUTPARAM) = 0;

  /* void SetLong (in long l); */
  NS_SCRIPTABLE NS_IMETHOD SetLong(PRInt32 l) = 0;

  /* void GetShort (out short s); */
  NS_SCRIPTABLE NS_IMETHOD GetShort(PRInt16 *s NS_OUTPARAM) = 0;

  /* void SetShort (in short s); */
  NS_SCRIPTABLE NS_IMETHOD SetShort(PRInt16 s) = 0;

  /* void SetChar (in char c); */
  NS_SCRIPTABLE NS_IMETHOD SetChar(char c) = 0;

  /* void GetChar (out char c); */
  NS_SCRIPTABLE NS_IMETHOD GetChar(char *c NS_OUTPARAM) = 0;

  /* void GetBoolean (out boolean b); */
  NS_SCRIPTABLE NS_IMETHOD GetBoolean(PRBool *b NS_OUTPARAM) = 0;

  /* void SetBoolean (in boolean b); */
  NS_SCRIPTABLE NS_IMETHOD SetBoolean(PRBool b) = 0;

  /* void GetOctet (out octet o); */
  NS_SCRIPTABLE NS_IMETHOD GetOctet(PRUint8 *o NS_OUTPARAM) = 0;

  /* void SetOctet (in octet o); */
  NS_SCRIPTABLE NS_IMETHOD SetOctet(PRUint8 o) = 0;

  /* void GetLongLong (out long long ll); */
  NS_SCRIPTABLE NS_IMETHOD GetLongLong(PRInt64 *ll NS_OUTPARAM) = 0;

  /* void SetLongLong (in long long ll); */
  NS_SCRIPTABLE NS_IMETHOD SetLongLong(PRInt64 ll) = 0;

  /* void GetUnsignedShort (out unsigned short us); */
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedShort(PRUint16 *us NS_OUTPARAM) = 0;

  /* void SetUnsignedShort (in unsigned short us); */
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedShort(PRUint16 us) = 0;

  /* void GetUnsignedLong (out unsigned long ul); */
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedLong(PRUint32 *ul NS_OUTPARAM) = 0;

  /* void SetUnsignedLong (in unsigned long ul); */
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedLong(PRUint32 ul) = 0;

  /* void GetFloat (out float f); */
  NS_SCRIPTABLE NS_IMETHOD GetFloat(float *f NS_OUTPARAM) = 0;

  /* void SetFloat (in float f); */
  NS_SCRIPTABLE NS_IMETHOD SetFloat(float f) = 0;

  /* void GetDouble (out double d); */
  NS_SCRIPTABLE NS_IMETHOD GetDouble(double *d NS_OUTPARAM) = 0;

  /* void SetDouble (in double d); */
  NS_SCRIPTABLE NS_IMETHOD SetDouble(double d) = 0;

  /* void GetWchar (out wchar wc); */
  NS_SCRIPTABLE NS_IMETHOD GetWchar(PRUnichar *wc NS_OUTPARAM) = 0;

  /* void SetWchar (in wchar wc); */
  NS_SCRIPTABLE NS_IMETHOD SetWchar(PRUnichar wc) = 0;

  /* void GetPRBool (out PRBool b); */
  NS_SCRIPTABLE NS_IMETHOD GetPRBool(PRBool *b NS_OUTPARAM) = 0;

  /* void SetPRBool (in PRBool b); */
  NS_SCRIPTABLE NS_IMETHOD SetPRBool(PRBool b) = 0;

  /* void GetPRInt32 (out PRInt32 l); */
  NS_SCRIPTABLE NS_IMETHOD GetPRInt32(PRInt32 *l NS_OUTPARAM) = 0;

  /* void SetPRInt32 (in PRInt32 l); */
  NS_SCRIPTABLE NS_IMETHOD SetPRInt32(PRInt32 l) = 0;

  /* void GetPRInt16 (out PRInt16 l); */
  NS_SCRIPTABLE NS_IMETHOD GetPRInt16(PRInt16 *l NS_OUTPARAM) = 0;

  /* void SetPRInt16 (in PRInt16 l); */
  NS_SCRIPTABLE NS_IMETHOD SetPRInt16(PRInt16 l) = 0;

  /* void GetPRInt64 (out PRInt64 i); */
  NS_SCRIPTABLE NS_IMETHOD GetPRInt64(PRInt64 *i NS_OUTPARAM) = 0;

  /* void SetPRInt64 (in PRInt64 i); */
  NS_SCRIPTABLE NS_IMETHOD SetPRInt64(PRInt64 i) = 0;

  /* void GetPRUint8 (out PRUint8 i); */
  NS_SCRIPTABLE NS_IMETHOD GetPRUint8(PRUint8 *i NS_OUTPARAM) = 0;

  /* void SetPRUint8 (in PRUint8 i); */
  NS_SCRIPTABLE NS_IMETHOD SetPRUint8(PRUint8 i) = 0;

  /* void GetPRUint16 (out PRUint16 i); */
  NS_SCRIPTABLE NS_IMETHOD GetPRUint16(PRUint16 *i NS_OUTPARAM) = 0;

  /* void SetPRUint16 (in PRUint16 i); */
  NS_SCRIPTABLE NS_IMETHOD SetPRUint16(PRUint16 i) = 0;

  /* void GetPRUint32 (out PRUint32 i); */
  NS_SCRIPTABLE NS_IMETHOD GetPRUint32(PRUint32 *i NS_OUTPARAM) = 0;

  /* void SetPRUint32 (in PRUint32 i); */
  NS_SCRIPTABLE NS_IMETHOD SetPRUint32(PRUint32 i) = 0;

  /* void GetPRUint64 (out PRUint64 i); */
  NS_SCRIPTABLE NS_IMETHOD GetPRUint64(PRUint64 *i NS_OUTPARAM) = 0;

  /* void SetPRUint64 (in PRUint64 i); */
  NS_SCRIPTABLE NS_IMETHOD SetPRUint64(PRUint64 i) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestOut, NS_IXPCTESTOUT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTOUT \
  NS_SCRIPTABLE NS_IMETHOD GetLong(PRInt32 *l NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetLong(PRInt32 l); \
  NS_SCRIPTABLE NS_IMETHOD GetShort(PRInt16 *s NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetShort(PRInt16 s); \
  NS_SCRIPTABLE NS_IMETHOD SetChar(char c); \
  NS_SCRIPTABLE NS_IMETHOD GetChar(char *c NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetBoolean(PRBool *b NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetBoolean(PRBool b); \
  NS_SCRIPTABLE NS_IMETHOD GetOctet(PRUint8 *o NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetOctet(PRUint8 o); \
  NS_SCRIPTABLE NS_IMETHOD GetLongLong(PRInt64 *ll NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetLongLong(PRInt64 ll); \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedShort(PRUint16 *us NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedShort(PRUint16 us); \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedLong(PRUint32 *ul NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedLong(PRUint32 ul); \
  NS_SCRIPTABLE NS_IMETHOD GetFloat(float *f NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetFloat(float f); \
  NS_SCRIPTABLE NS_IMETHOD GetDouble(double *d NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetDouble(double d); \
  NS_SCRIPTABLE NS_IMETHOD GetWchar(PRUnichar *wc NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetWchar(PRUnichar wc); \
  NS_SCRIPTABLE NS_IMETHOD GetPRBool(PRBool *b NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRBool(PRBool b); \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt32(PRInt32 *l NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt32(PRInt32 l); \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt16(PRInt16 *l NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt16(PRInt16 l); \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt64(PRInt64 *i NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt64(PRInt64 i); \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint8(PRUint8 *i NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint8(PRUint8 i); \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint16(PRUint16 *i NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint16(PRUint16 i); \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint32(PRUint32 *i NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint32(PRUint32 i); \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint64(PRUint64 *i NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint64(PRUint64 i); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTOUT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetLong(PRInt32 *l NS_OUTPARAM) { return _to GetLong(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetLong(PRInt32 l) { return _to SetLong(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetShort(PRInt16 *s NS_OUTPARAM) { return _to GetShort(s); } \
  NS_SCRIPTABLE NS_IMETHOD SetShort(PRInt16 s) { return _to SetShort(s); } \
  NS_SCRIPTABLE NS_IMETHOD SetChar(char c) { return _to SetChar(c); } \
  NS_SCRIPTABLE NS_IMETHOD GetChar(char *c NS_OUTPARAM) { return _to GetChar(c); } \
  NS_SCRIPTABLE NS_IMETHOD GetBoolean(PRBool *b NS_OUTPARAM) { return _to GetBoolean(b); } \
  NS_SCRIPTABLE NS_IMETHOD SetBoolean(PRBool b) { return _to SetBoolean(b); } \
  NS_SCRIPTABLE NS_IMETHOD GetOctet(PRUint8 *o NS_OUTPARAM) { return _to GetOctet(o); } \
  NS_SCRIPTABLE NS_IMETHOD SetOctet(PRUint8 o) { return _to SetOctet(o); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongLong(PRInt64 *ll NS_OUTPARAM) { return _to GetLongLong(ll); } \
  NS_SCRIPTABLE NS_IMETHOD SetLongLong(PRInt64 ll) { return _to SetLongLong(ll); } \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedShort(PRUint16 *us NS_OUTPARAM) { return _to GetUnsignedShort(us); } \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedShort(PRUint16 us) { return _to SetUnsignedShort(us); } \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedLong(PRUint32 *ul NS_OUTPARAM) { return _to GetUnsignedLong(ul); } \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedLong(PRUint32 ul) { return _to SetUnsignedLong(ul); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloat(float *f NS_OUTPARAM) { return _to GetFloat(f); } \
  NS_SCRIPTABLE NS_IMETHOD SetFloat(float f) { return _to SetFloat(f); } \
  NS_SCRIPTABLE NS_IMETHOD GetDouble(double *d NS_OUTPARAM) { return _to GetDouble(d); } \
  NS_SCRIPTABLE NS_IMETHOD SetDouble(double d) { return _to SetDouble(d); } \
  NS_SCRIPTABLE NS_IMETHOD GetWchar(PRUnichar *wc NS_OUTPARAM) { return _to GetWchar(wc); } \
  NS_SCRIPTABLE NS_IMETHOD SetWchar(PRUnichar wc) { return _to SetWchar(wc); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRBool(PRBool *b NS_OUTPARAM) { return _to GetPRBool(b); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRBool(PRBool b) { return _to SetPRBool(b); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt32(PRInt32 *l NS_OUTPARAM) { return _to GetPRInt32(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt32(PRInt32 l) { return _to SetPRInt32(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt16(PRInt16 *l NS_OUTPARAM) { return _to GetPRInt16(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt16(PRInt16 l) { return _to SetPRInt16(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt64(PRInt64 *i NS_OUTPARAM) { return _to GetPRInt64(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt64(PRInt64 i) { return _to SetPRInt64(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint8(PRUint8 *i NS_OUTPARAM) { return _to GetPRUint8(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint8(PRUint8 i) { return _to SetPRUint8(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint16(PRUint16 *i NS_OUTPARAM) { return _to GetPRUint16(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint16(PRUint16 i) { return _to SetPRUint16(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint32(PRUint32 *i NS_OUTPARAM) { return _to GetPRUint32(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint32(PRUint32 i) { return _to SetPRUint32(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint64(PRUint64 *i NS_OUTPARAM) { return _to GetPRUint64(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint64(PRUint64 i) { return _to SetPRUint64(i); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTOUT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetLong(PRInt32 *l NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLong(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetLong(PRInt32 l) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLong(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetShort(PRInt16 *s NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetShort(s); } \
  NS_SCRIPTABLE NS_IMETHOD SetShort(PRInt16 s) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetShort(s); } \
  NS_SCRIPTABLE NS_IMETHOD SetChar(char c) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChar(c); } \
  NS_SCRIPTABLE NS_IMETHOD GetChar(char *c NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChar(c); } \
  NS_SCRIPTABLE NS_IMETHOD GetBoolean(PRBool *b NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBoolean(b); } \
  NS_SCRIPTABLE NS_IMETHOD SetBoolean(PRBool b) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBoolean(b); } \
  NS_SCRIPTABLE NS_IMETHOD GetOctet(PRUint8 *o NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOctet(o); } \
  NS_SCRIPTABLE NS_IMETHOD SetOctet(PRUint8 o) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetOctet(o); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongLong(PRInt64 *ll NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLongLong(ll); } \
  NS_SCRIPTABLE NS_IMETHOD SetLongLong(PRInt64 ll) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLongLong(ll); } \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedShort(PRUint16 *us NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUnsignedShort(us); } \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedShort(PRUint16 us) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetUnsignedShort(us); } \
  NS_SCRIPTABLE NS_IMETHOD GetUnsignedLong(PRUint32 *ul NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUnsignedLong(ul); } \
  NS_SCRIPTABLE NS_IMETHOD SetUnsignedLong(PRUint32 ul) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetUnsignedLong(ul); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloat(float *f NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloat(f); } \
  NS_SCRIPTABLE NS_IMETHOD SetFloat(float f) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFloat(f); } \
  NS_SCRIPTABLE NS_IMETHOD GetDouble(double *d NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDouble(d); } \
  NS_SCRIPTABLE NS_IMETHOD SetDouble(double d) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDouble(d); } \
  NS_SCRIPTABLE NS_IMETHOD GetWchar(PRUnichar *wc NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWchar(wc); } \
  NS_SCRIPTABLE NS_IMETHOD SetWchar(PRUnichar wc) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetWchar(wc); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRBool(PRBool *b NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRBool(b); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRBool(PRBool b) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRBool(b); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt32(PRInt32 *l NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRInt32(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt32(PRInt32 l) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRInt32(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt16(PRInt16 *l NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRInt16(l); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt16(PRInt16 l) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRInt16(l); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRInt64(PRInt64 *i NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRInt64(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRInt64(PRInt64 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRInt64(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint8(PRUint8 *i NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRUint8(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint8(PRUint8 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRUint8(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint16(PRUint16 *i NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRUint16(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint16(PRUint16 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRUint16(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint32(PRUint32 *i NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRUint32(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint32(PRUint32 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRUint32(i); } \
  NS_SCRIPTABLE NS_IMETHOD GetPRUint64(PRUint64 *i NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPRUint64(i); } \
  NS_SCRIPTABLE NS_IMETHOD SetPRUint64(PRUint64 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPRUint64(i); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestOut : public nsIXPCTestOut
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTOUT

  nsXPCTestOut();

private:
  ~nsXPCTestOut();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestOut, nsIXPCTestOut)

nsXPCTestOut::nsXPCTestOut()
{
  /* member initializers and constructor code */
}

nsXPCTestOut::~nsXPCTestOut()
{
  /* destructor code */
}

/* void GetLong (out long l); */
NS_IMETHODIMP nsXPCTestOut::GetLong(PRInt32 *l NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetLong (in long l); */
NS_IMETHODIMP nsXPCTestOut::SetLong(PRInt32 l)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetShort (out short s); */
NS_IMETHODIMP nsXPCTestOut::GetShort(PRInt16 *s NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetShort (in short s); */
NS_IMETHODIMP nsXPCTestOut::SetShort(PRInt16 s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetChar (in char c); */
NS_IMETHODIMP nsXPCTestOut::SetChar(char c)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetChar (out char c); */
NS_IMETHODIMP nsXPCTestOut::GetChar(char *c NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetBoolean (out boolean b); */
NS_IMETHODIMP nsXPCTestOut::GetBoolean(PRBool *b NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetBoolean (in boolean b); */
NS_IMETHODIMP nsXPCTestOut::SetBoolean(PRBool b)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetOctet (out octet o); */
NS_IMETHODIMP nsXPCTestOut::GetOctet(PRUint8 *o NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetOctet (in octet o); */
NS_IMETHODIMP nsXPCTestOut::SetOctet(PRUint8 o)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetLongLong (out long long ll); */
NS_IMETHODIMP nsXPCTestOut::GetLongLong(PRInt64 *ll NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetLongLong (in long long ll); */
NS_IMETHODIMP nsXPCTestOut::SetLongLong(PRInt64 ll)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetUnsignedShort (out unsigned short us); */
NS_IMETHODIMP nsXPCTestOut::GetUnsignedShort(PRUint16 *us NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetUnsignedShort (in unsigned short us); */
NS_IMETHODIMP nsXPCTestOut::SetUnsignedShort(PRUint16 us)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetUnsignedLong (out unsigned long ul); */
NS_IMETHODIMP nsXPCTestOut::GetUnsignedLong(PRUint32 *ul NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetUnsignedLong (in unsigned long ul); */
NS_IMETHODIMP nsXPCTestOut::SetUnsignedLong(PRUint32 ul)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetFloat (out float f); */
NS_IMETHODIMP nsXPCTestOut::GetFloat(float *f NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetFloat (in float f); */
NS_IMETHODIMP nsXPCTestOut::SetFloat(float f)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetDouble (out double d); */
NS_IMETHODIMP nsXPCTestOut::GetDouble(double *d NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetDouble (in double d); */
NS_IMETHODIMP nsXPCTestOut::SetDouble(double d)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetWchar (out wchar wc); */
NS_IMETHODIMP nsXPCTestOut::GetWchar(PRUnichar *wc NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetWchar (in wchar wc); */
NS_IMETHODIMP nsXPCTestOut::SetWchar(PRUnichar wc)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRBool (out PRBool b); */
NS_IMETHODIMP nsXPCTestOut::GetPRBool(PRBool *b NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRBool (in PRBool b); */
NS_IMETHODIMP nsXPCTestOut::SetPRBool(PRBool b)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRInt32 (out PRInt32 l); */
NS_IMETHODIMP nsXPCTestOut::GetPRInt32(PRInt32 *l NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRInt32 (in PRInt32 l); */
NS_IMETHODIMP nsXPCTestOut::SetPRInt32(PRInt32 l)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRInt16 (out PRInt16 l); */
NS_IMETHODIMP nsXPCTestOut::GetPRInt16(PRInt16 *l NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRInt16 (in PRInt16 l); */
NS_IMETHODIMP nsXPCTestOut::SetPRInt16(PRInt16 l)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRInt64 (out PRInt64 i); */
NS_IMETHODIMP nsXPCTestOut::GetPRInt64(PRInt64 *i NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRInt64 (in PRInt64 i); */
NS_IMETHODIMP nsXPCTestOut::SetPRInt64(PRInt64 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRUint8 (out PRUint8 i); */
NS_IMETHODIMP nsXPCTestOut::GetPRUint8(PRUint8 *i NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRUint8 (in PRUint8 i); */
NS_IMETHODIMP nsXPCTestOut::SetPRUint8(PRUint8 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRUint16 (out PRUint16 i); */
NS_IMETHODIMP nsXPCTestOut::GetPRUint16(PRUint16 *i NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRUint16 (in PRUint16 i); */
NS_IMETHODIMP nsXPCTestOut::SetPRUint16(PRUint16 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRUint32 (out PRUint32 i); */
NS_IMETHODIMP nsXPCTestOut::GetPRUint32(PRUint32 *i NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRUint32 (in PRUint32 i); */
NS_IMETHODIMP nsXPCTestOut::SetPRUint32(PRUint32 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetPRUint64 (out PRUint64 i); */
NS_IMETHODIMP nsXPCTestOut::GetPRUint64(PRUint64 *i NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetPRUint64 (in PRUint64 i); */
NS_IMETHODIMP nsXPCTestOut::SetPRUint64(PRUint64 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_out_h__ */
