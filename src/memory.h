#ifndef _6502_MEMORY_H_
#define _6502_MEMORY_H_

#include <fstream>
#include <cstdint>

#include "common.h"

constexpr uint32_t MAX_MEM = 1024 * 64;

struct Memory {
 Byte Data[MAX_MEM];

 void Init();

 // Read bytes from file to memory struct
 void Read(std::fstream& program);

 // Verbose function
 void PrintRange(Word addr, Word end);

 Byte operator[](Word Address) const { return Data[Address]; }
 Byte& operator[](Word Address) { return Data[Address]; }
};

#endif