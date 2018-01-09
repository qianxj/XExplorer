#pragma once
enum throws
{
	thr_outmemory	   = 0x0001,
	thr_divbyzero	   = 0x0002,
	thr_outstacktop    = 0x0003,
	thr_outstackbottom = 0x0004,
	thr_outarraybound  = 0x0005,
	thr_unvalidindex   = 0x0006
};