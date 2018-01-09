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
using namespace std;
namespace xsharp {

class symtab;
class strtab;
class astnode;
class xsflexer;
class XSHAPE_API complier
{
public:
	complier(void);
	~complier(void);
public:
	symtab* m_psymtab;
	symtab* m_psymtabcur;
	symtab* m_psymtabstore;//if m_psymtabcur not used, give it to m_psymtabstore for next use;
public:
	strtab * m_pstrtab;
public:
	//bool eval(lptstr psource,error * perror);
	//bool eval(lptstr psource,bool debug = false);
	bool eval(istream* arg_yyin , ostream* arg_yyout,bool debug = false);

public:
	xsflexer * _lexer;
public:
	int err(lptstr err);
public:
	symtab* askchildsymtab();
	void releasecursymtab();
public:
	symtab* getcursymtab();
public:
	astnode* m_prootnode;
public:
	int seq;
};

}