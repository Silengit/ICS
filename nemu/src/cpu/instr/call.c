#include "cpu/instr.h"

make_instr_func(call_rel)//only 32_bit
{
	cpu.esp-=data_size/8;

	//printf("cpu.esp = %x\n", cpu.esp);
	OPERAND imm;
	//m.data_size=data_size;
	//m.type=OPR_MEM;//memery
	//m.mem_addr.base=cpu.esp;
	//m.val=cpu.eip;
	uint32_t val=cpu.eip+1+data_size/8;
	//memcpy(hw_mem+cpu.esp, &val, data_size/8);
	vaddr_write(cpu.esp, 2, data_size/8, val);
	//operand_write(&m);
	imm.data_size=data_size;
	imm.type=OPR_IMM;
	imm.addr=eip+1;
	operand_read(&imm);

	print_asm_1("call", "", 1+data_size/8,  &imm);

	return 1+data_size/8+imm.val;
}
make_instr_func(call_rm_v)
{	
	cpu.esp-=data_size/8;
	
	OPERAND rm;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);
	
        operand_read(&rm);
	
	uint32_t val=cpu.eip+len;
	//memcpy(hw_mem+cpu.esp, &val, data_size/8);
	vaddr_write(cpu.esp, 2, data_size/8, val);
	//printf("%x\n",rm.val);
	// read memory with physical address and size of bytes

	print_asm_1("call", "", len, &rm);
	
        //cpu.eip = rm.val;

	return rm.val-cpu.eip;
}
