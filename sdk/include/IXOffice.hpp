#pragma once

#include "xbxbase.h"
#include "xofficefrmsvr.hpp"

namespace Hxsoft{ namespace XFrame { namespace XOffice
{
	class XOFFICE_API IXOffice : public IXFControl
	{
	public:
		IXOffice();
		~IXOffice();
	public:
		int LoadTemplate(LPTSTR pXml,int state);
		int LoadTemplate(IXMLDOMElement* pElement,int state);
		int LoadTemplate(xbXml& x,int state);
	public:
		int Retrieve(IXMLDOMElement* pElement);
		int Retrieve(xbXml& x);
		int Retrieve(LPTSTR bstr);
	public:
		int DwUpdate(IXMLDOMElement* pElement);
		int DwUpdate(xbXml& x);
	public:
		xbObject * GetSheet(int nIndex);
		int GetSheetCount();
	public:
		int SelectSheet(int nIndex);
		int GetSelectedSheetIndex();
		int RemoveSheet(int nIndex);
	public:
		int GetData(xbXml* px);
		int GetData(xbXml* px,int nIndex);
	public:
		int SaveFileAs();
		int SaveFile();
		int SaveFile(wchar_t * fileName);
	public:
		int OpenFile(wchar_t* fileName);
		int OpenFile();
	public:
		XOfficeFrmSvr * m_pFrameSvr;
	public:
		wchar_t* m_pFileName;
	};
}}}