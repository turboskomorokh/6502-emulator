#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <fstream>

#include <cstdint>
#include <cstdio>

#include "common.h"

constexpr uint32_t MAX_MEM = 1024 * 64;

struct Memory {
 Byte Data[MAX_MEM];

 void Init();

 void PrintRange(Word Begin, Word End);

 bool ReadProgram(std::ifstream& Binary, Word StartAddress = 0x0, Word EndAddress = MAX_MEM - 1);

 // Memory interface
 Byte operator[](Word Address) const { return Data[Address]; }
 Byte& operator[](Word Address) { return Data[Address]; }
};

#endif