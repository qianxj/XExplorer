#ifndef  WPP_RULER_PATH_H
#define  WPP_RULER_PATH_H

#include "vlang3.h"

#include "pred.hpp"
using namespace wpp::how;

namespace wruler{

struct VLANG3_API CPath : public CWord
{
	CPath operator + (CPath & w);
	CPath operator * (CPath & w);
	CPath operator () (wchar_t ch);
};

CPath VLANG3_API operator * (wchar_t ch, CPath &);
CPath VLANG3_API operator * (CPath &, wchar_t ch);
VLANG3_API CPath & path(CPath & name, CPath& route);
VLANG3_API CPath & path(CWord & name, CWord& route);
VLANG3_API CPath & path(CPath & name, CWord& route);
CPath VLANG3_API keyword(wchar_t* kw);
CPath VLANG3_API operate(wchar_t ch);
CPath VLANG3_API operate(wchar_t* str);
CPath VLANG3_API number(int n);

} //namespace wruler

#endif //WPP_RULER_PATH_H