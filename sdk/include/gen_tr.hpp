#ifndef GEN_H
#define  GEN_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include <stack>

using namespace std;
using namespace wpp::how;

typedef  CWord::iterator TuringVmTape;

struct WContext;
struct VLANG3_API turingGen : public Turing<TuringVmTape>
{
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	stack<TuringTape> tapes;
#pragma warning( pop )
	WContext * contextPtr;
public:
	bool isTapeEnd(TuringTape &tape);
	bool matchValue(TuringVmTape &tape,int v);

	bool MyMatch(TuringVmTape &tape,CWord & t, CWord::iterator iter);
	int	 MyAction(TuringVmTape &tape,CWord & t, CWord::iterator & action,int &halt);
public:
	turingGen();
};

#endif //GEN_H
