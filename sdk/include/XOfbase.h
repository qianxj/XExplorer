// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XOFFICE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XOFFICE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifdef XOFFICE_EXPORTS
#define XOFFICE_API __declspec(dllexport)
#else
#define XOFFICE_API __declspec(dllimport)
#endif

#include "svrbase.hpp"
#include "xtabsheet.hpp"
#include "xscrollbar.hpp"
#include "xnavigate.hpp"
#include "XOFBaseFrmsvr.hpp"
#include "XOFBaseFrm.hpp"
#include "XOfficeSheet.hpp"
#include "XOfficeSheetsvr.hpp"
#include "XOfficeData.hpp"
#include "XOfficeSerializeSvr.hpp"
#include "XOfficeStyleSvr.hpp"




