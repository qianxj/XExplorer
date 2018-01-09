#pragma once
#include "xbXBase.h"
#include "xuxutil.h"
#include "commctrl.h"
#include <string>
#include <map>
using namespace std;


namespace Hxsoft{ namespace XFrame
{

	class xfNodeResource;
	template class XFRAME_API Hxsoft::XUtil::xuVector<xfNodeResource *>;
	template class XFRAME_API Hxsoft::XUtil::xuVector<HIMAGELIST>;
	class XFRAME_API xfResourceMgr :
		public xfAreaObject
	{
	public:
		xfResourceMgr(void);
		~xfResourceMgr(void);
	public:
		Hxsoft::XUtil::xuVector<xfNodeResource *>	m_Resources;
		int AddResource(xfNodeResource *);
	public:
		Hxsoft::XUtil::xuVector<HIMAGELIST> m_hImageLists;
		int AddImageList(HIMAGELIST hImageList);
		Hxsoft::XUtil::xuVector<HIMAGELIST>&  GetImageList();
	public:
		static LPTSTR m_DefaultLocal;
	public:
		map<wstring,map<wstring,wstring>>* m_pStringResource;
		LPCTSTR GetStringItem(LPTSTR strItem);
		LPCTSTR GetStringItem(LPTSTR strLocal,LPTSTR strItem);
		bool AddStringItem(LPTSTR strItemName,LPTSTR strItemText);
		bool AddStringItem(LPCTSTR strLocal,LPCTSTR strItemName,LPCTSTR strItemText);
		map<wstring,wstring> * GetLocalItem(LPCTSTR strLocal);
		map<wstring,wstring> * GetLocalItem();
		int AddLocalFile(LPTSTR pStrFile);
	};
}}
