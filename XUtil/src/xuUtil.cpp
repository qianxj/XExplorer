#include "stdafx.h"
#include "xuutil.hpp"
#include <commdlg.h>
namespace Hxsoft{ namespace XUtil
{
	static HINSTANCE  hZlibDLL = ::LoadLibrary(_T("zlibwapi.dll"));
	static int (WINAPI * fnCompress)(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen) = (int (WINAPI * )(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen))::GetProcAddress(hZlibDLL,"compress");
	static int (WINAPI * fnUnCompress)(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen) = (int (WINAPI * )(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen))::GetProcAddress(hZlibDLL,"uncompress");
	static ULONG (WINAPI * fnCompressBound)(ULONG) = (ULONG (WINAPI * )(ULONG))::GetProcAddress(hZlibDLL,"compressBound");

	LPSTR MD5(LPSTR str)
	{
		return MD5String(str); 
	}

	LPTSTR MD5(LPTSTR str)
	{
		int _convert = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* pszA = new char[_convert];
		int nRet = WideCharToMultiByte(CP_ACP, 0, str, -1, pszA, _convert, NULL, NULL);
		char * md5str = MD5String(pszA); 

		BSTR bstr;
		int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, md5str, (int)strlen(md5str), NULL, 0);
		bstr = SysAllocStringLen(NULL,cchWide + 1);
		bstr[cchWide]=0;
		::MultiByteToWideChar(CP_UTF8, 0, md5str, (int)strlen(md5str), bstr, cchWide + 1);

		return bstr;
	}
	int SaveToFile(LPTSTR pFile,LPTSTR pData,LPTSTR ext, LPTSTR pFilter, LPTSTR pFileName)
	{
		if(!pFile || pFile[0]==0)
		{
			TCHAR szFileName[255],szPath[255];
			int	 nFilterIndex = 1;
			ZeroMemory(szFileName,255);
			ZeroMemory(szPath,255);
			if(pFileName) lstrcpy(szPath, pFileName);

			OPENFILENAME ofn;
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.hInstance = NULL;
			ofn.lpstrFilter = pFilter;
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = nFilterIndex;
			ofn.lpstrFile = szPath;
			ofn.nMaxFile = 255;
			ofn.lpstrFileTitle = szFileName;
			ofn.nMaxFileTitle = 255;
			ofn.lpstrTitle = L"请指定文件名";
			ofn.lpstrDefExt = ext;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
			ofn.lCustData = 0;
			ofn.lpfnHook = NULL;
			ofn.lpTemplateName = NULL;

			if(::GetSaveFileName(&ofn))
				pFile = szPath;
			else
				return -1;
		}
		if(pFile)
		{
			HANDLE fp1;
			DWORD nWrite;
			fp1 = CreateFile(pFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

			BYTE p[2]={0xFF,0xFE};
			if(!(pData[0]==0xff &&  pData[1]==0xfe))
				::WriteFile(fp1,p,2,&nWrite,NULL);

			::WriteFile(fp1,pData,_tcslen(pData)*2,&nWrite,NULL);
			::CloseHandle(fp1);
			return 1;
		}
		return -1;
	}

	int Compress(BYTE *dest, ULONG *destLen,const BYTE *source, ULONG sourceLen)
	{
		if(!fnCompress) return -1;
		return fnCompress(dest,destLen,source,sourceLen);
	}
	int UnCompress (BYTE *dest,   ULONG *destLen,const BYTE *source, ULONG sourceLen)
	{
		if(!fnUnCompress) return -1;
		return fnUnCompress(dest,destLen,source,sourceLen);
	}
	ULONG CompressBound(ULONG nSrcLen)
	{
		if(!fnCompressBound) return -1;
		return fnCompressBound( nSrcLen);
	}

	LPTSTR UnXMLText(LPCTSTR str)
	{
		LPTSTR pStr = (LPTSTR)str;

		pStr = _tcschr(pStr,'&');
		bool bEntity = false;
		while(pStr)
		{
			if((pStr[1]=='l' || pStr[1]=='g') && pStr[2]=='t' && pStr[3]==';')
				bEntity = true;
			else if (pStr[1]=='a' && pStr[2]=='m' && pStr[3]=='p' && pStr[4]==';')
				bEntity = true;
			else if (pStr[1]=='a' && pStr[2]=='p' && pStr[3]=='o' && pStr[3]=='s' && pStr[4]==';' )
				bEntity = true;
			else if (pStr[1]=='q' && pStr[2]=='u' && pStr[3]=='o' && pStr[3]=='t' && pStr[3]==';')
				bEntity = true;

			if(bEntity)break;
			pStr = _tcschr(++pStr,'&');
		}

		pStr = new TCHAR[_tcslen(str) + 1];
		LPTSTR pStrEx = pStr;
		while(str[0])
		{
			if(str[0]=='&')
			{
				if(str[1]=='l' && str[2]=='t' && str[3]==';')
				{
					pStr[0]='<';
					str += 4;
				}else if(str[1]=='g' && str[2]=='t' && str[3]==';')
				{
					pStr[0]='>';
					str += 4;
				}else if(str[1]=='a' && str[2]=='m' &&  str[3]=='p' && str[4]==';')
				{
					pStr[0]='&';
					str += 5;
				}else if(str[1]=='a' && str[2]=='p' &&  str[3]=='o' && str[4]=='s' && str[5]==';')
				{
					pStr[0]='\'';
					str += 6;
				}else if(str[1]=='q' && str[2]=='u' &&  str[3]=='o' && str[4]=='t' && str[5]==';')
				{
					pStr[0]='"';
					str += 6;
				}else
					pStr++[0] = str++[0];
			}else
				pStr++[0] = str++[0];
		}
		pStr[0] ='\0';

		return pStrEx;
	}

	LPTSTR ToXmlText(LPCTSTR str)
	{
		if(!str) return NULL;

		int nCount = 0;
		int nEntity = 0;
		LPTSTR pStr = (LPTSTR)str;
		for(;pStr[0]!='\0';pStr++)
		{
			if(pStr[0]=='<'||pStr[0]=='>'||pStr[0]=='\''||pStr[0]=='"'||pStr[0]=='&' )
			{
				nEntity++;
			}
			nCount++;
		}
		if(nEntity==0) return NULL;

		pStr =  new TCHAR[nEntity*5 + nCount + 1];
		LPTSTR pStrEx = pStr ;
		
		int nPos = 0;
		for(;str[0]!='\0';str++)
		{
			if(str[0]=='&')
			{
				pStr[nPos++]='&';
				pStr[nPos++]='a';
				pStr[nPos++]='m';
				pStr[nPos++]='p';
				pStr[nPos++]=';';
			}
			else if(str[0]=='<')
			{
				pStr[nPos++]='&';
				pStr[nPos++]='l';
				pStr[nPos++]='t';
				pStr[nPos++]=';';
			}
			else if(str[0]=='>')
			{
				pStr[nPos++]='&';
				pStr[nPos++]='g';
				pStr[nPos++]='t';
				pStr[nPos++]=';';
			}
			else if(str[0]=='\'')
			{
				pStr[nPos++]='&';
				pStr[nPos++]='a';
				pStr[nPos++]='p';
				pStr[nPos++]='o';
				pStr[nPos++]='s';
				pStr[nPos++]=';';
			}
			else if(str[0]=='"')
			{
				pStr[nPos++]='&';
				pStr[nPos++]='q';
				pStr[nPos++]='u';
				pStr[nPos++]='o';
				pStr[nPos++]='t';
				pStr[nPos++]=';';
			}
			else
				pStr[nPos++] = str[0];
		}
		pStr[nPos]='\0';
		return pStrEx;
	}

	int CalcCssStrToNumber(LPTSTR cssStr,int Radix)
	{
		int value=0;
		TCHAR *endptr;
		value = ::_tcstol(cssStr,&endptr, 10 );
		if(!endptr ||endptr[0]=='\0') return value;
		if(endptr[0]=='p' && endptr[1]=='t')
		{
			value = ::MulDiv(value,96,72);
		}else if(endptr[0]=='m' && endptr[1]=='m')
		{
			value = (int)(value * 96.0 /25.4);
		}else if(endptr[0]=='c' && endptr[1]=='m')
		{
			value = (int)(value * 960 /25.4);
		}
		return value;
	}

	int GetTokens( LPCTSTR StrSource, LPTSTR spStr, vector<LPTSTR> & StrTokens)
	{
		int startIndex = (int)StrTokens.size();
		int pos=0;
		int spStrLen;
		LPTSTR	strPos;
		LPTSTR	pItem;
		if(!StrSource || StrSource[0]=='\0')return 0;
		spStrLen = ::lstrlen(spStr);
		if(spStrLen < 1) return 0;

		LPTSTR str = (LPTSTR)StrSource;
		//trim space
		while(str[0]==' '||str[0]=='\r'|| str[0]=='\n')str++;
		strPos = ::_tcsstr(str,spStr);
		if(!strPos) strPos = str + ::lstrlen(str);
		while(strPos)
		{
			pItem = new TCHAR[strPos - str + 1];
			::lstrcpyn(pItem,str,(int)(strPos - str + 1));
			StrTokens.push_back(pItem);
			if(strPos[0]=='\0') break;
			str = strPos + spStrLen;
			while(str[0]==' '||str[0]=='\r'|| str[0]=='\n')str++;
			if(str[0]=='\0') break;
			strPos = ::_tcsstr(str,spStr);
			if(!strPos)
				strPos = str + ::lstrlen(str);
		}

		return (int)StrTokens.size() - startIndex;
	}

	LPTSTR Round(wchar_t* str, int part, bool &bfree, bool full)
	{
		if(!str) return str;
		if(!wcschr(str,'.'))
		{
			if(!full) return str;
			if(part==0) return str;
			int len = wcslen(str);
			wchar_t* temp = new wchar_t[len+part+2];
			wcsncpy(temp,str,wcslen(str));
			temp[len+part+1] = 0;
			temp[len]='.';
			int pos = 0;
			while(pos < part)
			{
				temp[len+pos + 1]='0';
				pos++;
			}
			if(bfree) delete str;
			str = temp;
			return str;
		}
		//整数部分的长度
		int ilen = wcschr(str,'.') - str;
		//取小数部分
		wchar_t* spart = wcschr(str,'.') +1;
		int pos = 0;
		while(spart[pos] && pos < part) pos++;
		if(pos < part)
		{
			if(!full) return str;
			wchar_t* temp = new wchar_t[ilen+part+2];
			wcsncpy(temp,str,ilen+pos+1);
			temp[ilen+part+1] = 0;
			while(pos < part)
			{
				temp[ilen+pos + 1]='0';
				pos++;
			}
			if(bfree) delete str;
			str = temp;
			return str;
		}
		if(!spart[pos]) return str;
		if(spart[pos] <'5' && spart[pos] >='0')
		{
			if(!bfree)
			{
				wchar_t* temp = new wchar_t[ilen+pos+2];
				wcsncpy(temp,str,ilen+pos+1);
				str = temp;
				bfree = true;
			}
			str[ilen+1+part] = 0;
			if(part==0)
			{
				if(str[0]!='.')
				{
					str[ilen]=0;
					return str;
				}else
				{
					str[0]='0';
					str[1]=0;
				}
			}
			return str;
		}
		int p = ilen + pos;
		while(p>= 0 && (str[p]=='9'||str[p]=='.'))p--;
		if(p>=0 && !(p==0 && str[0]=='-'))
		{
			if(!bfree)
			{
				wchar_t* temp = new wchar_t[ilen+pos+2];
				wcsncpy(temp,str,ilen+pos+1);
				str = temp;
				bfree = true;
			}
			str[ilen+1+part] = 0;
			str[p++]++;
			while(p <= ilen + pos )
			{
				if(str[p]!='.')str[p]='0';
				p++;
			}
			if(part==0)
			{
				if(str[0]!='.')
				{
					str[ilen]=0;
					return str;
				}else
				{
					str[0]='0';
					str[1]=0;
				}
			}
			return str;
		}
		else
		{
			wchar_t* temp = new wchar_t[ilen+pos+3];
			wcsncpy(temp + 1,str,ilen+pos+1);
			if(bfree) delete str;
			bfree = true;
			str = temp;
			if(str[0]='-')
			{
				temp[0]='-';
				temp[1]='1';
				p = 2;
			}else
			{
				temp[0]='1';
				p=1;
			}
			
			while(p <= ilen + pos + 1  )
			{
				if(str[p]!='.')str[p]='0';
				p++;
			}
			ilen++;
			if(part==0)
			{
				if(str[0] !='.')
				{
					str[ilen]=0;
					return str;
				}else
				{
					str[0]='0';
					str[1]=0;
				}
			}
			return str;
		}
	}

	LPTSTR FormatDecimalString(LPTSTR  str, LPTSTR format, bool & bfree)  
	{
		if(!str) return str;
		if(!format || !format[0]) return str;

		wchar_t* pStr = wcschr(format,'.');
		if(!pStr) pStr = L"\0";

		pStr++;
		int len1 = 0;
		int len2 = 0;
		while(*pStr=='0')pStr++,len1++;
		while(*pStr=='#')pStr++,len2++;

		if(!wcschr(str,'.'))return Round(str,len1,bfree,true);

		//规范化
		if(str[0]=='.')
		{
			int nlen = wcslen(str);
			wchar_t* pTemp = new wchar_t[nlen + 2];
			pTemp[0]='0';
			wcsncpy_s(pTemp + 1,nlen + 1,str,nlen);
			if(bfree)delete str;
			bfree = true;
			str = pTemp;
		}

		wchar_t* spart = wcschr(str,'.') +1;
		int pos = 0;
		while(spart[pos]) pos++;
		while(pos>=0 && spart[pos - 1]=='0')pos--;
		if(pos>=len1+len2)return Round(str,len1+len2,bfree);
		if(pos<=len1) return Round(str,len1,bfree,true);
		return Round(str,pos,bfree);
	}
}}