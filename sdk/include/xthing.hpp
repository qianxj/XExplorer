#ifndef XTHING_H
#define XTHING_H

#include "word.hpp"
#include "xaffect.hpp"

using namespace std;
using namespace wpp::how;

struct VLANG3_API XThing : CWord
{
	XThing(wchar_t * id);
	XThing( XAffect &affect);
};

#endif //XTHING_H