#include "stdafx.h"
#include "ixword.hpp"
#include "xworddatasvr.hpp"

IXWord::IXWord() : m_pWordSvr(0)
{

}

vord_t IXWord::GetVord()
{
	return ((XWordDataSvr*)m_pWordSvr->m_pDataSvr)->m_pContentSvr->document; 
}

vord_t IXWord::GetDataVord()
{
	return ((XWordDataSvr*)m_pWordSvr->m_pDataSvr)->m_pContentSvr->data; 
}

void IXWord::SetReadOnly(bool bReadOnly)
{
	m_pWordSvr->SetReadOnly(bReadOnly);
}

bool IXWord::GetReadOnly()
{
	return m_pWordSvr->GetReadOnly();
}

