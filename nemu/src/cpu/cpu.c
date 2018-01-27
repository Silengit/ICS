#include "nemu.h"
#include "cpu/cpu.h"
#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"
#include "monitor/breakpoint.h"
#include <stdio.h>
#include <assert.h>

CPU_STATE cpu;
FPU fpu;
int nemu_state;

#define sign(x) ((uint32_t)(x) >> 31)

void init_cpu(const uint32_t init_eip) {
	cpu.eflags.val = 0x0;

#ifdef IA32_SEG 
	cpu.gdtr.limit = 0x0;
	cpu.gdtr.base = 0x0;
	for(int i = 0; i < 6; i++) {
		cpu.segReg[i].val = 0x0;
		cpu.segReg[i].base = 0x0;
		cpu.segReg[i].limit = 0x0;
		cpu.segReg[i].type = 0x0;
		cpu.segReg[i].privilege_level = 0x0;
		cpu.segReg[i].soft_use = 0x0;
	}
	cpu.cr0.val = 0x0;
#endif
#ifdef IA32_PAGE
	cpu.cr3.val = 0x0;
#endif
#ifdef IA32_INTR   
	cpu.idtr.limit = 0x0;
	cpu.idtr.base = 0x0;
#endif 
#ifdef HAS_DEVICE_TIMER
	cpu.intr = 0x0;
#endif
	fpu.status.val = 0x0;
	int i=0;
	for(i = 0; i < 8; i++) {
		cpu.gpr[i].val = 0x0;
		fpu.regStack[i].val = 0x0;
	}
	cpu.eip = init_eip;
	cpu.esp = (128 << 20) - 16;
}

bool verbose = false;

#ifdef HAS_DEVICE_TIMER
static void do_intr();
#endif

void exec(uint32_t n) {
	static BP *bp = NULL;
	verbose = (n <= 100000);
	int instr_len = 0;
	bool hit_break_rerun = false;

	if(nemu_state == NEMU_BREAK) {
		hit_break_rerun = true;
	}
	nemu_state = NEMU_RUN;
	while( n > 0 && nemu_state == NEMU_RUN) {
		//printf("cpu.esp = %x\n", cpu.esp);
		//printf("%x\n",cpu.eax); 
		//for(int i=0;i<8;i++)
		//printf("%x\n",cpu.gpr[i].val);
		//printf("%x\n",fpu.status.val);//delete
#ifdef DEBUG
		if(verbose) clear_operand_mem_addr(&opr_src);
		if(verbose) clear_operand_mem_addr(&opr_dest);
#endif
		instr_len = exec_inst();

		cpu.eip += instr_len;
		n--;

		if(hit_break_rerun) {
			resume_breakpoints();
			hit_break_rerun = false;
		}

		// check for breakpoints
		if(nemu_state == NEMU_BREAK) {
			// find break in the list
			bp = find_breakpoint(cpu.eip - 1);
			if(bp) {
				// found, then restore the original opcode
				//printf("bp->addr = %x\n", bp->addr);
				vaddr_write(bp->addr, SREG_CS, 1, bp->ori_byte);
				cpu.eip--;
			}
			// not found, it is triggered by BREAK_POINT in the code, do nothing
		}

		// check for watchpoints

		BP *wp = scan_watchpoint();
		if(wp != NULL) {
			// print_bin_instr(eip_temp, instr_len);
			// puts(assembly);
			printf("\n\nHit watchpoint %d at address 0x%08x, expr = %s\n", wp->NO, cpu.eip - instr_len, wp->expr);
			printf("old value = %#08x\nnew value = %#08x\n", wp->old_val, wp->new_val);
			wp->old_val = wp->new_val;
			nemu_state = NEMU_READY;
			break;
		}

#ifdef HAS_DEVICE_TIMER
		do_intr();
		//printf("\nPlease call do_intr() here\n");
		//assert(0);
#endif
	}
	if(nemu_state == NEMU_STOP)
		printf("NEMU2 terminated\n");
	else if(n == 0) {
		nemu_state = NEMU_READY;
	}
}

#ifdef HAS_DEVICE_TIMER
static void do_intr() {
	// check for interrupt
	if(cpu.intr && cpu.eflags.IF) {
		// get interrupt number
		uint8_t intr_no = i8259_query_intr_no(); // get interrupt number
		assert(intr_no != I8259_NO_INTR);
		i8259_ack_intr(); // tell the PIC interrupt info received
		raise_intr(intr_no); // raise intrrupt to turn into kernel handler
	}
}
#endif

int exec_inst() {
	uint8_t opcode = 0;
	// get the opcode
	opcode = instr_fetch(cpu.eip, 1);
	// instruction decode and execution
	int len = opcode_entry[opcode](cpu.eip, opcode);
	return len;
}
uint32_t rfun(uint32_t x){
	if(x==0)
		return 0;
	uint32_t nx=x>>1;
	int rv=rfun(nx);
	return (x&1)+rv;
}//compute bit 1 in x
void set_CF(uint32_t src, uint32_t dst)
{
	if(dst<src)
		cpu.eflags.CF=1;
	else 
		cpu.eflags.CF=0;
	
}

void set_PF(uint32_t dst)
{
	dst=dst&0xFF;
	uint32_t len=rfun(dst);
	if(len%2==0)
		cpu.eflags.PF=1;
	else 
		cpu.eflags.PF=0;
}
void set_OF(uint32_t src, uint32_t dst, uint32_t rst)
{
	if((src>>31==dst>>31)&&(rst>>31!=src>>31))
		cpu.eflags.OF=1;
	else
		cpu.eflags.OF=0;
}
void set_ZF(uint32_t dst)
{
	if(dst==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;
}
void set_SF(uint32_t dst)
{	
	if((int)dst<0)
		cpu.eflags.SF=1;
	else 
		cpu.eflags.SF=0;
}

