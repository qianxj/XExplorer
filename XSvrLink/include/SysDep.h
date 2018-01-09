/*
 *  XMail by Davide Libenzi ( Intranet and Internet mail server )
 *  Copyright (C) 1999,..,2004  Davide Libenzi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#ifndef _SYSDEP_H
#define _SYSDEP_H

#define SYS_SHUTDOWN_SOFT           0

#define LOG_LEV_DEBUG               0
#define LOG_LEV_MESSAGE             1
#define LOG_LEV_WARNING             2
#define LOG_LEV_ERROR               3

#define SYS_PRIORITY_LOWER          -1
#define SYS_PRIORITY_NORMAL         0
#define SYS_PRIORITY_HIGHER         +1

#define SYS_THREAD_ATTACH           1
#define SYS_THREAD_DETACH           2

#define SYS_MMAP_READ               (1 << 0)
#define SYS_MMAP_WRITE              (1 << 1)

#define SYS_IS_VALID_FILENAME(f)    ((strcmp(f, ".") != 0) && (strcmp(f, "..") != 0))

enum SysFileTypes {
	ftNormal = 1,
		ftDirectory,
		ftLink,
		ftOther,

		ftMax
};

struct SYS_FILE_INFO {
	int iFileType;
	SYS_OFF_T llSize;
	time_t tMod;
};

extern "C"
{
	int WINAPI SysInitLibrary(void);
	void WINAPI  SysCleanupLibrary(void);
	int WINAPI SysAddThreadExitHook(void (*pfHook)(void *, SYS_THREAD, int), void *pPrivate);
	int WINAPI SysShutdownLibrary(int iMode = SYS_SHUTDOWN_SOFT);

	int WINAPI SysSetupSocketBuffers(int *piSndBufSize, int *piRcvBufSize);
	SYS_SOCKET WINAPI  SysCreateSocket(int iAddressFamily, int iType, int iProtocol);
	void WINAPI SysCloseSocket(SYS_SOCKET SockFD);
	int WINAPI SysBlockSocket(SYS_SOCKET SockFD, int iBlocking);
	int WINAPI SysBindSocket(SYS_SOCKET SockFD, const struct sockaddr *SockName, int iNameLen);
	void SysListenSocket(SYS_SOCKET SockFD, int iConnections);
	int WINAPI  SysRecvData(SYS_SOCKET SockFD, char *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysRecv(SYS_SOCKET SockFD, char *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysRecvDataFrom(SYS_SOCKET SockFD, struct sockaddr *pFrom, int iFromlen,
				char *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysSendData(SYS_SOCKET SockFD, char const *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysSend(SYS_SOCKET SockFD, char const *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysSendDataTo(SYS_SOCKET SockFD, const struct sockaddr *pTo,
			  int iToLen, char const *pszBuffer, int iBufferSize, int iTimeout);
	int WINAPI  SysConnect(SYS_SOCKET SockFD, const SYS_INET_ADDR *pSockName, int iNameLen, int iTimeout);
	SYS_SOCKET SysAccept(SYS_SOCKET SockFD, SYS_INET_ADDR *pSockName, int *iNameLen, int iTimeout);
	int WINAPI SysSelect(int iMaxFD, SYS_fd_set *pReadFDs, SYS_fd_set *pWriteFDs, SYS_fd_set *pExcptFDs,
			  int iTimeout);
	int WINAPI  SysSendFile(SYS_SOCKET SockFD, char const *pszFileName, SYS_OFF_T llBaseOffset,
			SYS_OFF_T llEndOffset, int iTimeout);
	int WINAPI SysSetupAddress(SYS_INET_ADDR &AddrInfo, int iFamily,
				NET_ADDRESS const &NetAddr, int iPortNo);
	int WINAPI SysGetAddrAddress(SYS_INET_ADDR const &AddrInfo, NET_ADDRESS &NetAddr);
	int WINAPI SysGetAddrPort(SYS_INET_ADDR const &AddrInfo);
	int WINAPI SysSetAddrAddress(SYS_INET_ADDR &AddrInfo, NET_ADDRESS const &NetAddr);
	int WINAPI SysSetAddrPort(SYS_INET_ADDR &AddrInfo, int iPortNo);
	int WINAPI SysGetHostByName(char const *pszName, NET_ADDRESS &NetAddr);
	int WINAPI SysGetHostByAddr(SYS_INET_ADDR const &AddrInfo, char *pszFQDN, int iSize);
	int WINAPI SysGetPeerInfo(SYS_SOCKET SockFD, SYS_INET_ADDR &AddrInfo);
	int WINAPI SysGetSockInfo(SYS_SOCKET SockFD, SYS_INET_ADDR &AddrInfo);
	char *SysInetNToA(SYS_INET_ADDR const &AddrInfo, char *pszIP);
	int WINAPI SysInetAddr(char const *pszDotName, NET_ADDRESS &NetAddr);
	int WINAPI SysSameAddress(NET_ADDRESS const &NetAddr1, NET_ADDRESS const &NetAddr2);

	SYS_SEMAPHORE SysCreateSemaphore(int iInitCount, int iMaxCount);
	int WINAPI SysCloseSemaphore(SYS_SEMAPHORE hSemaphore);
	int WINAPI SysWaitSemaphore(SYS_SEMAPHORE hSemaphore, int iTimeout);
	int WINAPI SysReleaseSemaphore(SYS_SEMAPHORE hSemaphore, int iCount);
	int WINAPI SysTryWaitSemaphore(SYS_SEMAPHORE hSemaphore);

	SYS_MUTEX SysCreateMutex(void);
	int WINAPI SysCloseMutex(SYS_MUTEX hMutex);
	int WINAPI SysLockMutex(SYS_MUTEX hMutex, int iTimeout);
	int WINAPI SysUnlockMutex(SYS_MUTEX hMutex);
	int WINAPI SysTryLockMutex(SYS_MUTEX hMutex);

	SYS_EVENT SysCreateEvent(int iManualReset);
	int WINAPI SysCloseEvent(SYS_EVENT hEvent);
	int WINAPI SysWaitEvent(SYS_EVENT hEvent, int iTimeout);
	int WINAPI SysSetEvent(SYS_EVENT hEvent);
	int WINAPI SysResetEvent(SYS_EVENT hEvent);
	int WINAPI SysTryWaitEvent(SYS_EVENT hEvent);

	SYS_THREAD SysCreateThread(unsigned int (*pThreadProc) (void *), void *pThreadData);
	void SysCloseThread(SYS_THREAD ThreadID, int iForce);
	int WINAPI SysSetThreadPriority(SYS_THREAD ThreadID, int iPriority);
	int WINAPI SysWaitThread(SYS_THREAD ThreadID, int iTimeout);
	unsigned long SysGetCurrentThreadId(void);
	int WINAPI SysExec(char const *pszCommand, char const *const *pszArgs, int iWaitTimeout = 0,
			int iPriority = SYS_PRIORITY_NORMAL, int *piExitStatus = NULL);
	void SysSetBreakHandler(void (*BreakHandler) (void));

	int WINAPI SysCreateTlsKey(SYS_TLSKEY &TlsKey, void (*pFreeProc) (void *) = NULL);
	int WINAPI SysDeleteTlsKey(SYS_TLSKEY &TlsKey);
	int WINAPI SysSetTlsKeyData(SYS_TLSKEY &TlsKey, void *pData);
	void *SysGetTlsKeyData(SYS_TLSKEY &TlsKey);

	void SysThreadOnce(SYS_THREAD_ONCE *pThrOnce, void (*pOnceProc) (void));

	void *SysAllocNZ(unsigned int uSize);
	void *SysAlloc(unsigned int uSize);
	void SysFree(void *pData);
	void *SysRealloc(void *pData, unsigned int uSize);

	int WINAPI SysEventLogV(int iLogLevel, char const *pszFormat, va_list Args);
	int WINAPI SysEventLog(int iLogLevel, char const *pszFormat, ...);
	int WINAPI SysLogMessage(int iLogLevel, char const *pszFormat, ...);
	void SysSleep(int iTimeout);
	void SysMsSleep(int iMsTimeout);
	SYS_INT64 SysMsTime(void);
	char *SysStrDup(const char *pszString);
	char *SysGetEnv(const char *pszVarName);

	int WINAPI SysVSNPrintf(char *pszBuffer, int iSize, char const *pszFormat, va_list Args);
	int WINAPI SysFileSync(FILE *pFile);

	char *SysStrTok(char *pszData, char const *pszDelim, char **ppszSavePtr);
	char *SysCTime(time_t *pTimer, char *pszBuffer, int iBufferSize);
	struct tm *SysLocalTime(time_t *pTimer, struct tm *pTStruct);
	struct tm *SysGMTime(time_t *pTimer, struct tm *pTStruct);
	char *SysAscTime(struct tm *pTStruct, char *pszBuffer, int iBufferSize);
	long SysGetTimeZone(void);
	long SysGetDayLight(void);

	
	//add by qianxj
	int WINAPI MscGetServerAddress(char const *pszServer, SYS_INET_ADDR &SvrAddr, int iPortNo);

}
#endif

