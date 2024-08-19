#include <chrono>
#include <cstdio>
#include <fstream>
#include <sys/types.h>
#include <thread>

#include "common.h"
#include "cpu_6502.h"
#include "parser.h"

int32_t workCycles = 1000;
uint32_t startPC   = 0x8000;
uint32_t tickSpeed = 0;

std::string binPath = "program.bin";

int main(int argc, char** argv) {
 CPU_6502 cpu;
 Memory mem;

 if (!argv[1]) {
  printf("Usage: emulator [program] [Cycles]\n");
  return 1;
 }
 cpu.Reset(mem);

 parseArgs(argv);

 std::ifstream binFile(binPath, std::ios::binary);
 mem.ReadProgram(binFile, 0x0, 0xFFFF);
 binFile.close();

 cpu.PC = startPC;

 Word loop;
 for (; workCycles > 0; workCycles--) {
  //  Word OldPc = cpu.PC;
  cpu.Execute(1, mem);

  // Check PC loop
  //   if (OldPc == cpu.PC)
  //    loop++;
  //   if (loop > 5)
  //    break;

  std::this_thread::sleep_for(std::chrono::nanoseconds(tickSpeed));
 }
}
