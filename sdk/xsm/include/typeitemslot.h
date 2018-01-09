#pragma once
#include "xvmdef.h"

class typeitemslot
{
public:
	typeitemslot();
	~typeitemslot();
private:
	typeitem** pSlot;
	int m_nSizeSlot;
	int m_nIndex;
	int m_nTop;
public:
	typeitem* asktypeitem();
	typeitem* gettypeitem(int nIndex);
};
