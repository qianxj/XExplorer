#include "stdafx.h"
#include "XGridComplier.hpp"
#include "stdio.h"
#include "math.h"
//#include "optionsvr.hpp"
//#include <atlbase.h>
//#include <atlconv.h>
#include "xuXUtil.h"
#include "XGridSheetSvrEx.hpp"

#define DWV_ASKDATA	 0x0023

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell{

CXGridComplier::CXGridComplier():_expStep(NULL), _opStep(NULL), _error(0), _FGrid(NULL), _FCell(NULL)
{

}

CXGridComplier::~CXGridComplier()
{
	if(_expStep) _expStep->clear();
	if(_opStep) _opStep->clear();
}

void CXGridComplier::_NextChar(void)
{
	if(_SourceLen == _curPos)
		_curChar = '\0';
	else
	{
		_curPos++;
		_curChar=_Source[_curPos];
	}
	return;
}

void CXGridComplier::Parse(void)
{
	TCHAR pText[256];
	pText[0] = '\0';
	int k = 0;
	bool isstr = false;
	xtpy lastCharTpy = undef;
	do
	{
		_NextChar();

		//字符串
		if(_curChar=='"' && !isstr)
		{
			isstr = true;
			continue;
		}
		if(isstr)
		{
			//字符串结束
			if(_curChar=='"')
			{
				item xItem;
				xItem.pStr = _tcsdup(pText);
				xItem.nTpy = var;
				xItem.nNo = xstr;
				_expStep->push_back(xItem);
				k = 0;
				pText[0] = '\0';
				isstr = false;
				continue;
			}
			if(_curChar == '\\') _NextChar();
			pText[k] = _curChar;
			pText[k + 1] = '\0';
			k++;
			continue;
		}

		//转义
		if(_curChar == '\\') _NextChar();

		//空格回车忽略
		if(_curChar==' ' ||_curChar=='\t' || _curChar=='\r'||_curChar=='\n') continue;

		//字符
		if(_curChar =='_' || _curChar > 255 ||_curChar == '.' || 
			(_curChar>='a' && _curChar<='z') || 
			(_curChar>='A' && _curChar<='Z') || 
			(_curChar>='0' && _curChar<='9'))
		{
			pText[k] = _curChar;
			pText[k + 1] = '\0';
			k++;
			continue;
		}
		
		//if(k <= 0) continue;
		if(k > 0)
		{
			item xItem;
			xItem.pStr = _tcsdup(pText);

			//命名空间
			if(_curChar == '!' && _SourceLen > _curPos && _Source[_curPos + 1] != '=') 
			{
				xItem.nTpy = other;
				xItem.nNo = ns;
				_expStep->push_back(xItem);
				k = 0;
				pText[k] = '\0';
				lastCharTpy = other;
				continue;
			}		
			
			//函数、变量、值、列 
			xfn zfn;
			if(_isFn(xItem.pStr, &zfn))
			{
				xItem.nTpy = fn;
				xItem.nNo = zfn | 0x100;
				_opStep->push_back(xItem);
				lastCharTpy = fn;
			}
			else
			{
				xItem.nTpy = var;
				xItem.nNo = xstr;
				_expStep->push_back(xItem);
				lastCharTpy = var;
			}
			k = 0;
		}

		//负数
		if(_curChar =='-')
		{
			if(lastCharTpy == op || lastCharTpy == undef)
			{
				pText[k] = _curChar;
				pText[k + 1] = '\0';
				k++;
				continue;
			}
		}
		
		//参数结束
		if(_curChar==',')
		{
			while((int)_opStep->size() > 0)
			{
				item tItem = _opStep->back();
				if(tItem.nTpy == op && tItem.nNo == leftb) break;
				if(tItem.nTpy == other && tItem.nNo == comma) break;
				_expStep->push_back(tItem);
				_opStep->pop_back();
			}
			lastCharTpy = other;
			continue;
		}

		//多字节比较符
		if((_curChar == '=' || _curChar == '!' || _curChar == '<' || _curChar == '>') && 
			_SourceLen > _curPos && _Source[_curPos + 1] == '=')
		{
			pText[k] = _curChar;
			pText[k + 1] = '\0';
			k++;
			_NextChar();
		}
		if((_curChar == '&' && _SourceLen > _curPos && _Source[_curPos + 1] == '&') ||
			(_curChar == '|' && _SourceLen > _curPos && _Source[_curPos + 1] == '|'))
		{
			pText[k] = _curChar;
			pText[k + 1] = '\0';
			k++;
			_NextChar();
		}

		pText[k] = _curChar;
		pText[k + 1] = '\0';
		item zItem;
		zItem.pStr = _tcsdup(pText);

		//运算符
		xop zop;
		if(_isOp(zItem.pStr, &zop))
		{
			lastCharTpy = op;
			zItem.nTpy = op;
			zItem.nNo = zop;
			if(zop == leftb)
			{
				_opStep->push_back(zItem);
				lastCharTpy = other;
			}
			else if(zop == rightb) //不停地弹出堆栈中的内容，直到遇到'('
			{
				bool brackets = false;
				while((int)_opStep->size() > 0)
				{
					item tItem = _opStep->back();
					if(zItem.nTpy == op && tItem.nNo == leftb)
					{
						_opStep->pop_back();
						if((int)_opStep->size() > 0) //'('之前是函数也弹出
						{
							item tItem2 = _opStep->back();
							if(tItem2.nTpy == fn)
							{
								_expStep->push_back(tItem2);
								_opStep->pop_back();
							}
						}
						brackets = true;
						break;
					}
					_expStep->push_back(tItem);
					_opStep->pop_back();
				}
				lastCharTpy = other;
				if(!brackets)
				{	
					_setError(100);
					return;
				}
			}
			else
			{
				while((int)_opStep->size() > 0)
				{
					item tItem = _opStep->back();
					if(_getLev(tItem.nNo) < _getLev(zItem.nNo)) //如果栈顶元素的优先级小于读取到的操作符
					{
						break;
					}
					else
					{
						_expStep->push_back(tItem);
						_opStep->pop_back();
					}
				}
				_opStep->push_back(zItem);
			}
		}		
		else if(_curChar != '\0') //什么都不是
		{
			zItem.nTpy = undef;
			delete zItem.pStr;
			//_setError(120);
			//return;
			//_expStep->push_back(zItem);
		}
		k = 0;
		pText[k] = '\0';
	}while(_curChar != '\0' || _SourceLen != _curPos);
	pText[0] = '\0';

	while((int)_opStep->size() > 0)
	{
		item tItem = _opStep->back();
		_expStep->push_back(tItem);
		_opStep->pop_back();
	}

	return;
}
/*
void CXGridComplier::Parse(void)
{
	TCHAR pText[256];
	pText[0] = '\0';
	int k = 0;
	bool isstr = false;
	do
	{
		_NextChar();

		if(_curChar==' ' ||_curChar=='\t' || _curChar=='\r'||_curChar=='\n') continue;

		if(_curChar=='"' && !isstr)
		{
			isstr = true;
			continue;
		}
		
		if(_curChar =='_' || _curChar > 255 ||_curChar == '.' || 
			(isstr && _curChar!='"') ||
			(_curChar>='a' && _curChar<='z') || 
			(_curChar>='A' && _curChar<='Z') || 
			(_curChar>='0' && _curChar<='9'))
		{
			pText[k] = _curChar;
			pText[k + 1] = '\0';
			k++;
			continue;
		}
		//函数、变量、值
		if(k > 0)
		{
			item xItem;
			xItem.pStr = _tcsdup(pText);
			xfn zfn;
			if(_isFn(xItem.pStr, &zfn) && !isstr)
			{
				xItem.nTpy = fn;
				xItem.nNo = zfn | 0x100;
				_opStep->push_back(xItem);
			}
			else if(_curChar == '!')
			{
				xItem.nTpy = other;
				xItem.nNo = ns;
				_expStep->push_back(xItem);
			}
			else
			{
				xItem.nTpy = var;
				xItem.nNo = xstr;
				_expStep->push_back(xItem);
			}
			k = 0;
		}
		//字符串
		if(_curChar=='"' && isstr)
		{
			isstr = false;
			continue;
		}
		//参数结束
		if(_curChar==',')
		{
			while((int)_opStep->size() > 0)
			{
				item tItem = _opStep->back();
				if(tItem.nTpy == op && tItem.nNo == leftb) break;
				if(tItem.nTpy == other && tItem.nNo == comma) break;
				_expStep->push_back(tItem);
				_opStep->pop_back();
			}
			continue;
		}
		//运算符
		pText[k] = _curChar;
		pText[k + 1] = '\0';
		item zItem;
		zItem.pStr = _tcsdup(pText);
		xop zop;
		if(_isOp(zItem.pStr, &zop))
		{
			zItem.nTpy = op;
			zItem.nNo = zop;
			if(zop == leftb)
			{
				_opStep->push_back(zItem);
			}
			else if(zop == rightb) //不停地弹出堆栈中的内容，直到遇到'('
			{
				bool brackets = false;
				while((int)_opStep->size() > 0)
				{
					item tItem = _opStep->back();
					if(zItem.nTpy == op && tItem.nNo == leftb)
					{
						_opStep->pop_back();
						if((int)_opStep->size() > 0) //'('之前是函数也弹出
						{
							item tItem2 = _opStep->back();
							if(tItem2.nTpy == fn)
							{
								_expStep->push_back(tItem2);
								_opStep->pop_back();
							}
						}
						brackets = true;
						break;
					}
					_expStep->push_back(tItem);
					_opStep->pop_back();
				}
				if(!brackets)
				{	
					_setError(100);
					return;
				}
			}
			else
			{
				while((int)_opStep->size() > 0)
				{
					item tItem = _opStep->back();				
					if(_getLev(tItem.nNo) < _getLev(zItem.nNo)) //如果栈顶元素的优先级小于读取到的操作符
					{
						break;
					}
					else
					{
						_expStep->push_back(tItem);
						_opStep->pop_back();
					}
				}
				_opStep->push_back(zItem);
			}
		}
		else if(_curChar != '\0')
		{
			zItem.nTpy = undef;
			delete zItem.pStr;
			//_setError(120);
			//return;
			//_expStep->push_back(zItem);
		}
	}while(_curChar != '\0' || _SourceLen != _curPos);
	pText[0] = '\0';

	while((int)_opStep->size() > 0)
	{
		item tItem = _opStep->back();
		_expStep->push_back(tItem);
		_opStep->pop_back();
	}

	return;
}
*/
void CXGridComplier::Calc(void)
{
	while((int)_expStep->size() > 0)
	{
		item tItem = *_expStep->begin();
		_expStep->erase(_expStep->begin());

		if(tItem.nTpy == undef) continue;
		//if(tItem.nTpy == ns) continue;

		switch(tItem.nTpy)
		{
			case var:
				_opStep->push_back(tItem);
				break;
			case other:
				{
					if(tItem.nNo == ns)
						_opStep->push_back(tItem);
				}
				break;
			case fn:
				{
					item xItem;
					xItem.nTpy = var;
					xItem.nNo = xint;
					int fnno = tItem.nNo &~ 0x100;
					switch(fnno)
					{
						case sum:
						case sumex:
							{
								if(_hasError(300 + fnno, 1|0x100)) return;
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								xItem.fValue = 0;
								xItem.iValue = 0;
								xvar tt = xint;
								int row = _FRow;
								
								if(!_toStr(&tItem1))
								{
									_setError(350 + fnno);
									return;
								}
								
								bool quick = false;
								if( _FBox == xgrid && _FGrid && fnno==sum)
								{
									quick = true;
									LPTSTR pStr = _FGrid->GetItemFormula(tItem1.pStr);
									if(pStr && pStr[0])quick = false;
								}
								if( quick)
								{
									
									LPTSTR pStr= __getTotalString(L"sum",tItem1.pStr);
									if(pStr)
									{
										xvar t;
										int r1;
										double r3;
										if(_toNumber(pStr, t, r1, r3))
										{
											if(t == xfloat)
												xItem.fValue = r3;
											else
												xItem.iValue = r1;
											xItem.nNo = t;
										}
										else
										{
											_setError(390 + fnno);
											return;
										}
									}
								}else
								{
									for(int i=1; i<=__getRowCount(); i++)
									{
										_FRow = i;
										xvar t;
										int r1;
										double r3;
										LPTSTR pStr = NULL;
										if(fnno == sumex)
											pStr = _askData(tItem1.pStr);
										else
											pStr = __getItemString(i, tItem1.pStr);

										if(_toNumber(pStr, t, r1, r3))
										{
											if(t == xfloat && tt != xfloat)
											{
												tt = xfloat;
												xItem.fValue = (double)xItem.iValue;
												xItem.iValue = 0;
											}
											if(tt == xfloat)
												xItem.fValue += r3;
											else
												xItem.iValue += r1;
										}
										else
										{
											_setError(390 + fnno);
											return;
										}
									}
									_FRow = row;
									xItem.nNo = tt;
								}
							}
							break;
						case row:
							{
								xItem.iValue = __getCurRow();
							}
							break;
						case rowcount:
							{
								if(_hasError(300 + fnno, 0x100)) return;
								xItem.iValue = __getRowCount();
							}
							break;
						case getrow:
							{
								if(_hasError(300 + fnno, 0x100)) return;
								xItem.iValue = __getRow();
							}
							break;
						case col:
							{
								if(_hasError(300 + fnno, 1|0x100)) return;
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toStr(&tItem1))
								{
									_setError(350 + fnno);
									return;
								}
								xItem.pStr = __getItemString(__getCurRow(), tItem1.pStr);
								xItem.nNo = xstr;
							}
							break;
						case getcol:
							{
								xItem.iValue = _FCol;
							}
							break;
						case getitemstring:
							{
								if(_hasError(300 + fnno, 2|0x100)) return;
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toNumber(&tItem1) || !_toStr(&tItem2))
								{
									_setError(350 + fnno);
									return;
								}
								if(tItem2.pStr && tItem1.iValue>0 && tItem1.iValue<=__getRowCount())
									xItem.pStr = __getItemString(tItem1.iValue, tItem2.pStr);
								else
									xItem.pStr = NULL;
								xItem.nNo = xstr;
							}
							break;
						case format:
							{
								if(_hasError(300 + fnno, 2|0x100)) return;
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								xItem.fValue = 0;
							}
							break;
						case user:
							{
								if(_hasError(300 + fnno, 1|0x100)) return;
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toStr(&tItem1))
								{
									_setError(350 + fnno);
									return;
								}
								xItem.pStr = _askData(tItem1.pStr);
								xItem.nNo = xstr;
							}
							break;
						case sround:
							{
								if(_hasError(300 + fnno, 2)) return;
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toStr(&tItem1) || !_toNumber(&tItem2))
								{
									_setError(350 + fnno);
									return;
								}
								bool bdelete = false;
								xItem.pStr = Hxsoft::XUtil::Round(tItem1.pStr,tItem2.iValue,bdelete);
								xItem.nNo = xstr;
							}
							break;
						case left:
							{
								if(_hasError(300 + fnno, 2)) return;
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toStr(&tItem1) || !_toNumber(&tItem2))
								{
									_setError(350 + fnno);
									return;
								}
								if(tItem1.pStr && (int)::_tcslen(tItem1.pStr) > tItem2.iValue)
								{
									LPTSTR pStr = ::_tcsdup(tItem1.pStr);
									pStr[tItem2.iValue] = '\0';
									xItem.pStr = pStr;
								}
								else
									xItem.pStr = tItem1.pStr;
								xItem.nNo = xstr;
							}
							break;
						case right:
							{
								if(_hasError(300 + fnno, 2)) return;
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toStr(&tItem1) || !_toNumber(&tItem2))
								{
									_setError(350 + fnno);
									return;
								}
								if(tItem1.pStr && (int)::_tcslen(tItem1.pStr) > tItem2.iValue)
								{
									LPTSTR pStr = ::_tcsdup(tItem1.pStr);
									pStr = pStr + (::_tcslen(tItem1.pStr) - tItem2.iValue);
									xItem.pStr = pStr;
								}
								else
									xItem.pStr = tItem1.pStr;
								xItem.nNo = xstr;
							}
							break;
						case xabs:
							{
								if(_hasError(400 + fnno, 1)) return;
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toNumber(&tItem1))
								{
									_setError(350 + fnno);
									return;
								}
								xItem.iValue = abs(tItem1.iValue);
								xItem.fValue = tItem1.fValue > 0 ? tItem1.fValue : tItem1.fValue * -1;
								xItem.nNo = tItem1.nNo;
							}
							break;
						case xif:
							{
								if(_hasError(300 + fnno, 3)) return;
								item tItem3 = _opStep->back();
								_opStep->pop_back();
								item tItem2 = _opStep->back();
								_opStep->pop_back();
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toBool(&tItem1))
								{
									_setError(330 + fnno);
									return;
								}
								if(tItem1.bValue)
									xItem = tItem2;
								else
									xItem = tItem3;
							}
							break;
						case xnot:
							{
								if(_hasError(400 + fnno, 1)) return;
								item tItem1 = _opStep->back();
								_opStep->pop_back();
								if(!_toBool(&tItem1))
								{
									_setError(350 + fnno);
									return;
								}
								xItem.bValue = !tItem1.bValue;
								xItem.nNo = xbool;
							}
							break;
					}
					if((int)_opStep->size() > 0)
					{
						item zItem = _opStep->back();
						if(zItem.nTpy == other && zItem.nNo == ns) _opStep->pop_back();
					}
					_opStep->push_back(xItem);
				}
				break;
			case op:
				{
					if(_hasError(200 + tItem.nNo, 2)) return;
					item tItem2 = _opStep->back();
					_opStep->pop_back();
					item tItem1 = _opStep->back();
					_opStep->pop_back();

					if(tItem1.nTpy != var || tItem2.nTpy != var)
					{
						_setError(1000 + tItem.nNo);
						return;
					}

					item xItem;
					xItem.nTpy = var;
					switch(tItem.nNo)
					{
						case sadd:
							{
								if(!_toStr(&tItem1) || !_toStr(&tItem2))
								{
									_setError(250 + tItem.nNo);
									return;
								}
								if(!tItem1.pStr || !tItem2.pStr)
								{
									_setError(260 + tItem.nNo);
									return;
								}
								int nLen = (int)_tcslen(tItem1.pStr) + (int)_tcslen(tItem2.pStr) + 1;
								LPTSTR pStr = new TCHAR[nLen];
								::_stprintf_s(pStr,nLen,_T("%s%s"),tItem1.pStr,tItem2.pStr);
								xItem.pStr = pStr;
								xItem.nNo = xstr;
							}
							break;
						case add:
							{
								if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
								{
									_setError(250 + tItem.nNo);
									return;
								}						
								if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
								{
									xItem.fValue = tItem1.fValue + tItem2.fValue;
									xItem.nNo = xfloat;
								}
								else
								{
									xItem.iValue = tItem1.iValue + tItem2.iValue;
									xItem.nNo = xint;
								}
							}
							break;
						case sub:
							{
								if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
								{
									_setError(250 + tItem.nNo);
									return;
								}						
								if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
								{
									xItem.fValue = tItem1.fValue - tItem2.fValue;
									xItem.nNo = xfloat;
								}
								else
								{
									xItem.iValue = tItem1.iValue - tItem2.iValue;
									xItem.nNo = xint;
								}
							}
							break;
						case mul:
							{
								if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
								{
									_setError(250 + tItem.nNo);
									return;
								}						
								if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
								{
									xItem.fValue = tItem1.fValue * tItem2.fValue;
									xItem.nNo = xfloat;
								}
								else
								{
									xItem.iValue = tItem1.iValue * tItem2.iValue;
									xItem.nNo = xint;
								}
							}
							break;
						case div:
							{
								if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
								{
									_setError(250 + tItem.nNo);
									return;
								}						
								if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
								{
									if(tItem2.fValue == 0)
									{
										_setError(290 + tItem.nNo);
										return;
									}
									xItem.fValue = tItem1.fValue / tItem2.fValue;
									xItem.nNo = xfloat;
								}
								else
								{
									if(tItem2.iValue == 0)
									{
										_setError(280 + tItem.nNo);
										return;
									}
									xItem.iValue = tItem1.iValue / tItem2.iValue;
									xItem.nNo = xint;
								}
							}
							break;
						case eq:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) == 0? true:false;
									else
										xItem.bValue = false;
								}
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue == tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue == tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case ne:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) != 0? true:false;
									else
										xItem.bValue = false;
								}									
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue != tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue != tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case lt:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) < 0? true:false;
									else
										xItem.bValue = false;
								}																	
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue < tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue < tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case le:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) <= 0? true:false;
									else
										xItem.bValue = false;
								}																										
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue <= tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue <= tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case gt:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) > 0? true:false;
									else
										xItem.bValue = false;
								}																										
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue > tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue > tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case ge:
							{
								if(tItem1.nNo == xstr && tItem2.nNo == xstr)
								{
									if(!tItem1.pStr && !tItem2.pStr)
										xItem.bValue = true;
									else if(tItem1.pStr && tItem2.pStr)
										xItem.bValue = _tcscmp(tItem1.pStr,tItem2.pStr) >= 0? true:false;
									else
										xItem.bValue = false;
								}																																			
								else
								{
									if(!_toNumber(&tItem1) || !_toNumber(&tItem2)) 
									{
										_setError(250 + tItem.nNo);
										return;
									}						
									if(tItem1.nNo == xfloat || tItem2.nNo == xfloat)
										xItem.bValue = tItem1.fValue >= tItem2.fValue;
									else
										xItem.bValue = tItem1.iValue >= tItem2.iValue;
								}
								xItem.nNo = xbool;
							}
							break;
						case xand:
							{
								xItem.bValue = false;
								xItem.nNo = xbool;

								if(tItem1.nNo == xstr && !tItem1.pStr) break;
								else if(tItem1.nNo == xbool && !tItem1.bValue) break;
								else if(tItem1.nNo == xfloat && tItem1.fValue == 0) break;	
								else if(tItem1.nNo == xint && tItem1.iValue == 0) break;

								if(tItem2.nNo == xstr && !tItem2.pStr) break;
								else if(tItem2.nNo == xbool && !tItem2.bValue) break;
								else if(tItem2.nNo == xfloat && tItem2.fValue == 0) break;	
								else if(tItem2.nNo == xint && tItem2.iValue == 0) break;

								xItem.bValue = true;
							}
							break;
						case xor:
							{
								xItem.bValue = true;
								xItem.nNo = xbool;

								if(tItem1.nNo == xstr && tItem1.pStr) break;
								else if(tItem1.nNo == xbool && tItem1.bValue) break;
								else if(tItem1.nNo == xfloat && tItem1.fValue != 0) break;	
								else if(tItem1.nNo == xint && tItem1.iValue != 0) break;

								if(tItem2.nNo == xstr && tItem2.pStr) break;
								else if(tItem2.nNo == xbool && tItem2.bValue) break;
								else if(tItem2.nNo == xfloat && tItem2.fValue != 0) break;	
								else if(tItem2.nNo == xint && tItem2.iValue != 0) break;

								xItem.bValue = false;
							}
							break;
					}
					_opStep->push_back(xItem);
				}
				break;
		}
	}
	return;
}

bool CXGridComplier::SetSource(LPTSTR szSource, void* grid, int box, int row, int col)
{
	_Source = ::_tcsdup(szSource);
	//if(_Source[0]== '=') *_Source = _Source[1];
	_SourceLen = (int)::_tcslen(_Source);
	_FBox = (xbox)box;
	if(_FBox == xgrid)
		_FGrid = (XGridReport*)grid;
	else if(_FBox == xcell)
		_FCell = (CGridFrameSvr*)grid;
	else
	{
		_FGrid = NULL;
		_FCell = NULL;
	}
	_FRowInit = row;
	_FColInit = col;
	_FRow = row;
	_FCol = col;
	_curPos = -1;
	_curChar = '\0';
	if(!_expStep) 
		_expStep = new std::vector<item>();
	else
		_expStep->clear();
	if(!_opStep)
		_opStep = new std::vector<item>();
	else
		_opStep->clear();

	_error = 0;

	Parse();
	if(_error > 0) return false;

	Calc();
	if(_error > 0) return false;
	return true;
}

bool CXGridComplier::_isFn(LPTSTR pStr, xfn * fn)
{
	if(!pStr) return false;

	if(_tcsicmp(pStr, L"sum") == 0)
		*fn = sum;
	else if(_tcsicmp(pStr, L"sumex") == 0)
		*fn = sumex;
	else if(_tcsicmp(pStr, L"row") == 0)
		*fn = row;
	else if(_tcsicmp(pStr, L"rowcount") == 0)
		*fn = rowcount;
	else if(_tcsicmp(pStr, L"col") == 0)
		*fn = col;
	else if(_tcsicmp(pStr, L"getrow") == 0)
		*fn = getrow;
	else if(_tcsicmp(pStr, L"format") == 0)
		*fn = format;
	else if(_tcsicmp(pStr, L"user") == 0)
		*fn = user;
	else if(_tcsicmp(pStr, L"getcol") == 0)
		*fn = getcol;
	else if(_tcsicmp(pStr, L"getitemstring") == 0)
		*fn = getitemstring;
	else if(_tcsicmp(pStr, L"left") == 0)
		*fn = left;
	else if(_tcsicmp(pStr, L"right") == 0)
		*fn = right;
	else if(_tcsicmp(pStr, L"if") == 0)
		*fn = xif;
	else if(_tcsicmp(pStr, L"abs") == 0)
		*fn = xabs;
	else if(_tcsicmp(pStr, L"not") == 0)
		*fn = xnot;
	else if(_tcsicmp(pStr, L"sround") == 0)
		*fn = sround;
	else
		return false;
	return true;
}

bool CXGridComplier::_isOp(LPTSTR pStr, xop * op)
{
	if(!pStr) return false;

	if(_tcscmp(pStr, L"+") == 0)
		*op = add;
	else if(_tcscmp(pStr, L"-") == 0)
		*op = sub;
	else if(_tcscmp(pStr, L"*") == 0)
		*op = mul;
	else if(_tcscmp(pStr, L"/") == 0)
		*op = div;
	else if(_tcscmp(pStr, L"(") == 0)
		*op = leftb;
	else if(_tcscmp(pStr, L")") == 0)
		*op = rightb;
	else if(_tcscmp(pStr, L"&") == 0)
		*op = sadd;
	else if(_tcscmp(pStr, L"==") == 0)
		*op = eq;
	else if(_tcscmp(pStr, L"!=") == 0)
		*op = ne;
	else if(_tcscmp(pStr, L"<") == 0)
		*op = lt;
	else if(_tcscmp(pStr, L"<=") == 0)
		*op = le;
	else if(_tcscmp(pStr, L">") == 0)
		*op = gt;
	else if(_tcscmp(pStr, L">=") == 0)
		*op = ge;
	else if(_tcscmp(pStr, L"&&") == 0)
		*op = xand;
	else if(_tcscmp(pStr, L"||") == 0)
		*op = xor;
	else
		return false;
	return true;
}

/*
优先级	运算符
1	括号()
2	负号-
3	乘方**
4	乘*，除/，求余%
5	加+，减-
6	小于<，小于等于<=，大于>，大于等于>=
7	等于==，不等于!=
8	逻辑与&&
9	逻辑或||
100 函数, &字符串连接

一元运算符 > 二元运算符 > 多元运算符。
*/
int CXGridComplier::_getLev(int op)
{
	int ret = 100;
	switch(op)
	{
		case leftb:
		case rightb:
			ret = 1;
			break;
		case add:
		case sub:
			ret -= 5;
			break;
		case mul:
		case div:
			ret -= 4;
			break;	
		case xand:
		case xor:
			ret -= 10;
			break;	
	}
	return ret;
}

/*
double CXGridComplier::_StrToFloat(LPTSTR pStr, bool bDelete)
{
	if(!pStr) return 0;
	//USES_CONVERSION;
	//char *s = W2A(pStr);
	//return atof(s);
	double f = ::_tstof(pStr);
	if(bDelete) delete pStr;
	return f;
}

LPTSTR CXGridComplier::_FloatToStr(double pValue, bool bInt)
{
	//char *s = new char[128];
	//gcvt(pValue,128,s);
	//USES_CONVERSION;
	//LPTSTR pStr = A2W(s);
	LPTSTR pStr = new TCHAR[128];
	if(bInt)
		::_stprintf_s(pStr,128,_T("%d"),(int)pValue);
	else
		::_stprintf_s(pStr,128,_T("%f"),pValue);
	return pStr;
}
*/

bool CXGridComplier::_toBool(item * xItem)
{
	if(xItem->nTpy != var)
		return false;

	if(xItem->nNo == xbool)
		return true;

	if(xItem->nNo == xstr)
	{
		if(xItem->pStr && _tcscmp(xItem->pStr, L"") != 0) 
			xItem->bValue = true;
		else
			xItem->bValue = false;
	}

	if(xItem->nNo == xint)
	{
		if(xItem->iValue != 0) 
			xItem->bValue = true;
		else
			xItem->bValue = false;
	}

	if(xItem->nNo == xfloat)
	{
		if(xItem->fValue != 0) 
			xItem->bValue = true;
		else
			xItem->bValue = false;
	}

	xItem->nNo = xbool;
	return true;
}

bool CXGridComplier::_toStr(item * xItem)
{
	if(xItem->nTpy != var)
		return false;

	if(xItem->nNo == xstr)
	{
		if(xItem->pStr && xItem->pStr[0] == '=')
		{
			CXGridComplier x;
			if(x.SetSource(xItem->pStr, _FGrid, _FBox, _FRow, _FCol))
			{
				xItem->pStr = x.GetString();
			}
		}
		return true;
	}

	LPTSTR pStr = new TCHAR[128];
	if(xItem->nNo == xint)
	{
		::_stprintf_s(pStr,128,_T("%d"),xItem->iValue);
	}
	else if(xItem->nNo == xfloat)
	{
		::_stprintf_s(pStr,128,_T("%f"),xItem->fValue);
	}
	else if(xItem->nNo == xbool)
	{
		if(xItem->bValue)
			::_tcscpy_s(pStr,60,_T("true"));
		else
			::_tcscpy_s(pStr,60,_T("false"));
	}
	else
		return false;

	xItem->pStr = pStr;
	xItem->nNo = xstr;
	return true;
}

bool CXGridComplier::_toNumber(LPTSTR pStr, xvar &t, int &r1, double &r3)
{
	if(!pStr)
	{
		t = xint;
		r1 = 0;
		r3 = 0;
		return true;
	}

	if(pStr && pStr[0] == '=')
	{
		CXGridComplier x;
		if(_FBox == xgrid && _FGrid)
		{
			if(x.SetSource(pStr, _FGrid, _FBox, _FRow, _FCol))
				pStr = x.GetString();
		}
		else if(_FBox == xcell && _FCell)
		{		
			if(x.SetSource(pStr, _FCell, _FBox, _FRow, _FCol))
				pStr = x.GetString();
		}
	}

	int nLen = (int)::_tcslen(pStr);
	int C;
	int P = -1;
	int f = 1;
	/*int*/ r1=0;
	int r2=0;
	/*double*/ r3=0;
	int f1=-1;  //小数点位置
	do
	{
		C = pStr[++P];
		if(C=='\r'||C=='\t'||C==' '||C=='\n') continue;
		if((C=='+' || C=='-') && P!=0) return false;
		if(C=='.' && f1!=-1) return false;

		if(C=='-' && P == 0)
			f = -1;

		if(C>='0' && C<='9')
		{
			if(f1 >= 0)
				r2 = 10 * r2 + C - '0';
			else
				r1 = 10 * r1 + C - '0';
		}
		
		if(C=='.')
			f1 = P;
	}while(C != '\0' || nLen != P);
	if(f1 >= 0)
	{
		t = xfloat;
		r3 = (double)r2;
		for(int i=1; i<(P-f1); i++)
			r3 = r3 / 10;
		r3 += (double)r1;
		r1 *= f;
		r3 *= f;
	}
	else
	{
		t = xint;
		r1 *= f;
		r3 = (double)r1;
	}
	return true;
}

bool CXGridComplier::_toNumber(item * xItem, bool bDelete)
{
	if(xItem->nTpy != var)
		return false;

	if(xItem->nNo == xint)
	{
		xItem->fValue = (double)xItem->iValue;
		return true;
	}
	else if(xItem->nNo == xfloat)
	{
		xItem->iValue = (int)xItem->fValue;
		return true;
	}
	else if(xItem->nNo == xbool)
	{
		if(xItem->bValue)
			xItem->iValue = 1;
		else
			xItem->iValue = 0;
		xItem->fValue = (double)xItem->iValue;
		xItem->nNo = xint;
		return true;
	}
	else if(xItem->nNo == xstr)
	{
		int r1;
		double r3;
		xvar t;
		if(_toNumber(xItem->pStr, t, r1, r3))
		{
			xItem->nNo = t;
			xItem->fValue = r3;
			xItem->iValue = r1;
		}
		else
			return false;
		//if(bDelete) delete xItem->pStr;
		return true;
	}
	else
		return false;
}

void CXGridComplier::_setError(int errid)
{
	_error = errid;
}

bool CXGridComplier::_hasError(int errid, int errTpy)
{
	if(errTpy & 0x100)
	{
		if(_FBox == xgrid && !_FGrid)
		{
			_setError(errid);
			return true;
		}
		else if(_FBox == xcell && !_FCell)
		{
			_setError(errid);
			return true;
		}
		errTpy = errTpy &~ 0x100;
	}
	if(errTpy > 0)
	{
		if((int)_opStep->size() < errTpy)
		{
			_setError(errid);
			return true;
		}	
	}
	return false;
}

LPTSTR CXGridComplier::GetError()
{
	char err[128];
	sprintf_s(err,128, "ErrorID: %d", _error);
	return (LPTSTR)strdup(err);
}

int CXGridComplier::GetInt()
{
	if((int)_opStep->size() != 1)
	{
		_setError(110);
		return NULL;
	}
	item tItem = _opStep->back();
	if(_toNumber(&tItem, false))
	{
		return tItem.iValue;
	}
	else
		return 0;
}

double CXGridComplier::GetFloat()
{
	if((int)_opStep->size() != 1)
	{
		_setError(110);
		return NULL;
	}
	item tItem = _opStep->back();
	if(_toNumber(&tItem, false))
	{
		return tItem.fValue;
	}
	else
		return 0;

}

LPTSTR CXGridComplier::GetString()
{
	if((int)_opStep->size() != 1)
	{
		_setError(110);
		return NULL;
	}
	item tItem = _opStep->back();
	if(_toStr(&tItem))
	{
		return tItem.pStr;
	}
	else
		return NULL;
}

LPTSTR CXGridComplier::_askData(LPTSTR pStr)
{
	if(pStr)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = __getHWND();
		hdr.idFrom = 0;//(int)_FGrid->GetInterface();
		hdr.code = DWV_ASKDATA;
		hdr.DwTable = NULL;
		hdr.row = __getCurRow();
		hdr.col = _FCol;
		hdr.colname = pStr;
		hdr.data = __getItemString(_FRow, pStr);

		::SendMessage(::GetParent(__getHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		return _tcsdup(hdr.data);
	}
	else
		return NULL;
}

CDwTable * CXGridComplier::__getTableByName(CDwTable * pTable, LPTSTR name)
{
	if(!pTable) return NULL;
	if(!name) return pTable;

	if(pTable)
	{
		if(_tcsicmp(name, pTable->m_strBoundGroup)==0) return pTable;

		CDwTable * t;
		for(int i=0;i<(int)pTable->m_pChilds.size();i++)
		{
			t = __getTableByName(pTable->m_pChilds[i], name);
			if(t) return t;
		}
	}
	return NULL;
}

CDwTable * CXGridComplier::__getTable()
{
	if(!_FCell->m_pSheetSvr) return NULL;
	CDwTable * pTable = _FCell->GetGridDataSvr()->GetDwTableFromCell(_FColInit,_FRowInit);
	if(!pTable) return NULL;

	if((int)_opStep->size() < 1) 
		return pTable;
	item tItem = _opStep->back();
	if(tItem.nTpy != other || (tItem.nTpy == other && tItem.nNo != ns))
		return pTable;

	LPTSTR name = tItem.pStr;
	if(!name) return pTable;
	if(_tcsicmp(name, pTable->m_strBoundGroup)==0) return pTable;

	if(pTable->m_pParent)
	{
		CDwTable * pParentTable = pTable->m_pParent;
		while(pParentTable->m_pParent)
			pParentTable = pParentTable->m_pParent;
		pParentTable = __getTableByName(pParentTable, name);
		return pParentTable;
	}
	else
		return __getTableByName(pTable, name);
}

int CXGridComplier::__getRowCount()
{
	if(_FBox == xgrid && _FGrid)
		return _FGrid->GetTreeRowCount();
	else if(_FBox == xcell && _FCell)
	{		
		CDwTable * pDwTable = __getTable();
		if(!pDwTable) return 0;
		return pDwTable->GetRowCount();		
	}

	return 0;
}

int CXGridComplier::__getRow()
{
	if(_FBox == xgrid && _FGrid)
		return _FGrid->GetRow();
	else if(_FBox == xcell && _FCell)
	{
		CDwTable * pDwTable = __getTable();
		if(!pDwTable) return -1;
		return pDwTable->GetRow();		
	}
	return -1;
}

int CXGridComplier::__getCurRow()
{
	if(_FBox == xcell && _FCell)
	{
		CDwTable * pDwTable = __getTable();
		if(!pDwTable) return _FRow;
		return pDwTable->GetRowFromCell(_FRow);		
	}
	return _FRow;	
}

LPTSTR CXGridComplier::__getTotalString(const LPTSTR totalType, LPTSTR colname)
{
	if(!colname) return NULL;
	if(_FBox == xgrid && _FGrid)
	{
		TCHAR buf[256];
		swprintf_s(buf,256,L"%s:%s",totalType,colname);
		LPCTSTR pFormat = _FGrid->GetColumnDecimalFormat(colname);
		
		if(_FGrid->hasBindData()) return _FGrid->m_pSheetSvr->m_pSheetCtrl->bindGetData(_FGrid->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, buf);
		LPTSTR pStr = (LPTSTR)_FGrid->GetGridDataSvr()->m_pContentSvr->GetTotalItemString(buf,pFormat);
		if(pStr) return pStr;
	}
	return NULL;
}

LPTSTR CXGridComplier::__getItemString(int row, LPTSTR colname)
{
	if(!colname) return NULL;

	if(_FBox == xgrid && _FGrid)
	{
		LPTSTR pStr = _FGrid->GetItemFormula(colname);		
		if(pStr && _tcscmp(pStr, _T("")) != 0) return pStr;
		pStr = (LPTSTR)_FGrid->GetItemStringEx(row, colname);
		if(pStr) return pStr;
	}
	else if(_FBox == xcell && _FCell)
	{
		CDwTable * pDwTable = __getTable();
		if(!pDwTable) return NULL;
		PStrItem pItem;
		int col = pDwTable->getColumnIndex(colname);
		LPTSTR pStr = NULL;
		pItem = pDwTable->getDwItem(row,col);
		if(pItem && pItem->pStrXml)
		{
			IXMLDOMElement * pElement1 = NULL;
			IXMLDOMNode * pNode;
			_variant_t var;

			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
				pElement1->getAttribute(L"cellsource",&var);
				if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
					pStr = _tcsdup(var.bstrVal);
				pNode->Release();
				pElement1->Release();
				if(pStr && _tcscmp(pStr, _T("")) != 0) return pStr;
			}
		}
		pStr = pDwTable->getDwItemText(row, col);
		if(pStr) return pStr;
	}
	return NULL;
}

HWND CXGridComplier::__getHWND()
{
	if(_FBox == xgrid && _FGrid)
		return _FGrid->GetHWND();
	else if(_FBox == xcell && _FCell)
	{
		if(!_FCell->m_pSheetSvr) return (HWND)0;
		return _FCell->GetHWND();
		CDwTable * pDwTable = _FCell->GetGridDataSvr()->GetDwTableFromCell(_FCol,_FRow);
		if(!pDwTable) return (HWND)0;
		return pDwTable->m_pGrid->GetHWND();
	}
	return (HWND)0;
}
}}}}