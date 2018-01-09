/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLLegendElement.idl
 */

#ifndef __gen_nsIDOMHTMLLegendElement_h__
#define __gen_nsIDOMHTMLLegendElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLLegendElement */
#define NS_IDOMHTMLLEGENDELEMENT_IID_STR "d84fcdd0-5962-42aa-ab83-dc92321553aa"

#define NS_IDOMHTMLLEGENDELEMENT_IID \
  {0xd84fcdd0, 0x5962, 0x42aa, \
    { 0xab, 0x83, 0xdc, 0x92, 0x32, 0x15, 0x53, 0xaa }}

/**
 * The nsIDOMHTMLLegendElement interface is the interface to a [X]HTML
 * legend element.
 *
 * This interface is trying to follow the DOM Level 2 HTML specification:
 * http://www.w3.org/TR/DOM-Level-2-HTML/
 *
 * with changes from the work-in-progress WHATWG HTML specification:
 * http://www.whatwg.org/specs/web-apps/current-work/
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLLegendElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLLEGENDELEMENT_IID)

  /* readonly attribute nsIDOMHTMLFormElement form; */
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) = 0;

  /* attribute DOMString align; */
  NS_SCRIPTABLE NS_IMETHOD GetAlign(nsAString & aAlign) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetAlign(const nsAString & aAlign) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLLegendElement, NS_IDOMHTMLLEGENDELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLLEGENDELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm); \
  NS_SCRIPTABLE NS_IMETHOD GetAlign(nsAString & aAlign); \
  NS_SCRIPTABLE NS_IMETHOD SetAlign(const nsAString & aAlign); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLLEGENDELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) { return _to GetForm(aForm); } \
  NS_SCRIPTABLE NS_IMETHOD GetAlign(nsAString & aAlign) { return _to GetAlign(aAlign); } \
  NS_SCRIPTABLE NS_IMETHOD SetAlign(const nsAString & aAlign) { return _to SetAlign(aAlign); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLLEGENDELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetForm(nsIDOMHTMLFormElement **aForm) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetForm(aForm); } \
  NS_SCRIPTABLE NS_IMETHOD GetAlign(nsAString & aAlign) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAlign(aAlign); } \
  NS_SCRIPTABLE NS_IMETHOD SetAlign(const nsAString & aAlign) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAlign(aAlign); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLLegendElement : public nsIDOMHTMLLegendElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLLEGENDELEMENT

  nsDOMHTMLLegendElement();

private:
  ~nsDOMHTMLLegendElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLLegendElement, nsIDOMHTMLLegendElement)

nsDOMHTMLLegendElement::nsDOMHTMLLegendElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLLegendElement::~nsDOMHTMLLegendElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMHTMLFormElement form; */
NS_IMETHODIMP nsDOMHTMLLegendElement::GetForm(nsIDOMHTMLFormElement **aForm)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString align; */
NS_IMETHODIMP nsDOMHTMLLegendElement::GetAlign(nsAString & aAlign)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLLegendElement::SetAlign(const nsAString & aAlign)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLLegendElement_h__ */
