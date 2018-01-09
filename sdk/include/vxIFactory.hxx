/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XBase\include\vxIFactory.xdl
 */
#pragma once

#include "vxiobject.hxx"

/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
namespace Hxsoft{ namespace XFrame
{ 

/* starting interface:    vxIFactory */
#define VXIFACTORY_IID_STR "173308dc-29bc-43f7-bbcc-ddddb0a8fc16"

#define VXIFACTORY_IID \
  {0x173308dc, 0x29bc, 0x43f7, \
    { 0xbb, 0xcc, 0xdd, 0xdd, 0xb0, 0xa8, 0xfc, 0x16 }}

class XNO_VTABLE vxIFactory : public vxIObject {
 public: 

  /* LPVOID createObject (in IIDREF guid); */
  XIMETHOD_(LPVOID) createObject(xfNode * guid) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_VXIFACTORY \
  XDECL_VXIOBJECT \
  XIMETHOD_(LPVOID) createObject(xfNode * guid); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_VXIFACTORY(_to) \
  XFORWARD_VXIOBJECT(_to) \
  XIMETHOD_(LPVOID) createObject(xfNode * guid) { return _to CreateObject(guid); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_VXIFACTORY(_to) \
  XFORWARD_SAFE_VXIOBJECT(_to) \
  XIMETHOD_(LPVOID) createObject(xfNode * guid) { return !_to ? XERROR_NULL_POINTER : _to->CreateObject(guid); } 

/* Use the code below as a template for the implementation class for this interface. */
template<class T> 
class vxIFactoryImpl : public T, public vxIFactory
{
public:
  XFORWARD_VXIFACTORY(T::)
};
/* End of implementation class. */
}}
