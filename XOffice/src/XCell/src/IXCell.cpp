#include "StdAfx.h"
#include "XCellSheetSvr.hpp"
#include "XCellSheet.hpp"
#include "GridFrameSvr.hpp"
#include "XCellSerializeSvr.hpp"
#include "ixcell.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	IXCell::IXCell():m_pFileName(NULL)
	{
	}
	
	IXCell::~IXCell()
	{
		if(m_pFileName) delete m_pFileName;
	}

	int IXCell::GetRow()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetRow();
	}
	int IXCell::GetColumn()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetCol();

	}
	int IXCell::SetRowColumn(int ARow,int ACol)
	{
		CGridFrameSvr * pGrid = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs;
		pGrid->MoveCurrent(ACol,ARow,false,false);
		return 1;

	}

	int IXCell::GetAnchorRow()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.y;
	}
	int IXCell::GetAnchorCol()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->FAnchor.x;
	}
	int IXCell::SetAnchorRowColumn(int ARow,int ACol)
	{
		CGridFrameSvr * pGrid = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs;
		
		TGridCoord tc;
		tc.x = ACol;
		tc.y = ARow;
		pGrid->MoveAnchor(tc);
		return 1;
	}

	int IXCell::GetRowCount()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetRowCount();
	}
	int IXCell::SetRowCount()
	{
		return -1;
	}

	int IXCell::GetColumnCount()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetColCount();
	}
	int IXCell::SetColumnCount()
	{
		return -1;
	}

	LPCTSTR IXCell::GetItemString(int ARow,int ACol)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemString(ACol,ARow);
	}

	int IXCell::SetItemString(int ARow,int ACol,LPTSTR pText)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemString(ACol,ARow,pText);
	}

	int IXCell::SetBorder(int nBorder)
	{
		return -1;
	}

	int IXCell::Copy()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->Copy();
	}
	int IXCell::Cut()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->Cut();
	}
	int IXCell::Paste()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->Paste();
	}

	int IXCell::GetColumnWidth(int nCol)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetColWidth(nCol);
	}
	bool IXCell::SetColumnWidth(int nCol,int nWidth)
	{
		((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->SetColWidth(nCol,nWidth);
		return true;
	}
	int IXCell::GetRowHeight(int nRow)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetRowHeight(nRow);
	}
	bool IXCell::SetRowHeight(int nRow,int nHeight)
	{
		((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->SetRowHeight(nRow,nHeight);
		return true;
	}

	int IXCell::SetRowSelectorWidth(int nWidth)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->SetRowSelectorWidth(nWidth);
	}

	int IXCell::ResetRowSelector()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->ResetRowSelector();
	}
	int IXCell::GetRowSelectorWidth()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->GetRowSelectorWidth();
	}
	int IXCell::GetRowSelectorDefaultWidth()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->GetRowSelectorDefaultWidth();
	}

	int IXCell::SetColHeaderHeight(int nHeight)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->SetColHeaderHeight(nHeight);
	}
	int IXCell::GetColHeaderDefaultHeight()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->GetColHeaderDefaultHeight();
	}
	int IXCell::ResetColHeaderHeight()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->ResetColHeaderHeight();
	}
	int IXCell::GetColHeaderHeight()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->GetColHeaderHeight();
	}

	//得到单元整体XML属性
	LPTSTR IXCell::GetCellProps(int nRow,int nCol)
	{
		PStrItem pStrItem = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(nCol,nRow);
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				BSTR bstr;
				LPTSTR pStr = NULL;
				pNode->get_xml(&bstr);
				if(bstr)
				{
					pStr = _tcsdup(bstr);
					::SysFreeString(bstr);
				}
				pNode->Release();
				if(pStr) return pStr;
			}
		}
		return NULL;
	} 
	//得到单元整体XML属性
	int IXCell::SetCellProps(int nRow,int nCol,LPCTSTR pXmlProp)
	{
		PStrItem pStrItem = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(nCol,nRow);
		bool bNew = false;
		if(!pStrItem)
		{
			pStrItem = new StrItem;
			bNew = true;

		}
		if(!pStrItem->pStrXml)
		{
			
			int nAlloc = _tcslen(pXmlProp) + 2 * _tcslen(L"<cell>") + 2;
			TCHAR * pStr = new TCHAR[nAlloc];
			_stprintf_s(pStr,nAlloc,L"<cell>%s</cell>",pXmlProp);
			
			pStrItem->pStrXml = pStr;
		}
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMElement * pRoot;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);

			xbXml x;
			x.LoadXml(pXmlProp);
			xml.m_pXmlDoc->get_documentElement(&pRoot);
			if(pNode)
			{
				pRoot->removeChild(pNode,NULL);
				pNode->Release();
			}

			x.GetXmlDoc()->get_documentElement(&pElement);
			pRoot->appendChild(pElement,NULL);
			pElement->Release();

			if(pStrItem->pStrXml) delete pStrItem->pStrXml;
			pStrItem->pStrXml = NULL;
			BSTR bstr =NULL;
			pRoot->get_xml(&bstr);
			pStrItem->pStrXml = _tcsdup(bstr);
			::SysFreeString(bstr);
			pRoot->Release();
		}
		if(bNew)
			((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemData(nCol,nRow,pStrItem);
		return 1;
	}
	//得到单元属性
	LPTSTR IXCell::GetCellProp(int nRow,int nCol,LPCTSTR pItem)
	{
		PStrItem pStrItem = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(nCol,nRow);
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute((BSTR)pItem,&var);
					pElement->Release();
					if(var.bstrVal)return _tcsdup(var.bstrVal);
				}
				pNode->Release();
			}
		}
		return NULL;
	}

	//设置单元属性
	int IXCell::SetCellProp(int nRow,int nCol,LPCTSTR pItem,LPCTSTR pProp)
	{
		PStrItem pStrItem = ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(nCol,nRow);
		if(!pStrItem) pStrItem = new StrItem;
		if(!pStrItem->pStrXml)
		{
			pStrItem->pStrXml = _tcsdup(L"<cell><cellprop/></cell>");
			((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemData(nCol,nRow,pStrItem);
		}
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);
			if(!pNode)
			{
				xml.m_pXmlDoc->selectSingleNode(L"cell",&pNode);
				if(!pNode)
				{
					IXMLDOMElement * pRoot;
					xml.m_pXmlDoc->createElement(L"cell",&pRoot);
					xml.m_pXmlDoc->appendChild(pRoot,NULL);
					xml.m_pXmlDoc->createElement(L"cellprop",&pElement);
					pRoot->appendChild(pElement,NULL);
					pElement->Release();
					pRoot->Release();
				}else
				{
					xml.m_pXmlDoc->createElement(L"cellprop",&pElement);
					pNode->appendChild(pElement,NULL);
					pElement->Release();
					pNode->Release();
				}
				xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);
			}
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					var = pProp;
					pElement->setAttribute((BSTR)pItem,var);
					pElement->Release();
					return 1;
				}
				pNode->Release();
			}
		}
		return 1;
	}

	LPTSTR IXCell::GetHtml()
	{
		return ((XCellSerializeSvr*)((XCellSheet *)m_pControl)->m_pSheetSvr->m_pSerializeSvr)
			->SerializeSheetDataToHtml(((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr);
	}

	LPTSTR IXCell::GetData()
	{
		xbXml xml;
		xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xsheet/>");

		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);
		((XCellSheet *)m_pControl)->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr,pDocElement,xml.m_pXmlDoc);
		pDocElement->Release();
		return xml.GetXml();
	}

	void * IXCell::GetSheetSvr()
	{
		return ((XCellSheet *)m_pControl)->m_pSheetSvr;
	}

	void	IXCell::SetSchmaSource(LPCTSTR pSchema)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->SetSchmaSource(pSchema);
	}
	LPCTSTR	IXCell::GetSchemaSource()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->GetSchemaSource();
	}
	void	IXCell::SetSchmaString(LPCTSTR pSchemaStr)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->SetSchmaString(pSchemaStr);
	}
	LPCTSTR	IXCell::GetSchemaString()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->GetSchemaString();
	}

	int		IXCell::GetSpanRight(int ARow,int ACol)
	{
		TGridRect	rect;
		if(((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,rect)) return  rect.right;
		return  ACol;
	}

	int		IXCell::GetSpanBottom(int ARow,int ACol)
	{
		TGridRect	rect;
		if(((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,rect)) return  rect.bottom;
		return  ARow;
	}

	int		IXCell::GetSpanLeft(int ARow,int ACol)
	{
		TGridRect	rect;
		if(((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,rect)) return  rect.left;
		return ACol;
	}
	int		IXCell::GetSpanTop(int ARow,int ACol)
	{
		TGridRect	rect;
		if(((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,rect)) return  rect.top;
		return ARow;
	}
	bool	IXCell::IsSpanCell(int ARow,int ACol)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow);
	}

	int	IXCell::GetValidRow()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetValidRow();
	}
	int	IXCell::GetValidCol(int ARow)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetValidCol(ARow);
	}
	int	IXCell::GetValidRowCol(int &ARow,int &ACol)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(ACol,ARow);
	}
	int	IXCell::GetValidCol()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetValidCol();
	}
	int IXCell::SetRunState(int runstate)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->SetRunState(runstate);
	}
	int IXCell::SetRunState()
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->SetRunState(goRunState);
	}
	int IXCell::SetReadOnly(bool bReadOnly)
	{
		return ((XCellSheetSvr *)((XCellSheet *)m_pControl)->m_pSheetSvr)->m_pGridFrameSvrs->SetReadOnly(bReadOnly);
	}

	int IXCell::LoadTemplate(IXMLDOMElement * pElement,LPTSTR strState)
	{
		DWORD state = 0;
		if(strState && _wcsicmp(strState,L"run")==0)
		{
			state |= goRunState;
			state &= ~goDesignState;
		}
		else if(strState &&  _wcsicmp(strState,L"design")==0)
		{
			state |= goDesignState;
			state &= ~goRunState;
		}
		else
		{
			state &= ~goDesignState;
			state &= ~goRunState;
			state = goNormalState;
		}
		if(((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr)
		{
			delete ((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr;
			((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr = new  CGridDataSvr();
		}

		return ((XCellSheet *)m_pControl)->m_pSheetSvr->LoadTemplate(pElement,state);
	}

	int IXCell::LoadTemplate(IXMLDOMDocument * pDoc,LPTSTR strState)
	{
		IXMLDOMElement *  pElement;
		pDoc->get_documentElement(&pElement);
		
		if(pElement)
		{
			int nRet= LoadTemplate(pElement,strState);
			pElement->Release();
			return nRet;
		}
		return 0;
	}

	int IXCell::OpenFile(wchar_t* fileName)
	{
		xbXml xml;
		xml.Load(fileName);
		IXMLDOMDocument * pDoc = xml.GetXmlDoc();
		LoadTemplate(pDoc,0);
		return 0;
	}

	int IXCell::OpenFile()
	{
		TCHAR filter[]= _T("xcell 文件(*.xdoc)\0*.xdoc;*.xml;*.xsl\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("xcell 文件(*.xdoc)\0*.xdoc\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetHWND();
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = szCustomFilter;
		ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
		ofn.lpstrTitle = _T("文件选择");
		ofn.lpstrDefExt = _T("xdoc");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if(::GetOpenFileName(&ofn))
		{
			OpenFile(szPath);
			m_pFileName = _wcsdup(szPath);
			return 1;
		}
		return 1;
	}

	int  IXCell::SaveFileAs()
	{
		LPTSTR ext =_T("xdoc");
		LPTSTR filter = _T("xcell 文件(*.xdoc)\0*.xdoc;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[255],szPath[255];
		int	 nFilterIndex = 1;
		ZeroMemory(szFileName,255);
		ZeroMemory(szPath,255);

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetHWND();
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = 255;
		ofn.lpstrTitle = _T("另存为");
		ofn.lpstrDefExt = ext;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if(::GetSaveFileName(&ofn))
		{
			SaveFile(szPath);
			m_pFileName = _wcsdup(szPath);
			return 1;
		}
		return 0;
	}

	int IXCell::SaveFile() 
	{
		if(!m_pFileName) return SaveFileAs();
		return SaveFile(m_pFileName);
	}

	int IXCell::SaveFile(wchar_t * fileName)
	{
		xbXml xml;
		xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xsheet/>");

		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);
		((XCellSheet *)m_pControl)->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(((XCellSheet *)m_pControl)->m_pSheetSvr->m_pDataSvr,pDocElement,xml.m_pXmlDoc);
		pDocElement->Release();
		xml.m_pXmlDoc->save((_variant_t)fileName);

		return 1;
	}

	void IXCell::SetEditUpperMode(bool upper)
	{
		((CGridFrameSvr*)GetSheetSvr())->SetEditUpperMode(upper);
	}
	bool IXCell::GetEditUpperMode()
	{
		return ((CGridFrameSvr*)GetSheetSvr())->GetEditUpperMode();
	}

}}}}