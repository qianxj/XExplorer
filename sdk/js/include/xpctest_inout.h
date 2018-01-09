/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_inout.idl
 */

#ifndef __gen_xpctest_inout_h__
#define __gen_xpctest_inout_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestInOut */
#define NS_IXPCTESTINOUT_IID_STR "626dc196-5599-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTINOUT_IID \
  {0x626dc196, 0x5599, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestInOut : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTINOUT_IID)

  /* void EchoLong (in long li, out long lo); */
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 li, PRInt32 *lo NS_OUTPARAM) = 0;

  /* void EchoShort (in short si, out short so); */
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 si, PRInt16 *so NS_OUTPARAM) = 0;

  /* void EchoChar (in char ci, out char co); */
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char ci, char *co NS_OUTPARAM) = 0;

  /* void EchoBoolean (in boolean bi, out boolean bo); */
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool bi, PRBool *bo NS_OUTPARAM) = 0;

  /* void EchoOctet (in octet oi, out octet oo); */
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 oi, PRUint8 *oo NS_OUTPARAM) = 0;

  /* void EchoLongLong (in long long lli, out long long llo); */
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 lli, PRInt64 *llo NS_OUTPARAM) = 0;

  /* void EchoUnsignedShort (in unsigned short usi, out unsigned short uso); */
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 usi, PRUint16 *uso NS_OUTPARAM) = 0;

  /* void EchoUnsignedLong (in unsigned long uli, out unsigned long ulo); */
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 uli, PRUint32 *ulo NS_OUTPARAM) = 0;

  /* void EchoFloat (in float fi, out float fo); */
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float fi, float *fo NS_OUTPARAM) = 0;

  /* void EchoDouble (in double di, out double dout); */
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double di, double *dout NS_OUTPARAM) = 0;

  /* void EchoWchar (in wchar wci, out wchar wco); */
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wci, PRUnichar *wco NS_OUTPARAM) = 0;

  /* void EchoString (in wstring wsi, out wstring wso); */
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *wsi, PRUnichar **wso NS_OUTPARAM) = 0;

  /* void EchoPRBool (in PRBool bi, out PRBool bo); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool bi, PRBool *bo NS_OUTPARAM) = 0;

  /* void EchoPRInt32 (in PRInt32 li, out PRInt32 lo); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 li, PRInt32 *lo NS_OUTPARAM) = 0;

  /* void EchoPRInt16 (in PRInt16 li, out PRInt16 lo); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 li, PRInt16 *lo NS_OUTPARAM) = 0;

  /* void EchoPRInt64 (in PRInt64 ii, out PRInt64 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 ii, PRInt64 *io NS_OUTPARAM) = 0;

  /* void EchoPRUint8 (in PRUint8 ii, out PRUint8 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 ii, PRUint8 *io NS_OUTPARAM) = 0;

  /* void EchoPRUint16 (in PRUint16 ii, out PRUint16 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 ii, PRUint16 *io NS_OUTPARAM) = 0;

  /* void EchoPRUint32 (in PRUint32 ii, out PRUint32 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 ii, PRUint32 *io NS_OUTPARAM) = 0;

  /* void EchoPRUint32_2 (in PRUint32 ii, out PRUint32 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 ii, PRUint32 *io NS_OUTPARAM) = 0;

  /* void EchoPRUint64 (in PRUint64 ii, out PRUint64 io); */
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 ii, PRUint64 *io NS_OUTPARAM) = 0;

  /* [noscript] void EchoVoidPtr (in voidPtr vsi, out voidPtr vso); */
  NS_IMETHOD EchoVoidPtr(void *vsi, void **vso NS_OUTPARAM) = 0;

  /* [noscript] void EchoCharPtr (in charPtr csi, out charPtr cso); */
  NS_IMETHOD EchoCharPtr(char *csi, char **cso NS_OUTPARAM) = 0;

  /* [noscript] void EchoNsIDPtr (in nsIDPtr pi, out nsIDPtr po); */
  NS_IMETHOD EchoNsIDPtr(const nsID *pi, nsID **po NS_OUTPARAM) = 0;

  /* [noscript] void EchoNsIIDPtr (in nsIIDPtr pi, out nsIIDPtr po); */
  NS_IMETHOD EchoNsIIDPtr(const nsIID *pi, nsIID **po NS_OUTPARAM) = 0;

  /* [noscript] void EchoNsCIDPtr (in nsCIDPtr pi, out nsCIDPtr po); */
  NS_IMETHOD EchoNsCIDPtr(const nsCID *pi, nsCID **po NS_OUTPARAM) = 0;

  /* [noscript] void EchoNsQIResult (in nsQIResult ri, out nsQIResult ro); */
  NS_IMETHOD EchoNsQIResult(void *ri, void **ro NS_OUTPARAM) = 0;

  /* void EchoVoid (); */
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestInOut, NS_IXPCTESTINOUT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTINOUT \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 li, PRInt32 *lo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 si, PRInt16 *so NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char ci, char *co NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool bi, PRBool *bo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 oi, PRUint8 *oo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 lli, PRInt64 *llo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 usi, PRUint16 *uso NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 uli, PRUint32 *ulo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float fi, float *fo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double di, double *dout NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wci, PRUnichar *wco NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *wsi, PRUnichar **wso NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool bi, PRBool *bo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 li, PRInt32 *lo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 li, PRInt16 *lo NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 ii, PRInt64 *io NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 ii, PRUint8 *io NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 ii, PRUint16 *io NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 ii, PRUint32 *io NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 ii, PRUint32 *io NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 ii, PRUint64 *io NS_OUTPARAM); \
  NS_IMETHOD EchoVoidPtr(void *vsi, void **vso NS_OUTPARAM); \
  NS_IMETHOD EchoCharPtr(char *csi, char **cso NS_OUTPARAM); \
  NS_IMETHOD EchoNsIDPtr(const nsID *pi, nsID **po NS_OUTPARAM); \
  NS_IMETHOD EchoNsIIDPtr(const nsIID *pi, nsIID **po NS_OUTPARAM); \
  NS_IMETHOD EchoNsCIDPtr(const nsCID *pi, nsCID **po NS_OUTPARAM); \
  NS_IMETHOD EchoNsQIResult(void *ri, void **ro NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTINOUT(_to) \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 li, PRInt32 *lo NS_OUTPARAM) { return _to EchoLong(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 si, PRInt16 *so NS_OUTPARAM) { return _to EchoShort(si, so); } \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char ci, char *co NS_OUTPARAM) { return _to EchoChar(ci, co); } \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool bi, PRBool *bo NS_OUTPARAM) { return _to EchoBoolean(bi, bo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 oi, PRUint8 *oo NS_OUTPARAM) { return _to EchoOctet(oi, oo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 lli, PRInt64 *llo NS_OUTPARAM) { return _to EchoLongLong(lli, llo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 usi, PRUint16 *uso NS_OUTPARAM) { return _to EchoUnsignedShort(usi, uso); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 uli, PRUint32 *ulo NS_OUTPARAM) { return _to EchoUnsignedLong(uli, ulo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float fi, float *fo NS_OUTPARAM) { return _to EchoFloat(fi, fo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double di, double *dout NS_OUTPARAM) { return _to EchoDouble(di, dout); } \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wci, PRUnichar *wco NS_OUTPARAM) { return _to EchoWchar(wci, wco); } \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *wsi, PRUnichar **wso NS_OUTPARAM) { return _to EchoString(wsi, wso); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool bi, PRBool *bo NS_OUTPARAM) { return _to EchoPRBool(bi, bo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 li, PRInt32 *lo NS_OUTPARAM) { return _to EchoPRInt32(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 li, PRInt16 *lo NS_OUTPARAM) { return _to EchoPRInt16(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 ii, PRInt64 *io NS_OUTPARAM) { return _to EchoPRInt64(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 ii, PRUint8 *io NS_OUTPARAM) { return _to EchoPRUint8(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 ii, PRUint16 *io NS_OUTPARAM) { return _to EchoPRUint16(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 ii, PRUint32 *io NS_OUTPARAM) { return _to EchoPRUint32(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 ii, PRUint32 *io NS_OUTPARAM) { return _to EchoPRUint32_2(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 ii, PRUint64 *io NS_OUTPARAM) { return _to EchoPRUint64(ii, io); } \
  NS_IMETHOD EchoVoidPtr(void *vsi, void **vso NS_OUTPARAM) { return _to EchoVoidPtr(vsi, vso); } \
  NS_IMETHOD EchoCharPtr(char *csi, char **cso NS_OUTPARAM) { return _to EchoCharPtr(csi, cso); } \
  NS_IMETHOD EchoNsIDPtr(const nsID *pi, nsID **po NS_OUTPARAM) { return _to EchoNsIDPtr(pi, po); } \
  NS_IMETHOD EchoNsIIDPtr(const nsIID *pi, nsIID **po NS_OUTPARAM) { return _to EchoNsIIDPtr(pi, po); } \
  NS_IMETHOD EchoNsCIDPtr(const nsCID *pi, nsCID **po NS_OUTPARAM) { return _to EchoNsCIDPtr(pi, po); } \
  NS_IMETHOD EchoNsQIResult(void *ri, void **ro NS_OUTPARAM) { return _to EchoNsQIResult(ri, ro); } \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) { return _to EchoVoid(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTINOUT(_to) \
  NS_SCRIPTABLE NS_IMETHOD EchoLong(PRInt32 li, PRInt32 *lo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoLong(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoShort(PRInt16 si, PRInt16 *so NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoShort(si, so); } \
  NS_SCRIPTABLE NS_IMETHOD EchoChar(char ci, char *co NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoChar(ci, co); } \
  NS_SCRIPTABLE NS_IMETHOD EchoBoolean(PRBool bi, PRBool *bo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoBoolean(bi, bo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoOctet(PRUint8 oi, PRUint8 *oo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoOctet(oi, oo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoLongLong(PRInt64 lli, PRInt64 *llo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoLongLong(lli, llo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedShort(PRUint16 usi, PRUint16 *uso NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoUnsignedShort(usi, uso); } \
  NS_SCRIPTABLE NS_IMETHOD EchoUnsignedLong(PRUint32 uli, PRUint32 *ulo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoUnsignedLong(uli, ulo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoFloat(float fi, float *fo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoFloat(fi, fo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoDouble(double di, double *dout NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoDouble(di, dout); } \
  NS_SCRIPTABLE NS_IMETHOD EchoWchar(PRUnichar wci, PRUnichar *wco NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoWchar(wci, wco); } \
  NS_SCRIPTABLE NS_IMETHOD EchoString(const PRUnichar *wsi, PRUnichar **wso NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoString(wsi, wso); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRBool(PRBool bi, PRBool *bo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRBool(bi, bo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt32(PRInt32 li, PRInt32 *lo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt32(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt16(PRInt16 li, PRInt16 *lo NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt16(li, lo); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRInt64(PRInt64 ii, PRInt64 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRInt64(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint8(PRUint8 ii, PRUint8 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint8(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint16(PRUint16 ii, PRUint16 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint16(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32(PRUint32 ii, PRUint32 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint32(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint32_2(PRUint32 ii, PRUint32 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint32_2(ii, io); } \
  NS_SCRIPTABLE NS_IMETHOD EchoPRUint64(PRUint64 ii, PRUint64 *io NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoPRUint64(ii, io); } \
  NS_IMETHOD EchoVoidPtr(void *vsi, void **vso NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoVoidPtr(vsi, vso); } \
  NS_IMETHOD EchoCharPtr(char *csi, char **cso NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoCharPtr(csi, cso); } \
  NS_IMETHOD EchoNsIDPtr(const nsID *pi, nsID **po NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoNsIDPtr(pi, po); } \
  NS_IMETHOD EchoNsIIDPtr(const nsIID *pi, nsIID **po NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoNsIIDPtr(pi, po); } \
  NS_IMETHOD EchoNsCIDPtr(const nsCID *pi, nsCID **po NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoNsCIDPtr(pi, po); } \
  NS_IMETHOD EchoNsQIResult(void *ri, void **ro NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoNsQIResult(ri, ro); } \
  NS_SCRIPTABLE NS_IMETHOD EchoVoid(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoVoid(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestInOut : public nsIXPCTestInOut
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTINOUT

  nsXPCTestInOut();

private:
  ~nsXPCTestInOut();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestInOut, nsIXPCTestInOut)

nsXPCTestInOut::nsXPCTestInOut()
{
  /* member initializers and constructor code */
}

nsXPCTestInOut::~nsXPCTestInOut()
{
  /* destructor code */
}

/* void EchoLong (in long li, out long lo); */
NS_IMETHODIMP nsXPCTestInOut::EchoLong(PRInt32 li, PRInt32 *lo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoShort (in short si, out short so); */
NS_IMETHODIMP nsXPCTestInOut::EchoShort(PRInt16 si, PRInt16 *so NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoChar (in char ci, out char co); */
NS_IMETHODIMP nsXPCTestInOut::EchoChar(char ci, char *co NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoBoolean (in boolean bi, out boolean bo); */
NS_IMETHODIMP nsXPCTestInOut::EchoBoolean(PRBool bi, PRBool *bo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoOctet (in octet oi, out octet oo); */
NS_IMETHODIMP nsXPCTestInOut::EchoOctet(PRUint8 oi, PRUint8 *oo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoLongLong (in long long lli, out long long llo); */
NS_IMETHODIMP nsXPCTestInOut::EchoLongLong(PRInt64 lli, PRInt64 *llo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoUnsignedShort (in unsigned short usi, out unsigned short uso); */
NS_IMETHODIMP nsXPCTestInOut::EchoUnsignedShort(PRUint16 usi, PRUint16 *uso NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoUnsignedLong (in unsigned long uli, out unsigned long ulo); */
NS_IMETHODIMP nsXPCTestInOut::EchoUnsignedLong(PRUint32 uli, PRUint32 *ulo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoFloat (in float fi, out float fo); */
NS_IMETHODIMP nsXPCTestInOut::EchoFloat(float fi, float *fo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoDouble (in double di, out double dout); */
NS_IMETHODIMP nsXPCTestInOut::EchoDouble(double di, double *dout NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoWchar (in wchar wci, out wchar wco); */
NS_IMETHODIMP nsXPCTestInOut::EchoWchar(PRUnichar wci, PRUnichar *wco NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoString (in wstring wsi, out wstring wso); */
NS_IMETHODIMP nsXPCTestInOut::EchoString(const PRUnichar *wsi, PRUnichar **wso NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRBool (in PRBool bi, out PRBool bo); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRBool(PRBool bi, PRBool *bo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRInt32 (in PRInt32 li, out PRInt32 lo); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRInt32(PRInt32 li, PRInt32 *lo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRInt16 (in PRInt16 li, out PRInt16 lo); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRInt16(PRInt16 li, PRInt16 *lo NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRInt64 (in PRInt64 ii, out PRInt64 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRInt64(PRInt64 ii, PRInt64 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRUint8 (in PRUint8 ii, out PRUint8 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRUint8(PRUint8 ii, PRUint8 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRUint16 (in PRUint16 ii, out PRUint16 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRUint16(PRUint16 ii, PRUint16 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRUint32 (in PRUint32 ii, out PRUint32 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRUint32(PRUint32 ii, PRUint32 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRUint32_2 (in PRUint32 ii, out PRUint32 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRUint32_2(PRUint32 ii, PRUint32 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoPRUint64 (in PRUint64 ii, out PRUint64 io); */
NS_IMETHODIMP nsXPCTestInOut::EchoPRUint64(PRUint64 ii, PRUint64 *io NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoVoidPtr (in voidPtr vsi, out voidPtr vso); */
NS_IMETHODIMP nsXPCTestInOut::EchoVoidPtr(void *vsi, void **vso NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoCharPtr (in charPtr csi, out charPtr cso); */
NS_IMETHODIMP nsXPCTestInOut::EchoCharPtr(char *csi, char **cso NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoNsIDPtr (in nsIDPtr pi, out nsIDPtr po); */
NS_IMETHODIMP nsXPCTestInOut::EchoNsIDPtr(const nsID *pi, nsID **po NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoNsIIDPtr (in nsIIDPtr pi, out nsIIDPtr po); */
NS_IMETHODIMP nsXPCTestInOut::EchoNsIIDPtr(const nsIID *pi, nsIID **po NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoNsCIDPtr (in nsCIDPtr pi, out nsCIDPtr po); */
NS_IMETHODIMP nsXPCTestInOut::EchoNsCIDPtr(const nsCID *pi, nsCID **po NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void EchoNsQIResult (in nsQIResult ri, out nsQIResult ro); */
NS_IMETHODIMP nsXPCTestInOut::EchoNsQIResult(void *ri, void **ro NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EchoVoid (); */
NS_IMETHODIMP nsXPCTestInOut::EchoVoid()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_inout_h__ */
