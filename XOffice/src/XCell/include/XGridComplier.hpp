#pragma once

#include "windows.h"
#include "tchar.h"
#include <vector>
#include "XGridReport.hpp"
#include "GridFrameSvr.hpp"
#include "DwTable.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell{

class CXGridComplier
{
protected:
	enum xtpy{fn, op, var, other, undef};
	enum xfn{sum, sumex, row, getrow, rowcount, col, getcol, getitemstring, format, user, left, right, xif, xabs, xnot,sround};
	enum xop{add, sub, mul, div, leftb, rightb, sadd, eq, ne, lt, le, gt, ge, xand, xor};
	enum xvar{xstr, xfloat, xint, xbool, xcol};
	enum xother{ns, comma};
	enum xbox{xgrid, xcell, none};

	struct item
	{
		LPTSTR	pStr;
		double   fValue;
		int		iValue;
		bool	bValue;
		xtpy	nTpy;
		int		nNo;
	};

public:
	CXGridComplier();           
	~CXGridComplier();

private:
	LPTSTR	_Source;
	int		_SourceLen;

	int 	_curPos;
	TCHAR	_curChar;

	std::vector<item>* _expStep;
	std::vector<item>* _opStep;

	void _NextChar(void);
	void Parse(void);
	void Calc(void);

	bool _isFn(LPTSTR pStr, xfn * fn);
	bool _isOp(LPTSTR pStr, xop * op);

	int _getLev(int op);

	//double _StrToFloat(LPTSTR pStr, bool bDelete=true);
	//LPTSTR _FloatToStr(double pValue, bool bInt=false);
	bool _toNumber(item * xItem, bool bDelete=true);
	bool _toNumber(LPTSTR pStr, xvar &t, int &r1, double &r3);
	bool _toStr(item * xItem);
	bool _toBool(item * xItem);

	int _error;
	void _setError(int errid);
	bool _hasError(int errid, int errTpy);

	xbox _FBox;
	class XGridReport* _FGrid;
	class CGridFrameSvr* _FCell;
	int _FRowInit;
	int _FColInit;
	int _FRow;
	int _FCol;

	int __getRowCount();
	int __getRow();
	int __getCurRow();
	LPTSTR __getItemString(int row, LPTSTR colname);
	LPTSTR __getTotalString(const LPTSTR totalType, LPTSTR colname);
	CDwTable* __getTableByName(CDwTable * pTable, LPTSTR name);
	CDwTable* __getTable();
	HWND __getHWND();

	LPTSTR _askData(LPTSTR pStr);

public:
	bool SetSource(LPTSTR szSource, void* grid=NULL, int box=0, int row=-1, int col=-1);
	
	LPTSTR GetError();

	int GetInt();
	double GetFloat();
	LPTSTR GetString();
};
}}}}