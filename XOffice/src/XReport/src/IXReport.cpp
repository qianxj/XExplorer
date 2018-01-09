#include "StdAfx.h"
#include "IXReport.hpp"
#include "xreportsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	IXReport::IXReport(void)
	{

	}

	IXReport::~IXReport(void)
	{

	}


	int IXReport::GetRow()
	{
		return GetReportSheetSvr()->GetRow();
	}
	int IXReport::GetRowCount()
	{
		return GetReportSheetSvr()->GetRowCount();  
	}
	int IXReport::SetRow(int row)
	{
		GetReportSheetSvr()->SetRow(row);
		GetReportSheetSvr()->Redraw();
		return 0;
	}
	int IXReport::GetColumnCount()
	{
		return GetReportSheetSvr()->GetColumnCount();
	}
	LPCTSTR IXReport::GetColumnName(int col)
	{
		return GetReportSheetSvr()->GetColumnName(col);
	}
	int IXReport::GetColumnIndex(LPCTSTR pColumn)
	{
		return GetReportSheetSvr()->GetColumnIndex(pColumn);
	}

	LPCTSTR  IXReport::GetItemString(int row,LPCTSTR pColName,DWBuffer dwBuffer)
	{
		int i=0;
		for(i=0;i<GetReportSheetSvr()->GetColumnCount();i++)
		{
			//2017.8.23 if(_tcsicmp(pColName,GetReportSheetSvr()->GetColumnName(i+1))==0)break;
			if(_tcsicmp(pColName,GetReportSheetSvr()->GetColumnName(i))==0)break;
		}
		if(i >=GetReportSheetSvr()->GetColumnCount()) return NULL;
		//2017.8.23 return GetReportSheetSvr()->GetItemString(row,i + 1);
		return GetReportSheetSvr()->GetItemString(row,i);
	}
	LPCTSTR IXReport::GetItemString(int row,int col,DWBuffer dwBuffer)
	{
		return GetReportSheetSvr()->GetItemString(row,col);
	}

	bool  IXReport::SetItemString(int row,LPCTSTR pColName,LPCTSTR pData,DWBuffer dwBuffer)
	{
		int i=0;
		for(i=0;i<GetReportSheetSvr()->GetColumnCount();i++)
		{
			//2017.8.23 if(_tcsicmp(pColName,GetReportSheetSvr()->GetColumnName(i+1))==0)break;
			if(_tcsicmp(pColName,GetReportSheetSvr()->GetColumnName(i))==0)break;
		}
		if(i >=GetReportSheetSvr()->GetColumnCount()) return false;
		//2017.8.23 return GetReportSheetSvr()->SetItemString(row,i + 1,(LPTSTR)pData);
		return GetReportSheetSvr()->SetItemString(row,i,(LPTSTR)pData);

	}
	bool  IXReport::SetItemString(int row,int col,LPCTSTR pData,DWBuffer dwBuffer)
	{
		return GetReportSheetSvr()->SetItemString(row,col,(LPTSTR)pData);
	}
	int IXReport::InsertRow(int nRow)
	{
		return GetReportSheetSvr()->InsertRow(nRow);
	}
	int IXReport::DeleteRow(int nRow)
	{
		if(nRow ==0) nRow = this->GetRow();
		return GetReportSheetSvr()->DeleteRow(nRow);
	}

	int  IXReport::GetXml(BSTR &bstr)
	{
		return GetReportSheetSvr()->GetXml(bstr);
	}
	int  IXReport::GetXml(BSTR &bstr,LPCTSTR pGuid)
	{
		return GetReportSheetSvr()->GetXml(bstr,(LPTSTR)pGuid);

	}
	int IXReport::Load(IXMLDOMElement * pElement)
	{
		BSTR bstr;
		pElement->get_xml(&bstr);

		int nRet = GetReportSheetSvr()->SetXml(bstr);
		::SysFreeString(bstr);
		return nRet;
	}

	int  IXReport::LoadXml(BSTR bstr)
	{
		return GetReportSheetSvr()->SetXml(bstr);
	}

	int IXReport::SetReadOnly(bool bReadOnly)
	{
		return GetReportSheetSvr()->SetReadOnly(bReadOnly);
	}

	int IXReport::SelectRow(int nRow,bool bSelect)
	{
		return GetReportSheetSvr()->SelectRow(nRow,bSelect);
	}
	int IXReport::SelectRow(int sRow,int eRow, bool bSelect)
	{
		return GetReportSheetSvr()->SelectRow(sRow,eRow,bSelect);
	}
	int IXReport::GetNextSelectRow(int nStartRow)
	{
		return GetReportSheetSvr()->GetNextSelectRow(nStartRow);
	}
	bool IXReport::IsRowSelected(int nRow)
	{
		return GetReportSheetSvr()->IsRowSelected(nRow);
	}
	int IXReport::SetSelectBkColor(COLORREF color)
	{
		if(GetReportSheetSvr()) return  GetReportSheetSvr()->SetSelectBkColor(color);
		return 0;
	}
	int IXReport::SetSelectionMode(int nMode)
	{
		return GetReportSheetSvr()->SetSelectionMode(nMode);
	}
	int IXReport::AddObserver(xbObserver * pObserver)
	{
		return GetReportSheetSvr()->AddObserver(pObserver);
	}

	int IXReport::RemoveObserver(xbObserver * pObserver)
	{
		return GetReportSheetSvr()->RemoveObserver(pObserver);
	}
	int IXReport::ClearObserver()
	{
		return GetReportSheetSvr()->ClearObserver();
	}

	IDwView * IXReport::GetCurTable(int &nRow,int &nCol)
	{
		nRow = GetReportSheetSvr()->GetRow();
		nCol = GetReportSheetSvr()->GetCol();
		return GetCurTable();
	}
	IDwView * IXReport::GetCurTable()
	{
		return this;
	}

	int IXReport::DwUpdateTo(IXMLDOMDocument2 * pXmlDoc)
	{
		BSTR	bstr;
		GetReportSheetSvr()->GetXml(bstr);
		
		VARIANT_BOOL vRet;
		pXmlDoc->loadXML(bstr,&vRet);
		::SysFreeString(bstr);
		return 1;
	}

	int IXReport::DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc)
	{
		return DwUpdateTo(pXmlDoc);
	}

	int IXReport::Retrieve(ITrans * pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!pTrans) return -1;
		return Retrieve(pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}
	int IXReport::Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		if(!m_pTrans) return -1;
		return Retrieve(m_pTrans->GetServerUrl(), pDataUrl,pArgStr);
	}

	int IXReport::Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr)
	{
		GetReportSheetSvr()->HideEditor();

		TCHAR buf[512];
		
		if(_tcsstr(pDataUrl,_T("uriserver/")) == pDataUrl)
			pDataUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pDataUrl,_T("dev:xquery"))==pDataUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pDataUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pDataUrl);

		xbXml xmlRpt;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:GetReportSheetSvr()->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			BSTR bstr;
			xmlRpt.m_pXmlDoc->get_xml(&bstr);
			GetReportSheetSvr()->SetXml(bstr);
			::SysFreeString(bstr); 
		}

		return 1;
	}

	int IXReport::Retrieve(xbXml  &xml)
	{
		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int nRet = -1;
		if(pElement)
		{
			nRet =  Retrieve(pElement);
			pElement->Release();
		}
		return nRet;
	}

	int IXReport::Retrieve(IXMLDOMElement * pElement)
	{
		return ((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->Load(pElement);
	}

	int IXReport::SetDataObject(ITrans * pTrans,LPCTSTR pUrl)
	{
		GetReportSheetSvr()->HideEditor();

		if(!pTrans) return -1;
		return SetDataObject(pTrans->GetServerUrl(),pUrl);
	}
	int IXReport::SetDataObject(LPCTSTR pUrl)
	{
		if(!m_pTrans) return -1;
		return SetDataObject(m_pTrans->GetServerUrl(),pUrl);
	}

	int IXReport::SetDataObject(LPCTSTR pServer,LPCTSTR pUrl)
	{
		TCHAR buf[512];
		
		GetReportSheetSvr()->FCurrent.y = 0;
		GetReportSheetSvr()->FCurrent.x = 0;

		if(_tcsstr(pUrl,_T("uriserver/")) == pUrl)
			pUrl += _tcslen(_T("uriserver/"));
		if(_tcsstr(pUrl,_T("dev:xpage"))==pUrl)
			::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pUrl);
		else
			::_stprintf_s(buf,512,_T("%s"),pUrl);

		xbXml xmlRpt;
		if(xfUriLoader::FetchXml(pServer?(LPTSTR)pServer:GetReportSheetSvr()->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
		{
			IXMLDOMElement * pElement;
			xmlRpt.m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				GetReportSheetSvr()->LoadTemplate(pElement);
				pElement->Release();
				::InvalidateRect(GetReportSheetSvr()->m_pSheetCtrl->GetHWND(),NULL,false);
			}
		}else
		{

		}
		return 1;
	}

	int IXReport::SetDataObject(IXMLDOMElement * pElement)
	{
		return GetReportSheetSvr()->LoadTemplate(pElement);
	}
	
	int IXReport::SetDataObject(IXMLDOMDocument * pDoc)
	{
		IXMLDOMElement *  pElement;
		//IXMLDOMELement *  pElement;
		pDoc->get_documentElement(&pElement);
		if(pElement)
		{
			int nRet= SetDataObject(pElement);
			pElement->Release();
			return nRet;
		}
		return-1;
	}


	int IXReport::Reset()
	{
		GetReportSheetSvr()->HideEditor();
		GetReportSheetSvr()->GetReportDataSvr()->m_pContentSvr->Reset();
		return 1;
	}
	
	int	IXReport::ResetUpdateStatus()
	{
		return GetReportSheetSvr()->GetReportDataSvr()->m_pContentSvr->ResetUpdateStatus();
	}


	int IXReport::ImportFile()
	{
		TCHAR filter[]= _T("TXT 文件(*.txt)\0*.txt\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH];
		TCHAR szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("TXT 文件(*.txt)\0*.txt\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetReportSheetSvr()->m_pSheetCtrl->GetWin()->m_hWnd;
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
		ofn.lpstrDefExt = _T("txt");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if(::GetOpenFileName(&ofn))
		{
			return ImportFile(szPath);			
		}else
			return 0;
	}
	int IXReport::ImportFile(LPCTSTR pStrFile,LPCTSTR pType,bool Header)
	{
		HANDLE hFile = ::CreateFile(pStrFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		DWORD  nLen = ::GetFileSize(hFile,NULL);
		DWORD   dwSize;
		BYTE* pData = new BYTE[nLen + 2];
		ReadFile(hFile,pData,nLen,&dwSize,NULL);
		CloseHandle(hFile);
		if(nLen < 2) return 1;
		pData[nLen] = 0;
		pData[nLen + 1] = 0;
		
		LPTSTR pStr;
		bool bFree = false;
		if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
		{
			pStr = ((LPTSTR)(pData + 2));
		}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
		{
			int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
			psz[_convert]='\0';
			pStr = psz;
			bFree = true;
		}else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
		{
			int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
			psz[_convert]='\0';
			pStr = psz;
			bFree = true;
		}
		int nRet = ImportString(pStr,pType,Header);

		if(bFree && pStr) delete pStr;
		if(pData)delete pData;

		return nRet;
	}

	int IXReport::ImportString(LPCTSTR pStr,LPCTSTR pType,bool Header )
	{
		return GetReportSheetSvr()->ImportString(pStr,pType,Header);
	}

	int IXReport::Modify(LPCTSTR pStr)
	{
		return -99;
	}

	int IXReport::event_ItemFocusChanged(int nRow,int nCol)
	{
		return -99;
	}
	int IXReport::event_RowFocusChanged(int nRow,int nOldRow)
	{
		return -99;
	}
	int IXReport::event_GetFocus( )
	{
		return -99;
	}
	int IXReport::event_LoseFocus()
	{
		return -99;
	}
	int IXReport::event_ItemChanged(int nRow,int nCol,LPCTSTR pData)
	{
		return -99;
	}
	int IXReport::event_ItemChanging(int nRow,int nCol,LPCTSTR pData)
	{
		return -99;
	}
	int IXReport::GetDwType()
	{
		return DWT_GRID;
	}

	XReportSheetSvr * IXReport::GetReportSheetSvr()
	{
		return  m_pSheetSvr;
	}

	IXMLDOMElement * IXReport::GetRowElement(int nRow)
	{
		return m_pSheetSvr->GetRowElement(nRow);
	}

	int IXReport::InsertCol(int nCol,LPCTSTR pCaption,LPCTSTR pColName,int nWidth,LPCTSTR pColPath ,LPCTSTR pStyleClass,
				LPCTSTR pEditClass,LPCTSTR pFormatClass)
	{
		return m_pSheetSvr->InsertCol(nCol, pCaption, pColName,nWidth, pColPath ,pStyleClass,pEditClass, pFormatClass);
	}
	
	int IXReport::RemoveCol(int nCol)
	{
		return m_pSheetSvr->RemoveCol(nCol);
	}
	int IXReport::RemoveVCol(int nCol)
	{
		return m_pSheetSvr->RemoveVCol(nCol);
	}
	int IXReport::RemoveCol(LPCTSTR pColName)
	{
		return m_pSheetSvr->RemoveCol(pColName);
	}

	int IXReport::AddEditStyle(LPCTSTR pName, LPCTSTR pStr)
	{
		return m_pSheetSvr->AddEditStyle(pName,pStr);
	}
	int IXReport::RemoveEditStyle(LPCTSTR pName, LPCTSTR pStr)
	{
		return m_pSheetSvr->RemoveEditStyle(pName,pStr);
	}
	int IXReport::SetColEditStyle(LPCTSTR pColName,LPCTSTR pClassName)
	{
		return m_pSheetSvr->SetColEditStyle(pColName,pClassName);
	}

	int IXReport::SetDDLBData(LPCTSTR pColName,LPCTSTR pData)
	{
		return m_pSheetSvr->SetDDLBData(pColName,pData);
	}
	LPCTSTR IXReport::GetDDLBData(LPCTSTR pColName)
	{
		return m_pSheetSvr->GetDDLBData(pColName);
	}

	int IXReport::GetContentRow(int nRow)
	{
		((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->GetContentRow(nRow);
		return 1;
	}
	int IXReport::CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2)
	{
		IXMLDOMElementPtr pSchemaElement;
		vector<XOffice::XXmlContentSvr::TTreeItem>* pTreeData = NULL;
		//((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->GetTreeData(pTreeData);
		if(!pTreeData) pTreeData = new vector<XOffice::XXmlContentSvr::TTreeItem>;
		XOffice::XXmlContentSvr::TTreeItem item;
		for(int i=0;i<((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->GetRowCount();i++)
		{
			IXMLDOMElement * pElement;
			_variant_t val;
			pElement = ((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->GetRowElement(i + 1);

			IXMLDOMNode * pItem;
			pElement->selectSingleNode((BSTR)pPath,&pItem);
			if(pItem)
			{
				item.children = true;
				if(i==0)
					item.expanded = true;
				else
					item.expanded = false;
				item.nImage = nImage1;
				pItem->Release();
			}else
			{
				item.children =  false;
				if(i==0)
					item.expanded = true;
				else
					item.expanded = false;
				item.nImage = nImage2;
			}
			item.nContentCol = nCol;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			item.nLevel = 1;
			if(i>=0)
			{
				IXMLDOMNode * pNode;
				IXMLDOMNode * pParentNode;
				pElement->get_parentNode(&pParentNode);
				while(pParentNode)
				{
					item.nLevel++;
					pNode = pParentNode;
					pNode->get_parentNode(&pParentNode);
					pNode->Release();
				}
				item.nLevel--;
			}
			item.nSelectedImage = -1;
			pTreeData->push_back(item);
			pElement->Release();
		}
		((XReportDataSvr *)GetReportSheetSvr()->m_pDataSvr)->m_pContentSvr->SetTreeData(pTreeData);
		return 1;
	}
}}}}
