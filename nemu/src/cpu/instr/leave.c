#include "cpu/instr.h"

make_instr_func(leave){
	cpu.esp=cpu.ebp;
	
	uint32_t val=vaddr_read(cpu.esp, 2, data_size/8);
	//memcpy(&val, hw_mem+cpu.esp, data_size/8);
	cpu.ebp=val;
	cpu.esp+=data_size/8;
	
	print_asm_0("leave","" , 1);
	
	
	return 1;
}
