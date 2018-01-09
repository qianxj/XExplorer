#include "StdAfx.h"
#include "ixoffice.hpp"
#include "xcell.h"
#include "XOfficeFrmDataSvr.hpp"

namespace Hxsoft{ namespace XFrame { namespace XOffice
{
	IXOffice::IXOffice() : m_pFrameSvr(NULL),m_pFileName(NULL)
	{
	}
	
	IXOffice::~IXOffice()
	{
		if(m_pFileName) delete m_pFileName;
	}

	int IXOffice::LoadTemplate(LPTSTR pXml,int state)
	{
		xbXml x;
		x.LoadXml(pXml);
		return  LoadTemplate(x,state);
	}

	int IXOffice::LoadTemplate(xbXml & x,int state)
	{
		if(!m_pFrameSvr) return -1;
		int nRet = -1;
		IXMLDOMElement* pElement;
		x.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			nRet = LoadTemplate(pElement,state);
			pElement->Release();
		}
		return nRet;

	}

	int IXOffice::LoadTemplate(IXMLDOMElement* pElement,int state)
	{
		if(pElement)
		{
			m_pFrameSvr->Reset();
			m_pFrameSvr->m_pSerializeSvr->LoadSheetData(NULL,pElement,m_pFrameSvr->m_pDataSvr,state);
			m_pFrameSvr->SelectSheet(0);
			m_pFrameSvr->SetFRect(m_pFrameSvr->FRect);
			::InvalidateRect(m_pFrameSvr->GetHWND(),NULL,true);
			return 1;
		}else
			return -1;
	}

	int IXOffice::Retrieve(LPTSTR bstr)
	{
		if(bstr)
		{
			for(int i=0;i<m_pFrameSvr->m_Sheets.size();i++)
			{
				XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(m_pFrameSvr->m_Sheets[i]);
				if(pSheet)
				{
					if(!pSheet->m_pSheetSvr->m_bContentLoaded)
					{
						pSheet->m_pSheetSvr->LoadTemplate(m_pFrameSvr->m_SheetsElement[i],m_pFrameSvr->m_state);
						pSheet->m_pSheetSvr->m_bContentLoaded = true;
					}
					((XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->RetrieveXmlData(bstr);
				}else
				{
					XOffice::XCell::XGridSheetEx * pSheetEx = dynamic_cast<XOffice::XCell::XGridSheetEx *>(m_pFrameSvr->m_Sheets[i]);
					if(pSheetEx)
					{
						if(!pSheetEx->m_pSheetSvr->m_bContentLoaded)
						{
							pSheetEx->m_pSheetSvr->LoadTemplate(m_pFrameSvr->m_SheetsElement[i],m_pFrameSvr->m_state);
							pSheetEx->m_pSheetSvr->m_bContentLoaded = true;
						}
						((XOffice::XCell::XGridSheetSvrEx *)pSheetEx->m_pSheetSvr)->m_pGridReport->GetGridDataSvr()->m_pContentSvr->LoadXml(bstr);
					}
				}
			}
		}
		return 1;
	}

	int IXOffice::Retrieve(xbXml& x)
	{
		if(!m_pFrameSvr) return -1;
		int nRet = -1;
		IXMLDOMElement* pElement;
		x.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			nRet = Retrieve(pElement);
			pElement->Release();
		}
		return nRet;

	}

	int IXOffice::Retrieve(IXMLDOMElement* pElement)
	{
		if(pElement)
		{
			BSTR bstr;
			pElement->get_xml(&bstr);
			if(bstr)
			{
				Retrieve(bstr);
				::SysFreeString(bstr);
			}
			return 1;
		}else
			return -1;
	}

	int IXOffice::DwUpdate(IXMLDOMElement* pElement)
	{
		xbXml x;
		x.m_pXmlDoc->appendChild(pElement,NULL);
		return DwUpdate(x);
	}

	int IXOffice::DwUpdate(xbXml& x)
	{
		if(!m_pFrameSvr) return -1;
		for(int i=0;i<m_pFrameSvr->m_Sheets.size();i++)
		{
			XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(m_pFrameSvr->m_Sheets[i]);
			if(pSheet)
				((XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->DwUpdate(x.m_pXmlDoc);
			else
			{
				XOffice::XCell::XGridSheetEx * pSheetEx = dynamic_cast<XOffice::XCell::XGridSheetEx *>(m_pFrameSvr->m_Sheets[i]);
				if(pSheetEx)
				{
					((XOffice::XCell::IGridReport *)pSheetEx->GetInterface())->DwUpdateTo(x.m_pXmlDoc);
				}
			}
		}
		return 1;
	}

	xbObject* IXOffice::GetSheet(int nIndex)
	{
		XOfficeSheet* pSheet = m_pFrameSvr->m_Sheets[nIndex];
		return pSheet->GetInterface();
	}
	int IXOffice::GetSheetCount()
	{
		return m_pFrameSvr->m_Sheets.size();
	}
	
	int IXOffice::SelectSheet(int nIndex)
	{
		m_pFrameSvr->SelectSheet(nIndex);
		m_pFrameSvr->SetFRect(m_pFrameSvr->FRect);
		::InvalidateRect(m_pFrameSvr->GetHWND(),NULL,true);
		return 1;
	}

	int IXOffice::GetSelectedSheetIndex()
	{
		for(int i=0;i<m_pFrameSvr->m_Sheets.size();i++)
		{
			if(m_pFrameSvr->m_pSheet==m_pFrameSvr->m_Sheets[i]) return i;
		}
		return -1;
	}

	int IXOffice::GetData(xbXml* px)
	{
		px->LoadXml(L"<xdoc/>");
		IXMLDOMElement * pElement = NULL;
		px->GetXmlDoc()->get_documentElement(&pElement);
		m_pFrameSvr->m_pSerializeSvr->SerializeSheetData(
			m_pFrameSvr->m_pDataSvr,pElement,px->GetXmlDoc());
		return 1;
	}

	int IXOffice::GetData(xbXml* px,int nIndex)
	{
		if(nIndex >= this->GetSheetCount()) return -1;
		XOfficeFrmDataSvr * pFrmDataSvr = dynamic_cast<XOfficeFrmDataSvr *>(m_pFrameSvr->m_pDataSvr);
		XOfficeSheet* pSheet =  pFrmDataSvr->m_pOfficeFramSvr->m_Sheets[nIndex];
		if(pSheet->m_pSheetSvr->m_bContentLoaded)
		{
			px->LoadXml(L"<sheet/>");
			IXMLDOMElement * pSheetElement;
			px->GetXmlDoc()->get_documentElement(&pSheetElement);
			pSheet->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(pSheet->m_pSheetSvr->m_pDataSvr,pSheetElement,px->GetXmlDoc());
			pSheetElement->setAttribute(L"caption",(_variant_t)pFrmDataSvr->m_pOfficeFramSvr->m_pTabSheet->m_pSheets[nIndex]);
			pSheetElement->Release();
		}
		return 1;
	}

	int IXOffice::OpenFile(wchar_t* fileName)
	{
		xbXml xml;
		xml.Load(fileName);
		LoadTemplate(xml,0);
		return 0;
	}

	int IXOffice::OpenFile()
	{
		TCHAR filter[]= _T("xoffice 文件(*.xdoc)\0*.xdoc;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("xoffice 文件(*.xdoc)\0*.xdoc\0"));

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
		ofn.lpstrDefExt = _T("xml");
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

	int  IXOffice::SaveFileAs()
	{
		if(!m_pFrameSvr) return 0;

		LPTSTR ext =_T("xdoc");
		LPTSTR filter = _T("xoffice 文件(*.xdoc)\0*.xdoc;*.xml\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[255],szPath[255];
		int	 nFilterIndex = 1;
		ZeroMemory(szFileName,255);
		ZeroMemory(szPath,255);

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = this->GetHWND();
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

	int IXOffice::SaveFile() 
	{
		if(!m_pFileName) return SaveFileAs();
		return SaveFile(m_pFileName);
	}

	int IXOffice::SaveFile(wchar_t * fileName)
	{
		xbXml xml;
		xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xdoc/>");
		
		IXMLDOMElement * pDocElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);
		m_pFrameSvr->m_pSerializeSvr->SerializeSheetData(m_pFrameSvr->m_pDataSvr,pDocElement,xml.m_pXmlDoc);
		pDocElement->Release();
		xml.m_pXmlDoc->save((_variant_t)fileName);

		return 1;
	}

}}}