/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:/firefox-5.0.1/mozilla-release/dom/interfaces/svg/nsIDOMSVGGradientElement.idl
 */

#ifndef __gen_nsIDOMSVGGradientElement_h__
#define __gen_nsIDOMSVGGradientElement_h__


#ifndef __gen_nsIDOMSVGElement_h__
#include "nsIDOMSVGElement.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMSVGAnimatedEnumeration; /* forward declaration */

class nsIDOMSVGAnimatedTransformList; /* forward declaration */


/* starting interface:    nsIDOMSVGGradientElement */
#define NS_IDOMSVGGRADIENTELEMENT_IID_STR "8114186f-29ef-4642-b4e2-762f331661e5"

#define NS_IDOMSVGGRADIENTELEMENT_IID \
  {0x8114186f, 0x29ef, 0x4642, \
    { 0xb4, 0xe2, 0x76, 0x2f, 0x33, 0x16, 0x61, 0xe5 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGGradientElement : public nsIDOMSVGElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGGRADIENTELEMENT_IID)

  enum { SVG_SPREADMETHOD_UNKNOWN = 0U };

  enum { SVG_SPREADMETHOD_PAD = 1U };

  enum { SVG_SPREADMETHOD_REFLECT = 2U };

  enum { SVG_SPREADMETHOD_REPEAT = 3U };

  /* readonly attribute nsIDOMSVGAnimatedEnumeration gradientUnits; */
  NS_SCRIPTABLE NS_IMETHOD GetGradientUnits(nsIDOMSVGAnimatedEnumeration **aGradientUnits) = 0;

  /* readonly attribute nsIDOMSVGAnimatedTransformList gradientTransform; */
  NS_SCRIPTABLE NS_IMETHOD GetGradientTransform(nsIDOMSVGAnimatedTransformList **aGradientTransform) = 0;

  /* readonly attribute nsIDOMSVGAnimatedEnumeration spreadMethod; */
  NS_SCRIPTABLE NS_IMETHOD GetSpreadMethod(nsIDOMSVGAnimatedEnumeration **aSpreadMethod) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGGradientElement, NS_IDOMSVGGRADIENTELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGGRADIENTELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetGradientUnits(nsIDOMSVGAnimatedEnumeration **aGradientUnits); \
  NS_SCRIPTABLE NS_IMETHOD GetGradientTransform(nsIDOMSVGAnimatedTransformList **aGradientTransform); \
  NS_SCRIPTABLE NS_IMETHOD GetSpreadMethod(nsIDOMSVGAnimatedEnumeration **aSpreadMethod); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetGradientUnits(nsIDOMSVGAnimatedEnumeration **aGradientUnits) { return _to GetGradientUnits(aGradientUnits); } \
  NS_SCRIPTABLE NS_IMETHOD GetGradientTransform(nsIDOMSVGAnimatedTransformList **aGradientTransform) { return _to GetGradientTransform(aGradientTransform); } \
  NS_SCRIPTABLE NS_IMETHOD GetSpreadMethod(nsIDOMSVGAnimatedEnumeration **aSpreadMethod) { return _to GetSpreadMethod(aSpreadMethod); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetGradientUnits(nsIDOMSVGAnimatedEnumeration **aGradientUnits) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGradientUnits(aGradientUnits); } \
  NS_SCRIPTABLE NS_IMETHOD GetGradientTransform(nsIDOMSVGAnimatedTransformList **aGradientTransform) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetGradientTransform(aGradientTransform); } \
  NS_SCRIPTABLE NS_IMETHOD GetSpreadMethod(nsIDOMSVGAnimatedEnumeration **aSpreadMethod) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSpreadMethod(aSpreadMethod); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGGradientElement : public nsIDOMSVGGradientElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGGRADIENTELEMENT

  nsDOMSVGGradientElement();

private:
  ~nsDOMSVGGradientElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGGradientElement, nsIDOMSVGGradientElement)

nsDOMSVGGradientElement::nsDOMSVGGradientElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGGradientElement::~nsDOMSVGGradientElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMSVGAnimatedEnumeration gradientUnits; */
NS_IMETHODIMP nsDOMSVGGradientElement::GetGradientUnits(nsIDOMSVGAnimatedEnumeration **aGradientUnits)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedTransformList gradientTransform; */
NS_IMETHODIMP nsDOMSVGGradientElement::GetGradientTransform(nsIDOMSVGAnimatedTransformList **aGradientTransform)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedEnumeration spreadMethod; */
NS_IMETHODIMP nsDOMSVGGradientElement::GetSpreadMethod(nsIDOMSVGAnimatedEnumeration **aSpreadMethod)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

class nsIDOMSVGAnimatedLength; /* forward declaration */


/* starting interface:    nsIDOMSVGLinearGradientElement */
#define NS_IDOMSVGLINEARGRADIENTELEMENT_IID_STR "44e59baf-4ca5-48af-9749-69b30be11917"

#define NS_IDOMSVGLINEARGRADIENTELEMENT_IID \
  {0x44e59baf, 0x4ca5, 0x48af, \
    { 0x97, 0x49, 0x69, 0xb3, 0x0b, 0xe1, 0x19, 0x17 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGLinearGradientElement : public nsIDOMSVGGradientElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGLINEARGRADIENTELEMENT_IID)

  /* readonly attribute nsIDOMSVGAnimatedLength x1; */
  NS_SCRIPTABLE NS_IMETHOD GetX1(nsIDOMSVGAnimatedLength **aX1) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength y1; */
  NS_SCRIPTABLE NS_IMETHOD GetY1(nsIDOMSVGAnimatedLength **aY1) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength x2; */
  NS_SCRIPTABLE NS_IMETHOD GetX2(nsIDOMSVGAnimatedLength **aX2) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength y2; */
  NS_SCRIPTABLE NS_IMETHOD GetY2(nsIDOMSVGAnimatedLength **aY2) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGLinearGradientElement, NS_IDOMSVGLINEARGRADIENTELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGLINEARGRADIENTELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetX1(nsIDOMSVGAnimatedLength **aX1); \
  NS_SCRIPTABLE NS_IMETHOD GetY1(nsIDOMSVGAnimatedLength **aY1); \
  NS_SCRIPTABLE NS_IMETHOD GetX2(nsIDOMSVGAnimatedLength **aX2); \
  NS_SCRIPTABLE NS_IMETHOD GetY2(nsIDOMSVGAnimatedLength **aY2); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGLINEARGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetX1(nsIDOMSVGAnimatedLength **aX1) { return _to GetX1(aX1); } \
  NS_SCRIPTABLE NS_IMETHOD GetY1(nsIDOMSVGAnimatedLength **aY1) { return _to GetY1(aY1); } \
  NS_SCRIPTABLE NS_IMETHOD GetX2(nsIDOMSVGAnimatedLength **aX2) { return _to GetX2(aX2); } \
  NS_SCRIPTABLE NS_IMETHOD GetY2(nsIDOMSVGAnimatedLength **aY2) { return _to GetY2(aY2); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGLINEARGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetX1(nsIDOMSVGAnimatedLength **aX1) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetX1(aX1); } \
  NS_SCRIPTABLE NS_IMETHOD GetY1(nsIDOMSVGAnimatedLength **aY1) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetY1(aY1); } \
  NS_SCRIPTABLE NS_IMETHOD GetX2(nsIDOMSVGAnimatedLength **aX2) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetX2(aX2); } \
  NS_SCRIPTABLE NS_IMETHOD GetY2(nsIDOMSVGAnimatedLength **aY2) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetY2(aY2); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGLinearGradientElement : public nsIDOMSVGLinearGradientElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGLINEARGRADIENTELEMENT

  nsDOMSVGLinearGradientElement();

private:
  ~nsDOMSVGLinearGradientElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGLinearGradientElement, nsIDOMSVGLinearGradientElement)

nsDOMSVGLinearGradientElement::nsDOMSVGLinearGradientElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGLinearGradientElement::~nsDOMSVGLinearGradientElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMSVGAnimatedLength x1; */
NS_IMETHODIMP nsDOMSVGLinearGradientElement::GetX1(nsIDOMSVGAnimatedLength **aX1)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength y1; */
NS_IMETHODIMP nsDOMSVGLinearGradientElement::GetY1(nsIDOMSVGAnimatedLength **aY1)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength x2; */
NS_IMETHODIMP nsDOMSVGLinearGradientElement::GetX2(nsIDOMSVGAnimatedLength **aX2)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength y2; */
NS_IMETHODIMP nsDOMSVGLinearGradientElement::GetY2(nsIDOMSVGAnimatedLength **aY2)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

class nsIDOMSVGAnimatedLength; /* forward declaration */


/* starting interface:    nsIDOMSVGRadialGradientElement */
#define NS_IDOMSVGRADIALGRADIENTELEMENT_IID_STR "67d79c33-76b6-421a-a7ff-bb6793a4fd46"

#define NS_IDOMSVGRADIALGRADIENTELEMENT_IID \
  {0x67d79c33, 0x76b6, 0x421a, \
    { 0xa7, 0xff, 0xbb, 0x67, 0x93, 0xa4, 0xfd, 0x46 }}

/**
 * The nsIDOMSVGRadialGradientElement interface is the interface to an SVG
 * radial gradient element.
 *
 * For more information on this interface please see
 * http://www.w3.org/TR/SVG11/pservers.html
 *
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsIDOMSVGRadialGradientElement : public nsIDOMSVGGradientElement {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOMSVGRADIALGRADIENTELEMENT_IID)

  /* readonly attribute nsIDOMSVGAnimatedLength cx; */
  NS_SCRIPTABLE NS_IMETHOD GetCx(nsIDOMSVGAnimatedLength **aCx) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength cy; */
  NS_SCRIPTABLE NS_IMETHOD GetCy(nsIDOMSVGAnimatedLength **aCy) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength r; */
  NS_SCRIPTABLE NS_IMETHOD GetR(nsIDOMSVGAnimatedLength **aR) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength fx; */
  NS_SCRIPTABLE NS_IMETHOD GetFx(nsIDOMSVGAnimatedLength **aFx) = 0;

  /* readonly attribute nsIDOMSVGAnimatedLength fy; */
  NS_SCRIPTABLE NS_IMETHOD GetFy(nsIDOMSVGAnimatedLength **aFy) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIDOMSVGRadialGradientElement, NS_IDOMSVGRADIALGRADIENTELEMENT_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMSVGRADIALGRADIENTELEMENT \
  NS_SCRIPTABLE NS_IMETHOD GetCx(nsIDOMSVGAnimatedLength **aCx); \
  NS_SCRIPTABLE NS_IMETHOD GetCy(nsIDOMSVGAnimatedLength **aCy); \
  NS_SCRIPTABLE NS_IMETHOD GetR(nsIDOMSVGAnimatedLength **aR); \
  NS_SCRIPTABLE NS_IMETHOD GetFx(nsIDOMSVGAnimatedLength **aFx); \
  NS_SCRIPTABLE NS_IMETHOD GetFy(nsIDOMSVGAnimatedLength **aFy); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMSVGRADIALGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCx(nsIDOMSVGAnimatedLength **aCx) { return _to GetCx(aCx); } \
  NS_SCRIPTABLE NS_IMETHOD GetCy(nsIDOMSVGAnimatedLength **aCy) { return _to GetCy(aCy); } \
  NS_SCRIPTABLE NS_IMETHOD GetR(nsIDOMSVGAnimatedLength **aR) { return _to GetR(aR); } \
  NS_SCRIPTABLE NS_IMETHOD GetFx(nsIDOMSVGAnimatedLength **aFx) { return _to GetFx(aFx); } \
  NS_SCRIPTABLE NS_IMETHOD GetFy(nsIDOMSVGAnimatedLength **aFy) { return _to GetFy(aFy); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMSVGRADIALGRADIENTELEMENT(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCx(nsIDOMSVGAnimatedLength **aCx) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCx(aCx); } \
  NS_SCRIPTABLE NS_IMETHOD GetCy(nsIDOMSVGAnimatedLength **aCy) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCy(aCy); } \
  NS_SCRIPTABLE NS_IMETHOD GetR(nsIDOMSVGAnimatedLength **aR) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetR(aR); } \
  NS_SCRIPTABLE NS_IMETHOD GetFx(nsIDOMSVGAnimatedLength **aFx) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFx(aFx); } \
  NS_SCRIPTABLE NS_IMETHOD GetFy(nsIDOMSVGAnimatedLength **aFy) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFy(aFy); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMSVGRadialGradientElement : public nsIDOMSVGRadialGradientElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMSVGRADIALGRADIENTELEMENT

  nsDOMSVGRadialGradientElement();

private:
  ~nsDOMSVGRadialGradientElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMSVGRadialGradientElement, nsIDOMSVGRadialGradientElement)

nsDOMSVGRadialGradientElement::nsDOMSVGRadialGradientElement()
{
  /* member initializers and constructor code */
}

nsDOMSVGRadialGradientElement::~nsDOMSVGRadialGradientElement()
{
  /* destructor code */
}

/* readonly attribute nsIDOMSVGAnimatedLength cx; */
NS_IMETHODIMP nsDOMSVGRadialGradientElement::GetCx(nsIDOMSVGAnimatedLength **aCx)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength cy; */
NS_IMETHODIMP nsDOMSVGRadialGradientElement::GetCy(nsIDOMSVGAnimatedLength **aCy)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength r; */
NS_IMETHODIMP nsDOMSVGRadialGradientElement::GetR(nsIDOMSVGAnimatedLength **aR)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength fx; */
NS_IMETHODIMP nsDOMSVGRadialGradientElement::GetFx(nsIDOMSVGAnimatedLength **aFx)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMSVGAnimatedLength fy; */
NS_IMETHODIMP nsDOMSVGRadialGradientElement::GetFy(nsIDOMSVGAnimatedLength **aFy)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMSVGGradientElement_h__ */
