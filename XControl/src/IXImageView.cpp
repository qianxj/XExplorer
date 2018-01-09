#include "stdafx.h"
#include "IXImageView.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXImageView::IXImageView()
	{
		
	}
	IXImageView::~IXImageView()
	{

	}
	int IXImageView::AddImages(LPCTSTR pUrl, LPTSTR pText)
	{
		return ((xcImageView *)m_pControl)->AddImages(pUrl, pText);
	}
	int IXImageView::InsertImages(LPCTSTR pUrl, LPTSTR pText, int nIndex)
	{
		return ((xcImageView *)m_pControl)->InsertImages(pUrl, pText, nIndex);
	}
	int IXImageView::Reset()
	{
		return ((xcImageView *)m_pControl)->Reset();
	}

	int IXImageView::RemoveImage(int nIndex)
	{
		if(nIndex < 0 && nIndex >= GetImageCount())
			return -1;
		return ((xcImageView *)m_pControl)->RemoveImage(nIndex);
	}

	int IXImageView::RemoveImage()
	{
		int index = ((xcImageView *)m_pControl)->m_mIndex;
		if(index >= 0 && index < GetImageCount())
			return RemoveImage(index);
		else
			return -1;
	}

	int IXImageView::GetSelectIndex()
	{
		int index = ((xcImageView *)m_pControl)->m_mIndex;
		if(index >= 0 && index < GetImageCount())
			return index;
		else
			return -1;
	}

	int IXImageView::SetSelectIndex(int nIndex)
	{
		if(nIndex >= 0 && nIndex < GetImageCount())
			((xcImageView *)m_pControl)->m_mIndex = nIndex;
		return 1;
	}

	int IXImageView::GetImageCount()
	{
		return ((xcImageView *)m_pControl)->GetCount();
	}

	LPTSTR IXImageView::GetImageUrl(int nIndex)
	{
		if(nIndex < 0 && nIndex >= GetImageCount())
			return NULL;
		return (LPTSTR)((xcImageView *)m_pControl)->m_ImageUrls[nIndex].c_str();
	}
	int IXImageView::SetImageLabel(int nIndex, LPTSTR pText)
	{
		if(nIndex < 0 && nIndex >= GetImageCount())
			return -1;
		((xcImageView *)m_pControl)->m_LabelText[nIndex] = pText;
		return 1;
	}
	LPTSTR IXImageView::GetImageLabel(int nIndex)
	{
		if(nIndex < 0 && nIndex >= GetImageCount())
			return NULL;
		return (LPTSTR)((xcImageView *)m_pControl)->m_LabelText[nIndex].c_str();
	}
	int IXImageView::Preview(int percent)
	{
		return ((xcImageView *)m_pControl)->Preview(percent);
	}
	int IXImageView::SelectImage(int nRow,bool bSelect)
	{
		return ((xcImageView *)m_pControl)->SelectRow(nRow,bSelect);
	}
	int IXImageView::SelectImage(int sRow,int eRow, bool bSelect)
	{
		return ((xcImageView *)m_pControl)->SelectRow(sRow,eRow,bSelect);
	}
	bool IXImageView::IsImageSelected(int nRow)
	{
		return ((xcImageView *)m_pControl)->IsRowSelected(nRow);
	}
	int IXImageView::GetNextSelectImage(int nStartRow)
	{
		return ((xcImageView *)m_pControl)->GetNextSelectRow(nStartRow);
	}
	void IXImageView::SetSelectedMode(int mode)
	{
		((xcImageView *)m_pControl)->m_bSelectedMode = mode;
	}
	/*void IXImageView::AddImageXml(int nRow, IXMLDOMElement * pXml)
	{
		((xcImageView *)m_pControl)->AddImageXml(nRow, pXml);
	}*/
}}