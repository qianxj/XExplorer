#ifndef WPP_ENCODER32_H
#define WPP_ENCODER32_H

/*MACHINE INSTRUCTION FORMAT
 [Legacy Prefixes][REX Prefixes][Opcodes][ModR/M][SIB][Displacement][Immediate] 
 ModR/M	:		[Mod]{2} [R*]{3} [R/M]{3}
 SIB:			[Scale]{2} Index{3} Base{[3} 
 Displacement:	d32|16|8|null
 Immediate:		d32|16|8|null
 Note:
	* The Reg Field may be used as an opcode extension field (TTT) and as a
	  way to encode diagnostic registers (eee)
*/

/*
Field Name	Description												Number of Bits  
reg			General-register specifier	
w			Specifies if data is byte or full-sized,					1
			where full-sized is 16 or 32 bits 		
s			Specifies sign extension of an immediate field				1
sreg2		Segment register specifier for CS, SS, DS, ES				2
sreg3		Segment register specifier for CS, SS, DS, ES, FS, GS		3
eee			Specifies a special-purpose (control or debug)register 
tttn		For conditional instructions, specifies a condition			4
			asserted or negated 
d			Specifies direction of data operation						1
*/

#define EMIT_byte(x)		emit_b(x);	
#define EMIT_int(x)			emit(x);
#define EMIT_imm8(x)		emit_b(x);
#define EMIT_imm32(x)		emit(x);
#define EMIT_addr(r,x)		emit(r,x);
#define EMIT_imm(x)			if(IMM_w(x)) EMIT_imm8(x) else EMIT_imm32(x) 
#define IMM_w(x)			(x.is_int8()?0:1)

#define	ENCODER_imm(i)		EMIT_imm(i)
#define	ENCODER_imm8(i)		EMIT_imm8(i)
#define	ENCODER_imm32(i)	EMIT_imm32(i)
#define ENCODER_addr(r,m)	EMIT_addr(r,m)
#define ENCODER_Op(x)		EMIT_byte(x)

#define	ENCODER_d(o,d)		(o|(d<1))
#define	ENCODER_w(o,w)		(o|w)
#define	ENCODER_dw(o,d,w)	(o|(d<1)|w)
#define	ENCODER_rm(mod,r,rm) ((mod<<6)|(r<<3)|rm)
/*
AAA 每 ASCII Adjust after Addition 0011 0111
AAD 每 ASCII Adjust AX before Division 1101 0101 : 0000 1010
AAM 每 ASCII Adjust AX after Multiply  1101 0100 : 0000 1010
AAS 每 ASCII Adjust AL after Subtraction 0011 1111
*/
#define	ENCODER_aaa()			ENCODER_Op(0x37)	
#define	ENCODER_aad()			ENCODER_Op(0xD5) ENCODER_Op(0xA)
#define	ENCODER_aam()			ENCODER_Op(0xD4) ENCODER_Op(0xA) 
#define	ENCODER_aas()			ENCODER_Op(0x3F)

/*ADC 每 ADD with Carry
register1 to register2 0001 000w : 11 reg1 reg2
register2 to register1 0001 001w : 11 reg1 reg2
memory to register 0001 001w : mod reg r/m
register to memory 0001 000w : mod reg r/m
immediate to register 1000 00sw : 11 010 reg : immediate data
immediate to AL, AX, or EAX 0001 010w : immediate data
immediate to memory 1000 00sw : mod 010 r/m : immediate data
*/
#define	ENCODER_adc_rr(r,rs,w)		ENCODER_dw(0x10,1,w) ENCODE_rm(11,r,rs)	
#define	ENCODER_adc_rm(r,m,w)		ENCODER_dw(0x10,1,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_adc_mr(m,r,w)		ENCODER_dw(0x10,0,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_adc_iax(i,s)		ENCODER_dw(0x14,s,IMM_w(i)) ENCODE_imm(i)	
#define	ENCODER_adc_ir(i,r,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_rm(11,0x2,r) ENCODE_imm(i)	
#define	ENCODER_adc_irm(i,m,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_addr(0x2,m) ENCODE_imm(i)	

/*
ADD 每 Add
register1 to register2 0000 000w : 11 reg1 reg2
register2 to register1 0000 001w : 11 reg1 reg2
memory to register 0000 001w : mod reg r/m
register to memory 0000 000w : mod reg r/m
immediate to register 1000 00sw : 11 000 reg : immediate data
immediate to AL, AX, or EAX 0000 010w : immediate data
immediate to memory 1000 00sw : mod 000 r/m : immediate data
*/
#define	ENCODER_add_rr(r,rs,w)		ENCODER_dw(0x00,1,w) ENCODE_rm(11,r,rs)	
#define	ENCODER_add_rm(r,m,w)		ENCODER_dw(0x00,1,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_add_mr(m,r,w)		ENCODER_dw(0x00,0,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_add_iax(i,s)		ENCODER_dw(0x04,s,IMM_w(i)) ENCODE_imm(i)	
#define	ENCODER_add_ir(i,r,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_rm(11,0x0,r) ENCODE_imm(i)	
#define	ENCODER_add_irm(i,m,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_addr(0x0,m) ENCODE_imm(i)	

/*
AND 每 Logical AND
register1 to register2 0010 000w : 11 reg1 reg2
register2 to register1 0010 001w : 11 reg1 reg2
memory to register 0010 001w : mod reg r/m
register to memory 0010 000w : mod reg r/m
immediate to register 1000 00sw : 11 100 reg : immediate data
immediate to AL, AX, or EAX 0010 010w : immediate data
immediate to memory 1000 00sw : mod 100 r/m : immediate data
*/
#define	ENCODER_and_rr(r,rs,w)		ENCODER_dw(0x20,1,w) ENCODE_rm(11,r,rs)	
#define	ENCODER_and_rm(r,m,w)		ENCODER_dw(0x20,1,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_and_mr(m,r,w)		ENCODER_dw(0x20,0,w) ENCODER_addr(mod,r,m)	
#define	ENCODER_and_iax(i,s)		ENCODER_dw(0x24,s,IMM_w(i)) ENCODE_imm(i)	
#define	ENCODER_and_ir(i,r,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_rm(11,0x4,r) ENCODE_imm(i)	
#define	ENCODER_and_irm(i,m,s)		ENCODER_dw(0x80,s,IMM_w(i)) ENCODE_addr(0x4,m) ENCODE_imm(i)	

/*
ARPL 每 Adjust RPL Field of Selector
from register 0110 0011 : 11 reg1 reg2
from memory 0110 0011 : mod reg r/m
*/
#define	ENCODER_arpl_rr(r,rs)		ENCODER_Op(0x63) ENCODE_rm(11,r,rs)	
#define	ENCODER_arpl_rm(r,m)		ENCODER_Op(0x63) ENCODER_addr(r,m)	

/*
BOUND 每 Check Array Against Bounds 0110 0010 : modA reg r/m
*/
#define	ENCODER_bound_rm(r,m)		ENCODER_Op(0x62) ENCODER_addr(r,m)	

/*
BSF 每 Bit Scan Forward
register1, register2 0000 1111 : 1011 1100 : 11 reg1 reg2
memory, register 0000 1111 : 1011 1100 : mod reg r/m
*/
#define	ENCODER_bsf_rr(r,rs)	ENCODER_Op(0x0f) ENCODER_Op(0xbc) ENCODER_rm(11,r,rs)		
#define	ENCODER_bsf_mr(r,m)		ENCODER_Op(0x0f) ENCODER_Op(0xbc) ENCODER_addr(r,m)	

/*
BSR 每 Bit Scan Reverse
register1, register2 0000 1111 : 1011 1101 : 11 reg1 reg2
memory, register 0000 1111 : 1011 1101 : mod reg r/m
*/
#define	ENCODER_bsr_rr(r,rs)	ENCODER_Op(0x0f) ENCODER_Op(0xbd) ENCODER_rm(11,r,rs)		
#define	ENCODER_bsr_mr(r,m)		ENCODER_Op(0x0f) ENCODER_Op(0xbd) ENCODER_addr(r,m)	

/*
BSWAP 每 Byte Swap 0000 1111 : 1100 1 reg
*/
#define	ENCODER_bswap_r(r)		ENCODER_Op(0x0f) ENCODER_rm(11,001,r)		

/*
BT 每 Bit Test
register, immediate 0000 1111 : 1011 1010 : 11 100 reg: imm8 data
memory, immediate 0000 1111 : 1011 1010 : mod 100 r/m : imm8 data
register1, register2 0000 1111 : 1010 0011 : 11 reg2 reg1
memory, reg 0000 1111 : 1010 0011 : mod reg r/m
*/
#define	ENCODER_bt_ri8(r,i8)	ENCODER_Op(0x0f) ENCODER_Op(0xba) ENCODER_rm(11,0x4,r) ENCODE_imm8(i)	
#define	ENCODER_bt_mi8(m,i8)	ENCODER_Op(0x0f) ENCODER_Op(0xba) ENCODER_addr(0x4,m) ENCODE_imm8(i)	
#define	ENCODER_bt_rr(r,rs)		ENCODER_Op(0x0f) ENCODER_Op(0xba) ENCODER_rm(11,0x4,r) ENCODE_imm8(i)	
#define	ENCODER_bt_mr(r,rs)		ENCODER_Op(0x0f) ENCODER_Op(0xba) ENCODER_rm(11,0x4,r) ENCODE_imm8(i)	

/*
BTC 每 Bit Test and Complement
register, immediate 0000 1111 : 1011 1010 : 11 111 reg: imm8 data
memory, immediate 0000 1111 : 1011 1010 : mod 111 r/m : imm8 data
register1, register2 0000 1111 : 1011 1011 : 11 reg2 reg1
memory, reg 0000 1111 : 1011 1011 : mod reg r/m
BTR 每 Bit Test and Reset
register, immediate 0000 1111 : 1011 1010 : 11 110 reg: imm8 data
memory, immediate 0000 1111 : 1011 1010 : mod 110 r/m : imm8 data
register1, register2 0000 1111 : 1011 0011 : 11 reg2 reg1
memory, reg 0000 1111 : 1011 0011 : mod reg r/m
BTS 每 Bit Test and Set
register, immediate 0000 1111 : 1011 1010 : 11 101 reg: imm8 data
memory, immediate 0000 1111 : 1011 1010 : mod 101 r/m : imm8 data
register1, register2 0000 1111 : 1010 1011 : 11 reg2 reg1
memory, reg 0000 1111 : 1010 1011 : mod reg r/m
CALL 每 Call Procedure (in same segment)
direct 1110 1000 : full displacement
register indirect 1111 1111 : 11 010 reg
memory indirect 1111 1111 : mod 010 r/m
CALL 每 Call Procedure (in other segment)
direct 1001 1010 : unsigned full offset, selector
indirect 1111 1111 : mod 011 r/m
CBW 每 Convert Byte to Word 1001 1000
CDQ 每 Convert Doubleword to Qword 1001 1001
CLC 每 Clear Carry Flag 1111 1000
CLD 每 Clear Direction Flag 1111 1100
CLI 每 Clear Interrupt Flag 1111 1010
CLTS 每 Clear Task-Switched Flag in CR0 0000 1111 : 0000 0110
CMC 每 Complement Carry Flag 1111 0101
CMP 每 Compare Two Operands
register1 with register2 0011 100w : 11 reg1 reg2
register2 with register1 0011 101w : 11 reg1 reg2
memory with register 0011 100w : mod reg r/m
register with memory 0011 101w : mod reg r/m
immediate with register 1000 00sw : 11 111 reg : immediate data
immediate with AL, AX, or EAX 0011 110w : immediate data
immediate with memory 1000 00sw : mod 111 r/m : immediate data
CMPS/CMPSB/CMPSW/CMPSD 每 Compare String Operands 1010 011w
CMPXCHG 每 Compare and Exchange
register1, register2 0000 1111 : 1011 000w : 11 reg2 reg1
memory, register 0000 1111 : 1011 000w : mod reg r/m
CPUID 每 CPU Identification 0000 1111 : 1010 0010
CWD 每 Convert Word to Doubleword 1001 1001
CWDE 每 Convert Word to Doubleword 1001 1000
DAA 每 Decimal Adjust AL after Addition 0010 0111
DAS 每 Decimal Adjust AL after Subtraction 0010 1111
DEC 每 Decrement by 1
register 1111 111w : 11 001 reg
register (alternate encoding) 0100 1 reg
memory 1111 111w : mod 001 r/m
DIV 每 Unsigned Divide
AL, AX, or EAX by register 1111 011w : 11 110 reg
AL, AX, or EAX by memory 1111 011w : mod 110 r/m
HLT 每 Halt 1111 0100
IDIV 每 Signed Divide
AL, AX, or EAX by register 1111 011w : 11 111 reg
AL, AX, or EAX by memory 1111 011w : mod 111 r/m
IMUL 每 Signed Multiply
AL, AX, or EAX with register 1111 011w : 11 101 reg
AL, AX, or EAX with memory 1111 011w : mod 101 reg
register1 with register2 0000 1111 : 1010 1111 : 11 : reg1 reg2
register with memory 0000 1111 : 1010 1111 : mod reg r/m
register1 with immediate to register2 0110 10s1 : 11 reg1 reg2 : immediate data
memory with immediate to register 0110 10s1 : mod reg r/m : immediate data
IN 每 Input From Port
fixed port 1110 010w : port number
variable port 1110 110w
INC 每 Increment by 1
reg 1111 111w : 11 000 reg
reg (alternate encoding) 0100 0 reg
memory 1111 111w : mod 000 r/m
INS 每 Input from DX Port 0110 110w
INT n 每 Interrupt Type n 1100 1101 : type
INT 每 Single-Step Interrupt 3 1100 1100
INTO 每 Interrupt 4 on Overflow 1100 1110
INVD 每 Invalidate Cache 0000 1111 : 0000 1000
INVLPG 每 Invalidate TLB Entry 0000 1111 : 0000 0001 : mod 111 r/m
INVPCID 每 Invalidate Process-Context Identifier 0110 0110:0000 1111:0011 1000:1000 0010: mod reg r/m
IRET/IRETD 每 Interrupt Return 1100 1111
Jcc 每 Jump if Condition is Met
8-bit displacement 0111 tttn : 8-bit displacement
full displacement 0000 1111 : 1000 tttn : full displacement
JCXZ/JECXZ 每 Jump on CX/ECX Zero
Address-size prefix differentiates JCXZ
and JECXZ
1110 0011 : 8-bit displacement
JMP 每 Unconditional Jump (to same segment)
short 1110 1011 : 8-bit displacement
direct 1110 1001 : full displacement
register indirect 1111 1111 : 11 100 reg
memory indirect 1111 1111 : mod 100 r/m
JMP 每 Unconditional Jump (to other segment)
direct intersegment 1110 1010 : unsigned full offset, selector
indirect intersegment 1111 1111 : mod 101 r/m
LAHF 每 Load Flags into AHRegister 1001 1111
LAR 每 Load Access Rights Byte
from register 0000 1111 : 0000 0010 : 11 reg1 reg2
from memory 0000 1111 : 0000 0010 : mod reg r/m
LDS 每 Load Pointer to DS 1100 0101 : modA,B reg r/m
LEA 每 Load Effective Address 1000 1101 : modA reg r/m
LEAVE 每 High Level Procedure Exit 1100 1001
LES 每 Load Pointer to ES 1100 0100 : modA,B reg r/m
LFS 每 Load Pointer to FS 0000 1111 : 1011 0100 : modA reg r/m
LGDT 每 Load Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 010 r/m
LGS 每 Load Pointer to GS 0000 1111 : 1011 0101 : modA reg r/m
LIDT 每 Load Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 011 r/m
LLDT 每 Load Local Descriptor Table Register
LDTR from register 0000 1111 : 0000 0000 : 11 010 reg
LDTR from memory 0000 1111 : 0000 0000 : mod 010 r/m
LMSW 每 Load Machine Status Word
from register 0000 1111 : 0000 0001 : 11 110 reg
from memory 0000 1111 : 0000 0001 : mod 110 r/m
LOCK 每 Assert LOCK# Signal Prefix 1111 0000
LODS/LODSB/LODSW/LODSD 每 Load String Operand 1010 110w
LOOP 每 Loop Count 1110 0010 : 8-bit displacement
LOOPZ/LOOPE 每 Loop Count while Zero/Equal 1110 0001 : 8-bit displacement
LOOPNZ/LOOPNE 每 Loop Count while not Zero/Equal 1110 0000 : 8-bit displacement
LSL 每 Load Segment Limit
from register 0000 1111 : 0000 0011 : 11 reg1 reg2
from memory 0000 1111 : 0000 0011 : mod reg r/m
LSS 每 Load Pointer to SS 0000 1111 : 1011 0010 : modA reg r/m
LTR 每 Load Task Register
from register 0000 1111 : 0000 0000 : 11 011 reg
from memory 0000 1111 : 0000 0000 : mod 011 r/m
MOV 每 Move Data
register1 to register2 1000 100w : 11 reg1 reg2
register2 to register1 1000 101w : 11 reg1 reg2
memory to reg 1000 101w : mod reg r/m
reg to memory 1000 100w : mod reg r/m
immediate to register 1100 011w : 11 000 reg : immediate data
immediate to register (alternate encoding) 1011 w reg : immediate data
immediate to memory 1100 011w : mod 000 r/m : immediate data
memory to AL, AX, or EAX 1010 000w : full displacement
AL, AX, or EAX to memory 1010 001w : full displacement
MOV 每 Move to/from Control Registers
CR0 from register 0000 1111 : 0010 0010 : -- 000 reg
CR2 from register 0000 1111 : 0010 0010 : -- 010reg
CR3 from register 0000 1111 : 0010 0010 : -- 011 reg
CR4 from register 0000 1111 : 0010 0010 : -- 100 reg
register from CR0-CR4 0000 1111 : 0010 0000 : -- eee reg
MOV 每 Move to/from Debug Registers
DR0-DR3 from register
DR4-DR5 from register 0000 1111 : 0010 0011 : -- eee reg
DR6-DR7 from register 0000 1111 : 0010 0011 : -- eee reg
register from DR6-DR7 0000 1111 : 0010 0001 : -- eee reg
register from DR4-DR5 0000 1111 : 0010 0001 : -- eee reg
register from DR0-DR3 0000 1111 : 0010 0001 : -- eee reg
MOV 每 Move to/from Segment Registers
register to segment register 1000 1110 : 11 sreg3 reg
register to SS 1000 1110 : 11 sreg3 reg
memory to segment reg 1000 1110 : mod sreg3 r/m
memory to SS 1000 1110 : mod sreg3 r/m
segment register to register 1000 1100 : 11 sreg3 reg
segment register to memory 1000 1100 : mod sreg3 r/m
MOVBE 每 Move data after swapping bytes
memory to register 0000 1111 : 0011 1000:1111 0000 : mod reg r/m
register to memory 0000 1111 : 0011 1000:1111 0001 : mod reg r/m
MOVS/MOVSB/MOVSW/MOVSD 每 Move Data from String to
String
1010 010w
MOVSX 每 Move with Sign-Extend
memory to reg 0000 1111 : 1011 111w : mod reg r/m
MOVZX 每 Move with Zero-Extend
register2 to register1 0000 1111 : 1011 011w : 11 reg1 reg2
memory to register 0000 1111 : 1011 011w : mod reg r/m
MUL 每 Unsigned Multiply
AL, AX, or EAX with register 1111 011w : 11 100 reg
AL, AX, or EAX with memory 1111 011w : mod 100 r/m
NEG 每 Two's Complement Negation
register 1111 011w : 11 011 reg
memory 1111 011w : mod 011 r/m
NOP 每 No Operation 1001 0000
NOP 每 Multi-byte No Operation1
register 0000 1111 0001 1111 : 11 000 reg
memory 0000 1111 0001 1111 : mod 000 r/m
NOT 每 One's Complement Negation
register 1111 011w : 11 010 reg
memory
OR 每 Logical Inclusive OR
register1 to register2 0000 100w : 11 reg1 reg2
register2 to register1 0000 101w : 11 reg1 reg2
memory to register 0000 101w : mod reg r/m
register to memory 0000 100w : mod reg r/m
immediate to register 1000 00sw : 11 001 reg : immediate data
immediate to AL, AX, or EAX 0000 110w : immediate data
immediate to memory 1000 00sw : mod 001 r/m : immediate data
OUT 每 Output to Port
fixed port 1110 011w : port number
variable port 1110 111w
OUTS 每 Output to DX Port 0110 111w
POP 每 Pop a Word from the Stack
register 1000 1111 : 11 000 reg
register (alternate encoding) 0101 1 reg
memory 1000 1111 : mod 000 r/m
POP 每 Pop a Segment Register from the Stack
(Note: CS cannot be sreg2 in this usage.)
segment register DS, ES 000 sreg2 111
segment register SS 000 sreg2 111
segment register FS, GS 0000 1111: 10 sreg3 001
POPA/POPAD 每 Pop All General Registers 0110 0001
POPF/POPFD 每 Pop Stack into FLAGS or EFLAGS Register 1001 1101
PUSH 每 Push Operand onto the Stack
register 1111 1111 : 11 110 reg
register (alternate encoding) 0101 0 reg
memory 1111 1111 : mod 110 r/m
immediate 0110 10s0 : immediate data
PUSH 每 Push Segment Register onto the Stack
segment register CS,DS,ES,SS 000 sreg2 110
segment register FS,GS 0000 1111: 10 sreg3 000
PUSHA/PUSHAD 每 Push All General Registers 0110 0000
PUSHF/PUSHFD 每 Push Flags Register onto the Stack 1001 1100
RCL 每 Rotate thru Carry Left
register by 1 1101 000w : 11 010 reg
memory by 1 1101 000w : mod 010 r/m
register by CL 1101 001w : 11 010 reg
memory by CL 1101 001w : mod 010 r/m
register by immediate count 1100 000w : 11 010 reg : imm8 data
memory by immediate count 1100 000w : mod 010 r/m : imm8 data
RCR 每 Rotate thru Carry Right
register by 1 1101 000w : 11 011 reg
memory by 1 1101 000w : mod 011 r/m
register by CL 1101 001w : 11 011 reg
memory by CL 1101 001w : mod 011 r/m
register by immediate count 1100 000w : 11 011 reg : imm8 data
memory by immediate count 1100 000w : mod 011 r/m : imm8 data
RDMSR 每 Read from Model-Specific Register 0000 1111 : 0011 0010
RDPMC 每 Read Performance Monitoring Counters 0000 1111 : 0011 0011
RDTSC 每 Read Time-Stamp Counter 0000 1111 : 0011 0001
RDTSCP 每 Read Time-Stamp Counter and Processor ID 0000 1111 : 0000 0001: 1111 1001
REP INS 每 Input String 1111 0011 : 0110 110w
REP LODS 每 Load String 1111 0011 : 1010 110w
REP MOVS 每 Move String 1111 0011 : 1010 010w
REP OUTS 每 Output String 1111 0011 : 0110 111w
REP STOS 每 Store String 1111 0011 : 1010 101w
REPE CMPS 每 Compare String 1111 0011 : 1010 011w
REPE SCAS 每 Scan String 1111 0011 : 1010 111w
REPNE CMPS 每 Compare String 1111 0010 : 1010 011w
REPNE SCAS 每 Scan String 1111 0010 : 1010 111w
RET 每 Return from Procedure (to same segment)
no argument 1100 0011
adding immediate to SP 1100 0010 : 16-bit displacement
RET 每 Return from Procedure (to other segment)
intersegment 1100 1011
adding immediate to SP 1100 1010 : 16-bit displacement
ROL 每 Rotate Left
register by 1 1101 000w : 11 000 reg
memory by 1 1101 000w : mod 000 r/m
register by CL 1101 001w : 11 000 reg
memory by CL 1101 001w : mod 000 r/m
register by immediate count 1100 000w : 11 000 reg : imm8 data
memory by immediate count 1100 000w : mod 000 r/m : imm8 data
ROR 每 Rotate Right
register by 1 1101 000w : 11 001 reg
memory by 1 1101 000w : mod 001 r/m
register by CL 1101 001w : 11 001 reg
memory by CL 1101 001w : mod 001 r/m
register by immediate count 1100 000w : 11 001 reg : imm8 data
memory by immediate count 1100 000w : mod 001 r/m : imm8 data
RSM 每 Resume from System Management Mode 0000 1111 : 1010 1010
SAHF 每 Store AH into Flags 1001 1110
SAL 每 Shift Arithmetic Left same instruction as SHL
SAR 每 Shift Arithmetic Right
register by 1 1101 000w : 11 111 reg
memory by 1 1101 000w : mod 111 r/m
register by CL 1101 001w : 11 111 reg
memory by CL 1101 001w : mod 111 r/m
register by immediate count 1100 000w : 11 111 reg : imm8 data
memory by immediate count 1100 000w : mod 111 r/m : imm8 data
SBB 每 Integer Subtraction with Borrow
register1 to register2 0001 100w : 11 reg1 reg2
register2 to register1 0001 101w : 11 reg1 reg2
memory to register 0001 101w : mod reg r/m
register to memory 0001 100w : mod reg r/m
immediate to register 1000 00sw : 11 011 reg : immediate data
immediate to AL, AX, or EAX 0001 110w : immediate data
immediate to memory 1000 00sw : mod 011 r/m : immediate data
SCAS/SCASB/SCASW/SCASD 每 Scan String 1010 111w
SETcc 每 Byte Set on Condition
register 0000 1111 : 1001 tttn : 11 000 reg
memory 0000 1111 : 1001 tttn : mod 000 r/m
SGDT 每 Store Global Descriptor Table Register 0000 1111 : 0000 0001 : modA 000 r/m
SHL 每 Shift Left
register by 1 1101 000w : 11 100 reg
memory by 1 1101 000w : mod 100 r/m
register by CL 1101 001w : 11 100 reg
memory by CL 1101 001w : mod 100 r/m
register by immediate count 1100 000w : 11 100 reg : imm8 data
memory by immediate count 1100 000w : mod 100 r/m : imm8 data
SHLD 每 Double Precision Shift Left
register by immediate count 0000 1111 : 1010 0100 : 11 reg2 reg1 : imm8
memory by immediate count 0000 1111 : 1010 0100 : mod reg r/m : imm8
register by CL 0000 1111 : 1010 0101 : 11 reg2 reg1
memory by CL 0000 1111 : 1010 0101 : mod reg r/m
SHR 每 Shift Right
register by 1 1101 000w : 11 101 reg
memory by 1 1101 000w : mod 101 r/m
register by CL 1101 001w : 11 101 reg
memory by CL 1101 001w : mod 101 r/m
register by immediate count 1100 000w : 11 101 reg : imm8 data
memory by immediate count 1100 000w : mod 101 r/m : imm8 data
SHRD 每 Double Precision Shift Right
register by immediate count 0000 1111 : 1010 1100 : 11 reg2 reg1 : imm8
memory by immediate count 0000 1111 : 1010 1100 : mod reg r/m : imm8
register by CL 0000 1111 : 1010 1101 : 11 reg2 reg1
memory by CL 0000 1111 : 1010 1101 : mod reg r/m
SIDT 每 Store Interrupt Descriptor Table Register 0000 1111 : 0000 0001 : modA 001 r/m
SLDT 每 Store Local Descriptor Table Register
to register 0000 1111 : 0000 0000 : 11 000 reg
to memory 0000 1111 : 0000 0000 : mod 000 r/m
SMSW 每 Store Machine Status Word
to register 0000 1111 : 0000 0001 : 11 100 reg
to memory 0000 1111 : 0000 0001 : mod 100 r/m
STC 每 Set Carry Flag 1111 1001
STD 每 Set Direction Flag 1111 1101
STI 每 Set Interrupt Flag 1111 1011
STOS/STOSB/STOSW/STOSD 每 Store String Data 1010 101w
STR 每 Store Task Register
to register 0000 1111 : 0000 0000 : 11 001 reg
to memory 0000 1111 : 0000 0000 : mod 001 r/m
SUB 每 Integer Subtraction
register1 to register2 0010 100w : 11 reg1 reg2
register2 to register1 0010 101w : 11 reg1 reg2
memory to register 0010 101w : mod reg r/m
register to memory 0010 100w : mod reg r/m
immediate to register 1000 00sw : 11 101 reg : immediate data
immediate to AL, AX, or EAX 0010 110w : immediate data
immediate to memory 1000 00sw : mod 101 r/m : immediate data
TEST 每 Logical Compare
register1 and register2 1000 010w : 11 reg1 reg2
memory and register 1000 010w : mod reg r/m
immediate and register 1111 011w : 11 000 reg : immediate data
immediate and AL, AX, or EAX 1010 100w : immediate data
immediate and memory 1111 011w : mod 000 r/m : immediate data
UD2 每 Undefined instruction 0000 FFFF : 0000 1011
VERR 每 Verify a Segment for Reading
register 0000 1111 : 0000 0000 : 11 100 reg
memory 0000 1111 : 0000 0000 : mod 100 r/m
VERW 每 Verify a Segment for Writing
register 0000 1111 : 0000 0000 : 11 101 reg
memory 0000 1111 : 0000 0000 : mod 101 r/m
WAIT 每 Wait 1001 1011
WBINVD 每 Writeback and Invalidate Data Cache 0000 1111 : 0000 1001
WRMSR 每 Write to Model-Specific Register 0000 1111 : 0011 0000
XADD 每 Exchange and Add
register1, register2 0000 1111 : 1100 000w : 11 reg2 reg1
memory, reg 0000 1111 : 1100 000w : mod reg r/m
XCHG 每 Exchange Register/Memory with Register
register1 with register2 1000 011w : 11 reg1 reg2
AX or EAX with reg 1001 0 reg
memory with reg 1000 011w : mod reg r/m
XLAT/XLATB 每 Table Look-up Translation 1101 0111
XOR 每 Logical Exclusive OR
register1 to register2 0011 000w : 11 reg1 reg2
register2 to register1 0011 001w : 11 reg1 reg2
memory to register 0011 001w : mod reg r/m
register to memory 0011 000w : mod reg r/m
immediate to register 1000 00sw : 11 110 reg : immediate data
immediate to AL, AX, or EAX 0011 010w : immediate data
immediate to memory 1000 00sw : mod 110 r/m : immediate data
Prefix Bytes
address size 0110 0111
LOCK 1111 0000
operand size 0110 0110
CS segment override 0010 1110
DS segment override 0011 1110
ES segment override 0010 0110
FS segment override 0110 0100
GS segment override 0110 0101
SS segment override 0011 0110
*/


#endif //WPP_ENCODER32_H