#pragma once

#include "systypes.h"
#include "sysdep.h"
#include "xbxbase.h"
#include "xfareaobject.hpp"
namespace Hxsoft{ namespace XFrame
{
	typedef struct tagExchHeader
	{
		DWORD nPackSize;
		BYTE Flag;      //0 bit flag iscontinue to receive data
		//7 bit flag iserror,
		//6 bit flag compress;
		//1 bit flag token;
		BYTE nFuction; // 0 normal reponse 
		// 1 auther respones token string
		// 2 auther respones token string
		// 3 notify information
		// 4 chat information
		// 5 mail protocol 
		// 6 http protocol
		WORD wReserved;
		DWORD Sequence; //Sequence same as requeste
		DWORD dwSrcLen; //used compress
		GUID  token;
		tagExchHeader(){::ZeroMemory(this,sizeof(tagExchHeader));}
	}TExchHeader;

	class XFRAME_API xfConnect :
		public xfAreaObject
	{
	public:
		xfConnect(void);
		~xfConnect(void);
	public:
		SYS_SOCKET	m_socket;
	public:
		SYS_SOCKET CreateSocket(int iAddressFamily = AF_INET, int iType=SOCK_STREAM, int iProtocol=IPPROTO_TCP);
		void CloseSocket(SYS_SOCKET	sock);
	public:
		bool connectto(SYS_SOCKET sock,LPTSTR host,int port);
		bool connectto(SYS_SOCKET sock,LPTSTR host,int port,int delay);
		bool SendCommand(SYS_SOCKET sock,LPCTSTR comstr);
		bool SendCommand(SYS_SOCKET sock,LPCTSTR comstr,LPCTSTR extStr);
		bool SendCommand(SYS_SOCKET sock,LPCTSTR comstr,LPVOID extStr,int nDataLen,bool bcompress = true);
		bool SendAuthCommand(SYS_SOCKET sock,LPCTSTR AuthStr);
		bool AutoLogin(SYS_SOCKET * sock);
		int ResponseCommand(SYS_SOCKET sock,LPBYTE & pVoid,int &len);
		int ResponseCommand(SYS_SOCKET sock,HWND hWnd);
		int ReadOnePack(SYS_SOCKET sock,TExchHeader& exchHeader,LPBYTE & pVoid,int &len);
	private:
		bool connected_;
		int	 errcode_;
	public:
		bool IsConnected();

	};
}}
