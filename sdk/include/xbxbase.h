// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XBASE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XBASE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XBASE_EXPORTS
#define XBASE_API __declspec(dllexport)
#else
#define XBASE_API __declspec(dllimport)
#endif

#include "xbxml.hpp"
#include "xbcache.hpp"
#include "xberror.hpp"
#include "xbevent.hpp"
#include "xbObserver.hpp"
#include "xbobject.hpp"
#include "xbobjectlog.hpp"
#include "xbobjectmgr.hpp"
#include "xbobserver.hpp"
#include "xbcomobject.hpp"
#include "vxIObject.hxx"

