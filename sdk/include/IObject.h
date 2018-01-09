/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM d:\qianxj\vc\XArch\XOffice\src\XOFObject\include\IObject.idl
 */

#ifndef __gen_IObject_h__
#define __gen_IObject_h__
/* For IDL files that don't want to include root IDL files. */
#ifndef XNO_VTABLE
#define XNO_VTABLE
#endif
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{

/* starting interface:    IObject */
#define IOBJECT_IID_STR "02b0625a-e7f3-11d2-9f5a-006008a6efe9"

#define IOBJECT_IID \
  {0x02b0625a, 0xe7f3, 0x11d2, \
    { 0x9f, 0x5a, 0x00, 0x60, 0x08, 0xa6, 0xef, 0xe9 }}

class XNO_VTABLE IObject {
 public: 

  /* attribute LPTSTR ClassName; */
  XIMETHOD GetClassName(LPTSTR *aClassName) = 0;
  XIMETHOD SetClassName(LPTSTR aClassName) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define XDECL_IOBJECT \
  XIMETHOD GetClassName(LPTSTR *aClassName); \
  XIMETHOD SetClassName(LPTSTR aClassName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define XFORWARD_IOBJECT(_to) \
  XIMETHOD GetClassName(LPTSTR *aClassName) { return _to GetClassName(aClassName); } \
  XIMETHOD SetClassName(LPTSTR aClassName) { return _to SetClassName(aClassName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define XFORWARD_SAFE_IOBJECT(_to) \
  XIMETHOD GetClassName(LPTSTR *aClassName) { return !_to ? XERROR_NULL_POINTER : _to->GetClassName(aClassName); } \
  XIMETHOD SetClassName(LPTSTR aClassName) { return !_to ? XERROR_NULL_POINTER : _to->SetClassName(aClassName); } 

}}}

#endif /* __gen_IObject_h__ */
