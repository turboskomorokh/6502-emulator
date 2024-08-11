
#ifndef _CPU_6502_H_
#define _CPU_6502_H_

#include <cstdint>
#include <stdint.h>

#include "common.h"
#include "cpu_65xx.h"
#include "memory.h"



struct CPU_6502 : CPU_65XX {

 void Execute(Memory& memory);

 void EatCycles(uint32_t amount /* amount of cycles to consume */);
};

#endif