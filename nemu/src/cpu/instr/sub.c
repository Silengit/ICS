#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_sub(opr_src.val,opr_dest.val);
	//printf("sub:addr = %x\n",opr_dest.addr);	
	operand_write(&opr_dest);
}

make_instr_func(sub_i_bv2rm_v){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);
	//printf("%x\n",rm.val);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	int offset = sign_ext(imm.val, 8);

	operand_read(&rm);
	//printf("%x\n",imm.val);
	//printf("%x\n",rm.val);
	rm.val=alu_sub(offset,rm.val);
	//printf("%x\n",rm.val);
	//printf("sub:addr = %x\n",rm.addr);	
	operand_write(&rm);
	print_asm_2("sub","",len + 1,&imm,&rm);

	return len + 1;
}

make_instr_func(sub_i2eax_v)
{	
	OPERAND imm;
	//printf("%x\n",data_size);
	imm.data_size = data_size;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	//printf("%x\n",imm.val);
	if(data_size==32){
		cpu.eax=alu_sub(imm.val,cpu.eax);
		print_asm_1("sub","",5,&imm);
		return 5;
	}
	else{
		cpu.gpr[0]._16=alu_sub(imm.val,cpu.gpr[0]._16);
		print_asm_1("sub","",3,&imm);
		return 3;
	}
	//assert(0);
}

make_instr_impl_2op(sub, rm, r, v)
make_instr_impl_2op(sub, r, rm, v)
make_instr_impl_2op(sub, i, rm, v)
make_instr_impl_2op(sub, i, rm, b)
