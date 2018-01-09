#pragma once

#include "xframe.h"
#include "xcImageView.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXImageView : public IXFControl
	{
	public:
		IXImageView();
		~IXImageView();
	public:
		int AddImages(LPCTSTR pUrl, LPTSTR pText=NULL);
		int InsertImages(LPCTSTR pUrl, LPTSTR pText, int nIndex);
		int RemoveImage(int nIndex);
		int RemoveImage();
		int GetSelectIndex();
		int SetSelectIndex(int nIndex);
		int GetImageCount();
		int Reset();
		LPTSTR GetImageUrl(int nIndex);
		int SetImageLabel(int nIndex, LPTSTR pText);
		LPTSTR GetImageLabel(int nIndex);
		int Preview(int percent);
		int SelectImage(int nRow,bool bSelect=true);
		int SelectImage(int sRow,int eRow, bool bSelect=true);
		bool IsImageSelected(int nRow);
		int GetNextSelectImage(int nStartRow);
		void SetSelectedMode(int mode);
		//void AddImageXml(int nRow, IXMLDOMElement * pXml);
	};
}}