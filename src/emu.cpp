#include <stdlib.h>

#include "memory.h"
#include "cpu_6502.h"

int main() {
 Memory mem;
 CPU_6502 cpu;
 cpu.Reset(mem, 0x0);
 FILE* fp = fopen("a.out", "rb");
 mem.ReadBin(fp);
 free(fp);

 cpu.Execute(20, mem);
 mem.PrintRange(0x0000, 0x0050);
 cpu.PrintFlags();
 cpu.PrintRegisters();
 return 0;
}