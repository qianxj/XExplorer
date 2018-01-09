/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XOffice\src\XOFObject\include\IRowSelector.idl
 */
#pragma once


#ifndef __gen_IBoundSvr_h__
#include "IBoundSvr.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
//#endif
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{

/* starting interface:    IRowSelector */
/* namespace:             "Hxsoft.XFrame" */
/* fully qualified name:  "Hxsoft.XFrame".IRowSelector */
#define IROWSELECTOR_IID_STR "02b0625a-e7f3-11d2-9f5a-006008a6efe9"

#define IROWSELECTOR_IID \
  {0x02b0625a, 0xe7f3, 0x11d2, \
    { 0x9f, 0x5a, 0x00, 0x60, 0x08, 0xa6, 0xef, 0xe9 }}

class XNO_VTABLE IRowSelector : public IBoundSvr , public IObject {
 public: 

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_IROWSELECTOR \
  XDECL_IBOUNDSVR \
  XDECL_IOBJECT \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_IOBJECT(_to) \
  XFORWARD_IBOUNDSVR(_to) \
  XFORWARD_IOBJECT(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_IOBJECT(_to) \
  XFORWARD_SAFE_IBOUNDSVR(_to) \
  XFORWARD_SAFE_IOBJECT(_to) \
  /* no methods! */

}}}
