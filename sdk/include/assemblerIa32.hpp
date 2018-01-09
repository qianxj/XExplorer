#ifndef WPP_ASSEMBLER_IA32_H
#define WPP_ASSEMBLER_IA32_H

#include "assembler.hpp"
namespace vm
{

struct Register
{
	int r_;
};

const Register 
	unkown_reg = { -1 },
	eax = { 0 },
	ecx = { 1 },
	edx = { 2 },
	ebx = { 3 },
	esp = { 4 },
	ebp = { 5 },
	esi = { 6 },
	edi = { 7 };

struct XMMRegister
{
	int r_;
};

const XMMRegister
	xmm0 = {0},
	xmm1 = {0},
	xmm2 = {0},
	xmm3 = {0},
	xmm4 = {0},
	xmm5 = {0},
	xmm6 = {0},
	xmm7 = {0};

class DAssembler
{
};
class IAssembler
{
};

class Assembler : public IAssembler, public DAssembler, public AssemblerBase
{

};

}//namespace vm

#endif //WPP_ASSEMBLER_IA32_H