#include "StdAfx.h"
#include "..\include\XReportDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportDataSvr::XReportDataSvr(void):m_pHeaderSvr(NULL),m_pRowSelectorSvr(NULL),
		m_pAixsSvr(NULL),m_pDetailSvr(NULL),m_pContentSvr(NULL),m_pQueryXml(NULL),
		m_pQueryUrl(NULL),m_pRightRowSelectorSvr(NULL),FOption(0),m_pEditor(NULL),m_pFormat(NULL),
		m_pClassEditor(NULL),m_pClassFormat(NULL)
	{
	}

	XReportDataSvr::~XReportDataSvr(void)
	{
		if(m_pAixsSvr) delete m_pAixsSvr;
		if(m_pRowSelectorSvr)delete m_pRowSelectorSvr;
		if(m_pRightRowSelectorSvr)delete m_pRightRowSelectorSvr;
		if(m_pHeaderSvr) delete m_pHeaderSvr;
		if(m_pDetailSvr) delete m_pDetailSvr;
		if(m_pContentSvr) delete m_pContentSvr;
		if(m_pQueryUrl)delete m_pQueryUrl;
		if(m_pQueryXml)delete m_pQueryXml;
		if(m_pEditor)
		{
			for(int i=0;i<(int)m_pEditor->size();i++)
			{
				if((*m_pEditor)[i])delete (*m_pEditor)[i];
			}
			delete m_pEditor;
		}
		if(m_pFormat)
		{
			for(int i=0;i<(int)m_pFormat->size();i++)
			{
				if((*m_pFormat)[i])delete (*m_pFormat)[i];
			}
			delete m_pFormat;
		}
		if(m_pClassEditor)
		{
			for(int i=0;i<(int)m_pClassEditor->size();i++)
			{
				if((*m_pClassEditor)[i])delete (*m_pClassEditor)[i];
			}
			delete m_pClassEditor;
		}
		if(m_pClassFormat)
		{
			for(int i=0;i<(int)m_pClassFormat->size();i++)
			{
				if((*m_pClassFormat)[i])delete (*m_pClassFormat)[i];
			}
			delete m_pClassFormat;
		}
	}
}}}}
