#pragma once

#include "xwordblock.hpp"
#include <xtree.hpp>

class XWordBlockMgr
{
public:
	XWordBlockMgr(void);
	~XWordBlockMgr(void);
public:
	xtree<XWordBlock>* m_pBlocks;
public:
	XWordBlock* MakeDocument();
public:
	XWordBlock* GetDocument();
};
