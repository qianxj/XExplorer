// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XCONTROL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XCONTROL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XCONTROL_EXPORTS
#define XCONTROL_API __declspec(dllexport)
#else
#define XCONTROL_API __declspec(dllimport)
#endif

#include "xcpropsheet.hpp"
#include "xcToolButton.hpp"
#include "xctreectrl.hpp"
#include "xctreedatasource.hpp"
#include "xctreexmldatasource.hpp"
//#include "xcwindesign.hpp"
#include "xcWCControl.hpp"
#include "xcSpliterBar.hpp"
#include "xcLabel.hpp" 
#include "xcLayerSheet.hpp"
#include "xcAgentEdit.hpp"
#include "xcSourceEdit.hpp"
#include "xcOleControlContain.hpp"
#include "xsEdit.hpp"
#include "xcimageview.hpp"
#include "ixsedit.hpp"
#include "ilayersheet.hpp"
#include "xcshtml.hpp"
#include "ixshtml.hpp"
#include "xcFile.hpp"
#include "xcPages.hpp"
