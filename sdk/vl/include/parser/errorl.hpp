/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

//error list
VL_error( 0x10000001,	1,	err_needkn,					cn,		"需要换行符")
VL_error( 0x10000002,	1,	err_needsemicolon,			cn,		"需要 ';'")
VL_error( 0x10000003,	1,	err_needrightbracket,		cn,		"需要 ']'")
VL_error( 0x10000004,	1,	err_needrightbrace,			cn,		"需要 '}'")
VL_error( 0x10000005,	1,	err_needleftbracket,		cn,		"需要 '['")
VL_error( 0x10000006,	1,	err_needleftbrace,			cn,		"需要 '{'")
VL_error( 0x10000007,	1,	err_muchdefine,				cn,		"多余的定义")
VL_error( 0x10000008,	1,	err_needoperator,			cn,		"需要操作符")
VL_error( 0x10000009,	1,	err_needleftparen,			cn,		"需要 '('")
VL_error( 0x1000000A,	1,	err_needrightparen,			cn,		"需要 ')'")
VL_error( 0x1000000B,	1,	err_needcomma,				cn,		"需要 ','")
VL_error( 0x1000000C,	1,	err_needgt,					cn,		"需要 '>'")
VL_error( 0x1000000D,	1,	err_needcolon,				cn,		"需要 ':'")

VL_error( 0x10000010,	1,	err_token,					cn,		"error token")
VL_error( 0x10000011,	1,	err_doublescale,			cn,		"error read double scale")
VL_error( 0x10000012,	1,	err_scannumber,				cn,		"error scan number")
VL_error( 0x10000013,	1,	err_string,					cn,		"error string")
VL_error( 0x10000014,	1,	err_undefinedtype,			cn,		"未定义类型: %s")
VL_error( 0x10000015,	1,	err_needvarname,			cn,		"需要变量名")
VL_error( 0x10000016,	1,	err_unvalidtype,			cn,		"无效的类别")
VL_error( 0x10000017,	1,	err_existtype,				cn,		" %s 类型已存在")
VL_error( 0x10000018,	1,	err_needname,				cn,		" 需要名称")
VL_error( 0x10000019,	1,	err_needindent,				cn,		" 需要标志符")
VL_error( 0x1000001A,	1,	err_undefinedvar,			cn,		"未定义标志符: %s")
VL_error( 0x1000001B,	1,	err_undefinedlang,			cn,		"未定义语言: %s")

VL_error( 0x10000020,	1,	err_needcommenttrail,		cn,		"需要 */")
VL_error( 0x10000021,	1,	err_stringtrail,			cn,		"需要字符串结束符")
VL_error( 0x10000022,	1,	err_unvaliddataitem,		cn,		"无效的数据项")
VL_error( 0x10000023,	1,	err_unvalidcommand,			cn,		"无效的命令 %s")
VL_error( 0x10000024,	1,	err_unkownconst,			cn,		"不认识的常数")
VL_error( 0x10000025,	1,	err_unkownstmt,				cn,		"不认识的句子")
VL_error( 0x10000026,	1,	err_needkeyword,			cn,		"需要关键字: %s")
VL_error( 0x10000027,	1,	err_needtype,				cn,		"需要类型名")
VL_error( 0x10000028,	1,	err_notneedkeyword,			cn,		"不需要关键字")
VL_error( 0x10000029,	1,	err_needdeclarestmt,		cn,		"需要申明语句")
VL_error( 0x1000002A,	1,	err_needclassornamespace,	cn,		"需要类或命名空间")
VL_error( 0x1000002B,	1,	err_undeftype,				cn,		"未定义类型 %s")
VL_error( 0x1000002C,	1,	err_needclassorinteface,	cn,		"需要类或接口")
VL_error( 0x1000002D,	1,	err_unknownstmt,			cn,		"不认识的句子")
VL_error( 0x1000002E,	1,	err_unknownmethod,			cn,		"未定义函数 %s")
VL_error( 0x1000002F,	1,	err_needstring,				cn,		"需要字符串")
VL_error( 0x10000030,	1,	err_needclassorstruct,		cn,		"需要类类型")

VL_error( 0x10000031,	1,	err_type_cast,				cn,		"%s")
VL_error( 0x10000032,	1,	err_type_op,				cn,		"%s的操作类型错误")
VL_error( 0x10000033,	1,	err_method_arg,				cn,		"%s 函数调用参数错误")
VL_error( 0x10000034,	1,	err_needcategoryint,		cn,		"需要整形类别的数据")
VL_error( 0x10000035,	1,	err_needleftval,			cn,		"只有左值变量才能%s操作")

//type group
VL_error( 0x10001001,	1,	err_needtypegroupindent,	cn,		"类型组定义缺少名称")
VL_error( 0x00001002,	1,	err_typegroup_needitem,		cn,		"需要类型组项")
VL_error( 0x00001003,	1,	err_typegroup_needitemval,	cn,		"需要类型组项的值")

VL_error( 0x00001008,	1,	err_typegroupval_ref,		cn,		"类型组项 \"ref\" 只能是 true|false ")
VL_error( 0x00001009,	1,	err_typegroupval_copy,		cn,		"类型组项: \"copy\" 只能是 true|false ")
VL_error( 0x0000100A,	1,	err_typegroupval_array,		cn,		"类型组项: \"array\" 只能是 true|false ")
VL_error( 0x0000100B,	1,	err_typegroupval_fctpure,	cn,		"类型组项 \"function.pure\" 只能是 true|false ")

//warning	type group list
VL_error(  0x00001001,	0,	warn_ypegroup_unvaditem,	cn,		"无效的类型组项\"%s\"")

//type list
VL_error( 0x10001101,	1,	err_needtypeindent,			cn,		"类型定义缺少名称")
