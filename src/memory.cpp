#include <cstdint>
#include <fstream>
#include <stdint.h>

#include "common.h"
#include "memory.h"

void Memory::Init() {
 for (uint32_t i = 0; i < MAX_MEM; i++) {
  Data[i] = 0;
 }
}

void Memory::Read(std::fstream& program) {
 char ch;
 uint32_t Addr = 0;
 while (Addr < MAX_MEM && program.get(ch)) {
  Data[Addr++] = ch;
 }
}

void Memory::PrintRange(Word addr, Word end) {
    printf("\nMemory dump 0x%04x-0x%04x:\n", addr, end);

    for (size_t i = addr; i < end; i += 8) {
        printf("0x%04zx ", i);
        for (size_t j = 0; j < 8; ++j) {
            if (i + j <= end) {
                printf("0x%02x ", Data[i + j]);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
}