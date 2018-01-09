#pragma once

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfNodeWin :
		public xfNodeBox
	{
	public:
		xfNodeWin(void);
		~xfNodeWin(void);
	public:
		LPTSTR	m_pXScript;			//xscript
		LPTSTR	m_pXDll;			//xdll
		LPTSTR	m_pstrGuid;			//xguid
		LPTSTR	m_pstrWorkGuid;		//xworkguid
		LPTSTR	m_pstrScriptUrl;	//ScriptUrl
		LPTSTR	m_pstrScriptClass;	//ScriptClass
	public:
		LPTSTR m_pStrUrl;
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
		virtual int CreateNodeControl(HWND	hWnd);
	public:
		virtual int AdjustCtrlRect();
		virtual int RemoveControl();
	public:
		virtual int ShowControl(bool bShow);

	};
}}
