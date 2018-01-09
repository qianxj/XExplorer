#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class CModulaDlg :public xfWin
{
public:
	CModulaDlg(void);
	~CModulaDlg(void);
public:
	int Create(LPTSTR pUrl,RECT bound,LPARAM lParam = NULL);
	bool m_bCanDestroy;
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	bool EvtNotify(TEvent* pEvent,LPARAM lParam);
	bool EvtActive(TEvent* pEvent,LPARAM lParam);
private:
	int GetLevel(HTREEITEM hItem);
	LPTSTR m_pStrUrl;
	std::vector<wstring> m_guids;
	wstring m_listguid;

	LPTSTR m_pWorkNodeType;
};
