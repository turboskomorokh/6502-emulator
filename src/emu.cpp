#include <fstream>
#include <string>

#include "memory.h"
#include "cpu_6502.h"

using std::fstream, std::ios;

int main(int argc, char **argv) {
  if(!argv[1]) {
    printf("Usage: emulator [binary file] [cycles]");
  }
 Memory mem;
 CPU_6502 cpu;
 cpu.Reset(mem);
 
 fstream program(argv[1], ios::binary | ios::in);
 mem.Read(program);

 cpu.Execute(std::stoi((std::string)argv[2]), mem);
 mem.PrintRange(0x0100, 0x0200);
 mem.PrintRange(0x2000, 0x2100);
 mem.PrintRange(0x8000, 0x8100);
 mem.PrintRange(0xFFFA, 0xFFFF);
 cpu.PrintFlags();
 cpu.PrintRegisters();
 return 0;
}
