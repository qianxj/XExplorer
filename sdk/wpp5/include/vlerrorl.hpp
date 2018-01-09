//error text
WPP5Error( 0x10000001,	1,	err_text,					cn,		"%s")

//error for scan
WPP5Error( 0x10000010,	1,	err_token,					cn,		"error token")
WPP5Error( 0x10000011,	1,	err_doublescale,			cn,		"error read double scale")
WPP5Error( 0x10000012,	1,	err_scannumber,				cn,		"error scan number")
WPP5Error( 0x10000013,	1,	err_string,					cn,		"error string")

//token
WPP5Error( 0x10000020,	1,	err_needToken,				cn,		"��Ҫ'%s'")
WPP5Error( 0x10000021,	1,	err_needoperator,			cn,		"��Ҫ������ %s ")
WPP5Error( 0x10000022,	1,	err_needkn,					cn,		"��Ҫ���з�")
WPP5Error( 0x10000023,	1,	err_needsemicolon,			cn,		"��Ҫ ';'")
WPP5Error( 0x10000024,	1,	err_needrightbracket,		cn,		"��Ҫ ']'")
WPP5Error( 0x10000025,	1,	err_needrightbrace,			cn,		"��Ҫ '}'")
WPP5Error( 0x10000026,	1,	err_needleftbracket,		cn,		"��Ҫ '['")
WPP5Error( 0x10000027,	1,	err_needleftbrace,			cn,		"��Ҫ '{'")
WPP5Error( 0x10000028,	1,	err_needleftparen,			cn,		"��Ҫ '('")
WPP5Error( 0x10000029,	1,	err_needrightparen,			cn,		"��Ҫ ')'")
WPP5Error( 0x1000002A,	1,	err_needcomma,				cn,		"��Ҫ ','")
WPP5Error( 0x1000002B,	1,	err_needgt,					cn,		"��Ҫ '>'")
WPP5Error( 0x1000002C,	1,	err_needcolon,				cn,		"��Ҫ ':'")

//mistoken
WPP5Error( 0x10000030,	1,	err_needvarname,			cn,		"��Ҫ������")
WPP5Error( 0x10000031,	1,	err_needname,				cn,		"��Ҫ����")
WPP5Error( 0x10000032,	1,	err_needident,				cn,		"��Ҫ��־��")
WPP5Error( 0x10000033,	1,	err_needcommenttrail,		cn,		"��Ҫ */")
WPP5Error( 0x10000034,	1,	err_stringtrail,			cn,		"��Ҫ�ַ���������")
WPP5Error( 0x10000035,	1,	err_needtype,				cn,		"��Ҫ������")
WPP5Error( 0x10000036,	1,	err_needkeyword,			cn,		"��Ҫ�ؼ���: %s")
WPP5Error( 0x10000037,	1,	err_needdeclarestmt,		cn,		"��Ҫ�������")
WPP5Error( 0x10000038,	1,	err_needstring,				cn,		"��Ҫ�ַ���")
//unknown
WPP5Error( 0x10000040,	1,	err_unvaliddataitem,		cn,		"��Ч��������")
WPP5Error( 0x10000041,	1,	err_unvalidcommand,			cn,		"��Ч������ %s")
WPP5Error( 0x10000042,	1,	err_unknownconst,			cn,		"����ʶ�ĳ���")
WPP5Error( 0x10000043,	1,	err_unknownstmt,			cn,		"����ʶ�ľ���")
WPP5Error( 0x10000044,	1,	err_unvalidoperator,		cn,		"��Ч�Ĳ�����")

//Excess
WPP5Error( 0x10000050,	1,	err_notneedkeyword,			cn,		"����Ҫ�ؼ���")
WPP5Error( 0x10000051,	1,	err_unnecessarytoken,		cn,		"����ķ���")

//check
//type error
WPP5Error( 0x10000060,	1,	err_undefinedtype,			cn,		"δ��������: %s")
WPP5Error( 0x10000061,	1,	err_type_op,				cn,		"%s�Ĳ������ʹ���")
WPP5Error( 0x10000062,	1,	err_method_arg,				cn,		"%s �������ò�������")
WPP5Error( 0x10000063,	1,	err_needcategoryint,		cn,		"��Ҫ������������")
WPP5Error( 0x10000064,	1,	err_needclassorinteface,	cn,		"��Ҫ���ӿ�")
WPP5Error( 0x10000065,	1,	err_unvalidtype,			cn,		"��Ч�����")
WPP5Error( 0x10000066,	1,	err_needtypeindent,			cn,		"���Ͷ���ȱ������")
WPP5Error( 0x10000067,	1,	err_needclassorstruct,		cn,		"��Ҫ������")
WPP5Error( 0x10000068,	1,	err_needmethod,				cn,		"��Ҫ����")
WPP5Error( 0x10000069,	1,	err_usedforclassmethod,		cn,		"%sֻ�����ڷǾ�̬���ຯ��")
WPP5Error( 0x1000006A,	1,	err_neednumbertype,			cn,		"��Ҫ��������")

//axis error
WPP5Error( 0x10000070,	1,	err_needclassornamespace,	cn,		"��Ҫ��������ռ�")
WPP5Error( 0x10000071,	1,	err_neednamespace,			cn,		"��Ҫ�����ռ�")
//not have defined for use or repeat define
WPP5Error( 0x10000072,	1,	err_undefinedvar,			cn,		"δ�����־��: %s")
WPP5Error( 0x10000073,	1,	err_undefinedlang,			cn,		"δ��������: %s")
WPP5Error( 0x10000074,	1,	err_muchdefine,				cn,		"����Ķ���")
WPP5Error( 0x10000075,	1,	err_unknownmethod,			cn,		"δ���庯��%s")
WPP5Error( 0x10000076,	1,	err_undeftype,				cn,		"δ�������� %s")
WPP5Error( 0x10000077,	1,	err_existfield,				cn,		" %s �ֶ��Ѵ���")
WPP5Error( 0x10000078,	1,	err_existvar,				cn,		" %s �����Ѵ���")
WPP5Error( 0x10000079,	1,	err_existtype,				cn,		" %s �����Ѵ���")
WPP5Error( 0x1000007A,	1,	err_existmethod,			cn,		" %s �����Ѵ���")
WPP5Error( 0x1000007B,	1,	err_hasdefinedfield,		cn,		" %s �Ѷ�������ֶ�")
WPP5Error( 0x1000007C,	1,	err_hasdefinedarg,			cn,		" %s �Ѷ���ɲ���")
WPP5Error( 0x1000007D,	1,	err_needfield,				cn,		"%s���Ǻ��ʵ����ֶ�")
WPP5Error( 0x1000007E,	1,	err_askclassornamespace,	cn,		"%s������������ռ�")

//need left value
WPP5Error( 0x10000080,	1,	err_needleftval,			cn,		"ֻ����ֵ��������%s����")
WPP5Error( 0x10000081,	1,	err_needleftvalex,			cn,		"ֻ����ֵ�������ܲ���")

//arry
WPP5Error( 0x10000090,	1,	err_needarraytype,			cn,		"��Ҫ��������")
WPP5Error( 0x10000091,	1,	err_needconstintrank,		cn,		"��Ҫ��������")

//warning	type group list
WPP5Error( 0x10001010,	1,	err_needtypegroupindent,	cn,		"�����鶨��ȱ������")
WPP5Error( 0x10001011,	1,	err_typegroup_needitem,		cn,		"��Ҫ��������")
WPP5Error( 0x10001012,	1,	err_typegroup_needitemval,	cn,		"��Ҫ���������ֵ")
WPP5Error( 0x10001013,	1,	err_typegroupval_ref,		cn,		"�������� \"ref\" ֻ���� true|false ")
WPP5Error( 0x10001014,	1,	err_typegroupval_copy,		cn,		"��������: \"copy\" ֻ���� true|false ")
WPP5Error( 0x10001015,	1,	err_typegroupval_array,		cn,		"��������: \"array\" ֻ���� true|false ")
WPP5Error( 0x10001016,	1,	err_typegroupval_fctpure,	cn,		"�������� \"function.pure\" ֻ���� true|false ")
WPP5Error( 0x10001017,	1,	warn_typegroup_unvaditem,	cn,		"��Ч����������\"%s\"")

//voc
WPP5Error( 0x10001040,	1,	err_needvocinstace,			cn,		"��Ҫ�ʻ�ʵ��")
WPP5Error( 0x10001041,	1,	err_vocundefpred,			cn,		"�ʻ�δ����%sν��")
WPP5Error( 0x10001042,	1,	err_casterror,				cn,		"��������޷�ת��")

//process error
WPP5Error( 0x10001060,	1,	err_actionshift,			cn,		"shift error")
WPP5Error( 0x10001061,	1,	err_actionreduce,			cn,		"reduce conflict")
WPP5Error( 0x10001062,	1,	err_actionshiftreduce,		cn,		"shift reduce conflict")
WPP5Error( 0x10001063,	1,	err_actionunknown,			cn,		"action unknown error")

