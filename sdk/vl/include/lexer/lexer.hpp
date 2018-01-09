#pragma once
#include "../vl.h"
#include "../parser/errors.hpp"
#include <vector>

using namespace std;

namespace vl
{
	struct token
	{
		//next char;
		tchar m_ch;

		//current symbol type
		int m_sy;

		//current symbol value
		union
		{
			double		 m_rnum;
			int			 m_inum;
			tchar		 m_snum[1024]; 
		};

		int m_cc;

		int row_;
		int col_;

	};

	struct srcpos
	{
		//pointer source code file name or url name;
		tchar * m_psrcname;
		//insymbol at row
		int m_currow;
		//insymbol at col
		int m_curcol;
	};

	//this class is for lexer code source
	class VL_API lexer
	{
	public:
		lexer(void);
		~lexer(void);
	public:
		//pointer to source code
		tchar * m_psource;
	public:
		//len of source code
		int m_nlen;
		//insymbol at position of code source
		int m_cc;

	public:
		token m_token;
		srcpos m_pos;
	public:
		//read next token
		int insymbol(unsigned long flag =0);
		//read next char
		tchar nextch();
	public:
		tchar getchar(int pos = 0);
	public:
		//trim comment
		int trimcomment();
		//process number
		int processnumber();
		//process string
		int processstring();
		//process operator
		int processoperator();
	public:
		//process bnf number
		int processbnfnumber();
		
	public:
		//set code source;
		int setsource(tchar * psrc,tchar * psrcname = null, int ntype = 0);
	private:
		//process error
		int error(error::err item);
	public:
		vector<token> tokenstack;
	public:
		int save();
		int restore();
		int pop();
	};
}