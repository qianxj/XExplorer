	#pragma once
	
	#define uint8_t  unsigned char
	#define int32_t  int
	#define uint32_t  unsigned int
	#include <stdio.h>
	#include <string>

	 int ndisasm(unsigned char code[], int len,std::wstring & str);