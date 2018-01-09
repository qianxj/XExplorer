#include "StdAfx.h"
#include "xfNode.hpp"
#include "xfNodeResource.hpp"
#include "xuxutil.h"
#include "xfResourceMgr.hpp"
#include "xfuriloader.hpp"
#include "xfxpage.hpp"
#include "xfApp.hpp"
#include "xfCacheMgr.hpp"
#include "xfconnect.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeResource::xfNodeResource(void):m_pResourceMgr(NULL)
	{
		m_LayerFlow = LayerFlow_None | LayerFlow_Hidden;
	}

	xfNodeResource::~xfNodeResource(void)
	{
	}
	int xfNodeResource::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		int Result = xfNode::ParseNode(pNodeEx,false);

		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodePtr	pNodeTool;
		long toolen;
		VARIANT	sval;

		//imagelist
		HBITMAP	hBmp;
		pNodeEx->selectNodes(L"ximagelist",&pNodeList);
		pNodeList->get_length(&toolen);
		for( int l=0;l<toolen;l++)
		{
			pNodeList->get_item(l,&pNodeTool);
			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"src",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				TCHAR Path[MAX_PATH];
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				strUrl[0] = 0;
				xfUriLoader::UrlCombineW(this->GetXPage()->m_pStrUrl,sval.bstrVal,strUrl,&dwSize,0);

				LPTSTR pCacheFile = NULL;
				pCacheFile = (LPTSTR)xfApp::GetApp()->m_pCacheMgr->GetCacheFile(strUrl);
				if(pCacheFile)
					hBmp = (HBITMAP)::LoadImageW(NULL,pCacheFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
				else
				{
					bool reload = 
						(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
					if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
					{
						xfApp::GetApp()->m_pCacheMgr->InsertCacheFile(strUrl,Path);
						hBmp = (HBITMAP)::LoadImageW(NULL,Path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					}
					else
						hBmp = (HBITMAP)::LoadImageW(NULL,sval.bstrVal,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
				}
			}else
				hBmp = NULL;
			::VariantClear(&sval);

			int cx=16,cy=16;
			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"cx",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				WCHAR * pNextStr;
				cx = ::wcstol(sval.bstrVal,&pNextStr,10);
			}
			::VariantClear(&sval);

			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"cy",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				WCHAR * pNextStr;
				cy = ::wcstol(sval.bstrVal,&pNextStr,10);
			}
			::VariantClear(&sval);

			COLORREF	colorMask = 0;
			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"colormask",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				LPTSTR	str;
				str = XUtil::tcsdupFromBstr(sval.bstrVal);

				Hxsoft::XUtil::Css::TColor	color;
				if(Hxsoft::XUtil::Css::ProcessCssColorEx(str,color,0))
					colorMask = color.color[0];
				else
					colorMask = RGB(0,0,0);
			}
			::VariantClear(&sval);

			HIMAGELIST hImageList = ::ImageList_Create(cx,cy,ILC_COLORDDB|ILC_MASK,4,4);
			if(colorMask==0)
				::ImageList_Add(hImageList,hBmp,NULL);
			else
				::ImageList_AddMasked(hImageList,hBmp,colorMask);
			::DeleteObject(hBmp);

			m_pResourceMgr->AddImageList(hImageList);
		}

		//string
		LPTSTR pLocalFile;
		xbXml*	pXml = new xbXml();
		pNodeEx->selectNodes(L"string",&pNodeList);
		pNodeList->get_length(&toolen);
		for( int l=0;l<toolen;l++)
		{
			pNodeList->get_item(l,&pNodeTool);
			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"src",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				pLocalFile = XUtil::tcsdupFromBstr(sval.bstrVal);
				m_pResourceMgr->AddLocalFile(pLocalFile);
				delete pLocalFile;
			}
			::VariantClear(&sval);
		}

		return Result;
	}
}}
