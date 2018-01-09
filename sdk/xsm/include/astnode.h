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
#include <vector>
using namespace std;

namespace xsharp {

class XSHAPE_API astnode
{
public:
	astnode(void);
	~astnode(void);
public:
	vector<astnode *>* m_pchilds;
	astnode * m_parent;
public:
	int destroychilds();
	int destroychilds(astnode * parentnode);
public:
	int addchild(astnode * pnode);
public:
	int m_nNodeType;
	int m_nSubNodeType;
public:
	int m_nOp;
	int m_nOpex;
public:
	void * m_pdata;
	double m_dval;
	int m_nval;
	bool m_bval;
	void *m_pv;
public:
	bool m_bQualifyName;
	bool m_bTemplateSpec;
	bool m_bGlobalSpec;
	astnode * m_pNestNameSpec;
};

} //end namespace