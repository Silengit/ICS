#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_add(opr_src.val,opr_dest.val);
	//printf("add:addr = %d\n",opr_dest.addr);
	operand_write(&opr_dest);
}


/*make_instr_func(add_i2rm_v){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);

	operand_read(&rm);
	operand_read(&imm);
	rm.val=alu_add(imm.val,rm.val);
	operand_write(&rm);

	print_asm_2("add","",len + data_size / 8,&imm,&rm);

	return len + data_size / 8;
}

make_instr_func(add_r2rm_b){
	OPERAND rm, r;
	rm.data_size = 8;
	r.data_size = 8;

	int len=1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	operand_read(&rm);
	operand_read(&r);
	rm.val=alu_add(r.val,rm.val);
	operand_write(&rm);

	print_asm_2("add","",len,&r,&rm);
	
	return len;
}*/

make_instr_func(add_i2eax_l){
	OPERAND imm;
	imm.data_size = 32;

	int len = 1;
	
	imm.type = OPR_IMM;
	imm.addr = eip + len;
	
	// printf("addr imm %08x, len=%d\n", imm.addr, len);
	
	operand_read(&imm);
	cpu.eax=alu_add(cpu.eax,imm.val);

	print_asm_1("add","",len + data_size / 8,&imm);

	return len + data_size / 8;
}

/*make_instr_func(add_r2rm_v){
	OPERAND rm, r;
	rm.data_size = data_size;
	r.data_size = data_size;

	int len=1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	operand_read(&rm);
	operand_read(&r);
	rm.val=alu_add(r.val,rm.val);
	operand_write(&rm);

	print_asm_2("add","",len,&r,&rm);
	
	return len;
}*/

make_instr_func(add_i_bv2rm_v)
{
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);
	operand_read(&imm);
	int offset = sign_ext(imm.val, 8);

	operand_read(&rm);
	rm.val=alu_add(offset,rm.val);
	//printf("add:addr = %d\n",rm.addr);
	operand_write(&rm);
	print_asm_2("add","",len + 1,&imm,&rm);

	return len + 1;
}
make_instr_impl_2op(add, i, rm, v)
make_instr_impl_2op(add, r, rm, b)
make_instr_impl_2op(add, i, rm, b)
make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, rm, r, v)
make_instr_impl_2op(add, rm, r, b)
