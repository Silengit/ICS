#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	//printf("\nWelcome to my page!\n");
	//printf("laddr = %x\n", laddr);

	uint32_t dir_idx = (laddr >> 22) & 0x3ff;
	uint32_t page_idx = ((laddr << 10) >> 22) &0x3ff;
	uint32_t offset = ((laddr << 20) >> 20) & 0xfff;
	uint32_t temp;
	//printf("dir_idx= %x\n",dir_idx);
	//first, read the fucking directory entry
	temp = paddr_read(cpu.cr3.val+dir_idx*4,4);
	//printf("cpu.cr3.val+dir_idx*4 = %x\n", cpu.cr3.val+dir_idx*4);
	//printf("%x\n", temp);
	PDE dir_entry;
	//memcpy(&dir_entry, hw_mem + cpu.cr3.val + dir_idx*4, 4);
	dir_entry.val = temp;

	//printf("laddr = %x\n",laddr);
	assert(dir_entry.present == 1);

	//second, read the fucking page_addr
	temp = paddr_read((dir_entry.page_frame<<12) + page_idx*4,4);
	PTE page_entry;
	//memcpy(&page_entry, hw_mem +(dir_entry.page_frame<<12) + page_idx*4, 4);
	page_entry.val = temp;
	assert(page_entry.present == 1);

	uint32_t final_addr = (page_entry.page_frame<<12) + offset;
	return final_addr;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
