/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest_multiple.idl
 */

#ifndef __gen_xpctest_multiple_h__
#define __gen_xpctest_multiple_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIXPCTestParentOne */
#define NS_IXPCTESTPARENTONE_IID_STR "5408fdcc-60a3-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTPARENTONE_IID \
  {0x5408fdcc, 0x60a3, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestParentOne : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTPARENTONE_IID)

  /* attribute string ParentOneAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetParentOneAttribute(char **aParentOneAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetParentOneAttribute(const char *aParentOneAttribute) = 0;

  /* string ParentOneMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ParentOneMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestParentOne, NS_IXPCTESTPARENTONE_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTPARENTONE \
  NS_SCRIPTABLE NS_IMETHOD GetParentOneAttribute(char **aParentOneAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetParentOneAttribute(const char *aParentOneAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ParentOneMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTPARENTONE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetParentOneAttribute(char **aParentOneAttribute) { return _to GetParentOneAttribute(aParentOneAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetParentOneAttribute(const char *aParentOneAttribute) { return _to SetParentOneAttribute(aParentOneAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ParentOneMethod(char **_retval NS_OUTPARAM) { return _to ParentOneMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTPARENTONE(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetParentOneAttribute(char **aParentOneAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParentOneAttribute(aParentOneAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetParentOneAttribute(const char *aParentOneAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParentOneAttribute(aParentOneAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ParentOneMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ParentOneMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestParentOne : public nsIXPCTestParentOne
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTPARENTONE

  nsXPCTestParentOne();

private:
  ~nsXPCTestParentOne();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestParentOne, nsIXPCTestParentOne)

nsXPCTestParentOne::nsXPCTestParentOne()
{
  /* member initializers and constructor code */
}

nsXPCTestParentOne::~nsXPCTestParentOne()
{
  /* destructor code */
}

/* attribute string ParentOneAttribute; */
NS_IMETHODIMP nsXPCTestParentOne::GetParentOneAttribute(char **aParentOneAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestParentOne::SetParentOneAttribute(const char *aParentOneAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ParentOneMethod (); */
NS_IMETHODIMP nsXPCTestParentOne::ParentOneMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestParentTwo */
#define NS_IXPCTESTPARENTTWO_IID_STR "63137392-60a3-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTPARENTTWO_IID \
  {0x63137392, 0x60a3, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestParentTwo : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTPARENTTWO_IID)

  /* attribute string ParentTwoAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetParentTwoAttribute(char **aParentTwoAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetParentTwoAttribute(const char *aParentTwoAttribute) = 0;

  /* string ParentTwoMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ParentTwoMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestParentTwo, NS_IXPCTESTPARENTTWO_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTPARENTTWO \
  NS_SCRIPTABLE NS_IMETHOD GetParentTwoAttribute(char **aParentTwoAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetParentTwoAttribute(const char *aParentTwoAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ParentTwoMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTPARENTTWO(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetParentTwoAttribute(char **aParentTwoAttribute) { return _to GetParentTwoAttribute(aParentTwoAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetParentTwoAttribute(const char *aParentTwoAttribute) { return _to SetParentTwoAttribute(aParentTwoAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ParentTwoMethod(char **_retval NS_OUTPARAM) { return _to ParentTwoMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTPARENTTWO(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetParentTwoAttribute(char **aParentTwoAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParentTwoAttribute(aParentTwoAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetParentTwoAttribute(const char *aParentTwoAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParentTwoAttribute(aParentTwoAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ParentTwoMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ParentTwoMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestParentTwo : public nsIXPCTestParentTwo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTPARENTTWO

  nsXPCTestParentTwo();

private:
  ~nsXPCTestParentTwo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestParentTwo, nsIXPCTestParentTwo)

nsXPCTestParentTwo::nsXPCTestParentTwo()
{
  /* member initializers and constructor code */
}

nsXPCTestParentTwo::~nsXPCTestParentTwo()
{
  /* destructor code */
}

/* attribute string ParentTwoAttribute; */
NS_IMETHODIMP nsXPCTestParentTwo::GetParentTwoAttribute(char **aParentTwoAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestParentTwo::SetParentTwoAttribute(const char *aParentTwoAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ParentTwoMethod (); */
NS_IMETHODIMP nsXPCTestParentTwo::ParentTwoMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestChild2 */
#define NS_IXPCTESTCHILD2_IID_STR "66bed216-60a3-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCHILD2_IID \
  {0x66bed216, 0x60a3, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestChild2 : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCHILD2_IID)

  /* attribute string ChildAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) = 0;

  /* string ChildMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestChild2, NS_IXPCTESTCHILD2_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCHILD2 \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCHILD2(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return _to GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return _to SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return _to ChildMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCHILD2(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChildMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestChild2 : public nsIXPCTestChild2
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCHILD2

  nsXPCTestChild2();

private:
  ~nsXPCTestChild2();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestChild2, nsIXPCTestChild2)

nsXPCTestChild2::nsXPCTestChild2()
{
  /* member initializers and constructor code */
}

nsXPCTestChild2::~nsXPCTestChild2()
{
  /* destructor code */
}

/* attribute string ChildAttribute; */
NS_IMETHODIMP nsXPCTestChild2::GetChildAttribute(char **aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestChild2::SetChildAttribute(const char *aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ChildMethod (); */
NS_IMETHODIMP nsXPCTestChild2::ChildMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestChild3 */
#define NS_IXPCTESTCHILD3_IID_STR "62353978-614e-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCHILD3_IID \
  {0x62353978, 0x614e, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestChild3 : public nsIXPCTestParentOne {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCHILD3_IID)

  /* attribute string ChildAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) = 0;

  /* string ChildMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestChild3, NS_IXPCTESTCHILD3_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCHILD3 \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCHILD3(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return _to GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return _to SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return _to ChildMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCHILD3(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChildMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestChild3 : public nsIXPCTestChild3
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCHILD3

  nsXPCTestChild3();

private:
  ~nsXPCTestChild3();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestChild3, nsIXPCTestChild3)

nsXPCTestChild3::nsXPCTestChild3()
{
  /* member initializers and constructor code */
}

nsXPCTestChild3::~nsXPCTestChild3()
{
  /* destructor code */
}

/* attribute string ChildAttribute; */
NS_IMETHODIMP nsXPCTestChild3::GetChildAttribute(char **aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestChild3::SetChildAttribute(const char *aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ChildMethod (); */
NS_IMETHODIMP nsXPCTestChild3::ChildMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestChild4 */
#define NS_IXPCTESTCHILD4_IID_STR "a6d22202-622b-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCHILD4_IID \
  {0xa6d22202, 0x622b, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestChild4 : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCHILD4_IID)

  /* attribute string ChildAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) = 0;

  /* string ChildMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestChild4, NS_IXPCTESTCHILD4_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCHILD4 \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCHILD4(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return _to GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return _to SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return _to ChildMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCHILD4(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChildMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestChild4 : public nsIXPCTestChild4
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCHILD4

  nsXPCTestChild4();

private:
  ~nsXPCTestChild4();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestChild4, nsIXPCTestChild4)

nsXPCTestChild4::nsXPCTestChild4()
{
  /* member initializers and constructor code */
}

nsXPCTestChild4::~nsXPCTestChild4()
{
  /* destructor code */
}

/* attribute string ChildAttribute; */
NS_IMETHODIMP nsXPCTestChild4::GetChildAttribute(char **aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestChild4::SetChildAttribute(const char *aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ChildMethod (); */
NS_IMETHODIMP nsXPCTestChild4::ChildMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXPCTestChild5 */
#define NS_IXPCTESTCHILD5_IID_STR "ba3eef4e-6250-11d3-82ef-0060b0eb596f"

#define NS_IXPCTESTCHILD5_IID \
  {0xba3eef4e, 0x6250, 0x11d3, \
    { 0x82, 0xef, 0x00, 0x60, 0xb0, 0xeb, 0x59, 0x6f }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIXPCTestChild5 : public nsIXPCTestParentOne {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IXPCTESTCHILD5_IID)

  /* attribute string ChildAttribute; */
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) = 0;

  /* string ChildMethod (); */
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIXPCTestChild5, NS_IXPCTESTCHILD5_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXPCTESTCHILD5 \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute); \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXPCTESTCHILD5(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return _to GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return _to SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return _to ChildMethod(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXPCTESTCHILD5(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetChildAttribute(char **aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD SetChildAttribute(const char *aChildAttribute) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetChildAttribute(aChildAttribute); } \
  NS_SCRIPTABLE NS_IMETHOD ChildMethod(char **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChildMethod(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXPCTestChild5 : public nsIXPCTestChild5
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCTESTCHILD5

  nsXPCTestChild5();

private:
  ~nsXPCTestChild5();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXPCTestChild5, nsIXPCTestChild5)

nsXPCTestChild5::nsXPCTestChild5()
{
  /* member initializers and constructor code */
}

nsXPCTestChild5::~nsXPCTestChild5()
{
  /* destructor code */
}

/* attribute string ChildAttribute; */
NS_IMETHODIMP nsXPCTestChild5::GetChildAttribute(char **aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsXPCTestChild5::SetChildAttribute(const char *aChildAttribute)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string ChildMethod (); */
NS_IMETHODIMP nsXPCTestChild5::ChildMethod(char **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest_multiple_h__ */
