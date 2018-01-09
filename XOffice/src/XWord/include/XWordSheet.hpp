#pragma once

#include "XOfbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class IXWord;
	class XWORD_API XWordSheet :
		public XOfficeSheet
	{
	public:
		XWordSheet(void);
		~XWordSheet(void);
	public:
		IXWord * m_pIWordView;
		virtual xbObject*  GetInterface();
	};
}}}}
