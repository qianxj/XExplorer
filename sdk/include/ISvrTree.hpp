#pragma once

#include "IXAServer.hpp"

namespace Hxsoft{ namespace XFrame
{
	//服务端树操作对象
	class  XFRAME_API  ISvrTree : public IXAServer
	{
	public:
		ISvrTree();
		ISvrTree(LPCTSTR pTreeObject,LPCTSTR pServerUrl);
		~ISvrTree();

		LPCTSTR m_pTreeObject; //服务端操作的对象树
		LPCTSTR m_pServerUrl; //服务器连接的Url
	public:
		//更新操作
		int UpdateItem(LPCTSTR pItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//插入根操作
		int InsertRoot(LPCTSTR pStrItem,xbXml & xml);
		//插入项操作
		int InsetrItem(LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//增加项操作
		int AddItem(LPCTSTR pParentItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//增加子项操作
		int AddSubItem(LPCTSTR pParentItemGuid, LPCTSTR pStrItem, xbXml & xml);
		//删除操作
		int DeleteItem(LPCTSTR pItemGuid,xbXml & xml);
		//移动项操作
		int MoveItem(LPCTSTR pItemGuid,LPCTSTR pParentItemGuid,LPCTSTR pBeforItemGuid,LPCTSTR pStrItem,xbXml & xml);
		//得到项数据，包含子项
		int GetItem(LPCTSTR pItemGuid,xbXml & xml);
		//得到项数据
		int GetItemEx(LPCTSTR pItemGuid,xbXml & xml);
	};
}}