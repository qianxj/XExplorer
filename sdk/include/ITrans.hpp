#pragma once

#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API ITrans :
		public xbObject
	{
	public:
		ITrans(void);
		~ITrans(void);
	public:
		LPTSTR m_pServerUrl;
		LPTSTR m_pUser;
		LPTSTR m_pAuthor;
	public:
		LPCTSTR GetServerUrl(){return m_pServerUrl;}
	public:
		static ITrans* GetTrans(LPCTSTR pServerUrl,LPCTSTR pUser=NULL,LPCTSTR pPass = NULL);
	};
}}
