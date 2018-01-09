#include "StdAfx.h"
#include "..\include\xfUriLoader.hpp"
#include "xfconnect.hpp"
#include "xfApp.hpp"
#include "xfWin.hpp"
#include "xfControl.hpp"
#include "ixaserverarg.hpp"
#include "ixaserver.hpp"
#include <atlbase.h>
#include "xfXPage.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfUriLoader::xfUriLoader(void)
	{
	}

	xfUriLoader::~xfUriLoader(void)
	{
	}

	xfConnect* xfUriLoader::GetNewConnect(LPCTSTR pStrUrl)
	{
		xfConnect * pConnect = 0;
		if(!pStrUrl || pStrUrl[0]=='\0') pStrUrl = xfApp::GetApp()->GetServerUrl();
		if(!pStrUrl || pStrUrl[0]=='\0') return NULL;
		DWORD dwSize;
		TCHAR strHostname[INTERNET_MAX_HOST_NAME_LENGTH ];
		TCHAR strPort[INTERNET_MAX_PORT_NUMBER_LENGTH];
		dwSize = INTERNET_MAX_HOST_NAME_LENGTH;
		UrlGetPart(pStrUrl,strHostname,&dwSize,URL_PART_HOSTNAME,URL_ESCAPE_UNSAFE);

		dwSize = INTERNET_MAX_PORT_NUMBER_LENGTH;
		if(UrlGetPart(pStrUrl,strPort,&dwSize,URL_PART_PORT,0)!=S_OK)
		::_tcscpy_s(strPort,INTERNET_MAX_PORT_NUMBER_LENGTH,_T("80"));

		if(!pConnect)
		{
			pConnect = new xfConnect();
		}else
			pConnect->CloseSocket(pConnect->m_socket);
		SYS_SOCKET sock = pConnect->CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
			SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("正在连接..."));
		if(!pConnect->connectto(sock,strHostname,::_ttol(strPort)))
		{
			if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
				SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("连接失败..."));
			pConnect->CloseSocket(pConnect->m_socket);
			return NULL;
		}
		if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
			SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("连接完成..."));
		pConnect->m_socket = sock;
		//if(xfApp::GetApp()->m_pConnect != pConnect)xfApp::GetApp()->m_pConnect = pConnect;
		return pConnect;
	}

	xfConnect* xfUriLoader::GetConnect(LPCTSTR pStrUrl,bool bReconnect)
	{
		xfConnect * pConnect = xfApp::GetApp()->m_pConnect;
		if(!pConnect || bReconnect)
		{
			if(!pStrUrl || pStrUrl[0]=='\0') pStrUrl = xfApp::GetApp()->GetServerUrl();
			if(!pStrUrl || pStrUrl[0]=='\0') return NULL;
			DWORD dwSize;
			TCHAR strHostname[INTERNET_MAX_HOST_NAME_LENGTH ];
			TCHAR strPort[INTERNET_MAX_PORT_NUMBER_LENGTH];
			dwSize = INTERNET_MAX_HOST_NAME_LENGTH;
			UrlGetPart(pStrUrl,strHostname,&dwSize,URL_PART_HOSTNAME,URL_ESCAPE_UNSAFE);

			dwSize = INTERNET_MAX_PORT_NUMBER_LENGTH;
			if(UrlGetPart(pStrUrl,strPort,&dwSize,URL_PART_PORT,0)!=S_OK)
			::_tcscpy_s(strPort,INTERNET_MAX_PORT_NUMBER_LENGTH,_T("80"));

			if(!pConnect)
			{
				pConnect = new xfConnect();
			}else
				pConnect->CloseSocket(pConnect->m_socket);
			SYS_SOCKET sock = pConnect->CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
				SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("正在连接..."));
			if(!pConnect->connectto(sock,strHostname,::_ttol(strPort)))
			{
				if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
					SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("连接失败..."));
				pConnect->CloseSocket(pConnect->m_socket);
				return NULL;
			}
			if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
				SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("连接完成..."));
			pConnect->m_socket = sock;
			if(xfApp::GetApp()->m_pConnect != pConnect)xfApp::GetApp()->m_pConnect = pConnect;
		}
		return pConnect;
	}

	int xfUriLoader::FetchXml(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml)
	{
		int nLen;
		LPBYTE pData = NULL;
		if(ProcessCmd(pServerUrl,pStrCmd,pExtStr,pData,nLen)!=1)
		{
			if(!pData)
			{
				xml.LoadXml(L"<error/>");
				IXMLDOMElement * pEle;
				xml.GetXmlDoc()->get_documentElement(&pEle);
				pEle->put_text((BSTR)L"Unkown source");
				pEle->Release();
			}else
			{
				xml.LoadXml(L"<error/>");
				IXMLDOMElement * pEle;
				xml.GetXmlDoc()->get_documentElement(&pEle);
				pEle->put_text((BSTR)pData);
				pEle->Release();
			}		
			return -1;
		}
		if(!pData) return 0;
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
		return 1;

	}

	int xfUriLoader::ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len,HWND hWnd)
	{
		xfConnect * pConnect = GetNewConnect(pServerUrl);
		if(!pConnect) return -1;
		//xfConnect * pConnect = GetConnect(pServerUrl);
		if(!pConnect) return -1;

		if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr))
		{
			//pConnect = GetConnect(pServerUrl,true);
			if(!pConnect) return -1;
			if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr)) return -1;
		}
		TExchHeader exchHeader;
		int ret = pConnect->ReadOnePack(pConnect->m_socket,exchHeader,pData,len);
		if(ret==-1)
		{
			if(pConnect)
			{
				pConnect->CloseSocket(pConnect->m_socket);
				delete pConnect;
			}
			return -1;
		}
		if(exchHeader.Flag & 0x01)
			pConnect->ResponseCommand(pConnect->m_socket,hWnd);
		else
			PostMessage(hWnd,0x400+ 81,0,0);
		return 1;
	}

	int xfUriLoader::ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPVOID pExtStr,int nDataLen,BYTE * &pData,int &len,bool bcompress)
	{
		xfConnect * pConnect = GetConnect(pServerUrl);
		if(!pConnect) return -1;

continuesendcommandex:
		if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr,nDataLen,bcompress))
		{
			pConnect = GetConnect(pServerUrl,true);
			if(!pConnect) return -1;
			if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr,nDataLen,bcompress)) return -1;
		}
		
		bool	bOk = true;
		int		nRet;
		nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);

		if(nRet < 0)
		{
			pConnect->CloseSocket(pConnect->m_socket);
			pConnect = NULL;
			xfApp::GetApp()->m_pConnect = NULL;
			bOk = false;
		}
		if(nRet == 1)//need login
		{
			TCHAR strLoginInfo[1024];
			strLoginInfo[0]='\0';
			if(pData[0] !=0 && pData[1]!=0) //ansi
			{
				int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, len, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, len, psz, _convert);
				psz[_convert]='\0';
				xfWin * pWin =  xfWin::LoadXPageXml(psz,(HWND)NULL,xfApp::GetApp()->GetInstance(),(LPVOID)strLoginInfo);
				delete psz;
			}else
			{
				xfWin * pWin =  xfWin::LoadXPageXml((LPTSTR)pData,(HWND)NULL,xfApp::GetApp()->GetInstance(),(LPVOID)strLoginInfo);
			}
			xfUriLoader::FreeData(pData);
			if(strLoginInfo[0]!='\0')
			{
				pConnect->SendAuthCommand(pConnect->m_socket,strLoginInfo);
				nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);
				if(nRet <0)
				{
					MessageBox(NULL,(LPTSTR)pData,_T("提示"),MB_OK);
					if(pData)xfUriLoader::FreeData(pData);
					pData = NULL;
					return -1;
				}else
				{
					goto continuesendcommandex;
				}
			}else
			{
				pData = NULL;
				return -1;
			}
		}
		if(!bOk && nRet <-1 )
		{
			if(xfApp::GetApp()->m_pConnect)
			{
				delete xfApp::GetApp()->m_pConnect;
				xfApp::GetApp()->m_pConnect = NULL;
			}
		}
		return bOk? 1:-1;
	}
	int xfUriLoader::ProcessCmd(LPCTSTR pServerUrl,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len)
	{
		xfConnect * pConnect = GetConnect(pServerUrl);
		if(!pConnect) return -1;

continuesendcommandex:
		if(!pConnect->IsConnected())
		{
			pConnect = GetConnect(pServerUrl,true);
			if(!pConnect)
			{
				if(xfApp::GetApp()->m_pDefaultView)
					::ShowWindow(xfApp::GetApp()->m_pDefaultView->GetHWND(), SW_SHOW);
				pConnect = GetConnect(pServerUrl,true);
			}
			if(!pConnect || !pConnect->SendCommand(pConnect->m_socket,pStrCmd)) return -1;
		}
		if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd,pExtStr))
		{
			pConnect = GetConnect(pServerUrl,true);
			if(!pConnect) return -1;
			if(!pConnect->SendCommand(pConnect->m_socket,pStrCmd)) return -1;
		}

		bool	bOk = true;
		int		nRet;
		nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);

		if(nRet < 0)
		{
			bOk = false;
		}
		if(nRet == 1)//need login
		{
			if(xfApp::GetApp()->m_pDefaultView)
				::ShowWindow(xfApp::GetApp()->m_pDefaultView->GetHWND(), SW_SHOW);
			else
			{
				if(!xfApp::GetApp()->m_pUser) return -1;

				TCHAR strLoginInfo[1024];
				strLoginInfo[0]='\0'; 

				IXAServerArg arg;
				arg.AddArg(L"username",xfApp::GetApp()->m_pUser);
				arg.AddArg(L"password",xfApp::GetApp()->m_pPassWord);
				arg.AddArg(L"computer",IXAServer::GetComputerNameEx());
				arg.AddArg(L"mac",IXAServer::GetMacAddress());
				arg.AddArg(L"net",L"xxxxx");
				arg.AddArg(L"ip",IXAServer::GetIpAddress());
				arg.AddArg(L"checkpass",L"Y");
				::_stprintf_s(strLoginInfo,1024,_T("%s"),arg.GetString());

				pConnect->SendAuthCommand(pConnect->m_socket,strLoginInfo);
				nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);
				if(nRet <0)
				{
					MessageBox(NULL,(LPTSTR)pData,_T("提示"),MB_OK);
					if(pData)xfUriLoader::FreeData(pData);
					pData = NULL;
					return -1;
				}else
				{
					goto continuesendcommandex;
				}
			}
			//::SetActiveWindow(xfApp::GetApp()->m_pDefaultView->GetHWND());
			//xfApp::GetApp()->m_pDefaultView->m_dwStatus &= WST_RESPONSELOOP & WST_RESPONSEWIN;
			bOk = true;
/*
			TCHAR strLoginInfo[1024];
			strLoginInfo[0]='\0';
			if(pData[0] !=0 && pData[1]!=0) //ansi
			{
				int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, len, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, len, psz, _convert);
				psz[_convert]='\0';
				xfWin * pWin =  xfWin::LoadXPageXml(psz,(HWND)NULL,xfApp::GetApp()->GetInstance(),(LPVOID)strLoginInfo);
				delete psz;
			}else
			{
				xfWin * pWin =  xfWin::LoadXPageXml((LPTSTR)pData,(HWND)NULL,xfApp::GetApp()->GetInstance(),(LPVOID)strLoginInfo);
			}

			xfUriLoader::FreeData(pData);
			if(strLoginInfo[0]!='\0')
			{
				pConnect->SendAuthCommand(pConnect->m_socket,strLoginInfo);
				nRet = pConnect->ResponseCommand(pConnect->m_socket,pData,len);
				if(nRet <0)
				{
					MessageBox(NULL,(LPTSTR)pData,_T("提示"),MB_OK);
					if(pData)xfUriLoader::FreeData(pData);
					pData = NULL;
					return -1;
				}else
				{
					/*IXAServerArg arg;
					if(arg.SetXml((LPTSTR)strLoginInfo) == 1)
					{
						LPTSTR pUser = arg.GetArgString(L"username");

						if(::PathFileExists(L"cache\\user.dat"))
							::DeleteFile(L"cache\\user.dat");
						std::wstring wcopy = pUser;
						DWORD dwSize;
						HANDLE hFile = ::CreateFile(L"cache\\user.dat",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
						WriteFile(hFile,pUser,_tcslen(pUser)*2,&dwSize,NULL);
						CloseHandle(hFile);
					}
					goto continuesendcommandex;
				}
			}else
			{
				pData = NULL;
				return -1;
			}*/
		}
		if(!bOk && nRet <-1 )
		{
			if(xfApp::GetApp()->m_pConnect)
			{
				delete xfApp::GetApp()->m_pConnect;
				xfApp::GetApp()->m_pConnect = NULL;
			}
		}
		return bOk? 1:-1;
	}
	
	FILETIME xfUriLoader::GetScriptDT(LPTSTR pStrUrl)
	{
		FILETIME ft;
		::ZeroMemory(&ft,sizeof(FILETIME));
		BYTE* pData;
		int len;
		if(_tcsstr(pStrUrl,L"dev:")==pStrUrl)
		{
			TCHAR buf[80];
			_stprintf_s(buf,80,L"dt:%s",pStrUrl + 4);
			if(ProcessCmd(NULL,buf,NULL,pData,len)>=0 && pData)
			{
				ft = *(FILETIME *)pData;
				FreeData(pData);
			}
		}
		return ft;
	}
	FILETIME xfUriLoader::GetScriptVER(LPTSTR pStrUrl)
	{
		FILETIME ft;
		::ZeroMemory(&ft,sizeof(FILETIME));
		return ft;
	}

	LPTSTR xfUriLoader::LoadScript(LPTSTR pStrUrl)
	{
		BYTE* pData;
		int len;
		LPTSTR pStr = NULL;
		if(ProcessCmd(NULL,pStrUrl,NULL,pData,len)>=0 && pData)
		{
			if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
			{
				pStr = (LPTSTR)(pData + 2);
			}else
				pStr = (LPTSTR)pData;
		}
		return pStr;
	}

	void	xfUriLoader::FreeScript(LPTSTR pStrUrl)
	{
		if(!pStrUrl)return;
		if(pStrUrl[-1]==0xfffe||pStrUrl[-1]==0xfeff) pStrUrl-= 1;
		FreeData((BYTE *)pStrUrl );
	}

	int xfUriLoader::Load(LPCTSTR pStrUrl,LPCTSTR pStrUri, LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache)
	{
		if(UrlIsFileUrl(pStrUrl)) return 1;
		if(!pStrUri)pStrUri = _T("uri.hxsoft.com/xaserver/xpage");

		DWORD dwSize;
		TCHAR strScheme[INTERNET_MAX_SCHEME_LENGTH];

		if(_tcsstr(pStrUrl,_T("xquery:")) || _tcsstr(pStrUrl,_T("xaction:"))/* || _tcsstr(pStrUrl,_T("dev:")) */||
			_tcsstr(pStrUrl,_T("worknode:")))
		{
			if(_tcsstr(pStrUrl,_T("xquery:")))
			{
				LPTSTR pArgsEx = (LPTSTR)IXAServer::ExecArgs(pExtStr);
				if(ProcessCmd(NULL,_tcsstr(pStrUrl,_T("xquery:")),pArgsEx,pData,len)<0)return -1;
				if(pArgsEx != pExtStr) delete pArgsEx;
			}
			else if(_tcsstr(pStrUrl,_T("xaction:")))
			{
				LPTSTR pArgsEx = (LPTSTR)IXAServer::ExecArgs(pExtStr);
				if(ProcessCmd(NULL,_tcsstr(pStrUrl,_T("xaction:")),pArgsEx,pData,len)<0)return -1;
				if(pArgsEx != pExtStr) delete pArgsEx;
			}
			/*else if(_tcsstr(pStrUrl,_T("dev:")))
			{
				if(ProcessCmd(NULL,_tcsstr(pStrUrl,_T("dev:")),pExtStr,pData,len)<0)return -1;
			}*/
			else if(_tcsstr(pStrUrl,_T("worknode:")))
			{
				if(ProcessCmd(NULL,_tcsstr(pStrUrl,_T("worknode:")),pExtStr,pData,len)<0)return -1;
			}
		
			//cache
			if(bCache)
			{
				HANDLE fp1;
				DWORD nWrite;
				TCHAR Path[MAX_PATH];
				GetCacheFileName(pStrUrl,Path);
				fp1 = CreateFile(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

				//if((pData[0]==0xff &&  pData[1]==0xfe)

				BYTE p[2]={0xFF,0xFE};
				if(!(pData[0]==0xff &&  pData[1]==0xfe))
					::WriteFile(fp1,p,2,&nWrite,NULL);

				::WriteFile(fp1,pData,len,&nWrite,NULL);
				::CloseHandle(fp1);
			}
			return 1;
		}

		dwSize = INTERNET_MAX_SCHEME_LENGTH;
		UrlGetPart(pStrUrl,strScheme,&dwSize,URL_PART_SCHEME,0);
		if(::StrCmpI(strScheme,L"http")!=0)
		{
			if(_tcsstr(pStrUrl,_T("dev:")))
			{
				if(ProcessCmd(NULL,_tcsstr(pStrUrl,_T("dev:")),pExtStr,pData,len)<0)return -1;

				//cache
				if(bCache)
				{
					HANDLE fp1;
					DWORD nWrite;
					TCHAR Path[MAX_PATH];
					GetCacheFileName(pStrUrl,Path);
					fp1 = CreateFile(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

					//if((pData[0]==0xff &&  pData[1]==0xfe)

					BYTE p[2]={0xFF,0xFE};
					if(!(pData[0]==0xff &&  pData[1]==0xfe))
						::WriteFile(fp1,p,2,&nWrite,NULL);

					::WriteFile(fp1,pData,len,&nWrite,NULL);
					::CloseHandle(fp1);
				}
				return 1;
			}
			return -1;
		}

		TCHAR Path[MAX_PATH];
		/*if(bCache && CacheFileExists(pStrUrl,Path))
		{
			len = 0;
			pData = NULL;
			return 1;
		}*/
		
		TCHAR * pStrUrlCanonicalize = new TCHAR[INTERNET_MAX_URL_LENGTH + ::_tcslen(pStrUri) + 1];
		::_tcscpy_s(pStrUrlCanonicalize,::_tcslen(pStrUri) + 1,pStrUri);
		pStrUrlCanonicalize[::_tcslen(pStrUri)]=' ';
		dwSize=INTERNET_MAX_URL_LENGTH;
		UrlCanonicalize(pStrUrl,pStrUrlCanonicalize + (::_tcslen(pStrUri) + 1) ,&dwSize,URL_ESCAPE_UNSAFE);

		int nLen = (int)::_tcslen(pStrUrlCanonicalize) + 12;
		if(pExtStr) nLen += (int)::_tcslen(pExtStr);
		TCHAR* pBuf = new TCHAR[nLen];

		::_stprintf_s(pBuf,nLen,_T("%s"),pStrUrlCanonicalize);

		if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar")))
			SendMessage(xfApp::GetApp()->m_pFrame->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)pBuf);

		if(ProcessCmd(pStrUrl,pBuf,pExtStr,pData,len)<0)
		{
			delete pBuf;
			return -1;
		}
		
		//cache
		if(bCache)
		{
			HANDLE fp1;
			DWORD nWrite;
			GetCacheFileName(pStrUrl,Path);
			fp1 = CreateFile(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			::WriteFile(fp1,pData,len,&nWrite,NULL);
			::CloseHandle(fp1);
		}
		return 1;
	}

	int xfUriLoader::Load(LPCTSTR pStrUrl,LPCTSTR pStrUri, BYTE * &pData,int &len,bool bCache)
	{
		return Load(pStrUrl,pStrUri,NULL,pData,len,bCache);
	}

	bool xfUriLoader::CacheFileExists(LPCTSTR pStrUrl,LPTSTR pStrCacheFile,DWORD dwSize)
	{
		GetCacheFileName(pStrUrl,pStrCacheFile,dwSize);
		if(PathFileExists(pStrCacheFile))
			return true;
		else
			return false;
	}

	int xfUriLoader::GetCacheFileName(LPCTSTR pStrUrl,LPCTSTR pExtStr,LPTSTR pStrCacheFile,DWORD dwSize,bool Loaded)
	{
		if(UrlIsFileUrl(pStrUrl))
		{
			::_tcscpy_s(pStrCacheFile,dwSize,pStrUrl);
			return 1;
		}

		if(Loaded)
		{
			BYTE* pData = NULL;
			int nLen = 0;
			if(Load(pStrUrl,NULL,pExtStr,pData,nLen)!=1)
			{
				if(pData)FreeData(pData);
				return -1;
			}
			if(pData)FreeData(pData);
		}

		DWORD dwAtom = (DWORD)::AddAtom(pStrUrl);
		::GetModuleFileName(NULL,pStrCacheFile,MAX_PATH);
		PathRemoveFileSpec(pStrCacheFile);
		_stprintf_s(pStrCacheFile,dwSize,_T("%s%s%i%s"),pStrCacheFile,_T("\\cache\\"),dwAtom,_T(".t"));
		return 1;
	}

	int xfUriLoader::GetCacheFileName(LPCTSTR pStrUrl,LPTSTR pStrCacheFile,DWORD dwSize,bool Loaded)
	{
		return GetCacheFileName(pStrUrl,NULL,pStrCacheFile,dwSize,Loaded);
	}

	int xfUriLoader::FreeData(BYTE * pData)
	{
		if(pData) delete pData;
		return 0;
	}

	bool xfUriLoader::UrlCombine(LPCTSTR pszBase,LPCTSTR pszRelative,LPTSTR pszCombined,LPDWORD pcchCombined,DWORD dwFlags)
	{
		HRESULT hr = ::UrlCombine(pszBase,pszRelative,pszCombined,pcchCombined,dwFlags);
		if(hr==S_OK) return true;
		return false;
	}
}}
