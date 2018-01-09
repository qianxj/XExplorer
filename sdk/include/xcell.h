#ifdef XCELL_EXPORTS
#define XCELL_API __declspec(dllexport)
#else
#define XCELL_API __declspec(dllimport)
#endif

#include "xcelldef.hpp"
#include "AddrSvr.hpp"
#include "AxisSvr.hpp"
#include "CellSvr.hpp"
#include "ColHeaderSvr.hpp"
#include "ContentSvr.hpp"
#include "DataStoreSvr.hpp"
#include "EditSvr.hpp"
#include "FormatSvr.hpp"
#include "FormulaSvr.hpp"
#include "GridFrameSvr.hpp"
#include "NameSvr.hpp"
#include "NoteSvr.hpp"
#include "OptionSvr.hpp"
#include "RowSelectorSvr.hpp"
#include "XCellSerializeSvr.hpp"
#include "XCellSheetSvr.hpp"
#include "XCellSheet.hpp"
#include "SpanRectSvr.hpp"
#include "TemplDefSvr.hpp"
#include "dwtable.hpp"
#include "idwtable.hpp"
#include "IDwTableObServer.hpp"

#include "xgridsheetex.hpp"
#include "xgridreport.hpp"
#include "xgridheaderex.hpp"
#include "xgridrowselector.hpp"
#include "xgriddatasvrex.hpp"
#include "xgridsheetsvrex.hpp"

#include "IGridReport.hpp"
