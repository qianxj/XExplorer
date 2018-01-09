#ifndef WPP7_TYPE_INT_H
#define WPP7_TYPE_INT_H

#include "vlang7.h"

namespace xlang
{

class VLANG_API xint
{
	int v;
public:
	static wchar_t* toString(int v);
};

} //namespace xlang

#endif //WPP7_TYPE_INT_H

