#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	//printf("sreg= %x\n",sreg);
	//printf("offset= %x\n",offset);
	//printf("cpu.segReg[sreg].base = %x\n",cpu.segReg[sreg].base);
	/*if(sreg!=0&&sreg!=1&&sreg!=2&&sreg!=3){
		printf("Sreg wrong! Can't equal to %x\n", sreg);
		assert(0);	
	}*/
	return cpu.segReg[sreg].base + offset;
	//return offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	//cpu.segReg[sreg].base = 0x0;
	//cpu.segReg[sreg].limit = 0xffffffff;
	//printf("%x\n",cpu.segReg[sreg].index);
	//printf("%x\n",cpu.gdtr.base);
	//printf("%x\n",*get_mem_addr());
	uint32_t pos_in_desc_table=cpu.gdtr.base+cpu.segReg[sreg].index*8;
	//printf("%x\n",cpu.segReg[sreg].index);
	//printf("%x\n",Pos_In_Desc_Table);
	uint32_t val0 = paddr_read(pos_in_desc_table,4);
	uint32_t val1 = paddr_read(pos_in_desc_table+4,4);
	SegDesc segc;	
	segc.val[0]=val0;
	segc.val[1]=val1;
	//printf("%x\n",sreg);
	//printf("%x\n",val0);
	//printf("%x\n",val1);
	cpu.segReg[sreg].base=segc.base_15_0+(segc.base_23_16<<16)+(segc.base_31_24<<24) ;
	cpu.segReg[sreg].limit=segc.limit_15_0+(segc.limit_19_16<<16);
	cpu.segReg[sreg].type= segc.type+(segc.segment_type<<4);
	cpu.segReg[sreg].privilege_level=segc.privilege_level;
	cpu.segReg[sreg].soft_use=segc.soft_use;
	assert(cpu.segReg[sreg].base==0&&cpu.segReg[sreg].limit==0xfffff&&segc.granularity==1);
}
