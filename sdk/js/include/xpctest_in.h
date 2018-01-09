/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_in.idl
 */

#ifndef __gen_xpctest_in_h__
#define __gen_xpctest_in_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestIn */
#define NS_IXPCTESTIN_IID_STR "a3cab49d-ae83-4e63-a735-009b9a759204"

#define NS_IXPCTESTIN_IID \
  {0xa3cab49d, 0xae83, 0x4e63, \
    { 0xa7, 0x35, 0x00, 0x9b, 0x9a, 0x75, 0x92, 0x04 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestIn : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTIN_IID)

  /* long EchoLong (in long l); */
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) = 0;

  /* short EchoShort (in short a); */
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 a, PRInt16 *_retval NS_OUTPARAM) = 0;

  /* char EchoChar (in char c); */
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char c, char *_retval NS_OUTPARAM) = 0;

  /* boolean EchoBoolean (in boolean b); */
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool b, PRBool *_retval NS_OUTPARAM) = 0;

  /* octet EchoOctet (in octet o); */
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 o, PRUint8 *_retval NS_OUTPARAM) = 0;

  /* long long EchoLongLong (in long long ll); */
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 ll, PRInt64 *_retval NS_OUTPARAM) = 0;

  /* unsigned short EchoUnsignedShort (in unsigned short us); */
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 us, PRUint16 *_retval NS_OUTPARAM) = 0;

  /* unsigned long EchoUnsignedLong (in unsigned long ul); */
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 ul, PRUint32 *_retval NS_OUTPARAM) = 0;

  /* float EchoFloat (in float f); */
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float f, float *_retval NS_OUTPARAM) = 0;

  /* double EchoDouble (in double d); */
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double d, double *_retval NS_OUTPARAM) = 0;

  /* wchar EchoWchar (in wchar wc); */
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wc, PRUnichar *_retval NS_OUTPARAM) = 0;

  /* wstring EchoString (in wstring ws); */
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *ws, PRUnichar **_retval NS_OUTPARAM) = 0;

  /* PRBool EchoPRBool (in PRBool b); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool b, PRBool *_retval NS_OUTPARAM) = 0;

  /* PRInt32 EchoPRInt32 (in PRInt32 l); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) = 0;

  /* PRInt16 EchoPRInt16 (in PRInt16 l); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 l, PRInt16 *_retval NS_OUTPARAM) = 0;

  /* PRInt64 EchoPRInt64 (in PRInt64 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 i, PRInt64 *_retval NS_OUTPARAM) = 0;

  /* PRUint8 EchoPRUint8 (in PRUint8 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 i, PRUint8 *_retval NS_OUTPARAM) = 0;

  /* PRUint16 EchoPRUint16 (in PRUint16 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 i, PRUint16 *_retval NS_OUTPARAM) = 0;

  /* PRUint32 EchoPRUint32 (in PRUint32 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) = 0;

  /* PRUint32 EchoPRUint32_2 (in PRUint32 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) = 0;

  /* PRUint64 EchoPRUint64 (in PRUint64 i); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 i, PRUint64 *_retval NS_OUTPARAM) = 0;

  /* [noscript] voidPtr EchoVoidPtr (in voidPtr vs); */
  NS_IMETHOD EchoVoidPtr(void *vs, void **_retval NS_OUTPARAM) = 0;

  /* [noscript] charPtr EchoCharPtr (in charPtr cs); */
  NS_IMETHOD EchoCharPtr(char *cs, char **_retval NS_OUTPARAM) = 0;

  /* void EchoVoid (); */
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestIn, NS_IXPCTESTIN_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTIN \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 l, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 a, PRInt16 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char c, char *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool b, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 o, PRUint8 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 ll, PRInt64 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 us, PRUint16 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 ul, PRUint32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float f, float *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double d, double *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wc, PRUnichar *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *ws, PRUnichar **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool b, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 l, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 l, PRInt16 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 i, PRInt64 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 i, PRUint8 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 i, PRUint16 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 i, PRUint32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 i, PRUint32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 i, PRUint64 *_retval NS_OUTPARAM); \
  NS_IMETHOD EchoVoidPtr(void *vs, void **_retval NS_OUTPARAM); \
  NS_IMETHOD EchoCharPtr(char *cs, char **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTIN(_to) \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) { return _to EchoLong(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 a, PRInt16 *_retval NS_OUTPARAM) { return _to EchoShort(a, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char c, char *_retval NS_OUTPARAM) { return _to EchoChar(c, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool b, PRBool *_retval NS_OUTPARAM) { return _to EchoBoolean(b, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 o, PRUint8 *_retval NS_OUTPARAM) { return _to EchoOctet(o, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 ll, PRInt64 *_retval NS_OUTPARAM) { return _to EchoLongLong(ll, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 us, PRUint16 *_retval NS_OUTPARAM) { return _to EchoUnsignedShort(us, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 ul, PRUint32 *_retval NS_OUTPARAM) { return _to EchoUnsignedLong(ul, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float f, float *_retval NS_OUTPARAM) { return _to EchoFloat(f, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double d, double *_retval NS_OUTPARAM) { return _to EchoDouble(d, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wc, PRUnichar *_retval NS_OUTPARAM) { return _to EchoWchar(wc, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *ws, PRUnichar **_retval NS_OUTPARAM) { return _to EchoString(ws, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool b, PRBool *_retval NS_OUTPARAM) { return _to EchoPRBool(b, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) { return _to EchoPRInt32(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 l, PRInt16 *_retval NS_OUTPARAM) { return _to EchoPRInt16(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 i, PRInt64 *_retval NS_OUTPARAM) { return _to EchoPRInt64(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 i, PRUint8 *_retval NS_OUTPARAM) { return _to EchoPRUint8(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 i, PRUint16 *_retval NS_OUTPARAM) { return _to EchoPRUint16(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) { return _to EchoPRUint32(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) { return _to EchoPRUint32_2(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 i, PRUint64 *_retval NS_OUTPARAM) { return _to EchoPRUint64(i, _retval); } \
  NS_IMETHOD EchoVoidPtr(void *vs, void **_retval NS_OUTPARAM) { return _to EchoVoidPtr(vs, _retval); } \
  NS_IMETHOD EchoCharPtr(char *cs, char **_retval NS_OUTPARAM) { return _to EchoCharPtr(cs, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) { return _to EchoVoid(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTIN(_to) \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoLong(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 a, PRInt16 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoShort(a, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char c, char *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoChar(c, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool b, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoBoolean(b, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 o, PRUint8 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoOctet(o, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 ll, PRInt64 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoLongLong(ll, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 us, PRUint16 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoUnsignedShort(us, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 ul, PRUint32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoUnsignedLong(ul, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float f, float *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoFloat(f, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double d, double *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoDouble(d, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wc, PRUnichar *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoWchar(wc, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *ws, PRUnichar **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoString(ws, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool b, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRBool(b, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 l, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt32(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 l, PRInt16 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt16(l, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 i, PRInt64 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt64(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 i, PRUint8 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint8(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 i, PRUint16 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint16(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint32(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 i, PRUint32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint32_2(i, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 i, PRUint64 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint64(i, _retval); } \
  NS_IMETHOD EchoVoidPtr(void *vs, void **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoVoidPtr(vs, _retval); } \
  NS_IMETHOD EchoCharPtr(char *cs, char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoCharPtr(cs, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoVoid(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestIn : public nsIXPCTestIn
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTIN

  nsXPCTestIn();

private:
  ~nsXPCTestIn();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestIn, nsIXPCTestIn)

nsXPCTestIn::nsXPCTestIn()
{
  /* member initializers and constructor code */
}

nsXPCTestIn::~nsXPCTestIn()
{
  /* destructor code */
}

/* long EchoLong (in long l); */
NS_IMETHODIMP nsXPCTestIn::EchoLong(PRInt32 l, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* short EchoShort (in short a); */
NS_IMETHODIMP nsXPCTestIn::EchoShort(PRInt16 a, PRInt16 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* char EchoChar (in char c); */
NS_IMETHODIMP nsXPCTestIn::EchoChar(char c, char *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean EchoBoolean (in boolean b); */
NS_IMETHODIMP nsXPCTestIn::EchoBoolean(PRBool b, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* octet EchoOctet (in octet o); */
NS_IMETHODIMP nsXPCTestIn::EchoOctet(PRUint8 o, PRUint8 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long long EchoLongLong (in long long ll); */
NS_IMETHODIMP nsXPCTestIn::EchoLongLong(PRInt64 ll, PRInt64 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned short EchoUnsignedShort (in unsigned short us); */
NS_IMETHODIMP nsXPCTestIn::EchoUnsignedShort(PRUint16 us, PRUint16 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned long EchoUnsignedLong (in unsigned long ul); */
NS_IMETHODIMP nsXPCTestIn::EchoUnsignedLong(PRUint32 ul, PRUint32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* float EchoFloat (in float f); */
NS_IMETHODIMP nsXPCTestIn::EchoFloat(float f, float *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* double EchoDouble (in double d); */
NS_IMETHODIMP nsXPCTestIn::EchoDouble(double d, double *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* wchar EchoWchar (in wchar wc); */
NS_IMETHODIMP nsXPCTestIn::EchoWchar(PRUnichar wc, PRUnichar *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* wstring EchoString (in wstring ws); */
NS_IMETHODIMP nsXPCTestIn::EchoString(const PRUnichar *ws, PRUnichar **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRBool EchoPRBool (in PRBool b); */
NS_IMETHODIMP nsXPCTestIn::EchoPRBool(PRBool b, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRInt32 EchoPRInt32 (in PRInt32 l); */
NS_IMETHODIMP nsXPCTestIn::EchoPRInt32(PRInt32 l, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRInt16 EchoPRInt16 (in PRInt16 l); */
NS_IMETHODIMP nsXPCTestIn::EchoPRInt16(PRInt16 l, PRInt16 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRInt64 EchoPRInt64 (in PRInt64 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRInt64(PRInt64 i, PRInt64 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint8 EchoPRUint8 (in PRUint8 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRUint8(PRUint8 i, PRUint8 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 EchoPRUint16 (in PRUint16 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRUint16(PRUint16 i, PRUint16 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint32 EchoPRUint32 (in PRUint32 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRUint32(PRUint32 i, PRUint32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint32 EchoPRUint32_2 (in PRUint32 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRUint32_2(PRUint32 i, PRUint32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint64 EchoPRUint64 (in PRUint64 i); */
NS_IMETHODIMP nsXPCTestIn::EchoPRUint64(PRUint64 i, PRUint64 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] voidPtr EchoVoidPtr (in voidPtr vs); */
NS_IMETHODIMP nsXPCTestIn::EchoVoidPtr(void *vs, void **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] charPtr EchoCharPtr (in charPtr cs); */
NS_IMETHODIMP nsXPCTestIn::EchoCharPtr(char *cs, char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoVoid (); */
NS_IMETHODIMP nsXPCTestIn::EchoVoid()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_in_h__ */
