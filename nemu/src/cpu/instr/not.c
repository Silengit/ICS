#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_src.val=~opr_src.val;
	//printf("not:addr = %x\n",opr_src.addr);
	operand_write(&opr_src);
}

make_instr_impl_1op(not, rm, v)
