#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(intb) {
	OPERAND imm;
	imm.data_size = 8;
	imm.type=OPR_IMM;
	imm.addr= eip + 1;
	//printf("eip+1  = %x\n",eip+1 );
	//panic("eip  = %x\n",eip );
	operand_read(&imm);
	//imm.val = vaddr_read(imm.addr, SREG_CS, 4);
	print_asm_1("intb","",2,&imm);
	//if(imm.val==0x80)
	//	printf("yes!\n");
	//printf("imm.val  = %x\n",imm.val );
	//printf("esp  = %x\n",cpu.esp  );
	raise_sw_intr(imm.val);	
	return 0;
}
