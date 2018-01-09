#pragma once


#include "xframe.h"
using namespace Hxsoft::XFrame;

/*
#include "xscomplier.h"
#include "astnode.h"

using namespace xsharp;
*/

#include "parse.h"
#include "xssparse.h"
using namespace  xcode;

#include "xsm.h"
#include "parser/parser.hxx"
#include "parser/pcodes.hxx"
#include "tpp/parser_tpp.hxx"
#include "gen/gen_tpp.hxx"

using namespace xsm;


class xCodeSheet :
	public xfWin
{
public:
	xCodeSheet(void);
	~xCodeSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
	virtual bool EvtNotify(TEvent* pEvent,LPARAM lParam);
	virtual bool EvtCodeUpdateUI(TEvent* pEvent,LPARAM lParam);
public:
	virtual int OnOpen();
public:
	int OnCodeFind();
	int OnCodeReplace();
public:
	//complier * m_cp;
	xcomplier * m_xcp;
	xsscomplier * m_xsscp;
	void * m_wpp;
	void * m_wpp5;
	void * m_xsmparser;
	int InitXssComplier();
	int InitXssComplier(void * parser);
	int Eval1(int nLine,void * parser = NULL);
	int Eval(int nLine,void * parser = NULL);
public:
	bool beval;
	bool m_btpp;
	bool m_bvl;
	bool m_bwpp;
	bool m_bwpp5;
public:
	bool EvtFctSelection(TEvent * pEvent, LPARAM lParam);
	bool EvtClsSelection(TEvent * pEvent, LPARAM lParam);
	bool EvtEmDblClicked(TEvent * pEvent, LPARAM lParam);
	bool EvtEditChanged(TEvent * pEvent, LPARAM lParam);

	struct TError 
	{
		POINT pt;
		std::wstring errText;
	};
	vector<TError>	m_errors;
public:
	bool ReFillClassInfo(int startIndex,int endIndex);
	bool ReFillFctInfo(void * pobject,bool bfield = false);
};
