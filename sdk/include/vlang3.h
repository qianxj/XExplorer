/***
*vlang3.h - definitions and declarations for math library
*
*       Copyright (c) Hxsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains constant definitions and external subroutine
*       declarations for the math subroutine library.
*       [ANSI/System V]
*
*       [Public]
*
****/

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VLANG3_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VLANG3_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VLANG3_EXPORTS
#define VLANG3_API __declspec(dllexport)
#else
#define VLANG3_API __declspec(dllimport)
#endif
