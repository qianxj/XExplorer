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
#include "xsconfigure.h"
#include "xsparser.hpp"
#include <vector>

using namespace std;
namespace xsharp {


struct XSHAPE_API strtabitem
{
	lptxstr text;
	strtabitem * next;
};

class XSHAPE_API strtab
{
public:
	strtab();
	~strtab();
private:
	int bucketsize;
	strtabitem ** items;
private:
	int hash(lpctxstr str);
public:
	void * addstring(lpctxstr str);
};

struct XSHAPE_API sym
{
	void * pToken;
	int	   ntype;
	int	   nclass;
	int    nLevel;
	int    nRef;
	int    nModula;
	int	   nParent;
	int	   nIndexBP;
	void*  pdclnode;
	class symtab * m_psymtab;
};

class XSHAPE_API symtab
{
public:
	symtab(void);
	~symtab(void);
public:
	int id;
public:
	vector<sym *> * m_syms;
	vector<symtab *>* m_pchilds;
	symtab * m_pParent ; 
public:
	int destroychilds();
	int destroychilds(symtab * parentnode);
	int addchild(symtab * child);
	void removechild(symtab * child);
public:
	bool  symexists(void * pToken , bool findtop = true);
	sym * addsym(void * pToken ,int ntype = _tUNDEF ,int nclass = _clsUNDEF);
public:
	sym * lookupsym( void *  pToken, bool findtop = true);
};

}
