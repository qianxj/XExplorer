#pragma once
#include "wpp4.h"

struct XWCursorInfo
{
	int nRow;
	int nCol;
	int XPosCursur;
	int YPosCursur;
	int CursorHeight;
	int nIndex;
	vord_t pBlock;
	vord_t pControl;
};

class XWordCursor
{
public:
	XWordCursor(void);
	~XWordCursor(void);
public:
	int GetLeftArrowPos();
	int GetRightArrowPos();
	int GetTopArrowPos();
	int GetBottomArrowPos();
};
