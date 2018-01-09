#include "StdAfx.h"
#include "XOfficeFrm.hpp"
#include "IXOffice.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeFrm::XOfficeFrm(void)
	{
		m_pFrameSvr = new XOfficeFrmSvr();
		m_pFrameSvr->Initial(this);
	}

	XOfficeFrm::~XOfficeFrm(void)
	{
	}

	void XOfficeFrm::Initial() 
	{
		if(m_pxfNode && m_pFrameSvr)
		{
			IXMLDOMNode * pNode;
			m_pxfNode->m_pElement->selectSingleNode(L"xdoc",&pNode);
			if(pNode)
			{
				IXMLDOMElement * pELement;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pELement);
				
				DWORD state = 0;
				_variant_t val;
				pELement->getAttribute(L"state",&val);
				if(val.bstrVal)
				{
					if(_wcsicmp(val.bstrVal,L"run")==0)
					{
						state = goRunState;
					}
					else if(_wcsicmp(val.bstrVal,L"design")==0)
					{
						state = goDesignState;
					}
					else
					{
						state = goNormalState;
					}
				}else
				{
					state = goNormalState;
				}

				m_pFrameSvr->LoadTemplate(pELement,state);
				pELement->Release();
			}
			else
			{
				m_pFrameSvr->LoadTemplate(m_pxfNode);
			}
			if(m_pFrameSvr->m_Sheets.size()>0)m_pFrameSvr->SelectSheet(0);
		}
	}
	
	xbObject * XOfficeFrm::GetInterface()
	{
		if(!m_pInterFace)
		{
			m_pInterFace = new IXOffice();
			((IXOffice *)m_pInterFace)->m_pFrameSvr = (XOfficeFrmSvr *)m_pFrameSvr;
		}

		return m_pInterFace;
	}

}}}
