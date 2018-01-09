#pragma once
#include <xframe.h>
#include "xofbase.h"
#include "ihttableview.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		#define HTC_OBJECTCLICKED  0x01
		#define HTC_OBJECTRCLICKED 0x02
		#define HTC_OBJECTDBCLICKED  0x03
		struct 	HTOBJCLICKEDNMHDR : public NMHDR
		{
			void * pObject;
		};

		class XCONTROL_API xcSHtml:
			public xfControl
		{
		public:
			enum eHtBlockType{
				ehtBlock,ehtText,ehtSpan,ehtP,ehtBr,ehtDiv,ehtSpace,ehtA,ehtControl,ehtImage,ehtBody,ehtHtml,ehtTable
			};

			struct XCONTROL_API htBlock
			{
				LPTSTR pCssStyle;
				LPTSTR pCssClass;
				LPTSTR pStrID;
				LPTSTR pData;
				vector<htBlock *>* pChilds;
				htBlock * pParentBlock;
				eHtBlockType ht;
				XUtil::Css::TBoxStyle* pBoxStyle;
				int rowHeight;
				bool bVisible;
				htBlock():pChilds(NULL),pParentBlock(NULL),ht(ehtBlock),pCssClass(NULL),pBoxStyle(NULL),rowHeight(0),pStrID(NULL),pCssStyle(NULL),pData(NULL),bVisible(true)
				{
				}
				~htBlock()
				{
					//if(pCssStyle)delete pCssStyle;
					if(pCssClass)delete pCssClass;
					if(pBoxStyle)delete pBoxStyle;
					if(pStrID)delete pStrID;
					if(pData)delete pData;
					if(pChilds)
					{
						for(int i=0;i<(int)pChilds->size();i++)
						{
							if((*pChilds)[i])
							{
								delete (*pChilds)[i];
							}
						}
						delete pChilds;
					}
				}
			};
			struct XCONTROL_API htText : public htBlock
			{
				LPTSTR text;
				htText():text(NULL)
				{
					ht = ehtText;
				}
				~htText()
				{
					if(text)delete text;
				}
			};
			struct XCONTROL_API htSpan : public htBlock
			{
				htSpan()
				{
					ht = ehtSpan;
				}
			};
			struct XCONTROL_API htP : public htBlock
			{
				htP()
				{
					ht = ehtP;
				}
			};
			struct XCONTROL_API htBr : public htBlock
			{
				htBr()
				{
					ht = ehtBr;
				}
			};
			struct XCONTROL_API htDiv : public htBlock
			{
				htDiv()
				{
					ht = ehtDiv;
				}
			};
			struct XCONTROL_API htSpace : public htBlock
			{
				int width;
				int height;
				htSpace():width(0),height(0)
				{
					ht = ehtSpace;
				}
			};
			struct XCONTROL_API htA : public htBlock
			{
				LPTSTR pStrUrl;
				htA():pStrUrl(NULL)
				{
					ht = ehtA;
				}
				~htA()
				{
					if(pStrUrl)delete pStrUrl;
				}
			};
			struct XCONTROL_API htControl : public htBlock
			{
				LPTSTR pStrControlClass;
				Hxsoft::XFrame::xfControl * pControl;
				RECT FRect;
				IXMLDOMElement * pXmlData;
				htControl():pControl(NULL),pXmlData(NULL)
				{
					ht = ehtControl;
				}
				~htControl()
				{
					if(pStrControlClass)delete pStrControlClass;
					if(pControl)delete pControl;
					if(pXmlData)pXmlData->Release();
				}
			};
			struct XCONTROL_API htImage : public htBlock
			{
				LPTSTR pStrUrl;
				int width;
				int height;
				double rate;
				int alpha;

				htImage():pStrUrl(NULL),width(0),height(0),rate(0),alpha(0)
				{
					ht = ehtImage;
				}
				~htImage()
				{
					if(pStrUrl)delete pStrUrl;
				}
			};
			struct XCONTROL_API htBody : public htBlock
			{
				htBody()
				{
					ht = ehtBody;
				}
			};
			struct XCONTROL_API htHtml : public htBlock
			{
				htHtml()
				{
					ht = ehtHtml;
				}
			};		
			struct XCONTROL_API htTable : public htBlock
			{
				XControl::IHtTableView * pView;
				int FRow;
				int FColumn;
				htTable()
				{
					ht = ehtTable;
					pView = NULL;
					FRow = 0;
					FColumn = 0;
				}
				~htTable()
				{
					if(pView)
					{
						if(pView->GetTableData())delete pView->GetTableData();
						delete pView;
					}
				}
			};
		public:
			xcSHtml();
		public:
			~xcSHtml();
		public:
			htBlock* m_pBlock;
		public:
			htBlock* ParseXmlByUrl(LPTSTR pStrUrl);
			htBlock* ParseXml(LPTSTR pStrHtml);
			htBlock* ParseXml(IXMLDOMElement * pElement);
		public:
			virtual void Initial(); 
			virtual	void Initial(IXMLDOMElement * pElement);
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
			int DoDrawBlock(HDC hPaintDC, htBlock * pBlock, RECT &rt,POINT & ptOffset,int & rowHeight);
			int CalcOffset(HDC hPaintDC,htBlock * pBlock, RECT &rt,POINT & ptOffset,int & rowHeight);
		public:
			int LoadHtml(LPCTSTR pHtml);
			int AdjustExtent();
		public:
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		public:
			XUtil::Css::TBoxStyle* GetBlockCssStyle(htBlock * pBlock);
		public:
			//virtual int AdjustControlRect(RECT rect,bool redraw = true);
			int GetContentRect(RECT &rt, XUtil::Css::TBoxStyle * pStyle, RECT & contentRect);
		public:
			POINT m_TopLeft;
		public:
			XOffice::XOfficeStyleSvr * m_pStyleSvr;
		public:
			xbObject * GetInterface(); 
			SIZE GetContentExtent();
		public:
			void UpdateScrollPos();
			void ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos);
			int CalcScrollBar(int Value, int ScrollCode, int ScrollBar,  int Pos);
			int ScrollBarMin(int ScrollBar);
			int ScrollBarMax(int ScrollBar);
			int PageUp(int ScrollBar);
			int PageDown(int ScrollBar);
			POINT  CalcMaxTopLeft(POINT Coord,int ScrollBar);
		public:
			bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
			bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
			bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
			bool EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam);
			bool EvtRButtonUp(TEvent * pEvent, LPARAM lParam);
			int GetHitedObject(POINT pt);
		public:
			bool EvtMouseWheel(TEvent *pEvent ,LPARAM lParam);
			bool EvtVScroll(TEvent *pEvent ,LPARAM lParam);
			bool EvtHScroll(TEvent *pEvent ,LPARAM lParam);
			bool EvtSize(TEvent *pEvent ,LPARAM lParam);
		public:
			std::vector<RECT> m_ctlRect;
			std::vector<htBlock*> m_Blocks;
		};
	}
}