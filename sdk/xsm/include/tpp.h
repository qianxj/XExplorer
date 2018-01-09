// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TPP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TPP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef TPP_EXPORTS
#define TPP_API __declspec(dllexport)
#else
#define TPP_API __declspec(dllimport)
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