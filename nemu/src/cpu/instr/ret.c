#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	uint32_t val=vaddr_read(cpu.esp, 2, opr_src.val);
	cpu.eip=val-3;

	cpu.esp+=data_size/8;
}

make_instr_func(return_near)
{
	print_asm_0("ret","" , 1);
	
 	//printf("%x\n",cpu.eip);
	uint32_t val=vaddr_read(cpu.esp, 2, data_size/8);
	cpu.eip=val;

	cpu.esp+=data_size/8;
	//printf("%x\n",cpu.eip);
	return 0;
}

make_instr_impl_1op(ret, i, w)

make_instr_func(iret){
	cpu.eip = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.cs.val = (uint16_t)vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;
	cpu.eflags.val = vaddr_read(cpu.esp, 2, 4);
	cpu.esp += 4;	
	print_asm_0("iret","" , 1);
	return 0;
}
