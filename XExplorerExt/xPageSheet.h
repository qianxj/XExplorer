#pragma once


#include "xframe.h"
using namespace Hxsoft::XFrame;

typedef struct tagxbObjectMgrEvent
{
	std::vector<std::wstring> m_eventName;
	std::vector<std::wstring> m_eventID;
	std::vector<std::wstring> m_eventParam;
}xbObjectMgrEvent;

class xPageSheet :
	public xfWin
{
public:
	bool m_btpp;
	bool m_bvl;
	bool m_bwpp;
	bool m_bwpp5;
public:
	xPageSheet(void);
	~xPageSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
	virtual bool EvtSetFocus(TEvent * pEvent, LPARAM lParam);
	virtual bool EvtNotify(TEvent* pEvent,LPARAM lParam);
	virtual bool EvtCodeUpdateUI(TEvent* pEvent,LPARAM lParam);
	virtual bool EvtPageUpdateUI(TEvent* pEvent,LPARAM lParam);
	int SetAgent();
public:
	virtual int OnOpen();
public:
	xfNode * m_pCodeAgent;
	xfNode * m_pPageAgent;
public:
	int m_nSheet;
public:
	std::map<std::wstring,xbObjectMgrEvent *>  m_mapObjectEvents;
public:
	void * m_xsscp;
	void * m_xsmparser;
	void * m_pvlang;
	void * m_wpp;
	void * m_wpp5;
	int InitXssComplier();
	int InitXssComplier(void * parser);
	int Eval(int nLine,void * parser = NULL);
public:
	int OnCodeFind();
	int OnCodeReplace();
public:
	bool beval;
public:
	bool EvtFctSelection(TEvent * pEvent, LPARAM lParam);
	bool EvtClsSelection(TEvent * pEvent, LPARAM lParam);
public:
	bool ReFillClassInfo(int startIndex,int endIndex);
	bool ReFillFctInfo(void * pobject,bool bfield = false);
};
