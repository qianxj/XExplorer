#pragma once

class XWordText
{
public:
	XWordText(void);
	~XWordText(void);
public:
	//�õ�ָ��λ�õ��ı�
	LPCTSTR getText(int nOffset);
public:
	//�����ַ�
	int insertChar(wchar_t ch);
	//ɾ��һ���ַ�
	int deleteChar(int nOffset);
public:
	//�õ�ָ���ַ���nOffset��ʼ���ֵ�λ��
	int getCharPosition(int nOffset);
};
