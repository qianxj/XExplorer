#include "StdAfx.h"
#include "..\include\XProjectSheet.hpp"
#include "xprojectsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	XProjectSheet::XProjectSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XProjectSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
	}

	XProjectSheet::~XProjectSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}
}}}}
