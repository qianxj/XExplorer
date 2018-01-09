#pragma once
#include "xfnodecontrol.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfNodeLayers :
		public xfNodeControl
	{
		enum XTabDrawStyle{tdsRoundRect,tdsRoundRectEx,tdsShape,tdsEclips,tdsEclipsEx,tdsCanClose};
		enum XTabArraow{taTop,taBottom,taLeft,taRight};
	public:
		xfNodeLayers(void);
		~xfNodeLayers(void);
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
		virtual int AdjustRect();
	public:
		virtual int CreateNodeControl(HWND	hWnd);
		virtual int AdjustCtrlRect();

	public:
		int	m_tabSize;
		int	m_nCurSheet;
		int SelectSheet(int newSheet);
		int SelectSheet(int newSheet,bool redrawcursheet);
		
		int InsertSheet(int nIndex,xfNode * pNode,bool bCreateControl = true);
		int RemoveSheet(int nIndex);
		int Clear();

		XTabArraow	m_nTabArrow;
		XTabDrawStyle m_nTabDrawStyle;
	public:
		virtual int ShowControl(bool bShow);
		virtual xfNode * GetLabelNode(bool bSelection ,LPTSTR pLayerFrame,LPTSTR pLayerSheet);
	public:
		LPTSTR  m_pStrFormNode;
		xfNode* m_pFormNode;
	};
}}
