// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XPOINT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XPOINT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XSHOW_EXPORTS
#define XSHOW_API __declspec(dllexport)
#else
#define XSHOW_API __declspec(dllimport)
#endif

#include "xshowsheet.hpp"
#include "xshowsheetsvr.hpp"
#include "xshowstylesvr.hpp"
#include "xshowdatasvr.hpp"
#include "XShowSerializeSvr.hpp"