#pragma once

#include "xframe.h"
#include "xccontrol.h"

using namespace Hxsoft::XFrame;


class CWorkNodeDlg :
	public xfWin
{
public:
	CWorkNodeDlg(void);
	~CWorkNodeDlg(void);
public:
	IXMLDOMElement * m_pDocElement;
public:
	int Initial(IXMLDOMElement * pElement);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
private:
	int GetLevel(HWND hWnd,HTREEITEM hItem);
public:
	LPTSTR m_pStrID;

};
