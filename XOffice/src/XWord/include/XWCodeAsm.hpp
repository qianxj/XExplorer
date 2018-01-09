#pragma once

#include "XWordCode.hpp"
#include "wpp4.h"

class XWCodeAsm
{
public:
	XWCodeAsm();
	XWCodeAsm(unsigned char * code,unsigned char * pc,int limit);
	~XWCodeAsm();
public:
	void ensureFree(int clen=64);
public:
	int newrow(int height);
	int newrow(double height);
	int torow(int row);
	int rowheight(int height);
	int rowheight(double height);
	int rowinsert(int row,int height);
	int rowinsert(int row,double height);
	int rowdelete(int row);

	int pushstyle(int cssIndex);
	int popstyle();
	int pushchapter(vord_t v);
	int popchapter();
	int pushpara(vord_t v);
	int poppara();
	int pushblock(vord_t v);
	int popblock();

	int setxpos(int xpos);
	int setypos(int ypos);
	int rendertext(int pos, int len);

	int jmp(int rel);
	int nop();
public:
	unsigned char *  code;
	unsigned char *  pc;
	int limit_;
};