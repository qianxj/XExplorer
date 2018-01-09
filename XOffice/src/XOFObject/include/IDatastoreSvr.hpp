#pragma once

#include "icontentsvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API IDataStoreSvr :public IContentSvr
	{
	public:
		/*virtual XAPI STDMETHODIMP_(bool) Load(LPTSTR pStrFile) = 0;
		virtual XAPI STDMETHODIMP_(bool) LoadXml(LPTSTR pStrXml) = 0;
		virtual XAPI STDMETHODIMP_(bool) Load(IXMLDOMElement* pElement) = 0;

		virtual XAPI STDMETHODIMP_(bool) SetXPath(LPTSTR pXPath) = 0;
		virtual XAPI STDMETHODIMP_(LPTSTR) GetXPath() = 0;
		virtual XAPI STDMETHODIMP_(bool) SetRootElement(IXMLDOMElement  * pRoot) = 0;
		virtual XAPI STDMETHODIMP_(bool) Initial(IXMLDOMElement  * pElement) = 0;
	public:
		virtual XAPI STDMETHODIMP_(IXMLDOMNode*) GetItemNode(int ACol,int ARow) = 0;
		virtual XAPI STDMETHODIMP_(IXMLDOMElement*) GetRowElement(int ARow) = 0;*/
	public:
		virtual XAPI STDMETHODIMP_(bool) LoadData(unsigned char * pData) = 0;
		virtual XAPI STDMETHODIMP_(bool) AppendData(unsigned char * pData) = 0;
		virtual XAPI STDMETHODIMP_(bool) Reset() = 0;
	};
}}}