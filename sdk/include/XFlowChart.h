// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XFLOWCHART_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XFLOWCHART_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XFLOWCHART_EXPORTS
#define XFLOWCHART_API __declspec(dllexport)
#else
#define XFLOWCHART_API __declspec(dllimport)
#endif

#include "xflowsheetsvr.hpp"
#include "xflowsheet.hpp"
#include "XFlowChartObject.hpp"
#include "IFlowChartEvent.hpp"
#include "XFlowSheet.hpp"
#include "XFlowSheetSvr.hpp"
#include "XFlowChartDataSvr.hpp"
#include "XFlowChartContentSvr.hpp"
