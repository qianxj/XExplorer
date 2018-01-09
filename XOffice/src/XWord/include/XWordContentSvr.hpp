#pragma once

#include "xofbase.h"
#include "xuxutil.h"
#include "wpp4.h"

class XWordBlockMgr;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	#define LayerFlow_Span			0x00010000		//flow span
	#define LayerFlow_Block			0x00020000		//flow block
	#define LayerFlow_BlockEx		0x00040000		//flow blockex
	#define LayerFlow_Round			0x00080000		//flow round
	#define LayerFlow_NewsPaper		0x00100000		//flow newpaper
	#define LayerFlow_NewsPaper2	0x00200000		//flow newpaper2
	#define LayerFlow_NewsPaper3	0x00400000		//flow newpaper3

	enum emWordDataType
	{
		_XWordDataSpanControlBlock,_XWordDataSpanNullBlock,_XWordDataTextBlock,
		_XWordDataControlParagraph,_XWordDataParagraph,
		_XWordDataChapter,
		_XWordDataDocument
	};

	DWORD GetLayerFlow(BSTR pstrLayerFlow);
	 struct XWordDataBase
	 {
		int	m_nIndexStyle;
		XWordDataBase *	m_pDataParent;
		XUtil::xuVector<XWordDataBase * >* m_pChilds;
		int width,height;
		int x,y;
		emWordDataType eWordDataType;
		DWORD	align;
		bool m_bBreakPage;
		LPARAM param;

		XWordDataBase():m_nIndexStyle(-1),m_pDataParent(NULL),align(DT_LEFT|DT_TOP),
			m_pChilds(new XUtil::xuVector<XWordDataBase * >),width(-1),height(-1),x(-1),y(-1),m_bBreakPage(false)
		{
		};
		~XWordDataBase()
		{
			if(m_pChilds)delete m_pChilds;
		}
	 };

	 struct XWordDataControlBase
	 {
		 LPTSTR m_pStrControlClass;
		 DWORD dwFlowLayer;
		 bool m_bShape;
		 LPTSTR m_pStrShape;
		 bool bSelected;
		 void* m_pObj;
		 XWordDataControlBase():bSelected(false),m_pStrControlClass(NULL),dwFlowLayer(0),m_bShape(false),m_pStrShape(NULL),m_pObj(NULL){};
		 ~XWordDataControlBase()
		 {
			 if(m_pStrControlClass)delete m_pStrControlClass;
			 if(m_pStrShape)delete m_pStrShape;
		 }
	 };

	 struct XWordDataSpanBlock :public XWordDataBase
	 {
	 };

	 struct XWordDataSpanControlBlock :public XWordDataSpanBlock,public XWordDataControlBase
	 {
		 XWordDataSpanControlBlock(){eWordDataType = _XWordDataSpanControlBlock;}
	 };

	 struct XWordDataSpanNullBlock :public XWordDataSpanBlock
	 {
		 XWordDataSpanNullBlock(){eWordDataType = _XWordDataSpanNullBlock;}
	 };

	 struct XWordDataTextBlock :public XWordDataSpanBlock
	 {
		LPTSTR	m_pStrText;
		int nStrLimit;
		XWordDataTextBlock():m_pStrText(NULL),nStrLimit(0)
		{
			eWordDataType = _XWordDataTextBlock;
		}
		~XWordDataTextBlock()
		{
			if(m_pStrText) delete m_pStrText;
		}
	 };

	 //data Paragraph
 	 struct XWordDataContentParagraph:public XWordDataBase
	 {
	 };

 	 struct XWordDataControlParagraph:public XWordDataContentParagraph,public XWordDataControlBase
	 {
		 XWordDataControlParagraph(){eWordDataType = _XWordDataControlParagraph;}
	 };

	 struct XWordDataParagraph:public XWordDataContentParagraph
	 {
		 XWordDataParagraph(){eWordDataType = _XWordDataParagraph;}
	 };

	 //data chapter
	 struct XWordDataChapter:public XWordDataBase
	 {
		 XWordDataChapter(){eWordDataType = _XWordDataChapter;}
	 };

	 struct XWordDataDocument: public XWordDataBase
	 {
		 XWordDataDocument(){eWordDataType = _XWordDataDocument;}
	 };

	class XWordContentSvr :
		public CSvrbase
	{
	public:
		XWordContentSvr(void);
		~XWordContentSvr(void);
	public:
		XWordDataDocument* m_pXWordDataDocument;
	public:
		vord_t document; 
		vord_t data; 
	};
}}}}
