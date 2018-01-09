#pragma once

#include "xbxbase.h"
#include "itrans.hpp"

namespace Hxsoft{ namespace XFrame
{
	class  XFRAME_API  IXAServer : public xbObject
	{
	public:
		IXAServer():m_pTrans(NULL){};
	public:
		//读取开发的对象,pObject should in( 'xpage','entity','sheet','workflow','worknode','xquery','xaction'
		static int GetDevObject(LPCTSTR pStrServer,LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml); 
		static int GetDevObject(ITrans * pTrans,LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml); 
		int GetDevObject(LPCTSTR pObject,LPCTSTR pObjGuid,xbXml &xml); 
	
	public:
		//执行服务命令
		static LPTSTR ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr, bool needlogin=false);
		static int ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len);
		static int ProcessCmd(ITrans * pTrans,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len);
		int ProcessCmd(LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len);
		int ProcessCmd(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,BYTE * &pData,int &len, HWND hWnd);
		//执行服务命令并装载到xml对象中
		static int FetchXml(LPCTSTR pStrServer,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml);
		static int FetchXml(ITrans * pTrans,LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml);
		int FetchXml(LPCTSTR pStrCmd,LPCTSTR pExtStr,xbXml &xml);
	
	public:
		//执行服务命令并装载到xml对象中
		static int LoadUrl(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache=true);
		static int LoadUrl(ITrans * pTrans,LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache=true);
		int LoadUrl(LPCTSTR pUrl,LPCTSTR pExtStr,BYTE * &pData,int &len,bool bCache=true);
		//执行服务命令并装载到xml对象中
		static int LoadUrl(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml);
		static int LoadUrl(ITrans * pTrans,LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml);
		int LoadUrl(LPCTSTR pUrl,LPCTSTR pExtStr,xbXml &xml);
		//读取XPage
		static int LoadXPage(LPCTSTR pStrServer,LPCTSTR pStrUrl,xbXml &xml);
		static int LoadXPage(ITrans * pTrans,LPCTSTR pStrUrl,xbXml &xml);
		int LoadXPage(LPCTSTR pStrUrl,xbXml &xml);
	
	public:
		//执行XQuery命令
		static int ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		static int ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);
		static int ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		static int ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);

		static int ExecXQuery(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs,int &val);
		static int ExecXAction(LPCTSTR pStrServer,LPCTSTR pObjGuid,LPCTSTR pArgs);

		static int ExecXQuery(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		static int ExecXQuery(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);
		static int ExecXAction(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		static int ExecXAction(ITrans * pTrans,LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);

		int ExecXQuery(LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		int ExecXQuery(LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);
		int ExecXAction(LPCTSTR pObjGuid,LPCTSTR pArgs,BYTE * &pData,int &len);
		int ExecXAction(LPCTSTR pObjGuid,LPCTSTR pArgs,xbXml &xml);

		static LPCTSTR ExecArgs(LPCTSTR pArgs);
	public:
		//重新连接
		static int ReConnect(LPCTSTR pStrServer);
		static int ReConnect(ITrans * pTrans);
		int ReConnect();
	public:
		//得到用户
		static LPCTSTR GetUserNo();
		static LPCTSTR GetVersion();
		static LPWSTR GetComputerNameEx();
		static LPWSTR GetAppPath();
		static int SetUID(LPWSTR uid);
		static LPWSTR GetUID();
		static LPWSTR GetMacAddress();
		static LPWSTR GetIpAddress();
		static LPWSTR GetConnectId();
		static int SetLoginLog(LPCTSTR pStrServer,LPWSTR pStatus);		
		static void SetDataToClip(LPCTSTR DataStr);

	public:
		//设置交易对象
		ITrans * m_pTrans;
		int SetTrans(ITrans *);
		ITrans * GetTrans();

		static int UploadEx(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource=NULL,BOOL bCompress=false);
		static int Upload(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource=NULL,BOOL bCompress=false);
		static int Upload(LPTSTR pUri,LPTSTR pName,LPTSTR pPath,HWND m_hWnd,LPTSTR pSource=NULL,BOOL bCompress=false);

		static int FileCopy(LPTSTR pUri,LPTSTR pName,LPTSTR pUri1,LPTSTR pName1);
		static LPTSTR UploadFile(LPTSTR pUri,LPTSTR pName,HWND m_hWnd,LPTSTR pSource,BOOL bCompress);
		static int __DeleteFile(LPTSTR pName);
		static int __CreateDirectory(LPTSTR pName);
		static int DownLoadFile(LPCTSTR pStrServer,LPCTSTR pUrl,LPCTSTR pExt,LPTSTR pName=NULL);

		static int UploadData(LPTSTR pUri,LPTSTR pName,LPTSTR pData);
	};
}}