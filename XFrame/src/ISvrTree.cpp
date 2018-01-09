#include "StdAfx.h"
#include "ISvrTree.hpp"
#include "xfUriLoader.hpp"

//树操作URI模板:
//uri.hxsoft.com/xaserver/tree:[treeobject]/[tree command]
static LPTSTR CMDSTR_BASE         =  _T("uri.hxsoft.com/xaserver/tree");  //基本树操作uri地址
static LPTSTR CMDSTR_UPDATEITEM   =  _T("update");  //更新命令
static LPTSTR CMDSTR_NEWITEM      =  _T("new"); //增加命令
static LPTSTR CMDSTR_DELETEITEM   =  _T("delete"); //树删除命令
static LPTSTR CMDSTR_GETITEM      =  _T("getitem"); //得到节点数据包含子节点
static LPTSTR CMDSTR_GETITEMEX    =  _T("getitemex");//得到节点数据不包含子节点
static LPTSTR CMDSTR_INSERTROOT   =  _T("insertroot"); //插入根节点
static LPTSTR CMDSTR_MOVEITEM     =  _T("move"); //移动节点位置

//follow is server tree command
//int AddItem(LPTSTR pEntity,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPTSTR pStrItem);
//int UpdateItem(LPTSTR pEntity,LPCTSTR pItemGuid,LPTSTR pStrItem);
//int DeleteItem(LPTSTR pEntity,LPCTSTR pItemGuid);
//int MoveItem(LPTSTR pEntity,LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPTSTR pStrItem);
//int GetItem(LPTSTR pEntity,LPCTSTR pItemGuid);
//int GetItemEx(LPTSTR pEntity,LPCTSTR pItemGuid);
//int InsertRoot(LPTSTR pEntity,LPTSTR pStrItem);

//服务端树操作对象
namespace Hxsoft{ namespace XFrame
{
	ISvrTree::ISvrTree():m_pTreeObject(NULL),m_pServerUrl(NULL)
	{
	}
	ISvrTree::ISvrTree(LPCTSTR pTreeObject,LPCTSTR pServerUrl):m_pTreeObject(pTreeObject),m_pServerUrl(pServerUrl)
	{
	}
	ISvrTree::~ISvrTree()
	{
		if( m_pTreeObject)delete m_pTreeObject;
		if( m_pServerUrl) delete m_pServerUrl;
	}

	//更新操作
	int ISvrTree::UpdateItem(LPCTSTR pItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_UPDATEITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//插入根操作
	int ISvrTree::InsertRoot(LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_INSERTROOT);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//插入项操作
	int ISvrTree::InsetrItem(LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, 
			pParentItemGuid,pBeforItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//增加项操作
	int ISvrTree::AddItem(LPCTSTR pParentItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, pParentItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//增加子项操作
	int ISvrTree::AddSubItem(LPCTSTR pParentItemGuid, LPCTSTR pStrItem, xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, pParentItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//删除操作
	int ISvrTree::DeleteItem(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_DELETEITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
	//移动项操作
	int ISvrTree::MoveItem(LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s %s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_MOVEITEM, 
			pItemGuid, pParentItemGuid,pBeforItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//得到项数据，包含子项
	int ISvrTree::GetItem(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_GETITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
	//得到项数据
	int ISvrTree::GetItemEx(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_GETITEMEX, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
}}