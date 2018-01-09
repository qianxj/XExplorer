/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_attributes.idl
 */

#ifndef __gen_xpctest_attributes_h__
#define __gen_xpctest_attributes_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestObjectReadOnly */
#define NS_IXPCTESTOBJECTREADONLY_IID_STR "1364941e-4462-11d3-82ee-0060b0eb596f"

#define NS_IXPCTESTOBJECTREADONLY_IID \
  {0x1364941e, 0x4462, 0x11d3, \
    { 0x82, 0xee, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestObjectReadOnly : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTOBJECTREADONLY_IID)

  /* readonly attribute string strReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetStrReadOnly(char **aStrReadOnly) = 0;

  /* readonly attribute boolean boolReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetBoolReadOnly(PRBool *aBoolReadOnly) = 0;

  /* readonly attribute short shortReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetShortReadOnly(PRInt16 *aShortReadOnly) = 0;

  /* readonly attribute long longReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetLongReadOnly(PRInt32 *aLongReadOnly) = 0;

  /* readonly attribute float floatReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetFloatReadOnly(float *aFloatReadOnly) = 0;

  /* readonly attribute char charReadOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetCharReadOnly(char *aCharReadOnly) = 0;

  /* string getID (); */
  NS_SCRIPTABLE NS_IMETHOD GetID(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestObjectReadOnly, NS_IXPCTESTOBJECTREADONLY_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTOBJECTREADONLY \
  NS_SCRIPTABLE NS_IMETHOD GetStrReadOnly(char **aStrReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetBoolReadOnly(PRBool *aBoolReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetShortReadOnly(PRInt16 *aShortReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetLongReadOnly(PRInt32 *aLongReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetFloatReadOnly(float *aFloatReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetCharReadOnly(char *aCharReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetID(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTOBJECTREADONLY(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStrReadOnly(char **aStrReadOnly) { return _to GetStrReadOnly(aStrReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetBoolReadOnly(PRBool *aBoolReadOnly) { return _to GetBoolReadOnly(aBoolReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetShortReadOnly(PRInt16 *aShortReadOnly) { return _to GetShortReadOnly(aShortReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongReadOnly(PRInt32 *aLongReadOnly) { return _to GetLongReadOnly(aLongReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloatReadOnly(float *aFloatReadOnly) { return _to GetFloatReadOnly(aFloatReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetCharReadOnly(char *aCharReadOnly) { return _to GetCharReadOnly(aCharReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetID(char **_retval NS_OUTPARAM) { return _to GetID(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTOBJECTREADONLY(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStrReadOnly(char **aStrReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStrReadOnly(aStrReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetBoolReadOnly(PRBool *aBoolReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBoolReadOnly(aBoolReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetShortReadOnly(PRInt16 *aShortReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetShortReadOnly(aShortReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongReadOnly(PRInt32 *aLongReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLongReadOnly(aLongReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloatReadOnly(float *aFloatReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloatReadOnly(aFloatReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetCharReadOnly(char *aCharReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCharReadOnly(aCharReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetID(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetID(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestObjectReadOnly : public nsIXPCTestObjectReadOnly
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTOBJECTREADONLY

  nsXPCTestObjectReadOnly();

private:
  ~nsXPCTestObjectReadOnly();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestObjectReadOnly, nsIXPCTestObjectReadOnly)

nsXPCTestObjectReadOnly::nsXPCTestObjectReadOnly()
{
  /* member initializers and constructor code */
}

nsXPCTestObjectReadOnly::~nsXPCTestObjectReadOnly()
{
  /* destructor code */
}

/* readonly attribute string strReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetStrReadOnly(char **aStrReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean boolReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetBoolReadOnly(PRBool *aBoolReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute short shortReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetShortReadOnly(PRInt16 *aShortReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long longReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetLongReadOnly(PRInt32 *aLongReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute float floatReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetFloatReadOnly(float *aFloatReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute char charReadOnly; */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetCharReadOnly(char *aCharReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string getID (); */
NS_IMETHODIMP nsXPCTestObjectReadOnly::GetID(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestObjectReadWrite */
#define NS_IXPCTESTOBJECTREADWRITE_IID_STR "3b9b1d38-491a-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTOBJECTREADWRITE_IID \
  {0x3b9b1d38, 0x491a, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestObjectReadWrite : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTOBJECTREADWRITE_IID)

  /* attribute string stringProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetStringProperty(char **aStringProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetStringProperty(const char *aStringProperty) = 0;

  /* attribute boolean booleanProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetBooleanProperty(PRBool *aBooleanProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetBooleanProperty(PRBool aBooleanProperty) = 0;

  /* attribute short shortProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetShortProperty(PRInt16 *aShortProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetShortProperty(PRInt16 aShortProperty) = 0;

  /* attribute long longProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetLongProperty(PRInt32 *aLongProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetLongProperty(PRInt32 aLongProperty) = 0;

  /* attribute float floatProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetFloatProperty(float *aFloatProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetFloatProperty(float aFloatProperty) = 0;

  /* attribute char charProperty; */
  NS_SCRIPTABLE NS_IMETHOD GetCharProperty(char *aCharProperty) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetCharProperty(char aCharProperty) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestObjectReadWrite, NS_IXPCTESTOBJECTREADWRITE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTOBJECTREADWRITE \
  NS_SCRIPTABLE NS_IMETHOD GetStringProperty(char **aStringProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetStringProperty(const char *aStringProperty); \
  NS_SCRIPTABLE NS_IMETHOD GetBooleanProperty(PRBool *aBooleanProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetBooleanProperty(PRBool aBooleanProperty); \
  NS_SCRIPTABLE NS_IMETHOD GetShortProperty(PRInt16 *aShortProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetShortProperty(PRInt16 aShortProperty); \
  NS_SCRIPTABLE NS_IMETHOD GetLongProperty(PRInt32 *aLongProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetLongProperty(PRInt32 aLongProperty); \
  NS_SCRIPTABLE NS_IMETHOD GetFloatProperty(float *aFloatProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetFloatProperty(float aFloatProperty); \
  NS_SCRIPTABLE NS_IMETHOD GetCharProperty(char *aCharProperty); \
  NS_SCRIPTABLE NS_IMETHOD SetCharProperty(char aCharProperty); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTOBJECTREADWRITE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStringProperty(char **aStringProperty) { return _to GetStringProperty(aStringProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetStringProperty(const char *aStringProperty) { return _to SetStringProperty(aStringProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetBooleanProperty(PRBool *aBooleanProperty) { return _to GetBooleanProperty(aBooleanProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetBooleanProperty(PRBool aBooleanProperty) { return _to SetBooleanProperty(aBooleanProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetShortProperty(PRInt16 *aShortProperty) { return _to GetShortProperty(aShortProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetShortProperty(PRInt16 aShortProperty) { return _to SetShortProperty(aShortProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongProperty(PRInt32 *aLongProperty) { return _to GetLongProperty(aLongProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetLongProperty(PRInt32 aLongProperty) { return _to SetLongProperty(aLongProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloatProperty(float *aFloatProperty) { return _to GetFloatProperty(aFloatProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetFloatProperty(float aFloatProperty) { return _to SetFloatProperty(aFloatProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetCharProperty(char *aCharProperty) { return _to GetCharProperty(aCharProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetCharProperty(char aCharProperty) { return _to SetCharProperty(aCharProperty); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTOBJECTREADWRITE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetStringProperty(char **aStringProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStringProperty(aStringProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetStringProperty(const char *aStringProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetStringProperty(aStringProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetBooleanProperty(PRBool *aBooleanProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBooleanProperty(aBooleanProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetBooleanProperty(PRBool aBooleanProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBooleanProperty(aBooleanProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetShortProperty(PRInt16 *aShortProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetShortProperty(aShortProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetShortProperty(PRInt16 aShortProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetShortProperty(aShortProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetLongProperty(PRInt32 *aLongProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLongProperty(aLongProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetLongProperty(PRInt32 aLongProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLongProperty(aLongProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetFloatProperty(float *aFloatProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloatProperty(aFloatProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetFloatProperty(float aFloatProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFloatProperty(aFloatProperty); } \
  NS_SCRIPTABLE NS_IMETHOD GetCharProperty(char *aCharProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCharProperty(aCharProperty); } \
  NS_SCRIPTABLE NS_IMETHOD SetCharProperty(char aCharProperty) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCharProperty(aCharProperty); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestObjectReadWrite : public nsIXPCTestObjectReadWrite
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTOBJECTREADWRITE

  nsXPCTestObjectReadWrite();

private:
  ~nsXPCTestObjectReadWrite();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestObjectReadWrite, nsIXPCTestObjectReadWrite)

nsXPCTestObjectReadWrite::nsXPCTestObjectReadWrite()
{
  /* member initializers and constructor code */
}

nsXPCTestObjectReadWrite::~nsXPCTestObjectReadWrite()
{
  /* destructor code */
}

/* attribute string stringProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetStringProperty(char **aStringProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetStringProperty(const char *aStringProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean booleanProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetBooleanProperty(PRBool *aBooleanProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetBooleanProperty(PRBool aBooleanProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute short shortProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetShortProperty(PRInt16 *aShortProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetShortProperty(PRInt16 aShortProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long longProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetLongProperty(PRInt32 *aLongProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetLongProperty(PRInt32 aLongProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute float floatProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetFloatProperty(float *aFloatProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetFloatProperty(float aFloatProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute char charProperty; */
NS_IMETHODIMP nsXPCTestObjectReadWrite::GetCharProperty(char *aCharProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestObjectReadWrite::SetCharProperty(char aCharProperty)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_attributes_h__ */
