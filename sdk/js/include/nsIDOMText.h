/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/core/nsIDOMText.idl
 */

#ifndef __gen_nsIDOMText_h__
#define __gen_nsIDOMText_h__


#ifndef __gen_nsIDOMCharacterData_h__
#include "nsIDOMCharacterData.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMText */
#define NS_IDOMTEXT_IID_STR "48642156-d686-46b7-8e96-35edd5b2afa8"

#define NS_IDOMTEXT_IID \
  {0x48642156, 0xd686, 0x46b7, \
    { 0x8e, 0x96, 0x35, 0xed, 0xd5, 0xb2, 0xaf, 0xa8 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMText : public nsIDOMCharacterData {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMTEXT_IID)

  /**
 * The nsIDOMText interface inherits from nsIDOMCharacterData and represents 
 * the textual content (termed character data in XML) of an Element or Attr.
 *
 * For more information on this interface please see 
 * http://www.w3.org/TR/DOM-Level-2-Core/
 */
  /* nsIDOMText splitText (in unsigned long offset)  raises (DOMException); */
  NS_SCRIPTABLE NS_IMETHOD SplitText(PRUint32 offset, nsIDOMText **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMText, NS_IDOMTEXT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMTEXT \
  NS_SCRIPTABLE NS_IMETHOD SplitText(PRUint32 offset, nsIDOMText **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMTEXT(_to) \
  NS_SCRIPTABLE NS_IMETHOD SplitText(PRUint32 offset, nsIDOMText **_retval NS_OUTPARAM) { return _to SplitText(offset, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMTEXT(_to) \
  NS_SCRIPTABLE NS_IMETHOD SplitText(PRUint32 offset, nsIDOMText **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->SplitText(offset, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMText : public nsIDOMText
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMTEXT

  nsDOMText();

private:
  ~nsDOMText();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMText, nsIDOMText)

nsDOMText::nsDOMText()
{
  /* member initializers and constructor code */
}

nsDOMText::~nsDOMText()
{
  /* destructor code */
}

/* nsIDOMText splitText (in unsigned long offset)  raises (DOMException); */
NS_IMETHODIMP nsDOMText::SplitText(PRUint32 offset, nsIDOMText **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMText_h__ */
