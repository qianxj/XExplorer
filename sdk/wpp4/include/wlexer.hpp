#ifndef WPP_WLEXER_H
#define WPP_WLEXER_H

#include "pred.hpp"
#include "errors.hpp"
#include "werr.hpp"

#include <vector>

using namespace std;

namespace wpp {
namespace how {

#define	LEXERLN				0x0001
#define	LEXERIDENTDOT		0x0002
#define	LEXERIDENTSUB		0x0004
#define LEXERBNF			0x0008
#define LEXERBLANK			0x0010
#define LEXERCOMMENT		0x0020
#define LEXERESCAPE			0x0040
#define LEXERXPATH			0x0080
#define LEXERXQUERY			0x0100
#define LEXERXMLTEXT		0x0200

struct wtoken
{
	//next char;
	wchar_t m_ch;

	int ty;
	union
	{
		int inum;
		double dnum;
	};
	const wchar_t* snum;
	int nlen;

	int prow;
	int pcol;
	int plen;
	int pcc;
	int slen;

	int sindex;

	int m_cc;

	int row;
	int col;

	inline void setOperate(int op)
	{
		ty = pw::sm::scan::token_op;
		inum = op;
	}
};

struct wsrcpos
{
	//pointer source code file name or url name;
	wchar_t * m_psrcname;
	//insymbol at row
	int m_currow;
	//insymbol at col
	int m_curcol;
}; 

class VLANG3_API wlexer
{
public:
	const wchar_t*  source;      //code souce
	//len of source code
	int m_nlen;

	//current token
	wtoken m_token;  
	wsrcpos m_pos;

	//lexer  state   see #cp.state.*
	unsigned int	wlexerstate;

	//construct
	wlexer();
	~wlexer();
	int error(unsigned int errcode);
	int setsource(const wchar_t* psrc,wchar_t* purl=0);
	int insymbol(unsigned int state = 0);
	void skipsym(int n, unsigned int state = 0);
	wchar_t nextch();
	wchar_t gewchar_t(int pos);
	
	int startIndex;
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<wtoken> tokenstack	;
#pragma warning( pop )

	//save state

	int save();
	int restore();
	int push();
	int push(wtoken & tk);
	int pop();
	wtoken & item(int index);
	int find(int tk);
};  

}} //namespace wpp::now

#endif //WPP_WLEXER_H