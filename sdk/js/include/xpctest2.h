/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/js/src/xpconnect/tests/idl/xpctest2.idl
 */

#ifndef __gen_xpctest2_h__
#define __gen_xpctest2_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsITestXPCSomeUselessThing */
#define NS_ITESTXPCSOMEUSELESSTHING_IID_STR "e8fc0860-3bb1-11d3-988a-006008962422"

#define NS_ITESTXPCSOMEUSELESSTHING_IID \
  {0xe8fc0860, 0x3bb1, 0x11d3, \
    { 0x98, 0x8a, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsITestXPCSomeUselessThing : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ITESTXPCSOMEUSELESSTHING_IID)

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsITestXPCSomeUselessThing, NS_ITESTXPCSOMEUSELESSTHING_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITESTXPCSOMEUSELESSTHING \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITESTXPCSOMEUSELESSTHING(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITESTXPCSOMEUSELESSTHING(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTestXPCSomeUselessThing : public nsITestXPCSomeUselessThing
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITESTXPCSOMEUSELESSTHING

  nsTestXPCSomeUselessThing();

private:
  ~nsTestXPCSomeUselessThing();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTestXPCSomeUselessThing, nsITestXPCSomeUselessThing)

nsTestXPCSomeUselessThing::nsTestXPCSomeUselessThing()
{
  /* member initializers and constructor code */
}

nsTestXPCSomeUselessThing::~nsTestXPCSomeUselessThing()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_xpctest2_h__ */
