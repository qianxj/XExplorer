#pragma once
#include <vector>
#include "XWordDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	/**
		��ʾ�����������Ű�Ŀռ�
	*/
	class XFlowItem
	{
	public:
		XFlowItem(void);
		~XFlowItem(void);
	public:
		//�ⲿ����
		RECT FRect;
		//�����ⲿ����
		int SetFRect(RECT &rc);
		//��ȡ�ⲿ����
		const RECT & GetFRect();
	public:
		//��������
		RECT FContentRect;
		//������������
		int SetContentRect(RECT &rc);
		//��ȡ��������
		const RECT & GetContentRect();
	public:
		//�����ų�����ľ�����
		std::vector<RECT> m_exRects;
		//�����ų�����
		int AddExRect(RECT &rc){m_exRects.push_back(rc); return (int)m_exRects.size();}
		//����ų�����
		int ClearExRect();

		//�����ų����������
		HRGN m_eRgn;
	public:
		//��ʾ���ռ������
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL)=0;
	public:
		//��ȡ�趨�߶ȺͿ����Ϣ
		virtual POINT GetExtent()=0;
		//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
		virtual POINT GetRealExtent()=0;
		//���¼���λ����Ϣ
		virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC)=0;
	public:
		//ֱ���и߶�
		int m_nRowHeight;
		//�м��
		std::vector<int> m_spRowHeights;
		//�о�
		std::vector<int> m_RowHeights;
		//�õ�����ʼλ��
		POINT GetRowPosition(int nRow);
	};
}}}}