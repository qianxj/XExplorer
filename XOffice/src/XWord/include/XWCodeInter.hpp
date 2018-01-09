#pragma once

#include "XWCodeContext.hpp"

class XWCodeInter
{
public:
	XWCodeInter();
	~XWCodeInter();
public:
	void DefaultProcess(unsigned char* & pc);
public:
	void Inter(XWCodeContentExtent* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(XWCodePaintContext* ctx, unsigned char* pc, unsigned char * end = 0);
	void Inter(XWCodeContextPosition* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(CalcBlockAtRow    &blockatrow,   XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(CalcBlockAtRowCol &blockatrowcol,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(CalcColCount &colcount,XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(CalcRowColAtPoint& xt, XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);
	void Inter(CalcPointAtRowCol& xt, XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);

	void Inter(CalcRowColAtBlock    &rcblock,   XWCodeContextInfo* ctx,unsigned char* pc, unsigned char * end = 0);
};