/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

#include "vl.h"
#include "../parser/errors.hpp"
#pragma once

namespace vl
{
	#define	LEXERLN				0x0001
	#define	LEXERIDENTDOT		0x0002

	//this class is for lexer code source
	class VL_API lexer
	{
	public:
		lexer(void);
		~lexer(void);
	public:
		//pointer source code file name or url name;
		tchar * m_psrcname;
		//pointer to source code
		tchar * m_psource;
	public:
		//insymbol at row
		int m_currow;
		//insymbol at col
		int m_curcol;
	public:
		//len of source code
		int m_nlen;
		//insymbol at position of code source
		int m_cc;

	public:
		//next char;
		tchar m_ch;

		//current symbol type
		int m_sy;

		//current symbol value
		union
		{
			double		 m_rnum;
			int			 m_inum;
			//when strlen < 255 pointer to m_buf;
			tchar		 m_snum[255]; 
		};
		tchar*		 m_pstr;
	private:
		//read double scale
		int readscale(void);
		//addjust double scale
		int adjustscale(int e);
		//adjust double scale
		int adjustscale(double &rnum, int e);
	public:
		//read next token
		int insymbol(unsigned long flag = 0);
		//read next char
		tchar nextch();
	public:
		//set code source;
		int setsource(tchar * psrc,tchar * psrcname = null, int ntype = 0);
	private:
		//trim comment
		int trimcomment();

		//process number
		int processnumber(unsigned long flag = 0);
		//process string
		int processstring(unsigned long flag = 0);
		//process token
		int processtoken(unsigned long flag = 0);
		//process operator
		int processoperator(unsigned long flag = 0);
	private:
		//process error
		int error(error::err item);
	};
}