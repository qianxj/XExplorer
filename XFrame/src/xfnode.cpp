#include "stdafx.h"
#include "xfnode.hpp"
#include "xfNodeBox.hpp"
#include "xfNodeControl.hpp"
#include "xfNodelayers.hpp"
#include "xfNodelayer.hpp"
#include "xfNoderesource.hpp"
#include "xfNodewin.hpp"
#include "xfNoderesource.hpp"
#include "xfXPage.hpp"
#include "xfNodeXPage.hpp"
#include "xuXUtil.h"
#include "xfWin.hpp"
#include "ixnode.hpp"
#include "xfNodescript.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNode::xfNode(void):m_pCaption(NULL),m_pParentNode(NULL),m_pxfPage(NULL),m_pElement(NULL),
			m_pCssStyle(NULL),m_hExtRate(0),m_vExtRate(0),m_width(0),m_height(0),m_minWidth(0),m_hopeWidth(0),
			m_strNodeName(NULL),m_strNodeID(NULL),m_strTagName(NULL),m_strClass(NULL),m_LayerFlow(LayerFlow_None),m_zOrder(0),m_nX(0),m_nY(0),
			m_nRX(0),m_nRY(0),m_posStatus(0),m_nCssIndex(-1),m_pCaptionEx(NULL),m_bSelection(false),m_bHover(false),
			m_nCssIndexSelection(-1),m_nCssIndexHover(-1),m_pData(NULL),m_pTabOrder(NULL),m_bDeleteing(false)

	{
		m_pChilds = new std::vector<xfNode *>;
		::SetRect(&m_AreaRect,0,0,0,0);
		::SetRect(&m_ContentRect,0,0,0,0);
	}

	xfNode::~xfNode(void)
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->m_pParentNode = NULL;
			if((*m_pChilds)[i])(*m_pChilds)[i]->Release();
		}
		m_pChilds->clear();
		delete m_pChilds;
		if(m_pElement)m_pElement->Release();
		if(m_pCaptionEx)delete m_pCaptionEx;
		if(m_pCssStyle)delete m_pCssStyle;
		if(m_strNodeName)delete m_strNodeName;
		if(m_strNodeID)delete m_strNodeID;
		if(m_strClass)delete m_strClass;
		if(m_strTagName)delete m_strTagName;
	}

	xfNode * xfNode::GetRootNode()
	{
		xfNode * pParentNode;
		pParentNode = this;
		while(pParentNode->m_pParentNode)pParentNode = pParentNode->m_pParentNode;
		return pParentNode;
	}

	xfNode * xfNode::GetNodeByName(LPCTSTR pStrName,bool bIncUnLayered)
	{
		if(!pStrName) return NULL;
		xfNode * pNode;
		if(m_strNodeName && _tcscmp(m_strNodeName,pStrName)==0) return this;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(!(*m_pChilds)[i] || (*m_pChilds)[i]->m_bDeleteing) continue;
			if((*m_pChilds)[i]->IsLayerNode())
			{
				pNode = (*m_pChilds)[i]->GetNodeByName(pStrName,false);
				if(pNode) return pNode;
			}
		}
		if(bIncUnLayered)
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(!(*m_pChilds)[i] || (*m_pChilds)[i]->m_bDeleteing) continue;
				pNode = (*m_pChilds)[i]->GetNodeByName(pStrName,true);
				if(pNode) return pNode;
			}
		}
		return NULL;
	}

	int xfNode::ParseCssStyle(LPCTSTR pStrCssStyle,XUtil::xuCssStyleSheet::eCssItemType et)	//parse cssstyle string
	{
		int nCssIndex = -1;
		xfXPage * pPage = GetXPage();
		if(pPage)
		{
			if(et==XUtil::xuCssStyleSheet::etCssNormal)
				return pPage->m_pStyleSheet->GetClassStyle(this->m_strTagName,
					this->m_strNodeID,m_strClass,pStrCssStyle,et);
			
			if(pStrCssStyle && pStrCssStyle[0]!='\0')
			{
				
				XUtil::Css::TBoxStyle * pBoxStyle = pPage->m_pCCssStyle->ParseCssStyle(pStrCssStyle);
				if(pBoxStyle)
				{
					nCssIndex = pPage->m_pCCssStyle->AddCssCellStyle(pBoxStyle);
					if(pBoxStyle)
						pBoxStyle->Release();
				}
			}
		}
		return nCssIndex;
	}
	int xfNode::SetCssStyle(LPCTSTR	cssStyle) //set cssstyle string ,next parse cssstyle,adjust child node or brother node
	{
		m_pCssStyle = ::_tcsdup(cssStyle);
		m_nCssIndex = ParseCssStyle(m_pCssStyle);
		return m_nCssIndex;
	}

	int xfNode::ClearCssStyle()
	{
		if(m_pCssStyle) delete m_pCssStyle;
		m_pCssStyle = NULL;
		m_nCssIndex = -1;
		return 1;
	}

	xfNode * xfNode::GetNodeByID(LPCTSTR pStrNodeID,bool bIncUnLayered)
	{
		xfNode * pNode;
		if(m_strNodeID && _tcscmp(m_strNodeID,pStrNodeID)==0) return this;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(!(*m_pChilds)[i] || (*m_pChilds)[i]->m_bDeleteing) continue;
			if((*m_pChilds)[i]->IsLayerNode())
			{
				pNode = (*m_pChilds)[i]->GetNodeByID(pStrNodeID,false);
				if(pNode) return pNode;
			}
		}
		if(bIncUnLayered)
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(!(*m_pChilds)[i] || (*m_pChilds)[i]->m_bDeleteing) continue;
				pNode = (*m_pChilds)[i]->GetNodeByID(pStrNodeID,true);
				if(pNode) return pNode;
			}
		}
		return NULL;
	}

	xfXPage * xfNode::GetXPage()
	{

		if(m_pxfPage)	return m_pxfPage;
		xfNode * pxfNode = m_pParentNode ;
		while(pxfNode)
		{
			if(pxfNode->m_pxfPage)	return pxfNode->m_pxfPage;
			pxfNode = pxfNode->m_pParentNode;
		}
		return NULL;
	}

	int xfNode::ParseNode(IXMLDOMElement* pNodeEx, bool bParseChild)
	{
		pNodeEx->AddRef();
		m_pElement = pNodeEx;

		m_pCaptionEx = xbXml::GetStringAttribe(m_pElement,_T("caption"));
		if(m_pCaptionEx)
		{
			m_pCaption = m_pCaptionEx; 
			if(m_pCaptionEx[0]=='[' && m_pCaptionEx[::_tcslen(m_pCaptionEx) - 1]==']')
			{
				m_pCaption++;
				m_pCaption[::_tcslen(m_pCaption) - 1]='\0';
				m_pCaption = (LPTSTR)GetXPage()->m_pResourceMgr->GetStringItem(GetXPage()->m_pStrLocal,m_pCaption);
				m_pCaptionEx[::_tcslen(m_pCaptionEx)]=']';
			}
		}

		m_strNodeID = xbXml::GetStringAttribe(m_pElement,_T("id"));
		m_strNodeName = xbXml::GetStringAttribe(m_pElement,_T("name"));
		m_strClass = xbXml::GetStringAttribe(m_pElement,_T("class"));
		
		BSTR str;
		m_pElement->get_tagName(&str);
		m_strTagName = _tcsdup(str);
		::SysFreeString(str);

		m_pCssStyle = xbXml::GetStringAttribe(m_pElement,_T("style"));
		/*if(m_pCssStyle)*/m_nCssIndex = ParseCssStyle(m_pCssStyle);

		_variant_t valex;
		m_pElement->getAttribute(L"taborder",&valex);
		if(valex.bstrVal)
			this->m_pTabOrder = valex;

		LPTSTR pStrStyle = NULL;
		pStrStyle = xbXml::GetStringAttribe(m_pElement,_T("selectionstyle"));
		if(pStrStyle)
		{
			m_nCssIndexSelection = ParseCssStyle(pStrStyle,XUtil::xuCssStyleSheet::etCssSelection);
			XUtil::FreeStr(pStrStyle);
		}

		pStrStyle = xbXml::GetStringAttribe(m_pElement,_T("hoverstyle"));
		if(pStrStyle)
		{
			m_nCssIndexHover = ParseCssStyle(pStrStyle,XUtil::xuCssStyleSheet::etCssHover);
			XUtil::FreeStr(pStrStyle);
		}

		VARIANT	val;
		pNodeEx->getAttribute(L"width",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			m_width = ::wcstol(val.bstrVal,&pNextStr,10);
			if(pNextStr && pNextStr[0]=='%')
				m_hExtRate = m_width,m_width = 0;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"height",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			m_height = ::wcstol(val.bstrVal,&pNextStr,10);
			if(pNextStr && pNextStr[0]=='%')m_vExtRate = m_height,m_height = 0;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"minsize",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_minWidth = ::wcstol(val.bstrVal,&pNextStr,10);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"hopesize",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_hopeWidth = ::wcstol(val.bstrVal,&pNextStr,10);
		}
		::VariantClear(&val);

		this->m_posStatus = 0;
		pNodeEx->getAttribute(L"x",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_nX= ::wcstol(val.bstrVal,&pNextStr,10);
			this->m_posStatus |= PosStatus_nX;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"y",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_nY = ::wcstol(val.bstrVal,&pNextStr,10);
			this->m_posStatus |= PosStatus_nY;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"rx",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_nRX= ::wcstol(val.bstrVal,&pNextStr,10);
			this->m_posStatus |= PosStatus_nRX;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"ry",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_nRY = ::wcstol(val.bstrVal,&pNextStr,10);
			this->m_posStatus |= PosStatus_nRY;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"break",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"row")==0 || ::_wcsicmp(val.bstrVal,L"yes")==0 ||
				::_wcsicmp(val.bstrVal,L"1")==0 )
				this->m_posStatus |= PosStatus_Row;
			else if(::_wcsicmp(val.bstrVal,L"para")==0)
				this->m_posStatus |= PosStatus_Paragraph;
			else if(::_wcsicmp(val.bstrVal,L"page")==0)
				this->m_posStatus |= PosStatus_Page;

		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"layerflow",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"none")==0)
				this->m_LayerFlow = LayerFlow_None;
			else if(::_wcsicmp(val.bstrVal,L"inherit")==0)
			{
				if(m_pParentNode) m_LayerFlow = m_pParentNode->m_LayerFlow;
			}
			else if(::_wcsicmp(val.bstrVal,L"ns")==0)
				this->m_LayerFlow = LayerFlow_NS;
			else if(::_wcsicmp(val.bstrVal,L"we")==0)
				this->m_LayerFlow = LayerFlow_WE;
			else if(::_wcsicmp(val.bstrVal,L"htmlflow")==0)
				this->m_LayerFlow = LayerFlow_HtmlFlow;
			else if(::_wcsicmp(val.bstrVal,L"sheets")==0)
				this->m_LayerFlow = LayerFlow_Sheets;
			else if(::_wcsicmp(val.bstrVal,L"control")==0)
				this->m_LayerFlow = LayerFlow_Control;
			else if(::_wcsicmp(val.bstrVal,L"freeform")==0)
				this->m_LayerFlow = LayerFlow_FreeForm;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"visible",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"hidden")==0)
				this->m_LayerFlow |= LayerFlow_Hidden;
			else if(::_wcsicmp(val.bstrVal,L"unlayer")==0)
				this->m_LayerFlow |= LayerFlow_UnLayer;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"zorder",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			this->m_zOrder = ::wcstol(val.bstrVal,&pNextStr,10);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"selection",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"true")==0 || ::_wcsicmp(val.bstrVal,L"yes")==0|| ::_wcsicmp(val.bstrVal,L"1")==0)
				m_bSelection = true;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"hover",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"true")==0 || ::_wcsicmp(val.bstrVal,L"yes")==0|| ::_wcsicmp(val.bstrVal,L"1")==0)
				m_bHover = true;
		}
		::VariantClear(&val);


		if(bParseChild)
		{
			IXMLDOMNodeList*	pList;
			IXMLDOMNode*		pNode;
			IXMLDOMElement*		pElement;
			long				len;

			pNodeEx->selectNodes(L"*",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				pList->get_item(i,&pNode);
				pElement = xbXml::GetElement(pNode);
				xfNode * pxfNode = CreateNode(pElement);
				if(pxfNode)
				{
					pxfNode->m_pParentNode = this;
					m_pChilds->push_back(pxfNode);
					pxfNode->ParseNode(pElement);
				}
				pElement->Release();
				pNode->Release();
			}
			pList->Release();
		}
		return 1;
	}

	xfNode * xfNode::CreateNode(IXMLDOMElement * pElement,bool setClassName)
	{
		xfNode * pNode;
		BSTR bstrItem= NULL;
		pElement->get_tagName(&bstrItem);
		if(::wcscmp(bstrItem,L"xframe")==0)
			pNode = new xfNodeWin();
		else if(::wcscmp(bstrItem,L"sheet")==0)
			pNode = new xfNodeWin();
		else if(::_wcsicmp(bstrItem,L"resource")==0)
		{
			pNode = NULL;
		}
		else if(::_wcsicmp(bstrItem,L"box")==0)
			pNode = new xfNodeBox();
		else if(::_wcsicmp(bstrItem,L"hbox")==0)
		{
			pNode = new xfNodeBox();
			pNode->m_LayerFlow = LayerFlow_WE;
		}
		else if(::_wcsicmp(bstrItem,L"vbox")==0)
		{
			pNode = new xfNodeBox();
			pNode->m_LayerFlow = LayerFlow_NS;
		}
		else if(::_wcsicmp(bstrItem,L"layers")==0)
			pNode = new xfNodeLayers();
		else if(::_wcsicmp(bstrItem,L"layer")==0)
			pNode = new xfNodeLayer();
		else if(::_wcsicmp(bstrItem,L"control")==0)
			pNode = new xfNodeControl();
		else if(::_wcsicmp(bstrItem,L"xpage")==0)
			pNode = new xfNodeXPage();
		else if(::_wcsicmp(bstrItem,L"script")==0)
			pNode = new xfNodeScript();
		else if(::_wcsicmp(bstrItem,L"args")==0   || 
			    ::_wcsicmp(bstrItem,L"params")==0 ||
			    ::_wcsicmp(bstrItem,L"note")==0   ||
				::_wcsicmp(bstrItem,L"ref")==0)
			return NULL;
		else 
			pNode = new xfNodeControl();
		if(pNode)
			if(setClassName)pNode->SetClassName(NULL,xbXml::BStr2T(bstrItem));
		::SysFreeString(bstrItem);

		return pNode;
	}

	bool	xfNode::IsLayerNode()
	{
		return !(m_LayerFlow & LayerFlow_NotLayerNode );
	}

	bool	xfNode::ShouldLayerChild()
	{
		return (m_LayerFlow & LayerFlow_LayerChild && !(m_LayerFlow & LayerFlow_UnLayer)) ? true :false;
	}

	int xfNode::AdjustRect()
	{
		if(m_AreaRect.left == 0 && m_AreaRect.right == 0 && m_AreaRect.top == 0 && m_AreaRect.bottom==0)
		{
			::SetRect(&m_ContentRect,0,0,0,0);
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				(*m_pChilds)[i]->m_AreaRect = m_AreaRect;
			}

		}else
		{
			this->CalcContentRect();

			RECT	rect = m_ContentRect;
			RECT	rectex;

			if(m_LayerFlow & LayerFlow_WE)
			{
				int		width = rect.right - rect.left;
				int		startX = rect.left;
				int		zeroWidthCount = 0;
				double	tRate=0;
				int		bwidth=0;
				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;
					if((*m_pChilds)[i]->m_width !=0)
						width -= (*m_pChilds)[i]->m_width;
					else if((*m_pChilds)[i]->m_width ==0 &&  (*m_pChilds)[i]->m_hExtRate==0)
						zeroWidthCount += 1;
					else
						tRate += (*m_pChilds)[i]->m_hExtRate;
				}
				if(zeroWidthCount > 0 && width > 0)
				{
					bwidth = (int)(width * ( 100 - tRate) /100);
					if(bwidth < 2) bwidth = 0;
					bwidth = bwidth/zeroWidthCount;
				}
				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;

					rectex = rect;
					rectex.left = startX;
					if((*m_pChilds)[i]->m_width !=0)
						rectex.right = rectex.left + (*m_pChilds)[i]->m_width - 1;
					else if((*m_pChilds)[i]->m_hExtRate >0 && width > 0)
					{
						if(width - (long)(width * (*m_pChilds)[i]->m_hExtRate/100.0) > 2)
						{
							rectex.right = rectex.left + (long)((width * (*m_pChilds)[i]->m_hExtRate)/100.0) - 1;
							width -= (long)((width * (*m_pChilds)[i]->m_hExtRate)/100.0) - 1;
						}else
						{
							rectex.right = rectex.left + width - 1;
							width = 0;
						}
					}else if(width > 2 && (*m_pChilds)[i]->m_width==0 && bwidth > 0)
					{
						rectex.right = rectex.left + bwidth;

					}else
						rectex.right = rectex.left;
					startX = rectex.right + 1;

					if((*m_pChilds)[i]->m_height > 0)
					{
						rectex.bottom = rectex.top + (*m_pChilds)[i]->m_height - 1;
						if(rectex.bottom > rect.bottom) rectex.bottom = rect.bottom;
					}
					(*m_pChilds)[i]->m_AreaRect = rectex;
				}
			}else if(m_LayerFlow & LayerFlow_NS)
			{
				int		height = rect.bottom - rect.top;
				int		startY = rect.top;
				int		zeroheightCount = 0;
				double	tRate=0;
				int		bheight=0;
				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;
					if((*m_pChilds)[i]->m_height !=0)
						height -= (*m_pChilds)[i]->m_height;
					else if((*m_pChilds)[i]->m_height ==0 &&  (*m_pChilds)[i]->m_vExtRate==0)
						zeroheightCount += 1;
					else
						tRate += (*m_pChilds)[i]->m_vExtRate;
				}
				if(zeroheightCount > 0 && height > 0)
				{
					bheight = (int)(height * ( 100 - tRate) /100);
					if(bheight < 2) bheight = 0;
					bheight = bheight/zeroheightCount;
				}
				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;

					rectex = rect;
					rectex.top = startY;
					if((*m_pChilds)[i]->m_height !=0)
						rectex.bottom = rectex.top + (*m_pChilds)[i]->m_height - 1;
					else if((*m_pChilds)[i]->m_vExtRate >0 && height > 0)
					{
						if(height - (long)(height * (*m_pChilds)[i]->m_vExtRate)/100.0 > 2)
						{
							rectex.bottom = rectex.top + (long)((height * (*m_pChilds)[i]->m_vExtRate)/100.0) - 1;
							height -= (long)((height * (*m_pChilds)[i]->m_vExtRate)/100.0) - 1;
						}else
						{
							rectex.bottom = rectex.top + height - 1;
							height = 0;
						}
					}else if(height > 2 && (*m_pChilds)[i]->m_height==0 && bheight > 0)
					{
						rectex.bottom = rectex.top + bheight;

					}else
						rectex.bottom = rectex.top;
					startY = rectex.bottom + 1;

					if((*m_pChilds)[i]->m_width > 0)
					{
						rectex.right = rectex.left + (*m_pChilds)[i]->m_width - 1;
						if(rectex.right > rect.right) rectex.right = rect.right;
					}
					(*m_pChilds)[i]->m_AreaRect = rectex;
				}
			}else if(m_LayerFlow & LayerFlow_FreeForm)
			{
				int		height = rect.bottom - rect.top ;
				int		width = rect.right - rect.left;
				int		nX;
				int		nY;
				RECT	prect;

				::SetRect(&prect,rect.left,rect.top,rect.left,rect.top);
				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;

					nX = (*m_pChilds)[i]->m_nX;
					nY = (*m_pChilds)[i]->m_nY;
					if((*m_pChilds)[i]->m_posStatus & PosStatus_nX)
						rectex.left = rect.left + nX;
					else
						rectex.left = prect.right + (*m_pChilds)[i]->m_nRX;

					if((*m_pChilds)[i]->m_posStatus & PosStatus_nY)
						rectex.top = rect.top + nY;
					else 
						rectex.top = prect.top + (*m_pChilds)[i]->m_nRY;

					if((*m_pChilds)[i]->m_width > 0)
						rectex.right = rectex.left + (*m_pChilds)[i]->m_width -1;
					else
					{
						if ((*m_pChilds)[i]->m_hExtRate > 0)
							rectex.right = rectex.left + (long)((width * (*m_pChilds)[i]->m_hExtRate)/100.0) -1;
						else
							continue;
					}
					if((*m_pChilds)[i]->m_height > 0)
						rectex.bottom = rectex.top + (*m_pChilds)[i]->m_height -1;
					else
					{
						if ((*m_pChilds)[i]->m_vExtRate > 0)
							rectex.bottom = rectex.top + (long)(((height * (*m_pChilds)[i]->m_vExtRate)/100.0)) -1;
						else
							continue;
					}
					(*m_pChilds)[i]->m_AreaRect = rectex;
					prect = rectex;
				}
			}else if(m_LayerFlow & LayerFlow_HtmlFlow)
			{
				int		height = rect.bottom - rect.top ;
				int		width = rect.right - rect.left;

				int		startX0 = rect.left;
				int		startY0 = rect.top;

				int		startX = rect.left;
				int		startY = rect.top;

				int		dx=0;
				int		dy=0;
				int		mheight = 0;
				int		nwidth  = 0;
				int		nheight = 0;
				int		nLastAmt = 17;

				for(int i=0;i<(int)m_pChilds->size();i++)
				{
					if(!(*m_pChilds)[i]->IsLayerNode())continue;

					//get nwidth
					if((*m_pChilds)[i]->m_width > 0)
						nwidth = (*m_pChilds)[i]->m_width;
					else
					{
						if ((*m_pChilds)[i]->m_hExtRate > 0)
							nwidth =  (long)((width * (*m_pChilds)[i]->m_hExtRate)/100.0);
						else
							continue;
					}

					//get nheight
					if((*m_pChilds)[i]->m_height > 0)
						nheight =  (*m_pChilds)[i]->m_height;
					else
					{
						if ((*m_pChilds)[i]->m_vExtRate > 0)
							nheight =  (long)((height * (*m_pChilds)[i]->m_vExtRate)/100.0) ;
						else
							continue;
					}
					mheight = mheight > nheight ? mheight : nheight;

					if(nwidth + startX > rect.right)
					{
						if(startX != startX0)
						{
							startY += mheight;
							startX = startX0;
						}
					}
					if(startY + nheight > rect.bottom)
					{
						if(startY > rect.bottom) break;
						if(startY > rect.bottom - nLastAmt) continue;
					}

					rectex.left = startX ;
					rectex.top  = startY  ;
					rectex.right = startX + nwidth -1;
					rectex.bottom = startY + nheight - 1;
					if(rectex.right > rect.right) rectex.right = rect.right;
					if(rectex.bottom > rect.bottom) rectex.bottom = rect.bottom;
					startX = rectex.right + 1;

					(*m_pChilds)[i]->m_AreaRect = rectex;
				}
			}
		}
		if(ShouldLayerChild())
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(!(*m_pChilds)[i]->IsLayerNode())continue;
				(*m_pChilds)[i]->AdjustRect();
			}
		}
		return 1;
	}

	//adjust content rect,adjust child node or brother node
	int xfNode::AdjustRect(RECT rect)
	{
		m_AreaRect = rect;
		return AdjustRect();
	}

	int xfNode::CalcContentRect()
	{
		m_ContentRect = m_AreaRect;

		const XUtil::Css::TBoxStyle * pCssBoxStyle;
		if(this->m_nCssIndex >=0)
		{
			pCssBoxStyle = GetXPage()->m_pCCssStyle->GetBoxStyle(m_nCssIndex);
			if(pCssBoxStyle && pCssBoxStyle->mask & cssMaskMargin)
			{
				if(pCssBoxStyle->pMargin[0])
				{
					int valuex =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0]);
					int valuey =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[0],false);
					m_ContentRect.left += valuex;
					m_ContentRect.right -= valuey;
					m_ContentRect.top += valuex;
					m_ContentRect.bottom -= valuey;
				}
				if(pCssBoxStyle->pMargin[1])m_ContentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[1]);
				if(pCssBoxStyle->pMargin[2])m_ContentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[2],false);
				if(pCssBoxStyle->pMargin[3])m_ContentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[3]);
				if(pCssBoxStyle->pMargin[4])m_ContentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pMargin[4],false);

			}
			if(pCssBoxStyle && pCssBoxStyle->mask & cssMaskPadding)
			{
				if(pCssBoxStyle->pPadding[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[0]);
					m_ContentRect.left += value;
					m_ContentRect.right -= value;
					m_ContentRect.top += value;
					m_ContentRect.bottom -= value;
				}
				if(pCssBoxStyle->pPadding[1])m_ContentRect.left += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[1]);
				if(pCssBoxStyle->pPadding[2])m_ContentRect.top += (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[2],false);
				if(pCssBoxStyle->pPadding[3])m_ContentRect.right -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[3]);
				if(pCssBoxStyle->pPadding[4])m_ContentRect.bottom -= (int) XUtil::Css::GetPixelExtent(pCssBoxStyle->pPadding[4],false);

			}
			if(pCssBoxStyle && pCssBoxStyle->mask & cssMaskBorder)
			{
				if(pCssBoxStyle->pBorder[0])
				{
					int value =(int) XUtil::Css::GetPixelExtent(&pCssBoxStyle->pBorder[0]->widthmesure);
					m_ContentRect.left += value;
					m_ContentRect.right -= value;
					m_ContentRect.top += value;
					m_ContentRect.bottom -= value;
				}
				if(pCssBoxStyle->pBorder[1])m_ContentRect.left += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[1]);
				if(pCssBoxStyle->pBorder[2])m_ContentRect.top += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[2],false);
				if(pCssBoxStyle->pBorder[3])m_ContentRect.right -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[3]);
				if(pCssBoxStyle->pBorder[4])m_ContentRect.bottom -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[4],false);
			}
		}
		return 1;
	}

	int xfNode::AdjustRect(int dx)
	{
		return 1;
	}

	int xfNode::SetWidthHeight(int width, int height)
	{
		if(width >= 0)
		{
			this->m_width = width;
			//this->m_AreaRect.right = this->m_AreaRect.left + width;
		}
		if(height >= 0)
		{
			this->m_height = height;
			//this->m_AreaRect.bottom = this->m_AreaRect.top + height;
		}
		return 1;
	}
	int xfNode::MoveTo(LPTSTR Name)
	{
		xfNode * pNode = GetRootNode()->GetNodeByName(Name);
		if(!pNode) return -1;
		if(this->m_pParentNode && this->m_pParentNode == pNode) return -1;
		for(int i=(int)m_pParentNode->m_pChilds->size() - 1;i>=0;i--)
		{
			if((*m_pParentNode->m_pChilds)[i] == this)
			{
				m_pParentNode->m_pChilds->erase(m_pParentNode->m_pChilds->begin() + i);
				break;
			}
		}
		pNode->m_pChilds->push_back(this);
		this->m_pParentNode = pNode;
		int height = m_height == 0?m_AreaRect.bottom - m_AreaRect.top:m_height;
		int width = m_width == 0?m_AreaRect.right - m_AreaRect.left:m_width;
		if(pNode->m_LayerFlow == LayerFlow_NS)
		{
			this->m_AreaRect.bottom = pNode->m_AreaRect.bottom;
			this->m_AreaRect.top = pNode->m_AreaRect.bottom - height;
			this->m_AreaRect.left = pNode->m_AreaRect.left;
			this->m_AreaRect.right = pNode->m_AreaRect.left + width;
		}
		else if(pNode->m_LayerFlow == LayerFlow_WE)
		{
			this->m_AreaRect.right = pNode->m_AreaRect.right;
			this->m_AreaRect.left = pNode->m_AreaRect.right - width;
			this->m_AreaRect.top = pNode->m_AreaRect.top;
			this->m_AreaRect.bottom = pNode->m_AreaRect.top + height;
		}
		else
		{
			this->m_AreaRect.right = pNode->m_AreaRect.right;
			this->m_AreaRect.left = pNode->m_AreaRect.right - width;
			this->m_AreaRect.bottom = pNode->m_AreaRect.bottom;
			this->m_AreaRect.top = pNode->m_AreaRect.bottom - height;
		}
		return 1;
	}
	int xfNode::DrawNode(HDC hDC)
	{
		RECT	rect;
		rect = this->m_AreaRect;
		int nCssIndex;
		if(m_bHover && m_nCssIndexHover >=0)
			nCssIndex = m_nCssIndexHover;
		else if(this->m_bSelection && m_nCssIndexSelection >=0)
			nCssIndex = m_nCssIndexSelection;
		else
			nCssIndex = m_nCssIndex;

		if(nCssIndex>=0)
		{
			const XUtil::Css::tagBoxStyle * pCssStyle = GetXPage()->m_pCCssStyle->GetBoxStyle(nCssIndex);
			if(pCssStyle && (( pCssStyle->mask & cssMaskBackGround && pCssStyle->pBackGround)||(pCssStyle->mask&cssMaskBorder && pCssStyle->pBorder)))
			{
				Hxsoft::XUtil::Css::DrawCssFrame(hDC, &rect,pCssStyle);
				//delete pCssStyle;
			}
			/*
			if(pCssStyle)
			{
				if(pCssStyle->mask & cssMaskBackGround)
				{
					if(pCssStyle->pBackGround->mask & cssMaskBackGroundColor)
					{
						if(pCssStyle->pBackGround->color.mask & cssMaskColor)
						{
							COLORREF color = pCssStyle->pBackGround->color.color[0];
							HBRUSH hBrush = ::CreateSolidBrush(color);
							::FillRect(hDC,&rect,hBrush);
							::DeleteObject(hBrush);
						}
						if(pCssStyle->pBackGround->color.mask & cssMaskLeft && pCssStyle->pBackGround->color.colorRef[1]!=XUtil::Css::_undef && 
							pCssStyle->pBackGround->color.mask & cssMaskRight && pCssStyle->pBackGround->color.colorRef[3]!=XUtil::Css::_undef)
						{
							TRIVERTEX	vertex[2];
							GRADIENT_RECT grect[1];

							vertex[0].x = rect.left;
							vertex[0].y = rect.top;
							vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
							vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
							vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;

							vertex[1].x = rect.right;
							vertex[1].y = rect.bottom;
							vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
							vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[3])*256;
							vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;

							grect[0].UpperLeft = 1;
							grect[0].LowerRight = 0;
							GradientFill(hDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
						}else if(pCssStyle->pBackGround->color.mask & cssMaskTop && pCssStyle->pBackGround->color.colorRef[2]!=XUtil::Css::_undef && 
							pCssStyle->pBackGround->color.mask & cssMaskBottom && pCssStyle->pBackGround->color.colorRef[4]!=XUtil::Css::_undef)
						{
							TRIVERTEX	vertex[2];
							GRADIENT_RECT grect[1];

							vertex[0].x = rect.left;
							vertex[0].y = rect.top;
							vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
							vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
							vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;

							vertex[1].x = rect.right;
							vertex[1].y = rect.bottom;
							vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
							vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[4])*256;
							vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;

							grect[0].UpperLeft = 1;
							grect[0].LowerRight = 0;
							GradientFill(hDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
						}
					}
				}
			}*/
		}
		if(ShouldLayerChild())
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(!(*m_pChilds)[i]->IsLayerNode())continue;
				(*m_pChilds)[i]->DrawNode(hDC);
			}
		}
		return 1;
	}
	int xfNode::CreateNodeControl(HWND	hWnd)
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->CreateNodeControl(hWnd);
		}
		return 1;
	}
	int xfNode::AdjustCtrlRect()
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->AdjustCtrlRect();
		}
		return 1;
	}

	int xfNode::RemoveControl()
	{
		return 1;
	}

	int xfNode::OnClose()
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if((*m_pChilds)[i])
				(*m_pChilds)[i]->OnClose();
		}
		return 1;
	}


	int xfNode::RemoveChild()
	{
		return RemoveChild(this);
	}
	int xfNode::RemoveChild(xfNode * pParentNode)
	{
		if(!pParentNode) return 0;
		for(int i=(int)pParentNode->m_pChilds->size() - 1;i>=0;i--)
		{
			if((*pParentNode->m_pChilds)[i])
			{
				RemoveChild((*pParentNode->m_pChilds)[i]);
				(*pParentNode->m_pChilds)[i]->m_bDeleteing = true;
				(*pParentNode->m_pChilds)[i]->RemoveControl();
				xfNode * pNode = (*pParentNode->m_pChilds)[i];
				pParentNode->m_pChilds->erase(pParentNode->m_pChilds->begin() + i);
				delete pNode;
			}
		}
		return 1;
	}
	
	int xfNode::ShowControl(bool bShow)
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{

			if((*m_pChilds)[i]->IsLayerNode())(*m_pChilds)[i]->ShowControl(bShow);
		}
		return 1;
	}
	xfNode * xfNode::GetLabelNode(bool bSelection ,LPCTSTR pLayerFrame,LPCTSTR pLayerSheet)
	{
		xfNode * pNode;
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			pNode = (*m_pChilds)[i]->GetLabelNode(bSelection,pLayerFrame,pLayerSheet);
			if(pNode) return pNode;
		}
		return NULL;
	}

	HWND  xfNode::GetHWND()
	{
		//return this->GetRootNode()->m_pxfPage->m_pWin->GetHWND();
		return this->GetXPage()->m_pWin->GetHWND();
	}

	bool  xfNode::GetClientRect(RECT *pRect) 
	{
		*pRect = this->m_AreaRect;
		return true;
	}
	int xfNode::GetNodesByTab(std::set<xfNode *,nodetab_less> & pSetNode)
	{
		if(m_pTabOrder>0) pSetNode.insert(this);
		for(int i=0;i<(int)this->m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->GetNodesByTab(pSetNode);
		}
		return (int)pSetNode.size();
	}

	int xfNode::GetXml(BSTR &bstr)
	{
		bstr = NULL;
		if((int)this->m_pChilds->size() >0)
		{
			BSTR * pbstr = new BSTR[(int)this->m_pChilds->size()];
			int nLen = 0;
			for(int i=0;i<(int)this->m_pChilds->size();i++)
			{
				(*m_pChilds)[i]->GetXml(pbstr[i]);
				if(pbstr[i]) nLen += (int)wcslen(pbstr[i]);
			}
			if(nLen > 0)
			{
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				bstr[0] = '\0';
				for(int i=0;i<(int)this->m_pChilds->size();i++)
				{
					if(pbstr[i])
					{
						::wcscat_s(bstr,nLen + 1,pbstr[i]);
						::SysFreeString(pbstr[i]);
					}
				}
			}
			if(pbstr)delete pbstr;
		}
		return 1;
	}

	xbObject * xfNode::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IXNode();
		((IXNode *)m_pInterFace)->m_pNode = this;
		return m_pInterFace;
	}

}}