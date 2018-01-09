/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/html/nsIDOMHTMLFrameSetElement.idl
 */

#ifndef __gen_nsIDOMHTMLFrameSetElement_h__
#define __gen_nsIDOMHTMLFrameSetElement_h__


#ifndef __gen_nsIDOMHTMLElement_h__
#include "nsIDOMHTMLElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMHTMLFrameSetElement */
#define NS_IDOMHTMLFRAMESETELEMENT_IID_STR "a827ecd2-472d-4cf7-ae3b-21bca7e5c6aa"

#define NS_IDOMHTMLFRAMESETELEMENT_IID \
  {0xa827ecd2, 0x472d, 0x4cf7, \
    { 0xae, 0x3b, 0x21, 0xbc, 0xa7, 0xe5, 0xc6, 0xaa }}

/**
 * The nsIDOMHTMLFrameSetElement interface is the interface to a
 * [X]HTML frameset element.
 *
 * This interface is trying to follow the DOM Level 2 HTML specification:
 * http://www.w3.org/TR/DOM-Level-2-HTML/
 *
 * with changes from the work-in-progress WHATWG HTML specification:
 * http://www.whatwg.org/specs/web-apps/current-work/
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMHTMLFrameSetElement : public nsIDOMHTMLElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMHTMLFRAMESETELEMENT_IID)

  /* attribute DOMString cols; */
  NS_SCRIPTABLE NS_IMETHOD GetCols(nsAString & aCols) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetCols(const nsAString & aCols) = 0;

  /* attribute DOMString rows; */
  NS_SCRIPTABLE NS_IMETHOD GetRows(nsAString & aRows) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetRows(const nsAString & aRows) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMHTMLFrameSetElement, NS_IDOMHTMLFRAMESETELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMHTMLFRAMESETELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetCols(nsAString & aCols); \
  NS_SCRIPTABLE NS_IMETHOD SetCols(const nsAString & aCols); \
  NS_SCRIPTABLE NS_IMETHOD GetRows(nsAString & aRows); \
  NS_SCRIPTABLE NS_IMETHOD SetRows(const nsAString & aRows); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMHTMLFRAMESETELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCols(nsAString & aCols) { return _to GetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD SetCols(const nsAString & aCols) { return _to SetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD GetRows(nsAString & aRows) { return _to GetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD SetRows(const nsAString & aRows) { return _to SetRows(aRows); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMHTMLFRAMESETELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCols(nsAString & aCols) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD SetCols(const nsAString & aCols) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCols(aCols); } \
  NS_SCRIPTABLE NS_IMETHOD GetRows(nsAString & aRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRows(aRows); } \
  NS_SCRIPTABLE NS_IMETHOD SetRows(const nsAString & aRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRows(aRows); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMHTMLFrameSetElement : public nsIDOMHTMLFrameSetElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMHTMLFRAMESETELEMENT

  nsDOMHTMLFrameSetElement();

private:
  ~nsDOMHTMLFrameSetElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMHTMLFrameSetElement, nsIDOMHTMLFrameSetElement)

nsDOMHTMLFrameSetElement::nsDOMHTMLFrameSetElement()
{
  /* member initializers and constructor code */
}

nsDOMHTMLFrameSetElement::~nsDOMHTMLFrameSetElement()
{
  /* destructor code */
}

/* attribute DOMString cols; */
NS_IMETHODIMP nsDOMHTMLFrameSetElement::GetCols(nsAString & aCols)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLFrameSetElement::SetCols(const nsAString & aCols)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString rows; */
NS_IMETHODIMP nsDOMHTMLFrameSetElement::GetRows(nsAString & aRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMHTMLFrameSetElement::SetRows(const nsAString & aRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMHTMLFrameSetElement_h__ */
