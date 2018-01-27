#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out2al){
	uint16_t val = cpu.gpr[2]._16;
	pio_write(val, 1, cpu.gpr[0]._8[0]);
	return 1;
}
