#pragma once

struct ChapterStyle
{
};

struct ParamStyle
{
	//param before
	//param after
	//row height
	//row before
};

struct SpanStyle
{
	//css style
};

struct BlockStyle
{

};

class XWordStyle
{
public:
	XWordStyle(void);
	~XWordStyle(void);
public:
	void * addSheetStyle();
	void * getSheetStyle(int nStyle);
public:
	void * getDocumentStyle();
	void * getChapterStyle();
	void * getParamStyle();
	void * getNormalStyle();
	void * getStyle(int nStyle);
	void * getBlockStyle(void * p);
};
