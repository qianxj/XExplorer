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

#include "windows.h"
#include "tchar.h"

#define STR_BUCKET_SIZE 8192

#define assert(x) 
#define xstrcmp  _stricmp
#define xstrlen strlen

#define lptxstr char * 
#define lpctxstr const char * 
#define xchar char
#define null NULL
#ifdef UNICODE
	#define _X(x)  L#x
#else
	#define _X(x) x
#endif