#include "stdafx.h"
#include "xnode.h"
#include "xutil.h"
#include "xcontrolmgr.h"
#include "xapp.h"
#include <vector>
#include "commctrl.h"
#include "xtoolbutton.h"
#include "xwin.h"
#include "xmdiframe.h"
#include "xtreectrl.h"
#include "xsedit.h"
#include "xcellctrl.h"
#include "xpropsheet.h"
#include "xlayersheet.h"
#include "xwindesign.h"
#include "xword.h"
#include "xflowtable.h"

using namespace std;
using namespace Hxsoft::XFrame;

namespace Hxsoft
{
	namespace XFrame
	{
		XEditTemplate::XEditTemplate()
		{
			m_pTemplateElemnt = new vector<IXMLDOMElement *>;
			m_pTempletName = new vector<LPTSTR>;
			m_pTempletCategory = new vector<LPTSTR>;

		}
		int XEditTemplate::CreateXmlDoc(IXMLDOMDocument * &pXmlDocument)
		{
			return XApp::CreateXmlDoc(pXmlDocument);
		}
		XEditTemplate::~XEditTemplate()
		{
			for(int i=0;i<(int)m_pTemplateElemnt->size();i++)
			{
				//if((*m_pTemplateElemnt)[i]) (*m_pTemplateElemnt)[i]->Release(); 
				if((*m_pTempletName)[i])delete (*m_pTempletName)[i]; 
				if((*m_pTempletCategory)[i])delete (*m_pTempletCategory)[i];
			}
			if(m_pTemplateElemnt) delete m_pTemplateElemnt;
			if(m_pTempletName) delete m_pTempletName;
			if(m_pTempletCategory) delete m_pTempletCategory;
		}

		int XEditTemplate::AddXmlTemplate(LPTSTR pCategory, LPTSTR pStrName, IXMLDOMElement * pElement)
		{
			if(!pElement) return 0;
			pElement->AddRef();
			m_pTemplateElemnt->push_back(pElement);
			m_pTempletName->push_back(pStrName);
			m_pTempletCategory->push_back(pCategory);
			return 1;

		}
		int XEditTemplate::AddXmlTemplate(LPTSTR pCategory,LPTSTR pStrName, LPTSTR pXmlStr)
		{
			VARIANT_BOOL ret;
			IXMLDOMElement *	pElement;
			_bstr_t bstrXml = pXmlStr;

			IXMLDOMDocument * pXmlDoc ;
			CreateXmlDoc(pXmlDoc);
			pXmlDoc->loadXML(bstrXml,&ret);
			pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				this->AddXmlTemplate(pCategory,pStrName,pElement);
				pElement->Release();
			}
			pXmlDoc->Release();
			return 1;

		}
		int XEditTemplate::AddXmlFileTemplate(LPTSTR pCategory,LPTSTR pStrName, LPTSTR pStrFileName)
		{
			VARIANT_BOOL ret;
			IXMLDOMElement *	pElement;
			_variant_t bstrFile = pStrFileName;

			IXMLDOMDocument * pXmlDoc ;
			CreateXmlDoc(pXmlDoc);
			pXmlDoc->load(bstrFile,&ret);
			pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				this->AddXmlTemplate(pCategory,pStrName,pElement);
				pElement->Release();
			}
			pXmlDoc->Release();
			return 1;
		}
		int XEditTemplate::AddXmlTemplateParseFile(LPTSTR pCategory,LPTSTR pStrFileName)
		{
			VARIANT_BOOL ret;
			IXMLDOMElement *	pElement, *pElement1 = NULL;
			IXMLDOMNode*	pNode;
			IXMLDOMNodeList* pList,* pList1;
			DOMNodeType NodeType;
			long	len,len1;
			_variant_t	val;
			LPTSTR pStrName;

			IXMLDOMDocument * pXmlDoc ;
			CreateXmlDoc(pXmlDoc);
			_variant_t bstrFile = pStrFileName;
			pXmlDoc->load(bstrFile,&ret);
			pXmlDoc->selectNodes(L"/templates/template",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				pList->get_item(i,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();
				if(pElement)
				{
					pElement->getAttribute(L"name",&val);
					pStrName = ::_tcsdup(((_bstr_t)val));

					pElement->get_childNodes(&pList1);
					pList1->get_length(&len1);
					int k;
					for(k=0;k <len1;k++)
					{
						pList1->get_item(k,&pNode);
						pNode->get_nodeType(&NodeType);
						if(NodeType==NODE_ELEMENT) break;
						pNode->Release();
					}
					if(k<len1)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
						pNode->Release();
						if(pElement1)
						{
							this->AddXmlTemplate(pCategory,pStrName,pElement1);
							pElement1->Release();
						}
					}
					pList1->Release();
					pElement->Release();
				}
			}
			pList->Release();
			pXmlDoc->Release();
			return 1;
		}


		int XEditTemplate::AddXmlTemplate(LPTSTR pStrName, IXMLDOMElement * pElement)
		{
			return AddXmlTemplate(NULL,pStrName,pElement);
		}
		int XEditTemplate::AddXmlTemplate(LPTSTR pStrName, LPTSTR pXmlStr)
		{
			return AddXmlTemplate(NULL,pStrName,pXmlStr);
		}
		int XEditTemplate::AddXmlFileTemplate(LPTSTR pStrName, LPTSTR pStrFileName)
		{
			return AddXmlFileTemplate(NULL,pStrName,pStrFileName);
		}
		IXMLDOMElement * XEditTemplate::FindTemplate(LPTSTR pStrName)
		{
			return FindTemplate(NULL,pStrName);
		}
		IXMLDOMElement * XEditTemplate::FindTemplate(LPTSTR pCategory, LPTSTR pStrName)
		{
			int i,i0 = -1;
			for(i=0;i<(int)m_pTemplateElemnt->size();i++)
			{
				if(_tcscmp(pStrName,(*m_pTempletName)[i])!=0)continue;
				if(pCategory && (*m_pTempletCategory)[i] && 
					_tcscmp(pCategory,(*m_pTempletCategory)[i])==0)
				{
					return (*m_pTemplateElemnt)[i];
				}else 
					i0 = i;
			}
			if(i0>=0) return (*m_pTemplateElemnt)[i0];
			return NULL;
		}

		XLayers::XLayers():nCurLayer(-1),m_pNode(NULL),m_bTopArrow(0)
		{
			pLayers = new vector<XNode *>;
			pCtrlWinCreated = new vector<bool>;
		}
		XLayers::~XLayers()
		{
			for(int i=0;i<(int)pLayers->size();i++)
			{
				if(i!=nCurLayer) //nCurSheet has deleted by entry tree
				{
					if((*pLayers)[i])delete (*pLayers)[i];
				}
			}
			pLayers->clear();
			delete pLayers;
			if(m_pNode) delete m_pNode;

			if(pCtrlWinCreated) 
			{
				pCtrlWinCreated->clear();
				delete pCtrlWinCreated;
			}
		}
		void * XLayers::GetLayerSheetCtrl()
		{
			return m_bTopArrow ==1?m_pNode->m_pRightNode->m_pControlWins[0]:m_pNode->m_pLeftNode->m_pControlWins[0];
		}

		int XLayers::ParseNode(XNode * pPparentNode, IXMLDOMElement * pElement)
		{
			long	len;
			IXMLDOMNodeListPtr pList;
			IXMLDOMNodePtr	pNode;
			if(m_pNode) delete m_pNode;
			m_pNode = new XNode();
			m_pNode->m_pParentNode = pPparentNode;
			m_pNode->ParseNode(pElement);
	
			
			IXMLDOMElement * pElement0;
			IXMLDOMElement * pElementCtrl;
			IXMLDOMDocumentPtr  pXmlDoc;

			_variant_t	val;
			pElement->getAttribute(L"tabarrow",&val);
			if(val.bstrVal && wcscmp(L"top",val.bstrVal)!=0)this->m_bTopArrow=1; //bottom

			pElement->get_ownerDocument(&pXmlDoc);
			pXmlDoc->createElement(L"box",&pElement0);
			pElement0->setAttribute(L"height",(_variant_t)30);
			pElement0->setAttribute(L"style",(_variant_t)_T("margin:1 0 0 0"));
			pXmlDoc->createElement(L"xlayersheet",&pElementCtrl);
			pElement0->appendChild(pElementCtrl,NULL);

			XNode*	pCtrlNode = new XNode();
			if(m_bTopArrow==1)	//bottom
			{
				pCtrlNode->m_spStatus &= ~SP_LEFT;
				pCtrlNode->m_spStatus |= SP_RIGHT;
				m_pNode->m_pRightNode = pCtrlNode;
				pElement0->setAttribute(L"height",(_variant_t)21);
			}
			else
				m_pNode->m_pLeftNode = pCtrlNode;
			pCtrlNode->m_pParentNode = m_pNode;
			pCtrlNode->ParseNode(pElement0);

			pElement->selectNodes(L"layer",&pList);
			pList->get_length(&len);
			pLayers->clear();
			for(int i=0;i<len;i++)
			{
				XNode * pChild = new XNode();
				pChild->m_pParentNode = m_pNode;
				if(m_bTopArrow==0)	//bottom
				{
					pChild->m_spStatus &= ~SP_LEFT;
					pChild->m_spStatus |= SP_RIGHT;
				}
				pLayers->push_back(pChild);
				pCtrlWinCreated->push_back(i==nCurLayer?true:false);
				pList->get_item(i,&pNode);
				pChild->ParseNode(pNode);
			}

			if(nCurLayer<0 && (int)pLayers->size()>0)nCurLayer = 0;
			if(nCurLayer>=0 && nCurLayer < (int)pLayers->size())
			{
				if(m_bTopArrow==0)	//bottom
				{
					m_pNode->m_pRightNode =  (*pLayers)[nCurLayer];
				}else
				{
					m_pNode->m_pLeftNode =  (*pLayers)[nCurLayer];
				}
			}
			pPparentNode->m_pLeftNode = m_pNode;

			return 1;
		}
		
		int XLayers::AddSheet(IXMLDOMElement * pElement)
		{
			XNode * pChild = new XNode();
			pChild->m_pParentNode = m_pNode;
			if(m_bTopArrow==0)	//top
			{
				pChild->m_spStatus &= ~SP_LEFT;
				pChild->m_spStatus |= SP_RIGHT;
			}
			pLayers->push_back(pChild);
			pCtrlWinCreated->push_back(false);
			pChild->ParseNode(pElement);

			return 1;
		}
		int AddXPageSheet(LPTSTR pStrCaption)
		{
			return 1;
		}

		int XLayers::SwitchTo(int nIndex)
		{
			if(nIndex >= (int)pLayers->size()) return -1;
			if(nCurLayer == nIndex) return 1;
			

			if(m_bTopArrow==1)
				m_pNode->m_pLeftNode = (*pLayers)[nIndex];
			else
				m_pNode->m_pRightNode = (*pLayers)[nIndex];

			if(!(*pCtrlWinCreated)[nIndex])
			{
				(*pLayers)[nIndex]->CreateNodeControl(m_pNode->GetRootNode()->m_pOwnerWin->m_hWnd);
				(*pCtrlWinCreated)[nIndex] = true;
			}

			(*pLayers)[nIndex]->AdjustRect();
			(*pLayers)[nIndex]->AdjustCtrlRect();
			if(nCurLayer >=0 && nCurLayer <  (int)pLayers->size())
			{
				(*pLayers)[nCurLayer]->HideCtrlWin();
			}
			(*pLayers)[nIndex]->ShowCtrlWin();
			nCurLayer = nIndex;

			return 1;
		}


		int XSpliterBar::OnPaint(HDC hdc)
		{
			return 1;
		}

		//draw sizebar
		void XSpliterBar::DrawSizeBar(HWND hWnd,POINT pt)	
		{
			HDC hDC = ::GetDC(hWnd);
			RECT rc = this->GetBarRect();
			HBRUSH brush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
			if(this->m_bIsVertbar)
			{
				::PatBlt(hDC, pt.x , 
					rc.top,	rc.right - rc.left, rc.bottom - rc.top,PATINVERT);
			}else
			{
				::PatBlt(hDC,rc.left  , pt.y,	rc.right - rc.left, rc.bottom - rc.top,PATINVERT);
			}
			brush = (HBRUSH)::SelectObject(hDC,brush);
			::DeleteObject(brush);
			::ReleaseDC(hWnd,hDC);
		}
		//adjust splite pos
		int XSpliterBar::AdjustSplitePos(POINT pt)
		{
			int dx=0;
			RECT rc = this->GetBarRect();
			if(this->m_bIsVertbar)
				dx = pt.x - rc.left;
			else
				dx = pt.y - rc.top;
			if(dx !=0)
			{
				if(this->m_pLeftNode)m_pLeftNode->AdjustRect(dx);
				if(this->m_pRightNode)m_pRightNode->AdjustRect(dx);
				if(this->m_pLeftNode)m_pLeftNode->AdjustCtrlRect();
				if(this->m_pRightNode)m_pRightNode->AdjustCtrlRect();
			}
			return 1;
		}

		RECT XSpliterBar::GetBarRect()
		{
			RECT	rect;
			::SetRect(&rect, 0,0,0,0);
			if(m_pLeftNode)
			{
				if(m_bIsVertbar)
				{
					rect.left = m_pLeftNode->m_AreaRect.right + 1;
					rect.right = rect.left + this->GetBarWidth();
					rect.top =  m_pLeftNode->m_AreaRect.top;
					rect.bottom = m_pLeftNode->m_AreaRect.bottom;
				}else
				{
					rect.top = m_pLeftNode->m_AreaRect.bottom + 1;
					rect.bottom = rect.top + this->GetBarWidth();
					rect.left =  m_pLeftNode->m_AreaRect.left;
					rect.right = m_pLeftNode->m_AreaRect.right;
				}
			}
			return rect;
		}

		XNode::XNode(void):m_pParentNode(NULL),m_pLeftNode(NULL),m_pRightNode(NULL),m_pCssStyle(NULL),
			m_BoundStatus(0),m_FMargin(0),m_FPadding(0),m_FBorder(0),m_FBackground(0),
			m_pSpliterBar(NULL),m_spStatus(SP_VERT|SP_LEFT),
			m_width(0),m_height(0),m_minWidth(20),m_hopeWidth(240),
			m_pCaption(NULL),m_AlignArrow(AR_NS),m_pOwnerWin(NULL),m_strCtlName(NULL),m_strCtlID(NULL),
			m_pXScript(NULL),m_pXDll(NULL),m_pstrGuid(NULL),m_pLayers(NULL),m_pOwerControl(NULL)
		{
		}

		XNode::~XNode()
		{
			if(m_pCssStyle)delete m_pCssStyle;
			this->ClearChild();
			if(m_pCaption)delete m_pCaption;
			if(m_pXScript)delete m_pXScript;
			if(m_pXDll)delete m_pXDll;
			if(m_pstrGuid)delete m_pstrGuid;
			for(int i=0;i<(int)m_pControlWins.size();i++)
			{
				if(m_pControlWins[i])
				{
					::SetProp(m_pControlWins[i]->m_hWnd,_T("this"),NULL);
					delete m_pControlWins[i];
				}
			}
			m_pControlWins.clear();
			if(m_pSpliterBar) delete m_pSpliterBar;
			if(m_strCtlName) delete m_strCtlName;
			if(m_strCtlID) delete m_strCtlID;
		}

		int GetCssStyles(LPTSTR cssStyleStr,vector<LPTSTR> &cssStyles, vector<LPTSTR> &cssValues)
		{
			_variant_t cs;
			LPTSTR csStr;
			LPTSTR csPos;
			LPTSTR csStyle,csValue;

			csStr = cssStyleStr;
			csPos = ::_tcsstr(csStr,_T(":"));
			while(csPos)
			{
				csStyle = new TCHAR[csPos - csStr + 1];
				csPos[0]='\0';
				::lstrcpy(csStyle,csStr);
				::_tcslwr_s(csStyle,_tcslen(csStyle)+1);
				cssStyles.push_back(csStyle);
				csStr = csPos + 1;
				csPos = ::_tcsstr(csStr,_T(";"));
				if(csPos)
				{
					csPos[0]='\0';
					csValue = new TCHAR[csPos - csStr + 1];
					::lstrcpy(csValue,csStr);
					cssValues.push_back(csValue);
					csStr = csPos + 1;
				}else
				{
					csValue = new TCHAR[::lstrlen(csStr) + 1];
					::lstrcpy(csValue,csStr);
					::_tcslwr_s(csValue,_tcslen(csValue)+1);
					cssValues.push_back(csValue);
					break;
				}
				csPos = ::_tcsstr(csStr,_T(":"));
			}
			return 1;
		}

		vector<HIMAGELIST>&  XNode::GetImageList()
		{
			XNode*	pParent;
			pParent = this->m_pParentNode;
			if(!pParent)
				if(this->m_pOwerControl)
					return this->m_pOwerControl->m_hImageLists;
				else
					return this->m_pOwnerWin->m_hImageLists;
			while(pParent->m_pParentNode)pParent = pParent->m_pParentNode;
			if(pParent->m_pOwerControl)
				return pParent->m_pOwerControl->m_hImageLists;
			return  pParent->m_pOwnerWin->m_hImageLists;
		}
		//clear magin border background padding flag to zero
		int XNode::ClearCssStyle()
		{
			m_FMargin = 0;
			m_FBorder = 0;
			m_FPadding = 0;
			m_FBackground = 0;

			return 1;
		}

		//parse cssstyle string
		int XNode::ParseCssStyle()
		{
			int start = 0;
			vector<LPTSTR> cssStyles;
			vector<LPTSTR> cssValue;
			GetCssStyles(this->m_pCssStyle,cssStyles,cssValue);

			LPTSTR styleStrs[8]={_T("margin-left"),_T("margin-right"),_T("margin-top"),_T("margin-bottom"),
				_T("padding-left"),_T("padding-right"),_T("padding-top"),_T("padding-bottom")};
			UINT styleItem[8]={MarginLeft,MarginRight,MarginTop,MarginBottom,
				PaddingLeft,PaddingRight,PaddingTop,PaddingBottom};
			DWORD* styleCmpItem[8]={&m_FMargin,&m_FMargin,&m_FMargin,&m_FMargin,
				&m_FPadding,&m_FPadding,&m_FPadding,&m_FPadding};

			int * items[8]={ &m_MarginWidths[0],&m_MarginWidths[2],&m_MarginWidths[1],&m_MarginWidths[3],
				&m_PaddingWidths[0],&m_PaddingWidths[2],&m_PaddingWidths[1],&m_PaddingWidths[3]};

			UINT  borderItem[4]={BorderLeft,BorderRight,BorderTop,BorderBottom};
			DWORD* borderCmpItem[4]={&m_FBorder,&m_FBorder,&m_FBorder,&m_FBorder};

			int* borderWidthItem[4]={ &m_BorderWidths[0],&m_BorderWidths[2],&m_BorderWidths[1],&m_BorderWidths[3]};
			COLORREF* borderColorItem[4]={ &m_BorderColors[0],&m_BorderColors[2],&m_BorderColors[1],&m_BorderColors[3]};
			int* borderTypehItem[4]={ &m_BorderType[0],&m_BorderType[2],&m_BorderType[1],&m_BorderType[3]};
			LPTSTR styleBorderStrs[4]={_T("border-left"),_T("border-right"),_T("border-top"),_T("border-bottom")};

			for(int i= (int)cssStyles.size() - 1;i>=start;i--)
			{
				for(int j=0;j<sizeof(styleItem)/sizeof(styleItem[0]);j++)
				{
					if((*styleCmpItem[j]) & styleItem[j]) continue;
					if(::_tcscmp(styleStrs[j],cssStyles[i])==0)
					{
						*items[j] = XUtil::CalcCssStrToNumber(cssValue[i], 10 );
						*styleCmpItem[j] |= styleItem[j];
					}
				}

				for(int j=0;j<sizeof(borderItem)/sizeof(borderItem[0]);j++)
				{
					if(*borderCmpItem[j] & borderItem[j]) continue;
					if(::_tcscmp(styleBorderStrs[j],cssStyles[i])==0)
					{
						vector<LPTSTR> vals;
						XUtil::GetTokens(cssValue[i],_T(" "),vals);
						for(int k=0;k<(int)vals.size();k++)
						{
							::_tcslwr_s(vals[k],_tcslen(vals[k]) + 1);
							//border type
							if(::_tcsstr(vals[k],_T("none")) ||::_tcsstr(vals[k],_T("solid")))
							{
								if(::_tcsstr(vals[k],_T("solid")))*borderTypehItem[j] = 1;		
							}else if(vals[k][0]=='#')
							{
								*borderColorItem[j] = XUtil::GetColourFromString(vals[k],RGB(0,0,0));
							}else
							{
								*borderWidthItem[j] = XUtil::CalcCssStrToNumber(vals[k], 10 );
							}
							delete vals[k];
						}
						*borderCmpItem[j] |= borderItem[j];
					}
				}
				if(!(m_FBackground & Background))
				{
					if(::_tcscmp(cssStyles[i],_T("background"))==0)
					{
						vector<LPTSTR> vals;
						XUtil::GetTokens(cssValue[i],_T(" "),vals);  

						for(int k=0;k<(int)vals.size();k++)
						{
							::_tcslwr_s(vals[k],_tcslen(vals[k]) + 1);
							if(::_tcsstr(vals[k],_T("url(")))
							{
								LPTSTR	strPosStart,strPosEnd;
								strPosStart = ::_tcsstr(vals[k],_T("url(")) + ::lstrlen(_T("url("));
								strPosEnd = ::_tcsstr(strPosStart,_T(")"));
								::lstrcpyn(m_BackgroundUrl,strPosStart, (int)(strPosEnd - strPosStart + 1));
							}

							if(vals[k][0]=='#')
							{
								m_BackgroundColor = XUtil::GetColourFromString(vals[k],RGB(0,0,0));
							}
							delete vals[k];
						}
						m_FBackground |= Background;
					}
				}
				if(::_tcscmp(_T("margin"),cssStyles[i])==0)
				{
					vector<LPTSTR> vals;
					XUtil::GetTokens(cssValue[i],_T(" "),vals);  
					if((int)vals.size() > 0)
					{
						m_FMargin |= MarginLeft|MarginRight|MarginTop|MarginBottom;
						if((int)vals.size()==1)
						{
							int len = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_MarginWidths[0] = len;m_MarginWidths[1] = len;
							m_MarginWidths[2] = len;m_MarginWidths[3] = len;
						}else if((int)vals.size()==2)
						{
							int len;
							len = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_MarginWidths[1] = len;m_MarginWidths[3] = len;
							len = XUtil::CalcCssStrToNumber(vals[1], 10 );
							m_MarginWidths[0] = len;m_MarginWidths[2] = len;
						}
						else if((int)vals.size()==3)
						{
							m_MarginWidths[1] = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_MarginWidths[2] = XUtil::CalcCssStrToNumber(vals[1], 10 );
							m_MarginWidths[3] = XUtil::CalcCssStrToNumber(vals[2], 10 );
							m_MarginWidths[0] = XUtil::CalcCssStrToNumber(vals[1], 10 );
						}
						else if((int)vals.size()==4)
						{
							m_MarginWidths[1] = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_MarginWidths[2] = XUtil::CalcCssStrToNumber(vals[1], 10 );
							m_MarginWidths[3] = XUtil::CalcCssStrToNumber(vals[2], 10 );
							m_MarginWidths[0] = XUtil::CalcCssStrToNumber(vals[3], 10 );
						}
						for(int k= 0;k<(int)vals.size();k++)
						{
							delete vals[k];
						}
					}
				}
				if(::_tcscmp(_T("padding"),cssStyles[i])==0)
				{
					vector<LPTSTR> vals;
					XUtil::GetTokens(cssValue[i],_T(" "),vals);  
					if((int)vals.size() > 0)
					{
						m_FPadding |= PaddingLeft|PaddingRight|PaddingTop|PaddingBottom;
						if((int)vals.size()==1)
						{
							int len = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_PaddingWidths[0] = len;m_PaddingWidths[1] = len;
							m_PaddingWidths[2] = len;m_PaddingWidths[3] = len;
						}else if((int)vals.size()==2)
						{
							int len;
							len = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_PaddingWidths[1] = len;m_PaddingWidths[3] = len;
							len = XUtil::CalcCssStrToNumber(vals[1], 10 );
							m_PaddingWidths[0] = len;m_PaddingWidths[2] = len;
						}
						else if((int)vals.size()==4)
						{
							m_PaddingWidths[1] = XUtil::CalcCssStrToNumber(vals[0], 10 );
							m_PaddingWidths[2] = XUtil::CalcCssStrToNumber(vals[1], 10 );
							m_PaddingWidths[3] = XUtil::CalcCssStrToNumber(vals[2], 10 );
							m_PaddingWidths[0] = XUtil::CalcCssStrToNumber(vals[3], 10 );
						}
						for(int k= 0;k<(int)vals.size();k++)
						{
							delete vals[k];
						}
					}
				}
				if(::_tcscmp(_T("border"),cssStyles[i])==0)
				{
					vector<LPTSTR> vals;
					XUtil::GetTokens(cssValue[i],_T(","),vals);  
					if((int)vals.size() > 0)
					{
						m_FBorder |= BorderLeft|BorderRight|BorderTop|BorderBottom;
						if((int)vals.size()==1)
						{
							vector<LPTSTR> valsex;
							XUtil::GetTokens(vals[0],_T(" "),valsex);
							for(int k=0;k<(int)valsex.size();k++)
							{
								::_tcslwr_s(valsex[k],_tcslen(valsex[k]) + 1);
								//border type
								if(::_tcsstr(valsex[k],_T("none")) ||::_tcsstr(valsex[k],_T("solid")))
								{
									if(::_tcsstr(valsex[k],_T("solid")))
										m_BorderType[0] = m_BorderType[1]=m_BorderType[2]=m_BorderType[3]=1;	
								}else if(valsex[k][0]=='#')
								{
									COLORREF color = XUtil::GetColourFromString(valsex[k],RGB(0,0,0));
									m_BorderColors[0] = m_BorderColors[1] = m_BorderColors[2] = m_BorderColors[3] =color;
								}else
								{
									int bwidth = XUtil::CalcCssStrToNumber(valsex[k], 10 );
									m_BorderWidths[0] = m_BorderWidths[1] = m_BorderWidths[2] =m_BorderWidths[3] = bwidth;
								}
								delete valsex[k];
							}
						}else if((int)vals.size()==2)
						{
							for(int m=0;m<(int)vals.size();m++)
							{
								vector<LPTSTR> valsex;
								XUtil::GetTokens(vals[m],_T(" "),valsex);
								for(int k=0;k<(int)valsex.size();k++)
								{
									::_tcslwr_s(valsex[k],_tcslen(valsex[k]) + 1);
									//border type
									if(::_tcsstr(valsex[k],_T("none")) ||::_tcsstr(valsex[k],_T("solid")))
									{
										if(::_tcsstr(valsex[k],_T("solid")))
										{
											if(m==0)
												m_BorderType[1] = m_BorderType[3] = 1;
											else
												m_BorderType[0] = m_BorderType[2] = 1;
										}
									}else if(valsex[k][0]=='#')
									{
										COLORREF color = XUtil::GetColourFromString(valsex[k],RGB(0,0,0));
										if(m==0)
											m_BorderColors[1] = m_BorderColors[3] = color;
										else
											m_BorderColors[0] = m_BorderColors[2] = color;
									}else
									{
										int bwidth = XUtil::CalcCssStrToNumber(valsex[k], 10 );
										if(m==0)
											m_BorderWidths[1] = m_BorderWidths[3] = bwidth;
										else
											m_BorderWidths[0] = m_BorderWidths[2] = bwidth;
									}
									delete valsex[k];
								}
							}
						}
						else if((int)vals.size()==4)
						{
							for(int m=0;m<(int)vals.size();m++)
							{
								vector<LPTSTR> valsex;
								XUtil::GetTokens(vals[m],_T(" "),valsex);
								for(int k=0;k<(int)valsex.size();k++)
								{
									::_tcslwr_s(valsex[k],_tcslen(valsex[k]) + 1);
									//border type
									if(::_tcsstr(valsex[k],_T("none")) ||::_tcsstr(valsex[k],_T("solid")))
									{
										if(::_tcsstr(valsex[k],_T("solid")))
										{
											if(m==0)m_BorderType[1] = 1;
											else if(m==2)m_BorderType[3] = 1;
											else if(m==1)m_BorderType[2] = 1;
											else m_BorderType[0] = 1;
										}
									}else if(valsex[k][0]=='#')
									{
										COLORREF color = XUtil::GetColourFromString(valsex[k],RGB(0,0,0));
										if(m==0)m_BorderColors[1] = color;
										else if(m==2)m_BorderColors[3] = color;
										else if(m==1)m_BorderColors[2] = color;
										else m_BorderColors[0] = color;
									}else
									{
										int bwidth = XUtil::CalcCssStrToNumber(valsex[k], 10 );
										if(m==0)m_BorderWidths[1] = bwidth;
										else if(m==2)m_BorderWidths[3] = bwidth;
										else if(m==1)m_BorderWidths[2] = bwidth;
										else m_BorderWidths[0] = bwidth;
									}
									delete valsex[k];
								}
							}
						}
						for(int k= 0;k<(int)vals.size();k++)
						{
							delete vals[k];
						}
					}
				}
			}
			return 1;
		}

		//set cssstyle string ,next parse cssstyle,adjust child node or brother node
		int XNode::SetCssStyle(LPCTSTR	cssStyle)
		{
			m_pCssStyle =::_tcsdup(cssStyle);
			ClearCssStyle();
			ParseCssStyle();
			return 1;
		}

		XSpliterBar* XNode::GetSpliterbar()
		{
			if(m_pSpliterBar) return m_pSpliterBar;
			return GetBrotherNode()? GetBrotherNode()->m_pSpliterBar:NULL;
		}

		int XNode::CalcContentRect()
		{
			RECT rect = m_AreaRect;
			if(m_FMargin & MarginLeft) rect.left += m_MarginWidths[0];
			if(m_FMargin & MarginRight) rect.right -= m_MarginWidths[2];
			if(m_FMargin & MarginTop) rect.top += m_MarginWidths[1];
			if(m_FMargin & MarginBottom) rect.bottom -= m_MarginWidths[3];

			if(m_FBorder & BorderLeft)rect.left += m_BorderWidths[0];
			if(m_FBorder & BorderRight)rect.right -= m_BorderWidths[2];
			if(m_FBorder & BorderTop)rect.top += m_BorderWidths[1];
			if(m_FBorder & BorderBottom)rect.bottom -= m_BorderWidths[3];

			if(m_FPadding & PaddingLeft) rect.left += m_PaddingWidths[0];
			if(m_FPadding & PaddingRight) rect.right -= m_PaddingWidths[2];
			if(m_FPadding & PaddingTop) rect.top += m_PaddingWidths[1];
			if(m_FPadding & PaddingBottom) rect.bottom -= m_PaddingWidths[3];

			m_ContentRect = rect;

			return 1;
		}

		bool XNode::HasChild()
		{
			if( this->m_pLeftNode) return true;
			if( this->m_pRightNode) return true;
			return false;
		}

		int XNode::ClearChild()
		{
			if(!this->m_pLeftNode) return 1;
			if(!this->m_pLeftNode->HasChild())
				delete this->m_pLeftNode;
			else
				if(this->m_pLeftNode) this->m_pLeftNode->ClearChild();
			if(!this->m_pRightNode) return 1;
			if(!this->m_pRightNode->HasChild())
				delete this->m_pRightNode;
			else
				if(this->m_pRightNode) this->m_pRightNode->ClearChild();

			if(m_pLayers) delete m_pLayers;

			return 1;
		}

		int XNode::ClearChild( bool bClearControl)
		{
			return ClearChild();
		}

		int XNode::AdjustRect()
		{
			RECT	rect;
			if(m_pParentNode)
				rect = m_pParentNode->m_ContentRect;
			else
				rect = m_AreaRect;
			return AdjustRect(rect);
		}

		//adjust content rect,adjust child node or brother node
		int XNode::AdjustRect(RECT rect)
		{
			int	spBarWidth = 0;
			if(GetSpliterbar())
				spBarWidth = GetSpliterbar()->GetBarWidth();
			XNode * pBrother = this->GetBrotherNode();
			if(pBrother)
			{
				DOUBLE	rate = 0;
				if(m_spRate > 0)
					rate = m_spRate;
				else if(pBrother->m_spRate)
					rate = 100 - pBrother->m_spRate;
				if(rate > 0)
				{
					if(m_spStatus & SP_VERT  && pBrother->m_spStatus & SP_VERT)
					{
						if(m_spStatus & SP_LEFT)
							rect.right = rect.left + (long)((rect.right - rect.left - spBarWidth )*rate/100);
						else
							rect.left = rect.right - (long)((rect.right - rect.left - spBarWidth )*rate/100);
					}else
					{
						if(m_spStatus & SP_LEFT)
							rect.bottom = rect.top + (long)((rect.bottom - rect.top - spBarWidth )*rate/100);
						else
							rect.top = rect.bottom - (long)((rect.bottom - rect.top - spBarWidth )*rate/100);
					}
				}else
				{
					if(m_spStatus & SP_VERT && pBrother->m_spStatus & SP_VERT)
					{
						int width = this->m_width;
						if(width==0) width = - pBrother->m_width;
						if(width==0) width = (rect.right - rect.left + 1 - spBarWidth)/2;
						if(width <0) width = rect.right - rect.left + 1 - spBarWidth + width;
						if(m_spStatus & SP_LEFT)
							rect.right = rect.left + width;
						else
							rect.left = rect.right - width;
					}else
					{
						int height = this->m_height;
						if(height==0) height = - pBrother->m_height;
						if(height==0) height = (rect.bottom - rect.top + 1 - spBarWidth)/2;
						if(height <0) height = rect.bottom - rect.top + 1 - spBarWidth + height;
						if(m_spStatus & SP_LEFT)
							rect.bottom = rect.top+ height;
						else
							rect.top = rect.bottom - height;
					}
				}
			}else
			{
				if(this->m_width)
					rect.right = rect.left + m_width;
				if(this->m_height)
					rect.bottom = rect.top + m_height;
				if(m_spRate)
				{
					if(m_spStatus & SP_VERT)
						rect.right = rect.left + (long)((rect.right - rect.left - spBarWidth )*m_spRate/100);
					else
						rect.bottom = rect.top + (long)((rect.bottom - rect.top - spBarWidth )*m_spRate/100);
				}
			}
			m_AreaRect = rect;
			this->CalcContentRect();
			if(this->m_pLeftNode)this->m_pLeftNode->AdjustRect();
			if(this->m_pRightNode)this->m_pRightNode->AdjustRect();
			//if(this->m_pLayers)this->m_pLayers->AdjustRect();
			return 1;
		}

		//adjust content rect,adjust child node or brother node
		int XNode::AdjustRect(int dx)
		{
			RECT rect = this->m_AreaRect;

			if(m_spStatus & SP_VERT)
			{
				if(m_spStatus & SP_LEFT)
				{
					rect.right = rect.right + dx;
					if(rect.right < rect.left + GetMinWidth()) rect.right = rect.left + GetMinWidth();
				}
				else
				{
					rect.left = rect.left + dx;
					if(rect.left > rect.right - GetMinWidth())rect.left = rect.right - GetMinWidth();
				}
			}else
			{
				if(m_spStatus & SP_LEFT)
				{
					rect.bottom = rect.bottom + dx;
					if(rect.bottom < rect.top + GetMinWidth()) rect.bottom = rect.top + GetMinWidth();
				}
				else
				{
					rect.top = rect.top + dx;
					if(rect.top > rect.bottom - GetMinWidth())rect.top = rect.bottom - GetMinWidth();
				}
			}
			this->m_spRate = 0;
			if(this->GetBrotherNode())this->GetBrotherNode()->m_spRate =0;

			m_AreaRect = rect;
			this->CalcContentRect();
			if(this->m_pLeftNode)this->m_pLeftNode->AdjustRect();
			if(this->m_pRightNode)this->m_pRightNode->AdjustRect();
			//if(this->m_pLayers)this->m_pLayers->AdjustRect();
			return 1;
		}

		int XNode::DoDraw(HDC hDC)
		{
			DrawNode(hDC);
			if(this->m_pLeftNode)this->m_pLeftNode->DoDraw(hDC);
			if(this->m_pRightNode)this->m_pRightNode->DoDraw(hDC);
			//if(this->m_pLayers) this->m_pLayers->DoDraw(hDC);
			return 1;
		}

		int XNode::DrawCtrlWin(XControl * pControl,HDC hDC)
		{
			//pControl->m_pNode->DrawNode(hDC);

			/*
			HDC	hdc = ::GetDC(pControl->m_hWnd);
			for(int i=0;i<(int)pControl->m_pChilds.size();i++)
			{
				DrawCtrlWin(pControl->m_pChilds[i],hdc);
			}
			::ReleaseDC(pControl->m_hWnd,hdc);
			*/
			return 1;
		}

		//draw 
		int XNode::DrawNode(HDC hDC)
		{
			HBRUSH	hbrush;
			RECT	brect;
			RECT	rect;
			rect = this->m_AreaRect;

			if(m_FMargin & MarginLeft) rect.left += m_MarginWidths[0];
			if(m_FMargin & MarginRight) rect.right -= m_MarginWidths[2];
			if(m_FMargin & MarginTop) rect.top += m_MarginWidths[1];
			if(m_FMargin & MarginBottom) rect.bottom -= m_MarginWidths[3];

			if(m_FBorder & BorderLeft)
			{
				brect = rect;
				brect.right = brect.left + m_BorderWidths[0] ;
				hbrush = ::CreateSolidBrush(m_BorderColors[0]);
				::FillRect(hDC,&brect,hbrush);
				::DeleteObject(hbrush);
				rect.left += m_BorderWidths[0];
			}
			if(m_FBorder & BorderRight)
			{
				brect = rect;
				brect.left = brect.right -  m_BorderWidths[2] ;
				hbrush = ::CreateSolidBrush(m_BorderColors[1]);
				::FillRect(hDC,&brect,hbrush);
				::DeleteObject(hbrush);
				rect.right -= m_BorderWidths[2];
			}

			if(m_FBorder & BorderTop)
			{
				brect = rect;
				brect.bottom = brect.top + m_BorderWidths[1];
				hbrush = ::CreateSolidBrush(m_BorderColors[2]);
				::FillRect(hDC,&brect,hbrush);
				::DeleteObject(hbrush);
				rect.top += m_BorderWidths[1];
			}

			if(m_FBorder & BorderBottom)
			{
				brect = rect;
				brect.top = brect.bottom - m_BorderWidths[3];
				hbrush = ::CreateSolidBrush(m_BorderColors[3]);
				::FillRect(hDC,&brect,hbrush);
				::DeleteObject(hbrush);
				rect.bottom -= m_BorderWidths[3];
			}

			if(m_FBackground &Background)
			{
				COLORREF color = m_BackgroundColor;
				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = rect.left;
				vertex[0].y = rect.top;
				vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 25:0)*256;
				vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 25:0)*256;
				vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 25:0)*256;

				vertex[1].x = rect.right;
				vertex[1].y = rect.bottom;
				vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
				vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
				vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;

				GradientFill(hDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
			}

			if(m_FPadding & PaddingLeft) rect.left += m_PaddingWidths[0];
			if(m_FPadding & PaddingRight) rect.right -= m_PaddingWidths[2];
			if(m_FPadding & PaddingTop) rect.top += m_PaddingWidths[1];
			if(m_FPadding & PaddingBottom) rect.bottom -= m_PaddingWidths[3];

			/*
			HPEN	hPen;
			hPen = ::CreatePen(0,2,RGB(192,192,192));
			hPen = (HPEN)::SelectObject(hDC,hPen);
			::MoveToEx(hDC,rect.left,rect.top,NULL);
			::LineTo(hDC,rect.right,rect.top);
			::LineTo(hDC,rect.right,rect.bottom);
			::LineTo(hDC,rect.left,rect.bottom);
			::LineTo(hDC,rect.left,rect.top);
			hPen = (HPEN)::SelectObject(hDC,hPen);
			::DeleteObject(hPen);*/

			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				DrawCtrlWin(m_pControlWins[i],hDC);
			}
			return 1;
		}

		//link object
		XNode * XNode::GetBrotherNode()
		{
			if(m_pParentNode)
			{
				if(m_pParentNode->m_pLeftNode == this)
					return m_pParentNode->m_pRightNode;
				else
					return m_pParentNode->m_pLeftNode;
			}else
				return NULL;
		}

		int XNode::ParseNode(IXMLDOMElementPtr pNodeEx)
		{
			m_pElement = pNodeEx;

			VARIANT	val;
			LPTSTR pCssStyle = NULL;
			pNodeEx->getAttribute(L"caption",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_pCaption)delete m_pCaption;
				m_pCaption = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"xscript",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_pXScript)delete m_pXScript;
				m_pXScript = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"xdll",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_pXDll)delete m_pCaption;
				m_pXDll = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"guid",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_pstrGuid)delete m_pCaption;
				m_pstrGuid = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);

			pNodeEx->getAttribute(L"id",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_strCtlID) delete m_strCtlID;
				m_strCtlID = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			::VariantClear(&val);
			pNodeEx->getAttribute(L"name",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(m_strCtlName) delete m_strCtlName;
				m_strCtlName = XUtil::tcsdupFromBstr(val.bstrVal);
				
			}
			::VariantClear(&val);


			pNodeEx->getAttribute(L"style",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				m_pCssStyle = XUtil::tcsdupFromBstr(val.bstrVal);
				ClearCssStyle();
				ParseCssStyle();
			}
			::VariantClear(&val);

			this->m_spRate=0,this->m_width=0,this->m_height=0;
			pNodeEx->getAttribute(L"width",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				LPWSTR pNextStr;
				m_width = ::wcstol(val.bstrVal,&pNextStr,10);
				if(pNextStr && pNextStr[0]=='%')this->m_spRate = m_width,m_width = 0;
				this->m_spStatus |= SP_VERT;
				::VariantClear(&val);
			}else
			{
				pNodeEx->getAttribute(L"height",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
				{
					LPWSTR pNextStr;
					m_height = ::wcstol(val.bstrVal,&pNextStr,10);
					if(pNextStr && pNextStr[0]=='%')this->m_spRate = m_height,m_height = 0;
					this->m_spStatus &= ~SP_VERT;
					this->m_spStatus |= SP_HORZ;
					::VariantClear(&val);
				}
			}
			if(this->m_spStatus & SP_RIGHT)
			{
				if(this->m_spStatus & SP_HORZ)
				{
					if(this->GetBrotherNode())this->GetBrotherNode()->m_spStatus &= ~SP_VERT;
					if(this->GetBrotherNode())this->GetBrotherNode()->m_spStatus |= SP_HORZ;
				}else if(this->GetBrotherNode() && this->GetBrotherNode()->m_spStatus & SP_HORZ)
				{
					this->m_spStatus &= ~SP_VERT;
					this->m_spStatus |= SP_HORZ;
				}
			}
			pNodeEx->getAttribute(L"resize",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				::_wcslwr_s(val.bstrVal,wcslen(val.bstrVal) + 1);
				if(::wcscmp(val.bstrVal,L"yes")==0)
				{
					if(!GetSpliterbar())this->m_pSpliterBar = new XSpliterBar();
				}
			}
			::VariantClear(&val);
			if(GetSpliterbar())
			{
				if(this->m_spStatus&SP_LEFT)
					GetSpliterbar()->m_pLeftNode = this;
				else
				{
					GetSpliterbar()->m_pLeftNode = this->GetBrotherNode();
					GetSpliterbar()->m_pRightNode = this;
				}

				if(this->m_spStatus &SP_HORZ)
					GetSpliterbar()->m_bIsVertbar = false;
				else
					GetSpliterbar()->m_bIsVertbar = true;
			}

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

			pNodeEx->getAttribute(L"alignarrow",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				if(wcscmp(val.bstrVal,L"ns")==0)this->m_AlignArrow=AR_NS;
				else if(wcscmp(val.bstrVal,L"vert")==0)this->m_AlignArrow=AR_NS;
				else if(wcscmp(val.bstrVal,L"we")==0)this->m_AlignArrow=AR_WE;
				else if(wcscmp(val.bstrVal,L"horz")==0)this->m_AlignArrow=AR_WE;
				else if(wcscmp(val.bstrVal,L"abs")==0)this->m_AlignArrow=AR_ABS;
			}
			::VariantClear(&val);

			IXMLDOMNodeListPtr	pList;
			IXMLDOMNodePtr	pNodeLeft;
			IXMLDOMNodePtr	pNodeRight;
			long	len;
			pNodeEx->selectNodes(L"layers",&pList);
			pList->get_length(&len);
			if(len >0)
			{
				if(m_pLayers) delete m_pLayers;
				m_pLayers = new XLayers();
				pList->get_item(0,&pNodeRight);
				m_pLayers->ParseNode(this,(IXMLDOMElementPtr)pNodeRight);
			}else
			{
				pNodeEx->selectNodes(L"box",&pList);
				pList->get_length(&len);
				if(len==1)
				{
					XNode * pChild = new XNode();
					pChild->m_pParentNode = this;
					this->m_pLeftNode = pChild;
					pList->get_item(0,&pNodeLeft);
					pChild->ParseNode(pNodeLeft);
				}else if(len==2)
				{
					XNode * pChildLeft = new XNode();
					pChildLeft->m_pParentNode = this;
					this->m_pLeftNode = pChildLeft;
					pChildLeft->m_spStatus |= SP_LEFT;
					pList->get_item(0,&pNodeLeft);
					pChildLeft->ParseNode(pNodeLeft);

					XNode * pChildRight = new XNode();
					pChildRight->m_pParentNode = this;
					this->m_pRightNode = pChildRight;
					pChildRight->m_spStatus &= ~SP_LEFT;
					pChildRight->m_spStatus |= SP_RIGHT;
					pList->get_item(1,&pNodeRight);
					pChildRight->ParseNode(pNodeRight);
				}
			}

			IXMLDOMNodeListPtr	pNodeToolList;
			IXMLDOMNodePtr	pNodeTool;
			pNodeEx->selectNodes(L"resource/ximagelist",&pNodeToolList);
			long toolen;

			VARIANT	sval;
			HBITMAP	hBmp;
			pNodeToolList->get_length(&toolen);
			for( int l=0;l<toolen;l++)
			{
				pNodeToolList->get_item(l,&pNodeTool);
				((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"src",&sval);
				if(sval.vt==VT_BSTR && sval.bstrVal)
				{
					hBmp = (HBITMAP)::LoadImageW(NULL,sval.bstrVal,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
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
					colorMask = XUtil::GetColourFromString(str,RGB(0,0,0));
				}
				::VariantClear(&sval);

				HIMAGELIST hImageList = ::ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,4,4);
				if(colorMask==0)
					::ImageList_Add(hImageList,hBmp,NULL);
				else
					::ImageList_AddMasked(hImageList,hBmp,colorMask);
				::DeleteObject(hBmp);
				GetImageList().push_back(hImageList);
			}

			return 1;
		}

		XSpliterBar* XNode::GetSpliterBarAtPointer(POINT pt)
		{
			XSpliterBar * pBar;
			if(this->GetSpliterbar() && this->GetSpliterbar()->IsHited(pt)) return this->GetSpliterbar();
			if(m_pLeftNode)
			{
				pBar = this->m_pLeftNode->GetSpliterBarAtPointer(pt);
				if(pBar && pBar->IsHited(pt)) return pBar;
			}
			if(m_pRightNode)
			{
				pBar = this->m_pRightNode->GetSpliterBarAtPointer(pt);
				if(pBar && pBar->IsHited(pt)) return pBar;
			}
			return NULL;
		}

		int XNode::CreateNodeControl(HWND	hWnd)
		{
			long	len;
			RECT	rc = this->m_ContentRect;
			IXMLDOMNodeList* pList;
			IXMLDOMNode*	pNode;
			IXMLDOMElement* pElement;
			IXMLDOMElement * pElementEx;
	
			if(this->m_pControlWins.size()>0) return 1;

			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				if(m_pControlWins[i])
				{
					::SetProp(m_pControlWins[i]->m_hWnd,_T("this"),NULL);
					delete m_pControlWins[i];
				}
			}
			m_pControlWins.clear();

			m_pElement->selectNodes(L"*",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				BSTR	bstrTag = NULL;
				pList->get_item(i,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				
				LPTSTR	szWinClass = NULL;
				LPTSTR	szXClass = NULL;
				LPTSTR  szCtrlID = NULL;

				pElement->get_tagName(&bstrTag);

				if(!bstrTag) continue;
				szXClass = XUtil::tcsdupFromBstr(bstrTag);
				::SysFreeString(bstrTag);
				if(::_tcscmp(szXClass,_T("box"))==0||::_tcscmp(szXClass,_T("layer"))==0)
				{	if(szXClass)delete szXClass;
					pElement->Release();
					continue;
				}

				VARIANT	val;
				pElement->getAttribute(L"id",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
					szCtrlID = XUtil::tcsdupFromBstr(val.bstrVal);
				::VariantClear(&val);

				szWinClass =XControlMgr::GetControlClass(szXClass);

				XControl * pControl = NULL;
				XWin *	pWin = this->GetRootNode()->m_pOwnerWin;
				if(pWin && pWin->m_hDLL && szCtrlID)
				{
					pElementEx = pElement;
					XControl *  (WINAPI * fnCreateObject)(XWin* pWin,LPTSTR pCtrlID,LPTSTR pStrClass,IXMLDOMElement *) = NULL;
					fnCreateObject = (XControl *  (WINAPI * )(XWin * pWin,LPTSTR pCtrlID,LPTSTR pStrClass,IXMLDOMElement *))::GetProcAddress(pWin->m_hDLL,"CreateObject");
					if(fnCreateObject)
						pControl = (XControl *)fnCreateObject(pWin,szCtrlID,szXClass,pElementEx);
					pElementEx->Release();
				}
				if(szCtrlID) delete szCtrlID;

				if(szWinClass || _tcscmp(_T("xspace"),szXClass)==0)
				{
					XNode *  pXNode = new XNode();
					pXNode->ParseNode(pElement);
					pXNode->m_pParentNode = this;
					
					DWORD	dwStyle = XControlMgr::GetControlDefStyle(szXClass);
					DWORD	dwStyleEx = XControlMgr::GetControlDefStyleEx(szXClass);

					RECT	rect;
					::SetRect(&rect,0,0,0,0);

					if(!pControl)
					{
						if(_tcscmp(szXClass,_T("xtoolbutton"))==0)
						{
							pControl = new XToolButton();
						}else if(_tcscmp(szXClass,_T("xlayersheet"))==0) 
						{
							pControl = new XLayerSheetCtrl();
							if(m_pParentNode && m_pParentNode->m_pParentNode && m_pParentNode->m_pParentNode->m_pLayers)
							{
								XLayers* pLayers = m_pParentNode->m_pParentNode->m_pLayers;
								((XLayerSheetCtrl *)pControl)->m_pLayer = pLayers;
								for(int i=0;i<(int)pLayers->pLayers->size();i++)
								{
									((XLayerSheetCtrl *)pControl)->AddSheet( (*(pLayers->pLayers))[i]->m_pCaption);
								}
								if(pLayers->m_bTopArrow==0)
								{
									((XLayerSheetCtrl *)pControl)->m_TabDrawStyle =XLayerSheetCtrl::tdsEclips;
									((XLayerSheetCtrl *)pControl)->m_TabArrow =XLayerSheetCtrl::taTop;
								}
								else
								{
									((XLayerSheetCtrl *)pControl)->m_TabDrawStyle =XLayerSheetCtrl::tdsShape;
									((XLayerSheetCtrl *)pControl)->m_TabArrow =XLayerSheetCtrl::taBottom;
								}
							}
							
						}else if(_tcscmp(szXClass,_T("xtree"))==0) 
						{
							pControl = new XTreeCtrl();
						}else if(_tcscmp(szXClass,_T("xword"))==0) 
						{
							pControl = new XWord();
						}else if(_tcscmp(szXClass,_T("xsedit"))==0) 
						{
							pControl = new XSEdit();
						}else if(_tcscmp(szXClass,_T("xwindesign"))==0) 
						{
							pControl = new XWinDesign();
						}else if(_tcscmp(szXClass,_T("xflowtable"))==0) 
						{
							pControl = new XFlowTable();
						}else if(_tcscmp(szXClass,_T("xpropsheet"))==0) 
						{
							pControl = new XPropSheetCtrl();
						}else if(_tcscmp(szXClass,_T("xcell"))==0)
						{
							pControl = new XCellCtrl();
						}else
							pControl = new XControl();
					}

					VARIANT	svalex;
					pElement->getAttribute(L"image",&svalex);
					if(svalex.vt==VT_BSTR && svalex.bstrVal)
					{
						LPTSTR	strValues;
						strValues = XUtil::wcsdupFromBstr(svalex.bstrVal);
						vector<LPTSTR> vals;
						XUtil::GetTokens(strValues,_T(","),vals);
						if(vals.size()>0)
						{
							LPTSTR pstrNext;
							if(vals.size() < 2)
								pControl->m_ImagePos = ::_tcstol(vals[0],&pstrNext,10);
							else
							{
								pControl->m_ImageIndex = ::_tcstol(vals[0],&pstrNext,10);
								pControl->m_ImagePos = ::_tcstol(vals[1],&pstrNext,10);
							}
						}
						if(strValues) delete strValues;
					}
					::VariantClear(&svalex);

					pControl->m_pNode = pXNode;
					pControl->m_pstrXClass = szXClass;
					if(szWinClass)pControl->CreateEx(dwStyleEx,dwStyle,szWinClass,pXNode->m_pCaption,rect,hWnd,0,XApp::GetXApp()->GetInstance());
					this->m_pControlWins.push_back(pControl);
					::SetProp(pControl->m_hWnd,_T("this"),pControl);
					if(_tcscmp(szXClass,_T("xmdiclient"))==0)
						((XMdiFrame *)(this->GetRootNode()->m_pOwnerWin))->m_hMdiClient = pControl->m_hWnd;

					//process xsedit
					if(_tcscmp(_T("xsedit"),szXClass)==0)
					{
						((XSEdit *)pControl)->Init();
						//((XSEdit *)pControl)->SetDisplayLinenumbers(TRUE);
						//((XSEdit *)pControl)->SetDisplayFolding(TRUE);
						//((XSEdit *)pControl)->SetLexer(SCLEX_CPP);

						VARIANT	svalex;
						pElement->getAttribute(L"src",&svalex);
						if(svalex.vt==VT_BSTR && svalex.bstrVal)
						{
							LPTSTR	pStr = XUtil::tcsdupFromBstr(svalex.bstrVal);
							((XSEdit *)pControl)->LoadFile(pStr);
							if(pStr)delete pStr;
						}
						::VariantClear(&svalex);
						/*((XSEdit *)pControl)->Init();
						((XSEdit *)pControl)->SetDisplayLinenumbers(TRUE);
						((XSEdit *)pControl)->SetDisplayFolding(TRUE);
						((XSEdit *)pControl)->SetLexer(SCLEX_CPP);
						 SendMessage(((XSEdit *)pControl)->m_hWnd,SCI_SETKEYWORDS, 0, (long)"and and_eq asm auto bitand bitor bool break case catch char class compl const const_cast continue default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new not not_eq operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq");
						((XSEdit *)pControl)->SetText("int i=0;\nwhile(i<10)\n{\ni++;\n}\n");*/
					}
					if(_tcscmp(_T("xhtml"),szXClass)==0)
					{
						LPTSTR str=_T("<body bgcolor=#ecec00><h4>Welcome</h4><p><b>QHTM</b> is an incredibly easy control to use.<br>It can be as simple as:<br></body><img src='earth.mng'><img src=\"D:\\qianxj\\vc\\XExplorer\\debug\\FOREST.JPG\"><a>this is test</a>");
						::SetWindowText(pControl->m_hWnd,str);
						/*VARIANT	svalex;
						pElement->getAttribute(L"src",&svalex);
						if(svalex.vt==VT_BSTR && svalex.bstrVal)
						{
							LPTSTR	pStr = XUtil::tcsdupFromBstr(svalex.bstrVal);
							QHTM_LoadFromFile(pControl->m_hWnd,pStr);
							if(pStr)delete pStr;
						}
						::VariantClear(&svalex);*/
						
					}
					//process tree
					if(_tcscmp(_T("xtree"),szXClass)==0)
					{
						VARIANT	svalex;
						pElement->getAttribute(L"imagelist",&svalex);
						if(svalex.vt==VT_BSTR && svalex.bstrVal)
						{
							WCHAR* pNextStr;
							int index = ::wcstol(svalex.bstrVal,&pNextStr,10);
							if(index >=0 && this->GetRootNode()->m_pOwerControl && index < (int)this->GetRootNode()->m_pOwerControl->m_hImageLists.size())
							{
								::SendMessage(pControl->m_hWnd, TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)this->GetRootNode()->m_pOwerControl->m_hImageLists[index]);
							}else if(index >=0 && index < (int)this->GetRootNode()->m_pOwnerWin->m_hImageLists.size())
							{
								::SendMessage(pControl->m_hWnd, TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)this->GetRootNode()->m_pOwnerWin->m_hImageLists[index]);
							}
						}
						::VariantClear(&svalex);
						((XTreeCtrl *)pControl)->MakeTree();
					}
					//process combox
				
					if(_tcscmp(_T("xcombobox"),szXClass)==0)
					{
						IXMLDOMNodeListPtr	pNodeToolList;
						IXMLDOMNodePtr	pNodeTool;
						pElement->selectNodes(L"xcomboitem",&pNodeToolList);
						long toolen;

						VARIANT	svalex;
						pNodeToolList->get_length(&toolen);
						for( int l=0;l<toolen;l++)
						{
							(pNodeToolList)->get_item(l,&pNodeTool);
							((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"text",&svalex);
							if(svalex.vt==VT_BSTR && svalex.bstrVal)
							{
								LPTSTR pText;
								pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
								::SendMessage(pControl->m_hWnd,CB_ADDSTRING,0,(LPARAM)pText);
								delete pText;
							}
							::VariantClear(&svalex);
						}

					}

					//process toolbar
					if(_tcscmp(_T("xtoolbar"),szXClass)==0)
					{
						IXMLDOMNodeListPtr	pNodeToolList;
						IXMLDOMNodePtr	pNodeTool;
						pElement->selectNodes(L"ximagelist",&pNodeToolList);
						long toolen;

						VARIANT	sval;
						HBITMAP	hBmp;
						pNodeToolList->get_length(&toolen);
						for( int l=0;l<toolen;l++)
						{
							pNodeToolList->get_item(l,&pNodeTool);
							((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"src",&sval);
							if(sval.vt==VT_BSTR && sval.bstrVal)
							{
								hBmp = (HBITMAP)::LoadImageW(NULL,sval.bstrVal,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
							}else
								hBmp = NULL;

							::VariantClear(&sval);
							HIMAGELIST hImageList = ::ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,4,4);
							::ImageList_AddMasked(hImageList,hBmp,RGB(255,0,0));
							::DeleteObject(hBmp);
							::SendMessage (pControl->m_hWnd,TB_SETIMAGELIST,l,(LPARAM)hImageList);
							GetImageList().push_back(hImageList);
						}

						int cx_edit = 0, CX_SEPARATOR = 250; // Width of edit control.
						int cy_edit = 20;                // Height of edit control.
						#define IDM_START	42000
						pElement->selectNodes(L"*[local-name()!='ximagelist']",&pNodeToolList);
						pNodeToolList->get_length(&toolen);

						TBBUTTON *  ptbButtons = NULL;
						if(toolen > 0)
						{
							ptbButtons = new TBBUTTON[toolen];
							::memset(ptbButtons,0,sizeof(TBBUTTON) * toolen);
						}
						for( int l=0;l<toolen;l++)
						{
							BSTR 	bstrValue;
							pNodeToolList->get_item(l,&pNodeTool);
							((IXMLDOMElementPtr)pNodeTool)->get_tagName(&bstrValue);


							ptbButtons[l].iBitmap = l;
							ptbButtons[l].fsState = TBSTATE_ENABLED;
							ptbButtons[l].fsStyle = BTNS_BUTTON;
							ptbButtons[l].idCommand = IDM_START + l;
							ptbButtons[l].iString = 0;
							ptbButtons[l].dwData = 0L;

							if(::wcscmp(L"xspace",bstrValue)==0)
								ptbButtons[l].fsStyle =BTNS_SEP; 
							else if(::wcscmp(L"xedit",bstrValue)==0)
							{
								((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"width",&sval);
								WCHAR* pNextStr;
								ptbButtons[l].iBitmap = ::wcstol(sval.bstrVal,&pNextStr,10);
								ptbButtons[l].fsStyle = BTNS_SEP;
								::VariantClear(&sval);
							}
							::SysFreeString(bstrValue);
						}
						// Add the view buttons. 
						SendMessage (pControl->m_hWnd, TB_BUTTONSTRUCTSIZE ,sizeof(TBBUTTON),0);
						if(toolen > 0)
							SendMessage (pControl->m_hWnd, TB_ADDBUTTONS, toolen/*ARRAYSIZE(tbButtons)*/, (LPARAM)ptbButtons);
						if(ptbButtons) delete ptbButtons;

						for( int l=0;l<toolen;l++)
						{
							BSTR 	bstrValue;
							RECT	rect;
							::SetRect(&rect,0,0,0,0);
							pNodeToolList->get_item(l,&pNodeTool);
							((IXMLDOMElementPtr)pNodeTool)->get_tagName(&bstrValue);
							if(::wcscmp(L"xbutton",bstrValue) !=0 && ::wcscmp(L"xspace",bstrValue) !=0)
							{
								LPTSTR strXClass = XUtil::tcsdupFromBstr(bstrValue);
								LPCTSTR strClass = XControlMgr::GetControlClass(strXClass);
								SendMessage(pControl->m_hWnd,TB_GETITEMRECT,l,(LPARAM) (LPRECT)&rect);
								if(strClass)
								{
									XNode *  pXNodeChild = new XNode();
									pXNodeChild->ParseNode((IXMLDOMElementPtr)pNodeTool);

									DWORD	dwStyle = XControlMgr::GetControlDefStyle(strXClass);
									DWORD	dwStyleEx = XControlMgr::GetControlDefStyleEx(strXClass);
									XControl * pChildControl = new XControl();
									pChildControl->m_pNode = pXNodeChild;
									pChildControl->m_pstrXClass = strXClass;
									pChildControl->m_pNode->m_AreaRect = rect;
									pChildControl->m_pNode->CalcContentRect();
									if(strClass)pChildControl->CreateEx(dwStyleEx,dwStyle,(LPTSTR)strClass,pXNode->m_pCaption,pChildControl->m_pNode->m_ContentRect,pControl->m_hWnd,0,XApp::GetXApp()->GetInstance());
									pControl->m_pChilds.push_back(pChildControl);
								}else
									if(strXClass) delete strXClass;
							}							
							::SysFreeString(bstrValue);
						}
					}
				}else
				{
					if(szXClass)delete szXClass;
				}
				pElement->Release();
			}
			pList->Release();

			if(this->m_pLeftNode)this->m_pLeftNode->CreateNodeControl(hWnd);
			if(this->m_pRightNode)this->m_pRightNode->CreateNodeControl(hWnd);
			if(this->m_pLayers)
			{
				for(int i=0;(int)i<(int)m_pLayers->pLayers->size();i++)
				{
					if(m_pLayers->nCurLayer ==i)
					{
						(*(m_pLayers->pCtrlWinCreated))[i]=true;
					}else if(!(*(m_pLayers->pCtrlWinCreated))[i])
					{
						(*(m_pLayers->pCtrlWinCreated))[i]=true;
						(*(m_pLayers->pLayers))[i]->CreateNodeControl(hWnd);
					}
				}
			}
			return 1;
		}

		int XNode::AdjustCtrlRect()
		{
			int cnt = (int) this->m_pControlWins.size();
			if(cnt > 0)
			{
				int*  pWidths = new int[cnt];
				int*  pHeights = new int[cnt];
				RECT	rc = this->m_ContentRect;
				int twidth, theight;
				int bwidth, bheight;
				int txcnt = 0,tycnt = 0;
				int startx=0,starty=0;

				twidth = rc.right - rc.left;
				theight = rc.bottom - rc.top;
				bwidth = twidth;
				bheight = theight;

				if(m_AlignArrow ==AR_NS)
				{
					for(int i=0;i<cnt;i++)
					{
						if(m_pControlWins[i]->m_pNode->m_width != 0 )
						{
							pWidths[i] = m_pControlWins[i]->m_pNode->m_width;
							bwidth -= pWidths[i];
							if(m_pControlWins[i]->m_pNode->m_pSpliterBar)bwidth -= m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
						}else if(m_pControlWins[i]->m_pNode->m_spRate)
						{
							pWidths[i] = (int)(twidth * m_pControlWins[i]->m_pNode->m_spRate /100);
							bwidth -= pWidths[i];
							if(m_pControlWins[i]->m_pNode->m_pSpliterBar)bwidth -= m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
						}else 
						{
							txcnt++;
						}
					}
					twidth = 0;
					for(int i=0;i<cnt;i++)
					{
						if(m_pControlWins[i]->m_pNode->m_width ==0)
						{
							if(bwidth > 0)pWidths[i] = bwidth /txcnt;
						}
						twidth += pWidths[i];
					}

					int rate = 1000;
					RECT	rect;
					if(twidth > rc.right - rc.left) rate = (rc.right - rc.left) * 1000 / twidth;
					startx = this->m_ContentRect.left;
					for(int i=0;i<cnt;i++)
					{
						rect = rc;
						rect.left = startx;
						rect.right = rect.left + pWidths[i] * rate /1000;
						m_pControlWins[i]->m_pNode->m_AreaRect = rect;
						m_pControlWins[i]->m_pNode->CalcContentRect();
						rect = m_pControlWins[i]->m_pNode->m_ContentRect;
						if(m_pControlWins[i]->m_hWnd)
							::MoveWindow(m_pControlWins[i]->m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top +1,true);
						startx = rect.right + 1;
						if(m_pControlWins[i]->m_pNode->m_pSpliterBar) startx += m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
					}
				}else if(m_AlignArrow ==AR_WE)
				{
					for(int i=0;i<cnt;i++)
					{
						if(m_pControlWins[i]->m_pNode->m_height != 0 )
						{
							pHeights[i] = m_pControlWins[i]->m_pNode->m_height;
							bheight -= pHeights[i];
							if(m_pControlWins[i]->m_pNode->m_pSpliterBar)bheight -= m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
						}else if(m_pControlWins[i]->m_pNode->m_spRate)
						{
							pHeights[i] = (int)(theight * m_pControlWins[i]->m_pNode->m_spRate /100);
							bheight -= pHeights[i];
							if(m_pControlWins[i]->m_pNode->m_pSpliterBar)bheight -= m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
						}else 
						{
							tycnt++;
						}
					}
					theight = 0;
					for(int i=0;i<cnt;i++)
					{
						if(m_pControlWins[i]->m_pNode->m_height ==0)
						{
							if(bheight > 0)pHeights[i] = bheight /tycnt;
						}
						theight += pHeights[i];
					}

					int rate = 1000;
					RECT	rect;
					if(theight > rc.bottom - rc.top) rate = (rc.bottom - rc.top) * 1000 / theight;
					starty = this->m_ContentRect.top;
					for(int i=0;i<cnt;i++)
					{
						rect = rc;
						rect.top = starty;
						rect.bottom = rect.top + pHeights[i] * rate /1000;
						m_pControlWins[i]->m_pNode->m_AreaRect = rect;
						m_pControlWins[i]->m_pNode->CalcContentRect();
						rect = m_pControlWins[i]->m_pNode->m_ContentRect;
						if(m_pControlWins[i]->m_hWnd)
							::MoveWindow(m_pControlWins[i]->m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top +1,true);
						starty = rect.bottom + 1;
						if(m_pControlWins[i]->m_pNode->m_pSpliterBar) starty += m_pControlWins[i]->m_pNode->m_pSpliterBar->GetBarWidth();
					}
				}
				if(pWidths)  delete pWidths;
				if(pHeights) delete pHeights;
			}

			if(this->m_pLeftNode)this->m_pLeftNode->AdjustCtrlRect();
			if(this->m_pRightNode)this->m_pRightNode->AdjustCtrlRect();
			return 1;
		}

		int XNode::HideCtrlWin()
		{
			for(int i=0; i<(int)m_pControlWins.size();i++)
			{
				if(m_pControlWins[i])
					::ShowWindow(m_pControlWins[i]->m_hWnd,SW_HIDE);
			}
			if(m_pLeftNode)m_pLeftNode->HideCtrlWin();
			if(m_pRightNode) m_pRightNode->HideCtrlWin();

			
			return 1;
		}
		int XNode::ShowCtrlWin()
		{
			for(int i=0; i<(int)m_pControlWins.size();i++)
			{
				if(m_pControlWins[i])
					::ShowWindow(m_pControlWins[i]->m_hWnd,SW_SHOW);
			}
			if(m_pLeftNode)m_pLeftNode->ShowCtrlWin();
			if(m_pRightNode) m_pRightNode->ShowCtrlWin();
		
			return 1;
		}

		XNode * XNode::GetRootNode()
		{
			XNode * pParentNode;
			pParentNode = this;
			while(pParentNode->m_pParentNode)pParentNode = pParentNode->m_pParentNode;
			return pParentNode;
		}
		XControl * XNode::GetControlByName(LPTSTR pStrName)
		{
			XControl* pControl;
			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				if(m_pControlWins[i]->m_pNode->m_strCtlName)
					if(_tcscmp(m_pControlWins[i]->m_pNode->m_strCtlName,pStrName)==0) return m_pControlWins[i];
			}
			if(this->m_pLeftNode)
			{
				pControl = this->m_pLeftNode->GetControlByName(pStrName);
				if(pControl) return pControl;
			}
			if(this->m_pRightNode)
			{
				pControl =  this->m_pRightNode->GetControlByName(pStrName);
				if(pControl) return pControl;
			}
			if(this->m_pLayers)
			{
				for(int i=0;i<(int)m_pLayers->pLayers->size();i++)
				{
					if(m_pLayers->nCurLayer != i)
					{
						pControl = (*(m_pLayers->pLayers))[i]->GetControlByName(pStrName);
						if(pControl) return pControl;
					}
				}
			}
			return NULL;
		}

		XNode * XNode::GetNodeByName(LPTSTR pStrName)
		{
			XNode* pNode;
			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				if(m_pControlWins[i]->m_pNode->m_strCtlName)
					if(_tcscmp(m_pControlWins[i]->m_pNode->m_strCtlName,pStrName)==0) return m_pControlWins[i]->m_pNode;
			}
			if(m_strCtlName && _tcscmp(m_strCtlName,pStrName)==0) return this;
			if(this->m_pLeftNode)
			{
				pNode = this->m_pLeftNode->GetNodeByName(pStrName);
				if(pNode) return pNode;
			}
			if(this->m_pRightNode)
			{
				pNode =  this->m_pRightNode->GetNodeByName(pStrName);
				if(pNode) return pNode;
			}
			if(this->m_pLayers)
			{
				for(int i=0;i<(int)m_pLayers->pLayers->size();i++)
				{
					if(m_pLayers->nCurLayer != i)
					{
						pNode = (*(m_pLayers->pLayers))[i]->GetNodeByName(pStrName);
						if(pNode) return pNode;
					}
				}
			}
			return NULL;
		}

		XNode * XNode::GetNodeByID(LPTSTR pStrID)
		{
			XNode* pNode;
			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				if(m_pControlWins[i]->m_pNode->m_strCtlID)
					if(_tcscmp(m_pControlWins[i]->m_pNode->m_strCtlID,pStrID)==0) return m_pControlWins[i]->m_pNode;
			}
			if(m_strCtlID && _tcscmp(m_strCtlID,pStrID)==0) return this;
			if(this->m_pLeftNode)
			{
				pNode = this->m_pLeftNode->GetNodeByID(pStrID);
				if(pNode) return pNode;
			}
			if(this->m_pRightNode)
			{
				pNode =  this->m_pRightNode->GetNodeByID(pStrID);
				if(pNode) return pNode;
			}
			if(this->m_pLayers)
			{
				for(int i=0;i<(int)m_pLayers->pLayers->size();i++)
				{
					if(m_pLayers->nCurLayer != i)
					{
						pNode = (*(m_pLayers->pLayers))[i]->GetNodeByID(pStrID);
						if(pNode) return pNode;
					}
				}
			}
			return NULL;
		}

		XControl * XNode::GetControlByID(LPTSTR pStrID)
		{
			XControl* pControl;
			for(int i=0;i<(int)this->m_pControlWins.size();i++)
			{
				if(m_pControlWins[i]->m_pNode->m_strCtlID)
					if(_tcscmp(m_pControlWins[i]->m_pNode->m_strCtlID,pStrID)==0) return m_pControlWins[i];
			}
			if(this->m_pLeftNode)
			{
				pControl = this->m_pLeftNode->GetControlByID(pStrID);
				if(pControl) return pControl;
			}
			if(this->m_pRightNode)
			{
				pControl =  this->m_pRightNode->GetControlByID(pStrID);
				if(pControl) return pControl;
			}
			if(this->m_pLayers)
			{
				for(int i=0;i<(int)m_pLayers->pLayers->size();i++)
				{
					if(m_pLayers->nCurLayer != i)
					{
						pControl = (*(m_pLayers->pLayers))[i]->GetControlByID(pStrID);
						if(pControl) return pControl;
					}
				}
			}
			return NULL;
		}
	}
}
