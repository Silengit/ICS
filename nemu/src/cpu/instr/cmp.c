#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val,opr_dest.val);
}


make_instr_func(cmp_i_bv2rm_v)
{
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;//length of opcode
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	int offset = sign_ext(imm.val, 8);

	
	operand_read(&rm);
	alu_sub(offset, rm.val);
	
	print_asm_2("cmp", "", len+1 , &imm, &rm);

	return len+1;
}

make_instr_func(cmp_i2rm_v)
{
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;

	int len = 1;//length of opcode
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	operand_read(&rm);
	alu_sub(imm.val, rm.val);
	
	print_asm_2("cmp", "", len+data_size/8 , &imm, &rm);

	return len+data_size/8;
}

make_instr_func(cmp_r2rm_v){
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//printf("mov_r2rm rm addr = 0x%08x\n", rm.addr);
	operand_read(&r);
	operand_read(&rm);
	alu_sub(r.val, rm.val);
	
	print_asm_2("cmp", "", len , &r, &rm);

	return len;
}

make_instr_impl_2op(cmp, rm, r, v)

make_instr_func(cmp_i2al_b)
{
	OPERAND imm;
	imm.data_size = 8;

	int len = 1;

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	
	//printf("%x\n",cpu.gpr[0]._8[0]);
	alu_sub(imm.val, cpu.gpr[0]._8[0]);
	
	print_asm_1("cmp", "", 2 , &imm);

	return 2;
}

make_instr_func(cmp_i2eax_v)
{
	OPERAND imm;
	imm.data_size = data_size;

	int len = 1;

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	
	if(data_size==32){
		alu_sub(imm.val, cpu.eax);
		print_asm_1("cmp", "", 5 , &imm);
		return 5;
	}
	else{
		alu_sub(imm.val, cpu.gpr[0]._16);
		print_asm_1("cmp", "", 3 , &imm);
		return 3;
	}
}

make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, r, rm, b)
