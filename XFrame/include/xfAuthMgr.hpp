#pragma once

#include "xbxbase.h"
#include "xfareaobject.hpp"
#include <map>
#include <string>
namespace Hxsoft{ namespace XFrame
{
	struct AuthItem
	{
		std::wstring	UserID;
		std::wstring	UserTocken;
	};
	class xfAuthMgr :
		public xfAreaObject
	{
	public:
		xfAuthMgr(void);
		~xfAuthMgr(void);
	public:
		std::map<std::wstring,AuthItem> Authers;
	};
}}
