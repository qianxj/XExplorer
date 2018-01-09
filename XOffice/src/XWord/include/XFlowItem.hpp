#pragma once
#include <vector>
#include "XWordDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	/**
		表示正常数据流排版的空间
	*/
	class XFlowItem
	{
	public:
		XFlowItem(void);
		~XFlowItem(void);
	public:
		//外部区域
		RECT FRect;
		//设置外部区域
		int SetFRect(RECT &rc);
		//获取外部区域
		const RECT & GetFRect();
	public:
		//内容区域
		RECT FContentRect;
		//设置内容区域
		int SetContentRect(RECT &rc);
		//获取内容区域
		const RECT & GetContentRect();
	public:
		//定义排除在外的矩形组
		std::vector<RECT> m_exRects;
		//增加排除矩形
		int AddExRect(RECT &rc){m_exRects.push_back(rc); return (int)m_exRects.size();}
		//清除排除矩形
		int ClearExRect();

		//定义排除在外的区域
		HRGN m_eRgn;
	public:
		//显示流空间的内容
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL)=0;
	public:
		//获取设定高度和宽度信息
		virtual POINT GetExtent()=0;
		//获取实际占用的高度和宽度信息
		virtual POINT GetRealExtent()=0;
		//重新计算位置信息
		virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC)=0;
	public:
		//直接行高度
		int m_nRowHeight;
		//行间距
		std::vector<int> m_spRowHeights;
		//行距
		std::vector<int> m_RowHeights;
		//得到行起始位置
		POINT GetRowPosition(int nRow);
	};
}}}}