#include "StdAfx.h"
#include "IXFUtil.hpp"
#include <commdlg.h>
#include "xuXUtil.h"
#include "FreeImage.h"
#include <Shlobj.h>

#pragma comment(lib,"FreeImage.lib")

namespace Hxsoft{ namespace XFrame
{
	IXFUtil::IXFUtil(void)
	{
	}

	IXFUtil::~IXFUtil(void)
	{
	}
	
	HCURSOR  IXFUtil::SetCursorWait()
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		return ::SetCursor(hCursor);
	}

	int IXFUtil::RestoreCursor(HCURSOR hCursor)
	{
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return true;
	}
	
	int IXFUtil::SaveToFile(LPTSTR pFile,LPTSTR pData,LPTSTR ext, LPTSTR pFilter, LPTSTR pFileName)
	{
		return Hxsoft::XUtil::SaveToFile(pFile, pData, ext, pFilter, pFileName);
		/*if(!pFile || pFile[0]==0)
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
		return -1;*/
	}

	LPWSTR IXFUtil::MD5(LPWSTR pStr)
	{
		return Hxsoft::XUtil::MD5(pStr);
	}

	bool IXFUtil::SmallImage(LPCTSTR lpszPathName, LPCTSTR lpszNewPathName, int width, int height, int flag)
	{
		bool ret = false;

		int _convertA = WideCharToMultiByte(CP_ACP, 0, lpszPathName, -1, NULL, 0, NULL, NULL);
		char* pszA = new char[_convertA];
		WideCharToMultiByte(CP_ACP, 0, lpszPathName, -1, pszA, _convertA, NULL, NULL);

		int _convertB = WideCharToMultiByte(CP_ACP, 0, lpszNewPathName, -1, NULL, 0, NULL, NULL);
		char* pszB = new char[_convertB];
		WideCharToMultiByte(CP_ACP, 0, lpszNewPathName, -1, pszB, _convertB, NULL, NULL);

		FreeImage_Initialise(TRUE);
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; 
		fif = FreeImage_GetFileType(pszA);
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(pszA); 
		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
		{
			FIBITMAP * jpg = FreeImage_Load(fif,pszA,flag);
			if(jpg)
			{
				FIBITMAP * jpg_new = FreeImage_Rescale(jpg, width, height, FILTER_BOX);
				if(jpg_new)
				{
					ret = FreeImage_Save(fif, jpg_new, pszB, flag);
					FreeImage_Unload(jpg_new);
				}
				FreeImage_Unload(jpg);
			}
		}
		FreeImage_DeInitialise();
		delete pszB;
		delete pszA;
		return ret;
	}

	LPWSTR IXFUtil::OpenDialog(HWND Owner,LPWSTR str)
	{
		BROWSEINFO info;
		memset(&info, 0, sizeof(info));
		info.hwndOwner = Owner;
		info.pidlRoot = NULL;
		TCHAR szDisplayName[MAX_PATH];
		ZeroMemory(szDisplayName,MAX_PATH);
		info.pszDisplayName = szDisplayName;
		info.lpszTitle = _T("请选择文件夹...");
		info.ulFlags = 0;
		info.lpfn = NULL;
		info.lParam = NULL;

		LPITEMIDLIST pidl = ::SHBrowseForFolder(&info);

		TCHAR szDir[MAX_PATH];
		ZeroMemory(szDir,MAX_PATH);
		if (pidl) 
		{
			if (::SHGetPathFromIDList(pidl, szDir)) 
			{
				wcsncpy_s(str,wcslen(str),szDir,wcslen(szDir));
				return wcsdup(szDir);
			}

		}
		str[0] = 0;
		return L"";
	}

	map<wstring,wstring>* IXFUtil::getCacheMap()
	{
		static map<wstring,wstring> * data = 0;
		if(!data) data = new map<wstring,wstring>;
		return data;
	}

	const wchar_t* IXFUtil::GetUrlData(LPWSTR url)
	{
		if(getCacheMap()->find(url)==getCacheMap()->end()) return L"";
		return (*getCacheMap())[url].c_str();
	}

	bool IXFUtil::HasCacheData(LPWSTR url)
	{
		return getCacheMap()->find(url)==getCacheMap()->end() ? false : true;
	}

	void IXFUtil::CacheUrlData(LPWSTR url,LPWSTR data)
	{
		if(getCacheMap()->find(url)==getCacheMap()->end()) 
			getCacheMap()->insert(pair<wstring,wstring>(url,data));
		else
			(*getCacheMap())[url] = data;
	}

	std::wstring IXFUtil::EnsureComboStyleClassName(std::wstring styleClass,std::map<std::wstring,std::wstring> &items)
	{
		std::wstring comboName;
		std::wstring itemName;
		std::vector<std::wstring> names;
		int pos = 0;
		while(styleClass.c_str()[pos])
		{
			wchar_t ch = styleClass.c_str()[pos];
			if(ch!=' ')
				itemName += ch;
			else
			{
				if(itemName != L"")
				{
					comboName += (comboName==L""?L"":L"_") + itemName ;
					names.push_back(itemName);
					itemName =L"";
				}
				itemName =L"";
			}
			pos++;
		}
		if(itemName != L"")
		{
			comboName += (comboName==L""?L"":L"_") + itemName ;
			names.push_back(itemName);
		}
		if(items.find(comboName)==items.end())
		{
			std::wstring str;
			for(int i=0;i<(int)names.size();i++)
			{
				if(items.find(names[i])!=items.end())
				{
					str += (i==0?L"":L"\r\n") + items[names[i]];
				}
			}
			items[comboName] =  str;
		}
		return comboName;
	}

	std::wstring IXFUtil::MakeComboStyleClassName(std::wstring styleClass)
	{
		std::wstring comboName;
		std::wstring itemName;
		int pos = 0;
		while(styleClass.c_str()[pos])
		{
			wchar_t ch = styleClass.c_str()[pos];
			if(ch!=' ')
				itemName += ch;
			else
			{
				if(itemName != L"") comboName += (comboName==L""?L"":L"_") + itemName ;
				itemName =L"";
			}
			pos++;
		}
		if(itemName != L"") comboName += (comboName==L""?L"":L"_") + itemName ;
		return comboName;
	}

}}
