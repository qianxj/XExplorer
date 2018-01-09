#pragma once

#include "IXAServer.hpp"

namespace Hxsoft{ namespace XFrame
{
	//���������������
	class  XFRAME_API  ISvrTree : public IXAServer
	{
	public:
		ISvrTree();
		ISvrTree(LPCTSTR pTreeObject,LPCTSTR pServerUrl);
		~ISvrTree();

		LPCTSTR m_pTreeObject; //����˲����Ķ�����
		LPCTSTR m_pServerUrl; //���������ӵ�Url
	public:
		//���²���
		int UpdateItem(LPCTSTR pItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//���������
		int InsertRoot(LPCTSTR pStrItem,xbXml & xml);
		//���������
		int InsetrItem(LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//���������
		int AddItem(LPCTSTR pParentItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//�����������
		int AddSubItem(LPCTSTR pParentItemGuid, LPCTSTR pStrItem, xbXml & xml);
		//ɾ������
		int DeleteItem(LPCTSTR pItemGuid,xbXml & xml);
		//�ƶ������
		int MoveItem(LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//�õ������ݣ���������
		int GetItem(LPCTSTR pItemGuid,xbXml & xml);
		//�õ�������
		int GetItemEx(LPCTSTR pItemGuid,xbXml & xml);
	};
}}