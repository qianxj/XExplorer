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
#include "errorcode.h"
#include <vector>

using namespace std;

namespace xsharp {

struct XSHAPE_API  errormsg
{
	lptxstr pFile;
	int nrow;
	int ncol;
	int nErrNo;
	lptxstr pErrText;
};

class XSHAPE_API  xerror 
{
public:
	xerror(void);
	~xerror(void);
public:
	vector<errormsg *> * m_pErrors;
	vector<errormsg *> * m_pWarns;
public:
	virtual int adderror(lptxstr pFile,int nrow,int ncol,int nError,lptxstr pErrText);
	virtual int addwarn(lptxstr pFile,int nrow,int ncol,int nError,lptxstr pErrText);
};

}
