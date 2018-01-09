#ifndef WPP_WPP_H
#define WPP_WPP_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include "meta.hpp"
#include "werr.hpp"

using namespace std;
using namespace wpp;
using namespace wpp::how;

#include "ruler_base.hpp"
#include <map>

using namespace wruler;
using namespace wpp;

struct RulerType
{
	static const int wpp_parse = 1;
};

class VLANG3_API Wpp
{
public:
	Wpp();
	~Wpp();
private:
	std::map<int /*ruler_type*/, RulerSym_base* /*ruler sym*/> ruler_sym_;
public:
	void registerRuler(int /*ruler_type*/, RulerSym_base* /*ruler sym*/);
	RulerSym_base* getRuler(int ruler_type);
private:
	std::map<int /*ruler_type*/, RulerEngine_base* /*ruler sym*/> ruler_engine_;
public:
	void registerRulerEngine(int /*ruler_type*/, RulerEngine_base* /*ruler sym*/);
	RulerEngine_base* getRulerEngine(int ruler_type);

public:
	bool parse(/*wpp_context*/ void * context,wchar_t* pSrc, CWord* parseRuler = 0,bool btrace=false);
};

//

#endif //WPP_WPP_H