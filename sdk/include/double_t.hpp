#ifndef WPP_TYPE_DOUBLE_H
#define WPP_TYPE_DOUBLE_H

namespace wpp
{

class xdouble
{
private:
	double v;
public:
	static wchar_t* toString(double v);
	static wchar_t* toStringex(float v);
	static int toInt(double v);
};

}

#endif //WPP_TYPE_DOUBLE_H

