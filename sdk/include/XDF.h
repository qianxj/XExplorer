// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XPOINT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XPOINT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XDF_EXPORTS
#define XDF_API __declspec(dllexport)
#else
#define XDF_API __declspec(dllimport)
#endif

#include "XDFsheet.hpp"
#include "XDFsheetsvr.hpp"
#include "XDFdatasvr.hpp"
#include "XDFSerializeSvr.hpp"