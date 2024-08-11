#include "cpu_65xx.h"

#include "memory.h"

void CPU_65XX::ShowFlags() {
 printf("\nCPU Flags:\n"
        "Carry     (C): 0x%02x\n"
        "Zero      (Z): 0x%02x\n"
        "Interrupt (I): 0x%02x\n"
        "Decimal   (D): 0x%02x\n"
        "Break     (B): 0x%02x\n"
        "Overflow  (O): 0x%02x\n"
        "Negative  (N): 0x%02x\n",
        SREG.C, SREG.Z, SREG.I, SREG.D, SREG.B, SREG.O, SREG.N);
}

void CPU_65XX::ShowRegisters() {
 printf("\nCPU Registers:\n"
        "A: 0x%02x\n"
        "X: 0x%02x\n"
        "Y: 0x%02x\n",
        A, X, Y);
}

Byte CPU_65XX_SREG::GetSREG() const {
 Byte SREG = 0;
 SREG |= (Z ? ZeroBit             : 0x00);
 SREG |= (C ? CarryBit            : 0x00);
 SREG |= (I ? InterruptDisableBit : 0x00);
 SREG |= (B ? BreakBit            : 0x00);
 SREG |= (D ? UnusedBit           : 0x00);
 SREG |= (O ? OverflowBit         : 0x00);
 SREG |= (N ? NegativeBit         : 0x00);
 return SREG;
}

void CPU_65XX_SREG::SetSREG(Byte SREG) {
 Z = (SREG & ZeroBit);
 C = (SREG & CarryBit);
 I = (SREG & InterruptDisableBit);
 B = (SREG & BreakBit);
 D = (SREG & UnusedBit);
 O = (SREG & OverflowBit);
 N = (SREG & NegativeBit);
}

void CPU_65XX::Reset(Memory& mem) {
 PC   = 0xFFFC;
 SP   = 0xFF;
 SREG = 0x00;
 A = X = Y = 0;
 mem.Init();
}

void CPU_65XX::EatCycles(uint32_t amount) {
 if (Cycles >= amount)
  Cycles -= amount;
 else
  Cycles = 0;
}

// Cycles: 1
void CPU_65XX::StackPushByte(Memory& Memory, Byte M) {
 EatCycles(1);
 SP--;
 Memory[0x100 + SP] = M;
}

// Cycles: 2
void CPU_65XX::StackPushWord(Memory& Memory, Word M) {
 StackPushByte(Memory, M);
 StackPushByte(Memory, M >> 8);
}

// Cycles: 1
Byte CPU_65XX::StackPopByte(Memory& Memory) {
 EatCycles(1);
 Byte M = Memory[0x100 + SP];
 SP++;
 return M;
}

// Cycles: 2
Word CPU_65XX::StackPopWord(Memory& Memory) {
 Byte hi = StackPopByte(Memory);
 Byte lo = StackPopByte(Memory);
 Word M  = (hi << 8) | lo;
 return M;
}

// Cycles: 1
Byte CPU_65XX::FetchByte(Memory& Memory) {
 EatCycles(1);
 Byte Data = Memory[PC];
 PC++;

 return Data;
}

// Cycles: 2
Word CPU_65XX::FetchWord(Memory& Memory) {
 Byte lo   = FetchByte(Memory);
 Byte hi   = FetchByte(Memory);
 Word word = (Word)(hi << 8) | lo;

 return word;
}

// Cycles: 1
Byte CPU_65XX::ReadByte(Word Address, Memory& Memory) {
 EatCycles(1);
 Byte Data = Memory[Address];

 return Data;
}

// Cycles: 2
Word CPU_65XX::ReadWord(Word Address, Memory& Memory) {
 Byte lo   = ReadByte(Address, Memory);
 Byte hi   = ReadByte(Address + 1, Memory);
 Word word = (Word)(hi << 8) | lo;

 return word;
}

// Cycles: 1
void CPU_65XX::WriteByte(Memory& memory, Word Address, Byte M) {
 EatCycles(1);
 memory[Address] = M;
}

// Cycles: 2
void CPU_65XX::WriteWord(Memory& memory, Word Address, Word M) {
 WriteByte(memory, Address, M & 0xFF);
 WriteByte(memory, Address + 1, M >> 8);
}

// Cycles: 1 (+1 on Offset)
Byte CPU_65XX::FetchZPAddress(Memory& Memory, Byte Offset) {
 Byte ZPAddress = FetchByte(Memory);
 if (Offset) {
  EatCycles(1);
  ZPAddress += Offset;
 }
 return ZPAddress;
}

// Cycles: 2 (+1 on crossing page)
Word CPU_65XX::FetchABAddress(Memory& Memory, Byte Offset) {
 Word ABAddress = FetchWord(Memory);
 if ((ABAddress & 0xFF00) != ((ABAddress + Offset) & 0xFF00))
  EatCycles(1);
 return ABAddress;
}

// Cycles: 4
Word CPU_65XX::FetchINAddressX(Memory& memory) {
 Byte ZeroPageAddress = FetchByte(memory);
 Word IndirectAddress = (ZeroPageAddress + X) & 0xFF;
 EatCycles(1);

 Word EffectiveAddress = ReadWord(IndirectAddress, memory);
 return EffectiveAddress;
}

// Cycles: 3 (+1 on crossing page)
Word CPU_65XX::FetchINAddressY(Memory& memory) {
 Byte ZeroPageAddress  = FetchByte(memory);
 Word IndirectAddress  = ReadWord(ZeroPageAddress, memory);
 Word EffectiveAddress = IndirectAddress + Y;

 if (((IndirectAddress & 0xFF00) != (EffectiveAddress & 0xFF00))) {
  EatCycles(1);
 }

 return EffectiveAddress;
}
