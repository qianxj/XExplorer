/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLTextAreaElement.idl
 */

#ifndef __gen_nsIDOMHTMLTextAreaElement_h__
#define __gen_nsIDOMHTMLTextAreaElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLTextAreaElement */
#define NS_IDOMHTMLTEXTAREAELEMENT_IID_STR "905edd3e-c0b3-4d54-8a2c-0eaab6ccb3cf"

#define NS_IDOMHTMLTEXTAREAELEMENT_IID \
  {0x905edd3e, 0xc0b3, 0x4d54, \
    { 0x8a, 0x2c, 0x0e, 0xaa, 0xb6, 0xcc, 0xb3, 0xcf }}

/**
 * The nsIDOMHTMLTextAreaElement interface is the interface to a
 * [X]HTML textarea element.
 *
 * This interface is trying to follow the DOM Level 2 HTML specification:
 * http://www.w3.org/TR/DOM-Level-2-HTML/
 *
 * with changes from the work-in-progress WHATWG HTML specification:
 * http://www.whatwg.org/specs/web-apps/current-work/
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLTextAreaElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLTEXTAREAELEMENT_IID)

  /* attribute DOMString defaultValue; */
  NS_SCRIPTABLE NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDefaultValue(const nsAString & aDefaultValue) = 0;

  /* readonly attribute nsIDOMHTMLFormElement form; */
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) = 0;

  /* attribute unsigned long cols; */
  NS_SCRIPTABLE NS_IMETHOD GetCols(PRUint32 *aCols) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetCols(PRUint32 aCols) = 0;

  /* attribute boolean disabled; */
  NS_SCRIPTABLE NS_IMETHOD GetDisabled(PRBool *aDisabled) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetDisabled(PRBool aDisabled) = 0;

  /* attribute DOMString name; */
  NS_SCRIPTABLE NS_IMETHOD GetName(nsAString & aName) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetName(const nsAString & aName) = 0;

  /* attribute boolean readOnly; */
  NS_SCRIPTABLE NS_IMETHOD GetReadOnly(PRBool *aReadOnly) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetReadOnly(PRBool aReadOnly) = 0;

  /* attribute unsigned long rows; */
  NS_SCRIPTABLE NS_IMETHOD GetRows(PRUint32 *aRows) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetRows(PRUint32 aRows) = 0;

  /* attribute long tabIndex; */
  NS_SCRIPTABLE NS_IMETHOD GetTabIndex(PRInt32 *aTabIndex) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetTabIndex(PRInt32 aTabIndex) = 0;

  /* readonly attribute DOMString type; */
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) = 0;

  /* attribute DOMString value; */
  NS_SCRIPTABLE NS_IMETHOD GetValue(nsAString & aValue) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetValue(const nsAString & aValue) = 0;

  /* void select (); */
  NS_SCRIPTABLE NS_IMETHOD Select(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLTextAreaElement, NS_IDOMHTMLTEXTAREAELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLTEXTAREAELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue); \
  NS_SCRIPTABLE NS_IMETHOD SetDefaultValue(const nsAString & aDefaultValue); \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm); \
  NS_SCRIPTABLE NS_IMETHOD GetCols(PRUint32 *aCols); \
  NS_SCRIPTABLE NS_IMETHOD SetCols(PRUint32 aCols); \
  NS_SCRIPTABLE NS_IMETHOD GetDisabled(PRBool *aDisabled); \
  NS_SCRIPTABLE NS_IMETHOD SetDisabled(PRBool aDisabled); \
  NS_SCRIPTABLE NS_IMETHOD GetName(nsAString & aName); \
  NS_SCRIPTABLE NS_IMETHOD SetName(const nsAString & aName); \
  NS_SCRIPTABLE NS_IMETHOD GetReadOnly(PRBool *aReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD SetReadOnly(PRBool aReadOnly); \
  NS_SCRIPTABLE NS_IMETHOD GetRows(PRUint32 *aRows); \
  NS_SCRIPTABLE NS_IMETHOD SetRows(PRUint32 aRows); \
  NS_SCRIPTABLE NS_IMETHOD GetTabIndex(PRInt32 *aTabIndex); \
  NS_SCRIPTABLE NS_IMETHOD SetTabIndex(PRInt32 aTabIndex); \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType); \
  NS_SCRIPTABLE NS_IMETHOD GetValue(nsAString & aValue); \
  NS_SCRIPTABLE NS_IMETHOD SetValue(const nsAString & aValue); \
  NS_SCRIPTABLE NS_IMETHOD Select(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLTEXTAREAELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return _to GetDefaultValue(aDefaultValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetDefaultValue(const nsAString & aDefaultValue) { return _to SetDefaultValue(aDefaultValue); } \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) { return _to GetForm(aForm); } \
  NS_SCRIPTABLE NS_IMETHOD GetCols(PRUint32 *aCols) { return _to GetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD SetCols(PRUint32 aCols) { return _to SetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD GetDisabled(PRBool *aDisabled) { return _to GetDisabled(aDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD SetDisabled(PRBool aDisabled) { return _to SetDisabled(aDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_SCRIPTABLE NS_IMETHOD SetName(const nsAString & aName) { return _to SetName(aName); } \
  NS_SCRIPTABLE NS_IMETHOD GetReadOnly(PRBool *aReadOnly) { return _to GetReadOnly(aReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD SetReadOnly(PRBool aReadOnly) { return _to SetReadOnly(aReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetRows(PRUint32 *aRows) { return _to GetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD SetRows(PRUint32 aRows) { return _to SetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD GetTabIndex(PRInt32 *aTabIndex) { return _to GetTabIndex(aTabIndex); } \
  NS_SCRIPTABLE NS_IMETHOD SetTabIndex(PRInt32 aTabIndex) { return _to SetTabIndex(aTabIndex); } \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return _to GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD GetValue(nsAString & aValue) { return _to GetValue(aValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetValue(const nsAString & aValue) { return _to SetValue(aValue); } \
  NS_SCRIPTABLE NS_IMETHOD Select(void) { return _to Select(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLTEXTAREAELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultValue(aDefaultValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetDefaultValue(const nsAString & aDefaultValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultValue(aDefaultValue); } \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetForm(aForm); } \
  NS_SCRIPTABLE NS_IMETHOD GetCols(PRUint32 *aCols) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD SetCols(PRUint32 aCols) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD GetDisabled(PRBool *aDisabled) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDisabled(aDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD SetDisabled(PRBool aDisabled) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDisabled(aDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_SCRIPTABLE NS_IMETHOD SetName(const nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetName(aName); } \
  NS_SCRIPTABLE NS_IMETHOD GetReadOnly(PRBool *aReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReadOnly(aReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD SetReadOnly(PRBool aReadOnly) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReadOnly(aReadOnly); } \
  NS_SCRIPTABLE NS_IMETHOD GetRows(PRUint32 *aRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD SetRows(PRUint32 aRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD GetTabIndex(PRInt32 *aTabIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTabIndex(aTabIndex); } \
  NS_SCRIPTABLE NS_IMETHOD SetTabIndex(PRInt32 aTabIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTabIndex(aTabIndex); } \
  NS_SCRIPTABLE NS_IMETHOD GetType(nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_SCRIPTABLE NS_IMETHOD GetValue(nsAString & aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } \
  NS_SCRIPTABLE NS_IMETHOD SetValue(const nsAString & aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetValue(aValue); } \
  NS_SCRIPTABLE NS_IMETHOD Select(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Select(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLTextAreaElement : public nsIDOMHTMLTextAreaElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLTEXTAREAELEMENT

  nsDOMHTMLTextAreaElement();

private:
  ~nsDOMHTMLTextAreaElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLTextAreaElement, nsIDOMHTMLTextAreaElement)

nsDOMHTMLTextAreaElement::nsDOMHTMLTextAreaElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLTextAreaElement::~nsDOMHTMLTextAreaElement()
{
  /* destructor code */
}

/* attribute DOMString defaultValue; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetDefaultValue(nsAString & aDefaultValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetDefaultValue(const nsAString & aDefaultValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMHTMLFormElement form; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetForm(nsIDOMHTMLFormElement **aForm)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long cols; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetCols(PRUint32 *aCols)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetCols(PRUint32 aCols)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean disabled; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetDisabled(PRBool *aDisabled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetDisabled(PRBool aDisabled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString name; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetName(const nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean readOnly; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetReadOnly(PRBool *aReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetReadOnly(PRBool aReadOnly)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long rows; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetRows(PRUint32 *aRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetRows(PRUint32 aRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long tabIndex; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetTabIndex(PRInt32 *aTabIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetTabIndex(PRInt32 aTabIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute DOMString type; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetType(nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString value; */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::GetValue(nsAString & aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLTextAreaElement::SetValue(const nsAString & aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void select (); */
NS_IMETHODIMP nsDOMHTMLTextAreaElement::Select()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLTextAreaElement_h__ */
