#ifndef XEQUALITY_H
#define XEQUALITY_H

#include "word.hpp"

using namespace std;
using namespace wpp::how;

//equality 等式
//equalition 方程式
//implication 蕴含
struct VLANG3_API  XEquality : CWord
{
	XEquality(XEquality &e);
	XEquality(CWord & thing);

	XEquality XEquality::operator + (CWord & w);
	XEquality XEquality::operator * (CWord & w);
};

#endif //XEQUALITY_H