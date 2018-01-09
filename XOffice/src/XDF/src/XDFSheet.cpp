#include "StdAfx.h"
#include "..\include\XDFSheet.hpp"
#include "..\include\XDFSheetSvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	XDFSheet::XDFSheet(void)
	{
		m_Standalone = true;
		m_pSheetSvr = new XDFSheetSvr();
		m_pSheetSvr->m_pSheetCtrl = this;
	}

	XDFSheet::~XDFSheet(void)
	{
		if(m_pSheetSvr)delete m_pSheetSvr;
	}
}}}}
