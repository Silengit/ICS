#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#define CACHELINE_SIZE_B 1024

typedef struct{//every Cacheline has 1 valid bit and 27 bits address
	union{
		struct{
			uint32_t V:1;//valid bit
			uint32_t Tag:19;//Tag 14 bits  
			uint32_t invalid:12;
			//uint32_t cache_id;//cache_id 7 bits
			//uint32_t data:6;//data 6 bits;
		};
		uint32_t val;
	};
	uint8_t cache_mem[64];
}CacheLine;

extern CacheLine L1_dcache[];

//initialize the cache by clearing it
void init_cache();

//read memory with cache and size of bytes
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine *cache);

// write data to cache with memory address and size of bytes
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine *cache);

#endif

