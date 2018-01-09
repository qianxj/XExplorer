	xsm_error(  0x00001001,	0,	warn_ypegroup_unvaditem,		cn,		"无效的类型组项\"%s\"")

//error list
xsm_error( 0x10000001,	1,	__atom_err_needkn,				cn,		"需要换行符")
xsm_error( 0x10000002,	1,	__atom_err_needsemicolon,			cn,		"需要 ';'")
xsm_error( 0x10000003,	1,	__atom_err_needrightbracket,			cn,		"需要 ']'")
xsm_error( 0x10000004,	1,	__atom_err_needrightbrace,			cn,		"需要 '}'")
xsm_error( 0x10000005,	1,	__atom_err_needleftbracket,			cn,		"需要 '['")
xsm_error( 0x10000006,	1,	__atom_err_needleftbrace,			cn,		"需要 '{'")
xsm_error( 0x10000007,	1,	__atom_err_muchdefine,				cn,		"多余的定义")
xsm_error( 0x10000008,	1,	__atom_err_needoperator,			cn,		"需要操作符")
xsm_error( 0x10000009,	1,	__atom_err_leftparen,				cn,		"需要 '('")
xsm_error( 0x1000000A,	1,	__atom_err_rightparen,				cn,		"需要 ')'")
xsm_error( 0x1000000B,	1,	__atom_err_needcomma,			cn,		"需要 ','")
xsm_error( 0x1000000C,	1,	__atom_err_needgt,				cn,		"需要 '>'")
xsm_error( 0x1000000D,	1,	__atom_err_need_token,				cn,		"需要 '%c'")
xsm_error( 0x1000000E,	1,	__atom_err_need_indent,			cn,		"需要标志符")
xsm_error( 0x1000000F,	1,	__atom_err_muchtoken,				cn,		"多余的符号")

xsm_error( 0x10000010,	1,	__atom_err_token,					cn,		"error token")
xsm_error( 0x10000011,	1,	__atom_err_doublescale,			cn,		"error read double scale")
xsm_error( 0x10000012,	1,	__atom_err_scannumber,			cn,		"error scan number")
xsm_error( 0x10000013,	1,	__atom_err_string,				cn,		"error string")
xsm_error( 0x10000014,	1,	__atom_err_undefinedtype,			cn,		"未定义类型: %s")
xsm_error( 0x10000015,	1,	__atom_err_needname,				cn,		"需要变量名")
xsm_error( 0x10000016,	1,	__atom_err_unvalidtype,			cn,		"无效的类别")
xsm_error( 0x10000017,	1,	__atom_err_existtype,				cn,		" %s 类型已存在")

xsm_error( 0x10000018,	1,	__atom_err_need_indent_notkey,		cn,		" %s需要标志符而不是关键字")
xsm_error( 0x10000019,	1,	__atom_err_need_declarestmt,		cn,		"不是声明语句")
xsm_error( 0x1000001A,	1,	__atom_err_need_key_namespace,		cn,		"需要namespace")
xsm_error( 0x1000001B,	1,	__atom_err_need_keyword,			cn,		"需要关键字%s")

xsm_error( 0x10000021,	1,	__atom_err_need_int,		cn,		"只有整数类型才能进行 %s 操作")
xsm_error( 0x10000022,	1,	__atom_err_need_bool,		cn,		"只有bool类型才能进行 %s 操作")
xsm_error( 0x10000023,	1,	__atom_err_need_number,	cn,		"只有数字类型才能进行 %s 操作")
xsm_error( 0x10000024,	1,	__atom_err_need_type,		cn,		"需要类型")
xsm_error( 0x10000025,	1,	__atom_err_need_var,		cn,		"%s 不是变量名")
xsm_error( 0x10000026,	1,	__atom_err_nest_need_classornamespace,	 cn,	"嵌套名称要求类名或命名空间")
xsm_error( 0x10000027,	1,	__atom_err_nest_need_member,	 cn,	"需要成员变量或函数")
xsm_error( 0x10000028,	1,	__atom_err_nest_need_memberorvar,	 cn,	"需要变量、成员变量或函数")
xsm_error( 0x10000029,	1,	__atom_err_nest_need_classname,	 cn,	"需要类名称")
xsm_error( 0x1000002A,	1,	__atom_err_nest_invalid_memberorvar,	 cn,	"%s不是变量或成员变量或函数")
xsm_error( 0x1000002B,	1,	__atom_err_nest_invalid_this,	 cn,	"这里不能使用this")
xsm_error( 0x1000002C,	1,	__atom_err_ref_need_class,	 	 cn,	"引用需要类类型")
xsm_error( 0x1000002D,	1,	__atom_err_pointer_need_class,	 cn,	"指针需要类类型")

xsm_error( 0x1000002E,	1,	__atom_err_type_cast,	 cn,	"%s")
xsm_error( 0x1000002E,	1,	__atom_err_type_op,	 cn,	"%s的操作类型错误")
xsm_error( 0x1000002F,	1,	__atom_err_unknown_stmt,	 cn,	"不认识的句子")

xsm_error( 0x10000030,	1,	__atom_err_need_string,	 cn,	"需要字符串")
xsm_error( 0x10000031,	1,	__atom_err_need_function_body,	 cn,	"需要函数体")
xsm_error( 0x10000032,	1,	__atom_err_need_array,	 cn,	"需要数组类型")
xsm_error( 0x10000033,	1,	__atom_err_error_array_dim,	 cn,	"参数个数与数组维数不符")
xsm_error( 0x10000034,	1,	__atom_err_need_indentex,	 cn,	"需要标志符%s")

xsm_error( 0x10000035,	1,	__atom_err_not_namespace_indent,	 cn,	"%s不是命名空间")

//type list
xsm_error( 0x10001101,	1,	__atom_err_needtypeindent,			cn,		"类型定义缺少名称")
xsm_error( 0x10001101,	2,	__atom_err_method_arg,			cn,		"%s 函数调用参数错误")

//undef
xsm_error( 0x10001201,	1,	__atom_err_undef_var,				cn,		"%s 变量未定义")
xsm_error( 0x10001202,	1,	__atom_err_undef_method,				cn,		"%s 函数未定义")
xsm_error( 0x10001203,	1,	__atom_err_undef_arrayvar,				cn,		"%s 数组变量未定义")
xsm_error( 0x10001204,	1,	__atom_err_undef_type,				cn,		"%s 类型未定义")
xsm_error( 0x10001205,	1,	__atom_err_need_classorinteface,		cn,		"%s 需要是类和接口才能继承")
