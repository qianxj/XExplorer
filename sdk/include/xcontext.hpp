#ifndef XCONTEXT_H
#define XCONTEXT_H

#include "word.hpp"

using namespace std;
using namespace wpp::how;

typedef bool (*FnInter)(void * param);
struct VLANG3_API  XContext : CWord
{
	XContext(wchar_t* _id);

	int getInterItem(CWord & w);
	int getInterType(int nItem);
	void* getInterPtr(int nItem);

	//词汇上下文环境下的解释
	bool I(CWord &w, void* param);
	//作用M
	CWord M(CWord &w, void* param);
};

#endif //XCONTEXT_H
