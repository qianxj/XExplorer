#pragma once

#include "xframe.h"
#include "xclayersheet.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API ILayerSheet : public IXFControl
	{
	public:
		int InsertSheet(int nIndex,LPTSTR pText,IXMLDOMElement * pElement,int iImage = -1,bool bToCurrent = false);
		int InsertSheet(int nIndex,LPTSTR pText, IXNode * pNode,int iImage = -1,bool bToCurrent = false,bool bCreateControl = true);

		int InsertSheet(int nIndex,LPTSTR pText,IXMLDOMElement * pElement,void *,int iImage = -1,bool bToCurrent = false);
		int InsertSheet(int nIndex,LPTSTR pText, IXNode * pNode,void *,int iImage = -1,bool bToCurrent = false,bool bCreateControl = true);

		int DeleteSheet(int nIndex);
		bool SelectSheet(int nIndex);
		int Clear();

		int GetSheetCount();
		int GetSheetIndex();
	public:
		ILayerSheet();
		~ILayerSheet();
	public:
		xcLayerSheet*	m_pLayerSheet;
	public:
		vector<void *>* m_vcParam;
	public:
		bool SetParam(int nIndex,void *);
		void * GetParam(int nIndex);
	public:
		int SetSheetState(int sheet, int state);
		int GetSheetState(int sheet);
		int SetSheetText(int sheet, LPTSTR text);
		LPTSTR GetSheetText(int sheet);
	};
}}