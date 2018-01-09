#pragma once
#include "xofbase.h"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
#define PStrItem CContentSvr::LPContentItem
#define StrItem CContentSvr::TContentItem

	class XCELL_API CContentSvr :public CSvrbase
	{
	public:
		enum eSysColumn{key,status,note,count};
		enum eDataType{none,null,string,integer,decimal,datatime,time,date,boolean,image,object,xml,rtf,mail,report,xcell,pointer};
		
		typedef struct tagContentItem
		{
			LPTSTR pStrContent;
			LPTSTR pStrText;
			LPTSTR pStrFormula;
			LPTSTR pStrGroup;
			LPTSTR pStrColumn;
			LPTSTR pStrXml;
			LPTSTR pStrPreLast;
			LPTSTR pStrPre;
			LPVOID pData;
			eDataType eDTItem;
			LPVOID * pExtItem;
			LPTSTR pNote;
			LPTSTR pOrigin;
			LPTSTR pOriginStr;
			LPTSTR pStrAdvance;
			BOOL   bAdvance;
			BOOL   bDirect;

			tagContentItem(){::ZeroMemory(this,sizeof(tagContentItem));bAdvance=false;bDirect=false;}
			~tagContentItem(){
				if(pStrContent)delete pStrContent;
				if(pStrText)delete pStrText;
				if(pStrFormula) delete pStrFormula;
				if(pStrGroup) delete pStrGroup;
				if(pStrColumn) delete pStrColumn;
				if(pStrXml) delete pStrXml;
				if(pData) delete pData;
				if(pExtItem) delete pExtItem;
				if(pStrPreLast)delete pStrPreLast;
				if(pStrPre)delete pStrPre;
				if(pStrAdvance)delete pStrAdvance;
				if(pNote)delete pNote;
				if(pOrigin)delete pOrigin;
				if(pOriginStr)delete pOriginStr;
			}
			void CopyFrom(tagContentItem * pSrc)
			{
				if(pSrc->pStrContent)pStrContent = _tcsdup(pSrc->pStrContent);
				if(pSrc->pStrText)pStrText = _tcsdup(pSrc->pStrText);
				if(pSrc->pStrFormula) pStrFormula = _tcsdup(pSrc->pStrFormula);
				if(pSrc->pStrGroup) pStrGroup = _tcsdup(pSrc->pStrGroup);
				if(pSrc->pStrColumn)pStrColumn = _tcsdup(pSrc->pStrColumn);
				if(pSrc->pStrXml) pStrXml = _tcsdup(pSrc->pStrXml);
				if(pSrc->pStrPreLast)pStrPreLast = _tcsdup(pSrc->pStrPreLast);
				if(pSrc->pStrPre)pStrPre = _tcsdup(pSrc->pStrPre);
				if(pSrc->pStrAdvance)pStrAdvance = _tcsdup(pSrc->pStrAdvance); 
				if(pSrc->pNote)pNote = _tcsdup(pSrc->pNote); 
				if(pSrc->pOrigin)pOrigin = _tcsdup(pSrc->pOrigin); 
				if(pSrc->pOriginStr)pOriginStr = _tcsdup(pSrc->pOriginStr); 
				bAdvance = pSrc->bAdvance;
				bDirect  = pSrc->bDirect;
				eDTItem  = pSrc->eDTItem;
			}

		}TContentItem, * LPContentItem;


	public:
		CContentSvr(void);
		~CContentSvr(void);
	public:
		XUtil::xuSpList<XUtil::xuSpList<void *>*>*  m_pData;

		LPContentItem GetSysItemData(eSysColumn ASysCol,int ARow);
		int SetSysItemData(eSysColumn ASysCol,int ARow, LPContentItem pData);

		virtual LPContentItem GetItemData(int ACol,int ARow);
		virtual int  SetItemData(int ACol,int ARow,LPContentItem);
		virtual TCHAR * GetItemString(int ACol,int ARow);
		virtual int SetItemString(int ACol,int ARow, TCHAR * pData);
		
		int CopyTo(int FromRow,int ToRow);
		int MoveRow(int FromRow,int ToRow);
		int MoveRow(int FromRow,int ToRow,int Number);
		int MoveCol(int FromIndex,int ToIndex);
		
		void ClearRowData(int ARow);

		CContentSvr::eDataType GetItemType(int ACol,int ARow);
		int SetItemType(int ACol,int ARow, CContentSvr::eDataType dt);

		int GetValidRow();
		int GetValidCol(int ARow);
		int GetValidRowCol(int &ACol,int &ARow);
		int GetValidCol();
		//clear data and data type
		int Reset();
	
	public:
		//数据表格
		//vector<class CDwTable*> m_pDwTable;
		//bool AddTemplData(CContentSvr* m_pTemplData,bool bDestroyTemplData = false);
		//bool ResetTemplData();

	public:
		//CDwTable * GetDwTableFromCell(int ACol,int ARow);
	public:
		virtual TCHAR * GetItemText(int ACol,int ARow);
		virtual int SetItemText(int ACol,int ARow, TCHAR * pData);
		virtual int SetItemProp(int ACol,int ARow, LPCTSTR pAttribute, LPCTSTR pProp);
	};
}}}}
