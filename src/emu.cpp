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

 if(argv[2])
  cpu.Cycles = std::stoi((std::string)argv[2]);
 else
  cpu.Cycles = 1000;
 fstream program(argv[1], ios::binary | ios::in);
 mem.Read(program);

 

 cpu.Execute(mem);
 mem.PrintRange(0x0100, 0x0200);
 mem.PrintRange(0x2000, 0x2020);
 mem.PrintRange(0x8000, 0x8020);
 mem.PrintRange(0xFFFA, 0xFFFF);
 
 cpu.ShowFlags();
 cpu.ShowRegisters();
 return 0;
}
