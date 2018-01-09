#pragma once

#include "ihttabledata.hpp"
#include "httablecontent.hpp"
#include "httablestyle.hpp"
#include "httableaxis.hpp"
#include "httablespan.hpp"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class CHtTableData :
		public IHtTableData
	{
	public:
		CHtTableData(void);
		~CHtTableData(void);
	public:
		IHtTableAxis*		m_pAxis;
		IHtTableStyle*		m_pStyle;
		IHtTableContent*	m_pContent;
		IHtTableSpan*		m_pSpan;
	public:
		virtual IHtTableAxis *		GetAxis()	{	return m_pAxis;}
		virtual IHtTableContent *	GetContent(){	return m_pContent;}
		virtual IHtTableStyle *		GetStyle()	{	return m_pStyle;}
		virtual IHtTableSpan *		GetSpan()   {   return m_pSpan;}

		virtual bool LoadTable(IXMLDOMElement * pTableElement);
		virtual bool LoadTable(LPTSTR pStr);

	};
}}}