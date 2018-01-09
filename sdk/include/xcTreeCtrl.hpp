#pragma once
#include <xframe.h>
#include "xcTreexmldatasource.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		#define	TVN_RCLICK   TVN_FIRST-100
		#define	TVN_ENDDRAG  TVN_FIRST-101
		#define	TVN_POPUPMENU  TVN_FIRST-102
		#define	TVN_LCLICK   TVN_FIRST-103
		#define	TVN_CHECKBOXSEL   TVN_FIRST-104

		class XCONTROL_API xcTreeCtrl:
			public xfControl
		{
		public:
			xcTreeCtrl();
		public:
			~xcTreeCtrl();

		public:
			vector<xcTreeXmlDataSource *>* m_pTreeSource;
		public:
			int MakeTree();
			int ExpandTree(HTREEITEM hpItem, IXMLDOMElementPtr pElement,int level);
			bool isParentItem(HTREEITEM hpItem, HTREEITEM hItem,int &rLevel);
			int ExpandItem(HTREEITEM hpItem,bool expandChild = true,int flowLevel = 0);
			int ExpandItemEx(HTREEITEM	hItem);
			int ExpandAll();
			int ExpandRoot(bool expandChild = true,int flowLevel = 0);
			int ExpandAll(HTREEITEM hItem);
		public:
			virtual	void Initial();
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
		public:
			vector<LPTSTR>* m_pDatas;
			vector<IXMLDOMElement * >* m_pElements;
		public:
			bool EvtItemExpanding(TEvent* pEvent,LPARAM lParam);
			bool EvtBeginDrag(TEvent* pEvent,LPARAM lParam);
		public:
			HIMAGELIST m_DragImage;
			};
	}
}
