#include "cpu/instr.h"

make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;
	

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);

	print_asm_1("jmp", "", 1+data_size/8, &rel);
	
	cpu.eip += offset;
	
	
        return 1 + data_size / 8;
}

make_instr_func(jmp_b){
	OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;
	

        operand_read(&rel);

	int offset = sign_ext(rel.val, 8);
	
	print_asm_1("jmp", "", 2, &rel);
	cpu.eip += offset;	
	
        return 2;
}

make_instr_func(jmp_m_v)
{
	OPERAND rm;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

        operand_read(&rm);
	
	//printf("%x\n",rm.val);
	// read memory with physical address and size of bytes
	print_asm_1("jmp", "", len, &rm);
	
	cpu.eip = rm.val;
	
	
        return 0;
}

