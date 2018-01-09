#ifndef WPP_WOLE_H
#define WPP_WOLE_H

#include "type.hpp"
#include "platform.hpp"

//ole support
namespace wpp
{

class wole_t
{
public:
	static type_t trans_oletype(VARTYPE vt);
public:
	static ITypeLib* load_ITypeLib(const wchar_t * nsole);
	static ITypeInfo * lookup_ITypeInfo(ITypeLib* ptl, const wchar_t* tk);
	
public:
	static void * object_trans(void * p ,int ty1, int ty2);
};

}; //namespace wpp

#endif