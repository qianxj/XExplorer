#pragma once

#include <stdio.h>

namespace vl
{
	#define	LEXERLN				0x0001
	#define	LEXERIDENTDOT		0x0002
	#define	LEXERIDENTSUB		0x0004
	#define LEXERBNF			0x0008
	#define LEXERBLANK			0x0010
	#define LEXERCOMMENT		0x0020
	#define LEXERESCAPE			0x0040

	const int vCharSize     = sizeof(char);     
	const int vShortSize    = sizeof(short);    
	const int vIntSize      = sizeof(int);      
	const int vDoubleSize   = sizeof(double);   
	const int vIntptrSize   = sizeof(intptr_t);  
	const int vPointerSize  = sizeof(void*);    

	const int vDoubleSizeLog2 = 3;

	const int vBitsPerByte = 8;
	const int vBitsPerByteLog2 = 3;
	const int vBitsPerPointer = (vPointerSize * vBitsPerByte);
	const int vBitsPerInt = (vIntSize * vBitsPerByte);
}
