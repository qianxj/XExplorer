// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XREPORT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XREPORT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XREPORT_EXPORTS
#define XREPORT_API __declspec(dllexport)
#else
#define XREPORT_API __declspec(dllimport)
#endif

#include "xreportsheet.hpp"
#include "xreportsheetsvr.hpp"
#include "xreportdatasvr.hpp"
#include "XreportSerializeSvr.hpp"
#include "IReportEvent.hpp"