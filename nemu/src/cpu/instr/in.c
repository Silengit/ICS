#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in2al){
	uint16_t val = cpu.gpr[2]._16;
	cpu.gpr[0]._8[0] = pio_read(val,1);
	return 1;
}

make_instr_func(in2eax){
	uint16_t val = cpu.gpr[2]._16;
	if(data_size == 16){
		cpu.gpr[0]._16 = pio_read(val,2);
	}
	else{	
		cpu.gpr[0]._32 = pio_read(val,4);
	}	
	return 1;
}
