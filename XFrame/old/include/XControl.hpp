#pragma once

#include <vector>
#include <commctrl.h>

using namespace std;

namespace Hxsoft
{
	namespace XFrame
	{
		class XNode;
		class XFRAME_API XControl:
			public XWinbase
		{
		public:
			XControl():m_pNode(NULL),m_ImageIndex(0),m_ImagePos(-1){};
		public:
			~XControl();
		public:
			XNode *	m_pNode;
			LPTSTR m_pstrXClass;
			vector<XControl *>m_pChilds;
			int		m_ImageIndex;
			int		m_ImagePos;
			vector<HIMAGELIST> m_hImageLists;
		};
	}
}
