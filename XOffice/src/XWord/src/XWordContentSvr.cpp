#include "StdAfx.h"
#include "XWordContentSvr.hpp"
#include "XWordPred.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	DWORD GetLayerFlow(BSTR pstrLayerFlow)
	{
		DWORD dwLayerFlow=0;
		if(_wcsicmp(pstrLayerFlow,L"span")==0)
			dwLayerFlow = LayerFlow_Span;
		else if(_wcsicmp(pstrLayerFlow,L"block")==0)
			dwLayerFlow = LayerFlow_Block;
		else if(_wcsicmp(pstrLayerFlow,L"blockex")==0)
			dwLayerFlow = LayerFlow_BlockEx;
		else if(_wcsicmp(pstrLayerFlow,L"round")==0)
			dwLayerFlow = LayerFlow_Round;
		else if(_wcsicmp(pstrLayerFlow,L"newpaper2")==0)
			dwLayerFlow = LayerFlow_NewsPaper2;
		else if(_wcsicmp(pstrLayerFlow,L"newpaper3")==0)
			dwLayerFlow = LayerFlow_NewsPaper3;

		return dwLayerFlow;
 
	}

	XWordContentSvr::XWordContentSvr(void):m_pXWordDataDocument(NULL),
		document(word_group)
	{
	}

	XWordContentSvr::~XWordContentSvr(void)
	{
		if(m_pXWordDataDocument) delete m_pXWordDataDocument;
		document.release();
	}
}}}}
