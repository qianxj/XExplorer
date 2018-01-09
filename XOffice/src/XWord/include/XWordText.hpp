#pragma once

class XWordText
{
public:
	XWordText(void);
	~XWordText(void);
public:
	//得到指定位置的文本
	LPCTSTR getText(int nOffset);
public:
	//插入字符
	int insertChar(wchar_t ch);
	//删除一个字符
	int deleteChar(int nOffset);
public:
	//得到指定字符从nOffset开始出现的位置
	int getCharPosition(int nOffset);
};
