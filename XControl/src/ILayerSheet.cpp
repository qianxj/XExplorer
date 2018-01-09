#include "stdafx.h"
#include "ilayersheet.hpp"

namespace Hxsoft{ namespace XFrame
{
	ILayerSheet::ILayerSheet():m_pLayerSheet(NULL),m_vcParam(new vector<void *>)
	{
		
	}
	ILayerSheet::~ILayerSheet()
	{
		if(m_vcParam)delete m_vcParam; 
	}

	int ILayerSheet::InsertSheet(int nIndex,LPTSTR pText, IXNode * pNode,int iImage,bool bToCurrent,bool bCreateControl )
	{
		int nPos = m_pLayerSheet->InsertSheet(nIndex,pText,iImage,bToCurrent);
		if(!m_pLayerSheet->m_pFrame)
		{
			xfNodeLayers * pNodeLayer = dynamic_cast<xfNodeLayers *>(m_pLayerSheet->m_pxfNode);
			if(pNodeLayer && pNode )pNodeLayer->InsertSheet(nIndex,pNode->m_pNode,bCreateControl);
		}
		if(bToCurrent || GetSheetCount()==1)
			SelectSheet(nPos);
		return nPos;
	}
	int ILayerSheet::InsertSheet(int nIndex,LPTSTR pText,IXMLDOMElement * pElement,int iImage,bool bToCurrent )
	{
		xfNode * pNode =  xfNode::CreateNode(pElement);
		pNode->ParseNode(pElement);
		return InsertSheet(nIndex,pText,(IXNode *)pNode->GetInterface(),iImage,bToCurrent);
	}

	int ILayerSheet::InsertSheet(int nIndex,LPTSTR pText, IXNode * pNode,void * p,int iImage,bool bToCurrent,bool bCreateControl )
	{
		int nPos = m_pLayerSheet->InsertSheet(nIndex,pText,iImage,bToCurrent);
		this->SetParam(nPos, p);
		if(!m_pLayerSheet->m_pFrame)
		{
			xfNodeLayers * pNodeLayer = dynamic_cast<xfNodeLayers *>(m_pLayerSheet->m_pxfNode);
			pNode->m_pNode->m_pParam = p;
			if(pNodeLayer && pNode )pNodeLayer->InsertSheet(nIndex,pNode->m_pNode,bCreateControl);
			//pNode->m_pNode->m_pxfPage->m_pWin->m_pParam = p;
		}
		if(bToCurrent || GetSheetCount()==1)
			SelectSheet(nPos);
		return nPos;
	}
	int ILayerSheet::InsertSheet(int nIndex,LPTSTR pText,IXMLDOMElement * pElement,void * p,int iImage,bool bToCurrent )
	{
		xfNode * pNode =  xfNode::CreateNode(pElement);
		pNode->ParseNode(pElement);	
		return InsertSheet(nIndex,pText,(IXNode *)pNode->GetInterface(), p,iImage,bToCurrent);
	}

	int ILayerSheet::GetSheetIndex()
	{
		return this->m_pLayerSheet->m_iCurSheet;
	}

	int ILayerSheet::DeleteSheet(int nIndex)
	{
		if(nIndex <0) nIndex = GetSheetIndex();
		if(nIndex < 0 || nIndex >= GetSheetCount()) return false;
		m_pLayerSheet->RemoveSheet(nIndex);
		if(!m_pLayerSheet->m_pFrame)
		{
			xfNodeLayers * pNodeLayer = dynamic_cast<xfNodeLayers *>(m_pLayerSheet->m_pxfNode);
			if(pNodeLayer)pNodeLayer->RemoveSheet(nIndex);
		}

		if(nIndex<(int)m_vcParam->size())m_vcParam->erase(m_vcParam->begin()+nIndex);
		return 0;
	}

	bool ILayerSheet::SelectSheet(int nIndex)
	{
		if(nIndex < 0 || nIndex >= GetSheetCount()) return false;
		m_pLayerSheet->SelectSheet(nIndex);
		return true;
	}

	int ILayerSheet::Clear()
	{
		m_pLayerSheet->Clear();
		if(!m_pLayerSheet->m_pFrame)
		{
			xfNodeLayers * pNodeLayer = dynamic_cast<xfNodeLayers *>(m_pLayerSheet->m_pxfNode);
			pNodeLayer->Clear();
		}
		return 1;
	}

	int ILayerSheet::GetSheetCount()
	{
		return m_pLayerSheet->GetSheetCount();
	}

	bool ILayerSheet::SetParam(int nIndex,void * p)
	{
		if(nIndex >= GetSheetCount()) return false;
		if(nIndex < 0) return false;

		int cnt = (int)m_vcParam->size();
		if(nIndex <cnt)
			(*m_vcParam)[nIndex] = p;
		else
		{
			for(int i= cnt;i<= nIndex;i++)
			{
				if(i< nIndex)
					m_vcParam->push_back(NULL);
				else
					m_vcParam->push_back(p);
			}
		}
		return true;

	}
	void * ILayerSheet::GetParam(int nIndex)
	{
		if(nIndex >= GetSheetCount()) return NULL;
		if(nIndex < 0) return NULL;
		if(nIndex > (int)m_vcParam->size()) return NULL;
		return (*m_vcParam)[nIndex];
	}

	int ILayerSheet::SetSheetState(int sheet, int state)
	{
		return m_pLayerSheet->SetSheetState(sheet, state);		
	}

	int ILayerSheet::GetSheetState(int sheet)
	{
		return m_pLayerSheet->GetSheetState(sheet);
	}

	int ILayerSheet::SetSheetText(int sheet, LPTSTR text)
	{
		return m_pLayerSheet->SetSheetText(sheet, text);		
	}

	LPTSTR ILayerSheet::GetSheetText(int sheet)
	{
		return m_pLayerSheet->GetSheetText(sheet);
	}

}}