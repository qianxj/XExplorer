/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGAltGlyphElement.idl
 */

#ifndef __gen_nsIDOMSVGAltGlyphElement_h__
#define __gen_nsIDOMSVGAltGlyphElement_h__


#ifndef __gen_nsIDOMSVGTextPositionElem_h__
#include "nsIDOMSVGTextPositionElem.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDOMSVGAltGlyphElement */
#define NS_IDOMSVGALTGLYPHELEMENT_IID_STR "f624c6ea-ced6-404a-b8a0-5f67a4d86bce"

#define NS_IDOMSVGALTGLYPHELEMENT_IID \
  {0xf624c6ea, 0xced6, 0x404a, \
    { 0xb8, 0xa0, 0x5f, 0x67, 0xa4, 0xd8, 0x6b, 0xce }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGAltGlyphElement : public nsIDOMSVGTextPositioningElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGALTGLYPHELEMENT_IID)

  /* attribute DOMString glyphRef; */
  NS_SCRIPTABLE NS_IMETHOD GetGlyphRef(nsAString & aGlyphRef) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetGlyphRef(const nsAString & aGlyphRef) = 0;

  /* attribute DOMString format; */
  NS_SCRIPTABLE NS_IMETHOD GetFormat(nsAString & aFormat) = 0;
  NS_SCRIPTABLE NS_IMETHOD SetFormat(const nsAString & aFormat) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGAltGlyphElement, NS_IDOMSVGALTGLYPHELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGALTGLYPHELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetGlyphRef(nsAString & aGlyphRef); \
  NS_SCRIPTABLE NS_IMETHOD SetGlyphRef(const nsAString & aGlyphRef); \
  NS_SCRIPTABLE NS_IMETHOD GetFormat(nsAString & aFormat); \
  NS_SCRIPTABLE NS_IMETHOD SetFormat(const nsAString & aFormat); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGALTGLYPHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetGlyphRef(nsAString & aGlyphRef) { return _to GetGlyphRef(aGlyphRef); } \
  NS_SCRIPTABLE NS_IMETHOD SetGlyphRef(const nsAString & aGlyphRef) { return _to SetGlyphRef(aGlyphRef); } \
  NS_SCRIPTABLE NS_IMETHOD GetFormat(nsAString & aFormat) { return _to GetFormat(aFormat); } \
  NS_SCRIPTABLE NS_IMETHOD SetFormat(const nsAString & aFormat) { return _to SetFormat(aFormat); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGALTGLYPHELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetGlyphRef(nsAString & aGlyphRef) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGlyphRef(aGlyphRef); } \
  NS_SCRIPTABLE NS_IMETHOD SetGlyphRef(const nsAString & aGlyphRef) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetGlyphRef(aGlyphRef); } \
  NS_SCRIPTABLE NS_IMETHOD GetFormat(nsAString & aFormat) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFormat(aFormat); } \
  NS_SCRIPTABLE NS_IMETHOD SetFormat(const nsAString & aFormat) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFormat(aFormat); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGAltGlyphElement : public nsIDOMSVGAltGlyphElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGALTGLYPHELEMENT

  nsDOMSVGAltGlyphElement();

private:
  ~nsDOMSVGAltGlyphElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGAltGlyphElement, nsIDOMSVGAltGlyphElement)

nsDOMSVGAltGlyphElement::nsDOMSVGAltGlyphElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGAltGlyphElement::~nsDOMSVGAltGlyphElement()
{
  /* destructor code */
}

/* attribute DOMString glyphRef; */
NS_IMETHODIMP nsDOMSVGAltGlyphElement::GetGlyphRef(nsAString & aGlyphRef)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMSVGAltGlyphElement::SetGlyphRef(const nsAString & aGlyphRef)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString format; */
NS_IMETHODIMP nsDOMSVGAltGlyphElement::GetFormat(nsAString & aFormat)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMSVGAltGlyphElement::SetFormat(const nsAString & aFormat)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGAltGlyphElement_h__ */
