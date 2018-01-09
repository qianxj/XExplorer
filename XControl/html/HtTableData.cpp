#include "HtTableData.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	CHtTableData::CHtTableData(void)
	{
		m_pAxis = new CHtTableAxis();
		m_pStyle = new CHtTableStyle();
		m_pContent = new CHtTableContent();
		m_pSpan = new CHtTableSpan();
	}

	CHtTableData::~CHtTableData(void)
	{
		if(m_pAxis)delete m_pAxis;
		if(m_pStyle)delete m_pStyle;
		if(m_pContent) delete m_pContent;
		if(m_pSpan)delete m_pSpan;
	}
	
	
	bool CHtTableData::LoadTable(IXMLDOMElement * pTableElement)
	{
		m_pAxis->SetColWidth(0,0);
		m_pAxis->SetRowHeight(0,0);
		
		VARIANT var;
		pTableElement->getAttribute(L"class",&var);
		if(var.bstrVal)
		{
			//this->m_pStyle->SetCellCssStyle(0,0,
			::VariantClear(&var);
		}

		IXMLDOMNodeList * pNodeList;
		IXMLDOMNode * pNode;
		IXMLDOMElement * pElement;
		long nLen;
		int nRowSpan = 1;
		int nColSpan = 1;
		int nSpan = 1;
		int nCol = 0;
		int nRow = 0;
		int width = 0;
		RECT sRect;
		pTableElement->selectNodes(L"COLGROUP|colgroup",&pNodeList);
		pNodeList->get_length(&nLen);
		nCol = 1;
		for(int i=0;i<nLen;i++)
		{
			pNodeList->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();

			//get span
			pElement->getAttribute(L"span",&var);
			if(var.bstrVal) 
			{
				nSpan = _ttol(var.bstrVal);
				if(nSpan < 1) nSpan = 1;
				VariantClear(&var);
			}

			//get colwidth
			pElement->getAttribute(L"width",&var);
			if(var.bstrVal) 
			{
				width = _ttol(var.bstrVal);
				if(width < 0) width = 0;
				for(int k=0;k < nSpan;k++)
				{
					m_pAxis->SetColWidth(nCol + k ,width);
				}
				VariantClear(&var);
			}

			pElement->Release();
			nCol += nSpan;
		}
		pNodeList->Release();
		
		nRow = 1;
		nCol = 1;
		pTableElement->selectSingleNode(L"TBODY|tbody",&pNode);
		if(pNode)
		{
			pNode->selectNodes(L"TR|tr",&pNodeList);
			pNode->Release();
		}else
		{
			pTableElement->selectNodes(L"TR|tr",&pNodeList);
		}
		pNodeList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			IXMLDOMNodeList * pColList;
			long nColLen;
			pNodeList->get_item(i,&pNode);
			IXMLDOMElement * pRowElement = NULL;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pRowElement);
			pNode->selectNodes(L"TD|td",&pColList);
			pNode->Release();
			pColList->get_length(&nColLen);
			nCol = 0;
			pRowElement->getAttribute(L"height",&var);
			if(var.bstrVal) 
			{
				int height = _ttol(var.bstrVal);
				if(width > 0) 
					m_pAxis->SetRowHeight(i + 1 ,height);
				VariantClear(&var);
			}
			if(pRowElement) pRowElement->Release();
			for(int j=0;j<nColLen;j++)
			{
				pColList->get_item(j,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();

				//get rowspan
				nRowSpan = 1;
				pElement->getAttribute(L"rowspan",&var);
				if(var.bstrVal) 
				{
					nRowSpan = _ttol(var.bstrVal);
					if(nRowSpan < 1) nRowSpan = 1;
					VariantClear(&var);
				}
				//get colspan
				nColSpan = 1;
				pElement->getAttribute(L"colspan",&var);
				if(var.bstrVal) 
				{
					nColSpan = _ttol(var.bstrVal);
					if(nColSpan < 1) nColSpan = 1;
					VariantClear(&var);
				}

				nCol++;
				while(m_pSpan->isSpaned(nCol,nRow,sRect))
				{
					if(sRect.top==sRect.bottom)break;
					nCol = sRect.right + 1;
				}
				if(nRowSpan > 1 || nColSpan > 1)
				{
					RECT arect;
					::SetRect(&arect,nCol,nRow,nCol + nColSpan - 1,nRow + nRowSpan - 1);
					m_pSpan->Merge(arect);
				}
				BSTR bstr;
				pElement->get_text(&bstr);
				if(bstr)
				{
					m_pContent->SetItemString(nCol,nRow,bstr);
					::SysFreeString(bstr);
				}
				pElement->getAttribute(L"style",&var);
				if(var.bstrVal) 
				{
					LPTSTR pCssStr = _tcsdup(var.bstrVal);
					XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
					delete pCssStr;
					this->GetStyle()->SetCellCssStyle(nCol,nRow,pStyle);
					VariantClear(&var);
				}
				pElement->Release();
			}
			pColList->Release();
			nRow++;
		}
		pNodeList->Release();
		return true;
	}


	bool CHtTableData::LoadTable(LPTSTR pStr)
	{
		xbXml xml;
		IXMLDOMElement * pElement;
		xml.LoadXml(pStr);
		xml.GetXmlDoc()->get_documentElement(&pElement);
		if(pElement)
		{
			LoadTable(pElement);
			pElement->Release();
		}

		return true;
	}

}}}
