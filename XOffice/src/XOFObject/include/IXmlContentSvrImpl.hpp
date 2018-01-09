#pragma once

#include "ixmlcontentsvr.hpp"
#include "icontentsvrimpl.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	template<class T>
	class XOFOBJECT_API IXmlContentSvrImpl :public IDBContentSvrImpl<T>
	{
	public:
		inline virtual XAPI STDMETHODIMP_(bool) Load(LPTSTR pStrFile)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->Load(pStrFile);
		}
		inline virtual XAPI STDMETHODIMP_(bool) LoadXml(LPTSTR pStrXml)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->LoadXml(pStrXml);
		}
		inline virtual XAPI STDMETHODIMP_(bool) Load(IXMLDOMElement* pElement)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->Load(pElement);
		}

		inline virtual XAPI STDMETHODIMP_(bool) SetXPath(LPTSTR pXPath)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->SetXpath(pXPath);
		}
		inline virtual XAPI STDMETHODIMP_(LPTSTR) GetXPath()
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetXPath();
		}
		inline virtual XAPI STDMETHODIMP_(bool) SetRootElement(IXMLDOMElement  * pRoot)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()-> SetRootElement(pRoot);
		}
		inline virtual XAPI STDMETHODIMP_(bool) Initial(IXMLDOMElement  * pElement)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->Initial(pElement);
		}
	public:
		inline virtual XAPI STDMETHODIMP_(IXMLDOMNode*) GetItemNode(int ACol,int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetItemNode(ACol,ARow);
		}
		inline virtual XAPI STDMETHODIMP_(IXMLDOMElement*) GetRowElement(int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetRowElement(ARow);
		}
	};
}}}