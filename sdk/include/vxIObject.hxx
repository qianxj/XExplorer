/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XBase\include\vxIObject.xdl
 */
#pragma once

#include "vxiunknown.hxx"

/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
#include "unknwn.h"
#define XAPI
#define XEVENT
#define XIMETHOD virtual STDMETHODIMP
#define XIMETHOD_(x) virtual STDMETHODIMP_(x)
#define XOUTPARAM
namespace Hxsoft{ namespace XFrame
{ 

/* starting interface:    vxIObject */
#define VXIOBJECT_IID_STR "6c30b294-f55e-4f5b-affc-6f0c5d017d15"

#define VXIOBJECT_IID \
  {0x6c30b294, 0xf55e, 0x4f5b, \
    { 0xaf, 0xfc, 0x6f, 0x0c, 0x5d, 0x01, 0x7d, 0x15 }}

class XNO_VTABLE vxIObject : public IUnknown {
 public: 

  /* boolean setClassName (in LPTSTR pStrUri, in LPTSTR pStrClassName); */
  XIMETHOD_(bool) setClassName(LPTSTR pStrUri, LPTSTR pStrClassName) = 0;

  /* LPCTSTR getClassName (in LPTSTR pStrUri); */
  XIMETHOD_(LPCTSTR) getClassName(LPTSTR pStrUri) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_VXIOBJECT \
  XDECL_IUNKNOWN \
  XIMETHOD_(bool) setClassName(LPTSTR pStrUri, LPTSTR pStrClassName); \
  XIMETHOD_(LPCTSTR) getClassName(LPTSTR pStrUri); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_VXIOBJECT(_to) \
  XFORWARD_IUNKNOWN(_to) \
  XIMETHOD_(bool) setClassName(LPTSTR pStrUri, LPTSTR pStrClassName) { return _to SetClassName(pStrUri, pStrClassName); } \
  XIMETHOD_(LPCTSTR) getClassName(LPTSTR pStrUri) { return _to GetClassName(pStrUri); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_VXIOBJECT(_to) \
  XFORWARD_SAFE_IUNKNOWN(_to) \
  XIMETHOD_(bool) setClassName(LPTSTR pStrUri, LPTSTR pStrClassName) { return !_to ? XERROR_NULL_POINTER : _to->SetClassName(pStrUri, pStrClassName); } \
  XIMETHOD_(LPCTSTR) getClassName(LPTSTR pStrUri) { return !_to ? XERROR_NULL_POINTER : _to->GetClassName(pStrUri); } 

/* Use the code below as a template for the implementation class for this interface. */
template<class T> 
class vxIObjectImpl : public T, public vxIObject
{
public:
  XFORWARD_VXIOBJECT(T::)
};
/* End of implementation class. */
}}
