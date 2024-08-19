#ifndef _CPU_6502_H_
#define _CPU_6502_H_

#include "cpu_65xx.h"

struct CPU_6502 : CPU_65XX {
 int32_t Execute(int32_t Cycles, Memory& Memory);
};

#endif