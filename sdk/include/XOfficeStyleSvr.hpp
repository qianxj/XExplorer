#pragma once
#include "svrbase.hpp"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API XOfficeStyleSvr :
		public CSvrbase
	{
	public:
		XOfficeStyleSvr(void);
		~XOfficeStyleSvr(void);
	public:
		XUtil::xuCCssStyle * m_pCssStyle;
		//template class  XOFFICE_API XUtil::xuVector<XUtil::Css::CssClassItem>;
		XUtil::xuVector<XUtil::Css::CssClassItem> m_CssClass;  
	public:
		int AddCssClass(LPTSTR className,LPTSTR cssStr);
		const XUtil::Css::TBoxStyle* GetBoxStyle(int nIndex);
		int SetCssStyle(const XUtil::Css::TBoxStyle* pBoxStyle);
		int GetBoxStyleIndexByClass(LPTSTR pStrClass);
	};
}}}