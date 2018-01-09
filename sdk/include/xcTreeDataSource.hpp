#pragma once
#include "commctrl.h"

namespace Hxsoft
{
	namespace XFrame
	{
		class xcTreeCtrl;
		class XCONTROL_API xcTreeDataSource
		{
		public:
			xcTreeDataSource(void):m_hItem(NULL),m_RLevel(0),m_bLoop(false),m_pChild(NULL),m_iSelectedImage(-1),m_iImage(-1),m_strText(NULL),	m_strData(NULL){};
		public:
			~xcTreeDataSource(void){if(m_strText)delete m_strText; if(m_strData)delete m_strData;}
		public:
			HTREEITEM	m_hItem;
			int		m_RLevel;
			bool	m_bLoop;
			LPTSTR	m_strText;
			LPTSTR	m_strData;
			int		m_iImage;
			int		m_iSelectedImage;
		public:
			xcTreeDataSource*	m_pChild;
		public:
			virtual int GetChildItems(HTREEITEM	pItem) = 0;
			virtual int ExpandItem(xcTreeCtrl* pTree, HTREEITEM	pItem) = 0;
		};
	}
}
