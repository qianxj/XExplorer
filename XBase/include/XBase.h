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

// �����Ǵ� XBase.dll ������
class XBASE_API CXBase {
public:
	CXBase(void);
	// TODO: �ڴ�������ķ�����
};

extern XBASE_API int nXBase;

XBASE_API int fnXBase(void);
