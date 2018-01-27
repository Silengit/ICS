#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val=alu_or(opr_src.val,opr_dest.val);
	//printf("or:addr = %x\n",opr_src.addr);
	operand_write(&opr_dest);
}

/*make_instr_func(or_r2rm_v){
	OPERAND rm, r;
	rm.data_size = data_size;
	r.data_size = data_size;

	int len=1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	operand_read(&rm);
	operand_read(&r);
	rm.val=alu_or(r.val,rm.val);
	operand_write(&rm);

	print_asm_2("or","",len,&r,&rm);
	
	return len;
}*/
make_instr_impl_2op(or, r, rm, v)
make_instr_impl_2op(or, rm, r, b)
make_instr_impl_2op(or, i, rm, b)
make_instr_impl_2op(or, i, rm, v)
make_instr_impl_2op(or, rm, r, v)

make_instr_func(or_i_b2rm_v)
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

	rm.val=alu_or(offset,rm.val);
	//printf("or:addr = %x\n",rm.addr);
	operand_write(&rm);

	print_asm_2("or","",len + 1,&imm,&rm);

	return len + 1;
}

make_instr_func(or_i2al_b){
	OPERAND imm;
	imm.data_size = 8;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	cpu.gpr[0]._8[0]=alu_or(imm.val,cpu.gpr[0]._8[0]);
	print_asm_1("or","",2,&imm);
	return 2;	
}

make_instr_func(or_i2eax_v){
	OPERAND imm;
	imm.data_size = data_size;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	if(data_size==32){
		cpu.eax=alu_or(imm.val,cpu.eax);
		print_asm_1("or","",5,&imm);
		return 5;
	}
	else{
		cpu.gpr[0]._16=alu_or(imm.val,cpu.gpr[0]._16);
		print_asm_1("or","",3,&imm);
		return 3;
	}
}
