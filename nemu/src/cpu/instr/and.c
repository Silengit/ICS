#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val,opr_dest.val);
	//printf("and:addr = %d\n",opr_dest.addr);
	operand_write(&opr_dest);
}


/*make_instr_func(and_r2rm_b)
{	
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	operand_read(&rm);
	operand_read(&r);
	rm.val=alu_and(r.val,rm.val);
	operand_write(&rm);

	print_asm_2("and","",8,&r,&rm);


	return len;
}*/

make_instr_func(and_i_b2rm_v)
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

	operand_read(&rm);
	operand_read(&imm);

	int offset = sign_ext(imm.val, 8);

	rm.val=alu_and(offset,rm.val);
	//printf("and:addr = %d\n",rm.addr);
	operand_write(&rm);

	print_asm_2("and","",len + 1,&imm,&rm);

	return len + 1;
}


make_instr_func(and_i2eax_v)
{	
	OPERAND imm;
	//printf("%x\n",data_size);
	imm.data_size = data_size;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	//printf("%x\n",imm.val);
	if(data_size==32){
		cpu.eax=alu_and(imm.val,cpu.eax);
		print_asm_1("and","",5,&imm);
		return 5;
	}
	else{
		cpu.gpr[0]._16=alu_and(imm.val,cpu.gpr[0]._16);
		print_asm_1("and","",3,&imm);
		return 3;
	}
	//assert(0);
}

make_instr_func(and_i2al_b){
	OPERAND imm;
	imm.data_size = 8;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	cpu.gpr[0]._8[0]=alu_and(imm.val,cpu.gpr[0]._8[0]);
	print_asm_1("and","",2,&imm);
	return 2;	
}

make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, i, rm, v)
make_instr_impl_2op(and, i, rm, b)
make_instr_impl_2op(and, rm, r, v)
