#include "StdAfx.h"
#include "XWCodeAsm.hpp"

XWCodeAsm::XWCodeAsm(unsigned char * acode,unsigned char * apc,int limit):
	code(acode),pc(apc),limit_(limit)
{
}
XWCodeAsm::XWCodeAsm()
{
	limit_ = 0x1000;
	code = new unsigned char[limit_];
	::memset(code,XWCode::exit,limit_);
	pc = code;
	pc++;
}

XWCodeAsm::~XWCodeAsm()
{
	//delete code;
}

void XWCodeAsm::ensureFree(int clen)
{
	if(limit_ - (pc - code) >= clen) return;
	
	unsigned char * temp = new unsigned char [limit_  *2];
	::memset(temp,XWCode::exit,limit_*2);
	memcpy_s(temp,limit_ *2,code, pc - code);

	code[0]= 0xcc;
	delete code;
	int len = (int)(pc - code);
	code = temp;
	pc = code + len;
	limit_ = limit_ * 2;
}

int XWCodeAsm::newrow(int height)
{
	ensureFree();
	*pc++ = XWCode::newrow;
	*((int*)pc) = height;
	pc+=4;
	return 1;
}
int XWCodeAsm::newrow(double height)
{
	ensureFree();
	*pc++ = XWCode::newrow;
	*((int*)pc) = (int)height;
	pc+=4;
	return 1;
}

int XWCodeAsm::torow(int row)
{
	ensureFree();
	*pc++ = XWCode::torow;
	*((int*)pc) = row;
	pc+=4;
	return 1;
}

int XWCodeAsm::rowheight(int height)
{
	ensureFree();
	*pc++ = XWCode::rowheight;
	*((int*)pc) = height;
	pc+=4;
	return 1;
}

int XWCodeAsm::rowheight(double height)
{
	ensureFree();
	*pc++ = XWCode::rowheight;
	*((int*)pc) = (int)height;
	pc+=4;
	return 1;
}

int XWCodeAsm::rowinsert(int row,int height)
{
	ensureFree();
	*pc++ = XWCode::rowinsert;
	*((int*)pc) = row;
	pc+=4;
	*((int*)pc) = height;
	pc+=4;
	return 1;
}

int XWCodeAsm::rowinsert(int row,double height)
{
	ensureFree();
	*pc++ = XWCode::rowinsert;
	*((int*)pc) = row;
	pc+=4;
	*((int*)pc) = (int)height;
	pc+=4;
	return 1;
}

int XWCodeAsm::rowdelete(int row)
{
	ensureFree();
	*pc++ = XWCode::rowdelete;
	*((int*)pc) = row;
	pc+=4;
	return 1;
}

int XWCodeAsm::pushstyle(int cssIndex)
{
	ensureFree();
	*pc++ = XWCode::pushstyle;
	*((int*)pc) =cssIndex;
	pc+=4;
	return 1;
}

int XWCodeAsm::popstyle()
{
	ensureFree();
	*pc++ = XWCode::pushstyle;
	return 1;
}
int XWCodeAsm::pushchapter(vord_t v)
{
	ensureFree();
	*pc++ = XWCode::pushchapter;
	*(vord_t*)pc = v;
	pc += 8;
	return 1;
}

int XWCodeAsm::popchapter()
{
	ensureFree();
	*pc++ = XWCode::popchapter;
	return 1;
}

int XWCodeAsm::pushpara(vord_t v)
{
	ensureFree();
	*pc++ = XWCode::pushpara;
	*(vord_t*)pc = v;
	pc += 8;
	return 1;
}

int XWCodeAsm::poppara()
{
	ensureFree();
	*pc++ = XWCode::poppara;
	return 1;
}

int XWCodeAsm::pushblock(vord_t v)
{
	ensureFree();
	*pc++ = XWCode::pushblock;
	*(vord_t*)pc = v;
	pc += 8;
	return 1;
}
int XWCodeAsm::popblock()
{
	ensureFree();
	*pc++ = XWCode::popblock;
	return 1;
}

int XWCodeAsm::setxpos(int xpos)
{
	ensureFree();
	*pc++ = XWCode::setxpos;
	*((int*)pc) = xpos;
	pc+=4;
	return 1;
}

int XWCodeAsm::setypos(int ypos)
{
	ensureFree();
	*pc++ = XWCode::setypos;
	*((int*)pc) = ypos;
	pc+=4;
	return 1;
}

int XWCodeAsm::rendertext(int pos, int len)
{
	ensureFree();
	*pc++ = XWCode::rendertext;
	*((int*)pc) = pos;
	pc+=4;
	*((int*)pc) = len;
	pc+=4;
	return 1;
}

int XWCodeAsm::jmp(int rel)
{
	ensureFree();
	*pc++ = XWCode::jmp;
	*((int*)pc) = rel;
	pc+=4;
	return 1;
}

int XWCodeAsm::nop()
{
	ensureFree();
	*pc++ = XWCode::nop;
	return 1;
}
