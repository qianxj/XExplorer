#pragma once
#include "../vl.h"
#include "../parser/errors.hpp"
#include <vector>

using namespace std;

namespace vl
{
	#if !defined(dword)
	//#define dword unsigned int
	#endif

	struct vmlexertoken
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
		};
		tchar		 m_snum[1024]; 

		int prow;
		int pcol;
		int plen;
		int pcc;
		int slen;

		int sindex;

		int m_cc;

		int row;
		int col;
	};

	struct vmsrcpos
	{
		//pointer source code file name or url name;
		tchar * m_psrcname;
		//insymbol at row
		int m_currow;
		//insymbol at col
		int m_curcol;
	}; 

	class VL_API vmlexer
	{
	public:
		const tchar*  source;      //code souce
		//len of source code
		int m_nlen;

		//current token
		vmlexertoken m_token;  
		vmsrcpos m_pos;

		//lexer  state   see #cp.state.*
		unsigned int	vmlexerstate;

		//construct
		vmlexer();
		~vmlexer();
		int error(unsigned int errcode);
		int setsource(const tchar* psrc,tchar* purl=null);
		int insymbol(unsigned int state = 0);
		void skipsym(int n, unsigned int state = 0);
		tchar nextch();
		tchar getchar(int pos);
		
		int startIndex;
		std::vector<vmlexertoken> tokenstack	;

		//save state

		int save();
		int restore();
		int push();
		int push(vmlexertoken & tk);
		int pop();
		vmlexertoken & item(int index);
		int find(int tk);
	};  
}