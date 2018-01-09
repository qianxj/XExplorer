#pragma once


#include "xframe.h"
#include "xccontrol.h"
using namespace Hxsoft::XFrame;

class CMailView:public xfWin
{
public:
	enum EViewType{ VT_Text,VT_SafeHtml,VT_Html};
	EViewType m_viewType;
public:
	CMailView(void);
	~CMailView(void);

public:
	int OnOpen();

public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);  

	//define control
	class XMailFolder: public xcTreeCtrl
	{
		public:
			XMailFolder();
			~XMailFolder();
		public:
			XFRAME_EVENT_(DoubleClicked);
			XFRAME_EVENT_(SelectChanged);
			int GetLevel(HTREEITEM hItem);
	};

	class XMailList: public xcWCControl
	{
		public:
			XMailList();
			~XMailList();
		public:
			XFRAME_EVENT_(DoubleClicked);
			XFRAME_EVENT_(SelectChanged);
		public:
			int SelectedItem(int nItem);
		public:
			virtual int ControlCreated();
		public:
			int Retrieve(LPTSTR Url,LPTSTR pXmlQuery);
	};
	//end define control
};
