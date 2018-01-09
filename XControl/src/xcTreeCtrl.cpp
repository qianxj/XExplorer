#include "stdafx.h"
#include "xcTreectrl.hpp"
#include "xcTreexmldatasource.hpp"
namespace Hxsoft
{
	namespace XFrame
	{
		WNDPROC OldxTreeProc;
		bool m_Draging;
		int m_TimerID;
		HTREEITEM m_DragItemS;
		HTREEITEM m_DragItemD;

		struct 	TVNNMHDR : public NMHDR
		{
			HTREEITEM FromItem;
			HTREEITEM ToItem;
			POINT	pt;
			LPTSTR PopupMenu;
		};

		xcTreeCtrl::xcTreeCtrl():m_pDatas(new vector<LPTSTR>),m_pElements(new vector<IXMLDOMElement * >),
			m_pTreeSource( new vector<xcTreeXmlDataSource *>())
		{
			m_Draging = false;
			m_bTransparent = true;
			this->AttachCtrlListon(TVN_ITEMEXPANDING ,(FNEvent)&xcTreeCtrl::EvtItemExpanding);
		}

		xcTreeCtrl::~xcTreeCtrl()
		{
			for(int i=0;i<(int)m_pDatas->size();i++)
			{
				delete (*m_pDatas)[i];
			}
			if(m_pDatas) delete m_pDatas;

			for(int i=0;i<(int)m_pElements->size();i++)
			{
				(*m_pElements)[i]->Release();
			}
			if(m_pElements) delete m_pElements;
			for(int i=0;i<(int)m_pTreeSource->size();i++)
			{
				if((*m_pTreeSource)[i]) delete (*m_pTreeSource)[i];
			}
			if(m_pTreeSource)delete m_pTreeSource;
		}

		void OnTimer(HWND hWnd, UINT nIDEvent) 
		{
			if(m_DragItemD)
				TreeView_Expand(hWnd, m_DragItemD, TVE_EXPAND);
		}

		HTREEITEM OnCopyNode(HWND hWnd, HTREEITEM item1, HTREEITEM item2)
		{
			//源节点信息
			TVITEM tvi;
			tvi.hItem = item1;
			tvi.mask = TVIF_TEXT |TVIF_PARAM|TVIF_HANDLE|TVIF_IMAGE|TVIF_STATE|TVIF_SELECTEDIMAGE|TVIF_CHILDREN;
			char str[255] = "";
			strnset(str,0,255);
			tvi.pszText =(LPWSTR)str;
			tvi.cchTextMax = 255;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			TreeView_GetItem(hWnd, &tvi);
			tvi.cchTextMax = sizeof(tvi.pszText);
			tvi.hItem = NULL;
			//目标节点设置cChildren = 1
			TVITEM tvi2;
			tvi2.hItem = item2;
			tvi2.cChildren = 1;
			tvi2.mask = TVIF_HANDLE|TVIF_CHILDREN;
			TreeView_SetItem(hWnd, &tvi2);
			//复制节点
			TVINSERTSTRUCT tvins; 
			tvins.item = tvi;
			tvins.hParent = item2;
			tvins.hInsertAfter = TVI_LAST;
			return TreeView_InsertItem(hWnd, &tvins);
		}
	
		void OnCopy(HWND hWnd, HTREEITEM item1, HTREEITEM item2)
		{
			HTREEITEM iNext = item1;
			HTREEITEM NewItem = item2;
			while (iNext)
			{
				NewItem = OnCopyNode(hWnd, iNext, item2);
				HTREEITEM iChild = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CHILD,(LPARAM)iNext);
				if(iChild)
					OnCopy(hWnd, iChild, NewItem);
				iNext = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_NEXT,(LPARAM)iNext);
			}
		}

		LRESULT CALLBACK xTreeProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam)
		{
			POINT n_pt;
			TVNNMHDR hdr;
			memset(&hdr,0,sizeof(TVNNMHDR));
			TVHITTESTINFO tvinfo;
			switch(message)
			{
				case WM_MOUSEMOVE:
					if (m_Draging)
					{
						//POINT n_pt;
						n_pt.y = (int)HIWORD(lParam);
						n_pt.x = (int)LOWORD(lParam);
						if(m_TimerID)
							::KillTimer(hWnd, m_TimerID);
						ImageList_DragMove(n_pt.x, n_pt.y);	
						ImageList_DragShowNolock(false);
						TVHITTESTINFO n_tvitem;
						n_tvitem.pt = n_pt;
						m_DragItemD = TreeView_HitTest(hWnd, &n_tvitem);
						if(m_DragItemD)
						{
							m_TimerID = SetTimer(hWnd,1,800,(TIMERPROC)OnTimer); 
							TreeView_SelectDropTarget(hWnd, m_DragItemD);
						}
						else
							TreeView_SelectDropTarget(hWnd, 0);
						ImageList_DragShowNolock(true);
					}
					break;
				case WM_LBUTTONUP:
					if (m_Draging)
					{
						if(m_TimerID)
							::KillTimer(hWnd, m_TimerID);
						ImageList_EndDrag(); 
						TreeView_SelectDropTarget(hWnd, 0);
						::ReleaseCapture();
						//ShowCursor(TRUE);
						m_Draging = false;
						HTREEITEM n_NewItem;
						if(m_DragItemD && m_DragItemS)
						{
							//不能移动到自身
							if(m_DragItemD == m_DragItemS)
								break;
							//不能移动到子节点
							HTREEITEM n_Root = TreeView_GetParent(hWnd, m_DragItemD);
							while (n_Root)
							{
								if (n_Root == m_DragItemS) break;
								n_Root = TreeView_GetParent(hWnd, n_Root);
							}
							if(n_Root)
								break;
							//不能移动到同一个父节点
							HTREEITEM n_Root1 = TreeView_GetParent(hWnd, m_DragItemS);
							if (n_Root1 == m_DragItemD)
								break;
							//复制节点
							n_NewItem = OnCopyNode(hWnd, m_DragItemS, m_DragItemD);
							//复制子节点
							HTREEITEM n_ChildItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CHILD,(LPARAM)m_DragItemS);
							OnCopy(hWnd, n_ChildItem, n_NewItem);
							//删除原节点
							//TreeView_DeleteItem(hWnd, m_DragItemS);
						}
						//通知
						//TVNNMHDR hdr;
						//memset(&hdr,0,sizeof(TVNNMHDR));
						hdr.code = TVN_ENDDRAG;
						hdr.idFrom = 0;
						hdr.hwndFrom = hWnd;
						hdr.FromItem = m_DragItemS;
						hdr.ToItem = n_NewItem;
						::SendMessage(::GetParent(hWnd),WM_NOTIFY,0,(LPARAM)&hdr);
						//PostMessage(GetParent(hWnd),WM_XCOMMAND,(WPARAM)_T("_xTreeItemDrag"),(LPARAM)this);
					}
					break;
				case WM_RBUTTONDOWN:
					n_pt.y = (int)HIWORD(lParam);
					n_pt.x = (int)LOWORD(lParam);
					//TVHITTESTINFO tvinfo;
					tvinfo.pt = n_pt;
					//tvinfo.flags = TVHT_ONITEMLABEL;
					TreeView_HitTest(hWnd, &tvinfo);
					if(TVHT_ONITEMLABEL & tvinfo.flags)
						hdr.FromItem = tvinfo.hItem;
					else
						hdr.FromItem = 0;
					hdr.code = TVN_RCLICK;
					hdr.idFrom = 0;
					hdr.hwndFrom = hWnd;
					hdr.pt = n_pt;
					::SendMessage(::GetParent(hWnd),WM_NOTIFY,0,(LPARAM)&hdr);
					break;
				case WM_LBUTTONDOWN:
					n_pt.y = (int)HIWORD(lParam);
					n_pt.x = (int)LOWORD(lParam);
					tvinfo.pt = n_pt;
					//tvinfo.flags = TVHT_ONITEM;
					TreeView_HitTest(hWnd, &tvinfo);

					hdr.idFrom = 0;
					hdr.hwndFrom = hWnd;
					hdr.FromItem = tvinfo.hItem;
					hdr.pt = n_pt;
					hdr.code = TVN_LCLICK;
					::SendMessage(::GetParent(hWnd),WM_NOTIFY,0,(LPARAM)&hdr);

					if(TVHT_ONITEMSTATEICON & tvinfo.flags)
					{
						hdr.code = TVN_CHECKBOXSEL;
						::SendMessage(::GetParent(hWnd),WM_NOTIFY,0,(LPARAM)&hdr);
					}

					break;
				case 0x402:
					::GetCursorPos(&n_pt);
					//n_pt.y = (int)HIWORD(lParam);
					//n_pt.x = (int)LOWORD(lParam);
					//::ClientToScreen(hWnd,&n_pt);
					HMENU hMenuPopup = ::CreatePopupMenu();
					//::AppendMenu(hMenuPopup,MF_STRING, 1, _T("新增"));
					//::AppendMenu(hMenuPopup,MF_STRING, 2, _T("删除"));
					//::AppendMenu(hMenuPopup,MF_STRING, 3, LPTSTR(lParam));

					LPTSTR str = LPTSTR(wParam);
					xbXml xml;
					xml.LoadXml(str);
					IXMLDOMNodeList *pList;
					IXMLDOMNode *pNodeEx;
					xml.m_pXmlDoc->selectNodes(L"//*",&pList);
					long nLen;
					LPTSTR n_str;
					pList->get_length(&nLen);
					if(nLen>0)
					{
						for(int i=0;i<nLen;i++)
						{
							pList->get_item(i,&pNodeEx);
							BSTR bstr;
							pNodeEx->get_text(&bstr);
							if(bstr)
							{
								if(::_tcsicmp(bstr,_T("-"))==0 || ::_tcsicmp(bstr,_T(""))==0)
								{
									::AppendMenu(hMenuPopup,MF_SEPARATOR, i + 1, NULL);
								}
								else
								{
									::AppendMenu(hMenuPopup,MF_STRING, i + 1, bstr);
									::SysFreeString(bstr);
								}
							}
							pNodeEx->Release();
						}
						int n_ret = ::TrackPopupMenu (hMenuPopup, TPM_RIGHTBUTTON | TPM_RETURNCMD, n_pt.x, n_pt.y, 0, hWnd, NULL);
						if(n_ret>0)
						{
							IXMLDOMNode *pNode;
							pList->get_item(n_ret - 1,&pNode);
							pNode->get_text(&n_str);
							pNode->Release();
						}
						else
							n_str=NULL;
					}
					else
						n_str=NULL;
					pList->Release();
					::DestroyMenu(hMenuPopup);					
					return (LRESULT)n_str;
					break;
				//default:					
			}
			return OldxTreeProc(hWnd,message,wParam,lParam);
		}

		bool xcTreeCtrl::EvtBeginDrag(TEvent* pEvent,LPARAM lParam)
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pEvent->lParam;

			m_DragItemS = pnmtv->itemNew.hItem;
			m_DragItemD = NULL;
			if (m_DragImage) m_DragImage = NULL;
			m_DragImage = TreeView_CreateDragImage(this->m_hWnd, m_DragItemS);
			if(!m_DragImage)
				return false;
			m_Draging = true;

			ImageList_BeginDrag(m_DragImage, 0, 0, 0);
			POINT pt = pnmtv->ptDrag;
			ImageList_DragEnter(this->m_hWnd, pt.x, pt.y);
			::SetCapture(this->m_hWnd);
			//ShowCursor(FALSE); 
			return true;
		}

		bool xcTreeCtrl::EvtItemExpanding(TEvent* pEvent,LPARAM lParam)
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pEvent->lParam;

			if(!TreeView_GetChild(m_hWnd,pnmtv->itemNew.hItem))
				this->ExpandItemEx(pnmtv->itemNew.hItem);
			return true;
		}

		int xcTreeCtrl::MakeTree()
		{
			(*m_pTreeSource).clear();
			ExpandTree(TVI_ROOT,m_pxfNode->m_pElement,0);
			ExpandRoot(false,2);
			return 1;
		}

		bool xcTreeCtrl::isParentItem(HTREEITEM hpItem, HTREEITEM hItem,int &rLevel)
		{
			rLevel = 0;
			HTREEITEM pParentItem;
			pParentItem = hItem;
			if(pParentItem==NULL)return hpItem == TVI_ROOT?true:false;
			while(pParentItem && pParentItem != hpItem )
			{
				rLevel++;
				pParentItem = TreeView_GetParent(this->m_hWnd,pParentItem);
				if(pParentItem==NULL && hpItem == TVI_ROOT) return true;
			}
			if(pParentItem) return true;
			return false;
		}

		int xcTreeCtrl::ExpandTree(HTREEITEM hpItem, IXMLDOMElementPtr pElement,int level)
		{
			IXMLDOMNodeListPtr	pList;
			IXMLDOMNodePtr		pNode;
			TVITEM tvi; 
			TVINSERTSTRUCT tvins; 
			HTREEITEM hItem;

			long	ll_len;
			pElement->selectNodes(L"treeitem",&pList);
			pList->get_length(&ll_len);
			for(int i=0;i < ll_len;i++)
			{
				VARIANT	sval;
				pList->get_item(i,&pNode);

				((IXMLDOMElementPtr)pNode)->getAttribute(L"src",&sval);
				if(sval.vt==VT_BSTR && sval.bstrVal)
				{
					xcTreeXmlDataSource * pXmlSource = new xcTreeXmlDataSource();
					pXmlSource->m_hItem = hpItem;
					pXmlSource->m_strXSrc = XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);

					((IXMLDOMElementPtr)pNode)->getAttribute(L"srcex",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXSrcEx =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);

					((IXMLDOMElementPtr)pNode)->getAttribute(L"xroot",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXRootPath =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);


					((IXMLDOMElementPtr)pNode)->getAttribute(L"xpath",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXPath =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xcaption",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXText =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xcaptionex",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXTextEx =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xdata",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXData =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xdataex",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXDataEx =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"ximage",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXImage =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"ximageex",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXImageEx =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xselectedimage",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXSelectedImage =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"xselectedimageex",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strXSelectedImageEx =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"data",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strData =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"caption",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strText =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"param",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
						pXmlSource->m_strParam =  XUtil::tcsdupFromBstr(sval.bstrVal);
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"image",&sval);  
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						WCHAR * pNextStr;
						pXmlSource->m_iImage = ::wcstol(sval.bstrVal,&pNextStr,10);
					}
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"selectimage",&sval);  
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						WCHAR * pNextStr;
						pXmlSource->m_iSelectedImage = ::wcstol(sval.bstrVal,&pNextStr,10);
					}
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"loop",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						if(::wcscmp(sval.bstrVal,L"yes")==0)
							pXmlSource->m_bLoop = true;
						else
							pXmlSource->m_bLoop = false;
					}
					::VariantClear(&sval);
					((IXMLDOMElementPtr)pNode)->getAttribute(L"includesrc",&sval);  
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						pXmlSource->m_strExpSrc = XUtil::tcsdupFromBstr(sval.bstrVal);
					}
					::VariantClear(&sval);

					((IXMLDOMElementPtr)pNode)->getAttribute(L"xchildpath",&sval);  
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						pXmlSource->m_ChildPath = XUtil::tcsdupFromBstr(sval.bstrVal);
					}
					::VariantClear(&sval);

					pXmlSource->m_RLevel = level + 1; 
					pXmlSource->m_hItem = hpItem;
					if(_tcscmp(pXmlSource->m_strXSrc,_T("."))==0 && pXmlSource->m_RLevel>0)
					{
						//use parent xmldocument
						for(int k=(int)(*m_pTreeSource).size() - 1;k>=0; k--)
						{
							if((*m_pTreeSource)[k]->m_hItem == hpItem && (*m_pTreeSource)[k]->m_RLevel < pXmlSource->m_RLevel)
							{
								pXmlSource->m_pXml->m_pXmlDoc = (*m_pTreeSource)[k]->m_pXml->m_pXmlDoc;
								pXmlSource->m_pXml->m_pXmlDoc->AddRef();
								pXmlSource->m_bXmlLoaded = true;
							}
						}
					}
					(*m_pTreeSource).push_back(pXmlSource);
					ExpandTree(hpItem,(IXMLDOMElementPtr)pNode,level + 1);
				}else
				{
					::VariantClear(&sval);

					((IXMLDOMElementPtr)pNode)->getAttribute(L"caption",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						// Set the text of the item. 
						tvi.pszText = XUtil::tcsdupFromBstr(sval.bstrVal);
						tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
					}else
					{
						tvi.pszText = NULL;
						tvi.cchTextMax = 0;
					}
					::VariantClear(&sval);


					tvi.mask = TVIF_TEXT | TVIF_IMAGE 
						| TVIF_SELECTEDIMAGE | TVIF_PARAM; 


					// Assume the item is not a parent item, so give it a 
					// document image. 
					long	image = 0;
					((IXMLDOMElementPtr)pNode)->getAttribute(L"image",&sval);  
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						WCHAR * pNextStr;
						image = ::wcstol(sval.bstrVal,&pNextStr,10);
					}else
					{
						::VariantClear(&sval);
					}

					//image =1 ;

					tvi.iImage = image;
					tvi.iSelectedImage = image; 

					// Save the heading level in the item's application-defined 
					// data area. 
					((IXMLDOMElementPtr)pNode)->getAttribute(L"data",&sval);
					if(sval.vt==VT_BSTR && sval.bstrVal)
					{
						if(sval.bstrVal[0]=='.' && sval.bstrVal[1]=='\0')
						{
							IXMLDOMElement * pElement;
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
							tvi.lParam = (LPARAM)pElement;
							(*m_pElements).push_back(pElement);
						}
						else
						{
							// Set the text of the item. 
							tvi.lParam = (LPARAM)XUtil::tcsdupFromBstr(sval.bstrVal);
							this->m_pDatas->push_back((LPTSTR)tvi.lParam);
						}
							
					}else
					{
						tvi.lParam = NULL; 
					}
					::VariantClear(&sval);


					tvins.item = tvi; 
					tvins.hInsertAfter = NULL; 

					// Set the parent item based on the specified level. 
					tvins.hParent = hpItem; 

					// Add the item to the tree-view control. 
					hItem = (HTREEITEM)SendMessage(this->m_hWnd, 
						TVM_INSERTITEM, 
						0,
						(LPARAM)(LPTVINSERTSTRUCT)&tvins); 

					ExpandTree(hItem,(IXMLDOMElementPtr)pNode,0);
				}
			}
			return 1;
		}
		int xcTreeCtrl::ExpandItemEx(HTREEITEM hItem)
		{
			for(int i=0;i<(int)(*m_pTreeSource).size();i++)
			{
				int level = 0;
				if(isParentItem((*m_pTreeSource)[i]->m_hItem,hItem,level))
				{
					if(level == (*m_pTreeSource)[i]->m_RLevel -1)
						(*m_pTreeSource)[i]->ExpandItem(this,hItem);
					else
					{
						if((*m_pTreeSource)[i]->m_bLoop &&  level > (*m_pTreeSource)[i]->m_RLevel -1)
							(*m_pTreeSource)[i]->ExpandItem(this,hItem);
					}
				}
			}
			return 1;
		}

		int xcTreeCtrl::ExpandRoot(bool expandChild,int nflowLevel )
		{
			HTREEITEM	hItem;
			hItem = TreeView_GetRoot(this->m_hWnd);
			if(!hItem)
			{
				hItem = TVI_ROOT;
				ExpandItem(hItem,expandChild,nflowLevel - 1);
			}else
			{
				while(hItem)
				{
					ExpandItem(hItem,expandChild,nflowLevel - 1);
					hItem = TreeView_GetNextSibling(this->m_hWnd,hItem);
				}
			}
			return 1;
		}

		int xcTreeCtrl::ExpandAll()
		{
			return ExpandRoot(true);
		}

		int xcTreeCtrl::ExpandAll(HTREEITEM hItem)
		{
			return ExpandItem(hItem,true);
		}

		int xcTreeCtrl::ExpandItem(HTREEITEM	hItem,bool expandChild,int nflowLevel)
		{
			ExpandItemEx(hItem);

			if(expandChild || nflowLevel > 0)
			{
				HTREEITEM hcItem;
				hcItem = TreeView_GetChild(this->m_hWnd, hItem);
				while(hcItem)
				{
					ExpandItem(hcItem,expandChild,nflowLevel - 1);
					hcItem = TreeView_GetNextSibling(this->m_hWnd,hcItem);
				}
			}

			return 1;
		}

		void xcTreeCtrl::Initial()
		{
			VARIANT	svalex;
			m_pxfNode->m_pElement->getAttribute(L"imagelist",&svalex);
			if(svalex.vt==VT_BSTR && svalex.bstrVal)
			{
				WCHAR* pNextStr;
				int index = ::wcstol(svalex.bstrVal,&pNextStr,10);
				if(index >=0 && m_pxfNode->GetXPage()->m_pResourceMgr && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
				{
					::SendMessage(m_hWnd, TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
				}else if(index >=0 && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
				{
					::SendMessage(m_hWnd, TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
				}
			}
			::VariantClear(&svalex);
			MakeTree();

			m_pxfNode->m_pElement->getAttribute(L"drag",&svalex);
			if(svalex.bstrVal)
			{
				if(::_tcsicmp(svalex.bstrVal,_T("yes"))==0 || ::_tcsicmp(svalex.bstrVal,_T("true"))==0 || ::_tcsicmp(svalex.bstrVal,_T("1"))==0)
				{
					this->AttachCtrlListon(TVN_BEGINDRAG, (FNEvent)&xcTreeCtrl::EvtBeginDrag);
				}
			}
			::VariantClear(&svalex);

			m_pxfNode->m_pElement->getAttribute(L"checkbox",&svalex);
			if(svalex.vt==VT_BSTR && svalex.bstrVal)
			{
				if(::_tcsicmp(svalex.bstrVal,_T("yes"))==0 || ::_tcsicmp(svalex.bstrVal,_T("true"))==0 || ::_tcsicmp(svalex.bstrVal,_T("1"))==0)
				{
					long s = ::GetWindowLong(m_hWnd, GWL_STYLE);
					::SetWindowLong(m_hWnd, GWL_STYLE, s | TVS_CHECKBOXES );
				}
			}
			::VariantClear(&svalex);

			m_pxfNode->m_pElement->getAttribute(L"editlabel",&svalex);
			if(svalex.vt==VT_BSTR && svalex.bstrVal)
			{
				if(::_tcsicmp(svalex.bstrVal,_T("yes"))==0 || ::_tcsicmp(svalex.bstrVal,_T("true"))==0 || ::_tcsicmp(svalex.bstrVal,_T("1"))==0)
				{
					long s1 = ::GetWindowLong(m_hWnd, GWL_STYLE);
					::SetWindowLong(m_hWnd, GWL_STYLE, s1 | TVS_EDITLABELS );
				}
			}
			::VariantClear(&svalex);

			//OldxTreeProc1 = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)&xcTreeCtrl::xTreeProc1);
			OldxTreeProc = (WNDPROC)::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)xTreeProc);
		}
	}
}	
