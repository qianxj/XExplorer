// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef VL_EXPORTS
#define VL_API __declspec(dllexport)
#else
#define VL_API __declspec(dllimport)
#endif

#if !defined(tchar)
#define tchar wchar_t
#endif

#if !defined(null)
#define null 0
#endif

#if !defined(_T)
#define _T(x) L##x
#endif
