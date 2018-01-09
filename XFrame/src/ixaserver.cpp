#include "StdAfx.h"
#include "IXAServer.hpp"
#include "xfapp.hpp"
#include "xfWin.hpp"
#include "xfuriloader.hpp"
#include "xfconnect.hpp"
#include <commdlg.h>
#include "IXAServerArg.hpp"
#include <Iphlpapi.h>
#include "xuXUtil.h"
#include "xfXpage.hpp"

namespace Hxsoft{ namespace XFrame
{
	int IXAServer::SetTrans(ITrans * pTrans)
	{
		m_pTrans = pTrans;
		return 1;
	}
	ITrans * IXAServer::GetTrans()
	{
		return m_pTrans;
	}

	//读取开发的对象,pObject should in( 'xpage','entity','sheet','workflow','worknode','xquery','xaction'
	int IXAServer::GetDevObject(LPCTSTR pStrServer,LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml)
	{
		if(!pStrServer || !pObject || !pObjGuid) return -9;
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/dev:%s%s"),pObject,pObjGuid);
		return xfUriLoader::FetchXml(pStrServer,buf,NULL,xml);
	}

	int IXAServer::GetDevObject(ITrans * pTrans,LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml)
	{
		if(!pTrans) return -1;
		return GetDevObject(pTrans->GetServerUrl(),pObject,pObjGuid,xml);
	}
	int IXAServer::GetDevObject(LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml)
	{
		if(!m_pTrans) return -1;
		return GetDevObject(m_pTrans->GetServerUrl(),pObject,pObjGuid,xml);
	}


	//执行服务命令
	LPTSTR IXAServer::ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr, bool needlogin)
	{
		BYTE * pData;
		int len;

		if(!needlogin)
		{
			if(!pStrServer || !pStrCmd ) return NULL;
			if(xfUriLoader::ProcessCmd(pStrServer,pStrCmd,pExtStr,pData,len) == 1)
			{
				LPTSTR Result = NULL;
				if(!pData) return Result;
				if(len > 1 && (pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
				{
					Result = new TCHAR[len - 1];
					Result = (LPTSTR)(pData + 2);
				}
				else
				{
					Result = new TCHAR[len + 1];
					Result = (LPTSTR)pData;
				}
				return Result;
			}
		}

		DWORD dwSize;
		TCHAR strHostname[INTERNET_MAX_HOST_NAME_LENGTH ];
		TCHAR strPort[INTERNET_MAX_PORT_NUMBER_LENGTH];
		dwSize = INTERNET_MAX_HOST_NAME_LENGTH;
		UrlGetPart(pStrServer,strHostname,&dwSize,URL_PART_HOSTNAME,URL_ESCAPE_UNSAFE);

		dwSize = INTERNET_MAX_PORT_NUMBER_LENGTH;
		if(UrlGetPart(pStrServer,strPort,&dwSize,URL_PART_PORT,0)!=S_OK)
		::_tcscpy_s(strPort,INTERNET_MAX_PORT_NUMBER_LENGTH,_T("80"));

		xfConnect * pConnect = new xfConnect();
		SYS_SOCKET sock = pConnect->CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(!pConnect->connectto(sock,strHostname,::_ttol(strPort)))
		{
			pConnect->CloseSocket(pConnect->m_socket);
			return NULL;
		}
		pConnect->m_socket = sock;
		GUID guid = xfApp::GetApp()->guid;

continuesendcommandex:
		pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr);
		int	nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);

		if(nRet < 0)
		{
			xfApp::GetApp()->guid = guid;
			return NULL;
		}
		if(nRet == 1)//need login
		{
			if(!xfApp::GetApp()->m_pUser) return NULL;

			TCHAR strLoginInfo[1024];
			strLoginInfo[0]='\0'; 

			IXAServerArg arg;
			arg.AddArg(L"username",xfApp::GetApp()->m_pUser);
			arg.AddArg(L"password",xfApp::GetApp()->m_pPassWord);
			arg.AddArg(L"computer",IXAServer::GetComputerNameEx());
			arg.AddArg(L"mac",IXAServer::GetMacAddress());
			arg.AddArg(L"net",L"xxxxx");
			arg.AddArg(L"ip",IXAServer::GetIpAddress());
			arg.AddArg(L"checkpass",L"N");
			::_stprintf_s(strLoginInfo,1024,_T("%s"),arg.GetString());

			pConnect->SendAuthCommand(pConnect->m_socket,strLoginInfo);
			nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);
			if(nRet <0)
			{
				xfApp::GetApp()->guid = guid;
				return NULL;
			}else
			{
				goto continuesendcommandex;
			}
		}
		xfApp::GetApp()->guid = guid;
		if(!pData) return NULL;
		LPTSTR Result;
		if(len > 1 && (pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
		{
			Result = new TCHAR[len - 1];
			Result = (LPTSTR)(pData + 2);
		}
		else
		{
			Result = new TCHAR[len + 1];
			Result = (LPTSTR)pData;
		}
		return Result;
	}
	int IXAServer::ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len)
	{
		if(!pStrServer || !pStrCmd ) return -9;
		return xfUriLoader::ProcessCmd(pStrServer,pStrCmd,pExtStr,pData,len);
	}
	
	int IXAServer::ProcessCmd(ITrans * pTrans,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len)
	{
		if(!pTrans) return -1;
		return ProcessCmd(pTrans->GetServerUrl(),pStrCmd,pExtStr,pData,len);
	}
	int IXAServer::ProcessCmd(LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len)
	{
		if(!m_pTrans) return -1;
		return ProcessCmd(m_pTrans->GetServerUrl(),pStrCmd,pExtStr,pData,len);
	}

	int  IXAServer::ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len, HWND hWnd)
	{
		if(!pStrServer || !pStrCmd ) return -9;
		return xfUriLoader::ProcessCmd(pStrServer,pStrCmd,pExtStr,pData,len,hWnd);
	}


	//执行服务命令并装载到xml对象中
	int IXAServer::FetchXml(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml)
	{
		if(!pStrServer || !pStrCmd ) return -9;
		if(_tcsstr(pStrCmd,_T("dev:"))==pStrCmd || _tcsstr(pStrCmd,_T("worknode:"))==pStrCmd ||
				_tcsstr(pStrCmd,_T("xquery:"))==pStrCmd  || _tcsstr(pStrCmd,_T("entity:"))==pStrCmd )
		{
			TCHAR buf[255];
			_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/%s"),pStrCmd);
			return xfUriLoader::FetchXml(pStrServer,buf,pExtStr,xml);
		}
		else
			return xfUriLoader::FetchXml(pStrServer,pStrCmd,pExtStr,xml);
	}

	int IXAServer::FetchXml(ITrans * pTrans,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml)
	{
		if(!pTrans) return -1;
		return FetchXml(pTrans->GetServerUrl(),pStrCmd,pExtStr,xml);
	}
	int IXAServer::FetchXml(LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml)
	{
		if(!m_pTrans) return -1;
		return FetchXml(m_pTrans->GetServerUrl(),pStrCmd,pExtStr,xml);
	}

	//执行服务命令并装载到xml对象中
	int IXAServer::LoadUrl(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache)
	{
		if(!pStrServer || !pUrl ) return -9;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;

		xfUriLoader::UrlCombineW(pStrServer,pUrl,strUrl,&dwSize,0);

		return xfUriLoader::Load(strUrl,NULL,pData,len,bCache);
	}

	int IXAServer::LoadUrl(ITrans * pTrans,LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache)
	{
		if(!pTrans) return -1;
		return LoadUrl(pTrans->GetServerUrl(),pUrl,pExtStr,pData,len,bCache);

	}
	int IXAServer::LoadUrl(LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache)
	{
		if(!m_pTrans) return -1;
		return LoadUrl(m_pTrans->GetServerUrl(),pUrl,pExtStr,pData,len,bCache);
	}


	//执行服务命令并装载到xml对象中
	int IXAServer::LoadUrl(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml)
	{
		LPBYTE pData=NULL;
		int nLen = 0;
		int nRet = LoadUrl(pStrServer,pUrl,pExtStr,pData,nLen,false);
		if( nRet < 0)
		{
			if(pData)xfUriLoader::FreeData(pData);
			return -1;
		}
		if(!pData ) return -1;
		if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
		{
			xml.LoadXml((LPTSTR)(pData + 2));
		}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
		{
			int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
			psz[_convert]='\0';
			xml.LoadXml(psz);
			delete psz;
		}
		else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
		{
			int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
			TCHAR * psz = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
			psz[_convert]='\0';
			xml.LoadXml(psz);
			delete psz;
		}else //should utf-16
		{
			xml.LoadXml((LPTSTR)pData);
		}
		if(pData)xfUriLoader::FreeData(pData);

		return nRet;
	}
	int IXAServer::LoadUrl(ITrans * pTrans,LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml)
	{
		if(!pTrans)return -1;
		return LoadUrl(pTrans->GetServerUrl(),pUrl,pExtStr,xml);
	}
	int IXAServer::LoadUrl(LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml)
	{
		if(!m_pTrans)return -1;
		return LoadUrl(m_pTrans->GetServerUrl(),pUrl,pExtStr,xml);
	}

	//读取XPage
	int IXAServer::LoadXPage(LPCTSTR pStrServer,LPCTSTR pStrUrl,xbXml &xml)
	{
		return LoadUrl(pStrServer,pStrUrl,NULL,xml);
	}
	
	int IXAServer::LoadXPage(ITrans * pTrans,LPCTSTR pStrUrl,xbXml &xml)
	{
		if(!pTrans)return -1;
		return LoadXPage(pTrans->GetServerUrl(),pStrUrl,xml);
	}
	int IXAServer::LoadXPage(LPCTSTR pStrUrl,xbXml &xml)
	{
		if(!m_pTrans)return -1;
		return LoadXPage(m_pTrans->GetServerUrl(),pStrUrl,xml);
	}
	int IXAServer::__DeleteFile(LPTSTR pName)
	{
		if(::PathFileExists(pName)) ::DeleteFile(pName);
		return 1;
	}
	int IXAServer::__CreateDirectory(LPTSTR pName)
	{
		if(!::PathFileExists(pName)) ::CreateDirectory(pName,NULL);
		return 1;
	}
	LPTSTR IXAServer::UploadFile(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource,BOOL bCompress)
	{
		TCHAR filter[]= _T("全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("全部文件(*.*)\0*.*\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = szCustomFilter;
		ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
		ofn.lpstrTitle = _T("文件选择");
		ofn.lpstrDefExt = _T("*");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		LPTSTR ret = L"";
		if(::GetOpenFileName(&ofn))
		{
			TCHAR buf[255];
			TCHAR* pToken = NULL;
			int len = 0;
			BYTE * pData = NULL;

			DWORD dwFileSize = 0;
			DWORD dwReadSize = 0;

			HANDLE hFile = ::CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			dwFileSize = ::GetFileSize(hFile,NULL);
			BYTE* lpBuffer = new BYTE[dwFileSize];
			ReadFile(hFile,lpBuffer,dwFileSize,&dwReadSize,NULL);

			_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/transfile %s %s %d"),pUri,pName,dwFileSize);
			if(xfUriLoader::ProcessCmd(NULL,buf,lpBuffer,dwFileSize,pData,len,false)==1 )
				ret = _tcsdup(szPath);
			delete lpBuffer;
			if(pData)xfUriLoader::FreeData(pData);
			CloseHandle(hFile);
		}else
			ret = L"-1";
		return ret;
	}

	int IXAServer::UploadData(LPTSTR pUri,LPTSTR pName,LPTSTR pStrData)
	{
		int len = 0;
		int ret = 0;
		TCHAR buf[255];
		BYTE * pData = NULL;

		if(!pStrData) return -1;
		int dwFileSize = ::wcslen(pStrData)*sizeof(wchar_t) + 2;
		BYTE* lpBuffer = new BYTE[dwFileSize];
		memcpy_s(lpBuffer+2,dwFileSize - 2, (BYTE*)pStrData ,dwFileSize - 2) ;
		lpBuffer[0]=0xFF;
		lpBuffer[1]=0xFE;

		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/transfile %s %s %d"),pUri,pName,dwFileSize);
		if(xfUriLoader::ProcessCmd(NULL,buf,lpBuffer,dwFileSize,pData,len,false)==1 ) ret =1;

		delete lpBuffer;
		if(pData)xfUriLoader::FreeData(pData);

		return ret;
	}

	int IXAServer::UploadEx(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource,BOOL bCompress)
	{
		TCHAR filter[]= _T("JPEG 文件(*.jpg)\0*.jpg;*.jpeg;*.png;*.gif;*.bmp\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("JPEG文件(*.jpg)\0*.jpg\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = szCustomFilter;
		ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
		ofn.lpstrTitle = _T("文件选择");
		ofn.lpstrDefExt = _T("jpeg");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		int ret = 0;
		if(::GetOpenFileName(&ofn))
		{
			TCHAR buf[255];
			TCHAR* pToken = NULL;
			int len = 0;
			BYTE * pData = NULL;

			DWORD dwFileSize = 0;
			DWORD dwReadSize = 0;

			HANDLE hFile = ::CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			dwFileSize = ::GetFileSize(hFile,NULL);
			BYTE* lpBuffer = new BYTE[dwFileSize];
			ReadFile(hFile,lpBuffer,dwFileSize,&dwReadSize,NULL);

			_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/transfile %s %s %d"),pUri,pName,dwFileSize);
			if(xfUriLoader::ProcessCmd(NULL,buf,lpBuffer,dwFileSize,pData,len,false)==1 ) ret =1;
			delete lpBuffer;
			if(pData)xfUriLoader::FreeData(pData);
			CloseHandle(hFile);
		}else
			ret = -1;
		return ret;
	}
	int IXAServer::Upload(LPTSTR pUri,LPTSTR pName,LPTSTR pPath,HWND m_hWnd,LPTSTR pSource,BOOL bCompress)
	{
		int ret = 0;

		TCHAR buf[255];
		TCHAR* pToken = NULL;
		int len;
		BYTE * pData = NULL;

		DWORD dwFileSize = 0;

		HANDLE hFile = ::CreateFile(pPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		dwFileSize = ::GetFileSize(hFile,NULL);
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/start %s %s %d"),pUri,pName,dwFileSize);
		if(xfUriLoader::ProcessCmd(NULL,buf,NULL,pData,len)==1 && pData)
		{
			pToken = (LPTSTR)pData;

			int nIndex = 0;
			DWORD dwBlockSize = 24*1024;
			DWORD dwReadSize = 0;
			BYTE* lpBuffer = new BYTE[dwBlockSize];
			while(true)
			{
				SetFilePointer(hFile,nIndex * dwBlockSize,NULL,FILE_BEGIN);
				ReadFile(hFile,lpBuffer,dwBlockSize,&dwReadSize,NULL);
				if(dwBlockSize < 1)break;
				_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/data %s %d %d"),pToken,nIndex, dwBlockSize);
				if(xfUriLoader::ProcessCmd(NULL,buf,lpBuffer,dwReadSize,pData,len,false)!=1)break;
				if(pData)xfUriLoader::FreeData(pData);
				pData = NULL;
				if(dwReadSize < dwBlockSize)break;
				nIndex++;
			}

			_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/finish %s"),pToken);
			if(xfUriLoader::ProcessCmd(NULL,buf,NULL,pData,len)==1)
			{
				if(pData)xfUriLoader::FreeData(pData);
				ret = 1;
			}
		}
		CloseHandle(hFile);
		if(pToken) xfUriLoader::FreeData((BYTE *)pToken);

		return ret;
	}
	int IXAServer::Upload(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource,BOOL bCompress)
	{
		TCHAR filter[]= _T("JPEG 文件(*.jpg)\0*.jpg;*.jpeg;*.png;*.gif;*.bmp\0全部文件(*.*)\0*.*\0");
		TCHAR szFileName[MAX_PATH],szPath[MAX_PATH];
		TCHAR szCustomFilter[MAX_PATH];
		int	 nFilterIndex = 1;

		ZeroMemory(szFileName,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szPath,MAX_PATH *sizeof(TCHAR));
		ZeroMemory(szCustomFilter,MAX_PATH *sizeof(TCHAR));
		lstrcpy(szCustomFilter,_T("JPEG文件(*.jpg)\0*.jpg\0"));

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = szCustomFilter;
		ofn.nMaxCustFilter = sizeof(szCustomFilter)/sizeof(szCustomFilter[0]);
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = sizeof(szPath)/sizeof(szPath[0]);
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = sizeof(szFileName)/sizeof(szFileName[0]);
		ofn.lpstrTitle = _T("文件选择");
		ofn.lpstrDefExt = _T("jpeg");
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		int ret = 0;
		if(::GetOpenFileName(&ofn))
		{
			TCHAR buf[255];
			TCHAR* pToken = NULL;
			int len;
			BYTE * pData = NULL;

			DWORD dwFileSize = 0;

			HANDLE hFile = ::CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			dwFileSize = ::GetFileSize(hFile,NULL);
			_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/start %s %s %d"),pUri,pName,dwFileSize);
			if(xfUriLoader::ProcessCmd(NULL,buf,NULL,pData,len)==1 && pData)
			{
				pToken = (LPTSTR)pData;

				int nIndex = 0;
				DWORD dwBlockSize = 24*1024;
				DWORD dwReadSize = 0;
				BYTE* lpBuffer = new BYTE[dwBlockSize];
				while(true)
				{
					SetFilePointer(hFile,nIndex * dwBlockSize,NULL,FILE_BEGIN);
					ReadFile(hFile,lpBuffer,dwBlockSize,&dwReadSize,NULL);
					if(dwBlockSize < 1)break;
					_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/data %s %d %d"),pToken,nIndex, dwBlockSize);
					if(xfUriLoader::ProcessCmd(NULL,buf,lpBuffer,dwReadSize,pData,len,false)!=1)break;
					if(pData)xfUriLoader::FreeData(pData);
					pData = NULL;
					if(dwReadSize < dwBlockSize)break;
					nIndex++;
				}

				_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/finish %s"),pToken);
				if(xfUriLoader::ProcessCmd(NULL,buf,NULL,pData,len)==1)
				{
					if(pData)xfUriLoader::FreeData(pData);
					ret = 1;
				}
			}
			CloseHandle(hFile);
			if(pToken) xfUriLoader::FreeData((BYTE *)pToken);
		}
		else
			ret = -1;
		return ret;
	}

	int IXAServer::FileCopy(LPTSTR pUri,LPTSTR pName,LPTSTR pUri1,LPTSTR pName1)
	{
		int ret = 0;
		int len;
		BYTE * pData = NULL;
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/entity/upload/copy %s %s %s %s"),pUri,pName,pUri1,pName1);
		if(xfUriLoader::ProcessCmd(NULL,buf,NULL,pData,len)==1)
		{
			ret = 1;
		}
		if(pData)xfUriLoader::FreeData(pData);
		return ret;
	}

	//执行XQuery命令
	LPCTSTR IXAServer::ExecArgs(LPCTSTR pArgs)
	{
		IXAServerArg pArg;
		int ret;
		if(!pArgs || _tcscmp(pArgs, _T("")) == 0)
			ret = pArg.SetXml(_T("<args/>"));
		else
			ret = pArg.SetXml((LPTSTR)pArgs);
		if(ret != 1) return pArgs;
		pArg.AddArg(_T("__userid"), GetUID());
		return _tcsdup(pArg.GetString());
	}

	int IXAServer::ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,int &val)
	{
		if(!pStrServer || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		
		TCHAR buf[255];
		BYTE * pData = NULL;
		int len;
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xquery:%s"),pObjGuid);
		if(xfUriLoader::ProcessCmd(pStrServer,buf,pArgsEx,pData,len)==1)
		{
			val = *(int *)pData;
			if(pData)xfUriLoader::FreeData(pData);
			if(pArgsEx != pArgs) delete pArgsEx;
			return 1;
		}else
		{
			if(pArgsEx != pArgs) delete pArgsEx;
			return -1;
		}
	}

	int IXAServer::ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs)
	{
		if(!pStrServer  || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		BYTE * pData = NULL;
		int len;
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xaction:%s"),pObjGuid);
		if(xfUriLoader::ProcessCmd(pStrServer,buf,pArgsEx,pData,len)==1)
		{
			if(pData)xfUriLoader::FreeData(pData);
			if(pArgsEx != pArgs) delete pArgsEx;
			return 1;
		}else
		{
			if(pArgsEx != pArgs) delete pArgsEx;
			return -1;
		}
	}

	int IXAServer::ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!pStrServer || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xquery:%s"),pObjGuid);
		int ret = xfUriLoader::ProcessCmd(pStrServer,buf,pArgsEx,pData,len);
		if(pArgsEx != pArgs) delete pArgsEx;
		return ret;
	}

	int IXAServer::ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!pStrServer  || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xquery:%s"),pObjGuid);
		int ret = xfUriLoader::FetchXml(pStrServer,buf,pArgsEx,xml);
		if(pArgsEx != pArgs) delete pArgsEx;
		return ret;
	}
	int IXAServer::ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!pStrServer  || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xaction:%s"),pObjGuid);
		int ret = xfUriLoader::ProcessCmd(pStrServer,buf,pArgsEx,pData,len);
		if(pArgsEx != pArgs) delete pArgsEx;
		return ret;
	}
	int IXAServer::ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!pStrServer  || !pObjGuid) return -9;
		LPTSTR pArgsEx = (LPTSTR)ExecArgs(pArgs);
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("uri.hxsoft.com/xaserver/xaction:%s"),pObjGuid);
		int ret = xfUriLoader::FetchXml(pStrServer,buf,pArgsEx,xml);
		if(pArgsEx != pArgs) delete pArgsEx;
		return ret;
	}


	int IXAServer::ExecXQuery(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!pTrans)return -1;
		return ExecXQuery(pTrans->GetServerUrl(),pObjGuid,pArgs,pData,len);
	}
	int IXAServer::ExecXQuery(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!pTrans)return -1;
		return ExecXQuery(pTrans->GetServerUrl(),pObjGuid,pArgs,xml);
	}
	int IXAServer::ExecXAction(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!pTrans)return -1;
		return ExecXAction(pTrans->GetServerUrl(),pObjGuid,pArgs,pData,len);
	}
	int IXAServer::ExecXAction(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!pTrans)return -1;
		return ExecXAction(pTrans->GetServerUrl(),pObjGuid,pArgs,xml);
	}

	int IXAServer::ExecXQuery(LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!m_pTrans)return -1;
		return ExecXQuery(m_pTrans->GetServerUrl(),pObjGuid,pArgs,pData,len);
	}
	int IXAServer::ExecXQuery(LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!m_pTrans)return -1;
		return ExecXQuery(m_pTrans->GetServerUrl(),pObjGuid,pArgs,xml);
	}
	int IXAServer::ExecXAction(LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len)
	{
		if(!m_pTrans)return -1;
		return ExecXAction(m_pTrans->GetServerUrl(),pObjGuid,pArgs,pData,len);
	}
	int IXAServer::ExecXAction(LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml)
	{
		if(!m_pTrans)return -1;
		return ExecXAction(m_pTrans->GetServerUrl(),pObjGuid,pArgs,xml);
	}

	//重新连接
	int IXAServer::ReConnect(LPCTSTR pStrServer)
	{
		if(xfApp::GetApp()->m_pConnect)
		{
			delete xfApp::GetApp()->m_pConnect;
			xfApp::GetApp()->m_pConnect = NULL;
		}
		return 1;
	}

	int IXAServer::ReConnect(ITrans * pTrans)
	{
		if(!pTrans)return -1;
		return ReConnect(pTrans->GetServerUrl());

	}

	int IXAServer::ReConnect()
	{
		if(!m_pTrans)return -1;
		return ReConnect(m_pTrans->GetServerUrl());
	}

	//得到用户
	LPCTSTR IXAServer::GetUserNo()
	{
		return xfApp::GetApp()->GetUser();
	}

	LPCTSTR IXAServer::GetVersion()
	{
		xbXml xml;
		IXMLDOMNode* pNode=NULL;
		xml.Load(L"updater/updater.xml");

		BSTR bstrVer = NULL;
		BSTR bstrServer = NULL;
		xml.GetXmlDoc()->selectSingleNode(L"//application[@name='xexplorer']/@version",&pNode);
		if(pNode)pNode->get_text(&bstrVer);
		if(pNode)pNode->Release();

		if(bstrVer) 
			return bstrVer;
		else
			return NULL;
	}

	LPWSTR IXAServer::GetComputerNameEx()
	{
		const TCHAR Sz[256]={L""};
		DWORD dw=256;
		::GetComputerName((TCHAR*)Sz,&dw);
		return (TCHAR*)Sz;
	}
	LPWSTR IXAServer::GetAppPath()
	{
		const TCHAR Sz[256]={L""};
		DWORD dw=256;
		::GetCurrentDirectory(dw, (TCHAR*)Sz);
		return (TCHAR*)Sz;
	}
	int IXAServer::SetUID(LPWSTR uid)
	{
		delete xfApp::GetApp()->m_pUID;
		if(uid)
			xfApp::GetApp()->m_pUID = uid;
		return 1;
	}

	LPWSTR IXAServer::GetUID()
	{
		if(xfApp::GetApp()->m_pUID)
			return xfApp::GetApp()->m_pUID;
		else
			return NULL;
	}

	LPWSTR IXAServer::GetMacAddress()
	{
		char pMac[256];

		//PIP_ADAPTER_INFO结构体指针存储本机网卡信息    
		PIP_ADAPTER_INFO pIpAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));//new IP_ADAPTER_INFO();
		//得到结构体大小,用于GetAdaptersInfo参数    
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);    
		//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量    
		int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);    
			//释放原来的内存空间        
		if (ERROR_BUFFER_OVERFLOW==nRel)    
		{         
			//如果函数返回的是ERROR_BUFFER_OVERFLOW　　  
			//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小        
			//这也是说明为什么stSize既是一个输入量也是一个输出量        
			delete pIpAdapterInfo;       
			//重新申请内存空间用来存储所有网卡信息               
			pIpAdapterInfo = (IP_ADAPTER_INFO *)malloc(stSize);//(PIP_ADAPTER_INFO)new BYTE[stSize];      
			//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量        
			nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);        
		}    
		if (ERROR_SUCCESS==nRel)
		{      
			//输出网卡信息       
			//可能有多网卡,因此通过循环去判断 ip!=0和169开头  0.0.0.0
			char pIp[20];
			while (pIpAdapterInfo)
			{
				sprintf(pIp, pIpAdapterInfo->IpAddressList.IpAddress.String);
				if(pIp[0]==0 || pIp[0]=='0' || (pIp[0]=='1' && pIp[1]=='6' && pIp[2]=='9'))
					pIpAdapterInfo = pIpAdapterInfo->Next;
				else
					//pIpAdapterInfo = pIpAdapterInfo->Next;
					break; 
			}
			if(!pIpAdapterInfo)
				return L"00-00-00-00-00-00";
			sprintf(pMac, "%02X-%02X-%02X-%02X-%02X-%02X",
			int (pIpAdapterInfo->Address[0]),
			int (pIpAdapterInfo->Address[1]),
			int (pIpAdapterInfo->Address[2]),
			int (pIpAdapterInfo->Address[3]),
			int (pIpAdapterInfo->Address[4]),
			int (pIpAdapterInfo->Address[5]));

		}    
		//释放内存空间    
		//if (pIpAdapterInfo)    		
		//	delete pIpAdapterInfo; 

		BSTR pPCMAC;
		int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pMac, (int)strlen(pMac), NULL, 0);
		pPCMAC = SysAllocStringLen(NULL,cchWide + 1);
		pPCMAC[cchWide]=0;
		::MultiByteToWideChar(CP_UTF8, 0, pMac, (int)strlen(pMac), pPCMAC, cchWide + 1);
		pMac[0] = '\0';
		return pPCMAC;
	}

	LPWSTR IXAServer::GetIpAddress()
	{
		char pMac[20];

		//PIP_ADAPTER_INFO结构体指针存储本机网卡信息    
		PIP_ADAPTER_INFO pIpAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));//new IP_ADAPTER_INFO();
		//得到结构体大小,用于GetAdaptersInfo参数    
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);    
		//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量    
		int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);    
			//释放原来的内存空间        
		if (ERROR_BUFFER_OVERFLOW==nRel)    
		{         
			//如果函数返回的是ERROR_BUFFER_OVERFLOW　　  
			//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小        
			//这也是说明为什么stSize既是一个输入量也是一个输出量        
			delete pIpAdapterInfo;       
			//重新申请内存空间用来存储所有网卡信息               
			pIpAdapterInfo = (IP_ADAPTER_INFO *)malloc(stSize);//(PIP_ADAPTER_INFO)new BYTE[stSize];      
			//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量        
			nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);        
		}    
		if (ERROR_SUCCESS==nRel)
		{      
			//输出网卡信息       
			while (pIpAdapterInfo)
			{
				sprintf(pMac, pIpAdapterInfo->IpAddressList.IpAddress.String);
				if(pMac[0]==0 || pMac[0]=='0' || (pMac[0]=='1' && pMac[1]=='6' && pMac[2]=='9'))
					pIpAdapterInfo = pIpAdapterInfo->Next;
				else
					break; 
			}
			if(!pIpAdapterInfo)
				return L"0.0.0.0";

		}    
		//释放内存空间    
		//if (pIpAdapterInfo)    		
		//	delete pIpAdapterInfo; 

		BSTR pPCMAC;
		int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pMac, (int)strlen(pMac), NULL, 0);
		pPCMAC = SysAllocStringLen(NULL,cchWide + 1);
		pPCMAC[cchWide]=0;
		::MultiByteToWideChar(CP_UTF8, 0, pMac, (int)strlen(pMac), pPCMAC, cchWide + 1);
		pMac[0] = '\0';
		return pPCMAC;
	}
	
	LPWSTR IXAServer::GetConnectId()
	{
		GUID mGuid = xfApp::GetApp()->guid;
		static TCHAR pGuid[128];
		memset(pGuid,'\0',128);
		wnsprintf(pGuid,128,_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		  mGuid.Data1, mGuid.Data2, mGuid.Data3,
		  mGuid.Data4[0], mGuid.Data4[1], mGuid.Data4[2], mGuid.Data4[3], 
		  mGuid.Data4[4], mGuid.Data4[5], mGuid.Data4[6], mGuid.Data4[7]
		);
		return pGuid;
	}

	int IXAServer::SetLoginLog(LPCTSTR pStrServer,LPWSTR pStatus)
	{
		IXAServerArg arg;
		arg.AddArg(L"userno",GetUserNo());
		arg.AddArg(L"userid",GetUID());
		arg.AddArg(L"computer",GetComputerNameEx());
		arg.AddArg(L"mac",GetMacAddress());
		arg.AddArg(L"version",GetVersion());
		arg.AddArg(L"status",pStatus);
		arg.AddArg(L"net",pStrServer);
		arg.AddArg(L"ip",GetIpAddress());
		arg.AddArg(L"connectid",GetConnectId());
		xbXml xml;
		ExecXQuery(pStrServer,L"[SysLoginLog.xq]",arg.GetString(),xml);
		return 1;
	}
	void IXAServer::SetDataToClip(LPCTSTR DataStr)
	{    
		LPCTSTR szText = DataStr;
		int nLen = (wcslen(szText) + 1) * sizeof(szText[0]);
		if (::OpenClipboard (NULL)) {
			::EmptyClipboard ();
			HANDLE hData = ::GlobalAlloc (GMEM_MOVEABLE, nLen);
			LPWSTR pData = (LPWSTR)::GlobalLock (hData);
			//::lstrcpy (pData, szText);
			memcpy(pData, szText, nLen); 
			::GlobalUnlock(hData);

			::SetClipboardData(CF_UNICODETEXT, hData);
			::CloseClipboard ();
		}
		return;    
	}
	int IXAServer::DownLoadFile(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExt,LPTSTR pName)
	{
		BYTE * pData= NULL;
		int len;
		int ret = LoadUrl(pStrServer, pUrl, pExt, pData, len, false);
		if(ret == 1)
		{
			if(!pName || pName[0]==0)
			{
				TCHAR szFileName[255],szPath[255];
				int	 nFilterIndex = 1;
				ZeroMemory(szFileName,255);
				ZeroMemory(szPath,255);
				if(pExt) lstrcpy(szPath, pExt);

				OPENFILENAME ofn;
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.hInstance = NULL;
				ofn.lpstrFilter = NULL;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.nFilterIndex = nFilterIndex;
				ofn.lpstrFile = szPath;
				ofn.nMaxFile = 255;
				ofn.lpstrFileTitle = szFileName;
				ofn.nMaxFileTitle = 255;
				ofn.lpstrTitle = L"请指定文件名";
				ofn.lpstrDefExt = NULL;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
				ofn.lCustData = 0;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;

				if(::GetSaveFileName(&ofn))
					pName = szPath;
				else
					ret = -1;
			}
			if(ret > 0)
			{
				HANDLE fp1;
				DWORD nWrite;
				fp1 = CreateFile(pName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

				/*BYTE p[2]={0xFF,0xFE};
				if(!(pData[0]==0xff &&  pData[1]==0xfe))
					::WriteFile(fp1,p,2,&nWrite,NULL);
				*/
				::WriteFile(fp1,pData,len,&nWrite,NULL);
				::CloseHandle(fp1);				
			}
			//ret = Hxsoft::XUtil::SaveToFile(pName, (LPTSTR)pData, NULL,NULL,(LPTSTR)pExt);
		}
		if(pData)xfUriLoader::FreeData(pData);
		return ret;
	}
}}