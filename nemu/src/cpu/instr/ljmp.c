#include "cpu/instr.h"

make_instr_func(ljmp){

	
	OPERAND entry, imm_32;

	imm_32.data_size = 32;
	imm_32.type = OPR_IMM;
	imm_32.addr = eip + 1;
	operand_read(&imm_32);

	entry.data_size = 16;
	entry.type = OPR_IMM;
	entry.addr = eip + 5;
	operand_read(&entry);
	//printf("%x\n",entry.val);

	cpu.cs.val = entry.val;
	load_sreg(1);

	print_asm_2("ljmp","",7,&imm_32,&entry);

	cpu.eip = imm_32.val;
	
	return 0;
}
