#pragma once

#include "XWordDataSvr.hpp"
#include "XWordDevice.hpp"
#include <vector>

using namespace Hxsoft::XFrame::XOffice::XWord;

class XWCodeContent
{

};

class XWCodeContentExtent : public XWCodeContent
{
public:
	XWCodeContentExtent(){
		extent.cx = 0;
		extent.cy = 0;
	}
public:
	std::vector<int> rowHeights;
	SIZE extent;
};


struct XWSelection;
class XWCodePaintContext : public XWCodeContent
{
public:
	XWCodePaintContext();
	XWCodePaintContext(XWordDevice* pDevice);
	~XWCodePaintContext();
public:
	XWordDevice* GetDevice();
	void SetDevice(XWordDevice* pDevice);
private:
	XWordDevice* pDevice_;
public:
	POINT ptPoint;
	XWordDataSvr* pDataSvr;
public:
	XWSelection* pSelection;
	int	nRow;
public:
	int piror_ext;
public:
	bool fullpage;
};

struct CalcInfoType {};
struct CalcBlockAtRow : public CalcInfoType{
	//input
	int row_;
	//out
	vord_t block_;
	CalcBlockAtRow(int row):row_(row){}
};
struct CalcBlockAtRowCol : public CalcInfoType{
	//input
	int row_;
	int col_;
	//out
	vord_t block_;
	int index;
	CalcBlockAtRowCol(int row,int col):row_(row),col_(col),index(0){}
};

struct CalcRowColAtBlock : public CalcInfoType{
	//in
	vord_t block_;
	int index_;
	//ot
	int row_;
	int col_;
	CalcRowColAtBlock(vord_t block,int charPos):row_(-1),col_(-1),block_(block),index_(charPos){}
};

struct CalcColCount: public CalcInfoType{
	int row_;
	int count_;
	CalcColCount(int row):row_(row),count_(0){}
};

class XWCodeContextInfo : public XWCodeContent
{
public:
	XWCodeContextInfo(){};
	~XWCodeContextInfo(){};
};

struct CalcRowColAtPoint : public CalcInfoType{
	//input
	POINT pt_;
	int startRow;

	//output
	int row_;
	int col_;
	//option
	int index_;
	vord_t block_;
	int rowheight_;

	CalcRowColAtPoint(POINT pt):row_(0),col_(0),pt_(pt),
		rowheight_(-1),index_(-1),startRow(-1)
	{}
};

struct CalcPointAtRowCol : public CalcInfoType
{
	//input
	int row_;
	int col_;

	int startRow;
	//output
	POINT pt_;
	//option
	int index_;
	vord_t block_;
	int rowheight_;

	CalcPointAtRowCol(int row,int col):row_(row),col_(col),
		rowheight_(-1),index_(-1),startRow(-1)
	{
		pt_.x =0;
		pt_.y =0;
	}
};


class XWCodeContextPosition : public XWCodeContent
{
public:
	XWCodeContextPosition():calcpt(false),row(0),col(0),found(false),
		pDevice(0),pDataSvr(0)
	{
		pt.x = 0;
		pt.y = 0;
	}
public:
	bool    calcpt;
	int		row;
	int		col;
	POINT	pt;
	int		index;
public:
	int		rowheight;
public:
	bool    found;
public:
	XWordDevice* pDevice;
public:
	XWordDataSvr* pDataSvr;
};