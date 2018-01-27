#include "cpu/instr.h"

make_instr_func(lidt){
	int len = 1;
	OPERAND limit, base;
	limit.data_size = 16;
	limit.type = OPR_MEM;
	base.data_size = 32;

	len += modrm_rm(eip+1,&limit);
	//printf("%x\n",len);
	operand_read(&limit);

	base.addr = limit.addr+2;
	base.type = limit.type;
	base.val = laddr_read(base.addr, 4);
	//operand_read(&base);
	
	cpu.idtr.limit = limit.val;
	//printf("%x\n",cpu.idtr.base);
	//printf("%x\n",cpu.idtr.limit);
	if(data_size==16)
		cpu.idtr.base = base.val & 0x00ffffff;	
	else
		cpu.idtr.base = base.val;
	//printf("%x\n",cpu.idtr.base);
	print_asm_1("lidt","",len,&limit);
	//assert(0);
	return len;
}
