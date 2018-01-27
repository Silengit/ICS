#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_src.val=alu_add(1,opr_src.val);//CF
	//printf("inc:addr = %x\n",opr_src.addr);
	operand_write(&opr_src);
}

/*make_instr_func(inc_rm_v)//not consider CF
{
	OPERAND rm;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);


	operand_read(&rm);
	uint32_t t=rm.val+1;
	set_PF(t);
	set_OF(rm.val,1,t);
	set_SF(t);
	set_ZF(t);
	//set_AF(?) now not in consideration, following skip this.
	rm.val=t;
	operand_write(&rm);

	print_asm_1("inc", "", len , &rm);

	return len;
}*/
make_instr_impl_1op(inc, rm, v)
make_instr_impl_1op(inc, r, v)
