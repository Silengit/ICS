#include "cpu/instr.h"

make_instr_func(lgdt){
	int len = 1;
	OPERAND limit, base;
	limit.data_size = 16;
	base.data_size = 32;

	len += modrm_rm(eip+1,&limit);
	//printf("%x\n",len);
	//assert(len==6);
	operand_read(&limit);

	//printf("%x\n",len);
	//printf("%x\n",limit.addr);
	base.addr = limit.addr+2;
	base.type = limit.type;
	operand_read(&base);
	
	cpu.gdtr.limit = limit.val;
	//printf("%x\n",cpu.gdtr.limit);
	if(data_size==16)
		cpu.gdtr.base = base.val & 0x00ffffff;	
	else
		cpu.gdtr.base = base.val;
	//printf("%x\n",cpu.gdtr.base);
	print_asm_1("lgdt","",len,&limit);

	return len;
}
