#include "StdAfx.h"
#include "..\include\XShowSheet.hpp"
#include "..\include\XShowSheetSvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	XShowSheet::XShowSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XShowSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
	}

	XShowSheet::~XShowSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}
}}}}
