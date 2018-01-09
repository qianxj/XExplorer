#include "stdafx.h"
#include "DevWorkDlg.h"
#include "DevView.h"

enum enWork { eWorkNULL=-1,
			  eNewWorkGroup,
			  eNewWorkItem,
			  eNewWorkNode,
			  eWorkGroup,
			  eWorkItem,
			  eWorkNode,
			  eNewBFFolder,
			  eBFFolder,
			  eNewDocFolder,
			  eDocFolder,
			  eNewWorkRulerEx,
			  eWorkRulerEx,
			  eNewWorkDocEx,
			  eWorkDocEx,
			  eNewWorkDoc,
			  eWorkDoc,
			  eNewWorkEntry,
			  eWorkEntry,
			  eNewWorkTemplate,
			  eWorkTemplate,
			  eNewWorkList,
			  eWorkList,
			  eNewWorkReport,
			  eWorkReport,
			  eNewWorkXPage,
			  eWorkXPage,
			  eNewWorkCode,
			  eWorkCode,
			  eNewWorkResource,
			  eWorkResource,
			  eNewWorkXQuery,
			  eWorkXQuery,
			  eNewWorkXAction,
			  eWorkXAction,
			  eNewWorkSerialize,
			  eWorkSerialize,
			  eNewWorkRuler,
			  eWorkRuler,
			  eNewWorkXFile,
			  eWorkXFile,
			  eNewWorkXMode,
			  eWorkXMode,
			  eWorkCount};
struct tagWorkMsg
{
	enWork eWork;
	LPTSTR pStrCommand;
	int iImage;
	LPTSTR pStrItemName;
	LPTSTR pStrItemTitle;
	LPTSTR pStrInputMsg;
	LPTSTR pStrErrorMsg;
	LPTSTR pStrCmd;
	LPTSTR pTagName;
};
tagWorkMsg workMsg[]=
{
	{eNewWorkGroup,_T("xmDevNewWorkGroup"),15,_T("工作组"),  _T("新建工作组"),  _T("请输入工作组名称"),  _T("新建工作组出错"),  _T("addworkgroup"),NULL},
	{eNewWorkItem, _T("xmDevNewWorkItem"), 12,_T("工作项"),	 _T("新建工作项"),  _T("请输入工作项名称"),  _T("新建工作项出错"),  _T("addworkitem"),NULL },
	{eNewWorkNode, _T("xmDevNewWorkNode"), 21,_T("工作节点"),_T("新建工作节点"),_T("请输入工作节点名称"),_T("新建工作节点出错"),_T("addworknode"),_T("worknode") },
	{eWorkGroup,   _T("xmDevWorkGroup"),15,_T("工作组"),  _T("修改工作组"),  _T("请输入工作组名称"),  _T("修改工作组出错"),  _T("updateworkgroup"),NULL},
	{eWorkItem,    _T("xmDevWorkItem"), 12,_T("工作项"),	 _T("修改工作项"),  _T("请输入工作项名称"),  _T("修改工作项出错"),  _T("updateworkitem"),NULL },
	{eWorkNode,    _T("xmDevWorkNode"), 21,_T("工作节点"),_T("修改工作节点"),_T("请输入工作节点名称"),_T("修改工作节点出错"),_T("updateworknode"),_T("worknode") },
	
	{eNewBFFolder,    _T("xmDevNewBFFolder"), 3,_T("流程目录"),_T("修改流程目录"),_T("请输入流程目录名称"),_T("修改流程目录出错"),_T("addbffolder"),_T("bffolder") },
	{eBFFolder,    _T("xmDevBFFolder"), 3,_T("流程目录"),_T("修改流程目录"),_T("请输入流程目录名称"),_T("修改流程目录出错"),_T("updatebffolder"),_T("bffolder") },

	{eNewDocFolder,    _T("xmDevNewDocFolder"), 3,_T("需求目录"),_T("修改需求目录"),_T("请输入需求目录名称"),_T("修改需求目录出错"),_T("adddocfolder"),_T("docfolder") },
	{eDocFolder,    _T("xmDevDocFolder"), 3,_T("需求目录"),_T("修改需求目录"),_T("请输入需求目录名称"),_T("修改需求目录出错"),_T("updatedocfolder"),_T("docfolder") },

	{eNewWorkRulerEx,    _T("xmDevNewRulerEx"), 18,_T("规则"),_T("增加规则"),_T("请输入规则名称"),_T("增加规则出错"),_T("addworkrulerex"),_T("bffile") },
	{eWorkRulerEx,		 _T("xmDevWorkRulerEx"), 18,_T("规则"),_T("修改规则"),_T("请输入规则名称"),_T("修改规则出错"),_T("updateworkrulerex"),_T("bffile") },

	{eNewWorkDocEx,    _T("xmDevNewDocEx"), 18,_T("文档"),_T("增加文档"),_T("请输入文档名称"),_T("增加文档出错"),_T("addworkrequestex"),_T("reqfile") },
	{eWorkDocEx,		 _T("xmDevWorkDocEx"), 18,_T("文档"),_T("修改文档"),_T("请输入文档名称"),_T("修改文档出错"),_T("updateworkrequestex"),_T("reqfile") },

	{eNewWorkDoc,    _T("xmDevNewDoc"), 18,_T("文档"),_T("增加文档"),_T("请输入文档名称"),_T("增加文档出错"),_T("addworkrequest"),_T("request") },
	{eWorkDoc,		 _T("xmDevWorkDoc"), 18,_T("文档"),_T("修改文档"),_T("请输入文档名称"),_T("修改文档出错"),_T("updateworkrequest"),_T("request") },
	{eNewWorkEntry,    _T("xmDevNewEntry"), 22,_T("数据结构"),_T("增加数据结构"),_T("请输入数据结构名称"),_T("增加数据结构出错"),_T("addworkentry"),_T("entry") },
	{eWorkEntry,		 _T("xmDevWorkEntry"), 22,_T("数据结构"),_T("修改数据结构"),_T("请输入数据结构名称"),_T("修改数据结构出错"),_T("updateworkentry"),_T("entry") },
	{eNewWorkTemplate,    _T("xmDevNewTemplate"), 5,_T("模板"),_T("增加模板"),_T("请输入模板名称"),_T("增加模板出错"),_T("addworktemplate"),_T("template")},
	{eWorkTemplate,		 _T("xmDevWorkTemplate"), 5,_T("模板"),_T("修改模板"),_T("请输入模板名称"),_T("修改模板出错"),_T("updateworktemplate"),_T("template") },
	{eNewWorkList,    _T("xmDevNewList"), 29,_T("工作流"),_T("增加工作流"),_T("请输入工作流名称"),_T("增加工作流出错"),_T("addworklist"),_T("worklist") },
	{eWorkList,		 _T("xmDevWorkList"), 29,_T("工作流"),_T("修改工作流"),_T("请输入工作流名称"),_T("修改工作流出错"),_T("updateworklist"),_T("worklist") },
	{eNewWorkReport,    _T("xmDevNewReport"),25 ,_T("报表"),_T("增加报表"),_T("请输入报表名称"),_T("增加报表出错"),_T("addworkreport"),_T("report") },
	{eWorkReport,		 _T("xmDevWorkReport"), 25,_T("报表"),_T("修改报表"),_T("请输入报表名称"),_T("修改报表出错"),_T("updateworkreport"),_T("report") },

	{eNewWorkXPage,    _T("xmDevNewXPageFile"),25 ,_T("XPage页面"),_T("增加XPage页面"),_T("请输入XPage页面名称"),_T("增加XPage页面出错"),_T("addworkxpage"),_T("xpage") },
	{eWorkXPage,		 _T("xmDevWorkXPage"), 25,_T("XPage页面"),_T("修改XPage页面"),_T("请输入XPage页面名称"),_T("修改XPage页面出错"),_T("updateworkxpage"),_T("xpage") },
	{eNewWorkCode,    _T("xmDevNewCodeFile"),25 ,_T("代码"),_T("增加代码"),_T("请输入代码名称"),_T("增加代码出错"),_T("addworkcode"),_T("code") },
	{eWorkCode,		 _T("xmDevWorkCode"), 25,_T("代码"),_T("修改代码"),_T("请输入代码名称"),_T("修改代码出错"),_T("updateworkcode"),_T("code") },
	{eNewWorkResource,    _T("xmDevNewResourceFile"),25 ,_T("资源"),_T("增加资源"),_T("请输入资源名称"),_T("增加资源出错"),_T("addworkresource"),_T("resource") },
	{eWorkResource,		 _T("xmDevWorkResource"), 25,_T("资源"),_T("修改资源"),_T("请输入资源名称"),_T("修改资源出错"),_T("updateworkresource"),_T("resource") },
	{eNewWorkXQuery,    _T("xmDevNewXQuery"),25 ,_T("XQuery"),_T("增加XQuery"),_T("请输入XQuery名称"),_T("增加XQuery出错"),_T("addworkxquery"),_T("xquery") },
	{eWorkXQuery,		 _T("xmDevWorkXQuery"), 25,_T("XQuery"),_T("修改XQuery"),_T("请输入XQuery名称"),_T("修改XQuery出错"),_T("updateworkxquery"),_T("xquery") },
	{eNewWorkXAction,    _T("xmDevNewXAction"),25 ,_T("XAction"),_T("增加XAction"),_T("请输入XAction名称"),_T("增加XAction出错"),_T("addworkxaction"),_T("xaction") },
	{eWorkXAction,		 _T("xmDevWorkXAction"), 25,_T("XAction"),_T("修改XAction"),_T("请输入XAction名称"),_T("修改XAction出错"),_T("updateworkxaction"),_T("xaction") },
	{eNewWorkSerialize,    _T("xmDevNewSerialize"),25 ,_T("Serialize"),_T("增加Serialize"),_T("请输入序列化文件名称"),_T("增加序列化文件出错"),_T("addworkserialize"),_T("serialize") },
	{eWorkSerialize,		 _T("xmDevSerialize"), 25,_T("Serialize"),_T("修改Serialize"),_T("请输入序列化文件名称"),_T("修改序列化文件出错"),_T("updateworkserialize"),_T("serialize") },
	{eNewWorkRuler,    _T("xmDevNewRuler"),25 ,_T("Ruler"),_T("增加Ruler"),_T("请输入规则文件名称"),_T("增加规则文件出错"),_T("addworkruler"),_T("xruler") },
	{eWorkRuler,		 _T("xmDevRuler"), 25,_T("Ruler"),_T("修改Ruler"),_T("请输入规则文件名称"),_T("修改规则文件出错"),_T("updateworkruler"),_T("xruler") },
	{eNewWorkXFile,    _T("xmDevNewXFile"),25 ,_T("XFile"),_T("增加XFile"),_T("请输入接口文件名称"),_T("增加接口文件出错"),_T("addworkxfile"),_T("xfile") },
	{eWorkXFile,		 _T("xmDevXFile"), 25,_T("XFile"),_T("修改XFile"),_T("请输入接口文件名称"),_T("修改接口文件出错"),_T("updateworkxfile"),_T("xfile") },
	{eNewWorkXMode,    _T("xmDevNewXMode"),25 ,_T("XMode"),_T("增加XMode"),_T("请输入模式文件名称"),_T("增加模式文件出错"),_T("addworkxmode"),_T("xmode") },
	{eWorkXMode,		 _T("xmDevXMode"), 25,_T("XMode"),_T("修改XMode"),_T("请输入模式文件名称"),_T("修改模式文件出错"),_T("updateworkxmode"),_T("xmode") }
};

enWork GetenWork(LPTSTR pStr)
{
	for(int i=0;i<eWorkCount;i++)
	{
		if(::_tcsicmp(pStr,workMsg[i].pStrCommand)==0)return workMsg[i].eWork;
	}
	return eWorkNULL;
}

CDevWorkDlg::CDevWorkDlg(void)
{
}

CDevWorkDlg::~CDevWorkDlg(void)
{
}

int CDevWorkDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcscmp(pStrID,_T("xmDevCancel"))==0)
	{
		m_dwStatus &= ~WST_RESPONSELOOP;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmDevOk"))==0)
	{

		CDevView::TPara* Para = (CDevView::TPara*)this->m_pParam;
		enWork eWork = GetenWork(Para->pStrID);
		if(eWork==eWorkNULL) return 0;

		xfControl * pControlName = ((xfNodeControl *)pControl->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevName")))->m_pxfControl;
		xfControl * pControlDesc = ((xfNodeControl *)pControl->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevDesc")))->m_pxfControl;
		xfControl * pControlUri  = ((xfNodeControl *)pControl->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevUri")))->m_pxfControl;
		int nLenName = ::GetWindowTextLength(pControlName->m_hWnd);
		int nLenDesc = ::GetWindowTextLength(pControlDesc->m_hWnd);
		int nLenUri  = ::GetWindowTextLength(pControlUri->m_hWnd);
		if(nLenName < 1)
		{
			MessageBox(m_hWnd,workMsg[eWork].pStrInputMsg,workMsg[eWork].pStrItemTitle,MB_OK);
			return 1;
		}
		LPTSTR pStrControlName = new TCHAR[nLenName + 1];
		LPTSTR pStrControlDesc = NULL;
		LPTSTR pStrControlUri = NULL;
		bool bParentNode = false;
		if(nLenDesc > 0)
		{
			pStrControlDesc = new TCHAR[nLenDesc + 1];
			::GetWindowText(pControlDesc->m_hWnd,pStrControlDesc,nLenDesc + 1);
		}
		if(nLenUri > 0)
		{
			pStrControlUri = new TCHAR[nLenUri + 1];
			::GetWindowText(pControlUri->m_hWnd,pStrControlUri,nLenUri + 1);
		}
		::GetWindowText(pControlName->m_hWnd,pStrControlName,nLenName + 1);

		xbXml xmlData;
		xmlData.LoadXml(_T("<item/>"));
		IXMLDOMElement * pDocItem;
		xmlData.m_pXmlDoc->get_documentElement(&pDocItem);
		if(pStrControlName) pDocItem->setAttribute(L"name",(_variant_t)pStrControlName);
		if(pStrControlDesc) pDocItem->setAttribute(L"descibe",(_variant_t)pStrControlDesc);
		if(pStrControlUri)  pDocItem->setAttribute(L"uri",(_variant_t)pStrControlUri);
		pDocItem->Release();
		
		BSTR bstrItem;
		xmlData.m_pXmlDoc->get_xml(&bstrItem);

		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		xfWin * pDevWin = Para->pWin;
		CDevView::XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)pDevWin->m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;

		int	nLevel;
		TV_ITEM	tvItem;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
		if(!hItem )
		{
			delete pStrControlName;
			delete pStrControlDesc;
			MessageBox(m_hWnd,_T("请先选中父项"),workMsg[eWork].pStrItemTitle,MB_OK);
			return 1;
		}
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);

		HTREEITEM hpItem;
		hpItem = hItem;
		IXMLDOMElement * pElement = NULL;
		pElement = (IXMLDOMElement* )(tvItem.lParam);

		IXMLDOMElement * pElementEx;
		LPTSTR pStrGuidItem = NULL;
		if(Para->pElement) //modify
		{
			nLevel = pDevTreeCtrl->GetLevel(hpItem);
			if(eWork==eWorkNode ||eWork==eWorkDoc ||eWork==eWorkEntry ||eWork==eWorkTemplate || eWork==eWorkReport ||eWork==eWorkList ||
				eWork==eWorkXPage || eWork==eWorkCode || eWork==eWorkResource || eWork==eWorkXQuery || eWork==eWorkXAction || eWork==eWorkSerialize || eWork==eWorkRuler ||
				eWork==eWorkXFile || eWork==eWorkXMode)
			{
				pStrGuidItem = xbXml::GetStringAttribe(pElement,_T("guid"));

				BSTR bstrTagName;
findworkitem:	hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
				nLevel --;
				pElementEx = (IXMLDOMElement* )(tvItem.lParam);
				pElementEx->get_tagName(&bstrTagName);
				if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitem;
			}
		}else
		{
			pStrGuidItem = xbXml::GetStringAttribe(pElement,_T("guid"));
			if(eWork == eNewWorkGroup || eWork == eNewWorkItem || eWork==eNewWorkDocEx || eWork==eNewWorkRulerEx || eWork==eNewDocFolder || eWork==eNewBFFolder) 
			{
				nLevel = pDevTreeCtrl->GetLevel(hpItem);
			}else
			{
				BSTR bstrTagName;
				pElement->get_tagName(&bstrTagName);
				if(_tcsicmp(bstrTagName,_T("work"))!=0)
				{
					hpItem = hItem;
startfindworkitem:	hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
					tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
					tvItem.hItem = hpItem;
					TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					pElement = (IXMLDOMElement* )(tvItem.lParam);
					::SysFreeString(bstrTagName);
					pElement->get_tagName(&bstrTagName);
					if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto startfindworkitem;
					bParentNode = false;
				}else
					bParentNode = true;
				::SysFreeString(bstrTagName);
				nLevel = pDevTreeCtrl->GetLevel(hpItem);
			}
		}

		LPTSTR pSrc  = NULL;
		LPTSTR pStrGuid = NULL;
		if(nLevel<=2)
		{
			if(nLevel==1) return 1;
			pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));
		}else
		{
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);
			pStrGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
			HTREEITEM hpItem;
			hpItem = hItem;
			nLevel = pDevTreeCtrl->GetLevel(hpItem);
			for(int i=nLevel;i >2;i--)
			{
				hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
			}
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE|TVIF_TEXT;
			tvItem.hItem = hpItem;
			tvItem.pszText = new TCHAR[255];
			tvItem.cchTextMax = 255;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			delete tvItem.pszText;
			pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));
		}
		TCHAR strCmd[512];
		if(eWork==eWorkNode || eWork==eWorkDoc ||eWork==eWorkEntry || eWork==eWorkTemplate || eWork==eWorkList || eWork==eWorkReport||
			eWork==eWorkXPage || eWork==eWorkCode || eWork==eWorkResource || eWork==eWorkXQuery || eWork==eWorkXAction || eWork==eWorkSerialize || eWork==eWorkRuler ||  eWork==eWorkXFile 
			||  eWork==eWorkXMode)
		{
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer %s %s %s %s %s %s %s"),workMsg[eWork].pStrCmd,
				pSrc,pStrGuid?pStrGuid:_T("*"),pStrControlName,pStrControlDesc?pStrControlDesc:_T("*"),pStrGuidItem,
				 pStrControlUri? pStrControlUri:_T(""));
		}else
		{
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer %s %s %s %s %s %s %s"),workMsg[eWork].pStrCmd,
				pSrc,pStrGuid?pStrGuid:_T("*"),pStrControlName,pStrControlDesc?pStrControlDesc:_T("*"),"*",
				 pStrControlUri? pStrControlUri:_T(""));
		} 
		if(pSrc)delete pSrc;

		LPBYTE pData;
		int nLen;
		int nRet;


		nRet = xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,bstrItem,pData,nLen);
		if(nRet==1)
		{
			if(eWork==eNewWorkGroup || eWork==eNewWorkItem || eWork==eNewWorkNode || eWork==eNewWorkDoc ||eWork==eNewWorkDocEx
				||eWork==eNewWorkEntry || eWork==eNewWorkTemplate || eWork==eNewWorkList || eWork==eNewWorkReport ||
				eWork ==eNewWorkXPage || eWork==eNewWorkCode ||eWork==eNewWorkResource || eWork==eNewWorkXQuery || eWork==eNewWorkXAction || eWork==eNewWorkSerialize
				|| eWork == eNewDocFolder || eWork==eNewWorkRuler
				|| eWork==eNewWorkXFile || eWork==eNewWorkXMode || eWork == eNewBFFolder ||eWork==eNewWorkRulerEx)
			{
				if(bParentNode && !TreeView_GetChild(pDevTreeCtrl->m_hWnd,hItem))
				{
					tvItem.mask = TVIF_CHILDREN|TVIF_HANDLE;
					tvItem.hItem = hItem;
					TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					tvItem.cChildren = true;
					TreeView_SetItem(pDevTreeCtrl->m_hWnd,&tvItem);

					pDevTreeCtrl->ExpandItem(hItem);
				}
				else
				{
					if(bParentNode)
					{
						//get work node item
						hItem = TreeView_GetChild(pDevTreeCtrl->m_hWnd,hItem);
						BSTR bstrTagName;
						while(true)
						{
							tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
							tvItem.hItem = hItem;
							TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
							pElement = (IXMLDOMElement* )(tvItem.lParam);
							pElement->get_tagName(&bstrTagName);
							if(workMsg[eWork].pTagName && _tcsicmp(bstrTagName,workMsg[eWork].pTagName)==0)
							{
								::SysFreeString(bstrTagName);
								break;
							}
							::SysFreeString(bstrTagName);
							if(!TreeView_GetNextSibling(pDevTreeCtrl->m_hWnd,hItem))break;
							hItem = TreeView_GetNextSibling(pDevTreeCtrl->m_hWnd,hItem);
						}
					}
					TVINSERTSTRUCT tvi;
					tvi.hParent = hItem;
					tvi.hInsertAfter = TVI_LAST;
					tvi.itemex.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
					tvi.itemex.iImage = workMsg[eWork].iImage;
					tvi.itemex.iSelectedImage = workMsg[eWork].iImage;
					tvi.itemex.pszText = pStrControlName;
					tvi.itemex.cchTextMax = nLenName + 1;
					if(eWork==eNewWorkGroup || eWork==eNewWorkItem || eWork ==eNewDocFolder || eWork ==eNewBFFolder)
						tvi.itemex.cChildren = true;
					else
						tvi.itemex.cChildren = false;

					xbXml xml;
					xml.LoadXml((LPTSTR)pData);
					IXMLDOMElement * pCurElement;
					xml.m_pXmlDoc->get_documentElement(&pCurElement); 
					tvi.itemex.lParam = (LPARAM)(void *)pCurElement;
					HTREEITEM hItemChild = TreeView_InsertItem(pDevTreeCtrl->m_hWnd,&tvi);
					
					tvItem.mask = TVIF_CHILDREN|TVIF_HANDLE;
					tvItem.hItem = hItem;
					TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					if(!tvItem.cChildren)
					{
						tvItem.cChildren = true;
						TreeView_SetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					}
					TreeView_Expand( pDevTreeCtrl->m_hWnd,hItem,TVE_EXPAND);
					pDevTreeCtrl->ExpandItem(hItemChild);
				}
			}else if(eWork==eWorkGroup || eWork==eWorkItem || eWork==eWorkNode ||eWork==eWorkDoc ||eWork==eWorkEntry || eWork==eWorkTemplate || eWork==eWorkList || eWork==eWorkReport||
				eWork==eWorkXPage || eWork==eWorkCode || eWork==eWorkResource || eWork==eWorkXQuery || eWork==eWorkXAction ||eWork==eWorkRulerEx||eWork==eWorkDocEx || eWork == eDocFolder || eWork==eWorkSerialize
				|| eWork==eWorkRuler || eWork==eWorkXFile || eWork==eWorkXMode
				|| eWork==eWorkRulerEx || eWork==eBFFolder)
			{
				hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
				TV_ITEM tvi;
				tvi.hItem = hItem;
				tvi.mask = TVIF_HANDLE|TVIF_TEXT;
				tvi.pszText = pStrControlName;
				tvi.cchTextMax = nLenName + 1;
				TreeView_SetItem(pDevTreeCtrl->m_hWnd,&tvi);
				Para->pElement->setAttribute(L"caption",(_variant_t)pStrControlName);
				Para->pElement->setAttribute(L"desc",(_variant_t)pStrControlDesc);
				Para->pElement->setAttribute(L"uri",(_variant_t)pStrControlUri);

			}
			m_dwStatus &= ~WST_RESPONSELOOP;
			if(pData)delete pData;
		}
		else
			MessageBox(m_hWnd,workMsg[eWork].pStrErrorMsg,workMsg[eWork].pStrItemTitle,MB_OK);
		delete pStrControlName;
		delete pStrControlDesc;
		delete pStrControlUri;
		::SysFreeString(bstrItem);
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
	}
	return 0;
}
int CDevWorkDlg::OnOpen()
{
	CDevView::TPara* Para = (CDevView::TPara*)this->m_pParam;
	enWork eWork = GetenWork(Para->pStrID);
	if(eWork==eWorkNULL)return 0;
	::SetWindowText(m_hWnd,workMsg[eWork].pStrItemTitle);
	if(Para->pElement)//modify
	{
		xfControl * pControlName = ((xfNodeControl *)m_pXPage->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevName")))->m_pxfControl;
		xfControl * pControlDesc = ((xfNodeControl *)m_pXPage->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevDesc")))->m_pxfControl;
		xfControl * pControlPath = ((xfNodeControl *)m_pXPage->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevPath")))->m_pxfControl;
		xfControl * pControlUri = ((xfNodeControl *)m_pXPage->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevUri")))->m_pxfControl;
		xfControl * pControlGuid = ((xfNodeControl *)m_pXPage->m_pxfNode->GetRootNode()->GetNodeByName(_T("xmDevGuid")))->m_pxfControl;
		
		LPTSTR pStrName = xbXml::GetStringAttribe(Para->pElement,_T("caption"));
		LPTSTR pStrDesc = xbXml::GetStringAttribe(Para->pElement,_T("desc"));
		LPTSTR pStrPath = xbXml::GetStringAttribe(Para->pElement,_T("src"));
		LPTSTR pStrUri = xbXml::GetStringAttribe(Para->pElement,_T("uri"));
		LPTSTR pStrGuid = xbXml::GetStringAttribe(Para->pElement,_T("guid"));
		if(pStrName)
		{
			::SetWindowText(pControlName->m_hWnd,pStrName);
			delete pStrName;
		}
		if(pStrDesc)
		{
			::SetWindowText(pControlDesc->m_hWnd,pStrDesc);
			delete pStrDesc;
		}
		if(pStrPath)
		{
			::SetWindowText( pControlPath->m_hWnd,pStrPath);
			delete pStrPath;
		}
		if(pControlGuid && pStrGuid)
		{
			::SetWindowText( pControlGuid->m_hWnd,pStrGuid);
			delete pStrGuid;
		}
		if(pControlUri && pStrUri)
		{
			::SetWindowText( pControlUri->m_hWnd,pStrUri);
			delete pStrUri;
		}
	}
	return 1;
}



