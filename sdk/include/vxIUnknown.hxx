/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XBase\include\vxIUnknown.xdl
 */
#pragma once

#include "vxibase.hxx"

/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
#if 0

/* starting interface:    IUnknown */
#define IUNKNOWN_IID_STR "e52b4a3d-2f18-4907-ab7e-86c05e413f93"

#define IUNKNOWN_IID \
  {0xe52b4a3d, 0x2f18, 0x4907, \
    { 0xab, 0x7e, 0x86, 0xc0, 0x5e, 0x41, 0x3f, 0x93 }}

class XNO_VTABLE IUnknown {
 public: 

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_IUNKNOWN \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_IUNKNOWN(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_IUNKNOWN(_to) \
  /* no methods! */

/* Use the code below as a template for the implementation class for this interface. */
template<class T> 
class IUnknownImpl : public T, public IUnknown
{
public:
  XFORWARD_IUNKNOWN(T::)
};
/* End of implementation class. */
#endif
#define  XDECL_IUNKNOWN 
#define  XFORWARD_IUNKNOWN(_to) 
#define  XFORWARD_SAFE_IUNKNOWN(_to) 
