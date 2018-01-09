#pragma once

#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	//this interface all index param start by one
	class XFlowChartObject;
	class XFlowChartLink;
	class XFlowSheet;

	class XFLOWCHART_API IFlowChart :public IXFControl
	{
	public:
		IFlowChart(void);
		~IFlowChart(void);
	public:
		XFlowSheet * m_pFlowChart;
	public:
		XFlowChartObject * GetSelectedObject();
		bool SelectedObject(XFlowChartObject * pObject);
	public:
		int GetObjectCount();
		XFlowChartObject * GetObjectByIndex(int nIndex);

		int GetLinkCount();
		XFlowChartLink * GetLinkbyIndex(int nIndex);
	public:
		bool RemoveObject(XFlowChartObject * pObject,bool bDeletedLink = true);
		XFlowChartObject * AddObject(LPTSTR pShapeType,LPTSTR pCaption,LPTSTR pUrl = NULL,RECT * pRect = NULL);
	public:
		bool SetobjectUrl(XFlowChartObject * pObject,LPTSTR pUrl);
		bool SetobjectCaption(XFlowChartObject * pObject,LPTSTR pCaption);
	public:
		XFlowChartLink * AddLink(XFlowChartObject * pFrom,XFlowChartObject * pTo,bool bDoubleArrow = true);
	public: 
		XFlowChartObject * GetObjectAt(POINT pt);
	public: 
		int Save(IXMLDOMElement * pElement);
		int Load(IXMLDOMElement * pElement);
	};
}}}}