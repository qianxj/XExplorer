// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XOFOBJECT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XOFOBJECT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XOFOBJECT_EXPORTS
#define XOFOBJECT_API __declspec(dllexport)
#else
#define XOFOBJECT_API __declspec(dllimport)
#endif

#include "xgriddef.hpp"
#include "xgridoption.hpp"
#include "iaxissvr.hpp"
#include "xrowselector.hpp"
#include "xcellheader.hpp"
#include "icontentsvr.hpp"
#include "ixmlcontentsvr.hpp"
#include "xxmlcontentsvr.hpp"
#include "xgriddatasvr.hpp"
#include "xgridsvr.hpp"

#include "idwview.hpp"
#include "IDwViewObServer.hpp"
