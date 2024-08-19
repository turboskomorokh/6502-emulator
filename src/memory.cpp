#include "memory.h"

#include <cstdlib>

#include "common.h"

void Memory::Init() {
 for (Word i; i < MAX_MEM - 1; i++) {
  Data[i] = 0;
 }
}
void Memory::PrintRange(Word Begin, Word End) {
 printf("\nMemory dump 0x%04x-0x%04x:\n", Begin, End);

 for (size_t i = Begin; i < End; i += 16) {
  printf("0x%04zx ", i);
  for (size_t j = 0; j < 16; j++) {
   if (i + j < End) {
    printf("0x%02x ", Data[i + j]);
   } else {
    printf("   ");
   }
  }
  printf("\n");
 }
}

bool Memory::ReadProgram(std::ifstream& Binary, Word StartAddress, Word EndAddress) {
 if (!Binary.is_open()) return EXIT_FAILURE;
 char buf;
 for (Word Address = StartAddress; Address <= EndAddress; Address++) {
  if (!Binary.get(buf)) break;
  Data[Address] = buf;
 }
 return EXIT_SUCCESS;
}