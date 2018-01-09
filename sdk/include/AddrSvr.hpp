#pragma once
#include "xofbase.h"
#include <algorithm>
#include <limits>
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{

	// The typedefs
	typedef INT32 XCROW;
	typedef INT32 XCCOL;
	typedef INT32 XCTAB;
	typedef	INT32 XCCOLROW;     // a type capable of holding either XCCOL or XCROW

	// temporarily signed typedefs
	typedef INT32 XCsROW;
	typedef INT32 XCsCOL;
	typedef INT XCsTAB;
	typedef INT XCsCOLROW;

	// size_t typedef to be able to find places where code was changed from USHORT
	// to size_t and is used to read/write from/to streams.
	typedef size_t XCSIZE;

	// Maximum possible value of data type, NOT maximum row value.
	// MSC confuses numeric_limit max() with macro max() if vcl/wintypes.hxx is
	// included, we should not be using those stupid macros anyway.
	#undef min
	#undef max
	const XCROW    XCROW_MAX    = ::std::numeric_limits<XCROW>::max();
	const XCCOL    XCCOL_MAX    = ::std::numeric_limits<XCCOL>::max();
	const XCTAB    XCTAB_MAX    = ::std::numeric_limits<XCTAB>::max();
	const XCCOLROW XCCOLROW_MAX = ::std::numeric_limits<XCCOLROW>::max();
	const XCSIZE   XCSIZE_MAX   = ::std::numeric_limits<XCSIZE>::max();


	// The maximum values. Defines are needed for preprocessor checks in
	// bcaslot.cxx, otherwise type safe constants are preferred.
	#define MAXROWCOUNT_DEFINE XCROW_MAX//65536
	#define MAXCOLCOUNT_DEFINE 256

	// Count values
	const XCROW       MAXROWCOUNT    = MAXROWCOUNT_DEFINE;
	const XCCOL       MAXCOLCOUNT    = MAXCOLCOUNT_DEFINE;
	const XCTAB       MAXTABCOUNT    = 256;
	const XCCOLROW    MAXCOLROWCOUNT = MAXROWCOUNT;
	// Maximum values
	const XCROW       MAXROW         = MAXROWCOUNT - 1;
	const XCCOL       MAXCOL         = MAXCOLCOUNT - 1;
	const XCTAB       MAXTAB         = MAXTABCOUNT - 1;
	const XCCOLROW    MAXCOLROW      = MAXROW;


	// Special values
	const XCTAB XC_TAB_APPEND     = XCTAB_MAX;
	const XCTAB TABLEID_DOC       = XCTAB_MAX;  // entire document, e.g. protect
	const XCROW XCROWS32K         = 32000;
	const XCCOL XCCOL_REPEAT_NONE = XCCOL_MAX;
	const XCROW XCROW_REPEAT_NONE = XCROW_MAX;

	class XCELL_API CAddrSvr :public CSvrbase
	{
	public:
		CAddrSvr(void){};
		~CAddrSvr(void){};
	};
}}}}
