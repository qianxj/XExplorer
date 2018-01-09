#ifndef TYPE_H
#define TYPE_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include <stack>

using namespace std;
using namespace wpp::how;

class wType
{
public:

};

struct VLANG3_API turingType : public Turing<TuringIterator<wchar_t*>>
{
public:
	bool isTapeEnd(TuringTape &tape);
	bool matchValue(TuringTape &tape,int v);

	bool MyMatch(TuringTape &tape,CWord & t, CWord::iterator iter);
	int	 MyAction(TuringTape &tape,CWord & t, CWord::iterator & action,int &halt);
};

#endif //TYPE_H
