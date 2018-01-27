#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//printf("Please implement raise_intr()\n");
	//assert(0);

	//if(intr_no==0x80)
	//	printf("yes\n");

	//push(eflags)
	cpu.esp -= 4;
	uint32_t val = cpu.eflags.val;
	//printf("cpu.eflags.val   = %x\n",val);
	vaddr_write(cpu.esp, 2, 4, val);
	
	//push(cs)	
	cpu.esp -= 4;
	val = cpu.cs.val & 0x0000ffff;
	vaddr_write(cpu.esp, 2, 4, val);
	//printf("cpu.cs.val   = %x\n",val);
	/*cpu.esp -= 2;
	val = cpu.cs.val;
	vaddr_write(cpu.esp, 2, 2, val);
	//printf("cpu.cs.val   = %x\n",val);*/

	//push(eip)
	cpu.esp -= 4;
	val = cpu.eip;
	vaddr_write(cpu.esp, 2, 4, val);
	//printf("cpu.eip   = %x\n",cpu.eip);
	//Find the IDT entry using 'intr_no'
	
	uint32_t pos = cpu.idtr.base + intr_no*8;
	
	//printf("cpu.idtr.base = %x\n",cpu.idtr.base);
	//printf("pos  = %x\n",pos );
	//printf("intr_no  = %x\n",intr_no );
	//assert(0);
	
	//printf("fuck u!\n");

	uint32_t val0 = laddr_read(pos, 4);
	//printf("val0  = %x\n",val0 );
	//assert(0);

	uint32_t val1 = laddr_read(pos + 4, 4);
	GateDesc gatc;	
	gatc.val[0]=val0;
	gatc.val[1]=val1;
	
	//printf("cpu.eip   = %x\n",cpu.eip  );
	//printf("cpu.cs.val  = %x\n",cpu.cs.val );
	cpu.eip = gatc.offset_15_0 | (gatc.offset_31_16 << 16);
	//printf("cpu.eip   = %x\n",cpu.eip  );
	cpu.cs.val = gatc.selector;

	//clear IF
	//printf("gatc.type  = %x\n",gatc.type  ); 
	if(gatc.type == 6){
		//printf("yes\n");
		cpu.eflags.IF = 0;
	}
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
