#IFNDEF WPP_INSTRUCTION_IA32_H
#DEFINE WPP_INSTRUCTION_IA32_H

/*
Instruction Prefixes
. Group 1
！ Lock and repeat prefixes:
	. LOCK prefix is encoded using F0H
	. REPNE/REPNZ prefix is encoded using F2H. Repeat-Not-Zero prefix applies only to string and
	input/output instructions. (F2H is also used as a mandatory prefix for some instructions)
	. REP or REPE/REPZ is encoded using F3H. Repeat prefix applies only to string and input/output instructions.(
	F3H is also used as a mandatory prefix for some instructions)
. Group 2
！ Segment override prefixes:
	. 2EH！CS segment override (use with any branch instruction is reserved)
	. 36H！SS segment override prefix (use with any branch instruction is reserved)
	. 3EH！DS segment override prefix (use with any branch instruction is reserved)
	. 26H！ES segment override prefix (use with any branch instruction is reserved)
	. 64H！FS segment override prefix (use with any branch instruction is reserved)
	. 65H！GS segment override prefix (use with any branch instruction is reserved)
！ Branch hints:
	. 2EH！Branch not taken (used only with Jcc instructions)
	. 3EH！Branch taken (used only with Jcc instructions)
. Group 3
	. Operand-size override prefix is encoded using 66H (66H is also used as a mandatory prefix for some
	instructions).
. Group 4
	. 67H！Address-size override prefix

Opcodes
Addressing-Mode Encoding of ModR/M: Mod{2}REG{3}RM{3}
MOD bits Effective Address
00 [scaled index] + disp32
01 [scaled index] + disp8 + [EBP]
10 [scaled index] + disp32 + [EBP]

REX Prefixes
0100WRXB
Field Name		Bit		Position Definition
-				7:4		0100
W				3		0 = Operand size determined by CS.D
						1 = 64 Bit Operand Size
R				2		Extension of the ModR/M reg field
X				1		Extension of the SIB index field
B				0		Extension of the ModR/M r/m field, SIB base field, or Opcode reg field
*/

namespace vm { namespace ins{

	//Add with Carry
	//DEST ○ DEST + SRC + CF;
	const unsigned char adc_ib = 0x14;	//ADC AL, imm8
	const unsigned char adc_iw = 0x15;	//ADC AX, imm16
	const unsigned char adc_id = 0x16;	//ADC EAX,imm32

}}; //namespace vm::ins

#endif //WPP_INSTRUCTION_IA32_H
