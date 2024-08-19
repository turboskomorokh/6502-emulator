#include "cpu_65xx.h"

#include <climits>
#include <cstdint>

#include "common.h"
#include "memory.h"

void CPU_65XX::Reset(Memory& Mem) {
 PC = 0xFFFC;
 SP = 0xFF;
 PS.U = 1;
 Mem.Init();
}

int32_t CPU_65XX::EatCycles(int32_t amount) { return Cycles -= amount; }

Byte CPU_65XX::FetchByte(Memory& mem) {
 EatCycles(1);
 Byte Value = mem[PC];
 PC++;
 return Value;
}

Word CPU_65XX::FetchWord(Memory& mem) {
 EatCycles(2);
 Byte lo, hi;
 lo = mem[PC];
 PC++;
 hi = mem[PC];
 PC++;
 return (Word)(hi << 8) | lo;
}

Byte CPU_65XX::ReadByte(Memory& mem, Word Address) {
 EatCycles(1);
 return mem[Address];
}

Word CPU_65XX::ReadWord(Memory& mem, Word Address) {
 EatCycles(2);
 Byte lo = mem[Address];
 Byte hi = mem[Address + 1];
 return (Word)(hi << 8) | lo;
}

void CPU_65XX::WriteByte(Memory& mem, Word Address, Byte Value) {
 EatCycles(1);
 mem[Address] = Value;
}

void CPU_65XX::WriteWord(Memory& mem, Word Address, Word Value) {
 EatCycles(2);
 mem[Address] = (Value >> 8) & 0xFF;
 Address++;
 mem[Address] = (Value & 0xFF);
}

void CPU_65XX::StackPushByte(Memory& mem, Byte Value) {
 EatCycles(1);
 mem[0x100 + SP] = Value;
 SP--;
}

void CPU_65XX::StackPushWord(Memory& mem, Word Value) {
 EatCycles(2);
 mem[0x100 + SP] = Value >> 8;
 SP--;
 mem[0x100 + SP] = Value & 0xFF;
 SP--;
}

Byte CPU_65XX::StackPopByte(Memory& mem) {
 EatCycles(1);
 SP++;
 return mem[0x100 + SP];
}

Word CPU_65XX::StackPopWord(Memory& mem) {
 EatCycles(2);
 SP++;
 Byte lo = mem[0x100 + SP];
 SP++;
 Byte hi = mem[0x100 + SP];
 return (Word)(hi << 8) | lo;
}

Byte CPU_65XX::FetchZPAddress(Memory& mem, Byte Offset) {
 Byte Address = FetchByte(mem);
 if (Offset) {
  EatCycles(1);
  Address += Offset;
 }
 return Address;
}

Word CPU_65XX::FetchABAddress(Memory& mem, Byte Offset) {
 Word Address          = FetchWord(mem);

 Word EffectiveAddress = Address + Offset;

 if ((Address & 0xFF00) != (EffectiveAddress & 0xFF00)) EatCycles(1);
 return EffectiveAddress;
}

Word CPU_65XX::FetchINAddressX(Memory& mem) {
 Byte ZeroPageAddress = FetchByte(mem);
 Word IndirectAddress = (ZeroPageAddress + X) & 0xFF;
 EatCycles(1);

 Word EffectiveAddress = ReadWord(mem, IndirectAddress);
 return EffectiveAddress;
}

Word CPU_65XX::FetchINAddressY(Memory& mem) {
 Byte ZeroPageAddress  = FetchByte(mem);
 Word IndirectAddress  = ReadWord(mem, ZeroPageAddress);
 Word EffectiveAddress = IndirectAddress + Y;

 if ((IndirectAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
  EatCycles(1);
 }

 return EffectiveAddress;
}