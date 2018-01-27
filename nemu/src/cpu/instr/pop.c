#include "cpu/instr.h"

static void instr_execute_1op() {
	assert(data_size==32);
	operand_read(&opr_src);
	opr_src.val=vaddr_read(cpu.esp, 2, data_size/8);
	//printf("pop:addr = %x\n",opr_src.addr);
	operand_write(&opr_src);
	cpu.esp+=data_size/8;
}

make_instr_impl_1op(pop, r, v)

make_instr_func(popa){
	cpu.edi = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.esi = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.ebp = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	//uint32_t throwaway = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.ebx = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.edx = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.ecx = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.eax = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	print_asm_0("popa","" , 1);
	return 1;
}
/*make_instr_func(pop_r_v)
{

	if(data_size==32){
		uint32_t val;
		memcpy(&val, hw_mem+cpu.esp, data_size/8);
		cpu.gpr[opcode-0x50].val=val;
	}
	else{
		uint16_t val;
		memcpy(&val, hw_mem+cpu.esp, data_size/8);
		cpu.gpr[opcode-0x50]._16=val;
	}
	cpu.esp+=data_size/8;
	
	print_asm_0("pop","" , 1);
	
	return 1;
}*/
