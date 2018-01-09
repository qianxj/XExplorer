/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XOffice\src\XOFObject\include\IBoundSvr.idl
 */
#pragma once


#ifndef __gen_IObject_h__
#include "IObject.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
}}}
//#include "nscore.h"
//#include "prtime.h"
//#if 0
//#endif
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{

/* starting interface:    IBoundSvr */
#define IBOUNDSVR_IID_STR "02b0625a-e7f3-11d2-9f5a-006008a6efe9"

#define IBOUNDSVR_IID \
  {0x02b0625a, 0xe7f3, 0x11d2, \
    { 0x9f, 0x5a, 0x00, 0x60, 0x08, 0xa6, 0xef, 0xe9 }}

class XNO_VTABLE IBoundSvr : public IObject {
 public: 

  /* attribute RECT FRect; */
  XIMETHOD GetFRect(RECT *aFRect) = 0;
  XIMETHOD SetFRect(RECT aFRect) = 0;

  /* void DoPaint (in HDC hDC, in RECTREF rc); */
  XIMETHOD DoPaint(HDC hDC, RECT & rc) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_IBOUNDSVR \
  XDECL_IOBJECT \
  XIMETHOD GetFRect(RECT *aFRect); \
  XIMETHOD SetFRect(RECT aFRect); \
  XIMETHOD DoPaint(HDC hDC, RECT & rc); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_IOBJECT(_to) \
  XFORWARD_IOBJECT(_to) \
  XIMETHOD GetFRect(RECT *aFRect) { return _to GetFRect(aFRect); } \
  XIMETHOD SetFRect(RECT aFRect) { return _to SetFRect(aFRect); } \
  XIMETHOD DoPaint(HDC hDC, RECT & rc) { return _to DoPaint(hDC, rc); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_IOBJECT(_to) \
  XFORWARD_SAFE_IOBJECT(_to) \
  XIMETHOD GetFRect(RECT *aFRect) { return !_to ? XERROR_NULL_POINTER : _to->GetFRect(aFRect); } \
  XIMETHOD SetFRect(RECT aFRect) { return !_to ? XERROR_NULL_POINTER : _to->SetFRect(aFRect); } \
  XIMETHOD DoPaint(HDC hDC, RECT & rc) { return !_to ? XERROR_NULL_POINTER : _to->DoPaint(hDC, rc); } 

}}}
