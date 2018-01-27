#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	//printf("mov:addr = %x\n",opr_dest.addr);
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

/*
make_instr_func(mov_r2rm_b) {
	src.data_size = 8;
	dest.data_size = 8;

	len += modrm_r_rm(eip + 1, &src, &dest);
	
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute();
	return len;
}



make_instr_func(mov_r2rm_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//printf("mov_r2rm rm addr = 0x%08x\n", rm.addr);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute(&r, &rm);
	return len;
}

make_instr_func(mov_rm2r_b) {
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_rm2r_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_i2rm_b) {
	OPERAND rm, imm;
	rm.data_size = 8;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);
	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + 1; 
}

make_instr_func(mov_i2rm_v) {
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.data_size = data_size;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);

	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + data_size / 8;
}
make_instr_func(mov_i2r_b) {
	OPERAND r, imm;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = 8;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 2;
}

make_instr_func(mov_i2r_v) {
	OPERAND r, imm;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = data_size;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 1 + data_size / 8;
}

make_instr_func(mov_off2a_b) {
	OPERAND r, m;ov_r2cr   3
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}


make_instr_func(mov_off2a_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;
	
	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}

make_instr_func(mov_a2off_b) {
	OPERAND r, m;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}

make_instr_func(mov_a2off_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}
*/

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	//printf("mov:addr = %x\n",r.addr);
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
	//printf("mov:addr = %x\n",r.addr);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
	//printf("mov:addr = %x\n",r.addr);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
	//printf("mov:addr = %x\n",r.addr);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_r2cr){
	OPERAND imm_8;
        imm_8.type = OPR_IMM;
	imm_8.data_size = 8;
        imm_8.addr = eip + 1;
	
        operand_read(&imm_8);
	uint32_t select = imm_8.val & 0x07;
	uint32_t cr_idx = (imm_8.val & 0x18)>>3;
	//printf("mov_r2cr   %d\n", cr_idx);
	//printf("%x\n", imm_8.val);
	if(cr_idx==0)
		cpu.cr0.val = cpu.gpr[select].val;
	else if(cr_idx==3)
		cpu.cr3.val = cpu.gpr[select].val;
	//printf("%x\n",cpu.cr0.pg);
	//printf("%x\n",cpu.cr0.val);
	//print_asm_1("mov_r2cr", "",2, &imm_8);

	return 2;
}

make_instr_func(mov_cr2r){//reverse
	OPERAND imm_8;
        imm_8.type = OPR_IMM;
	imm_8.data_size = 8;
        imm_8.addr = eip + 1;

        operand_read(&imm_8);
	uint32_t select = imm_8.val & 0x07;
	uint32_t cr_idx = (imm_8.val & 0x18)>>3;
	//printf("mov_cr2r   %d\n", cr_idx);
	if(cr_idx==0)
		cpu.gpr[select].val = cpu.cr0.val;
	else if(cr_idx==3){
		cpu.gpr[select].val = cpu.cr3.val;
		//printf("%d\n", cpu.cr3.val);	
	}
	//print_asm_1("mov_cr2r", "",2, &imm_8);

	return 2;	
}

make_instr_func(mov_sreg){
	OPERAND imm_8;
        imm_8.type = OPR_IMM;
	imm_8.data_size = 8;
        imm_8.addr = eip+1;

        operand_read(&imm_8);
	//printf("imm_8 = %x\n",imm_8.val);
	uint32_t select = imm_8.val & 0x07;
	//printf("%x\n",select);
	uint32_t sreg = (imm_8.val & 0x38)>>3;
	//printf("sreg = %x\n",sreg);
	//printf("mov_sreg   %d\n", sreg);
	cpu.segReg[sreg].val = cpu.gpr[select]._16;
	//print_asm_1("mov_sreg", "",2, &imm_8);
	//printf("%d\n",sreg);
	load_sreg(sreg);

	return 2;
}

