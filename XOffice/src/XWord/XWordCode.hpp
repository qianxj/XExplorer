#pragma once

typedef  const unsigned char xwcode_t;
struct XWCode
{
	static xwcode_t newrow		= 0x10;
	static xwcode_t torow		= 0x11;
	static xwcode_t rowheight	= 0x12;
	static xwcode_t rowinsert   = 0x13;
	static xwcode_t rowdelete   = 0x14;

	static xwcode_t pushstyle	= 0x20;
	static xwcode_t popstyle	= 0x21;

	static xwcode_t pushchapter	= 0x30;
	static xwcode_t popchapter	= 0x31;
	static xwcode_t	pushpara	= 0x32;
	static xwcode_t	poppara		= 0x33;
	static xwcode_t	pushblock	= 0x34;
	static xwcode_t	popblock	= 0x35;
	
	static xwcode_t setxpos		= 0x40;
	static xwcode_t setypos		= 0x41;
	static xwcode_t rendertext	= 0x48;

	static xwcode_t exit		= 0x00;
	static xwcode_t jmp			= 0x51;
	static xwcode_t nop			= 0x52;
};
