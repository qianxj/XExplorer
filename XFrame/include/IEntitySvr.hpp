#pragma once

#include "IXAServer.hpp"

namespace Hxsoft{ namespace XFrame
{
	class  XFRAME_API  IEntitySvr : public IXAServer
	{
		//ʵ�����ݡ��鵵��ת��ʷ���ɰ桢�ݸ塢�����䡢����
		#define	EB_DATA		0x0000		//���� 
		#define	EB_SCRIPT	0x0001		//�ݸ�
		#define	EB_DELETED	0x0002		//������
		#define	EB_EBOLDVER	0x0004		//������

		#define	EB_DOCUMENT		0x0100		//�鵵
		#define	EB_HISTORY		0x0200		//ת��ʷ
		#define	EB_LONGHISTORY	0x0400		//תԶ����ʷ,һ��ͳ��Ҳ�����õ�

		#define	EB_SUSPEND	0x1000		//����,������������
		#define	EB_AUTO		0x2000		//�ڸ����ļ������Զ�����ʵ������,���ڵõ�����ʵ������

		//����״̬
		#define ES_NEW 0	0x0001
		#define ES_MODIFY	0x0002
		#define ES_CHECKING	0x0003
		#define ES_CHECKED  0x0004
		#define ES_FLOWING	0x0100
		#define ES_FINISHED 0x0200

		//�ע
		#define EN_AllNOTE	0x0000	//ȫ���ע
		#define EN_NOTE		0x0001	//�ע
		#define EN_CHKNOTE	0x0002	//��������ʾ
		#define EN_STARTOWNERNOTE 0x0010	//��ʼ�Զ�����

	public:
		IEntitySvr();
	public:
		//��ȡʵ���б�ģ��
		static int GetEntityListTemplate(LPCTSTR pServer,LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		static int GetEntityListTemplate(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);

		static int GetEntityListTemplate(ITrans * pTrans,LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		static int GetEntityListTemplate(ITrans * pTrans,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);
		
		int GetEntityListTemplate(LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		int GetEntityListTemplate(LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);

		//��ȡʵ���б�����
		static int GetEntityListData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);
		static int GetEntityListData(LPCTSTR pServer,LPTSTR pEntityName,int nMode,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);

		static int GetEntityListData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);
		static int GetEntityListData(ITrans * pTrans,LPTSTR pEntityName,int nMode,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);

		int GetEntityListData(LPTSTR pEntityName,int nMode,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);
		int GetEntityListData(LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox=EB_DATA);

		//��ȡʵ���б�����
		static int GetEntityListAgent(LPCTSTR pServer,LPCTSTR pWkUrl,xbXml &xml);
		static int GetEntityListAgent(LPCTSTR pServer,LPTSTR pEntityName,int nMode,xbXml &xml);
		static LPTSTR  GetEntityListAgent(LPCTSTR pServer,LPTSTR pEntityName,int nMode);
	
		static int GetEntityListAgent(ITrans * pTrans,LPCTSTR pWkUrl,xbXml &xml);
		static int GetEntityListAgent(ITrans * pTrans,LPTSTR pEntityName,int nMode,xbXml &xml);
		static LPTSTR  GetEntityListAgent(ITrans * pTrans,LPTSTR pEntityName,int nMode);

		int GetEntityListAgent(LPCTSTR pWkUrl,xbXml &xml);
		int GetEntityListAgent(LPTSTR pEntityName,int nMode,xbXml &xml);
		LPTSTR  GetEntityListAgent(LPTSTR pEntityName,int nMode);


		//��ȡʵ��༭ģ��
		static int GetEntityDoc(LPCTSTR pServer,LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		static int GetEntityDoc(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);

		static int GetEntityDoc(ITrans * pTrans,LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		static int GetEntityDoc(ITrans * pTrans,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);

		int GetEntityDoc(LPCTSTR pWkUrl,IXMLDOMElement * &pElement);
		int GetEntityDoc(LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement);

		//��ȡʵ��༭����
		static int GetEntityData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);
		static int GetEntityData(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);

		static int GetEntityData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);
		static int GetEntityData(ITrans * pTrans,LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);

		int GetEntityData(LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);
		int GetEntityData(LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);

		//����ʵ������
		static int SaveEntityData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);
		static int SaveEntityData(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);

		static int SaveEntityData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);
		static int SaveEntityData(ITrans * pTrans,LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);

		int SaveEntityData(LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);
		int SaveEntityData(LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox=EB_AUTO);

		//ɾ��ʵ��
		static int DeleteEntity(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pEntityKey,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);
		
		static int DeleteEntity(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pEntityKey,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);
		
		int DeleteEntity(LPCTSTR pEntityName,LPCTSTR pEntityKey,DWORD dwBox=EB_AUTO,DWORD Version=0,DWORD SubVersion=0);


		//�õ�ʵ��GUID
		static int GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl,BSTR & pEntity,int & nMode);
		static LPTSTR  GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl,int & nMode);
		static LPTSTR  GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl);

		static int GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl,BSTR & pEntity,int & nMode);
		static LPTSTR  GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl,int & nMode);
		static LPTSTR  GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl);

		int GetEntityName(LPCTSTR pWkUrl,BSTR & pEntity,int & nMode);
		LPTSTR  GetEntityName(LPCTSTR pWkUrl,int & nMode);
		LPTSTR  GetEntityName(LPCTSTR pWkUrl);

		//��ȡ�汾��
		static int GetEntityVersion(LPCTSTR pServer,DWORD &Version,DWORD &SubVersion);
		static int GetEntityVersion(ITrans * pTrans,DWORD &Version,DWORD &SubVersion);
		int GetEntityVersion(DWORD &Version,DWORD &SubVersion);

		//����״̬
		static int SetEntityStatus(LPCTSTR pServer,LPCTSTR pEntityName,int Status,int SubStatus);

		static int SetEntityStatus(ITrans * pTrans,LPCTSTR pEntityName,int Status,int SubStatus);
		int SetEntityStatus(LPCTSTR pEntityName,int Status,int SubStatus);

		//תĿ¼
		static int EntityToBox(LPCTSTR pServer,LPCTSTR pEntityName,DWORD dwBox);
		
		static int EntityToBox(ITrans * pTrans,LPCTSTR pEntityName,DWORD dwBox);
		int EntityToBox(LPCTSTR pEntityName,DWORD dwBox);

		//�޸�����,һ��������������Ҫ�޸ĵ�����
		//�����޸�
		static int AddItemModify(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int AddItemModify(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int AddItemModify(LPCTSTR pEntityName,LPCTSTR pXPath,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);

		//ɾ����
		static int DeletedItem(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pModifyStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int DeletedItem(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pModifyStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int DeletedItem(LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pModifyStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		
		//ɾ����
		static int InsertItem(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int InsertItem(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int InsertItem(LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		
		//�ָ�
		static int UndoItem(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int UndoItem(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int UndoItem(LPCTSTR pEntityName,LPCTSTR pXPath,LPCTSTR pXmlStr,bool bAutoCreateOldVersion=false,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);

		//�ע
		static int AddItemNote(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int GetItemNoteCount(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int DeletedItemNote(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,int nIndex,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);// nIndex=-1,ȫ��ɾ��

		static int AddItemNote(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int GetItemNoteCount(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		static int DeletedItemNote(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,int nIndex,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);// nIndex=-1,ȫ��ɾ��

		int AddItemNote(LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int GetItemNoteCount(LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,LPCTSTR pNoteStr,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);
		int DeletedItemNote(LPCTSTR pEntityName,LPCTSTR pXPath,int nNoteType,int nIndex,LPCTSTR pUser=NULL,LPCTSTR pData=NULL);// nIndex=-1,ȫ��ɾ��

		//����ͬ��
		static int SyncEntity(LPCTSTR pServer,LPCTSTR pEntityName,LPCTSTR pRemoteSvr,LPTSTR pRemoteUser,LPCTSTR pStrXslt = NULL,DWORD dwNoteType=-1);
		static int SyncEntity(ITrans * pTrans,LPCTSTR pEntityName,LPCTSTR pRemoteSvr,LPTSTR pRemoteUser,LPCTSTR pStrXslt = NULL,DWORD dwNoteType=-1);
		int SyncEntity(LPCTSTR pEntityName,LPCTSTR pRemoteSvr,LPTSTR pRemoteUser,LPCTSTR pStrXslt = NULL,DWORD dwNoteType=-1);
	public:
		//���ý��׶���
		ITrans * m_pTrans;
		int SetTrans(ITrans *);
		ITrans * GetTrans();
	};
}}