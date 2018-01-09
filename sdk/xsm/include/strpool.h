#pragma once

struct stritem
{
	TCHAR* text;
	stritem * next;
};

class strpool
{
public:
	strpool();
	~strpool();
private:
	int bucketsize;
	stritem ** items;
private:
	int hash(TCHAR* str);
public:
	void * addstring(TCHAR* str);
};