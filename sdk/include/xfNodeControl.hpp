#pragma once

namespace Hxsoft{ namespace XFrame
{
	class xfControl;
	class XFRAME_API xfNodeControl :  
		public xfNode
	{
	public:
		xfNodeControl(void);
		~xfNodeControl(void);
	public:
		xfControl*	m_pxfControl;
	public:
		int		m_ImageIndex;
		int		m_ImagePos;
		bool	m_bShow;
	public:
		LPTSTR	m_pLayerFrame;
		LPTSTR  m_pLayerSheet;
		virtual xfNode * GetLabelNode(bool bSelection ,LPCTSTR pLayerFrame,LPCTSTR pLayerSheet);

	public:
		virtual int DrawNode(HDC hDC);
		int DrawNodeEx(HDC hDC);
		HBITMAP m_hBackGround;
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
	public:
		virtual int CreateNodeControl(HWND	hWnd);
		virtual int AdjustCtrlRect();
		virtual int RemoveControl();
		virtual int OnClose();

	public:
		virtual int ShowControl(bool bShow);
	public:
		virtual HWND GetHWND();
	public:
		virtual int GetXml(BSTR &bstr);
	};
}}
