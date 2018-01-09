#include "StdAfx.h"
#include "..\include\XOfficeStyleSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeStyleSvr::XOfficeStyleSvr(void):m_pCssStyle(new XUtil::xuCCssStyle)
	{
	}

	XOfficeStyleSvr::~XOfficeStyleSvr(void)
	{
		if(m_pCssStyle)delete m_pCssStyle;
		for(int i=0;i<(int)m_CssClass.size();i++)
		{
			if(m_CssClass[i].pStrClass) delete m_CssClass[i].pStrClass;
		}
	}

	int XOfficeStyleSvr::AddCssClass(LPTSTR className,LPTSTR cssStr)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = m_pCssStyle->ParseCssStyle(cssStr);
		int nIndex = -1;
		nIndex = m_pCssStyle->AddCssCellStyle(pBoxStyle);
		if(nIndex >=0)
		{
			XUtil::Css::CssClassItem cssItem;
			cssItem.pStrClass = ::_tcsdup(className);
			cssItem.nIndex = nIndex;
			this->m_CssClass.push_back(cssItem);
		}
		return nIndex;
	}
	
	const XUtil::Css::TBoxStyle* XOfficeStyleSvr::GetBoxStyle(int nIndex)
	{
		return m_pCssStyle->GetBoxStyle(nIndex);
	}

	int XOfficeStyleSvr::SetCssStyle(const XUtil::Css::TBoxStyle* pBoxStyle)
	{
		return m_pCssStyle->AddCssCellStyle(pBoxStyle);
	}

	int XOfficeStyleSvr::GetBoxStyleIndexByClass(LPTSTR pStrClass)
	{
		int nIndex = -1;
		XUtil::Css::TBoxStyle* pBoxStyleOne = NULL;
		const XUtil::Css::TBoxStyle* pBoxStyleTwo;

		vector<LPTSTR> vals;
		XUtil::GetTokens(pStrClass,_T(" "),vals);
		for(int k=0;k<(int)vals.size();k++)
		{
			for(int i=0;i<m_CssClass.size();i++)
			{
				if(_tcsicmp(vals[k],m_CssClass[i].pStrClass)==0)
				{
					pBoxStyleTwo =  GetBoxStyle(m_CssClass[i].nIndex);
					if(pBoxStyleTwo)
					{
						if(!pBoxStyleOne) pBoxStyleOne = new XUtil::Css::TBoxStyle();
						*pBoxStyleOne = *pBoxStyleOne + *pBoxStyleTwo; 
					}
					break;
				}
			}
		}
		if(pBoxStyleOne)
		{
			nIndex = SetCssStyle(pBoxStyleOne);
			delete pBoxStyleOne;
		}
		return nIndex;
	}
}}}
