#pragma once
#include "svrbase.hpp"
#include "xofficesheet.hpp"
#include "xOfficeSerializeSvr.hpp"
#include "xOfficeData.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API XOfficeSheetSvr :
		public xfAreaObject
	{
	public:
		XOfficeSheetSvr(void);
		~XOfficeSheetSvr(void);
	public:
		XOfficeSheet *	m_pSheetCtrl;
	public:
		RECT FRect;
		virtual int SetFRect(RECT rc);
		virtual int ShowSheet(bool bShow);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		XOfficeSerializeSvr* m_pSerializeSvr;
		XOfficeData* m_pDataSvr;
	public:
		bool m_bContentLoaded;
	public:
		virtual int LoadTemplate();
		virtual int LoadTemplate(xfNode * pNode);
		virtual int LoadTemplate(IXMLDOMDocument * pXmlDoc);
		virtual int LoadTemplate(LPTSTR pStrTemplateFile,LPTSTR pStrXPath=NULL);
		virtual int LoadTemplate(IXMLDOMElement * pElement,int state = goNormalState);
		virtual int LoadTemplateByXml(LPTSTR pStrXml,LPTSTR pStrXPath=NULL);
	public:
		LPTSTR	m_pStrTemplate;
		LPTSTR  m_pStrXPath;
		LPTSTR  m_pStrDataUri;
		LPTSTR  m_pStrFile;
		int		m_state;
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	public:
		virtual bool SetEventListons(){return true;};
		virtual bool UnSetEventListons(){return true;};
	public:
		virtual int TemplateLoaded(IXMLDOMElement * pElement,int state){return 1;}
		virtual int PreLoadTemplate(IXMLDOMElement * pElement,int state){return 1;}
	};
}}}
