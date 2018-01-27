#include "cpu/instr.h"

static void instr_execute_2op() {
	//operand_read(&opr_src);
	opr_dest.val = opr_src.addr;
	//printf("lea:addr = %x\n",opr_dest.addr);
	operand_write(&opr_dest);
}

make_instr_impl_2op(lea, rm, r, v);

/*make_instr_func(lea)
{
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	r.val = rm.addr;
	operand_write(&r);
	
	print_asm_2("lea", "", len , &rm, &r);

	return len;
}*/
