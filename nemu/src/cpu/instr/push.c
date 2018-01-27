#include "cpu/instr.h"

static void instr_execute_1op() {
	//cpu.esp-=data_size/8;
	assert(data_size == 32);
	cpu.esp -= 4;
	//opr_src.addr=cpu.eip+1;
	operand_read(&opr_src);
	int offset = sign_ext(opr_src.val, opr_src.data_size);
	//printf("offset = %x\n", offset);
	//vaddr_write(cpu.esp, 2, data_size/8, offset);
	vaddr_write(cpu.esp, 2, 4, offset);
	/*if(opr_src.val ==  0&& opr_src.data_size==8){
		printf("push:0x00 esp = %x\n",cpu.esp);
	}
	else if(opr_src.val == 0x80 && opr_src.data_size == 32){
		printf("push:0x80 esp = %x\n",cpu.esp);
	}*/
	//memcpy(hw_mem+cpu.esp, &offset, data_size/8);
}


make_instr_func(push_r_v)
{
	assert(data_size == 32);
	//cpu.esp-=data_size/8;
	uint32_t temp = cpu.esp;
	cpu.esp -= 4;
	//printf("opcode = %x\n", opcode);
	if(opcode == 0x54){//push(esp)
		vaddr_write(cpu.esp, 2, 4, temp);
	}
	else{
		uint32_t val=cpu.gpr[opcode-0x50].val;
		//printf("%x\n",val);
		//memcpy(hw_mem+cpu.esp, &val, data_size/8);
		//vaddr_write(cpu.esp, 2, data_size/8, val);
		vaddr_write(cpu.esp, 2, 4, val);
		//printf("reg = %x\n", vaddr_read(cpu.esp,1,data_size/8));
	}
	print_asm_0("push","" , 1);
	return 1;
}

make_instr_func(push_m_v)//??
{
	assert(data_size == 32);
	//cpu.esp-=data_size/8;
	cpu.esp -= 4;
	
	OPERAND m;
	m.data_size=data_size;
	int len=1;
	len += modrm_rm(eip + 1, &m);
	operand_read(&m);
	//memcpy(hw_mem+cpu.esp, &(m.val), data_size/8);
	//vaddr_write(cpu.esp, 2, data_size/8, m.val);
	vaddr_write(cpu.esp, 2, 4, m.val);
	print_asm_1("push","" , len,&m);
	return len;
}

make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha)
{
	uint32_t temp = cpu.esp;
	//printf("irq = %x\n", vaddr_read(cpu.esp,1,4));
	//printf("irq = %x\n", vaddr_read(cpu.esp+4,1,4));
	//printf("pusha:esp = %x\n", cpu.esp);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.eax);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.ecx);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.edx);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.ebx);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, temp);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.ebp);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.esi);
	cpu.esp -= 4;
	vaddr_write(cpu.esp, 2, 4, cpu.edi);
	//printf("irq = %x\n", vaddr_read(cpu.esp+32,1,4));
	//printf("pusha:esp = %x\n", cpu.esp);
	print_asm_0("pusha","" , 1);
	return 1;
}



