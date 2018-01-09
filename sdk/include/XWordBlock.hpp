#pragma once

struct XWordBlockType
{
	static const int document = 1;
	static const int chapter = 2;
	static const int paragraph = 3;
	static const int box = 4;
	static const int text = 5;
	static const int control = 6;
	static const int page = 7;
	static const int graph = 8;
};

class XWordBlock
{
public:
	XWordBlock(int ty);
	~XWordBlock(void);
private:
	int _cssIndex;
public:
	int getCssIndex();
	void setCssIndex(int cssIndex);
private:
	int _offset;
	int _textLength;
private:
	wchar_t  * _pText;
public:
	void	setText(wchar_t* pStrText); 
	wchar_t  * getTextPtr();
	wchar_t  * getText(int & nTextLen);
private:
	bool _bBreakPage;
public:
	inline void setBreakPage(bool bBreakPage ){_bBreakPage=bBreakPage;}
	inline bool getBreakPage(){ return _bBreakPage;}
public:
	int getTextOffset();
	int getTextLength();
private: 
	int _blockType;
public:
	int getBlockType();
	void setBlockType(int ty);
public:
	XWordBlock * MergeBlock();
	XWordBlock * SplitBlock();
private:
	wchar_t  * _pStrControlClass;
	void * _pControl;
	void * _pElement;
public:
	inline wchar_t  * getControlClass(){return _pStrControlClass;}
	inline void setControlClass(wchar_t  * pStrControlClass){_pStrControlClass=pStrControlClass;} 
	inline void * getControl(){return _pControl;}
	inline void setControl(void * pControl){_pControl = pControl;}
	inline void * getControlElement(){return _pElement;}
	inline void setControlElement(void * pElement){_pElement = pElement;}
private:
	int _left;
	int _top;
	int _right;
	int _bottom;
public:
	int getLeft(){ return _left;}
	int getTop(){ return _top;}
	int getRight(){ return _right;}
	int getBottom(){ return _bottom;}
	int getWidth(){ return _right - _left;}
	int getHeight(){ return _bottom - _top;}
	void setLeft(int val)  { _left = val;}
	void setTop(int val)   { _top = val;}
	void setRight(int val) { _right = val;}
	void setBottom(int val){ _bottom = val;}
private:
	bool _bShape;
	wchar_t  * _pStrShape;
public:
	inline bool isShape() {return _bShape;}
	inline void setShape(bool bShape){_bShape = bShape;}
public:
	inline wchar_t  * getShapeRef(){return _pStrShape;}
	inline void setShapeRef(wchar_t  * pStrShape){_pStrShape = pStrShape;} 

};
