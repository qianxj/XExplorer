#ifndef XEQUALITY_H
#define XEQUALITY_H

#include "word.hpp"

using namespace std;
using namespace wpp::how;

//equality ��ʽ
//equalition ����ʽ
//implication �̺�
struct VLANG3_API  XEquality : CWord
{
	XEquality(XEquality &e);
	XEquality(CWord & thing);

	XEquality XEquality::operator + (CWord & w);
	XEquality XEquality::operator * (CWord & w);
};

#endif //XEQUALITY_H