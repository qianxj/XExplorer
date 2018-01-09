/*
 * Copyright (c) 2009-2009
 *     Hxsoft. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
 
#pragma once
#include "xsharp.h"
#include <istream>
#include "xerror.h"

using namespace std;
namespace xsharp {

class symtab;
class strtab;
class astnode;
class xsflexer;

struct xsRegister
{
	int sp;
	int bp;
	int ax;
	int bx;
	int cx;
	int dx;
	int cp;
	int si;
	int di;
	int* pStack;

	xsRegister();
	~xsRegister();
	void push(int x);
	int  pop();
};

class XSHAPE_API complier
{
public:
	complier(void);
	~complier(void);
public:
	symtab* m_psymtab;
	symtab* m_psymtabcur;
public:
	strtab * m_pstrtab;
public:
	//bool eval(lptxstr psource,error * perror);
	//bool eval(lptxstr psource,bool debug = false);
	bool eval(istream* arg_yyin , ostream* arg_yyout,bool debug = false);

public:
	xsflexer * _lexer;
public:
	int err(lptxstr err);
public:
	symtab* askchildsymtab();
	void releasecursymtab();
public:
	symtab* getcursymtab();
public:
	astnode* m_prootnode;
	xerror* m_error;
public:
	int seq;
public:
	const char * const * m_pyytnames;
public:
	char * GetLabel(astnode * pNode);
public:
	int interFct(xsRegister &Register,astnode * pnode);
	int interStmts(xsRegister &Register,astnode * pnode);
	int interExpr(xsRegister &Register, astnode* pnode);
	int interExprTerm(xsRegister &Register, astnode* pnode);
	int interReturn(xsRegister &Register,astnode * pnode);
	int exec();
	int exec(astnode * pnode);
};

}