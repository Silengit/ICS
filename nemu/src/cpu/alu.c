#include "cpu/cpu.h"
uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t t=src+dest;
	set_CF(src,t);
	set_PF(t);
	set_OF(src,dest,t);
	set_SF(t);
	set_ZF(t);
	//set_AF(?) now not in consideration, following skip this.
	return t;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	
	uint32_t t=src+dest+cpu.eflags.CF;
	if((src==-1||dest==-1)&&cpu.eflags.CF==1)
	{	
		cpu.eflags.CF=1;//be careful, maybe false;(9.15.11:55)	
	}
	else
		set_CF(src,t);
	set_PF(t);
	set_OF(src+cpu.eflags.CF,dest,t);
	set_SF(t);
	set_ZF(t);
	return t;
}

uint32_t alu_sub(uint32_t src, uint32_t dest) {
	uint32_t t=dest-src; 
	set_CF(t,dest);//change the position
	set_PF(t);
	set_OF(src,t,dest);
	set_SF(t);
	set_ZF(t);
	return t;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	
	uint32_t t=dest-src-cpu.eflags.CF; 
	if((src==-1||t==-1)&&cpu.eflags.CF==1)
		cpu.eflags.CF=1;//be careful, maybe false;(9.15.23:33)
	else
		set_CF(t,dest);//change the position
	set_PF(t);
	set_OF(src+cpu.eflags.CF,t,dest);
	set_SF(t);
	set_ZF(t);	
	return t;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t t=(uint64_t)src*(uint64_t)dest;
	if(data_size==8)
	{
		uint32_t temp=t>>8;
		if((temp&0xff)!=0)
		{
			cpu.eflags.CF=1;
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.CF=0;
			cpu.eflags.OF=0;
		}
		
	}
	else if(data_size==16)
	{
		uint32_t temp=t>>16;
		if((temp&0xffff)!=0)
		{
			cpu.eflags.CF=1;
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.CF=0;
			cpu.eflags.OF=0;
		}
		
	}
	else
	{
		uint64_t temp=t>>32;
		if((temp&0xffffffff)!=0)
		{
			cpu.eflags.CF=1;
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.CF=0;
			cpu.eflags.OF=0;
		}
	}
	return t;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	int64_t t=(int64_t)src*(int64_t)dest;
	return t;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	uint64_t t=dest/src;
	return (uint32_t)t;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	int64_t t=dest/src;
	return (int32_t)t;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	uint64_t t=dest%src;
	return (uint32_t)t;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	int64_t t=dest%src;
	return (int32_t)t;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	uint32_t t=src&dest;
	set_CF(t,src);
	set_PF(t);
	set_SF(t);
	set_ZF(t);
	return t;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t t=src^dest;
	set_PF(t);
	set_SF(t);
	set_ZF(t);
	return t;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t t=src|dest;
	set_PF(t);
	set_SF(t);
	set_ZF(t);
	return t;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t t;	
	if(data_size==8)
	{
		uint32_t temp=dest&0xff;
		temp<<=src;
		temp&=0xff;
		set_SF(temp<<24);
		set_ZF(temp<<24);
		if(src<=8)
			cpu.eflags.CF=((dest&0xff)>>(8-src))&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffffff00;
		t=dest+temp;
	}
	else if(data_size==16)
	{
		uint32_t temp=dest&0xffff;
		temp<<=src;
		temp&=0xffff;
		set_SF(temp<<16);
		set_ZF(temp<<16);
		if(src<=16)
			cpu.eflags.CF=((dest&0xffff)>>(16-src))&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffff0000;
		t=dest+temp;
	}
	else
	{
		t=dest<<src;
		set_SF(t);
		set_ZF(t);
		if(src<=32)
			cpu.eflags.CF=((dest&0xffffffff)>>(32-src))&0x1;
		else 
			cpu.eflags.CF=0;
	}
	set_PF(t);
	return t;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t t;	
	if(data_size==8)
	{
		uint32_t temp=dest&0xff;
		temp>>=src;
		temp&=0xff;
		set_SF(temp<<24);
		set_ZF(temp<<24);
		if(src<=8)
			cpu.eflags.CF=(((dest&0xff)<<(8-src))>>7)&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffffff00;
		t=dest+temp;
	}
	else if(data_size==16)
	{
		uint32_t temp=dest&0xffff;
		temp>>=src;
		temp&=0xffff;
		set_SF(temp<<16);
		set_ZF(temp<<16);
		if(src<=16)
			cpu.eflags.CF=(((dest&0xffff)<<(16-src))>>15)&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffff0000;
		t=dest+temp;
	}
	else
	{
		t=dest>>src;
		set_SF(t);
		set_ZF(t);
		if(src<=32)
			cpu.eflags.CF=(((dest&0xffffffff)<<(32-src))>>31)&0x1;
		else 
			cpu.eflags.CF=0;
	}
	set_PF(t);
	return t;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t t;	
	if(data_size==8)
	{
		int temp=dest&0xff;		
		temp=(temp<<24)>>(src+24);
		temp&=0xff;
		set_SF(temp<<24);
		set_ZF(temp<<24);
		if(src<=8)
			cpu.eflags.CF=(((dest&0xff)<<(8-src))>>7)&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffffff00;
		t=dest+temp;
	}
	else if(data_size==16)
	{
		int temp=dest&0xffff;		
		temp=(temp<<16)>>(src+16);
		temp&=0xffff;
		set_SF(temp<<16);
		set_ZF(temp<<16);
		if(src<=16)
			cpu.eflags.CF=(((dest&0xffff)<<(16-src))>>15)&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffff0000;
		t=dest+temp;
	}
	else
	{
		t=(int)dest>>src;
		set_SF(t);
		set_ZF(t);
		if(src<=32)
			cpu.eflags.CF=(((dest&0xffffffff)<<(32-src))>>31)&0x1;
		else 
			cpu.eflags.CF=0;
	}
	set_PF(t);
	return t;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t t;	
	if(data_size==8)
	{
		uint32_t temp=dest&0xff;
		temp<<=src;
		temp&=0xff;
		set_SF(temp<<24);
		set_ZF(temp<<24);
		if(src<=8)
			cpu.eflags.CF=((dest&0xff)>>(8-src))&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffffff00;
		t=dest+temp;
	}
	else if(data_size==16)
	{
		uint32_t temp=dest&0xffff;
		temp<<=src;
		temp&=0xffff;
		set_SF(temp<<16);
		set_ZF(temp<<16);
		if(src<=16)
			cpu.eflags.CF=((dest&0xffff)>>(16-src))&0x1;
		else 
			cpu.eflags.CF=0;
		dest&=0xffff0000;
		t=dest+temp;
	}
	else
	{
		t=dest<<src;
		set_SF(t);
		set_ZF(t);
		if(src<=32)
			cpu.eflags.CF=((dest&0xffffffff)>>(32-src))&0x1;
		else 
			cpu.eflags.CF=0;
	}
	set_PF(t);
	return t;
}
