#pragma once
#include "xbXBase.h"
#include "xuxutil.h"
#include "xfresourcemgr.hpp"

namespace Hxsoft{ namespace XFrame
{
	#define xfNodeAgentArea L"agentarea"
	#define xfNodeAgentToolArea L"agenttoolarea"
	#define xfNodeAgentExpanedArea L"agentexpandarea"

	class xfWin;
	class xfControl;
	class xfNodeScript;
	class XFRAME_API xfXPage : public xbObject
	{
	public:
		xfXPage(void);
		~xfXPage(void);
	public:
		xbXml*	m_pxbXml;
		IXMLDOMElement*	m_pElement;
	public:
		bool Load(LPCTSTR pStrXPageFile);
		bool LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl);
		bool Load(IXMLDOMElement* pElement);
		bool LoadXPage(LPCTSTR pStrPage);
	public:
		xfNode * GetAgentNode(LPCTSTR pStrName = xfNodeAgentArea);
		xfNode * SetAgentNodeContent(xfNode* pAgentNode,IXMLDOMElement* pElement,xfNode * pOwnerNode);
		xfNode * SetAgentNodeContent(xfNode* pAgentNode,LPCTSTR pStrXml,xfNode * pOwnerNode);
		int ClearAgentNodeContent(xfNode* pAgentNode = NULL,LPCTSTR pStrName = NULL);
		int ClearAgentNodeContentEx(xfNode* pAgentNode = NULL,LPCTSTR pStrName = NULL);
		xfNode * SetAgentNodeContent(xfNode* pAgentNode,xfNode * pNode,xfNode * pOwnerNode);
		int RemoveAgentNodeContent(xfNode* pAgentNode,xfNode * pNode);
		int RemoveAgentNodeContent(xfNode* pAgentNode,xfNode * pNode,bool bOwner);
	public:
		int DoDraw(HDC hDC);
	public:
		xfNode * m_pxfNode;
		xfWin * m_pWin;
		xfControl * m_pControl;
	public:
		LPTSTR m_pStrLocal;
		LPTSTR m_pStrUrl;
		LPTSTR m_pWkStrUrl;
	public:
		std::vector<xfNodeScript  *  > * m_pScript;
		int AddScript(xfNodeScript *);
		int RemoveScript(xfNodeScript *);
	public:
		bool ParsePage();
	public:
		XUtil::xuCCssStyle*	m_pCCssStyle;
		XUtil::xuCssStyleSheet* m_pStyleSheet;
	public:
		xfResourceMgr * m_pResourceMgr;
	public:
		class xfNodeControl	* m_pFocusNodeControl;
		class xfNodeControl	* GetFocusNodeControl();
	public:
		xbObject* GetInterface();
	public:
		struct tagAgentContents
		{
			xfNode * pAgentNode;
			xfNode * pNode;
		};
		vector<tagAgentContents> * m_pAgentContents;
	};
}}