#ifndef __CPU_H__
#define __CPU_H__

// interface for cpu functions

#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/alu.h"
#include "cpu/reg_fpu.h"

extern CPU_STATE cpu;

// initialize the cpu states
void init_cpu();

// execute n instructions starting from the current eip
// change eip according to the length of the instruction in each step
void exec(uint32_t n);

// execute an instruction pointed by the current eip
// return the length of the instruction
int exec_inst();


// test the cpu
void alu_test();

//my_function(9.15.9:00)
void set_CF(uint32_t src, uint32_t dst);
void set_PF(uint32_t dst);
void set_OF(uint32_t src, uint32_t dst, uint32_t rst);
void set_ZF(uint32_t dst);
void set_SF(uint32_t dst);

#endif
