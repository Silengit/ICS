#include "cpu/instr.h"

make_instr_func(sidt) {
	int len = 1;
	OPERAND limit, base;
	limit.data_size = 16;
	limit.val=cpu.idtr.limit;

	len += modrm_rm(eip+1,&limit);
	//printf("sidt:addr = %x\n",limit.addr);	
	operand_write(&limit);
	
	
	base.data_size = 32;
	if(data_size==16)
		base.val = cpu.idtr.base;	
	else
		base.val = cpu.idtr.base & 0x00ffffff;	
	base.addr = limit.addr+2;
	base.type = limit.type;
	//printf("sidt:addr = %x\n",base.addr);	
	operand_write(&base);
	
	print_asm_1("sidt","",len,&limit);

	return len;
}
