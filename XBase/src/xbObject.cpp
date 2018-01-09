#include "StdAfx.h"
#include "xbObject.hpp"

namespace Hxsoft{ namespace XFrame
{
	xbObject::xbObject(void):m_pStrUriObject(NULL),m_pStrClassName(NULL),m_pInterFace(NULL),m_nRef(1)
	{

	}

	xbObject::~xbObject(void)
	{
		if(m_pStrUriObject)delete m_pStrUriObject;
		if(m_pStrClassName)delete m_pStrClassName;  
		if(m_pInterFace)delete m_pInterFace;
	}

	 bool xbObject::SetClassName(LPTSTR pStrUri,LPTSTR pStrClassName)
	{
		m_pStrUriObject = ::_tcsdup(pStrUri);
		m_pStrClassName = ::_tcsdup(pStrClassName);   
		return true;  
	}

	LPCTSTR xbObject::GetClassName(LPCTSTR pStrUri)
	{
		pStrUri = m_pStrUriObject;
		return m_pStrClassName;
	}

	 xbObject * xbObject::GetInterface()
	 {
		 return m_pInterFace;
	 }

	 int xbObject::GetXml(BSTR &bstr)
	 {
		 bstr = NULL;
		 return 0;
	 }

	 int xbObject::SetXml(LPTSTR pStrXml)
	 {
		 return 0;
	 }

}}
