#ifndef _6502_MEMORY_H_
#define _6502_MEMORY_H_

#define MAX_MEM 1024 * 64

#include "common.h"
#include <fstream>


struct Memory {
 Byte Data[MAX_MEM];

 void Init();
 void Read(std::fstream& program);
 void PrintRange(Word addr, Word end);

 Byte operator[](Word Address) const { return Data[Address]; }
 Byte& operator[](Word Address) { return Data[Address]; }
};

#endif