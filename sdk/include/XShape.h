// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XSHAPE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XSHAPE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XSHAPE_EXPORTS
#define XSHAPE_API __declspec(dllexport)
#else
#define XSHAPE_API __declspec(dllimport)
#endif

#include "XShapeObject.hpp"
#include "XShapeDrawObject.hpp"
#include "XShapeObjectSetItem.hpp"
#include "XShapeObjectSet.hpp"
#include "XShapeMgr.hpp"

