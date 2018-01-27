#include "memory/memory.h"
#include <stdlib.h>

CacheLine L1_dcache[CACHELINE_SIZE_B];

void init_cache() {
	// clear the cache on initiation
	for(int i=0;i<CACHELINE_SIZE_B;i++)
		L1_dcache[i].V=0;	
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine *cache) {

	uint32_t ret = 0;
	//memcpy(&ret, hw_mem + paddr, len);
	//printf("%x\n", paddr);
	//return ret;
	//first	
	//find the cache_id, which is in the middle 7 bits of paddr	
	uint32_t C_id=(paddr>>6)&0x0000007f;
	uint32_t C_tag=paddr>>13;
	uint32_t C_data=paddr&0x0000003f;

	if((paddr/64)<((paddr+len)/64)){//if overflow in one cache 
		memcpy(&ret, hw_mem + paddr, len);
		return ret;
	}

	//second
	//compare the Tag with the 8 cache lines in the group
	bool flag=0;
	for(int i=0;i<8;i++)
		if(C_tag==cache[C_id*8+i].Tag&&cache[C_id*8+i].V==1){// if catch it, then return the data in the cache;
			//time +10
			flag=1;
			//memcpy(&ret, hw_mem + paddr, len);
			memcpy(&ret, &(cache[C_id*8+i].cache_mem[C_data]), len);
			return ret;
		}

	bool overflow=1;
	if (flag==0){//miss it
		//time +100
		for(int i=0;i<8;i++)
			if(cache[C_id*8+i].V==0){
				//The group has at least 1 line
				overflow=0;
				cache[C_id*8+i].V=1;
				cache[C_id*8+i].Tag=C_tag;
				//fill the relative cache_mem 
				for(int j=0;j<64;j++)
					cache[C_id*8+i].cache_mem[j]=hw_mem[(paddr&0xffffffc0)+j];
				//memcpy(&ret, hw_mem + paddr, len);				
				memcpy(&ret, &(cache[C_id*8+i].cache_mem[C_data]), len);
				return ret;
			}
		if(overflow==1){//if the group is full
			int x = rand()%8;// random delete
			//cache[C_id*8+x].V=1;
			cache[C_id*8+x].Tag=C_tag;
			//fill the relative cache_mem 
			for(int j=0;j<64;j++)
				cache[C_id*8+x].cache_mem[j]=hw_mem[(paddr&0xffffffc0)+j];
			//memcpy(&ret, hw_mem + paddr, len);			
			memcpy(&ret, &(cache[C_id*8+x].cache_mem[C_data]), len);
			return ret;
		}
	}
	return ret;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine *cache) {
	//first	
	//find the cache_id, which is in the middle 7 bits of paddr	
	uint32_t C_id=(paddr>>6)&0x0000007f;
	uint32_t C_tag=paddr>>13;
	uint32_t C_data=paddr&0x0000003f;


	//second
	//compare the Tag with the 8 cache lines in the group
	if((paddr/64)==(paddr+len)/64)
		for(int i=0;i<8;i++)
			if(C_tag==cache[C_id*8+i].Tag&&cache[C_id*8+i].V==1)// if catch it, then return the data in the cache;
				//time +10
				//write cache
				memcpy(&(cache[C_id*8+i].cache_mem[C_data]), &data, len);

			//Attention: length has not been considered
	memcpy(hw_mem + paddr, &data, len);//Whatever, updata the data in the hw_mem
}




