/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

//error list
VL_error( 0x10000001,	1,	err_needkn,					cn,		"��Ҫ���з�")
VL_error( 0x10000002,	1,	err_needsemicolon,			cn,		"��Ҫ ';'")
VL_error( 0x10000003,	1,	err_needrightbracket,		cn,		"��Ҫ ']'")
VL_error( 0x10000004,	1,	err_needrightbrace,			cn,		"��Ҫ '}'")
VL_error( 0x10000005,	1,	err_needleftbracket,		cn,		"��Ҫ '['")
VL_error( 0x10000006,	1,	err_needleftbrace,			cn,		"��Ҫ '{'")
VL_error( 0x10000007,	1,	err_muchdefine,				cn,		"����Ķ���")
VL_error( 0x10000008,	1,	err_needoperator,			cn,		"��Ҫ������")
VL_error( 0x10000009,	1,	err_needleftparen,			cn,		"��Ҫ '('")
VL_error( 0x1000000A,	1,	err_needrightparen,			cn,		"��Ҫ ')'")
VL_error( 0x1000000B,	1,	err_needcomma,				cn,		"��Ҫ ','")
VL_error( 0x1000000C,	1,	err_needgt,					cn,		"��Ҫ '>'")
VL_error( 0x1000000D,	1,	err_needcolon,				cn,		"��Ҫ ':'")

VL_error( 0x10000010,	1,	err_token,					cn,		"error token")
VL_error( 0x10000011,	1,	err_doublescale,			cn,		"error read double scale")
VL_error( 0x10000012,	1,	err_scannumber,				cn,		"error scan number")
VL_error( 0x10000013,	1,	err_string,					cn,		"error string")
VL_error( 0x10000014,	1,	err_undefinedtype,			cn,		"δ��������: %s")
VL_error( 0x10000015,	1,	err_needvarname,			cn,		"��Ҫ������")
VL_error( 0x10000016,	1,	err_unvalidtype,			cn,		"��Ч�����")
VL_error( 0x10000017,	1,	err_existtype,				cn,		" %s �����Ѵ���")
VL_error( 0x10000018,	1,	err_needname,				cn,		" ��Ҫ����")
VL_error( 0x10000019,	1,	err_needindent,				cn,		" ��Ҫ��־��")
VL_error( 0x1000001A,	1,	err_undefinedvar,			cn,		"δ�����־��: %s")
VL_error( 0x1000001B,	1,	err_undefinedlang,			cn,		"δ��������: %s")

VL_error( 0x10000020,	1,	err_needcommenttrail,		cn,		"��Ҫ */")
VL_error( 0x10000021,	1,	err_stringtrail,			cn,		"��Ҫ�ַ���������")
VL_error( 0x10000022,	1,	err_unvaliddataitem,		cn,		"��Ч��������")
VL_error( 0x10000023,	1,	err_unvalidcommand,			cn,		"��Ч������ %s")
VL_error( 0x10000024,	1,	err_unkownconst,			cn,		"����ʶ�ĳ���")
VL_error( 0x10000025,	1,	err_unkownstmt,				cn,		"����ʶ�ľ���")
VL_error( 0x10000026,	1,	err_needkeyword,			cn,		"��Ҫ�ؼ���: %s")
VL_error( 0x10000027,	1,	err_needtype,				cn,		"��Ҫ������")
VL_error( 0x10000028,	1,	err_notneedkeyword,			cn,		"����Ҫ�ؼ���")
VL_error( 0x10000029,	1,	err_needdeclarestmt,		cn,		"��Ҫ�������")
VL_error( 0x1000002A,	1,	err_needclassornamespace,	cn,		"��Ҫ��������ռ�")
VL_error( 0x1000002B,	1,	err_undeftype,				cn,		"δ�������� %s")
VL_error( 0x1000002C,	1,	err_needclassorinteface,	cn,		"��Ҫ���ӿ�")
VL_error( 0x1000002D,	1,	err_unknownstmt,			cn,		"����ʶ�ľ���")
VL_error( 0x1000002E,	1,	err_unknownmethod,			cn,		"δ���庯�� %s")
VL_error( 0x1000002F,	1,	err_needstring,				cn,		"��Ҫ�ַ���")
VL_error( 0x10000030,	1,	err_needclassorstruct,		cn,		"��Ҫ������")

VL_error( 0x10000031,	1,	err_type_cast,				cn,		"%s")
VL_error( 0x10000032,	1,	err_type_op,				cn,		"%s�Ĳ������ʹ���")
VL_error( 0x10000033,	1,	err_method_arg,				cn,		"%s �������ò�������")
VL_error( 0x10000034,	1,	err_needcategoryint,		cn,		"��Ҫ������������")
VL_error( 0x10000035,	1,	err_needleftval,			cn,		"ֻ����ֵ��������%s����")

//type group
VL_error( 0x10001001,	1,	err_needtypegroupindent,	cn,		"�����鶨��ȱ������")
VL_error( 0x00001002,	1,	err_typegroup_needitem,		cn,		"��Ҫ��������")
VL_error( 0x00001003,	1,	err_typegroup_needitemval,	cn,		"��Ҫ���������ֵ")

VL_error( 0x00001008,	1,	err_typegroupval_ref,		cn,		"�������� \"ref\" ֻ���� true|false ")
VL_error( 0x00001009,	1,	err_typegroupval_copy,		cn,		"��������: \"copy\" ֻ���� true|false ")
VL_error( 0x0000100A,	1,	err_typegroupval_array,		cn,		"��������: \"array\" ֻ���� true|false ")
VL_error( 0x0000100B,	1,	err_typegroupval_fctpure,	cn,		"�������� \"function.pure\" ֻ���� true|false ")

//warning	type group list
VL_error(  0x00001001,	0,	warn_ypegroup_unvaditem,	cn,		"��Ч����������\"%s\"")

//type list
VL_error( 0x10001101,	1,	err_needtypeindent,			cn,		"���Ͷ���ȱ������")
