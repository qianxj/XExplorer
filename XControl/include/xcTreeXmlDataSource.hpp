#pragma once
#include "xctreedatasource.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		class xbXml;
		class xcTreeCtrl;
		class XCONTROL_API xcTreeXmlDataSource: public xcTreeDataSource
		{
		public:
			xcTreeXmlDataSource(void);
		public:
			~xcTreeXmlDataSource(void);
		protected:
			vector<IXMLDOMElement *>* m_pElementsLists;
		public:
			LPTSTR	m_strXRootPath;
			LPTSTR	m_strXPath;
			LPTSTR	m_strXData;
			LPTSTR	m_strXText;
			LPTSTR	m_strXSrc;
			LPTSTR	m_strXUrl;
			LPTSTR	m_strXImage;
			LPTSTR	m_strXSelectedImage;

			LPTSTR	m_strXDataEx;
			LPTSTR	m_strXTextEx;
			LPTSTR	m_strXSrcEx;
			LPTSTR	m_strXUrlEx;
			LPTSTR	m_strXImageEx;
			LPTSTR	m_strXSelectedImageEx;
			LPTSTR	m_ChildPath;
			LPTSTR	m_strParam;
			LPTSTR	m_strExpSrc;

			bool	m_bXmlLoaded;
			xbXml*	m_pXml;
			xbXml*  m_pXmlEx;
			IXMLDOMElement* m_pXmlElement;

		protected:
			int LoadXmlFile(LPTSTR strFile);
			int LoadXmlFileEx(LPTSTR strFile);
			int LoadXmlStr(LPTSTR xmlStr);
			int LoadXmlStrEx(LPTSTR xmlStr);
		public:
			virtual int GetChildItems(HTREEITEM	pItem);
			virtual int ExpandItem(xcTreeCtrl* pTree, HTREEITEM	pItem);
			virtual bool HasChild(xcTreeCtrl* pTree, HTREEITEM	pItem);
		};
	}
}
