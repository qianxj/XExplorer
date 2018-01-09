#include "StdAfx.h"
#include "ISvrTree.hpp"
#include "xfUriLoader.hpp"

//������URIģ��:
//uri.hxsoft.com/xaserver/tree:[treeobject]/[tree command]
static LPTSTR CMDSTR_BASE         =  _T("uri.hxsoft.com/xaserver/tree");  //����������uri��ַ
static LPTSTR CMDSTR_UPDATEITEM   =  _T("update");  //��������
static LPTSTR CMDSTR_NEWITEM      =  _T("new"); //��������
static LPTSTR CMDSTR_DELETEITEM   =  _T("delete"); //��ɾ������
static LPTSTR CMDSTR_GETITEM      =  _T("getitem"); //�õ��ڵ����ݰ����ӽڵ�
static LPTSTR CMDSTR_GETITEMEX    =  _T("getitemex");//�õ��ڵ����ݲ������ӽڵ�
static LPTSTR CMDSTR_INSERTROOT   =  _T("insertroot"); //������ڵ�
static LPTSTR CMDSTR_MOVEITEM     =  _T("move"); //�ƶ��ڵ�λ��

//follow is server tree command
//int AddItem(LPTSTR pEntity,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPTSTR pStrItem);
//int UpdateItem(LPTSTR pEntity,LPCTSTR pItemGuid,LPTSTR pStrItem);
//int DeleteItem(LPTSTR pEntity,LPCTSTR pItemGuid);
//int MoveItem(LPTSTR pEntity,LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPTSTR pStrItem);
//int GetItem(LPTSTR pEntity,LPCTSTR pItemGuid);
//int GetItemEx(LPTSTR pEntity,LPCTSTR pItemGuid);
//int InsertRoot(LPTSTR pEntity,LPTSTR pStrItem);

//���������������
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

	//���²���
	int ISvrTree::UpdateItem(LPCTSTR pItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_UPDATEITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//���������
	int ISvrTree::InsertRoot(LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_INSERTROOT);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//���������
	int ISvrTree::InsetrItem(LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, 
			pParentItemGuid,pBeforItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//���������
	int ISvrTree::AddItem(LPCTSTR pParentItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, pParentItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//�����������
	int ISvrTree::AddSubItem(LPCTSTR pParentItemGuid, LPCTSTR pStrItem, xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_NEWITEM, pParentItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//ɾ������
	int ISvrTree::DeleteItem(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_DELETEITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
	//�ƶ������
	int ISvrTree::MoveItem(LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s %s %s %s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_MOVEITEM, 
			pItemGuid, pParentItemGuid,pBeforItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,pStrItem,xml);
	}
	//�õ������ݣ���������
	int ISvrTree::GetItem(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_GETITEM, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
	//�õ�������
	int ISvrTree::GetItemEx(LPCTSTR pItemGuid,xbXml & xml)
	{
		TCHAR strCmd[512];
		_stprintf_s(strCmd,512,_T("%s:%s/%s"),CMDSTR_BASE,m_pTreeObject,CMDSTR_GETITEMEX, pItemGuid);
		return xfUriLoader::FetchXml(m_pServerUrl,strCmd,NULL,xml);
	}
}}