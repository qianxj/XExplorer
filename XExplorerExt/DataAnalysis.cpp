#include "StdAfx.h"
#include "DataAnalysis.h"
//#include "IXAServerArg.hpp"
#include "xcell.h"
#include "IGridReport.hpp"

CDataAnalysis::CDataAnalysis(void)
{
	m_SpaceWidth = 8;
	m_ColWidth = 80;
	m_ColHeight = 25;
	m_Index = -1;
	m_GroupIndex = -1;

	//this->AttachListon(WM_PAINT,(FNEvent)&CDataAnalysis::EvtPaint);
	this->AttachListon(WM_MOUSEMOVE,(FNEvent)&CDataAnalysis::EvtMouseMove);
	this->AttachListon(WM_LBUTTONUP,(FNEvent)&CDataAnalysis::EvtLButtonUp);
	this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CDataAnalysis::EvtLButtonDown);
	this->AttachListon(WM_MOUSELEAVE,(FNEvent)&CDataAnalysis::EvtMouseLeave);

	this->AttachListon(L"col", TVN_BEGINDRAG, (FNEvent)&CDataAnalysis::EvtTreeBeginDrag);
}

CDataAnalysis::~CDataAnalysis(void)
{
	//if(m_pParam)delete (XOffice::XCell::IGridReport*)m_pParam;
	this->DetachListon();
	m_Cols.clear();
	m_Rows.clear();
	m_Groups.clear();
	m_Datas.clear();
	m_Blocks.clear();
	m_DataColumns.clear();

	xfControl * xcLabel = GetControl(_T("block"));
	if(xcLabel)
	{
		SetWindowLongPtr(xcLabel->GetHWND(),GWLP_WNDPROC,(long)m_PrevWndFunc);
		::SetProp(xcLabel->GetHWND(),L"thisex",0);
	}
}

LPTSTR CDataAnalysis::GetColumnName(int index)
{
	if(wcscmp(m_DataColumns[index],L":year")==0)	return m_DataColumns[index - 3];
	if(wcscmp(m_DataColumns[index],L":quarter")==0)return m_DataColumns[index - 2];
	if(wcscmp(m_DataColumns[index],L":month")==0)	return m_DataColumns[index - 1];
	return m_DataColumns[index];
}

int CDataAnalysis::OnXCommand(LPCTSTR pStrID, class xfControl * pControl)
{
	if(pStrID && ::_tcscmp(pStrID, L"xmOK") == 0)
	{
		xbXml xml;
		IXMLDOMElement * pDocElement;
		IXMLDOMElement * pElement =  NULL;
		IXMLDOMElement * pItemElement =  NULL;
		xml.LoadXml(_T("<mode/>"));
		xml.m_pXmlDoc->get_documentElement(&pDocElement);
		int i;
		if((int)m_Groups.size() > 0)
		{
			xml.m_pXmlDoc->createElement(L"group",&pElement);
			for(i=0; i<(int)m_Groups.size(); i++)
			{
				xml.m_pXmlDoc->createElement(L"item",&pItemElement);
				pItemElement->put_text(m_Blocks[m_Groups[i]]);
				pItemElement->setAttribute(L"column",(_variant_t)GetColumnName(m_Groups[i]));

				if(wcscmp(m_DataColumns[m_Groups[i]],L":year")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"year");
				if(wcscmp(m_DataColumns[m_Groups[i]],L":quarter")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"quarter");
				if(wcscmp(m_DataColumns[m_Groups[i]],L":month")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"month");
				pElement->appendChild(pItemElement,NULL);
			}
			pDocElement->appendChild(pElement,NULL);
		
		}
		if((int)m_Rows.size() > 0)
		{
			xml.m_pXmlDoc->createElement(L"row",&pElement);
			for(i=0; i<(int)m_Rows.size(); i++)
			{
				xml.m_pXmlDoc->createElement(L"item",&pItemElement);
				pItemElement->put_text(m_Blocks[m_Rows[i]]);
				pItemElement->setAttribute(L"column",(_variant_t)GetColumnName(m_Rows[i]));

				if(wcscmp(m_DataColumns[m_Rows[i]],L":year")==0)		
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"year");
				if(wcscmp(m_DataColumns[m_Rows[i]],L":quarter")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"quarter");
				if(wcscmp(m_DataColumns[m_Rows[i]],L":month")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"month");

				pElement->appendChild(pItemElement,NULL);
			}
			pDocElement->appendChild(pElement,NULL);
		
		}
		if((int)m_Cols.size() > 0)
		{
			xml.m_pXmlDoc->createElement(L"col",&pElement);
			for(i=0; i<(int)m_Cols.size(); i++)
			{
				xml.m_pXmlDoc->createElement(L"item",&pItemElement);
				pItemElement->put_text(m_Blocks[m_Cols[i]]);
				pItemElement->setAttribute(L"column",(_variant_t)GetColumnName(m_Cols[i]));

				if(wcscmp(m_DataColumns[m_Cols[i]],L":year")==0)		
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"year");
				if(wcscmp(m_DataColumns[m_Cols[i]],L":quarter")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"quarter");
				if(wcscmp(m_DataColumns[m_Cols[i]],L":month")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"month");

				pElement->appendChild(pItemElement,NULL);
			}
			pDocElement->appendChild(pElement,NULL);
		
		}
		if((int)m_Datas.size() > 0)
		{
			xml.m_pXmlDoc->createElement(L"data",&pElement);
			for(i=0; i<(int)m_Datas.size(); i++)
			{
				xml.m_pXmlDoc->createElement(L"item",&pItemElement);
				pItemElement->put_text(m_Blocks[m_Datas[i]]);
				pItemElement->setAttribute(L"column",(_variant_t)GetColumnName(m_Datas[i]));

				if(wcscmp(m_DataColumns[m_Datas[i]],L":year")==0)		
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"year");
				if(wcscmp(m_DataColumns[m_Datas[i]],L":quarter")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"quarter");
				if(wcscmp(m_DataColumns[m_Datas[i]],L":month")==0)	
					pItemElement->setAttribute(L"dateitem",(_variant_t)L"month");

				pElement->appendChild(pItemElement,NULL);
			}
			pDocElement->appendChild(pElement,NULL);
		
		}
		BSTR bstr;
		pDocElement->get_xml(&bstr);

		if(!m_pParam) return 0;
		XOffice::XCell::IGridReport* dw = (XOffice::XCell::IGridReport*)m_pParam;

		dw->FDataBISchema = _tcsdup(bstr);
		::SysFreeString(bstr);

		if(pItemElement)pItemElement->Release();
		if(pElement) pElement->Release();
		if(pDocElement)pDocElement->Release();

		m_dwStatus &= ~WST_RESPONSELOOP;
	}
	if(pStrID && ::_tcscmp(pStrID, L"xmClose") == 0)
	{
		m_dwStatus &= ~WST_RESPONSELOOP;
	}
	return 0;
}
HTREEITEM CDataAnalysis::addTreeNode(HWND hWnd, HTREEITEM pParent, LPTSTR Name, int nIndex, int pImageIndex)
{
	if(nIndex > -1) m_Blocks.push_back(Name);

	TVITEM tvi;
	tvi.hItem = pParent;
	tvi.mask = TVIF_PARAM|TVIF_TEXT|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	tvi.pszText =Name;
	tvi.cchTextMax = _tcslen(Name);
	tvi.stateMask = TVIS_STATEIMAGEMASK;
	tvi.iImage = pImageIndex;
	tvi.iSelectedImage = pImageIndex;
	tvi.lParam = (LPARAM)nIndex;

	TVINSERTSTRUCT tvins; 
	tvins.item = tvi;
	tvins.hParent = pParent;
	tvins.hInsertAfter = TVI_LAST;
	return TreeView_InsertItem(hWnd, &tvins);
}
int CDataAnalysis::OnOpen()
{
	xfControl * xcTree = GetControl(_T("col"));

	if(!m_pParam) return 1;
	XOffice::XCell::IGridReport* dw = (XOffice::XCell::IGridReport*)m_pParam;

	HTREEITEM rItem = addTreeNode(xcTree->GetHWND(), 0, L"维项目", -1, 17);
	HTREEITEM dItem = addTreeNode(xcTree->GetHWND(), 0, L"度量项目", -1, 16);
	int j = 0;
	for(int i=1; i<=dw->GetColumnCount(); i++)
	{
		LPTSTR name = (LPTSTR)dw->GetTitleName(i);
		if(!name || !name[0]) continue;
		std::wstring sname(name);
		//if(wcsstr(name,'(')) sname = sname.
		if(name)
		{
			//2017.8.23 m_DataColumns.push_back(_wcsdup(dw->GetColumnName(i + 1)));
			m_DataColumns.push_back(_wcsdup(dw->GetColumnName(i)));
			LPTSTR celltype = dw->GetColumnProp(i,L"celltype");
			if(celltype && _tcscmp(celltype,L"xs:decimal")==0)
			{
				addTreeNode(xcTree->GetHWND(), dItem, name, j,14);
			}else if(celltype && _tcscmp(celltype,L"xs:date")==0)
			{
				addTreeNode(xcTree->GetHWND(), rItem, name, j);
				TCHAR data[255];
				_tcsncpy_s(data,255,name,180);
				_tcscat(data,L"_月");
				j++;
				m_DataColumns.push_back(L":month");
				addTreeNode(xcTree->GetHWND(), rItem, wcsdup(data),j);

				_tcsncpy_s(data,255,name,180);
				_tcscat(data,L"_季度");
				j++;
				m_DataColumns.push_back(L":quarter");
				addTreeNode(xcTree->GetHWND(), rItem, wcsdup(data),j);

				_tcsncpy_s(data,255,name,180);
				_tcscat(data,L"_年");
				j++;
				m_DataColumns.push_back(L":year");
				addTreeNode(xcTree->GetHWND(), rItem, wcsdup(data),j);
			}else
			{
				addTreeNode(xcTree->GetHWND(), rItem, name, j);
			}
			j++;
		}
	}
	addTreeNode(xcTree->GetHWND(), dItem, L"个数", j, 14);
	m_DataColumns.push_back(L":count");
	TreeView_Expand(xcTree->GetHWND(), rItem, TVE_EXPAND);
	TreeView_Expand(xcTree->GetHWND(), dItem, TVE_EXPAND);

	xfControl * xcLabel = GetControl(_T("block"));
	m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(xcLabel->GetHWND(),GWLP_WNDPROC);
	SetWindowLongPtr(xcLabel->GetHWND(),GWLP_WNDPROC,(long)&CDataAnalysis::WindowProc);
	::SetProp(xcLabel->GetHWND(),L"thisex",(HANDLE)this);

	return 1;
}
int CDataAnalysis::DoBlockDraw(POINT pt)
{
	xfControl * xcLabel = GetControl(_T("block"));
	HWND hwnd = xcLabel->GetHWND();
	//HWND hwnd = this->m_pXPage->m_pxfNode->GetNodeByName(L"block")->GetHWND();
	RECT rect;
	::GetClientRect(hwnd, &rect);
	HDC dc = ::GetDC(hwnd);
	HDC MemDC = dc;//::CreateCompatibleDC(dc);
	//背景
	HBRUSH hbrush = ::CreateSolidBrush(RGB(235,235,255));
	::FillRect(MemDC, &rect, hbrush);
	::DeleteObject(hbrush);
	//区块大小
	int width = m_SpaceWidth*2+m_ColWidth;
	int height = m_SpaceWidth*2+m_ColHeight;
	//线
	::MoveToEx(MemDC, width, 0, NULL);
	::LineTo(MemDC, width, rect.bottom);
	::MoveToEx(MemDC, width, height, NULL);
	::LineTo(MemDC, rect.right, height);
	::MoveToEx(MemDC, 0, height*2, NULL);
	::LineTo(MemDC, rect.right, height*2);
	//字
	::SetBkMode(MemDC,TRANSPARENT);
	LOGFONT	logFont;
	logFont.lfHeight = - 20;//MulDiv(9, GetDeviceCaps(MemDC, LOGPIXELSY), 72);
	logFont.lfCharSet = DEFAULT_CHARSET ;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfEscapement = 0;
	_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
	logFont.lfItalic = 1;
	logFont.lfOrientation = 0;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfPitchAndFamily = DEFAULT_PITCH;
	logFont.lfQuality = CLEARTYPE_QUALITY;
	logFont.lfStrikeOut = 0;
	logFont.lfUnderline = 0;
	logFont.lfWidth = 0;
	logFont.lfWeight = FW_BOLD;
	HFONT hFont =::CreateFontIndirect(&logFont);
	hFont = (HFONT)::SelectObject(MemDC, hFont);
	COLORREF color = RGB(218,218,218);
	::SetTextColor(MemDC, color);
	RECT rect1;
	::SetRect(&rect1, 0, 0, width, height*2);
	::DrawText(MemDC,L"回收站",(int)::_tcslen(L"回收站"),&rect1, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	::SetRect(&rect1, 0, height*2, width, rect.bottom);
	::DrawText(MemDC,L"行 区",(int)::_tcslen(L"行 区"),&rect1, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	::SetRect(&rect1, width, 0, rect.right, height);
	::DrawText(MemDC,L"组 区",(int)::_tcslen(L"组 区"),&rect1, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	::SetRect(&rect1, width, height, rect.right, height*2);
	::DrawText(MemDC,L"列 区",(int)::_tcslen(L"列 区"),&rect1, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	::SetRect(&rect1, width, height*2, rect.right, rect.bottom);
	::DrawText(MemDC,L"数据区",(int)::_tcslen(L"数据区"),&rect1, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	hFont = (HFONT)::SelectObject(MemDC, hFont);
	::DeleteObject(hFont);
	//行 列
	color = RGB(0,0,0);
	::SetTextColor(MemDC, color);
	logFont.lfHeight = - MulDiv(9, GetDeviceCaps(MemDC, LOGPIXELSY), 72);
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	hFont =::CreateFontIndirect(&logFont);
	hFont = (HFONT)::SelectObject(MemDC, hFont);
	int i;
	for(i=0; i<(int)m_Rows.size(); i++)
	{
		::SetRect(&rect1, m_SpaceWidth, (i+1)*m_SpaceWidth+i*m_ColHeight+height*2, m_SpaceWidth+m_ColWidth, (i+1)*(m_SpaceWidth+m_ColHeight)+height*2);
		if(m_Index == i && m_GroupIndex == rowArea)
			DoBoxDraw(dc, rect1, m_Blocks[m_Rows[i]], true);
		else
			DoBoxDraw(dc, rect1, m_Blocks[m_Rows[i]]);
	}
	for(i=0; i<(int)m_Groups.size(); i++)
	{
		::SetRect(&rect1, width+i*m_ColWidth+(i+1)*m_SpaceWidth, m_SpaceWidth, width+(i+1)*(m_SpaceWidth+m_ColWidth), m_SpaceWidth+m_ColHeight);
		if(m_Index == i && m_GroupIndex == groupArea)
			DoBoxDraw(dc, rect1, m_Blocks[m_Groups[i]], true);
		else
			DoBoxDraw(dc, rect1, m_Blocks[m_Groups[i]]);
	}
	for(i=0; i<(int)m_Cols.size(); i++)
	{
		::SetRect(&rect1, width+i*m_ColWidth+(i+1)*m_SpaceWidth, m_SpaceWidth+height, width+(i+1)*(m_SpaceWidth+m_ColWidth), height*2-m_SpaceWidth);
		if(m_Index == i && m_GroupIndex == colArea)
			DoBoxDraw(dc, rect1, m_Blocks[m_Cols[i]], true);
		else
			DoBoxDraw(dc, rect1, m_Blocks[m_Cols[i]]);
	}
	for(i=0; i<(int)m_Datas.size(); i++)
	{
		::SetRect(&rect1, width+i*m_ColWidth+(i+1)*m_SpaceWidth, m_SpaceWidth+height*2, width+(i+1)*(m_SpaceWidth+m_ColWidth), height*3-m_SpaceWidth);
		if(m_Index == i && m_GroupIndex == dataArea)
			DoBoxDraw(dc, rect1, m_Blocks[m_Datas[i]], true);
		else
			DoBoxDraw(dc, rect1, m_Blocks[m_Datas[i]]);
	}
	//hFont = (HFONT)::SelectObject(MemDC, hFont);
	::DeleteObject(hFont);
	//鼠标
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(218,218,218));
	hPen = (HPEN)SelectObject(MemDC, hPen);
	color = RGB(180,180,180);
	::SetTextColor(MemDC, color);
	if(m_Index > -1 && pt.x>-1)
	{
		::SetRect(&rect1, pt.x-m_ColWidth/2, pt.y-m_ColHeight/2, pt.x+m_ColWidth/2, pt.y+m_ColHeight/2);
		DoBoxDraw(dc, rect1, GetColName());
	}
	hPen = (HPEN)SelectObject(MemDC, hPen);
	::DeleteObject(hPen);

	BitBlt(dc,0,0,rect.right,rect.bottom,MemDC,0,0,SRCCOPY);
	//::DeleteObject(MemDC);
	::ReleaseDC(hwnd, dc);
	return 1;
}
int CDataAnalysis::DoBlockDraw()
{
	POINT pt;
	pt.x=-1;
	pt.y=-1;
	return DoBlockDraw(pt);
}
int CDataAnalysis::DoBoxDraw(HDC dc, RECT rect, LPTSTR str, bool flag)
{
	HDC MemDC = dc;
	HPEN hPen;
	if(flag)
	{
		hPen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
		hPen = (HPEN)SelectObject(MemDC, hPen);
	}
	::MoveToEx(MemDC, rect.left, rect.top, NULL);
	::LineTo(MemDC, rect.left, rect.bottom);
	::LineTo(MemDC, rect.right, rect.bottom);
	::LineTo(MemDC, rect.right, rect.top);
	::LineTo(MemDC, rect.left, rect.top);
	if(str)
		::DrawText(MemDC,str,(int)::_tcslen(str),&rect, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_CENTER);
	if(flag)
	{
		hPen = (HPEN)SelectObject(MemDC, hPen);
		::DeleteObject(hPen);
	}
	return 1;
}
int CDataAnalysis::DoColDraw(POINT pt)
{
	if(m_Index > -1)
	{
		DoBlockDraw(pt);
	}
	return 1;
}
int CDataAnalysis::SetColDraw(POINT pt)
{
	if(m_Index > -1)
	{
		LPTSTR pCaption = GetColName();
		int index =  GetColIndex();

		if(!pCaption) return -1;
		int width = m_SpaceWidth*2+m_ColWidth;
		int height = m_SpaceWidth*2+m_ColHeight;
		int pGroup = -1;
		if(pt.x <= width && pt.y > height*2 && m_GroupIndex!=rowArea)//行
		{
			//m_Rows.push_back(::_tcsdup(pCaption));
			m_Rows.push_back(index);
			pGroup = rowArea;
		}
		else if(pt.x > width && pt.y <= height && m_GroupIndex!=groupArea)//组
		{
			//m_Groups.push_back(::_tcsdup(pCaption));
			m_Groups.push_back(index);
			pGroup = groupArea;
		}
		else if(pt.x > width && pt.y > height && pt.y <= height*2 && m_GroupIndex!=colArea)//列
		{
			//m_Cols.push_back(::_tcsdup(pCaption));
			m_Cols.push_back(index);
			pGroup = colArea;
		}
		else if(pt.x > width && pt.y > height*2 && m_GroupIndex!=dataArea)//数据
		{
			//m_Datas.push_back(::_tcsdup(pCaption));
			m_Datas.push_back(index);
			pGroup = dataArea;
		}
		else if(pt.x <= width && pt.y <= height*2)//回收站
		{
			pGroup = -2;
		}
		
		if(pGroup != -1 && pGroup != m_GroupIndex)
		{
			if(m_GroupIndex==groupArea)
				m_Groups.erase(m_Groups.begin()+m_Index);
			else if(m_GroupIndex==colArea)
				m_Cols.erase(m_Cols.begin()+m_Index);
			else if(m_GroupIndex==rowArea)
				m_Rows.erase(m_Rows.begin()+m_Index);
			else if(m_GroupIndex==dataArea)
				m_Datas.erase(m_Datas.begin()+m_Index);
		}
		m_Index = -1;
		DoBlockDraw();
	}
	return 1;
}
int CDataAnalysis::GetColDraw(POINT pt)
{
	m_Index = -1;
	
	int width = m_SpaceWidth*2+m_ColWidth;
	int height = m_SpaceWidth*2+m_ColHeight;
	int i;
	if(pt.x <= width && pt.y > height)//行
	{
		m_GroupIndex = rowArea;
		for(i=0; i<(int)m_Rows.size(); i++)
		{
			if(pt.x>=m_SpaceWidth && pt.x<= width-m_SpaceWidth 
				&& pt.y >= (i+1)*m_SpaceWidth+i*m_ColHeight+height*2 && pt.y <= (i+1)*(m_SpaceWidth+m_ColHeight)+height*2)
			{
				m_Index = i;
				break;
			}
		}
	}
	else if(pt.x > width && pt.y <= height)//组
	{
		m_GroupIndex = groupArea;
		for(i=0; i<(int)m_Groups.size(); i++)
		{
			if(pt.x >= width+i*m_ColWidth+(i+1)*m_SpaceWidth && pt.x <= width+(i+1)*(m_SpaceWidth+m_ColWidth)
				&& pt.y>=m_SpaceWidth && pt.y<=height-m_SpaceWidth)
			{
				m_Index = i;
				break;
			}
		}
	}
	else if(pt.x > width && pt.y > height && pt.y <= height*2)//列
	{
		m_GroupIndex = colArea;
		for(i=0; i<(int)m_Cols.size(); i++)
		{
			if(pt.x >= width+i*m_ColWidth+(i+1)*m_SpaceWidth && pt.x <= width+(i+1)*(m_SpaceWidth+m_ColWidth)
				&& pt.y>=m_SpaceWidth+height && pt.y<=height*2-m_SpaceWidth)
			{
				m_Index = i;
				break;
			}
		}
	}
	else if(pt.x > width && pt.y > height*2)//数据
	{
		m_GroupIndex = dataArea;
		for(i=0; i<(int)m_Datas.size(); i++)
		{
			if(pt.x >= width+i*m_ColWidth+(i+1)*m_SpaceWidth && pt.x <= width+(i+1)*(m_SpaceWidth+m_ColWidth)
				&& pt.y>=m_SpaceWidth+height*2 && pt.y<=height*3-m_SpaceWidth)
			{
				m_Index = i;
				break;
			}
		}
	}

	DoBlockDraw();
	
	return 1;
}
LPTSTR CDataAnalysis::GetColName()
{
	LPTSTR pCaption = NULL;
	if(m_GroupIndex==groupArea)
		pCaption = m_Blocks[m_Groups[m_Index]];
	else if(m_GroupIndex==colArea)
		pCaption = m_Blocks[m_Cols[m_Index]];
	else if(m_GroupIndex==rowArea)
		pCaption = m_Blocks[m_Rows[m_Index]];
	else if(m_GroupIndex==dataArea)
		pCaption = m_Blocks[m_Datas[m_Index]];
	else if(m_GroupIndex==otherArea)
		pCaption = m_Blocks[m_Index];
	return pCaption;
}

int CDataAnalysis::GetColIndex()
{
	if(m_GroupIndex==groupArea)
		return m_Groups[m_Index];
	else if(m_GroupIndex==colArea)
		return m_Cols[m_Index];
	else if(m_GroupIndex==rowArea)
		return m_Rows[m_Index];
	else if(m_GroupIndex==dataArea)
		return m_Datas[m_Index];
	else if(m_GroupIndex==otherArea)
		return m_Index;
	return -1;
}

bool CDataAnalysis::EvtPaint(TEvent* pEvent,LPARAM lParam)
{
	xfWin::EvtPaint(pEvent, lParam);
	DoBlockDraw();
	return true;
}
bool CDataAnalysis::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
{
	return true;
}
bool CDataAnalysis::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
{
	m_Index = -1;
	return true; 
}
bool CDataAnalysis::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
{
	return true;
}
bool CDataAnalysis::EvtMouseLeave(TEvent* pEvent,LPARAM lParam)
{
	return true;
}
LRESULT CALLBACK CDataAnalysis::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	CDataAnalysis * pThis = ((CDataAnalysis *)GetProp(hwnd,L"thisex"));
	CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
	switch(uMsg)
	{
		case WM_MOUSELEAVE:
		case WM_PAINT:
			{
				pThis->DoBlockDraw();
				break;
			}
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.y = (int)HIWORD(lParam);
				pt.x = (int)LOWORD(lParam);
				pThis->DoColDraw(pt);
				break;
			}
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.y = (int)HIWORD(lParam);
				pt.x = (int)LOWORD(lParam);
				pThis->SetColDraw(pt);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.y = (int)HIWORD(lParam);
				pt.x = (int)LOWORD(lParam);
				pThis->GetColDraw(pt);
				break;
			}
	}
	return 1;
}
bool CDataAnalysis::EvtTreeBeginDrag(TEvent* pEvent,LPARAM lParam)
{
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pEvent->lParam;
	HTREEITEM hItem = pnmtv->itemNew.hItem;
	TVITEM tvi;
	tvi.hItem = hItem;
	tvi.mask = TVIF_HANDLE|TVIF_PARAM;
	tvi.stateMask = TVIS_STATEIMAGEMASK;
	TreeView_GetItem(pnmtv->hdr.hwndFrom, &tvi);
	m_Index = tvi.lParam;
	m_GroupIndex = otherArea;
	return true;
}
