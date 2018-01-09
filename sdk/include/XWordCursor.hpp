#pragma once

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
