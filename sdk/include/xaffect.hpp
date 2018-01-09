#ifndef XAFFECT_H
#define XAFFECT_H

#include "word.hpp"

using namespace std;
using namespace wpp::how;

struct XThing;
struct VLANG3_API  XAffect : CWord
{
	XAffect();
	XAffect(wchar_t* _id,  XThing & _thing);
	XAffect(wchar_t* _id, XAffect & _affect);

	XAffect operator + (XAffect & w);
	XAffect operator * (XAffect & w);

	static wchar_t* getAffectId(int nItem);
	static int getAffectObject(int nItem);
};

#endif //XAFFECT_H