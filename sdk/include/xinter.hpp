#ifndef XINTER_H
#define XINTER_H

#include "word.hpp"
#include "xthing.hpp"
#include "xaffect.hpp"
#include "xinter.hpp"

using namespace std;
using namespace wpp::how;

struct VLANG3_API  XInterType : CWord
{
	XInterType(wchar_t* _id);
	XInterType& XInterType::operator()(XInterType & ty);
};

struct VLANG3_API  XInterItem : CWord
{
	XInterItem(XInterType & ty, void* p);
};

struct VLANG3_API  XInter : CWord
{
	XInter(XThing &thing, XInterItem &item);
	XInter(XAffect &affect,XInterItem &item);
	XInter(XAffect &affect,XAffect &inter);
};

#endif XINTER_H