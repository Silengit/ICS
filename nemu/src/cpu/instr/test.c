#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	uint32_t temp = alu_and(opr_src.val,opr_dest.val);
	if(opr_src.data_size == 8 || opr_dest.data_size == 8)
		cpu.eflags.SF = (temp & 0x80) ? 1 : 0;
}


/*make_instr_func(test_r2rm_v)
{
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&r);
	operand_read(&rm);
	alu_and(r.val,rm.val);

	print_asm_2("test","",len,&r,&rm);

	return len;
}*/

make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, i, rm, v)
make_instr_impl_2op(test, i, a, v)
make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, rm, b)
