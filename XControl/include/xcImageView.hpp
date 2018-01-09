#pragma once
#include <xframe.h>
#include "xcImage.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		class xcImageEdit:public xfControl
		{
			public:
				virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
		};

		class XCONTROL_API xcImageView:
			public xfControl
		{
		protected:
			enum fstate{fedit, fborder, fonebig, none};

		public:
		struct XCONTROL_API imageBlock
		{
			vector<imageBlock *>* pChilds;
			LPTSTR pStrControlClass;
			Hxsoft::XFrame::xfControl * pControl;
			RECT FRect;
			IXMLDOMElement * pXmlData;

			imageBlock():pChilds(NULL),pControl(NULL),pXmlData(NULL)
			{
			}
			~imageBlock()
			{
				if(pStrControlClass)delete pStrControlClass;
				if(pControl)delete pControl;
				if(pXmlData)pXmlData->Release();
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

		public:	xcImageView();
				~xcImageView();
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * rc);
			virtual	void Initial();
			virtual	void Initial(IXMLDOMElement * pElement);
		public:
			virtual bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			virtual bool EvtMouseWheel(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtVScroll(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtHScroll(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
			virtual bool EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtDoubleClickedEx(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtTimer(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtKeyDown(TEvent *pEvent ,LPARAM lParam);
			virtual bool EvtChar(TEvent *pEvent ,LPARAM lParam);
		public:
			SIZE m_ItemSize;
			SIZE m_Labelsize;
			RECT m_marginRect;
			SIZE m_spliterSize;
			SIZE m_ItemSizeEx;
		public:
			POINT m_TopLeft;
		public:
			bool m_bPlay;
			int	 m_timer;
			int  m_nPlayType;
			int  m_nWait;
			int  m_nStep;
			int  m_offset;
		public:
			bool   m_bSizing;
			POINT  m_mousePt;
			POINT  m_mvstart;
			int    m_mIndex;
		public:
			bool  m_bOnlyOneShow;
		public:
			int m_Count;
			int GetCount();
			int SetCount(int count);

		public:
			int GetHNumberPerPages(RECT &rc);
			int GetVNumberPerPages(RECT &rc);

		public:
			bool GetItemRect(int nIndex,RECT &rc);
			int  GetItemAtPoint(int x,int y);
		public:
			int DrawSizingLine(int sx,int sy ,POINT pt);
		public:
			ULONG_PTR           gdiplusToken ;
		public:
			int TopLeftMoved(POINT pt);
		public:
			vector<wstring> m_ImageUrls;
			vector<void *>	m_pImages;
			vector<wstring> m_LabelText;
			int AddImages(LPCTSTR pUrl, LPTSTR pText);
			int RemoveImage(int nIndex);
			int InsertImages(LPCTSTR pUrl, LPTSTR pText, int nIndex);
		public:
			int Reset();
		public:
			bool LoadFromByte(byte * pData,int nLen,void * &pImage);
			bool LoadFromUrl(LPCTSTR pServerUrl,LPCTSTR pUrl,void * &pImage);
		public:
			xbObject * GetInterface(); 
		public:
			bool m_bSBoerder; //选中时是否显示边框
			bool m_bCanOne;    //双击时是否放大图片
			bool m_bEditLabel;    //编辑图片文字
			bool m_bCanEditLabel; 
			int m_bSelectedMode;
			bool m_bShowLast;
			fstate m_bState;
		public:
			xcImageEdit * m_pLabelEdit;
			int UpdateLabelText();
			int UpdateImageText();
		public:
			void UpdateScrollPos();
			void ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos);
		public:
			int Preview(int percent);
		public:
			XUtil::xuCsArray<INT32, BYTE> * m_pRowSelects;
			int SelectRow(int nRow,bool bSelect=true);
			int SelectRow(int sRow,int eRow, bool bSelect=true);
			bool IsRowSelected(int nRow);
			int GetNextSelectRow(int nStartRow);
		/*public:
			XUtil::xuCsArray<INT32, imageBlock *> * m_pImageXml;
			void AddImageXml(int nRow, IXMLDOMElement * pXml);	
			imageBlock * ParseXml(IXMLDOMElement * pElement);
			int DoDrawBlock(HDC hPaintDC,RECT rc,imageBlock * pItem);*/
		};
	}
}