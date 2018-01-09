#ifndef WPP7_TYPE_DOUBLE_H
#define WPP7_TYPE_DOUBLE_H

#include "vlang7.h"

namespace xlang
{

class VLANG_API xdouble
{
private:
	double v;
public:
	static wchar_t* toString(double v);
	static wchar_t* toStringex(float v);
	static int toInt(double v);
};

} //namespace xlang

#endif //WPP7_TYPE_DOUBLE_H

