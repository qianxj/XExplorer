#pragma once
#include "mdiframe.h"

class CWorkFrame :
	public CMdiFrame
{
public:
	CWorkFrame(void);
	~CWorkFrame(void);
public:
	bool EvtNotify(TEvent* pEvent,LPARAM lParam);
	bool EvtTimer(TEvent* pEvent,LPARAM lParam);

};
