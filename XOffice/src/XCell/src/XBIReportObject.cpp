#include "StdAfx.h"
#include "..\include\XBIReportObject.hpp"
#include "griddatasvr.hpp"
#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XBIReportObject::XBIReportObject(void):m_pElement(NULL),m_sRow(0),m_sCol(0),m_Height(0),m_Width(0),m_gRow(0)
	{
		xbObject * pObject;
		xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,L"xcell",pObject );
		
		//m_pCellSheet = new XCellSheet;
		m_pCellSheet = (XCellSheet *)pObject;

		m_pCellSheetSvr = (XCellSheetSvr *)m_pCellSheet->m_pSheetSvr;// new XCellSheetSvr;
		m_pCellSheetSvr->m_pGridFrameSvrs->FOptions &= ~ goAllowEdit;
		CGridDataSvr * pCellDataSvr = (CGridDataSvr *)m_pCellSheetSvr->m_pDataSvr;
		if(!pCellDataSvr)
		{
			pCellDataSvr = new CGridDataSvr;
			m_pCellSheetSvr->m_pDataSvr = pCellDataSvr;
		}
		::SetRect(&FRect,0,0,0,0);

		m_pCellSheetSvr->m_pGridFrameSvrs->ShowGrid(false);
		m_pCellSheetSvr->SetRowSelectorWidth(0);
		m_pCellSheetSvr->SetColHeaderHeight(0);
	}

	XBIReportObject::~XBIReportObject(void)
	{
		if(m_pElement)m_pElement->Release();
		//if(m_pCellSheetSvr)delete m_pCellSheetSvr;
		if(m_pCellSheet)delete m_pCellSheet;
	}

	int XBIReportObject::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		RECT rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = FRect;
	
		m_pCellSheetSvr->SetFRect(rect);
		m_pCellSheetSvr->m_pGridFrameSvrs->FOptions &= ~goVisibleSelection;
		m_pCellSheetSvr->DoDraw(hPaintDC,&rect);
		return 1;
	}

	int XBIReportObject::GetTextWidth(int ACol,int ARow,LPCTSTR pStr)
	{
		if(!pStr) return 0;

		HDC hdc = ::GetDC(m_pCellSheetSvr->GetHWND());
		LOGFONT	LogFont;
		HFONT hFont = NULL;
		if(m_pCellSheetSvr->m_pGridFrameSvrs->GetGridDataSvr()->GetStyleSvr()->GetFont(ACol,ARow,LogFont) == 1)
		{
			hFont = ::CreateFontIndirect(&LogFont);
			hFont = (HFONT)::SelectObject(hdc,hFont);
		}else
		{
			XUtil::Css::GetDefaultFont(LogFont);
			hFont = ::CreateFontIndirect(&LogFont);
			hFont = (HFONT)::SelectObject(hdc,hFont);
		}

		SIZE sz;
		GetTextExtentPoint32(hdc,pStr,(int)_tcslen(pStr),&sz);
		hFont = (HFONT)::SelectObject(hdc,hFont);
		DeleteObject(hFont);
		::ReleaseDC(m_pCellSheetSvr->GetHWND(),hdc);

		return sz.cx;
	}

	int XBIReportObject::ProcessRowNode(IXMLDOMNode * pNode,int &nRow,int nCol)
	{
		IXMLDOMNodeList* pNodeListEx;
		IXMLDOMNode* pNodeEx;
		IXMLDOMElement* pItemEx;

		long nItemLen;
		_variant_t v;
		CGridDataSvr * pCellDataSvr = (CGridDataSvr *)m_pCellSheetSvr->m_pDataSvr;
		pNode->selectNodes(L"item",&pNodeListEx);
		pNodeListEx->get_length(&nItemLen);
		for(int k=0;k<nItemLen;k++)
		{
			pNodeListEx->get_item(k,&pNodeEx);
			pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **)&pItemEx);

			pItemEx->getAttribute(L"value",&v);
			if(v.bstrVal)
			{
				pCellDataSvr->m_pContentSvr->SetItemString(nCol,nRow,v.bstrVal);
				int cx = GetTextWidth(nCol,nRow,v.bstrVal);
				if(cx >= pCellDataSvr->m_pAxisSvr->GetColWidth(nCol))
					pCellDataSvr->m_pAxisSvr->SetColWidth(nCol,cx + 6);
			}

			int sRow = nRow;
			if(this->ProcessRowNode(pNodeEx,nRow,nCol+1)==0)
			{
				_variant_t v1;
				pItemEx->getAttribute(L"col",&v1);
				if(v1.bstrVal && _tcsicmp(v1.bstrVal,L"0")==0 && m_gRow == 0)
					m_gRow = nRow  - 1;

				if(m_sRow > nCol)
				{
					TGridRect rt;
					rt.top = sRow;
					rt.left = nCol;
					rt.right = m_sRow;
					rt.bottom = sRow ;
					pCellDataSvr->m_pSpanSvr->Merge(rt);
				}
				nRow++;
			}
			else
			{
				TGridRect rt;
				rt.top = sRow;
				rt.left = nCol;
				rt.right = nCol;
				rt.bottom = nRow - 1;
				pCellDataSvr->m_pSpanSvr->Merge(rt);
			}

			pNodeEx->Release();
			pItemEx->Release();
		}
		pNodeListEx->Release();

		return nItemLen > 0 ? 1: 0;
	}

	int XBIReportObject::ProcessColNode(IXMLDOMNode * pNode,int nRow,int &nCol)
	{
		IXMLDOMNodeList* pNodeListEx;
		IXMLDOMNode* pNodeEx;
		IXMLDOMElement* pItemEx;

		long nItemLen;
		_variant_t v;
		CGridDataSvr * pCellDataSvr = (CGridDataSvr *)m_pCellSheetSvr->m_pDataSvr;
		pNode->selectNodes(L"item",&pNodeListEx);
		pNodeListEx->get_length(&nItemLen);
		for(int k=0;k<nItemLen;k++)
		{
			pNodeListEx->get_item(k,&pNodeEx);
			pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **)&pItemEx);

			pItemEx->getAttribute(L"value",&v);
			if(v.bstrVal)
			{
				pCellDataSvr->m_pContentSvr->SetItemString(nCol,nRow,v.bstrVal);
				int cx = GetTextWidth(nCol,nRow,v.bstrVal);
				if(cx >= pCellDataSvr->m_pAxisSvr->GetColWidth(nCol))
					pCellDataSvr->m_pAxisSvr->SetColWidth(nCol,cx + 6);
			}

			int sCol = nCol;
			if(this->ProcessColNode(pNodeEx,nRow + 1,nCol)==0)
				nCol++;
			else
			{
				TGridRect rt;
				rt.top = nRow;
				rt.left = sCol;
				rt.right = nCol - 1;
				rt.bottom = nRow;
				pCellDataSvr->m_pSpanSvr->Merge(rt);
			}

			pNodeEx->Release();
			pItemEx->Release();
		}
		pNodeListEx->Release();

		return nItemLen > 0 ? 1: 0;
	}

	int XBIReportObject::Load(IXMLDOMElement * pElement)
	{
		CGridDataSvr * pCellDataSvr = (CGridDataSvr *)m_pCellSheetSvr->m_pDataSvr;
		if(pCellDataSvr) delete pCellDataSvr;

		pCellDataSvr = new CGridDataSvr;
		m_pCellSheetSvr->m_pDataSvr = pCellDataSvr;

		pElement->AddRef();
		m_pElement =  pElement;

		_variant_t v;
		m_pElement->getAttribute(L"title",&v);

		int nRow = 1,nCol = 1;
		int sRow = 1,sCol = 1;
		int tRow,tCol;
		if(v.bstrVal)
		{
			pCellDataSvr->m_pContentSvr->SetItemString(1,1,v.bstrVal);
			sRow = 2;

			/*
			TGridRect rt;
			rt.top = 1;
			rt.left = 1;
			rt.right = 5;
			rt.bottom = 1;
			pCellDataSvr->m_pSpanSvr->Merge(rt);*/

		}

		IXMLDOMNodeList* pNodeList, * pColNodeList;
		IXMLDOMNode* pNode;
		IXMLDOMElement* pItem;
		long nLen;
		long nColLen;

		m_pElement->selectNodes(L"cols/items/item",&pColNodeList);
		pColNodeList->get_length(&nColLen);
		m_sCol = nColLen;


		m_pElement->selectNodes(L"rows/items/item",&pNodeList);
		pNodeList->get_length(&nLen);
		m_sRow = nLen;

		//process row label
		nRow = sRow + nColLen - 1;
		for(int i=0;i<nLen;i++)
		{
			pNodeList->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
			pNode->Release();

			pItem->getAttribute(L"title",&v);
			pCellDataSvr->m_pContentSvr->SetItemString(sCol + i ,nRow,v.bstrVal);

			pItem->Release();
		}

		//process row data
		m_pElement->selectSingleNode(L"rows/data",&pNode);
		if(pNode)
		{
			nRow++;
			this->ProcessRowNode(pNode,nRow,sCol);
			tRow = nRow - 1;
			pNode->Release();
		}
		pNodeList->Release();

		//process col label
		pNodeList = pColNodeList;
		nCol = sCol + nLen - 1;
		for(int i=0;i<nColLen;i++)
		{
			pNodeList->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
			pNode->Release();

			pItem->getAttribute(L"title",&v);
			if(i!=nColLen - 1)
				pCellDataSvr->m_pContentSvr->SetItemString(nCol,sRow + i,v.bstrVal);
			else
			{
				LPCTSTR pStr = pCellDataSvr->m_pContentSvr->GetItemString(nCol,sRow + i);
				TCHAR buf[255];
				_stprintf_s(buf,255,L"!#!%s!#!%s",pStr,v.bstrVal);
				pCellDataSvr->m_pContentSvr->SetItemString(nCol,sRow + i,buf);
			}

			pItem->Release();
		}

		//process col data
		m_pElement->selectSingleNode(L"cols/data",&pNode);
		if(pNode)
		{
			nCol++;
			this->ProcessColNode(pNode,sRow,nCol);
			pNode->Release();
			tCol = nCol - 1;
		}

		pNodeList->Release();

		TGridRect rt;
		rt.top = sRow;
		rt.left = sCol;
		rt.right = tCol;
		rt.bottom = tRow;

		if(m_gRow > 0) rt.bottom = m_gRow;

		m_pCellSheetSvr->m_pGridFrameSvrs->SetBorder(rt, BORDER_ALL);
		m_pCellSheetSvr->m_pGridFrameSvrs->SetAlign(rt,XUtil::Css::_center);
		m_pCellSheetSvr->m_pGridFrameSvrs->ShowGrid(false);
		m_pCellSheetSvr->SetRowSelectorWidth(0);
		m_pCellSheetSvr->SetColHeaderHeight(0);

		m_pCellSheetSvr->m_pGridFrameSvrs->FCurrent.x = sCol ;//+ nLen ;
		m_pCellSheetSvr->m_pGridFrameSvrs->FCurrent.y = sRow ;//+ nColLen ;
		m_pCellSheetSvr->m_pGridFrameSvrs->FAnchor = m_pCellSheetSvr->m_pGridFrameSvrs->FCurrent;

		m_sRow = sRow;
		m_sCol = sCol;

		m_Height =  pCellDataSvr ->m_pAxisSvr->m_pRowHeights->SumValues(0,tRow) + m_pCellSheetSvr->m_pGridFrameSvrs->FGridLineWidth * (tRow + 3) + 5;
		m_Width  =  pCellDataSvr ->m_pAxisSvr->m_pColWidths->SumValues(0,tCol) + m_pCellSheetSvr->m_pGridFrameSvrs->FGridLineWidth * ( tCol + 3) + 5;
		//this->m_pCellSheetSvr->m_pGridFrameSvrs->m_RowCount = tRow + 1;
		//this->m_pCellSheetSvr->m_pGridFrameSvrs->m_ColCount = tCol + 1;

		return 1;
	}

	int XBIReportObject::GetWidth()
	{
		return m_Width;
	}

	int XBIReportObject::GetHeight()
	{
		return m_Height;
	}

	int XBIReportObject::GetClientRect(RECT &rt)
	{
		rt = FRect;
		return 1;
	}
	int XBIReportObject::SetFRect(RECT & rc)
	{
		FRect = rc;
		return 1;
	}

}}}}
