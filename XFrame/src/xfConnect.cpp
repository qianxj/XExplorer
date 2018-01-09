#include "StdAfx.h"
#include "..\include\xfConnect.hpp"
#include <atlbase.h>
#include "xfApp.hpp"
#include "xuxutil.h"
#include "ixaserverarg.hpp"
#include "ixaserver.hpp"
#include "xfUriLoader.hpp"
#include "xfWin.hpp"
#include "xfxPage.hpp"
#include "shellapi.h"
#include "errors.h"

#define COMPRESSSEND 1
int nDelay = 120;
namespace Hxsoft{ namespace XFrame
{
	

	xfConnect::xfConnect(void):connected_(false),errcode_(0)
	{
	}

	xfConnect::~xfConnect(void)
	{
		if(this->m_socket) CloseSocket(this->m_socket);
	}

	SYS_SOCKET xfConnect::CreateSocket(int iAddressFamily , int iType, int iProtocol)
	{
		return ::SysCreateSocket(iAddressFamily,SOCK_STREAM,iProtocol);
	}

	void xfConnect::CloseSocket(SYS_SOCKET	sock)
	{
		return ::SysCloseSocket(sock);  
	}

	bool xfConnect::connectto(SYS_SOCKET sock,LPTSTR host,int port)
	{
		return connectto(sock,host,port,5);
	}

	bool xfConnect::connectto(SYS_SOCKET sock,LPTSTR host,int port, int nDelay)
	{
		SYS_INET_ADDR addr;
		NET_ADDRESS NetAddr;

		USES_CONVERSION;
		SysGetHostByName(T2A(host), NetAddr);
		SysSetupAddress(addr,AF_INET,NetAddr,port);

		//connect
		bool result;
L1:		errcode_ = SysConnect(sock,&addr,sizeof(SYS_INET_ADDR),nDelay);
		result = errcode_==0?true:false;
		if(result)
		{
			//初始会话
			TCHAR buf[255];
			::ZeroMemory(buf,255*sizeof(TCHAR));
			int nlen = SysRecvData(sock,(LPSTR)buf,255*sizeof(TCHAR),60);
			result = SendCommand(sock,_T("HELO")); 
			if(result) connected_ = true;
		}else
		{
			connected_ = false;
			return result;

			if(errcode_== ERR_TIMEOUT)
			{
				/*if(AutoLogin(&sock)) goto L1;*/
				HWND hWnd = 0;
				if(xfApp::GetApp()->m_pFrame) hWnd = xfApp::GetApp()->m_pFrame->GetHWND();
				int ret = MessageBox(hWnd,L"网络连接超时，是否继续连接？",L"提示",MB_ABORTRETRYIGNORE);
				if(ret==IDRETRY)
				{
					nDelay = 20;
					goto L1;
				}
				else
				{
					connected_ = false;
					if(ret==IDABORT)
						ShellExecute(NULL, L"open", L"Start.bat", NULL, NULL, SW_HIDE);
					else
						return result;
				}
				/*if(MessageBox(NULL,L"网络连接超时，是否继续连接？",L"提示",MB_YESNO)==IDNO)
					return result;
				else
				{
					nDelay = 20;
					goto L1;
				}*/
			}else //ERR_CONNECT ERR_SHUTDOWN
			{
				connected_ = false;
			}
		}
		return result; 
	}

	bool xfConnect::IsConnected()
	{
		return connected_;
	}

	bool xfConnect::SendCommand(SYS_SOCKET sock,LPCTSTR comstr)
	{
		return SendCommand(sock,comstr,NULL);
	}

	bool xfConnect::SendCommand(SYS_SOCKET sock,LPCTSTR comstr,LPVOID extStr,int nDataLen,bool bcompress)
	{
		int nLen;
		TExchHeader ExchHeader;
		LPBYTE pBuf = NULL;
		ExchHeader.token = xfApp::GetApp()->guid;
		if(!extStr)
		{
			nLen = (int)::_tcslen(comstr) + sizeof(ExchHeader)/sizeof(TCHAR) + 3;
			pBuf = new BYTE[nLen * sizeof(TCHAR)];
			::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)),nLen - sizeof(ExchHeader)/sizeof(TCHAR),_T("%s\r\n"),comstr);
			ExchHeader.nPackSize = nLen * (int)sizeof(TCHAR);
		}else
		{
#if	COMPRESSSEND
			if(bcompress)
			{
				int nLenCmd = ((int)::_tcslen(comstr) + 2) * sizeof(TCHAR);
				int nLen = nLenCmd + nDataLen;
				LPBYTE pSrc= new BYTE[nLen + 2]; //拷贝命令时,需要结束符
				::_stprintf_s(LPTSTR(pSrc) ,(int)::_tcslen(comstr) + 3,_T("%s\r\n"),comstr);
				memcpy_s(pSrc+nLenCmd,nDataLen,extStr, nDataLen);

				ULONG	nSize = Hxsoft::XUtil::CompressBound((ULONG)nLen);
				pBuf = new BYTE[nSize + sizeof(TExchHeader)];
				Hxsoft::XUtil::Compress(pBuf + sizeof(TExchHeader) ,&nSize,pSrc, nLen * sizeof(TCHAR));
				::ZeroMemory(pSrc,nLen * sizeof(TCHAR));
				
				//ULONG dwSrc = nLen * sizeof(TCHAR);
				//Hxsoft::XUtil::UnCompress(pSrc , &dwSrc,pBuf + sizeof(TExchHeader),nSize);
				
				ExchHeader.nPackSize = nSize + sizeof(ExchHeader);
				ExchHeader.dwSrcLen = nLen * sizeof(TCHAR);
				ExchHeader.Flag |= 0x40; //compress;
				if(pSrc)delete pSrc;
			}else
			{
				int nLenCmd = ((int)::_tcslen(comstr) + 2) * sizeof(TCHAR);
				int nLen = nLenCmd + nDataLen + sizeof(ExchHeader);
				pBuf= new BYTE[nLen+2]; //拷贝命令时,需要结束符
				::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)),(int)::_tcslen(comstr) + 3,_T("%s\r\n"),comstr);
				memcpy_s(pBuf+nLenCmd+sizeof(ExchHeader),nDataLen,extStr, nDataLen);

				ExchHeader.nPackSize = nLen;
			}
#else 
			nLen = (int)::_tcslen(comstr) + 3 + (int)::_tcslen(extStr) + sizeof(ExchHeader)/sizeof(TCHAR);
			pBuf = new BYTE[nLen * sizeof(TCHAR)];
			::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)) ,nLen - sizeof(ExchHeader)/sizeof(TCHAR),_T("%s\r\n%s"),comstr,extStr);
			ExchHeader.nPackSize = nLen * (int)sizeof(TCHAR);
#endif
		}
		if(pBuf)
		{
			::memcpy_s(pBuf,sizeof(ExchHeader),&ExchHeader,sizeof(ExchHeader));
L7:			nLen = ::SysSendData(sock,(LPCSTR)pBuf,ExchHeader.nPackSize,nDelay);
			if(nLen <0)
			{
				errcode_ = nLen;
				if(errcode_ == ERR_TIMEOUT)
				{
					if(AutoLogin(&sock)) goto L7;
					HWND hWnd = 0;
					if(xfApp::GetApp()->m_pFrame) hWnd = xfApp::GetApp()->m_pFrame->GetHWND();
					int ret = MessageBox(hWnd,L"数据发送超时，是否继续等待？",L"提示",MB_ABORTRETRYIGNORE);
					if(ret==IDRETRY)
						goto L7;
					else
					{
						connected_ = false;
						if(ret==IDABORT)
							ShellExecute(NULL, L"open", L"Start.bat", NULL, NULL, SW_HIDE);
					}
				}else
					connected_ = false;
			}

		}
		if(pBuf)delete pBuf;

		return nLen >0 ?true:false;
	}

	bool xfConnect::SendCommand(SYS_SOCKET sock,LPCTSTR comstr,LPCTSTR extStr)
	{
		int nLen;
		TExchHeader ExchHeader;
		LPBYTE pBuf = NULL;
		ExchHeader.token = xfApp::GetApp()->guid;
		if(!extStr)
		{
			nLen = (int)::_tcslen(comstr) + sizeof(ExchHeader)/sizeof(TCHAR) + 3;
			pBuf = new BYTE[nLen * sizeof(TCHAR)];
			::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)),nLen - sizeof(ExchHeader)/sizeof(TCHAR),_T("%s\r\n"),comstr);
			ExchHeader.nPackSize = nLen * (int)sizeof(TCHAR);
		}else
		{
#if	COMPRESSSEND
			nLen = (int)::_tcslen(comstr) + 3 + (int)::_tcslen(extStr);
			LPBYTE pSrc= new BYTE[nLen * sizeof(TCHAR)];
			::_stprintf_s(LPTSTR(pSrc) ,nLen,_T("%s\r\n%s"),comstr,extStr);
			
			ULONG	nSize = Hxsoft::XUtil::CompressBound((ULONG)nLen*sizeof(TCHAR) );
			pBuf = new BYTE[nSize + sizeof(TExchHeader)];
			Hxsoft::XUtil::Compress(pBuf + sizeof(TExchHeader) ,&nSize,pSrc, nLen * sizeof(TCHAR));
			::ZeroMemory(pSrc,nLen * sizeof(TCHAR));
			
			//ULONG dwSrc = nLen * sizeof(TCHAR);
			//Hxsoft::XUtil::UnCompress(pSrc , &dwSrc,pBuf + sizeof(TExchHeader),nSize);
			
			ExchHeader.nPackSize = nSize + sizeof(ExchHeader);
			ExchHeader.dwSrcLen = nLen * sizeof(TCHAR);
			ExchHeader.Flag |= 0x40; //compress;
			if(pSrc)delete pSrc;
#else 
			nLen = (int)::_tcslen(comstr) + 3 + (int)::_tcslen(extStr) + sizeof(ExchHeader)/sizeof(TCHAR);
			pBuf = new BYTE[nLen * sizeof(TCHAR)];
			::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)) ,nLen - sizeof(ExchHeader)/sizeof(TCHAR),_T("%s\r\n%s"),comstr,extStr);
			ExchHeader.nPackSize = nLen * (int)sizeof(TCHAR);
#endif
		}
		if(pBuf)
		{
			::memcpy_s(pBuf,sizeof(ExchHeader),&ExchHeader,sizeof(ExchHeader));
			errcode_ = 0;
L7:			nLen = ::SysSendData(sock,(LPCSTR)pBuf,ExchHeader.nPackSize,nDelay);
			if(nLen <0)
			{
				//if(MessageBox(NULL,L"数据发送错误，是否继续等待？",L"提示",MB_YESNO)==IDYES)
				//	goto L7;
				errcode_ = nLen;
				if(errcode_ == ERR_TIMEOUT)
				{
					if(AutoLogin(&sock)) goto L7;
					HWND hWnd = 0;
					if(xfApp::GetApp()->m_pFrame) hWnd = xfApp::GetApp()->m_pFrame->GetHWND();
					int ret = MessageBox(hWnd,L"数据发送超时，是否继续等待？",L"提示",MB_ABORTRETRYIGNORE);
					if(ret==IDRETRY)
						goto L7;
					else
					{
						connected_ = false;
						if(ret==IDABORT)
							ShellExecute(NULL, L"open", L"Start.bat", NULL, NULL, SW_HIDE);
					}
				}else
					connected_ = false;
			}

		}
		if(pBuf)delete pBuf;

		return nLen >0 ?true:false;
	}

	bool xfConnect::SendAuthCommand(SYS_SOCKET sock,LPCTSTR AuthStr)
	{
		int nLen;
		TExchHeader ExchHeader;
		LPBYTE pBuf = NULL;
		ExchHeader.nFuction = 0x01;

		nLen = (int)::_tcslen(AuthStr) + sizeof(ExchHeader)/sizeof(TCHAR) + 3;
		pBuf = new BYTE[nLen * sizeof(TCHAR)];
		::_stprintf_s(LPTSTR(pBuf + sizeof(ExchHeader)),nLen - sizeof(ExchHeader)/sizeof(TCHAR),_T("%s\r\n"),AuthStr);

		if(pBuf)
		{
			ExchHeader.nPackSize = nLen * (int)sizeof(TCHAR);
			::memcpy_s(pBuf,sizeof(ExchHeader),&ExchHeader,sizeof(ExchHeader));
			nLen = ::SysSendData(sock,(LPCSTR)pBuf,nLen * (int)sizeof(TCHAR),nDelay);
		}
		if(pBuf)delete pBuf;
		if(nLen > 0)
		{
			//if(xfApp::GetApp()->m_pUser) delete xfApp::GetApp()->m_pUser;
			//if(xfApp::GetApp()->m_pPassWord) delete xfApp::GetApp()->m_pPassWord;
			IXAServerArg arg;
			if(arg.SetXml((LPTSTR)AuthStr) == 1)
			{
				xfApp::GetApp()->m_pUser = arg.GetArgString(L"username");
				xfApp::GetApp()->m_pPassWord = arg.GetArgString(L"password");
			}
/*			
			LPTSTR pStr = (LPTSTR)_tcschr(AuthStr,'|');	
			if(pStr)
			{
				if(xfApp::GetApp()->m_pUser)delete xfApp::GetApp()->m_pUser;
				xfApp::GetApp()->m_pUser = new TCHAR[pStr - AuthStr + 1];
				_tcsncpy_s(xfApp::GetApp()->m_pUser,pStr - AuthStr + 1,AuthStr,pStr - AuthStr);
				xfApp::GetApp()->m_pUser[pStr - AuthStr]='\0';
			}*/
		}
		return nLen >0 ?true:false;
	}

	struct ResponseCommandParam
	{
		SYS_SOCKET sock;
		HWND hWnd;
		xfConnect * pConnect;
	};

	DWORD WINAPI ThreadProcResponseCommand( LPVOID lpParameter)
	{
		ResponseCommandParam* pParam = (ResponseCommandParam*)lpParameter;
		SYS_SOCKET sock =  pParam->sock;
		HWND hWnd = pParam->hWnd;
		xfConnect * pConnect = pParam->pConnect;
		
		LPBYTE pVoid = 0;
		int len = 0;
		int nSrcLen = 0;
		bool bResponseError = false;
		bool bContinue = false;
		bool bCompress = false;
		TExchHeader exchHeader;

		int ret = 1;
		while(true)
		{
			bContinue = false;
			int ret = pConnect->ReadOnePack(sock,exchHeader,pVoid,len);
			if(ret < 0)
			{
				SendMessage(hWnd,0x400+ 81,0,ret);
				ret = -1;
				break;
			}
			TExchHeader* pExchHeader = &exchHeader;
			if(pExchHeader->Flag & 0x80) bResponseError = true;
			if(pExchHeader->Flag & 0x01) bContinue = true;
			if(pExchHeader->Flag & 0x40)
			{
				bCompress = true;
				nSrcLen = pExchHeader->dwSrcLen;
			}
			if(bResponseError)
			{
				SendMessage(hWnd,0x400+ 81,0,-1);
				ret = -1;
				break;
			}
			SendMessage(hWnd, 0x400+ 80,(WPARAM)(pVoid),len );
			if(!bContinue)
			{
				SendMessage(hWnd,0x400+ 81,0,0);
				break;
			}
		}
		if(pParam) delete pParam;
		if(pConnect)
		{
			pConnect->CloseSocket(pConnect->m_socket);
			delete pConnect;
		}
		return ret;
	}


	int xfConnect::ResponseCommand(SYS_SOCKET sock,HWND hWnd)
	{
		ResponseCommandParam* pParam = new ResponseCommandParam();
		pParam->sock = sock;
		pParam->hWnd = hWnd;
		pParam->pConnect = this;
		::CreateThread(0,0,ThreadProcResponseCommand,pParam,0,0);
		return 1;
	}

	int xfConnect::ReadOnePack(SYS_SOCKET sock,TExchHeader& exchHeader, LPBYTE & pVoid,int &len)
	{
		pVoid = 0;
		len =0;
		int CountDelay = 30;
		char  pData[sizeof(TExchHeader)];
		int revLen = SysRecv(sock,pData,sizeof(TExchHeader),nDelay*CountDelay) ;
		if(revLen < sizeof(TExchHeader)) return -1;
		TExchHeader* pExchHeader = (TExchHeader *)pData;
		int ttllen = (int)((DWORD)pExchHeader->nPackSize);
		
		exchHeader = *pExchHeader; 

		char * pBuf = new char[ttllen + 2 - sizeof(TExchHeader)];
		if(!pBuf) return -2;
		ZeroMemory(pBuf,ttllen  - sizeof(TExchHeader) + 2);
		int iRtxBytes = 0;
		int iBufferSize = ttllen  - sizeof(TExchHeader) ;
		while (iRtxBytes < iBufferSize) {
			int iRtxCurrent = SysRecvData(sock, pBuf  + iRtxBytes,
							  iBufferSize - iRtxBytes, nDelay);
			if (iRtxCurrent == ERR_TIMEOUT)
			{
				CountDelay --;
				if(!CountDelay) return -1;
				continue;
			}
			if(iRtxCurrent  < 0) return iRtxCurrent;
			iRtxBytes += iRtxCurrent;
		}
		pVoid = (LPBYTE)pBuf;
		len = ttllen  - sizeof(TExchHeader);
		return pExchHeader->nFuction;
	}

	int xfConnect::ResponseCommand(SYS_SOCKET sock,LPBYTE & pVoid,int &len)
	{
		char  pData[0x8000];
		int pos = 0;
		int nStart =0;
		bool bcontinue = false;
		bool bCompress = false;
		long nSrcLen;
		pVoid = NULL;
		len = 0;

continueread:
		pos = 0;
		ZeroMemory(pData + nStart,sizeof(pData)/sizeof(pData[0]) - nStart);
		errcode_ = 0;
L8:		int revLen = SysRecvData(sock,pData + nStart,sizeof(pData)/sizeof(pData[0]) - nStart,nDelay) ;
		if(revLen <0)
		{
			errcode_ = revLen;
			if(errcode_ == ERR_TIMEOUT)
			{
				if(AutoLogin(&sock)) goto L8;
				HWND hWnd = 0;
				if(xfApp::GetApp()->m_pFrame) hWnd = xfApp::GetApp()->m_pFrame->GetHWND();
				int ret = MessageBox(hWnd,L"数据接收超时，是否继续等待？",L"提示",MB_ABORTRETRYIGNORE);
				if(ret==IDRETRY)
					goto L8;
				else
				{
					connected_ = false;
					if(ret==IDABORT)
						ShellExecute(NULL, L"open", L"Start.bat", NULL, NULL, SW_HIDE);
					else
						return -1;
				}
			}else
				connected_ = false;
			/*if(MessageBox(NULL,L"数据接收出现错误，是否继续等待？",L"提示",MB_YESNO)==IDNO)
				//return revLen;
				return -1;
			else
				goto L8;*/
		}
		if(revLen > 0)
		{
continueprocess:
			revLen += nStart;
			if(revLen < 4)
			{
				//unfinish read pack size
				nStart += revLen;
				goto continueread;
			}
			nStart = 0;
			TExchHeader* pExchHeader = (TExchHeader *)pData;
			int ttllen = (int)((DWORD)pExchHeader->nPackSize) - (int)sizeof(TExchHeader);
			if(ttllen <0) return -1;
			
			BYTE * pBuf = NULL;
			if(ttllen>0)
			{
				pBuf = new BYTE[ttllen + 2];
				ZeroMemory(pBuf,ttllen + 2);

			}
			
			bool	bResponseError = false;
			if(pExchHeader->Flag & 0x80) bResponseError = true;
			if(pExchHeader->Flag & 0x01)
				bcontinue = true;
			else
				bcontinue = false;

			if(pExchHeader->Flag & 0x40)
			{
				bCompress = true;
				nSrcLen = pExchHeader->dwSrcLen;
			}
			else
				bCompress = false;


			BYTE nFuction = 0;
			nFuction = pExchHeader->nFuction;

			if(revLen > sizeof(TExchHeader))
			{
				if(revLen - (int)sizeof(TExchHeader) <= ttllen)
				{
					memcpy_s(pBuf + pos,revLen - sizeof(TExchHeader),pData + sizeof(TExchHeader) ,revLen - sizeof(TExchHeader));
					pos += revLen - sizeof(TExchHeader);
				}else
				{
					//read more than on pack;
					memcpy_s(pBuf + pos,ttllen - pos,pData + sizeof(TExchHeader),ttllen - pos );
					::memcpy(pData,pData + sizeof(TExchHeader) + (ttllen - pos),revLen - (ttllen - pos) - sizeof(TExchHeader));
					nStart = revLen - (ttllen - pos) - sizeof(TExchHeader);
					pos = ttllen;
					goto readfinishonepack;
				}
			}else
			{
				if(revLen == ttllen + sizeof(TExchHeader)) 
					goto readfinishonepack;
				nStart = revLen;
				goto continueread; //continue read until read finished one pack head;
			}

			//continue read data until one pack finished
			while(true)
			{
				if(pos >= (int)ttllen)break; 
				ZeroMemory(pData,sizeof(pData)/sizeof(pData[0]));
				errcode_ = 0;
L9:				revLen = SysRecvData(sock,pData,sizeof(pData)/sizeof(pData[0]),nDelay);
				if(revLen <0)
				{
					errcode_ = revLen;
					if(errcode_ == ERR_TIMEOUT)
					{
						if(AutoLogin(&sock)) goto L9;
						HWND hWnd = 0;
						if(xfApp::GetApp()->m_pFrame) hWnd = xfApp::GetApp()->m_pFrame->GetHWND();
						if(MessageBox(NULL,L"数据接收超时，是否继续等待？",L"提示",MB_YESNO)==IDNO)
						{
							connected_ = false;
							return -1;
						}
						else
							goto L9;
					}else
						connected_ = false;
				}
				if(revLen > 0)
				{
					if(revLen + pos <= ttllen)
					{
						memcpy_s(pBuf + pos,revLen,pData,revLen);
						pos += revLen;
					}
					else
					{
						//read more than on pack
						memcpy_s(pBuf + pos,ttllen - pos,pData,ttllen - pos );
						::memcpy(pData,pData + (ttllen - pos),revLen - (ttllen - pos));
						nStart = revLen - (ttllen - pos);
						pos = ttllen;
						break;
					}
				}else
					break;
			}
readfinishonepack:
			if(len==0)
			{
				pVoid = pBuf;
				len = ttllen;
			}else{
				LPBYTE pTemp = new BYTE[len + ttllen + 2];
				ZeroMemory(pTemp,len + ttllen + 2);

				::memcpy_s(pTemp,len,pVoid,len);
				::memcpy_s(pTemp+len,ttllen,pBuf,ttllen);
				delete pVoid;
				delete pBuf;
				pVoid = pTemp;
				len += ttllen;
			}
			if(nFuction == 2)xfApp::GetApp()->guid = pExchHeader->token; //auth ok
			if(bResponseError)
				return -1;
			else
			{
				if(!bcontinue)
				{
					if(bCompress && pVoid)
					{
						DWORD dwSize;
						pBuf = new BYTE[nSrcLen + 2];
						ZeroMemory(pBuf,nSrcLen + 2);
						dwSize = nSrcLen;
						Hxsoft::XUtil::UnCompress(pBuf,&dwSize,pVoid,len);
						delete pVoid;
						pVoid = pBuf;
						len = nSrcLen;
					}
					return nFuction;
				}
				else
				{
					pos = 0;
					revLen = 0;
					if(nStart <=1 )
					{
						//can't go to contineprocess,because can bala data had finished all data
						goto continueread;
					}
					//continue read data
					goto continueprocess;
				}
			}
		}else
		{
			CloseSocket(sock);
			return -1;
		}
	}

	bool xfConnect::AutoLogin(SYS_SOCKET * sock)
	{
		return false;
		if(!xfApp::GetApp()->m_pUser || !xfApp::GetApp()->m_pPassWord)
			return false;

		/*xfConnect * pConnect = xfApp::GetApp()->m_pConnect;
		if(pConnect)
		{
			pConnect->CloseSocket(pConnect->m_socket);
			delete pConnect;
			xfApp::GetApp()->m_pConnect = NULL;
			::ZeroMemory(&xfApp::GetApp()->guid,sizeof(GUID));
		}
		if(!pConnect)
			pConnect = new xfConnect();

		LPCTSTR pStrUrl = xfApp::GetApp()->GetServerUrl();
		if(!pStrUrl) return false; 

		DWORD dwSize;
		TCHAR strHostname[INTERNET_MAX_HOST_NAME_LENGTH ];
		TCHAR strPort[INTERNET_MAX_PORT_NUMBER_LENGTH];
		dwSize = INTERNET_MAX_HOST_NAME_LENGTH;
		UrlGetPart(pStrUrl,strHostname,&dwSize,URL_PART_HOSTNAME,URL_ESCAPE_UNSAFE);

		dwSize = INTERNET_MAX_PORT_NUMBER_LENGTH;
		if(UrlGetPart(pStrUrl,strPort,&dwSize,URL_PART_PORT,0)!=S_OK)
		::_tcscpy_s(strPort,INTERNET_MAX_PORT_NUMBER_LENGTH,_T("80"));

		SYS_SOCKET sock1 = pConnect->CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		SYS_INET_ADDR addr;
		NET_ADDRESS NetAddr;

		USES_CONVERSION;
		SysGetHostByName(T2A(strHostname), NetAddr);
		SysSetupAddress(addr,AF_INET,NetAddr,::_ttol(strPort));

		int nDelay = 5;
		bool result = SysConnect(sock1,&addr,sizeof(SYS_INET_ADDR),nDelay)==0?true:false;

		if(!result)
		//if(!pConnect->connectto(sock1,strHostname,::_ttol(strPort)))
		{
			return false;
		}
		pConnect->m_socket = sock1;
		sock = &sock1;
		xfApp::GetApp()->m_pConnect = pConnect;
		xfConnect * pConnect = xfUriLoader::GetConnect(xfApp::GetApp()->GetServerUrl(), true);
		if(!pConnect)
		{
			//MessageBox(NULL,_T("-2000 连接不到服务器!"),_T("提示"),MB_OK);
			return false;
		}
		sock = &pConnect->m_socket;

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

		return pConnect->SendAuthCommand(pConnect->m_socket,strLoginInfo);*/
	}

}}
