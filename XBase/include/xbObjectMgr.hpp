#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>

namespace Hxsoft{ namespace XFrame
{
	typedef bool (WINAPI * fnCreateInstance)( LPTSTR ,  xbObject * & );



	typedef struct tagxbObjectMgrItem
	{
		LPTSTR	m_pStrCtrlFile;
		HMODULE m_hCtrlDLL;
		std::set<std::wstring>  m_setObject;
		fnCreateInstance m_pFnCreateInstance;
		tagxbObjectMgrItem():m_pStrCtrlFile(NULL),m_hCtrlDLL(NULL),m_pFnCreateInstance(NULL){};
		~tagxbObjectMgrItem()
		{
			if(m_pStrCtrlFile) delete m_pStrCtrlFile;
			std::set<std::wstring>::const_iterator iterator;
			for(iterator= m_setObject.begin();iterator!=m_setObject.end();iterator++)
			{
				//if(*iterator) delete *iterator;
			}
			m_setObject.clear();
			if(m_hCtrlDLL)::FreeLibrary(m_hCtrlDLL);
		}
	}xbObjectMgrItem;

	class  XBASE_API xbObjectMgr : public xbObject
	{
	public:
		xbObjectMgr(void);
		virtual ~xbObjectMgr(void);
	public:
		std::vector<xbObjectMgr *>*   m_pObjectMgrs;
		std::multimap<std::wstring, xbObjectMgrItem *>*   m_pmapObjectMgrItem;
	public:
		virtual bool CreateInstance(LPTSTR pStrUri, LPTSTR pStrObject,xbObject * &pxbObject,bool bSetClassName = true);
	public:
		void RegisterObjectsByXmlFile(LPTSTR pStrFile );
		void RegisterObjectsByObjectMgr(LPTSTR pStrUri, xbObjectMgr * pObjectMgr );
	public:
		bool IsControl(LPTSTR pStrUri, LPTSTR pStrObject);
	};
}}