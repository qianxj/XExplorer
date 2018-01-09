#include "StdAfx.h"
#include "..\include\ITrans.hpp"
#include "xfapp.hpp"

namespace Hxsoft{ namespace XFrame
{
	ITrans::ITrans(void):m_pServerUrl(NULL),m_pUser(NULL),m_pAuthor(NULL)
	{

	}

	ITrans::~ITrans(void)
	{
		if(m_pServerUrl)delete m_pServerUrl;
		if(m_pUser)delete m_pUser;
		if(m_pAuthor)delete m_pAuthor;
	}

	ITrans* ITrans::GetTrans(LPCTSTR pServerUrl,LPCTSTR pUser,LPCTSTR pPass)
	{
		return xfApp::GetApp()->GetTrans(pServerUrl,pUser,pPass);
	}
}}
