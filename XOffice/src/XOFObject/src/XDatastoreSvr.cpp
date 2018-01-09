#include "StdAfx.h"
#include "..\include\XDatastoreSvr.hpp"
#include "msxml2.h"
#include <comdef.h>
#include <time.h>
#include <atlbase.h>
#include "XDataView.hpp"
#include "xuUtil.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XDataStoreSvr::XDataStoreSvr(void):m_pDatastore(NULL),m_pTotalStr(NULL),m_pQueryStr(NULL),m_pRows(NULL)
	{
	}
	XDataStoreSvr::~XDataStoreSvr(void)
	{
		if(m_pDatastore)
		{
			m_pDatastore->Reset();
			delete m_pDatastore;
		}
		if(m_pTotalStr) delete m_pTotalStr;
		if(m_pQueryStr) delete m_pQueryStr;
		if(m_pRows) delete m_pRows;
	}

	//method for IContentCountSvr
	int XDataStoreSvr::GetRowCount()
	{
		if(!m_pDatastore) return 0;
		if(m_pRows) return m_pRows->size();
		return m_pDatastore-> rowOffset.size();
	}

	int XDataStoreSvr::GetColCount()
	{
		if(!m_pDatastore) return 0;
		return m_pDatastore->m_cols.size();
	}

	//end for IContentSvr

	//method for IContentObjectSvr
	void * XDataStoreSvr::GetItemData(int ACol,int ARow)
	{
		return 0;
	}

	bool  XDataStoreSvr::SetItemData(int ACol,int ARow,void * pData)
	{
		return false;
	}

	IContentSvr::eDataType XDataStoreSvr::GetItemType(int ACol,int ARow)
	{
		return IContentSvr::string;
	}

	bool XDataStoreSvr::SetItemType(int ACol,int ARow, IContentSvr::eDataType dt)
	{
		return false;
	}

	//method for IContentStringSvr
	LPCTSTR XDataStoreSvr::GetItemString(int ACol,int ARow)
	{
		if(!m_pDatastore) return L"";

		if(ARow < 1) return L"";

		//if(m_pRows && ARow > m_pRows->size()) return L"";
		//if(m_pRows) ARow = (*m_pRows)[ARow - 1];

		if(ARow < 1 || ARow > (int)m_pDatastore->rowOffset.size()) return L"";
		if(ACol < 1 || ACol > (int)m_pDatastore->m_cols.size()) return L"";
		unsigned char* pData = m_pDatastore->pDatas[m_pDatastore->rowOffsetSource[ARow - 1]];
		pData = pData + m_pDatastore->rowOffset[ARow - 1];
		ULONG offset = *(ULONG*)(pData + sizeof(ULONG)*(3 + ACol - 1));
		if(offset==0x1) return L""; //nill
		if(offset==0x3) return L""; //error
		offset >>= 2;
		wchar_t* Value = (wchar_t*)(pData + sizeof(ULONG)*(3 + GetColCount()) + offset);
		return Value;
	}

	LPCTSTR XDataStoreSvr::GetItemString(LPTSTR pColName,int ARow)
	{
		if(!m_pDatastore) return L"";
		if(m_pDatastore->mCols.find(pColName)==m_pDatastore->mCols.end()) return L"";
		return GetItemString(m_pDatastore->mCols[pColName],ARow);
	}

	bool XDataStoreSvr::SetItemString(int ACol,int ARow, LPTSTR pData)
	{
		return false;
	}
	bool XDataStoreSvr::SetItemString(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		return false;
	}
	
	bool XDataStoreSvr::SetItemStringEx(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		return false;
	}

	LPCTSTR XDataStoreSvr::GetItemDisplayString(int ACol,int ARow)
	{
		return GetItemString(ACol,ARow);
	}

	LPCTSTR XDataStoreSvr::GetItemDisplayString(LPTSTR pColName,int ARow)
	{
		if(!m_pDatastore) return L"";
		if(m_pDatastore->mCols.find((std::wstring)pColName+ L"/@_displaystring")!=m_pDatastore->mCols.end())
			return GetItemString(m_pDatastore->mCols[(LPTSTR)(pColName+(std::wstring)L"/@_displaystring").c_str()],ARow);
		return GetItemString(pColName,ARow);
	}

	bool XDataStoreSvr::SetItemDisplayString(int ACol,int ARow, LPTSTR pData)
	{
		return false;
	}

	bool XDataStoreSvr::SetItemDisplayString(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		return false;
	}

	bool  XDataStoreSvr::SetItemDisplayStringEx(LPTSTR pColName,int ARow, LPTSTR pData)
	{
		return false;
	}

	//end method for IStringContentSvr
	int XDataStoreSvr::InsertRow(int nRow)
	{
		return 1;
	}

	int XDataStoreSvr::DeleteRow(int nRow)
	{
		return 1;
	}

	//method for IContentDbSvr
	int XDataStoreSvr::Retrieve(LPTSTR pXmlStr )
	{
		return 1;
	}

	int XDataStoreSvr::Update()
	{
		return 1;
	}

	int XDataStoreSvr::GetContent(BSTR &bstr)
	{
		return 1;
	}

	int XDataStoreSvr::GetContent(BSTR &bstr,LPTSTR pGuid)
	{
		return 1;
	}

	LPTSTR XDataStoreSvr::GetContent()
	{
		return NULL;
	}


	bool XDataStoreSvr::LoadData(unsigned char * pData)
	{
		if(m_pDatastore) m_pDatastore->Reset();
		if(!m_pDatastore) m_pDatastore = new TDatastore;

		return AppendData(pData);
	}

	bool XDataStoreSvr::AppendData(unsigned char * pData)
	{
		if(m_pTotalStr) delete m_pTotalStr;
		m_pTotalStr = NULL;

		m_pDatastore->pDatas.push_back(pData);
		int sourceIndex = m_pDatastore->pDatas.size() - 1;

		int pos = 0;
		//parse column info
		if(m_pDatastore->pDatas.size()==1)
		{

			if(m_pRows) delete m_pRows;
			if(m_pQueryStr) delete m_pQueryStr;
			m_pRows = NULL;
			m_pQueryStr = NULL;


			ULONG colDataSize = *(ULONG*)pData;
			ULONG colCount = *(((ULONG*)pData) +1);
			ULONG* colOffset = ((ULONG*)pData) + 2;
			unsigned char* colData = (unsigned char*)(colOffset + colCount);

			ULONG offset = 0;
			for(ULONG i=0; i<colCount; i++)
			{
				offset = colOffset[i];
				ULONG colDBType = *((ULONG *)(colData + offset));
				offset += 4;
				wchar_t* colName = (wchar_t *)(colData + offset);
				m_pDatastore->colsDBType.push_back((WORD)colDBType);
				m_pDatastore->mCols.insert(std::pair<std::wstring,int>(colName,i+1));
				m_pDatastore->m_cols.push_back(colName);
			}
			pos += colDataSize;
		}
		
		//parse row info
		unsigned char* pRowData = pData + pos;
		int rowDataSize = *((ULONG*)pRowData);
		while(rowDataSize)
		{
			m_pDatastore->rowOffset.push_back(pos);
			m_pDatastore->rowOffsetSource.push_back(sourceIndex);

			pos += rowDataSize;
			pRowData = pData + pos;
			rowDataSize = *((ULONG*)pRowData);
		}
		return true;		
	}

	bool XDataStoreSvr::Reset()
	{
		if(m_pDatastore)m_pDatastore->Reset();
		delete m_pDatastore;
		m_pDatastore = NULL;
		return true;
	}

	XDataStoreSvr * XDataStoreSvr::CreateInstance()
	{
		return new XDataStoreSvr;
	}


	LPCTSTR XDataStoreSvr::GetTotalItemString(LPCTSTR pTotalColumn,LPCTSTR pFormat)
	{
		if(!pTotalColumn) return NULL;

		if(wcsstr(pTotalColumn,L"bidata:"))
		{
			return this->GetBIData((wchar_t*)(wcschr(pTotalColumn,':') + 1));
		}
		else if(wcsstr(pTotalColumn,L"filter:"))
		{
			wchar_t buf[128];
			buf[0] = 0;
			wchar_t * pTerm = wcschr((wchar_t *)pTotalColumn,':')+1;
			if(pTerm[0]==',')
				ColumnFilter(NULL,NULL);
			else
			{
				int size = wcschr(pTotalColumn,',') - pTerm;
				wcsncpy(buf,pTerm,size);
				buf[size] = 0;
				ColumnFilter(buf,(wchar_t*)(wcschr(pTotalColumn,',') + 1));
			}
			return L"";
		}
		else if(wcsstr(pTotalColumn,L"sort:"))
		{
			Sort((wchar_t*)(wcschr(pTotalColumn,':') + 1));
			return L"";
		}
		else if(wcsstr(pTotalColumn,L"retrievedata:"))
		{
			unsigned char * pData = *(unsigned char **)(pTotalColumn + wcslen(pTotalColumn) + 1);
			AppendData(pData);
			return L"";
		}

		bool ksum = false;
		bool kavg = false;
		bool kcount = false;

		if(wcsstr(pTotalColumn,L"sum:")) ksum=true;
		else if(wcsstr(pTotalColumn,L"avg:")) kavg=true;
		else if(wcsstr(pTotalColumn,L"count:")) kcount=true;
		else return NULL;

		if(m_pTotalStr && m_pTotalStr->find(pTotalColumn)!=m_pTotalStr->end())
			return (*m_pTotalStr)[pTotalColumn].c_str();



		if(!GetRowCount()) return NULL;

		bool kfloat = false;
		double rval=0.0;
		int ival = 0;
		long len = GetRowCount();
		if(kcount)
		{
			ival = len;
		}else
		{
			pTotalColumn  = wcschr(pTotalColumn,':') + 1;
			for(int i=0;i<len;i++)
			{
				wchar_t* bstr = (wchar_t*)GetItemString((wchar_t*)pTotalColumn,GetContentRow(i+1));
				if(bstr)
				{
					bool bDelete = false;
					wchar_t* sval = bstr;
					//if(pFormat)
					//	sval = Hxsoft::XUtil::FormatDecimalString(sval,(LPTSTR)pFormat,bDelete);
					if(!kfloat && wcschr((const wchar_t*)sval,L'.'))
					{
						rval = ival;
						kfloat = true;
					}
					if(kfloat)
						rval += _wtof(sval);
					else
						ival += _wtoi(sval);
					if(bDelete) delete sval;
				}
			}
		}
		if(kavg && kfloat)
			rval = rval/len;
		else if(kavg && !kfloat)
		{
			kfloat = true;
			rval = ival*1.0/len;
		}
		wchar_t str[255];
		if(kfloat)
			swprintf_s(str,255,L"%f",rval);
		else
			swprintf_s(str,255,L"%d",ival);
		if(!m_pTotalStr) m_pTotalStr = new std::map<std::wstring,std::wstring>;
		m_pTotalStr->insert(std::pair<std::wstring,std::wstring>(pTotalColumn,str));
		return (*m_pTotalStr)[pTotalColumn].c_str();
	}


	LPCTSTR XDataStoreSvr::GetTotalItemString(LPCTSTR pTotalColumn)
	{
		return GetTotalItemString(pTotalColumn, NULL);
	}

	int XDataStoreSvr::Sort(wchar_t* sortString)
	{
		vector<wstring> m_pCols;
		vector<bool> m_pOrders;
		vector<wstring> m_pColTypes;

		wchar_t buf[255];
		int pos =0;

		while(true)
		{
			while(sortString[pos] && (sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' '))pos++;
			if(!sortString[pos])break;
			if(sortString[pos]==',')
			{
				pos++;
				continue;
			}

			int index = 0;
			while(sortString[pos] && !(sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' ') && index < 253)
			{
				buf[index] = sortString[pos];
				pos++;
				index++;
			}
			buf[index] =0;
			m_pCols.push_back(buf);

			while(sortString[pos] && (sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' '))pos++;
			if(sortString[pos]==',')
			{
				pos++;
				m_pOrders.push_back(true);
				m_pColTypes.push_back(L"xs:string");
				continue;
			}
			if(sortString[pos]=='a' || sortString[pos]=='A')
				m_pOrders.push_back(true);
			else
				m_pOrders.push_back(false);
			while(sortString[pos] && !(sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' ')) pos++;
			while(sortString[pos] && (sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' '))pos++;

			if(sortString[pos]==',')
			{
				pos++;
				m_pColTypes.push_back(L"xs:string");
				continue;
			}
			if(!sortString[pos]) 
			{
				m_pColTypes.push_back(L"xs:string");
				break;
			}
			index = 0;
			while(sortString[pos] && !(sortString[pos]=='\t' || sortString[pos]=='\r'||  sortString[pos]=='\n' || sortString[pos]==' ') && index < 253)
			{
				buf[index] = sortString[pos];
				pos++;
				index++;
			}
			buf[index]=0;
			m_pColTypes.push_back(buf);

			while(sortString[pos] && sortString[pos] !=',')pos++;
			if(!sortString[pos]) break;
			pos++;
		}
		return Sort(&m_pCols,&m_pOrders, &m_pColTypes,false);
	}

	int XDataStoreSvr::Filter()
	{
		if(!m_pDatastore) return -1;
		int nRowCount = GetRowCount();
		bool filterd = true;

		struct ColumnFilterItem
		{
			wchar_t* pColumn;
			int ColumnIndex;
			int DisplayColumnIndex;
			enum OP
			{
				ceq = 0,
				blank =1,
				noblank = 2,
				eq = 3,
				neq = 4,
				contains = 5,
				nocontains = 6,
				gt = 7,
				igt = 8,
				fgt = 9,
				gte = 10,
				igte = 11,
				fgte = 12,
				lt = 13,
				ilt = 14,
				flt = 15,
				lte = 16,
				ilte = 17,
				flte = 18,
			};
			int opCount;
			int bOr;
			int bComp;
			OP ops[4];
			wchar_t * filter[4];
			wchar_t * sVal[4];
			double  dVal[4];
			int iVal[4];
			wchar_t* format;
			ColumnFilterItem* next;
		};

		ColumnFilterItem* columnFilter = 0;
		ColumnFilterItem* columnCurrentFilter = 0;
		for(int i=0; i<(int)m_pQueryStr->size(); i++)
		{
			if(!columnCurrentFilter)
			{
				columnFilter = new ColumnFilterItem;
				columnCurrentFilter = columnFilter;
			}else
			{
				columnCurrentFilter->next = new ColumnFilterItem;
				columnCurrentFilter = columnCurrentFilter->next;
			}
			columnCurrentFilter->next = 0;
			columnCurrentFilter->opCount = 0;
			columnCurrentFilter->pColumn = (wchar_t*)(*m_pQueryStr)[i][0].c_str();
			columnCurrentFilter->ColumnIndex = -1;
			columnCurrentFilter->DisplayColumnIndex = -1;

			wchar_t* pColName = columnCurrentFilter->pColumn;
			if(m_pDatastore->mCols.find((std::wstring)pColName+ L"/@_displaystring")!=m_pDatastore->mCols.end())
			{
				columnCurrentFilter->DisplayColumnIndex = m_pDatastore->mCols[(std::wstring)pColName+ L"/@_displaystring"];
			}
			if(m_pDatastore->mCols.find(pColName) != m_pDatastore->mCols.end())
			{
				columnCurrentFilter->ColumnIndex =  m_pDatastore->mCols[(std::wstring)pColName];
				if(columnCurrentFilter->DisplayColumnIndex==-1)columnCurrentFilter->DisplayColumnIndex= columnCurrentFilter->ColumnIndex;
			}
			const wchar_t* pFilterStr = (*m_pQueryStr)[i][1].c_str();
			bool comp = false;
			if(pFilterStr[0]=='!' && pFilterStr[1]=='!' && pFilterStr[2]=='!')
			{
				comp = true;
				columnCurrentFilter->bComp = true;
				pFilterStr += 3;
				bool or = false;
				vector<LPTSTR> StrTokens; 
				if(pFilterStr[0]=='o' && pFilterStr[1]=='r')
				{
					or = true;
					pFilterStr += 3;
					XUtil::GetTokens( pFilterStr, L"!#!",StrTokens);
				}
				else if(pFilterStr[0]=='a' && pFilterStr[1]=='n' && pFilterStr[2]=='d')
				{
					or = false;
					pFilterStr += 4;
					XUtil::GetTokens( pFilterStr, L"!#!",StrTokens);
				}else
				{
					StrTokens.push_back((LPTSTR)pFilterStr);
				}

				columnCurrentFilter->bOr = or;
				columnCurrentFilter->opCount = (int)StrTokens.size();
				for(int k=0;k< (int)StrTokens.size();k++)
				{
					columnCurrentFilter->filter[k] = StrTokens[k];
					//eq
					if(StrTokens[k][0]=='e' && StrTokens[k][1]=='q') 
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::eq;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 3;
					}
					//neq
					else if(StrTokens[k][0]=='n' && StrTokens[k][1]=='e' && StrTokens[k][2]=='q') 
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::neq;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
					}
					//con
					else if(StrTokens[k][0]=='c' && StrTokens[k][1]=='o' && StrTokens[k][2]=='n')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::contains;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
					}
					//nocon
					else if(StrTokens[k][0]=='n' && StrTokens[k][1]=='o' && StrTokens[k][2]=='c' && StrTokens[k][3]=='o' && StrTokens[k][4]=='n')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::nocontains;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 6;
					}
					//gte
					if(StrTokens[k][0]=='g' && StrTokens[k][1]=='t' && StrTokens[k][2]=='e' )
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::gte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
					}
					//fgte
					else if(StrTokens[k][0]=='f' && StrTokens[k][1]=='g' && StrTokens[k][2]=='t'  && StrTokens[k][3]=='e')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::fgte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 5;
						columnCurrentFilter->dVal[k] = wcstod(columnCurrentFilter->sVal[k],0);
					}
					//igte
					else if(StrTokens[k][0]=='i' && StrTokens[k][1]=='g' && StrTokens[k][2]=='t'  && StrTokens[k][3]=='e')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::igte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 5;
						columnCurrentFilter->iVal[k] = wcstol(columnCurrentFilter->sVal[k],0,10);
					}
					//gt
					else if(StrTokens[k][0]=='g' && StrTokens[k][1]=='t' )
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::gt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 3;
					}
					//fgt
					else if(StrTokens[k][0]=='f' && StrTokens[k][1]=='g' && StrTokens[k][2]=='t')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::fgt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
						columnCurrentFilter->dVal[k] = wcstod(columnCurrentFilter->sVal[k],0);
					}
					//igt
					else if(StrTokens[k][0]=='i' && StrTokens[k][1]=='g' && StrTokens[k][2]=='t')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::igt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
						columnCurrentFilter->iVal[k] = wcstol(columnCurrentFilter->sVal[k],0,10);
					}
					//lte
					else if(StrTokens[k][0]=='l' && StrTokens[k][1]=='t' && StrTokens[k][2]=='e' )
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::lte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
					}
					//flte
					else if(StrTokens[k][0]=='f' && StrTokens[k][1]=='l' && StrTokens[k][2]=='t'  && StrTokens[k][3]=='e')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::flte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 5;
						columnCurrentFilter->dVal[k] = wcstod(columnCurrentFilter->sVal[k],0);
					}
					//ilte
					else if(StrTokens[k][0]=='i' && StrTokens[k][1]=='l' && StrTokens[k][2]=='t'  && StrTokens[k][3]=='e')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::ilte;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 5;
						columnCurrentFilter->iVal[k] = wcstol(columnCurrentFilter->sVal[k],0,10);
					}
					//lt
					else if(StrTokens[k][0]=='l' && StrTokens[k][1]=='t' )
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::lt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 3;
					}
					//flt
					else if(StrTokens[k][0]=='f' && StrTokens[k][1]=='l' && StrTokens[k][2]=='t')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::flt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
						columnCurrentFilter->dVal[k] = wcstod(columnCurrentFilter->sVal[k],0);
					}
					//ilt
					else if(StrTokens[k][0]=='i' && StrTokens[k][1]=='l' && StrTokens[k][2]=='t')
					{
						columnCurrentFilter->ops[k] = ColumnFilterItem::ilt;
						columnCurrentFilter->sVal[k] = columnCurrentFilter->filter[k] + 4;
						columnCurrentFilter->iVal[k] = wcstol(columnCurrentFilter->sVal[k],0,10);
					}


				}
			}
			else
			{
				columnCurrentFilter->bComp = false;
				columnCurrentFilter->opCount = 1;
				columnCurrentFilter->filter[0] = 0;
				if(wcscmp(pFilterStr,L"(¿Õ°×)")==0)
				{
					columnCurrentFilter->ops[0] = ColumnFilterItem::blank;
				}else if(wcscmp(pFilterStr,L"(·Ç¿Õ°×)")==0)
				{
					columnCurrentFilter->ops[0] = ColumnFilterItem::noblank;
				}else
				{
					columnCurrentFilter->ops[0] = ColumnFilterItem::ceq;
					columnCurrentFilter->sVal[0] = (wchar_t*)pFilterStr;
				}
			}
			//columnCurrentFilter->bComp = comp;
		}
		
		int rowCount = m_pDatastore-> rowOffset.size();
		vector<int>* pRows =  new vector<int>;
		for(int row=1;row<=rowCount;row++)
		{
			filterd = true;
			columnCurrentFilter = columnFilter;
			while(columnCurrentFilter)
			{
				if(columnCurrentFilter->DisplayColumnIndex==-1 && columnCurrentFilter->pColumn && columnCurrentFilter->pColumn[0]=='*'
					&& columnCurrentFilter->pColumn[1]==0)
				{
					if(columnCurrentFilter->sVal[0]  && columnCurrentFilter->sVal[0][0])
					{
						bool match = false;
						for(int i=0;i<(int)m_pDatastore->m_cols.size();i++)
						{
							const wchar_t* str = GetItemDisplayString(i+1,row);
							if(!str) continue;
							if(wcsstr(str,columnCurrentFilter->sVal[0]))
							{
								match = true;
								break;
							}
						}
						if(!match)
						{
							filterd = false;
							break;
						}
					}
				}
				if(columnCurrentFilter->DisplayColumnIndex==-1) 
				{	
					columnCurrentFilter = columnCurrentFilter->next;
					continue;
				}
				const wchar_t* val = GetItemDisplayString(columnCurrentFilter->DisplayColumnIndex,row);
				if(!columnCurrentFilter->bComp)
				{
					if(columnCurrentFilter->ops[0]== ColumnFilterItem::blank)
					{
						if(val && val[0]) filterd = false;
					}
					else if(columnCurrentFilter->ops[0]== ColumnFilterItem::noblank)
					{
						if(!(val && val[0])) filterd = false;
					}
					else if(columnCurrentFilter->ops[0]== ColumnFilterItem::ceq)
					{
						if(!val || !val[0] ||wcscmp(val,columnCurrentFilter->sVal[0])!=0) filterd = false;
					}
				}else
				{
					bool curfilterd = false;
					int k = 0;
					for(k=0;k<columnCurrentFilter->opCount;k++)
					{
						curfilterd = false;
						switch(columnCurrentFilter->ops[k])
						{
						case ColumnFilterItem::eq:
							if(val && wcscmp(val,columnCurrentFilter->sVal[k])==0) curfilterd = true;
							break;
						case ColumnFilterItem::neq:
							if(!val || wcscmp(val,columnCurrentFilter->sVal[k])!=0) curfilterd = true;
							break;
						case ColumnFilterItem::contains:
							if(val &&  wcsstr(val,columnCurrentFilter->sVal[k])) curfilterd = true;
							break;
						case ColumnFilterItem::nocontains:
							if(!val ||  !wcsstr(val,columnCurrentFilter->sVal[k])) curfilterd = true;
							break;
						case ColumnFilterItem::gt:
							if(val && wcscmp(val,columnCurrentFilter->sVal[k])>0) curfilterd = true;
							break;
						case ColumnFilterItem::fgt:
							if(val && wcstod(val,0) > columnCurrentFilter->dVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::igt:
							if(val && wcstol(val,0,10) > columnCurrentFilter->iVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::gte:
							if(val && wcscmp(val,columnCurrentFilter->sVal[k])>=0) curfilterd = true;
							break;
						case ColumnFilterItem::fgte:
							if(val && wcstod(val,0) >= columnCurrentFilter->dVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::igte:
							if(val && wcstol(val,0,10) >= columnCurrentFilter->iVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::lt:
							if(val && wcscmp(val,columnCurrentFilter->sVal[k])<0) curfilterd = true;
							break;
						case ColumnFilterItem::flt:
							if(val && wcstod(val,0) < columnCurrentFilter->dVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::ilt:
							if(val && wcstol(val,0,10) < columnCurrentFilter->iVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::lte:
							if(val && wcscmp(val,columnCurrentFilter->sVal[k])<=0) curfilterd = true;
							break;
						case ColumnFilterItem::flte:
							if(val && wcstod(val,0) <= columnCurrentFilter->dVal[k]) curfilterd = true;
							break;
						case ColumnFilterItem::ilte:
							if(val && wcstol(val,0,10) <= columnCurrentFilter->iVal[k]) curfilterd = true;
							break;
						}
						if(curfilterd && columnCurrentFilter->bOr)break;
						if(!curfilterd && !columnCurrentFilter->bOr)
						{
							filterd = false;
							break;
						}
					}
					if(columnCurrentFilter->bOr && k>=columnCurrentFilter->opCount)filterd = false;
				}
				if(!filterd) break;
				columnCurrentFilter = columnCurrentFilter->next;
			}
			if(filterd)pRows->push_back(row);
		}

		if(m_pRows)delete m_pRows;
		m_pRows = pRows;
		return 1;
	}

	int XDataStoreSvr::ColumnFilter(LPTSTR pColumn, LPTSTR pStr)
	{
		if(!m_pQueryStr)m_pQueryStr = new std::vector<std::vector<std::wstring> >();
		if(m_pTotalStr) m_pTotalStr->clear();

		if(!pColumn || !pColumn[0])
		{
			m_pQueryStr->clear();
			if(m_pRows) delete m_pRows;
			m_pRows = 0;
			return 1;
		}
		else
		{
			int i = 0;
			for(i=0; i<(int)m_pQueryStr->size(); i++)
			{
				std::wstring pText = (*m_pQueryStr)[i][0];
				if(pText.c_str() && pColumn && _tcscmp(pText.c_str(), pColumn) == 0) break;
			}

			if(i<(int)m_pQueryStr->size())
			{
				(*m_pQueryStr)[i].clear();
				(*m_pQueryStr).erase((*m_pQueryStr).begin() + i);
			}

			if(pStr && pStr[0]!='\0')
			{	
				if(wcscmp(pStr,L"(È«²¿)")!=0)
				{
					std::vector<std::wstring> * array1 = new std::vector<std::wstring>();
					array1->push_back(pColumn);
					array1->push_back(pStr);
					m_pQueryStr->push_back(*array1);
				}
			}
		}
		
		
		Filter();

		return 1;
	}

	int XDataStoreSvr::Sort(vector<wstring>* m_pCols,vector<bool>* m_pOrders,vector<std::wstring>* m_pColTypes, bool bMovePositon)
	{
		int nRowCount = GetRowCount();
		if(nRowCount < 1) return 0;
		
		int nSize = (int)m_pCols->size();
		if(nSize < 1) return 0;

		vector<int> vPos;
		vector<vector<LPCTSTR>*>Vals;
		for(int i=0;i<nSize;i++)
		{
			vector<LPCTSTR>* pItem = new vector<LPCTSTR>;
			Vals.push_back(pItem);
			for(int row=1;row <=nRowCount;row++)
			{
				LPCTSTR str = this->GetItemDisplayString((LPTSTR)(*m_pCols)[i].c_str(),GetContentRow(row));
				if(str)
					pItem->push_back(str);
				else
					pItem->push_back(this->GetItemString((LPTSTR)(*m_pCols)[i].c_str(),GetContentRow(row)));
				if(i==0)
				{
					vPos.push_back(GetContentRow(row));
				}
			}
		}
		
		FSortItem item;
		item.pOrders =  m_pOrders;
		item.pPos = &vPos;
		item.pVals = &Vals;
		item.pColTypes = m_pColTypes;

		QuickSort(item,nRowCount);

		if(!m_pRows) m_pRows = new std::vector<int>;
		m_pRows->clear();
		(*m_pRows) = vPos;

		return 1;
	}

	int XDataStoreSvr::GetContentRow(int nRow)
	{
		if(!m_pRows) return nRow;
		return (*m_pRows)[nRow - 1];
	}

	wchar_t* XDataStoreSvr::GetBIData(wchar_t* columns)
	{
		wchar_t buf[255];

		vector<wstring> pCols;
		int pos = 0;
		while(true)
		{
			while(columns[pos] && (columns[pos]=='\t' || columns[pos]=='\r'||  columns[pos]=='\n' || columns[pos]==' '))pos++;
			if(!columns[pos])break;
			if(columns[pos]==',')
			{
				pos++;
				continue;
			}
			int index = 0;
			while(columns[pos] && !(columns[pos]==',' ||columns[pos]=='\t' || columns[pos]=='\r'||  columns[pos]=='\n' || columns[pos]==' ') && index < 253)
			{
				buf[index] = columns[pos];
				pos++;
				index++;
			}
			buf[index] =0;
			pCols.push_back(buf);
		}
		std::wstring str;
		std::vector<std::wstring> items;
		int len = 0;
		for(int row = 1; row <= GetRowCount(); row++)
		{
			str =L"<Item>";
			for(int i=0;i<pCols.size();i++)
			{
				const wchar_t* pValue =  GetItemDisplayString((LPTSTR)pCols[i].c_str(),GetContentRow(row));
				wchar_t* pXmlStr = XUtil::ToXmlText(pValue);
				if(!pXmlStr) pXmlStr =  (wchar_t*)pValue;
				str = str + L"<" + pCols[i] + L">" + pXmlStr + L"</" + pCols[i]+L">";
				if(pXmlStr && pXmlStr !=  (wchar_t*)pValue) delete pXmlStr;
			}
			str +=L"</Item>";
			items.push_back(str);
			len += (int)str.length();
		}
		unsigned char* pData = new unsigned char[(len + 32)*sizeof(wchar_t)];
		pData[0] = 0;
		{
			int pos = 0;
			int len = wcslen(L"<data>")*sizeof(wchar_t);
			memcpy(pData + pos,L"<data>",len);
			pos+=len;
			int rowCount = items.size();
			for(int row = 1; row <= rowCount ; row++)
			{
				len = (int)items[row - 1].length()*sizeof(wchar_t);
				memcpy(pData + pos, items[row - 1].c_str(),len);
				pos+=len;
			}
			len = wcslen(L"</data>")*sizeof(wchar_t);
			memcpy(pData + pos,L"</data>",len);
			pos += len;
			pData[pos]=0;
			pData[pos+1]=0;
		}

		return (wchar_t*)pData;
	}

}}}
