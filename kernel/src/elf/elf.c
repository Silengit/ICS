#include "common.h"
#include "memory.h"
#include "string.h"
#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
//void ide_write(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;
#ifdef HAS_DEVICE_IDE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {

			//panic("Please implement the loader");

			/* TODO: copy the segment from the ELF file to its proper memory area */
			// load the file from the p_offset compared to the elf whose size is p_filesz to the memory beginning at ph->p_vaddr
#ifndef IA32_PAGE	
			memcpy((void *)ph->p_vaddr, (void *)elf+ph->p_offset, ph->p_filesz);	
			/* TODO: zeror the memory area [vaddr + file_sz, vaddr + mem_sz) */
			memset((void*)ph->p_vaddr+ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);			
#else
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
			uint32_t paddr = mm_malloc(ph->p_vaddr, ph->p_memsz);//here needs to be modified
			//Log("%x",paddr);
#ifdef	HAS_DEVICE_IDE 
			ide_read((uint8_t *)paddr, ph->p_offset, ph->p_filesz);		
#else
			memcpy((void *)paddr, (void *)elf+ph->p_offset, ph->p_filesz);
#endif
			memset((void*)paddr+ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
