#pragma once
#include "XWordLayerBase.hpp"
#include <vector>
#include <xtree>

class XWordLayerDocument;
class XWordLayerPara : public XWordLayerBase
{
public:
	XWordLayerPara(void);
	~XWordLayerPara(void);
public:
	//直接行高度
	int m_nRowHeight;
	//行间距
	vector<int> m_spRowHeights;
	//行距
	vector<int> m_RowHeights;
	//得到行起始位置
	POINT GetRowPosition(int nRow);
public:
	//定义排除在外的矩形组
	std::vector<RECT> m_exRects;
	//增加排除矩形
	int AddExRect(RECT &rc){m_exRects.push_back(rc); return (int)m_exRects.size();}
	//清除排除矩形
	int ClearExRect();
	//得到排除区域,需要合并延续下来的和页的排除区域.
	int GetExceptRects(std::vector<RECT> & exRects);

public:
	//显示流空间的内容
	virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
public:
	int DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
	int DoDraw(XWordLayerDocument * pDocument,BlockMetaInfo pBlockMetaInfo[], int nLen, HDC hPaintDC);
public:
	int DrawContent(XWordLayerDocument * pDocument, BlockMetaInfo * pBlockMetaInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
public:
	//获取设定高度和宽度信息
	//virtual SIZE GetExtent();
	//获取实际占用的高度和宽度信息
	//virtual SIZE GetRealExtent();
	//重新计算位置信息
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	//计算占用的高度和宽度信息
	SIZE ReCalcExtent(BlockMetaInfo pBlockMetaInfo[],int nLen);

public:
	//计算数据块的布局信息
	SIZE CalcBlockMetaInfo(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordBlock* pBlock,BlockMetaInfo * pBlockMetaInfo);
	//清除数据块布局对象组
	int  ClearMetaInfo(BlockMetaInfo pBlockMetaInfo[],int nCount);
private:
	//计算右边可容纳矩阵的宽度，计算时需要排除ExRect
	int GetLayerRight(BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset);
	//计算右边可容纳矩阵的矩阵，计算时需要排除ExRect
	int GetLayerRightRects(std::vector<RECT> &rRect,BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset);
	//得到相交的排除矩阵
	BOOL GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound);
	//按照从左到右排序矩形组
	int SortRects(std::vector<RECT> &vRect);
};
