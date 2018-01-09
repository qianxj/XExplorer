#ifndef  SCAN_H
#define  SCAN_H

#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"

using namespace wpp::how;

struct VLANG3_API sop
{
	typedef int  valuetype;
	static const valuetype undef	= 0x00;
	static const valuetype add		= 0x01;
	static const valuetype sub		= 0x02;
	static const valuetype mul		= 0x03;
	static const valuetype div		= 0x04;
	static const valuetype shl		= 0x05;
	static const valuetype shr		= 0x06;
	static const valuetype asn		= 0x07;
	static const valuetype mod		= 0x08;
	static const valuetype inc		= 0x09;
	static const valuetype dec		= 0x10;
	static const valuetype or		= 0x12;
	static const valuetype and		= 0x13;
	static const valuetype not		= 0x14;
	static const valuetype exclam   = 0x14;
	static const valuetype band		= 0x15;
	static const valuetype bor		= 0x16;
	static const valuetype bnot		= 0x17;
	static const valuetype xor		= 0x18;
	static const valuetype eq		= 0x20;
	static const valuetype neq		= 0x21;
	static const valuetype gt		= 0x22;
	static const valuetype gte		= 0x23;
	static const valuetype lt		= 0x24;
	static const valuetype lte		= 0x25;
	static const valuetype rparen	= 0x26;
	static const valuetype lparen	= 0x27;
	static const valuetype rbrace	= 0x28;
	static const valuetype lbrace	= 0x29;
	static const valuetype rbrackets= 0x2A;
	static const valuetype lbrackets= 0x2B;
	static const valuetype comma	= 0x2C;
	static const valuetype semicolon= 0x2D;
	static const valuetype dot		= 0x2E;
	static const valuetype period	= 0x2E;
	static const valuetype ref		= 0x2E;
	static const valuetype colon	= 0x2F;
	static const valuetype hash		= 0x30;
	static const valuetype becomes  = 0x31;
	static const valuetype query	= 0x32;
	static const valuetype sign		= 0x33;
	static const valuetype backslash= 0x34;
	static const valuetype dollar	= 0x35;
	static const valuetype apos		= 0x36;
	static const valuetype quotes	= 0x37;
	static const valuetype nest		= 0x38;
	static const valuetype percent  = 0x39;
	static const valuetype pointer  = 0x3A;
	static const valuetype dot3	    = 0x3B;
	static const valuetype dot2	    = 0x3C;
	static const valuetype ln	    = 0x3D;
	static const valuetype mref		= 0x3E;
	static const valuetype mpointer = 0x3F;
	static const valuetype varrow   = 0x40;
	
	static const valuetype path		= div;
	static const valuetype path2	= 0x41;

	static const valuetype _asn		= 0x80;

	static valuetype getOperateByToken(wchar_t ch);
	static valuetype getOperateByString(wchar_t* str);
	static wchar_t * getOperateText(int op);
};


struct VLANG3_API token
{
	int ty;
	union
	{
		int inum;
		wchar_t* snum;
		double dnum;
	};
	int nlen;
};

typedef TuringIterator<wchar_t> TuringWCharTape;

struct VLANG3_API turingScan : public Turing<TuringWCharTape>
{
public:
	bool isTapeEnd(TuringTape &tape);
	bool matchValue(TuringWCharTape &tape,int v);
	bool matchString(TuringWCharTape &tape,wchar_t * v);
	bool matchRegex(TuringWCharTape &tape,wchar_t * regex);
	bool MyMatch(TuringWCharTape &tape,CWord & t, CWord::iterator iter);
	int MyAction(TuringWCharTape &tape,CWord & t, CWord::iterator & action,int &halt);
private:
	token _token;
public:
	token & getToken() { return _token;}
public:
	inline int insymbol()
	{
		_token.ty = 0;
		return step();
	}
};

#endif //SCAN_H