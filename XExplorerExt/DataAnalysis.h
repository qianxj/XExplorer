#pragma once
#include "xframe.h"
using namespace Hxsoft::XFrame;

enum BlockAreaGroup{
	groupArea=0,
	colArea=1,
	rowArea=2,
	dataArea=3,
	otherArea=4
};

class CDataAnalysis :public xfWin
{
	public:
		CDataAnalysis(void);
		~CDataAnalysis(void);
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual int OnOpen();
	public:
		WNDPROC m_PrevWndFunc;
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		int DoBlockDraw();
		int DoBlockDraw(POINT pt);
		int DoColDraw(POINT pt);
		int SetColDraw(POINT pt);
		int GetColDraw(POINT pt);
		int DoBoxDraw(HDC dc, RECT rect, LPTSTR str, bool flag=false);
	public:
		bool EvtTreeBeginDrag(TEvent* pEvent,LPARAM lParam);
	public:
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
		bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
		bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
	public:
		int m_SpaceWidth;  //���
		int m_ColWidth;    //���
		int m_ColHeight;   //�߶�
		int m_Index;       //ѡ���Ĵ���
		int m_GroupIndex;  //�ĸ���
		vector<LPTSTR> m_Blocks;  
		vector<int> m_Cols;
		vector<int> m_Rows;
		vector<int> m_Groups;
		vector<int> m_Datas;
		vector<LPTSTR> m_DataColumns;
	public:
		HTREEITEM addTreeNode(HWND hWnd, HTREEITEM pParent, LPTSTR Name, int nIndex=-1, int pImageIndex=15);
		LPTSTR GetColName();
		int GetColIndex();
		LPTSTR GetColumnName(int index);
};
