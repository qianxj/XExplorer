/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest.idl
 */

#ifndef __gen_xpctest_h__
#define __gen_xpctest_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_xpcexception_h__
#include "xpcexception.h"
#endif

#ifndef __gen_nsIVariant_h__
#include "nsIVariant.h"
#endif

#ifndef __gen_nsIPropertyBag_h__
#include "nsIPropertyBag.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsITestXPCSomeUselessThing; /* forward declaration */


/* starting interface:    nsITestXPCFunctionCallback */
#define NS_ITESTXPCFUNCTIONCALLBACK_IID_STR "4407fc90-1e41-11d5-909d-0010a4e73d9a"

#define NS_ITESTXPCFUNCTIONCALLBACK_IID \
  {0x4407fc90, 0x1e41, 0x11d5, \
    { 0x90, 0x9d, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsITestXPCFunctionCallback : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITESTXPCFUNCTIONCALLBACK_IID)

  /* void call (in string s); */
  NS_SCRIPTABLE NS_IMETHOD Call(const char *s) = 0;

  /* void callWithThis (in nsISupports self, in string s); */
  NS_SCRIPTABLE NS_IMETHOD CallWithThis(nsISupports *self, const char *s) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITestXPCFunctionCallback, NS_ITESTXPCFUNCTIONCALLBACK_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITESTXPCFUNCTIONCALLBACK \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *s); \
  NS_SCRIPTABLE NS_IMETHOD CallWithThis(nsISupports *self, const char *s); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITESTXPCFUNCTIONCALLBACK(_to) \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *s) { return _to Call(s); } \
  NS_SCRIPTABLE NS_IMETHOD CallWithThis(nsISupports *self, const char *s) { return _to CallWithThis(self, s); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITESTXPCFUNCTIONCALLBACK(_to) \
  NS_SCRIPTABLE NS_IMETHOD Call(const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->Call(s); } \
  NS_SCRIPTABLE NS_IMETHOD CallWithThis(nsISupports *self, const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallWithThis(self, s); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTestXPCFunctionCallback : public nsITestXPCFunctionCallback
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITESTXPCFUNCTIONCALLBACK

  nsTestXPCFunctionCallback();

private:
  ~nsTestXPCFunctionCallback();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTestXPCFunctionCallback, nsITestXPCFunctionCallback)

nsTestXPCFunctionCallback::nsTestXPCFunctionCallback()
{
  /* member initializers and constructor code */
}

nsTestXPCFunctionCallback::~nsTestXPCFunctionCallback()
{
  /* destructor code */
}

/* void call (in string s); */
NS_IMETHODIMP nsTestXPCFunctionCallback::Call(const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void callWithThis (in nsISupports self, in string s); */
NS_IMETHODIMP nsTestXPCFunctionCallback::CallWithThis(nsISupports *self, const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsITestXPCFoo */
#define NS_ITESTXPCFOO_IID_STR "159e36d0-991e-11d2-ac3f-00c09300144b"

#define NS_ITESTXPCFOO_IID \
  {0x159e36d0, 0x991e, 0x11d2, \
    { 0xac, 0x3f, 0x00, 0xc0, 0x93, 0x00, 0x14, 0x4b }}

class NS_NO_VTABLE NS_SCRIPTABLE nsITestXPCFoo : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITESTXPCFOO_IID)

  /* long Test (in long p1, in long p2); */
  NS_SCRIPTABLE NS_IMETHOD Test(PRInt32 p1, PRInt32 p2, PRInt32 *_retval NS_OUTPARAM) = 0;

  /* void Test2 (); */
  NS_SCRIPTABLE NS_IMETHOD Test2(void) = 0;

  /* attribute string Foo; */
  NS_SCRIPTABLE NS_IMETHOD GetFoo(char **aFoo) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetFoo(const char *aFoo) = 0;

  enum { one = 1 };

  enum { five = 5 };

  enum { six = 6 };

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITestXPCFoo, NS_ITESTXPCFOO_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITESTXPCFOO \
  NS_SCRIPTABLE NS_IMETHOD Test(PRInt32 p1, PRInt32 p2, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Test2(void); \
  NS_SCRIPTABLE NS_IMETHOD GetFoo(char **aFoo); \
  NS_SCRIPTABLE NS_IMETHOD SetFoo(const char *aFoo); \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITESTXPCFOO(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(PRInt32 p1, PRInt32 p2, PRInt32 *_retval NS_OUTPARAM) { return _to Test(p1, p2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Test2(void) { return _to Test2(); } \
  NS_SCRIPTABLE NS_IMETHOD GetFoo(char **aFoo) { return _to GetFoo(aFoo); } \
  NS_SCRIPTABLE NS_IMETHOD SetFoo(const char *aFoo) { return _to SetFoo(aFoo); } \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITESTXPCFOO(_to) \
  NS_SCRIPTABLE NS_IMETHOD Test(PRInt32 p1, PRInt32 p2, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(p1, p2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Test2(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test2(); } \
  NS_SCRIPTABLE NS_IMETHOD GetFoo(char **aFoo) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFoo(aFoo); } \
  NS_SCRIPTABLE NS_IMETHOD SetFoo(const char *aFoo) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFoo(aFoo); } \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTestXPCFoo : public nsITestXPCFoo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITESTXPCFOO

  nsTestXPCFoo();

private:
  ~nsTestXPCFoo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTestXPCFoo, nsITestXPCFoo)

nsTestXPCFoo::nsTestXPCFoo()
{
  /* member initializers and constructor code */
}

nsTestXPCFoo::~nsTestXPCFoo()
{
  /* destructor code */
}

/* long Test (in long p1, in long p2); */
NS_IMETHODIMP nsTestXPCFoo::Test(PRInt32 p1, PRInt32 p2, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Test2 (); */
NS_IMETHODIMP nsTestXPCFoo::Test2()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute string Foo; */
NS_IMETHODIMP nsTestXPCFoo::GetFoo(char **aFoo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsTestXPCFoo::SetFoo(const char *aFoo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsITestXPCFoo2 */
#define NS_ITESTXPCFOO2_IID_STR "5f9d20c0-9b6b-11d2-9ffe-000064657374"

#define NS_ITESTXPCFOO2_IID \
  {0x5f9d20c0, 0x9b6b, 0x11d2, \
    { 0x9f, 0xfe, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsITestXPCFoo2 : public nsITestXPCFoo {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITESTXPCFOO2_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITestXPCFoo2, NS_ITESTXPCFOO2_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITESTXPCFOO2 \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITESTXPCFOO2(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITESTXPCFOO2(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTestXPCFoo2 : public nsITestXPCFoo2
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITESTXPCFOO2

  nsTestXPCFoo2();

private:
  ~nsTestXPCFoo2();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTestXPCFoo2, nsITestXPCFoo2)

nsTestXPCFoo2::nsTestXPCFoo2()
{
  /* member initializers and constructor code */
}

nsTestXPCFoo2::~nsTestXPCFoo2()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIEcho */
#define NS_IECHO_IID_STR "cd2f2f40-c5d9-11d2-9838-006008962422"

#define NS_IECHO_IID \
  {0xcd2f2f40, 0xc5d9, 0x11d2, \
    { 0x98, 0x38, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIEcho : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IECHO_IID)

  /* void SetReceiver (in nsIEcho aReceiver); */
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIEcho *aReceiver) = 0;

  /* void SendOneString (in string str); */
  NS_SCRIPTABLE NS_IMETHOD SendOneString(const char *str) = 0;

  /* long In2OutOneInt (in long input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneInt(PRInt32 input, PRInt32 *_retval NS_OUTPARAM) = 0;

  /* long In2OutAddTwoInts (in long input1, in long input2, out long output1, out long output2); */
  NS_SCRIPTABLE NS_IMETHOD In2OutAddTwoInts(PRInt32 input1, PRInt32 input2, PRInt32 *output1 NS_OUTPARAM, PRInt32 *output2 NS_OUTPARAM, PRInt32 *_retval NS_OUTPARAM) = 0;

  /* string In2OutOneString (in string input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneString(const char *input, char **_retval NS_OUTPARAM) = 0;

  /* DOMString In2OutOneDOMString (in DOMString input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) = 0;

  /* DOMString EchoIn2OutOneDOMString (in DOMString input); */
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) = 0;

  /* AString In2OutOneAString (in AString input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) = 0;

  /* AString EchoIn2OutOneAString (in AString input); */
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) = 0;

  /* AUTF8String In2OutOneUTF8String (in AUTF8String input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) = 0;

  /* AUTF8String EchoIn2OutOneUTF8String (in AUTF8String input); */
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) = 0;

  /* ACString In2OutOneCString (in ACString input); */
  NS_SCRIPTABLE NS_IMETHOD In2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) = 0;

  /* ACString EchoIn2OutOneCString (in ACString input); */
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) = 0;

  /* void SimpleCallNoEcho (); */
  NS_SCRIPTABLE NS_IMETHOD SimpleCallNoEcho(void) = 0;

  /* void SendManyTypes (in octet p1, in short p2, in long p3, in long long p4, in octet p5, in unsigned short p6, in unsigned long p7, in unsigned long long p8, in float p9, in double p10, in boolean p11, in char p12, in wchar p13, in nsIDPtr p14, in string p15, in wstring p16); */
  NS_SCRIPTABLE NS_IMETHOD SendManyTypes(PRUint8 p1, PRInt16 p2, PRInt32 p3, PRInt64 p4, PRUint8 p5, PRUint16 p6, PRUint32 p7, PRUint64 p8, float p9, double p10, PRBool p11, char p12, PRUnichar p13, const nsID *p14, const char *p15, const PRUnichar *p16) = 0;

  /* void SendInOutManyTypes (inout octet p1, inout short p2, inout long p3, inout long long p4, inout octet p5, inout unsigned short p6, inout unsigned long p7, inout unsigned long long p8, inout float p9, inout double p10, inout boolean p11, inout char p12, inout wchar p13, inout nsIDPtr p14, inout string p15, inout wstring p16); */
  NS_SCRIPTABLE NS_IMETHOD SendInOutManyTypes(PRUint8 *p1 NS_INOUTPARAM, PRInt16 *p2 NS_INOUTPARAM, PRInt32 *p3 NS_INOUTPARAM, PRInt64 *p4 NS_INOUTPARAM, PRUint8 *p5 NS_INOUTPARAM, PRUint16 *p6 NS_INOUTPARAM, PRUint32 *p7 NS_INOUTPARAM, PRUint64 *p8 NS_INOUTPARAM, float *p9 NS_INOUTPARAM, double *p10 NS_INOUTPARAM, PRBool *p11 NS_INOUTPARAM, char *p12 NS_INOUTPARAM, PRUnichar *p13 NS_INOUTPARAM, nsID **p14 NS_INOUTPARAM, char **p15 NS_INOUTPARAM, PRUnichar **p16 NS_INOUTPARAM) = 0;

  /* [noscript] void MethodWithNative (in long p1, in voidPtr p2); */
  NS_IMETHOD MethodWithNative(PRInt32 p1, void *p2) = 0;

  /* void ReturnCode (in long code); */
  NS_SCRIPTABLE NS_IMETHOD ReturnCode(PRInt32 code) = 0;

  /* void FailInJSTest (in long fail); */
  NS_SCRIPTABLE NS_IMETHOD FailInJSTest(PRInt32 fail) = 0;

  /* void SharedString ([shared, retval] out string str); */
  NS_SCRIPTABLE NS_IMETHOD SharedString(const char **str NS_OUTPARAM) = 0;

  /* void ReturnCode_NS_OK (); */
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_OK(void) = 0;

  /* void ReturnCode_NS_ERROR_NULL_POINTER (); */
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_NULL_POINTER(void) = 0;

  /* void ReturnCode_NS_ERROR_UNEXPECTED (); */
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_UNEXPECTED(void) = 0;

  /* void ReturnCode_NS_ERROR_OUT_OF_MEMORY (); */
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_OUT_OF_MEMORY(void) = 0;

  /* nsISupports ReturnInterface (in nsISupports obj); */
  NS_SCRIPTABLE NS_IMETHOD ReturnInterface(nsISupports *obj, nsISupports **_retval NS_OUTPARAM) = 0;

  /* nsIStackFrame GetStack (); */
  NS_SCRIPTABLE NS_IMETHOD GetStack(nsIStackFrame **_retval NS_OUTPARAM) = 0;

  /* void SetReceiverReturnOldReceiver (inout nsIEcho aReceiver); */
  NS_SCRIPTABLE NS_IMETHOD SetReceiverReturnOldReceiver(nsIEcho **aReceiver NS_INOUTPARAM) = 0;

  /* void MethodWithForwardDeclaredParam (in nsITestXPCSomeUselessThing sut); */
  NS_SCRIPTABLE NS_IMETHOD MethodWithForwardDeclaredParam(nsITestXPCSomeUselessThing *sut) = 0;

  /* void PseudoQueryInterface (in nsIIDRef uuid, [iid_is (uuid), retval] out nsQIResult result); */
  NS_SCRIPTABLE NS_IMETHOD PseudoQueryInterface(const nsIID & uuid, void **result NS_OUTPARAM) = 0;

  /* void DebugDumpJSStack (); */
  NS_SCRIPTABLE NS_IMETHOD DebugDumpJSStack(void) = 0;

  /* void printArgTypes (); */
  NS_SCRIPTABLE NS_IMETHOD PrintArgTypes(void) = 0;

  /* void throwArg (); */
  NS_SCRIPTABLE NS_IMETHOD ThrowArg(void) = 0;

  /* void callReceiverSometimeLater (); */
  NS_SCRIPTABLE NS_IMETHOD CallReceiverSometimeLater(void) = 0;

  /* readonly attribute short throwInGetter; */
  NS_SCRIPTABLE NS_IMETHOD GetThrowInGetter(PRInt16 *aThrowInGetter) = 0;

  /* attribute string aString; */
  NS_SCRIPTABLE NS_IMETHOD GetAString(char **aAString) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetAString(const char *aAString) = 0;

  /* attribute PRInt32 SomeValue; */
  NS_SCRIPTABLE NS_IMETHOD GetSomeValue(PRInt32 *aSomeValue) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetSomeValue(PRInt32 aSomeValue) = 0;

  enum { one = 1 };

  enum { five = 5 };

  enum { six = 6 };

  enum { medium = 12345U };

  enum { big = 4294967295U };

  /* void callFunction (in nsITestXPCFunctionCallback callback, in string s); */
  NS_SCRIPTABLE NS_IMETHOD CallFunction(nsITestXPCFunctionCallback *callback, const char *s) = 0;

  /* void callFunctionWithThis (in nsITestXPCFunctionCallback callback, in nsISupports self, in string s); */
  NS_SCRIPTABLE NS_IMETHOD CallFunctionWithThis(nsITestXPCFunctionCallback *callback, nsISupports *self, const char *s) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIEcho, NS_IECHO_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIECHO \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIEcho *aReceiver); \
  NS_SCRIPTABLE NS_IMETHOD SendOneString(const char *str); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneInt(PRInt32 input, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutAddTwoInts(PRInt32 input1, PRInt32 input2, PRInt32 *output1 NS_OUTPARAM, PRInt32 *output2 NS_OUTPARAM, PRInt32 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneString(const char *input, char **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SimpleCallNoEcho(void); \
  NS_SCRIPTABLE NS_IMETHOD SendManyTypes(PRUint8 p1, PRInt16 p2, PRInt32 p3, PRInt64 p4, PRUint8 p5, PRUint16 p6, PRUint32 p7, PRUint64 p8, float p9, double p10, PRBool p11, char p12, PRUnichar p13, const nsID *p14, const char *p15, const PRUnichar *p16); \
  NS_SCRIPTABLE NS_IMETHOD SendInOutManyTypes(PRUint8 *p1 NS_INOUTPARAM, PRInt16 *p2 NS_INOUTPARAM, PRInt32 *p3 NS_INOUTPARAM, PRInt64 *p4 NS_INOUTPARAM, PRUint8 *p5 NS_INOUTPARAM, PRUint16 *p6 NS_INOUTPARAM, PRUint32 *p7 NS_INOUTPARAM, PRUint64 *p8 NS_INOUTPARAM, float *p9 NS_INOUTPARAM, double *p10 NS_INOUTPARAM, PRBool *p11 NS_INOUTPARAM, char *p12 NS_INOUTPARAM, PRUnichar *p13 NS_INOUTPARAM, nsID **p14 NS_INOUTPARAM, char **p15 NS_INOUTPARAM, PRUnichar **p16 NS_INOUTPARAM); \
  NS_IMETHOD MethodWithNative(PRInt32 p1, void *p2); \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode(PRInt32 code); \
  NS_SCRIPTABLE NS_IMETHOD FailInJSTest(PRInt32 fail); \
  NS_SCRIPTABLE NS_IMETHOD SharedString(const char **str NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_OK(void); \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_NULL_POINTER(void); \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_UNEXPECTED(void); \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_OUT_OF_MEMORY(void); \
  NS_SCRIPTABLE NS_IMETHOD ReturnInterface(nsISupports *obj, nsISupports **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetStack(nsIStackFrame **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD SetReceiverReturnOldReceiver(nsIEcho **aReceiver NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD MethodWithForwardDeclaredParam(nsITestXPCSomeUselessThing *sut); \
  NS_SCRIPTABLE NS_IMETHOD PseudoQueryInterface(const nsIID & uuid, void **result NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD DebugDumpJSStack(void); \
  NS_SCRIPTABLE NS_IMETHOD PrintArgTypes(void); \
  NS_SCRIPTABLE NS_IMETHOD ThrowArg(void); \
  NS_SCRIPTABLE NS_IMETHOD CallReceiverSometimeLater(void); \
  NS_SCRIPTABLE NS_IMETHOD GetThrowInGetter(PRInt16 *aThrowInGetter); \
  NS_SCRIPTABLE NS_IMETHOD GetAString(char **aAString); \
  NS_SCRIPTABLE NS_IMETHOD SetAString(const char *aAString); \
  NS_SCRIPTABLE NS_IMETHOD GetSomeValue(PRInt32 *aSomeValue); \
  NS_SCRIPTABLE NS_IMETHOD SetSomeValue(PRInt32 aSomeValue); \
  NS_SCRIPTABLE NS_IMETHOD CallFunction(nsITestXPCFunctionCallback *callback, const char *s); \
  NS_SCRIPTABLE NS_IMETHOD CallFunctionWithThis(nsITestXPCFunctionCallback *callback, nsISupports *self, const char *s); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIECHO(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIEcho *aReceiver) { return _to SetReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD SendOneString(const char *str) { return _to SendOneString(str); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneInt(PRInt32 input, PRInt32 *_retval NS_OUTPARAM) { return _to In2OutOneInt(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutAddTwoInts(PRInt32 input1, PRInt32 input2, PRInt32 *output1 NS_OUTPARAM, PRInt32 *output2 NS_OUTPARAM, PRInt32 *_retval NS_OUTPARAM) { return _to In2OutAddTwoInts(input1, input2, output1, output2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneString(const char *input, char **_retval NS_OUTPARAM) { return _to In2OutOneString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return _to In2OutOneDOMString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return _to EchoIn2OutOneDOMString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return _to In2OutOneAString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return _to EchoIn2OutOneAString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return _to In2OutOneUTF8String(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return _to EchoIn2OutOneUTF8String(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return _to In2OutOneCString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return _to EchoIn2OutOneCString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SimpleCallNoEcho(void) { return _to SimpleCallNoEcho(); } \
  NS_SCRIPTABLE NS_IMETHOD SendManyTypes(PRUint8 p1, PRInt16 p2, PRInt32 p3, PRInt64 p4, PRUint8 p5, PRUint16 p6, PRUint32 p7, PRUint64 p8, float p9, double p10, PRBool p11, char p12, PRUnichar p13, const nsID *p14, const char *p15, const PRUnichar *p16) { return _to SendManyTypes(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); } \
  NS_SCRIPTABLE NS_IMETHOD SendInOutManyTypes(PRUint8 *p1 NS_INOUTPARAM, PRInt16 *p2 NS_INOUTPARAM, PRInt32 *p3 NS_INOUTPARAM, PRInt64 *p4 NS_INOUTPARAM, PRUint8 *p5 NS_INOUTPARAM, PRUint16 *p6 NS_INOUTPARAM, PRUint32 *p7 NS_INOUTPARAM, PRUint64 *p8 NS_INOUTPARAM, float *p9 NS_INOUTPARAM, double *p10 NS_INOUTPARAM, PRBool *p11 NS_INOUTPARAM, char *p12 NS_INOUTPARAM, PRUnichar *p13 NS_INOUTPARAM, nsID **p14 NS_INOUTPARAM, char **p15 NS_INOUTPARAM, PRUnichar **p16 NS_INOUTPARAM) { return _to SendInOutManyTypes(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); } \
  NS_IMETHOD MethodWithNative(PRInt32 p1, void *p2) { return _to MethodWithNative(p1, p2); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode(PRInt32 code) { return _to ReturnCode(code); } \
  NS_SCRIPTABLE NS_IMETHOD FailInJSTest(PRInt32 fail) { return _to FailInJSTest(fail); } \
  NS_SCRIPTABLE NS_IMETHOD SharedString(const char **str NS_OUTPARAM) { return _to SharedString(str); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_OK(void) { return _to ReturnCode_NS_OK(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_NULL_POINTER(void) { return _to ReturnCode_NS_ERROR_NULL_POINTER(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_UNEXPECTED(void) { return _to ReturnCode_NS_ERROR_UNEXPECTED(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_OUT_OF_MEMORY(void) { return _to ReturnCode_NS_ERROR_OUT_OF_MEMORY(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnInterface(nsISupports *obj, nsISupports **_retval NS_OUTPARAM) { return _to ReturnInterface(obj, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStack(nsIStackFrame **_retval NS_OUTPARAM) { return _to GetStack(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetReceiverReturnOldReceiver(nsIEcho **aReceiver NS_INOUTPARAM) { return _to SetReceiverReturnOldReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD MethodWithForwardDeclaredParam(nsITestXPCSomeUselessThing *sut) { return _to MethodWithForwardDeclaredParam(sut); } \
  NS_SCRIPTABLE NS_IMETHOD PseudoQueryInterface(const nsIID & uuid, void **result NS_OUTPARAM) { return _to PseudoQueryInterface(uuid, result); } \
  NS_SCRIPTABLE NS_IMETHOD DebugDumpJSStack(void) { return _to DebugDumpJSStack(); } \
  NS_SCRIPTABLE NS_IMETHOD PrintArgTypes(void) { return _to PrintArgTypes(); } \
  NS_SCRIPTABLE NS_IMETHOD ThrowArg(void) { return _to ThrowArg(); } \
  NS_SCRIPTABLE NS_IMETHOD CallReceiverSometimeLater(void) { return _to CallReceiverSometimeLater(); } \
  NS_SCRIPTABLE NS_IMETHOD GetThrowInGetter(PRInt16 *aThrowInGetter) { return _to GetThrowInGetter(aThrowInGetter); } \
  NS_SCRIPTABLE NS_IMETHOD GetAString(char **aAString) { return _to GetAString(aAString); } \
  NS_SCRIPTABLE NS_IMETHOD SetAString(const char *aAString) { return _to SetAString(aAString); } \
  NS_SCRIPTABLE NS_IMETHOD GetSomeValue(PRInt32 *aSomeValue) { return _to GetSomeValue(aSomeValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetSomeValue(PRInt32 aSomeValue) { return _to SetSomeValue(aSomeValue); } \
  NS_SCRIPTABLE NS_IMETHOD CallFunction(nsITestXPCFunctionCallback *callback, const char *s) { return _to CallFunction(callback, s); } \
  NS_SCRIPTABLE NS_IMETHOD CallFunctionWithThis(nsITestXPCFunctionCallback *callback, nsISupports *self, const char *s) { return _to CallFunctionWithThis(callback, self, s); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIECHO(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIEcho *aReceiver) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD SendOneString(const char *str) { return !_to ? NS_ERROR_NULL_POINTER : _to->SendOneString(str); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneInt(PRInt32 input, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneInt(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutAddTwoInts(PRInt32 input1, PRInt32 input2, PRInt32 *output1 NS_OUTPARAM, PRInt32 *output2 NS_OUTPARAM, PRInt32 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutAddTwoInts(input1, input2, output1, output2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneString(const char *input, char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneDOMString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoIn2OutOneDOMString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneAString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoIn2OutOneAString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneUTF8String(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoIn2OutOneUTF8String(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD In2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->In2OutOneCString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD EchoIn2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->EchoIn2OutOneCString(input, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD SimpleCallNoEcho(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->SimpleCallNoEcho(); } \
  NS_SCRIPTABLE NS_IMETHOD SendManyTypes(PRUint8 p1, PRInt16 p2, PRInt32 p3, PRInt64 p4, PRUint8 p5, PRUint16 p6, PRUint32 p7, PRUint64 p8, float p9, double p10, PRBool p11, char p12, PRUnichar p13, const nsID *p14, const char *p15, const PRUnichar *p16) { return !_to ? NS_ERROR_NULL_POINTER : _to->SendManyTypes(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); } \
  NS_SCRIPTABLE NS_IMETHOD SendInOutManyTypes(PRUint8 *p1 NS_INOUTPARAM, PRInt16 *p2 NS_INOUTPARAM, PRInt32 *p3 NS_INOUTPARAM, PRInt64 *p4 NS_INOUTPARAM, PRUint8 *p5 NS_INOUTPARAM, PRUint16 *p6 NS_INOUTPARAM, PRUint32 *p7 NS_INOUTPARAM, PRUint64 *p8 NS_INOUTPARAM, float *p9 NS_INOUTPARAM, double *p10 NS_INOUTPARAM, PRBool *p11 NS_INOUTPARAM, char *p12 NS_INOUTPARAM, PRUnichar *p13 NS_INOUTPARAM, nsID **p14 NS_INOUTPARAM, char **p15 NS_INOUTPARAM, PRUnichar **p16 NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->SendInOutManyTypes(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); } \
  NS_IMETHOD MethodWithNative(PRInt32 p1, void *p2) { return !_to ? NS_ERROR_NULL_POINTER : _to->MethodWithNative(p1, p2); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode(PRInt32 code) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnCode(code); } \
  NS_SCRIPTABLE NS_IMETHOD FailInJSTest(PRInt32 fail) { return !_to ? NS_ERROR_NULL_POINTER : _to->FailInJSTest(fail); } \
  NS_SCRIPTABLE NS_IMETHOD SharedString(const char **str NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->SharedString(str); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_OK(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnCode_NS_OK(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_NULL_POINTER(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnCode_NS_ERROR_NULL_POINTER(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_UNEXPECTED(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnCode_NS_ERROR_UNEXPECTED(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnCode_NS_ERROR_OUT_OF_MEMORY(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnCode_NS_ERROR_OUT_OF_MEMORY(); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnInterface(nsISupports *obj, nsISupports **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnInterface(obj, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStack(nsIStackFrame **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStack(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD SetReceiverReturnOldReceiver(nsIEcho **aReceiver NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReceiverReturnOldReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD MethodWithForwardDeclaredParam(nsITestXPCSomeUselessThing *sut) { return !_to ? NS_ERROR_NULL_POINTER : _to->MethodWithForwardDeclaredParam(sut); } \
  NS_SCRIPTABLE NS_IMETHOD PseudoQueryInterface(const nsIID & uuid, void **result NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->PseudoQueryInterface(uuid, result); } \
  NS_SCRIPTABLE NS_IMETHOD DebugDumpJSStack(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->DebugDumpJSStack(); } \
  NS_SCRIPTABLE NS_IMETHOD PrintArgTypes(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrintArgTypes(); } \
  NS_SCRIPTABLE NS_IMETHOD ThrowArg(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ThrowArg(); } \
  NS_SCRIPTABLE NS_IMETHOD CallReceiverSometimeLater(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallReceiverSometimeLater(); } \
  NS_SCRIPTABLE NS_IMETHOD GetThrowInGetter(PRInt16 *aThrowInGetter) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetThrowInGetter(aThrowInGetter); } \
  NS_SCRIPTABLE NS_IMETHOD GetAString(char **aAString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAString(aAString); } \
  NS_SCRIPTABLE NS_IMETHOD SetAString(const char *aAString) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAString(aAString); } \
  NS_SCRIPTABLE NS_IMETHOD GetSomeValue(PRInt32 *aSomeValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSomeValue(aSomeValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetSomeValue(PRInt32 aSomeValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSomeValue(aSomeValue); } \
  NS_SCRIPTABLE NS_IMETHOD CallFunction(nsITestXPCFunctionCallback *callback, const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallFunction(callback, s); } \
  NS_SCRIPTABLE NS_IMETHOD CallFunctionWithThis(nsITestXPCFunctionCallback *callback, nsISupports *self, const char *s) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallFunctionWithThis(callback, self, s); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsEcho : public nsIEcho
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIECHO

  nsEcho();

private:
  ~nsEcho();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsEcho, nsIEcho)

nsEcho::nsEcho()
{
  /* member initializers and constructor code */
}

nsEcho::~nsEcho()
{
  /* destructor code */
}

/* void SetReceiver (in nsIEcho aReceiver); */
NS_IMETHODIMP nsEcho::SetReceiver(nsIEcho *aReceiver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SendOneString (in string str); */
NS_IMETHODIMP nsEcho::SendOneString(const char *str)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long In2OutOneInt (in long input); */
NS_IMETHODIMP nsEcho::In2OutOneInt(PRInt32 input, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long In2OutAddTwoInts (in long input1, in long input2, out long output1, out long output2); */
NS_IMETHODIMP nsEcho::In2OutAddTwoInts(PRInt32 input1, PRInt32 input2, PRInt32 *output1 NS_OUTPARAM, PRInt32 *output2 NS_OUTPARAM, PRInt32 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string In2OutOneString (in string input); */
NS_IMETHODIMP nsEcho::In2OutOneString(const char *input, char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* DOMString In2OutOneDOMString (in DOMString input); */
NS_IMETHODIMP nsEcho::In2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* DOMString EchoIn2OutOneDOMString (in DOMString input); */
NS_IMETHODIMP nsEcho::EchoIn2OutOneDOMString(const nsAString & input, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString In2OutOneAString (in AString input); */
NS_IMETHODIMP nsEcho::In2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString EchoIn2OutOneAString (in AString input); */
NS_IMETHODIMP nsEcho::EchoIn2OutOneAString(const nsAString & input, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String In2OutOneUTF8String (in AUTF8String input); */
NS_IMETHODIMP nsEcho::In2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String EchoIn2OutOneUTF8String (in AUTF8String input); */
NS_IMETHODIMP nsEcho::EchoIn2OutOneUTF8String(const nsACString & input, nsACString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* ACString In2OutOneCString (in ACString input); */
NS_IMETHODIMP nsEcho::In2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* ACString EchoIn2OutOneCString (in ACString input); */
NS_IMETHODIMP nsEcho::EchoIn2OutOneCString(const nsACString & input, nsACString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SimpleCallNoEcho (); */
NS_IMETHODIMP nsEcho::SimpleCallNoEcho()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SendManyTypes (in octet p1, in short p2, in long p3, in long long p4, in octet p5, in unsigned short p6, in unsigned long p7, in unsigned long long p8, in float p9, in double p10, in boolean p11, in char p12, in wchar p13, in nsIDPtr p14, in string p15, in wstring p16); */
NS_IMETHODIMP nsEcho::SendManyTypes(PRUint8 p1, PRInt16 p2, PRInt32 p3, PRInt64 p4, PRUint8 p5, PRUint16 p6, PRUint32 p7, PRUint64 p8, float p9, double p10, PRBool p11, char p12, PRUnichar p13, const nsID *p14, const char *p15, const PRUnichar *p16)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SendInOutManyTypes (inout octet p1, inout short p2, inout long p3, inout long long p4, inout octet p5, inout unsigned short p6, inout unsigned long p7, inout unsigned long long p8, inout float p9, inout double p10, inout boolean p11, inout char p12, inout wchar p13, inout nsIDPtr p14, inout string p15, inout wstring p16); */
NS_IMETHODIMP nsEcho::SendInOutManyTypes(PRUint8 *p1 NS_INOUTPARAM, PRInt16 *p2 NS_INOUTPARAM, PRInt32 *p3 NS_INOUTPARAM, PRInt64 *p4 NS_INOUTPARAM, PRUint8 *p5 NS_INOUTPARAM, PRUint16 *p6 NS_INOUTPARAM, PRUint32 *p7 NS_INOUTPARAM, PRUint64 *p8 NS_INOUTPARAM, float *p9 NS_INOUTPARAM, double *p10 NS_INOUTPARAM, PRBool *p11 NS_INOUTPARAM, char *p12 NS_INOUTPARAM, PRUnichar *p13 NS_INOUTPARAM, nsID **p14 NS_INOUTPARAM, char **p15 NS_INOUTPARAM, PRUnichar **p16 NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void MethodWithNative (in long p1, in voidPtr p2); */
NS_IMETHODIMP nsEcho::MethodWithNative(PRInt32 p1, void *p2)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReturnCode (in long code); */
NS_IMETHODIMP nsEcho::ReturnCode(PRInt32 code)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void FailInJSTest (in long fail); */
NS_IMETHODIMP nsEcho::FailInJSTest(PRInt32 fail)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SharedString ([shared, retval] out string str); */
NS_IMETHODIMP nsEcho::SharedString(const char **str NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReturnCode_NS_OK (); */
NS_IMETHODIMP nsEcho::ReturnCode_NS_OK()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReturnCode_NS_ERROR_NULL_POINTER (); */
NS_IMETHODIMP nsEcho::ReturnCode_NS_ERROR_NULL_POINTER()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReturnCode_NS_ERROR_UNEXPECTED (); */
NS_IMETHODIMP nsEcho::ReturnCode_NS_ERROR_UNEXPECTED()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReturnCode_NS_ERROR_OUT_OF_MEMORY (); */
NS_IMETHODIMP nsEcho::ReturnCode_NS_ERROR_OUT_OF_MEMORY()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISupports ReturnInterface (in nsISupports obj); */
NS_IMETHODIMP nsEcho::ReturnInterface(nsISupports *obj, nsISupports **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIStackFrame GetStack (); */
NS_IMETHODIMP nsEcho::GetStack(nsIStackFrame **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void SetReceiverReturnOldReceiver (inout nsIEcho aReceiver); */
NS_IMETHODIMP nsEcho::SetReceiverReturnOldReceiver(nsIEcho **aReceiver NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void MethodWithForwardDeclaredParam (in nsITestXPCSomeUselessThing sut); */
NS_IMETHODIMP nsEcho::MethodWithForwardDeclaredParam(nsITestXPCSomeUselessThing *sut)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void PseudoQueryInterface (in nsIIDRef uuid, [iid_is (uuid), retval] out nsQIResult result); */
NS_IMETHODIMP nsEcho::PseudoQueryInterface(const nsIID & uuid, void **result NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void DebugDumpJSStack (); */
NS_IMETHODIMP nsEcho::DebugDumpJSStack()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void printArgTypes (); */
NS_IMETHODIMP nsEcho::PrintArgTypes()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void throwArg (); */
NS_IMETHODIMP nsEcho::ThrowArg()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void callReceiverSometimeLater (); */
NS_IMETHODIMP nsEcho::CallReceiverSometimeLater()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute short throwInGetter; */
NS_IMETHODIMP nsEcho::GetThrowInGetter(PRInt16 *aThrowInGetter)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute string aString; */
NS_IMETHODIMP nsEcho::GetAString(char **aAString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsEcho::SetAString(const char *aAString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute PRInt32 SomeValue; */
NS_IMETHODIMP nsEcho::GetSomeValue(PRInt32 *aSomeValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsEcho::SetSomeValue(PRInt32 aSomeValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void callFunction (in nsITestXPCFunctionCallback callback, in string s); */
NS_IMETHODIMP nsEcho::CallFunction(nsITestXPCFunctionCallback *callback, const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void callFunctionWithThis (in nsITestXPCFunctionCallback callback, in nsISupports self, in string s); */
NS_IMETHODIMP nsEcho::CallFunctionWithThis(nsITestXPCFunctionCallback *callback, nsISupports *self, const char *s)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestParent */
#define NS_IXPCTESTPARENT_IID_STR "83849a30-0d6e-11d3-bab8-00805f8a5dd7"

#define NS_IXPCTESTPARENT_IID \
  {0x83849a30, 0x0d6e, 0x11d3, \
    { 0xba, 0xb8, 0x00, 0x80, 0x5f, 0x8a, 0x5d, 0xd7 }}

/***************************************************************************/
class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestParent : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTPARENT_IID)

  /* void method1 (in short i); */
  NS_SCRIPTABLE NS_IMETHOD Method1(PRInt16 i) = 0;

  /* void method1a (in nsIXPCTestParent foo); */
  NS_SCRIPTABLE NS_IMETHOD Method1a(nsIXPCTestParent *foo) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestParent, NS_IXPCTESTPARENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTPARENT \
  NS_SCRIPTABLE NS_IMETHOD Method1(PRInt16 i); \
  NS_SCRIPTABLE NS_IMETHOD Method1a(nsIXPCTestParent *foo); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTPARENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method1(PRInt16 i) { return _to Method1(i); } \
  NS_SCRIPTABLE NS_IMETHOD Method1a(nsIXPCTestParent *foo) { return _to Method1a(foo); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTPARENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method1(PRInt16 i) { return !_to ? NS_ERROR_NULL_POINTER : _to->Method1(i); } \
  NS_SCRIPTABLE NS_IMETHOD Method1a(nsIXPCTestParent *foo) { return !_to ? NS_ERROR_NULL_POINTER : _to->Method1a(foo); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestParent : public nsIXPCTestParent
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTPARENT

  nsXPCTestParent();

private:
  ~nsXPCTestParent();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestParent, nsIXPCTestParent)

nsXPCTestParent::nsXPCTestParent()
{
  /* member initializers and constructor code */
}

nsXPCTestParent::~nsXPCTestParent()
{
  /* destructor code */
}

/* void method1 (in short i); */
NS_IMETHODIMP nsXPCTestParent::Method1(PRInt16 i)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void method1a (in nsIXPCTestParent foo); */
NS_IMETHODIMP nsXPCTestParent::Method1a(nsIXPCTestParent *foo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestChild */
#define NS_IXPCTESTCHILD_IID_STR "9ddf0f00-0d6e-11d3-bab8-00805f8a5dd7"

#define NS_IXPCTESTCHILD_IID \
  {0x9ddf0f00, 0x0d6e, 0x11d3, \
    { 0xba, 0xb8, 0x00, 0x80, 0x5f, 0x8a, 0x5d, 0xd7 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestChild : public nsIXPCTestParent {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCHILD_IID)

  /* void method2 (in short i, in short j); */
  NS_SCRIPTABLE NS_IMETHOD Method2(PRInt16 i, PRInt16 j) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestChild, NS_IXPCTESTCHILD_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCHILD \
  NS_SCRIPTABLE NS_IMETHOD Method2(PRInt16 i, PRInt16 j); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCHILD(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method2(PRInt16 i, PRInt16 j) { return _to Method2(i, j); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCHILD(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method2(PRInt16 i, PRInt16 j) { return !_to ? NS_ERROR_NULL_POINTER : _to->Method2(i, j); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestChild : public nsIXPCTestChild
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCHILD

  nsXPCTestChild();

private:
  ~nsXPCTestChild();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestChild, nsIXPCTestChild)

nsXPCTestChild::nsXPCTestChild()
{
  /* member initializers and constructor code */
}

nsXPCTestChild::~nsXPCTestChild()
{
  /* destructor code */
}

/* void method2 (in short i, in short j); */
NS_IMETHODIMP nsXPCTestChild::Method2(PRInt16 i, PRInt16 j)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestOther */
#define NS_IXPCTESTOTHER_IID_STR "7bca65e0-0d73-11d3-bab8-00805f8a5dd7"

#define NS_IXPCTESTOTHER_IID \
  {0x7bca65e0, 0x0d73, 0x11d3, \
    { 0xba, 0xb8, 0x00, 0x80, 0x5f, 0x8a, 0x5d, 0xd7 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestOther : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTOTHER_IID)

  /* void method3 (in short i, in short j, in short k); */
  NS_SCRIPTABLE NS_IMETHOD Method3(PRInt16 i, PRInt16 j, PRInt16 k) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestOther, NS_IXPCTESTOTHER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTOTHER \
  NS_SCRIPTABLE NS_IMETHOD Method3(PRInt16 i, PRInt16 j, PRInt16 k); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTOTHER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method3(PRInt16 i, PRInt16 j, PRInt16 k) { return _to Method3(i, j, k); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTOTHER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Method3(PRInt16 i, PRInt16 j, PRInt16 k) { return !_to ? NS_ERROR_NULL_POINTER : _to->Method3(i, j, k); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestOther : public nsIXPCTestOther
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTOTHER

  nsXPCTestOther();

private:
  ~nsXPCTestOther();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestOther, nsIXPCTestOther)

nsXPCTestOther::nsXPCTestOther()
{
  /* member initializers and constructor code */
}

nsXPCTestOther::~nsXPCTestOther()
{
  /* destructor code */
}

/* void method3 (in short i, in short j, in short k); */
NS_IMETHODIMP nsXPCTestOther::Method3(PRInt16 i, PRInt16 j, PRInt16 k)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestNoisy */
#define NS_IXPCTESTNOISY_IID_STR "1e4cf7b0-237a-11d3-9879-006008962422"

#define NS_IXPCTESTNOISY_IID \
  {0x1e4cf7b0, 0x237a, 0x11d3, \
    { 0x98, 0x79, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestNoisy : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTNOISY_IID)

  /* void squawk (); */
  NS_SCRIPTABLE NS_IMETHOD Squawk(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestNoisy, NS_IXPCTESTNOISY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTNOISY \
  NS_SCRIPTABLE NS_IMETHOD Squawk(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTNOISY(_to) \
  NS_SCRIPTABLE NS_IMETHOD Squawk(void) { return _to Squawk(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTNOISY(_to) \
  NS_SCRIPTABLE NS_IMETHOD Squawk(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Squawk(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestNoisy : public nsIXPCTestNoisy
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTNOISY

  nsXPCTestNoisy();

private:
  ~nsXPCTestNoisy();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestNoisy, nsIXPCTestNoisy)

nsXPCTestNoisy::nsXPCTestNoisy()
{
  /* member initializers and constructor code */
}

nsXPCTestNoisy::~nsXPCTestNoisy()
{
  /* destructor code */
}

/* void squawk (); */
NS_IMETHODIMP nsXPCTestNoisy::Squawk()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestString */
#define NS_IXPCTESTSTRING_IID_STR "d970e910-30d8-11d3-9885-006008962422"

#define NS_IXPCTESTSTRING_IID \
  {0xd970e910, 0x30d8, 0x11d3, \
    { 0x98, 0x85, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestString : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTSTRING_IID)

  /* string GetStringA (); */
  NS_SCRIPTABLE NS_IMETHOD GetStringA(char **_retval NS_OUTPARAM) = 0;

  /* void GetStringB (out string s); */
  NS_SCRIPTABLE NS_IMETHOD GetStringB(char **s NS_OUTPARAM) = 0;

  /* void GetStringC ([shared, retval] out string s); */
  NS_SCRIPTABLE NS_IMETHOD GetStringC(const char **s NS_OUTPARAM) = 0;

  /* void GetWStringCopied ([retval] out wstring s); */
  NS_SCRIPTABLE NS_IMETHOD GetWStringCopied(PRUnichar **s NS_OUTPARAM) = 0;

  /* void GetWStringShared ([shared, retval] out wstring s); */
  NS_SCRIPTABLE NS_IMETHOD GetWStringShared(const PRUnichar **s NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestString, NS_IXPCTESTSTRING_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTSTRING \
  NS_SCRIPTABLE NS_IMETHOD GetStringA(char **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetStringB(char **s NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetStringC(const char **s NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetWStringCopied(PRUnichar **s NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetWStringShared(const PRUnichar **s NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTSTRING(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStringA(char **_retval NS_OUTPARAM) { return _to GetStringA(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStringB(char **s NS_OUTPARAM) { return _to GetStringB(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetStringC(const char **s NS_OUTPARAM) { return _to GetStringC(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetWStringCopied(PRUnichar **s NS_OUTPARAM) { return _to GetWStringCopied(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetWStringShared(const PRUnichar **s NS_OUTPARAM) { return _to GetWStringShared(s); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTSTRING(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStringA(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringA(_retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStringB(char **s NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringB(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetStringC(const char **s NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringC(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetWStringCopied(PRUnichar **s NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWStringCopied(s); } \
  NS_SCRIPTABLE NS_IMETHOD GetWStringShared(const PRUnichar **s NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWStringShared(s); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestString : public nsIXPCTestString
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTSTRING

  nsXPCTestString();

private:
  ~nsXPCTestString();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestString, nsIXPCTestString)

nsXPCTestString::nsXPCTestString()
{
  /* member initializers and constructor code */
}

nsXPCTestString::~nsXPCTestString()
{
  /* destructor code */
}

/* string GetStringA (); */
NS_IMETHODIMP nsXPCTestString::GetStringA(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetStringB (out string s); */
NS_IMETHODIMP nsXPCTestString::GetStringB(char **s NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetStringC ([shared, retval] out string s); */
NS_IMETHODIMP nsXPCTestString::GetStringC(const char **s NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetWStringCopied ([retval] out wstring s); */
NS_IMETHODIMP nsXPCTestString::GetWStringCopied(PRUnichar **s NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetWStringShared ([shared, retval] out wstring s); */
NS_IMETHODIMP nsXPCTestString::GetWStringShared(const PRUnichar **s NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestOverloaded */
#define NS_IXPCTESTOVERLOADED_IID_STR "0ff4faf0-439a-11d3-988c-006008962422"

#define NS_IXPCTESTOVERLOADED_IID \
  {0x0ff4faf0, 0x439a, 0x11d3, \
    { 0x98, 0x8c, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestOverloaded : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTOVERLOADED_IID)

  /* void Foo1 (in PRInt32 p1); */
  NS_SCRIPTABLE NS_IMETHOD Foo1(PRInt32 p1) = 0;

  /* void Foo2 (in PRInt32 p1, in PRInt32 p2); */
  NS_SCRIPTABLE NS_IMETHOD Foo2(PRInt32 p1, PRInt32 p2) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestOverloaded, NS_IXPCTESTOVERLOADED_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTOVERLOADED \
  NS_SCRIPTABLE NS_IMETHOD Foo1(PRInt32 p1); \
  NS_SCRIPTABLE NS_IMETHOD Foo2(PRInt32 p1, PRInt32 p2); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTOVERLOADED(_to) \
  NS_SCRIPTABLE NS_IMETHOD Foo1(PRInt32 p1) { return _to Foo1(p1); } \
  NS_SCRIPTABLE NS_IMETHOD Foo2(PRInt32 p1, PRInt32 p2) { return _to Foo2(p1, p2); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTOVERLOADED(_to) \
  NS_SCRIPTABLE NS_IMETHOD Foo1(PRInt32 p1) { return !_to ? NS_ERROR_NULL_POINTER : _to->Foo1(p1); } \
  NS_SCRIPTABLE NS_IMETHOD Foo2(PRInt32 p1, PRInt32 p2) { return !_to ? NS_ERROR_NULL_POINTER : _to->Foo2(p1, p2); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestOverloaded : public nsIXPCTestOverloaded
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTOVERLOADED

  nsXPCTestOverloaded();

private:
  ~nsXPCTestOverloaded();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestOverloaded, nsIXPCTestOverloaded)

nsXPCTestOverloaded::nsXPCTestOverloaded()
{
  /* member initializers and constructor code */
}

nsXPCTestOverloaded::~nsXPCTestOverloaded()
{
  /* destructor code */
}

/* void Foo1 (in PRInt32 p1); */
NS_IMETHODIMP nsXPCTestOverloaded::Foo1(PRInt32 p1)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Foo2 (in PRInt32 p1, in PRInt32 p2); */
NS_IMETHODIMP nsXPCTestOverloaded::Foo2(PRInt32 p1, PRInt32 p2)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestArray */
#define NS_IXPCTESTARRAY_IID_STR "3d2e64b0-6552-11d3-989e-006008962422"

#define NS_IXPCTESTARRAY_IID \
  {0x3d2e64b0, 0x6552, 0x11d3, \
    { 0x98, 0x9e, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestArray : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTARRAY_IID)

  /* void SetReceiver (in nsIXPCTestArray aReceiver); */
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIXPCTestArray *aReceiver) = 0;

  /* void PrintIntegerArray (in PRUint32 count, [array, size_is (count)] in PRInt32 valueArray); */
  NS_SCRIPTABLE NS_IMETHOD PrintIntegerArray(PRUint32 count, PRInt32 *valueArray) = 0;

  /* void PrintStringArray (in PRUint32 count, [array, size_is (count)] in string valueArray); */
  NS_SCRIPTABLE NS_IMETHOD PrintStringArray(PRUint32 count, const char **valueArray) = 0;

  /* void MultiplyEachItemInIntegerArray (in PRInt32 val, in PRUint32 count, [array, size_is (count)] inout PRInt32 valueArray); */
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArray(PRInt32 val, PRUint32 count, PRInt32 **valueArray NS_INOUTPARAM) = 0;

  /* void MultiplyEachItemInIntegerArrayAndAppend (in PRInt32 val, inout PRUint32 count, [array, size_is (count)] inout PRInt32 valueArray); */
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArrayAndAppend(PRInt32 val, PRUint32 *count NS_INOUTPARAM, PRInt32 **valueArray NS_INOUTPARAM) = 0;

  /* void CallEchoMethodOnEachInArray (inout nsIIDPtr uuid, inout PRUint32 count, [array, size_is (count), iid_is (uuid)] inout nsQIResult result); */
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray(nsIID **uuid NS_INOUTPARAM, PRUint32 *count NS_INOUTPARAM, void ***result NS_INOUTPARAM) = 0;

  /* void CallEchoMethodOnEachInArray2 (inout PRUint32 count, [array, size_is (count)] inout nsIEcho result); */
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray2(PRUint32 *count NS_INOUTPARAM, nsIEcho ***result NS_INOUTPARAM) = 0;

  /* void DoubleStringArray (inout PRUint32 count, [array, size_is (count)] inout string valueArray); */
  NS_SCRIPTABLE NS_IMETHOD DoubleStringArray(PRUint32 *count NS_INOUTPARAM, char ***valueArray NS_INOUTPARAM) = 0;

  /* void ReverseStringArray (in PRUint32 count, [array, size_is (count)] inout string valueArray); */
  NS_SCRIPTABLE NS_IMETHOD ReverseStringArray(PRUint32 count, char ***valueArray NS_INOUTPARAM) = 0;

  /* void PrintStringWithSize (in PRUint32 count, [size_is (count)] in string str); */
  NS_SCRIPTABLE NS_IMETHOD PrintStringWithSize(PRUint32 count, const char *str) = 0;

  /* void DoubleString (inout PRUint32 count, [size_is (count)] inout string str); */
  NS_SCRIPTABLE NS_IMETHOD DoubleString(PRUint32 *count NS_INOUTPARAM, char **str NS_INOUTPARAM) = 0;

  /* void GetStrings (out PRUint32 count, [array, size_is (count), retval] out string str); */
  NS_SCRIPTABLE NS_IMETHOD GetStrings(PRUint32 *count NS_OUTPARAM, char ***str NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestArray, NS_IXPCTESTARRAY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTARRAY \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIXPCTestArray *aReceiver); \
  NS_SCRIPTABLE NS_IMETHOD PrintIntegerArray(PRUint32 count, PRInt32 *valueArray); \
  NS_SCRIPTABLE NS_IMETHOD PrintStringArray(PRUint32 count, const char **valueArray); \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArray(PRInt32 val, PRUint32 count, PRInt32 **valueArray NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArrayAndAppend(PRInt32 val, PRUint32 *count NS_INOUTPARAM, PRInt32 **valueArray NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray(nsIID **uuid NS_INOUTPARAM, PRUint32 *count NS_INOUTPARAM, void ***result NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray2(PRUint32 *count NS_INOUTPARAM, nsIEcho ***result NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD DoubleStringArray(PRUint32 *count NS_INOUTPARAM, char ***valueArray NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD ReverseStringArray(PRUint32 count, char ***valueArray NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD PrintStringWithSize(PRUint32 count, const char *str); \
  NS_SCRIPTABLE NS_IMETHOD DoubleString(PRUint32 *count NS_INOUTPARAM, char **str NS_INOUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetStrings(PRUint32 *count NS_OUTPARAM, char ***str NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTARRAY(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIXPCTestArray *aReceiver) { return _to SetReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD PrintIntegerArray(PRUint32 count, PRInt32 *valueArray) { return _to PrintIntegerArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD PrintStringArray(PRUint32 count, const char **valueArray) { return _to PrintStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArray(PRInt32 val, PRUint32 count, PRInt32 **valueArray NS_INOUTPARAM) { return _to MultiplyEachItemInIntegerArray(val, count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArrayAndAppend(PRInt32 val, PRUint32 *count NS_INOUTPARAM, PRInt32 **valueArray NS_INOUTPARAM) { return _to MultiplyEachItemInIntegerArrayAndAppend(val, count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray(nsIID **uuid NS_INOUTPARAM, PRUint32 *count NS_INOUTPARAM, void ***result NS_INOUTPARAM) { return _to CallEchoMethodOnEachInArray(uuid, count, result); } \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray2(PRUint32 *count NS_INOUTPARAM, nsIEcho ***result NS_INOUTPARAM) { return _to CallEchoMethodOnEachInArray2(count, result); } \
  NS_SCRIPTABLE NS_IMETHOD DoubleStringArray(PRUint32 *count NS_INOUTPARAM, char ***valueArray NS_INOUTPARAM) { return _to DoubleStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD ReverseStringArray(PRUint32 count, char ***valueArray NS_INOUTPARAM) { return _to ReverseStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD PrintStringWithSize(PRUint32 count, const char *str) { return _to PrintStringWithSize(count, str); } \
  NS_SCRIPTABLE NS_IMETHOD DoubleString(PRUint32 *count NS_INOUTPARAM, char **str NS_INOUTPARAM) { return _to DoubleString(count, str); } \
  NS_SCRIPTABLE NS_IMETHOD GetStrings(PRUint32 *count NS_OUTPARAM, char ***str NS_OUTPARAM) { return _to GetStrings(count, str); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTARRAY(_to) \
  NS_SCRIPTABLE NS_IMETHOD SetReceiver(nsIXPCTestArray *aReceiver) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReceiver(aReceiver); } \
  NS_SCRIPTABLE NS_IMETHOD PrintIntegerArray(PRUint32 count, PRInt32 *valueArray) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrintIntegerArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD PrintStringArray(PRUint32 count, const char **valueArray) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrintStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArray(PRInt32 val, PRUint32 count, PRInt32 **valueArray NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->MultiplyEachItemInIntegerArray(val, count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD MultiplyEachItemInIntegerArrayAndAppend(PRInt32 val, PRUint32 *count NS_INOUTPARAM, PRInt32 **valueArray NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->MultiplyEachItemInIntegerArrayAndAppend(val, count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray(nsIID **uuid NS_INOUTPARAM, PRUint32 *count NS_INOUTPARAM, void ***result NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallEchoMethodOnEachInArray(uuid, count, result); } \
  NS_SCRIPTABLE NS_IMETHOD CallEchoMethodOnEachInArray2(PRUint32 *count NS_INOUTPARAM, nsIEcho ***result NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CallEchoMethodOnEachInArray2(count, result); } \
  NS_SCRIPTABLE NS_IMETHOD DoubleStringArray(PRUint32 *count NS_INOUTPARAM, char ***valueArray NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->DoubleStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD ReverseStringArray(PRUint32 count, char ***valueArray NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReverseStringArray(count, valueArray); } \
  NS_SCRIPTABLE NS_IMETHOD PrintStringWithSize(PRUint32 count, const char *str) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrintStringWithSize(count, str); } \
  NS_SCRIPTABLE NS_IMETHOD DoubleString(PRUint32 *count NS_INOUTPARAM, char **str NS_INOUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->DoubleString(count, str); } \
  NS_SCRIPTABLE NS_IMETHOD GetStrings(PRUint32 *count NS_OUTPARAM, char ***str NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStrings(count, str); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestArray : public nsIXPCTestArray
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTARRAY

  nsXPCTestArray();

private:
  ~nsXPCTestArray();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestArray, nsIXPCTestArray)

nsXPCTestArray::nsXPCTestArray()
{
  /* member initializers and constructor code */
}

nsXPCTestArray::~nsXPCTestArray()
{
  /* destructor code */
}

/* void SetReceiver (in nsIXPCTestArray aReceiver); */
NS_IMETHODIMP nsXPCTestArray::SetReceiver(nsIXPCTestArray *aReceiver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void PrintIntegerArray (in PRUint32 count, [array, size_is (count)] in PRInt32 valueArray); */
NS_IMETHODIMP nsXPCTestArray::PrintIntegerArray(PRUint32 count, PRInt32 *valueArray)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void PrintStringArray (in PRUint32 count, [array, size_is (count)] in string valueArray); */
NS_IMETHODIMP nsXPCTestArray::PrintStringArray(PRUint32 count, const char **valueArray)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void MultiplyEachItemInIntegerArray (in PRInt32 val, in PRUint32 count, [array, size_is (count)] inout PRInt32 valueArray); */
NS_IMETHODIMP nsXPCTestArray::MultiplyEachItemInIntegerArray(PRInt32 val, PRUint32 count, PRInt32 **valueArray NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void MultiplyEachItemInIntegerArrayAndAppend (in PRInt32 val, inout PRUint32 count, [array, size_is (count)] inout PRInt32 valueArray); */
NS_IMETHODIMP nsXPCTestArray::MultiplyEachItemInIntegerArrayAndAppend(PRInt32 val, PRUint32 *count NS_INOUTPARAM, PRInt32 **valueArray NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void CallEchoMethodOnEachInArray (inout nsIIDPtr uuid, inout PRUint32 count, [array, size_is (count), iid_is (uuid)] inout nsQIResult result); */
NS_IMETHODIMP nsXPCTestArray::CallEchoMethodOnEachInArray(nsIID **uuid NS_INOUTPARAM, PRUint32 *count NS_INOUTPARAM, void ***result NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void CallEchoMethodOnEachInArray2 (inout PRUint32 count, [array, size_is (count)] inout nsIEcho result); */
NS_IMETHODIMP nsXPCTestArray::CallEchoMethodOnEachInArray2(PRUint32 *count NS_INOUTPARAM, nsIEcho ***result NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void DoubleStringArray (inout PRUint32 count, [array, size_is (count)] inout string valueArray); */
NS_IMETHODIMP nsXPCTestArray::DoubleStringArray(PRUint32 *count NS_INOUTPARAM, char ***valueArray NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReverseStringArray (in PRUint32 count, [array, size_is (count)] inout string valueArray); */
NS_IMETHODIMP nsXPCTestArray::ReverseStringArray(PRUint32 count, char ***valueArray NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void PrintStringWithSize (in PRUint32 count, [size_is (count)] in string str); */
NS_IMETHODIMP nsXPCTestArray::PrintStringWithSize(PRUint32 count, const char *str)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void DoubleString (inout PRUint32 count, [size_is (count)] inout string str); */
NS_IMETHODIMP nsXPCTestArray::DoubleString(PRUint32 *count NS_INOUTPARAM, char **str NS_INOUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void GetStrings (out PRUint32 count, [array, size_is (count), retval] out string str); */
NS_IMETHODIMP nsXPCTestArray::GetStrings(PRUint32 *count NS_OUTPARAM, char ***str NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWrappedJSObjectTest */
#define NS_IWRAPPEDJSOBJECTTEST_IID_STR "13082da0-643c-11d4-8fe0-0010a4e73d9a"

#define NS_IWRAPPEDJSOBJECTTEST_IID \
  {0x13082da0, 0x643c, 0x11d4, \
    { 0x8f, 0xe0, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIWrappedJSObjectTest : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IWRAPPEDJSOBJECTTEST_IID)

  /* void interfaceMember (); */
  NS_SCRIPTABLE NS_IMETHOD InterfaceMember(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIWrappedJSObjectTest, NS_IWRAPPEDJSOBJECTTEST_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWRAPPEDJSOBJECTTEST \
  NS_SCRIPTABLE NS_IMETHOD InterfaceMember(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWRAPPEDJSOBJECTTEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD InterfaceMember(void) { return _to InterfaceMember(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWRAPPEDJSOBJECTTEST(_to) \
  NS_SCRIPTABLE NS_IMETHOD InterfaceMember(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->InterfaceMember(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWrappedJSObjectTest : public nsIWrappedJSObjectTest
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWRAPPEDJSOBJECTTEST

  nsWrappedJSObjectTest();

private:
  ~nsWrappedJSObjectTest();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWrappedJSObjectTest, nsIWrappedJSObjectTest)

nsWrappedJSObjectTest::nsWrappedJSObjectTest()
{
  /* member initializers and constructor code */
}

nsWrappedJSObjectTest::~nsWrappedJSObjectTest()
{
  /* destructor code */
}

/* void interfaceMember (); */
NS_IMETHODIMP nsWrappedJSObjectTest::InterfaceMember()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIBogus1 */
#define NS_IBOGUS1_IID_STR "41d923d0-5bc1-11d5-90db-0010a4e73d9a"

#define NS_IBOGUS1_IID \
  {0x41d923d0, 0x5bc1, 0x11d5, \
    { 0x90, 0xdb, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIBogus1 : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IBOGUS1_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIBogus1, NS_IBOGUS1_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIBOGUS1 \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIBOGUS1(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIBOGUS1(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsBogus1 : public nsIBogus1
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIBOGUS1

  nsBogus1();

private:
  ~nsBogus1();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsBogus1, nsIBogus1)

nsBogus1::nsBogus1()
{
  /* member initializers and constructor code */
}

nsBogus1::~nsBogus1()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIBogus2 */
#define NS_IBOGUS2_IID_STR "8d8f4210-5bc1-11d5-90db-0010a4e73d9a"

#define NS_IBOGUS2_IID \
  {0x8d8f4210, 0x5bc1, 0x11d5, \
    { 0x90, 0xdb, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIBogus2 : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IBOGUS2_IID)

  /* attribute nsIBogus1 bogus; */
  NS_SCRIPTABLE NS_IMETHOD GetBogus(nsIBogus1 **aBogus) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetBogus(nsIBogus1 *aBogus) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIBogus2, NS_IBOGUS2_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIBOGUS2 \
  NS_SCRIPTABLE NS_IMETHOD GetBogus(nsIBogus1 **aBogus); \
  NS_SCRIPTABLE NS_IMETHOD SetBogus(nsIBogus1 *aBogus); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIBOGUS2(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetBogus(nsIBogus1 **aBogus) { return _to GetBogus(aBogus); } \
  NS_SCRIPTABLE NS_IMETHOD SetBogus(nsIBogus1 *aBogus) { return _to SetBogus(aBogus); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIBOGUS2(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetBogus(nsIBogus1 **aBogus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBogus(aBogus); } \
  NS_SCRIPTABLE NS_IMETHOD SetBogus(nsIBogus1 *aBogus) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBogus(aBogus); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsBogus2 : public nsIBogus2
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIBOGUS2

  nsBogus2();

private:
  ~nsBogus2();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsBogus2, nsIBogus2)

nsBogus2::nsBogus2()
{
  /* member initializers and constructor code */
}

nsBogus2::~nsBogus2()
{
  /* destructor code */
}

/* attribute nsIBogus1 bogus; */
NS_IMETHODIMP nsBogus2::GetBogus(nsIBogus1 **aBogus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsBogus2::SetBogus(nsIBogus1 *aBogus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsITestVariant */
#define NS_ITESTVARIANT_IID_STR "34bc9990-95af-11d5-90fc-0010a4e73d9a"

#define NS_ITESTVARIANT_IID \
  {0x34bc9990, 0x95af, 0x11d5, \
    { 0x90, 0xfc, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE NS_SCRIPTABLE nsITestVariant : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITESTVARIANT_IID)

  /**
     * Just return the variant that was passed in.
     */
  /* nsIVariant passThruVariant (in nsIVariant value); */
  NS_SCRIPTABLE NS_IMETHOD PassThruVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) = 0;

  /**
     * Just return the type of the passed in variant.
     */
  /* PRUint16 returnVariantType (in nsIVariant value); */
  NS_SCRIPTABLE NS_IMETHOD ReturnVariantType(nsIVariant *value, PRUint16 *_retval NS_OUTPARAM) = 0;

  /**
     * Construct and return a default xpcom variant instance using the
     * value of the passed in variant. Use the type of the passed in variant. 
     */
  /* nsIVariant copyVariant (in nsIVariant value); */
  NS_SCRIPTABLE NS_IMETHOD CopyVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) = 0;

  /**
     * Construct and return a default xpcom variant instance using the
     * value of the passed in variant. Use the type as specified to get data 
     * from the passed in variant. 
     */
  /* nsIVariant copyVariantAsType (in nsIVariant value, in PRUint16 type); */
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsType(nsIVariant *value, PRUint16 type, nsIVariant **_retval NS_OUTPARAM) = 0;

  /**
     * Construct a default xpcom variant instance using the
     * value of the passed in variant. Use the type as specified in type1 to 
     * get data from the passed in variant. Then construct and return a default
     * xpcom variant based on that object using the type2. This allows for 
     * testing all the 'ConvertTo*' methods on the default xpcom variant class.
     */
  /* nsIVariant copyVariantAsTypeTwice (in nsIVariant value, in PRUint16 type1, in PRUint16 type2); */
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsTypeTwice(nsIVariant *value, PRUint16 type1, PRUint16 type2, nsIVariant **_retval NS_OUTPARAM) = 0;

  /**
     * Get a named property from object that will be QI'd to nsIPropertyBag.
     */
  /* nsIVariant getNamedProperty (in nsISupports aBag, in AString aName); */
  NS_SCRIPTABLE NS_IMETHOD GetNamedProperty(nsISupports *aBag, const nsAString & aName, nsIVariant **_retval NS_OUTPARAM) = 0;

  /**
     * Get a enumerator from object that will be QI'd to nsIPropertyBag.
     */
  /* nsISimpleEnumerator getEnumerator (in nsISupports aBag); */
  NS_SCRIPTABLE NS_IMETHOD GetEnumerator(nsISupports *aBag, nsISimpleEnumerator **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITestVariant, NS_ITESTVARIANT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITESTVARIANT \
  NS_SCRIPTABLE NS_IMETHOD PassThruVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD ReturnVariantType(nsIVariant *value, PRUint16 *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CopyVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsType(nsIVariant *value, PRUint16 type, nsIVariant **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsTypeTwice(nsIVariant *value, PRUint16 type1, PRUint16 type2, nsIVariant **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetNamedProperty(nsISupports *aBag, const nsAString & aName, nsIVariant **_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetEnumerator(nsISupports *aBag, nsISimpleEnumerator **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITESTVARIANT(_to) \
  NS_SCRIPTABLE NS_IMETHOD PassThruVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) { return _to PassThruVariant(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnVariantType(nsIVariant *value, PRUint16 *_retval NS_OUTPARAM) { return _to ReturnVariantType(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) { return _to CopyVariant(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsType(nsIVariant *value, PRUint16 type, nsIVariant **_retval NS_OUTPARAM) { return _to CopyVariantAsType(value, type, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsTypeTwice(nsIVariant *value, PRUint16 type1, PRUint16 type2, nsIVariant **_retval NS_OUTPARAM) { return _to CopyVariantAsTypeTwice(value, type1, type2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetNamedProperty(nsISupports *aBag, const nsAString & aName, nsIVariant **_retval NS_OUTPARAM) { return _to GetNamedProperty(aBag, aName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetEnumerator(nsISupports *aBag, nsISimpleEnumerator **_retval NS_OUTPARAM) { return _to GetEnumerator(aBag, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITESTVARIANT(_to) \
  NS_SCRIPTABLE NS_IMETHOD PassThruVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->PassThruVariant(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD ReturnVariantType(nsIVariant *value, PRUint16 *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReturnVariantType(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CopyVariant(value, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsType(nsIVariant *value, PRUint16 type, nsIVariant **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CopyVariantAsType(value, type, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD CopyVariantAsTypeTwice(nsIVariant *value, PRUint16 type1, PRUint16 type2, nsIVariant **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->CopyVariantAsTypeTwice(value, type1, type2, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetNamedProperty(nsISupports *aBag, const nsAString & aName, nsIVariant **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamedProperty(aBag, aName, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetEnumerator(nsISupports *aBag, nsISimpleEnumerator **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEnumerator(aBag, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTestVariant : public nsITestVariant
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITESTVARIANT

  nsTestVariant();

private:
  ~nsTestVariant();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTestVariant, nsITestVariant)

nsTestVariant::nsTestVariant()
{
  /* member initializers and constructor code */
}

nsTestVariant::~nsTestVariant()
{
  /* destructor code */
}

/* nsIVariant passThruVariant (in nsIVariant value); */
NS_IMETHODIMP nsTestVariant::PassThruVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 returnVariantType (in nsIVariant value); */
NS_IMETHODIMP nsTestVariant::ReturnVariantType(nsIVariant *value, PRUint16 *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIVariant copyVariant (in nsIVariant value); */
NS_IMETHODIMP nsTestVariant::CopyVariant(nsIVariant *value, nsIVariant **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIVariant copyVariantAsType (in nsIVariant value, in PRUint16 type); */
NS_IMETHODIMP nsTestVariant::CopyVariantAsType(nsIVariant *value, PRUint16 type, nsIVariant **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIVariant copyVariantAsTypeTwice (in nsIVariant value, in PRUint16 type1, in PRUint16 type2); */
NS_IMETHODIMP nsTestVariant::CopyVariantAsTypeTwice(nsIVariant *value, PRUint16 type1, PRUint16 type2, nsIVariant **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIVariant getNamedProperty (in nsISupports aBag, in AString aName); */
NS_IMETHODIMP nsTestVariant::GetNamedProperty(nsISupports *aBag, const nsAString & aName, nsIVariant **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISimpleEnumerator getEnumerator (in nsISupports aBag); */
NS_IMETHODIMP nsTestVariant::GetEnumerator(nsISupports *aBag, nsISimpleEnumerator **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_h__ */
