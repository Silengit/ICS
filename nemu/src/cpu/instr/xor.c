#include "cpu/instr.h"

make_instr_func(xor_r2rm_v){
	OPERAND rm, r;
	rm.data_size = data_size;
	r.data_size = data_size;

	int len=1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	operand_read(&rm);
	operand_read(&r);
	rm.val=alu_xor(r.val,rm.val);
	//printf("xor:addr = %x\n",rm.addr);	
	operand_write(&rm);

	print_asm_2("xor","",len,&r,&rm);
	
	return len;
}

make_instr_func(xor_i_bv2rm_v){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);

	operand_read(&rm);
	operand_read(&imm);

	int offset = sign_ext(imm.val, 8);

	rm.val=alu_xor(offset,rm.val);
	//printf("and:addr = %d\n",rm.addr);
	operand_write(&rm);

	print_asm_2("xor","",len + 1,&imm,&rm);

	return len + 1;
}
