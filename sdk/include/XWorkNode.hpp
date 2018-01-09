#pragma once

#include <xframe.h>
#include "xofobject.h"

namespace Hxsoft{namespace XFrame 
{
	struct XWorkNodeItem
	{
		enum eWorkNodeItem {wkUndef=-1,wkRun, wkOpen ,wkNew ,wkList,wkCheck,wkView,wkDemo,wkCopy,wkQuery,wkSearch};
		TCHAR Guid[48];
		eWorkNodeItem nMethod;
		int			  nMode;
		TCHAR Query[512];
	};

	class XWORK_API XWorkNode
	{
	public:
		XWorkNode(void);
		~XWorkNode(void);
	public:
		xfWin *  Exec(xfWin * pParentWin, LPTSTR pWorkNodeUrl,LPVOID pParam,DWORD state);
		xfWin *  Exec(xfWin * pParentWin, XWorkNodeItem &Item,LPVOID pParam,DWORD state);
	protected:
		int ParseWorkNodeUrl(LPTSTR pWorkNodeUrl ,XWorkNodeItem &wkItem);
		bool isWorkNodeUrl(LPTSTR pWorkNodeUrl);
		int TrimSpace(LPTSTR & pStr);
	};
}}
