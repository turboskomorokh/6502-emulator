#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "memory.h"

void Memory::Init() {
 for (uint32_t i = 0; i < MAX_MEM; i++) {
  Data[i] = 0;
 }
}

void Memory::ReadBin(FILE* fp) {
 char ch;
 uint32_t Addr = 0;
 while ((ch = fgetc(fp)) != EOF || Addr > MAX_MEM) {
  Data[Addr] = ch;
  Addr++;
 }
}

void Memory::PrintRange(Word addr, Word end) {
 printf("\nMemory dump 0x%04x-0x%04x:\n", addr, end);

 printf("0x%04x ", addr);
 while (addr <= end) {
  printf("0x%02x", Data[addr]);

  if ((addr + 1) % 8 == 0) {
   printf("\n");
   if (addr + 1 <= end) {
    printf("0x%04x ", addr + 1);
   }
  } else {
   printf(" ");
  }
  addr++;
 }
 printf("\n");
}

