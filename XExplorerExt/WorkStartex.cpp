#include "StdAfx.h"
#include "WorkStartex.h"
#include "xreport.h"
#include "xflowchart.h"
#include "cdosys.h"

#import "c:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "EndOfFile")

int  CWorkStartEx::MakeData(XOffice::XXmlContentSvr * pContentSvr )
{
	IXMLDOMElementPtr pSchemaElement;
	vector<XOffice::XXmlContentSvr::TTreeItem>* pTreeData = NULL;
	pContentSvr->GetTreeData(pTreeData);
	if(!pTreeData) pTreeData = new vector<XOffice::XXmlContentSvr::TTreeItem>;
	XOffice::XXmlContentSvr::TTreeItem item;
	for(int i=0;i<pContentSvr->GetRowCount();i++)
	{
		IXMLDOMElement * pElement;
		_variant_t val;
		pElement = pContentSvr->GetRowElement(i + 1);

		IXMLDOMNode * pItem;
		pElement->selectSingleNode(L"task",&pItem);
		if(pItem)
		{
			item.children = true;
			item.expanded = false;
			item.nImage = 22;
			pItem->Release();
		}else
		{
			item.children =  false;
			item.expanded = false;
			item.nImage = 28;
		}
		item.nContentCol = 1;
		item.nContentRow = i + 1;
		item.nImageList = 0;
		item.nLevel = 1;
		if(i>=0)
		{
			IXMLDOMNode * pNode;
			IXMLDOMNode * pParentNode;
			pElement->get_parentNode(&pParentNode);
			while(pParentNode)
			{
				item.nLevel++;
				pNode = pParentNode;
				pNode->get_parentNode(&pParentNode);
				pNode->Release();
			}
			item.nLevel--;
		}
		item.nSelectedImage = -1;
		pTreeData->push_back(item);
		pElement->Release();
	}
	pContentSvr->SetTreeData(pTreeData);
	return 1;
}


class IReportEventImpl : public XOffice::XReport::IReportEvent
{
public:
	CWorkStartEx * m_pWorkStart;
	int OnRowFoucsChanged(int nNewRow,int nOldRow)
	{
		XOffice::XReport::XReportSheet* pSheet = (XOffice::XReport::XReportSheet*)m_pWorkStart->GetControl(_T("tasklist"));
		XOffice::XReport::XReportSheetSvr* pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		int nRow = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetContentRow(nNewRow);
		IXMLDOMElement * pElement = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetRowElement(nRow);

		if(!pElement) return 1;

		_variant_t var;
		pElement->getAttribute(L"workfolwguid",&var);
		if(var.bstrVal)
		{
			variant_t worknodeguid;
			pElement->getAttribute(L"worknodeguid",&worknodeguid);
								
			TCHAR strCmd[512];
			if(worknodeguid.bstrVal)
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow:%s/getchart %s"),var.bstrVal,worknodeguid.bstrVal);
			else
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow:%s/getchart"),var.bstrVal);

			xbXml xml;
			if(xfUriLoader::FetchXml(m_pWorkStart->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;

			//IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pDataElement;

			xml.m_pXmlDoc->selectSingleNode(L"/sheet",&pNode);
			if(!pNode)
				xml.m_pXmlDoc->selectSingleNode(L"/xdoc/sheet[1]",&pNode);
			if(!pNode) return true;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pDataElement);
			pNode->Release();
			if(!pDataElement) return true;

			//get flowchart
			XOffice::XFlowChart::XFlowSheet* pSheet =  (XOffice::XFlowChart::XFlowSheet*)m_pWorkStart->GetControl(_T("flowchartctrl"));
			XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = new XOffice::XFlowChart::XFlowChartDataSvr();
			XOffice::XFlowChart::XFlowChartDataSvr * pOldDataSvr;
			pSheet->m_pSheetSvr->m_pSerializeSvr->LoadSheetData(NULL,pDataElement,pDataSvr);

			pOldDataSvr = (XOffice::XFlowChart::XFlowChartDataSvr *)pSheet->m_pSheetSvr->m_pDataSvr;
			((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->Clear();
			pSheet->m_pSheetSvr->m_pDataSvr = pDataSvr;
			if(pOldDataSvr)delete pOldDataSvr;

			::InvalidateRect(pSheet->m_hWnd,NULL,false);

			pDataElement->Release();

			//get plan and process
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow:%s/getplan"),var.bstrVal);
			if(xfUriLoader::FetchXml(m_pWorkStart->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;

			BSTR bstr;
			xml.m_pXmlDoc->get_xml(&bstr);
			if(bstr)
			{
				XOffice::XReport::XReportSheet* pPlanSheet = (XOffice::XReport::XReportSheet*)m_pWorkStart->GetControl(_T("taskplan"));
				XOffice::XReport::XReportSheetSvr* pPlanSheetSvr = (XOffice::XReport::XReportSheetSvr*)pPlanSheet->m_pSheetSvr;
				pPlanSheetSvr->GetReportDataSvr()->m_pContentSvr->LoadXml(bstr);
				::SysFreeString(bstr);
				::InvalidateRect(pPlanSheet->m_hWnd,NULL,false);
			}
		}

		return 1;
	}
	
	IXMLDOMElement * GetFlowWorkSheet(LPTSTR pWorkFlowGuid)
	{
		IXMLDOMElement * pWkSheet;
		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow:%s/getchart"),pWorkFlowGuid);

		xbXml xml;
		if(xfUriLoader::FetchXml(NULL,strCmd,NULL,xml)!=1) return NULL;

		//IXMLDOMElement * pElement;
		IXMLDOMNode * pNode;

		xml.m_pXmlDoc->selectSingleNode(L"/sheet",&pNode);
		if(!pNode)
			xml.m_pXmlDoc->selectSingleNode(L"/xdoc/sheet[1]",&pNode);
		if(!pNode) return NULL;
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pWkSheet);
		pNode->Release();
		return pWkSheet;	
	}

	LPTSTR GetFlowNodeDevLink(IXMLDOMElement * pWkSheet,LPTSTR pWorkNodeGuid)
	{
		TCHAR  buf[255];
		LPTSTR pDevLink = NULL;
		IXMLDOMNode* pNode;
		if(!pWkSheet) return NULL;
		_stprintf_s(buf,255,L"//shape[@guid='%s']/data/devlink",pWorkNodeGuid);
		pWkSheet->selectSingleNode(buf,&pNode);
		if(pNode)
		{
			IXMLDOMElement* pElement;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			pDevLink  = xbXml::GetStringAttribe(pElement,_T("url"));
			pElement->Release();
		}
		return pDevLink ;
	}

	int OnDoubleClicked(int nRow)
	{
		if(nRow >0 )
		{
			XOffice::XReport::XReportSheet* pSheet = (XOffice::XReport::XReportSheet*)m_pWorkStart->GetControl(_T("tasklist"));
			XOffice::XReport::XReportSheetSvr* pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

			int Row = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetContentRow(nRow);
			IXMLDOMElement * pElement = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetRowElement(Row);

			if(!pElement) return 1;

			_variant_t var;
			pElement->getAttribute(L"workfolwguid",&var);
			
			_variant_t vartype;
			pElement->getAttribute(L"type",&vartype);
			
			int nType = 1;

			if(var.bstrVal)
			{
				IXMLDOMElement* pWkSheet;
				pWkSheet = GetFlowWorkSheet(var.bstrVal);
				LPTSTR pServer = m_pWorkStart->m_pXPage->m_pStrUrl;
				if(vartype.bstrVal && _tcsicmp(vartype.bstrVal ,L"workflow")==0)
				{
					IXMLDOMNodeList* pList;
					IXMLDOMNode * pNode;
					IXMLDOMElement * pDataElement;


					 bool bShowTask = true;
					 if(m_pWorkStart->GetControl(_T("openshowtask")))
					 {
						 if(BST_CHECKED == SendMessage(m_pWorkStart->GetControl(_T("openshowtask"))->GetHWND(),BM_GETCHECK,0,0))bShowTask = false;
					 }

					xbXml xml;
					int nRet = xfUriLoader::FetchXml(pServer,L"dev:xpage{7C07E0CA-5EDB-4FFB-ABEB-FC10A9E316BB}",NULL,xml);
					if(nRet==1)
					{
						IXMLDOMElement * pElementEx;
						IXMLDOMElement * pTaskElement;
						IXMLDOMElement * pLayerElement;
						IXMLDOMElement * pItemElement;
						IXMLDOMElement * pLayersElement;
						xml.m_pXmlDoc->get_documentElement((IXMLDOMElement **)&pElementEx);
						if(pElementEx)
						{
							pElementEx->selectSingleNode(L"//layers",&pNode);
							if(pNode)
							{
								pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pDataElement);
								pNode->Release();

								pDataElement->selectSingleNode(L"layer[1]",&pNode);
								if(pNode)
								{
									pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pLayerElement);
									pNode->Release();
									pLayerElement->setAttribute(L"workflow",var);
									pLayerElement->Release();
								}


								long len;
								pElement->selectNodes(L"task",&pList);
								pList->get_length(&len);


								for(int i=0;i<len;i++)
								{
									pList->get_item(i,&pNode);
									pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pTaskElement);
									pNode->Release();

									variant_t caption;
									variant_t worknodeguid;
									pTaskElement->getAttribute(L"subject",&caption);
									pTaskElement->getAttribute(L"worknodeguid",&worknodeguid);
									pTaskElement->Release();

									
									if(bShowTask)
									{
										xml.m_pXmlDoc->createElement(L"layer",&pLayerElement);
										pLayerElement->setAttribute(L"caption",caption);

										pLayerElement->setAttribute(L"workflow",var);
										pLayerElement->setAttribute(L"worknode",worknodeguid);

										xml.m_pXmlDoc->createElement(L"layers",&pLayersElement);
										pLayersElement->setAttribute(L"tabsize",(_variant_t)L"20");
										pLayersElement->setAttribute(L"tabplace",(_variant_t)L"bottom");
										pLayersElement->setAttribute(L"tabstyle",(_variant_t)L"tdsShape");

										pLayerElement->appendChild(pLayersElement,NULL);
										pDataElement->appendChild(pLayerElement,NULL);
										pLayerElement->Release();

										//add task page
										xml.m_pXmlDoc->createElement(L"layer",&pLayerElement);
										pLayersElement->appendChild(pLayerElement,NULL);
										pLayerElement->setAttribute(L"caption",(_variant_t)L"任务描述");


										xml.m_pXmlDoc->createElement(L"xpage",&pItemElement);
										pItemElement->setAttribute(L"src",(_variant_t)L"uriserver/uri.hxsoft.com/xaserver/dev:xpage{8C643DF8-D2EF-4D9B-AED7-057C95AF4405}");
										pLayerElement->appendChild(pItemElement,NULL);
										pLayerElement->Release();
										
										//add business page
										if(worknodeguid.bstrVal)
										{
											LPTSTR DevLink = GetFlowNodeDevLink(pWkSheet,worknodeguid.bstrVal);
											if(DevLink)
											{
												xml.m_pXmlDoc->createElement(L"layer",&pLayerElement);
												pLayerElement->setAttribute(L"caption",caption);
												pLayersElement->appendChild(pLayerElement,NULL);


												xml.m_pXmlDoc->createElement(L"xframe",&pItemElement);
												pItemElement->setAttribute(L"src",(_variant_t)DevLink);
												pItemElement->setAttribute(L"visible",(_variant_t)L"hidden");
												pLayerElement->appendChild(pItemElement,NULL);
												pLayerElement->Release();
												if(DevLink)delete DevLink;
											}
										}
										pLayersElement->Release();
									}else
									{
										nType = 2;

										//add business page
										if(worknodeguid.bstrVal)
										{
											LPTSTR DevLink = GetFlowNodeDevLink(pWkSheet,worknodeguid.bstrVal);
											if(DevLink)
											{
												xml.m_pXmlDoc->createElement(L"layer",&pLayerElement);
												pLayerElement->setAttribute(L"caption",caption);
												pDataElement->appendChild(pLayerElement,NULL);

												xml.m_pXmlDoc->createElement(L"xframe",&pItemElement);
												pItemElement->setAttribute(L"src",(_variant_t)DevLink);
												pItemElement->setAttribute(L"visible",(_variant_t)L"hidden");
												pLayerElement->appendChild(pItemElement,NULL);
												pLayerElement->Release();
												if(DevLink)delete DevLink;
											}
										}
									}
								}

								pDataElement->Release();

								pList->Release();	
							}

							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							xfUriLoader::UrlCombineW(pServer,_T("/business/xpage.xml"),strUrl,&dwSize,0);

							xfXPage * pXPage = new xfXPage();
							pXPage->m_pStrUrl = _tcsdup(strUrl);
							pXPage->m_pWkStrUrl = NULL;
							pXPage->Load(pElementEx);
							pElementEx->Release();
							
							xfWin::LoadXPageXml(pXPage,(HWND)m_pWorkStart?m_pWorkStart->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)nType);
						}
					}
				}else
				{
					nType = 3;

					xbXml xml;
					int nRet = xfUriLoader::FetchXml(pServer,L"dev:xpage{7C07E0CA-5EDB-4FFB-ABEB-FC10A9E316BB}",NULL,xml);
					if(nRet!=1) return 1;

					//IXMLDOMElement * pElement;
					IXMLDOMNode * pNode;
					IXMLDOMElement * pDataElement;
					IXMLDOMElement * pElementEx;
					
					xml.m_pXmlDoc->get_documentElement(&pElementEx);
					if(!pElementEx) return true;	
											
					IXMLDOMElement * pLayerElement;
					IXMLDOMElement * pItemElement;
					pElementEx->selectSingleNode(L"//layers",&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pDataElement);
						pNode->Release();
					}

					variant_t caption;
					variant_t worknodeguid;
					pElement->getAttribute(L"subject",&caption);
					pElement->getAttribute(L"worknodeguid",&worknodeguid);
					if(worknodeguid.bstrVal)
					{
						LPTSTR DevLink = GetFlowNodeDevLink(pWkSheet,worknodeguid.bstrVal);
						if(DevLink)
						{
							xml.m_pXmlDoc->createElement(L"layer",&pLayerElement);
							pLayerElement->setAttribute(L"caption",caption);
							pDataElement->appendChild(pLayerElement,NULL);

							xml.m_pXmlDoc->createElement(L"xframe",&pItemElement);
							pItemElement->setAttribute(L"src",(_variant_t)DevLink);
							pItemElement->setAttribute(L"visible",(_variant_t)L"hidden");
							pLayerElement->appendChild(pItemElement,NULL);
							pLayerElement->Release();
							if(DevLink)delete DevLink;
						}

						pDataElement->selectSingleNode(L"layer[1]",&pNode);
						if(pNode)
						{
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pLayerElement);
							pNode->Release();
							pLayerElement->setAttribute(L"workflow",var);
							pLayerElement->setAttribute(L"worknode",worknodeguid);
							pLayerElement->Release();
						}

						TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
						DWORD dwSize = INTERNET_MAX_URL_LENGTH;
						xfUriLoader::UrlCombineW(pServer,_T("/business/xpage.xml"),strUrl,&dwSize,0);

						xfXPage * pXPage = new xfXPage();
						pXPage->m_pStrUrl = _tcsdup(strUrl);
						pXPage->m_pWkStrUrl = NULL;
						pXPage->Load(pElementEx);
						pElementEx->Release();
						
						xfWin::LoadXPageXml(pXPage,(HWND)m_pWorkStart?m_pWorkStart->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)nType);
					}
					pDataElement->Release();
				}
			}
		}
		return 1;
	}

	int RemoveBy(xbObject *pObject)
	{
		delete this;
		return 1;
	};
};

class IReportEventImplEx : public XOffice::XReport::IReportEvent
{
public:
	CWorkStartEx * m_pWorkStart;
	TCHAR m_rdf[255];
	TCHAR m_action[255];
	TCHAR m_pid[128];
	TCHAR m_taskid[128];
	TCHAR m_no[128];
	TCHAR m_id[128];
	bool  m_btask;
	int OnRowFoucsChanged(int nNewRow,int nOldRow)
	{
		XOffice::XReport::XReportSheet* pSheet = (XOffice::XReport::XReportSheet*)m_pWorkStart->GetControl(_T("maillist"));
		XOffice::XReport::XReportSheetSvr* pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;
		IXMLDOMElement * pElement = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetRowElement(nNewRow);

		_variant_t var;
		pElement->getAttribute(L"id",&var);
		if(var.bstrVal)
		{
			::SetWindowText(m_pWorkStart->GetControl(_T("mailcontent"))->m_hWnd,NULL);


			LPTSTR nMailID = var.bstrVal;
			TCHAR buf[64];
			LPBYTE pData;
			int nLen;
			::_stprintf_s(buf,64,_T("%s"),nMailID); 

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(m_pWorkStart->m_pXPage->m_pStrUrl,_T("uriserver/uri.hxsoft.com/xaserver/entity/mail/querybody"),strUrl,&dwSize,0);

			if( xfUriLoader::Load(strUrl,NULL,buf,pData,nLen,false)==1)
			{
				CDO::IMessage* pMsg = NULL;
				HRESULT hr = CoCreateInstance( __uuidof(CDO::Message), NULL, CLSCTX_INPROC_SERVER, __uuidof(CDO::IMessage), (void**) &pMsg );

				/*
				int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1,  NULL, 0);
				TCHAR* pszA = new TCHAR[_convert];
				int nRet = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1, pszA, _convert);
				*/


				SAFEARRAY FAR* psa;
				SAFEARRAYBOUND rgsabound[1];
				rgsabound[0].lLbound = 0;
				rgsabound[0].cElements = nLen - 2;
				psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
				psa->pvData = pData;

				VARIANT var;
				::VariantInit(&var);
				var.vt = VT_ARRAY|VT_UI1;
				var.parray = psa;

				_variant_t   varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);  
				_StreamPtr   adostream;
				adostream.CreateInstance(_T( "ADODB.Stream"));
				adostream-> Type   =   adTypeBinary;
				//adostream-> Type   =   adTypeText;//adTypeBinary;
				adostream->put_Mode(adModeReadWrite);
				adostream-> Open(varOptional,adModeUnknown,   adOpenStreamUnspecified,   _T(""),   _T(""));  
				//adostream-> LoadFromFile( "d:\\aa.txt");
				//adostream->WriteText((LPTSTR)pData,adWriteChar);
				adostream->Write(var);
				//adostream->put_Position(0);
				//_variant_t   vReadTo   =   adostream-> ReadText(adReadAll);  
				long   blob_size   =   adostream-> GetSize();


				CDO::IDataSource * pDataSource;
				pMsg->get_DataSource(&pDataSource);
				hr = pDataSource->OpenObject(adostream,L"_Stream");
				adostream-> Close(); 
				adostream->Release();

				//pMsg->put_HTMLBody((LPTSTR)pszA);
				CDO::IBodyParts* pAttach;
				CDO::IBodyPart* pItem = NULL;
				pMsg->get_Attachments(&pAttach);
				long ln;
				pAttach->get_Count(&ln);

				xcLayerSheet * pl = (Hxsoft::XFrame::xcLayerSheet *)m_pWorkStart->GetControl(_T("wknodelayer"));
				m_btask = false;
				if(pl)
				{
					while(pl->GetSheetCount() > 1)((ILayerSheet *)pl->GetInterface())->DeleteSheet(1); //pl->RemoveSheet(1);
					if(ln >= 1)
					{
						pAttach->get_Item(1,&pItem);
						if(pItem)
						{
							BSTR str;
							pItem->get_ContentMediaType(&str);
							if(str && _tcsicmp(str,L"application/worknode")==0)
							{
								CDO::_Stream * pstream = NULL;
								pItem->GetDecodedContentStream(&pstream);
								pstream->put_Type((CDO::StreamTypeEnum)adTypeText);
								
								BSTR wknode;
								pstream->ReadText(adReadLine,&wknode);
								while(wknode && wknode[0])
								{
									TCHAR * sp = _tcschr(wknode,':');
									if(sp)
									{
										sp[0] = 0;
										TCHAR* sname = wknode;
										TCHAR* surl = sp + 1;
										pl->InsertSheet(-1,sname);
									}
									::SysFreeString(wknode);
									pstream->ReadText(-2,&wknode);
								}
			
								::SysFreeString(str);
								if(pstream)pstream->Release();
							}else if(str && _tcsicmp(str,L"application/workflow")==0)
							{
								CDO::_Stream * pstream = NULL;
								pItem->GetDecodedContentStream(&pstream);
								pstream->put_Type((CDO::StreamTypeEnum)adTypeText);
								
								BSTR wknode;
								pstream->ReadText(adReadLine,&wknode);

								LPTSTR caption  = NULL;
								LPTSTR id		= NULL;
								LPTSTR no		= NULL;
								LPTSTR uri		= NULL;
								LPTSTR env		= NULL;
								LPTSTR action	= NULL;
								LPTSTR para		= NULL;
								LPTSTR pid		= NULL;
								LPTSTR taskid	= NULL;
								LPTSTR data		= NULL;

								while(wknode && wknode[0])
								{
									TCHAR * sp = _tcschr(wknode,':');
									if(sp)
									{
										sp[0] = 0;
										TCHAR* sname = wknode;
										TCHAR* surl = sp + 1;
										while(surl[0]==' '||surl[0]=='\t') surl++;
										if(_tcsicmp(sname,L"caption")==0)  caption=_tcsdup(surl);
										if(_tcsicmp(sname,L"id")==0)		id=_tcsdup(surl);
										if(_tcsicmp(sname,L"no")==0)		no=_tcsdup(surl);
										if(_tcsicmp(sname,L"uri")==0)		uri=_tcsdup(surl);
										if(_tcsicmp(sname,L"action")==0)	action=_tcsdup(surl);
										if(_tcsicmp(sname,L"para")==0)		para=_tcsdup(surl);
										if(_tcsicmp(sname,L"pid")==0)		pid=_tcsdup(surl);
										if(_tcsicmp(sname,L"taskid")==0)	taskid=_tcsdup(surl);
										if(_tcsicmp(sname,L"data")==0)		data=_tcsdup(surl);
									}
									::SysFreeString(wknode);
									pstream->ReadText(-2,&wknode);
								}
								

								if(uri && action)
								{
									m_btask = true;
									if(uri)_tcscpy_s(m_rdf,	255,	uri);
									if(action)_tcscpy_s(m_action, 255,	action);
									if(pid)_tcscpy_s(m_pid,	128,	pid);
									if(taskid)_tcscpy_s(m_taskid,	128,	taskid);
									if(no)_tcscpy_s(m_no,		128,	no);
									if(id)_tcscpy_s(m_id,		128,	id);

									xbXml x;
									IXAServerArg arg;
									arg.AddArg(L"rdf",uri);
									arg.AddArg(L"action",action);
									arg.AddArg(L"env","view");

									HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
									hCursor = ::SetCursor(hCursor);

									IXMLDOMDocument * pd;
									IXMLDOMNodeList * pList;
									IXMLDOMNode*	  pNode,* pNode1;
									IXMLDOMElement*   pElement;
									IXMLDOMElement * pEle;
									long nlen;

									//append child menu
									if(IXAServer::ExecXQuery(m_pWorkStart->m_pXPage->m_pStrUrl,L"[business.task.action]",arg.GetString(), x)==1)
									{
										pl->m_pxfNode->m_pElement->get_ownerDocument(&pd);


										xcToolButton * pButton = (xcToolButton *)m_pWorkStart->GetControl(L"xmMailOperator");
										pButton->m_pxfNode->m_pElement->selectSingleNode(L"box",&pNode);

										pNode->get_childNodes(&pList);
										pList->get_length(&nlen);
										for(int i=nlen -1;i>=0;i--)
										{
											pList->get_item(i,&pNode1);
											DOMNodeType ty;
											pNode1->get_nodeType(&ty);
											if(ty==NODE_ELEMENT) pNode->removeChild(pNode1,NULL);
											pNode1->Release();
										}
										x.GetXmlDoc()->selectNodes(L"//action",&pList);
										pList->get_length(&nlen);
										for(int i=0;i<nlen;i++)
										{
											pList->get_item(i,&pNode1);
											pNode1->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
											
											_variant_t vName,vCaption;
											pElement->getAttribute(L"for",&vName);
											pElement->getAttribute(L"tool",&vCaption);
											if(vCaption.bstrVal && vName.bstrVal)
											{	
												pd->createElement(L"xtoolbutton",&pEle);
												pEle->setAttribute(L"height",(_variant_t)L"20");
												pEle->setAttribute(L"caption",vCaption);
												pEle->setAttribute(L"name",vName);

												pNode->appendChild(pEle,NULL);
												pEle->Release();
											}
											pElement->Release();
											pNode1->Release();
										}
										if(pNode)pNode->Release();
									}

									if(IXAServer::ExecXQuery(m_pWorkStart->m_pXPage->m_pStrUrl,L"[business.taskflow]",arg.GetString(), x)==1)
									{
										
										BSTR str = x.GetXml();
										if(str)::SysFreeString(str);
										x.GetXmlDoc()->selectNodes(L"//item",&pList);
										pList->get_length(&nlen);
										for(int i=0;i<nlen;i++)
										{
											pList->get_item(i,&pNode);
											pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
											
											_variant_t vUrl,vCaption;
											pElement->getAttribute(L"url",&vUrl);
											pElement->getAttribute(L"caption",&vCaption);
											if(vCaption.bstrVal && vUrl.bstrVal)
											{
												//if(vCaption.bstrVal) pl->InsertSheet(-1,vCaption.bstrVal);

												IXMLDOMDocument * pdoc;
												pl->m_pxfNode->m_pElement->get_ownerDocument(&pdoc);

												IXMLDOMElement * pe;
												pdoc->createElement(L"xframe",&pe);
												pe->setAttribute(L"src",vUrl);
												pe->setAttribute(L"visible",(_variant_t)L"hidden"); 
												int nIndex = ((ILayerSheet *)pl->GetInterface())->InsertSheet(-1,vCaption.bstrVal,pe);

												TCHAR * pStr = new TCHAR[_tcslen(vCaption.bstrVal) + _tcslen(vUrl.bstrVal) + 2];
												_stprintf(pStr,L"%s:%s",vUrl.bstrVal, vCaption.bstrVal);
												((ILayerSheet *)pl->GetInterface())->SetParam(nIndex, pStr);
												pe->Release();
												pdoc->Release();
											}
											pElement->Release();
											pNode->Release();
										}
										pList->Release();
									}
									hCursor = ::SetCursor(hCursor);
									DestroyCursor(hCursor);
								}
								if(caption)delete caption;
								if(id)delete id;
								if(no)delete no;
								if(uri)delete uri;
								if(env)delete env;
								if(action)delete action;
								if(para)delete para;
								if(pid)delete pid;
								if(taskid)delete taskid;
								if(data)delete data;
			
								::SysFreeString(str);
								if(pstream)pstream->Release();
							}
							pItem->Release();
						}
					}
				}
				pAttach->Release();

				BSTR bstrText;
				BSTR bstrHtml = NULL;
				try
				{
					pMsg->get_TextBody(&bstrText);
					if(bstrText)
					{
						::SetWindowText(m_pWorkStart->GetControl(_T("mailcontent"))->m_hWnd,bstrText);
					}
					//::SysFreeString(bstr);

					pMsg->get_HTMLBody(&bstrHtml);
				}catch(...)
				{
				}

				if(true)
				{
					IWebBrowser2	*webBrowser2;
					LPDISPATCH		lpDispatch;
					IHTMLDocument2	*htmlDoc2;
					SAFEARRAY		*sfArray;
					VARIANT			*pVar;
					BSTR			bstr = NULL;
					SAFEARRAYBOUND ArrayBound = {1, 0};


					xcOleControlContain*  pControl = dynamic_cast<xcOleControlContain *>(m_pWorkStart->GetControl(_T("mailcontentex")));
					if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
					{
						if (!webBrowser2->get_Document(&lpDispatch))
						{
							if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
							{
								//htmlDoc2->put_designMode(L"on");
								if(m_pWorkStart->m_viewType== CWorkStartEx::VT_Text || ((!bstrHtml || !bstrHtml[0]) && bstrText))
								{
									IHTMLElement *pElement;
									htmlDoc2->get_body(&pElement);
									pElement->put_innerText(bstrText);
								}
								else
								{
									if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND *)&ArrayBound)))
									{
										if (!SafeArrayAccessData(sfArray, (void**)&pVar))
										{
											pVar->vt = VT_BSTR;
											bstr = SysAllocString(bstrHtml);

											if ((pVar->bstrVal = bstr))
											{
												htmlDoc2->write(sfArray);
												htmlDoc2->close();
											}
										}
										SafeArrayDestroy(sfArray);
									}
								}

								htmlDoc2->Release();
							}
							lpDispatch->Release();
						}
					}
				}
				if(bstrText)::SysFreeString(bstrText);
				if(bstrHtml)::SysFreeString(bstrHtml);

				pMsg->Release();
				//delete pszA;

				xfUriLoader::FreeData(pData);
			}else
			{
				SetWindowText(m_pWorkStart->GetControl(_T("mailcontent"))->m_hWnd,L"");
				
				IWebBrowser2	*webBrowser2;
				LPDISPATCH		lpDispatch;
				IHTMLDocument2	*htmlDoc2;

				xcOleControlContain*  pControl = dynamic_cast<xcOleControlContain *>(m_pWorkStart->GetControl(_T("mailcontentex")));
				if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
				{
					if (!webBrowser2->get_Document(&lpDispatch))
					{
						if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
						{
							IHTMLElement *pElement;
							htmlDoc2->get_body(&pElement);
							pElement->put_innerText(L"");
							htmlDoc2->Release();
						}
						lpDispatch->Release();
					}
					webBrowser2->Release();
				}
			}
		}

		return 1;
	}
	int RemoveBy(xbObject *pObject)
	{
		delete this;
		return 1;
	};
	int OnDoubleClicked(int nRow)
	{
		LPCTSTR pStr = L"worknode:{E90E04A6-4A01-4C3E-9B9A-ED8C2AAA8519}/open#1";
		XOffice::XReport::XReportSheet* pSheet;
		XOffice::XReport::XReportSheetSvr* pSheetSvr;
		pSheet = (XOffice::XReport::XReportSheet*)m_pWorkStart->GetControl(_T("maillist"));
		pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		if(nRow >0)
		{
			LPCTSTR pMailID = pSheetSvr->GetItemString(nRow,_T("id"));
			TEntityParam * pParam = new TEntityParam;
			::memset(pParam,0,sizeof(TEntityParam ));
			_tcscpy_s(pParam->pKeyValue,255,pMailID);

			IXFWin::OpenWindow(m_pWorkStart->m_pXPage->m_pStrUrl,pStr,(IXFWin *)m_pWorkStart->GetInterface(),(LPARAM)pParam);
		}
		return 1;
	}

};

CWorkStartEx::CWorkStartEx(void):m_viewType(VT_SafeHtml)
{
}

CWorkStartEx::~CWorkStartEx(void)
{
}

int CWorkStartEx::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmChartSave"))==0)
	{
		XOffice::XReport::XReportSheet* pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("tasklist"));
		XOffice::XReport::XReportSheetSvr* pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		int nRow = pSheetSvr->GetRow();
		if(nRow  < 1) return 1;
		int Row = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetContentRow(nRow);
		IXMLDOMElement * pElement = pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetRowElement(Row);

		if(!pElement) return 1;

		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
			

		_variant_t var;
		pElement->getAttribute(L"workfolwguid",&var);
		if(var.bstrVal)
		{
			
			XOffice::XFlowChart::XFlowSheet* pSheet =  (XOffice::XFlowChart::XFlowSheet*)GetControl(_T("flowchartctrl"));
			XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr;
			pDataSvr = (XOffice::XFlowChart::XFlowChartDataSvr *)pSheet->m_pSheetSvr->m_pDataSvr;

			xbXml xml;
			xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xdoc  version='3.0' editer='xoffice'><sheet version='3.0'/></xdoc>");
			
			IXMLDOMElement * pElementEx;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"xdoc/sheet",&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
			pNode->Release();
			
			pSheet->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(pSheet->m_pSheetSvr->m_pDataSvr,pElementEx,xml.m_pXmlDoc);
			pElementEx->Release();

			BSTR bstr;
			xml.m_pXmlDoc->get_xml(&bstr);

			TCHAR strCmd[512];
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/workflow:%s/setchart"),var.bstrVal);

			BYTE * pData = NULL;
			int nLen;
			xfUriLoader::ProcessCmd(this->m_pXPage->m_pStrUrl,strCmd,bstr,pData,nLen);

			if(pData)xfUriLoader::FreeData(pData);
			if(bstr)
			{
				::SysFreeString(bstr);
			}
		}
		pElement->Release();

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);

		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmViewText"))==0)
	{
		m_viewType = VT_Text;
		if(GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption)
		{
			delete GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption;
			GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption = _tcsdup(_T("纯文本"));
			::InvalidateRect(GetControl(_T("xmViewType"))->m_hWnd,NULL,false);

			XOffice::XReport::XReportSheet* pSheet;
			XOffice::XReport::XReportSheetSvr* pSheetSvr;
			pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
			pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;
			int nRow = pSheetSvr->GetRow();
			if(nRow > 0)
				m_pMailListon->OnRowFoucsChanged(nRow,nRow);
		}
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmViewSafeHtml"))==0)
	{
		m_viewType = VT_SafeHtml;
		if(GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption)
		{
			delete GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption;
			GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption = _tcsdup(_T("安全网页"));
			::InvalidateRect(GetControl(_T("xmViewType"))->m_hWnd,NULL,false);
			
			XOffice::XReport::XReportSheet* pSheet;
			XOffice::XReport::XReportSheetSvr* pSheetSvr;
			pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
			pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;
			int nRow = pSheetSvr->GetRow();
			if(nRow > 0)
				m_pMailListon->OnRowFoucsChanged(nRow,nRow);
		}
		return 1;
	}
	if(pStrID && _tcsstr(pStrID,_T("hx-action:")))
	{
		XOffice::XReport::XReportSheet* pSheet;
		XOffice::XReport::XReportSheetSvr* pSheetSvr;
		pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
		pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		int nRow = pSheetSvr->GetRow();
		if(nRow >0)
		{
			LPCTSTR pMailID = pSheetSvr->GetItemString(nRow,_T("id"));
			TEntityParam * pParam = new TEntityParam;
			::memset(pParam,0,sizeof(TEntityParam ));

			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			_tcscpy_s(pParam->pKeyValue ,255,pMailID); 
			_tcscpy_s(pParam->pEntity ,255,m_pMailListon->m_rdf); 
			_tcscpy_s(pParam->pWorkNodePath ,255,pStrID); 

			IXFWin::OpenWindow(m_pXPage->m_pStrUrl,L"dev:xpage[business.mail]",(IXFWin *)GetInterface(),(LPARAM)pParam);

			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
	}

	if(pStrID && _tcsstr(pStrID,_T("MailOpen:")))
	{
		LPCTSTR pStr = _tcschr(pStrID,':') + 1;
		XOffice::XReport::XReportSheet* pSheet;
		XOffice::XReport::XReportSheetSvr* pSheetSvr;
		pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
		pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		int nRow = pSheetSvr->GetRow();
		if(nRow >0)
		{
			LPCTSTR pMailID = pSheetSvr->GetItemString(nRow,_T("id"));
			TEntityParam * pParam = new TEntityParam;
			::memset(pParam,0,sizeof(TEntityParam ));
			pParam->pData = (LPARAM)pMailID;

			IXFWin::OpenWindow(m_pXPage->m_pStrUrl,pStr,(IXFWin *)GetInterface(),(LPARAM)pParam);
		}
		return 1;
	}
	if(pStrID && _tcsstr(pStrID,_T("FolderChanged:")))
	{
		LPCTSTR pStr = _tcschr(pStrID,':') + 1;
		if(_tcsicmp(pStr,_T("localfolder"))!=0 && _tcsicmp(pStr,_T("mail"))!=0)
		{
			XOffice::XReport::XReportSheet* pSheet;
			XOffice::XReport::XReportSheetSvr* pSheetSvr;
			pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
			pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

			pSheetSvr->Retrieve((LPTSTR)pStr);
			if(pSheetSvr->GetRowCount()>=1)
				m_pMailListon->OnRowFoucsChanged(1,1);
		}
		return 1;
	}
	return CDefaultView::OnXCommand(pStrID,pControl);
}


int CWorkStartEx::OnOpen()
{
	XOffice::XReport::XReportSheet* pSheet;
	XOffice::XReport::XReportSheetSvr* pSheetSvr;

	IReportEventImpl * pEvt = new IReportEventImpl();
	pEvt->m_pWorkStart = this;
	this->GetControl(_T("tasklist"))->AddObserver(pEvt);

	pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("tasklist"));
	pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;
	XOffice::XXmlContentSvr * pContentSvr = (XOffice::XXmlContentSvr *)((XOffice::XReport::XReportDataSvr *)pSheetSvr->m_pDataSvr)->m_pContentSvr;
	this->MakeData(pContentSvr);

	if(pSheetSvr->GetRowCount()>=1)pEvt->OnRowFoucsChanged(1,1);
	m_pTaskListon = pEvt;

	if(this->GetControl(_T("maillist")))
	{
		IReportEventImplEx * pEvtEx = new IReportEventImplEx();
		pEvtEx->m_btask = false;
		pEvtEx->m_pWorkStart = this;
		this->GetControl(_T("maillist"))->AddObserver(pEvtEx);

		pSheet = (XOffice::XReport::XReportSheet*)GetControl(_T("maillist"));
		pSheetSvr = (XOffice::XReport::XReportSheetSvr*)pSheet->m_pSheetSvr;

		if(pSheetSvr->GetRowCount()>=1)pEvtEx->OnRowFoucsChanged(1,1);
		m_pMailListon = pEvtEx;
	}


	return CDefaultView::OnOpen();
}

