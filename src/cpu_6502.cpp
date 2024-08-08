#include <cstdint>
#include <stdint.h>

#include "common.h"
#include "cpu_6502.h"
#include "memory.h"

void CPU_6502::PrintFlags() {
 printf("\nCPU Flags:\n"
        "Carry     (C): 0x%02x\n"
        "Zero      (Z): 0x%02x\n"
        "Interrupt (I): 0x%02x\n"
        "Decimal   (D): 0x%02x\n"
        "Break     (B): 0x%02x\n"
        "Overflow  (O): 0x%02x\n"
        "Negative  (N): 0x%02x\n",
        Flag.C, Flag.Z, Flag.I, Flag.D, Flag.B, Flag.O, Flag.N);
}
void CPU_6502::PrintRegisters() {
 printf("\nCPU Registers:\n"
        "A: 0x%02x\n"
        "X: 0x%02x\n"
        "Y: 0x%02x\n",
        A, X, Y);
}

void CPU_6502::Reset(Memory& mem, Word ResetAddress = 0xFFFC /* Something about debug purposes */) {
 PC     = ResetAddress;
 SP     = 0x01FF;
 Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.O = Flag.N = 0;
 A = X = Y = 0;
 mem.Init();
}

Byte CPU_6502::GetProcessorStatus() {
 Byte Status = 0;
 Status |= (Flag.C ? 0x01 : 0x00);
 Status |= (Flag.Z ? 0x02 : 0x00);
 Status |= (Flag.I ? 0x04 : 0x00);
 Status |= (Flag.D ? 0x08 : 0x00);
 Status |= (Flag.B ? 0x10 : 0x00);
 Status |= (Flag.O ? 0x40 : 0x00);
 Status |= (Flag.N ? 0x80 : 0x00);
 return Status;
}

// Cycles: 1
Byte CPU_6502::GetProcessorStatus(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 return GetProcessorStatus();
}

void CPU_6502::SetProcessorStatus(Byte ProcessorStatus) {
 Flag.C = (ProcessorStatus & 0x01);
 Flag.Z = (ProcessorStatus & 0x02);
 Flag.I = (ProcessorStatus & 0x04);
 Flag.D = (ProcessorStatus & 0x08);
 Flag.B = (ProcessorStatus & 0x10);
 Flag.O = (ProcessorStatus & 0x20);
 Flag.N = (ProcessorStatus & 0x80);
}

// Cycles: 1
void CPU_6502::SetProcessorStatus(uint32_t& Cycles, Byte ProcessorStatus) {
 EatCycles(Cycles, 1);
 SetProcessorStatus(ProcessorStatus);
}

// Cycles: amount
void CPU_6502::EatCycles(uint32_t& Cycles, uint32_t amount /* amount of cycles to consume */) {
 if (Cycles >= amount)
  Cycles -= amount;
 else
  Cycles = 0;
}

// Cycles: 1
void CPU_6502::StackPushByte(uint32_t& Cycles, Memory& Memory, Byte M) {
 SP--;
 Memory[SP] = M;
 EatCycles(Cycles, 1);
}

// Cycles: 2
void CPU_6502::StackPushWord(uint32_t& Cycles, Memory& Memory, Word M) {
 StackPushByte(Cycles, Memory, M);
 StackPushByte(Cycles, Memory, M >> 8);
}

// Cycles: 1
Byte CPU_6502::StackPullByte(uint32_t& Cycles, Memory& Memory) {
 SP++;
 Byte M = Memory[SP];
 EatCycles(Cycles, 1);
 return M;
}

// Cycles: 2
Word CPU_6502::StackPullWord(uint32_t& Cycles, Memory& Memory) {
 Byte hi = StackPullByte(Cycles, Memory);
 Byte lo = StackPullByte(Cycles, Memory);
 Word M  = (hi << 8) | lo;
 return M;
}

// Cycles: 1
Byte CPU_6502::FetchByte(uint32_t& Cycles, Memory& Memory) {
 EatCycles(Cycles, 1);
 Byte Data = Memory[PC];
 PC++;

 return Data;
}

// Cycles: 2
Word CPU_6502::FetchWord(uint32_t& Cycles, Memory& Memory) {
 Byte lo   = FetchByte(Cycles, Memory);
 Byte hi   = FetchByte(Cycles, Memory);
 Word word = (Word)(hi << 8) | lo;

 return word;
}

// Cycles: 1
Byte CPU_6502::ReadByte(uint32_t& Cycles, Word Address, Memory& Memory) {
 EatCycles(Cycles, 1);
 Byte Data = Memory[Address];

 return Data;
}

// Cycles: 2
Word CPU_6502::ReadWord(uint32_t& Cycles, Word Address, Memory& Memory) {
 Byte lo   = ReadByte(Cycles, Address, Memory);
 Byte hi   = ReadByte(Cycles, Address + 1, Memory);
 Word word = (Word)(hi << 8) | lo;

 return word;
}

// Cycles: 1 (+1 on offset)
Byte CPU_6502::FetchZeroPageAddress(uint32_t& Cycles, Memory& Memory, Byte Offset) {
 Byte Address = FetchByte(Cycles, Memory);
 if (Offset) {
  EatCycles(Cycles, 1);
  Address = (Address + Offset) & 0xFF;
 }
 return Address;
}

// Cycles: 2 (+1 on crossing page)
Word CPU_6502::FetchAbsoluteAddress(uint32_t& Cycles, Memory& Memory, Byte Offset) {
 Word Address          = FetchWord(Cycles, Memory);
 Word EffectiveAddress = Address + Offset;

 // Page crossing
 if (((Address & 0xFF00) != (EffectiveAddress & 0xFF00))) {
  EatCycles(Cycles, 1);
 }

 return EffectiveAddress;
}

// Cycles: 4
Word CPU_6502::FetchIndirectAddressX(uint32_t& Cycles, Memory& memory) {
 Byte ZeroPageAddress = FetchByte(Cycles, memory);
 Word IndirectAddress = (ZeroPageAddress + X) & 0xFF;
 EatCycles(Cycles, 1);

 Word EffectiveAddress = ReadWord(Cycles, IndirectAddress, memory);
 return EffectiveAddress;
}

// Cycles: 3 (+1 on crossing page)
Word CPU_6502::FetchIndirectAddressY(uint32_t& Cycles, Memory& memory) {
 Byte ZeroPageAddress  = FetchByte(Cycles, memory);
 Word IndirectAddress  = ReadWord(Cycles, ZeroPageAddress, memory);
 Word EffectiveAddress = IndirectAddress + Y;

 if (((IndirectAddress & 0xFF00) != (EffectiveAddress & 0xFF00))) {
  EatCycles(Cycles, 1);
 }

 return EffectiveAddress;
}

void CPU_6502::WriteValueToAddress(uint32_t Cycles, Memory& memory, Word Address, Byte M) {
 EatCycles(Cycles, 1);
 memory[Address] = M;
}

void CPU_6502::Execute(uint32_t Cycles, Memory& memory) {
 while (Cycles > 0) {
  printf("PC 0x%04x; ", PC);
  Byte Ins = FetchByte(Cycles, memory);
  printf("Instruction: 0x%02x \n", Ins);
  switch (Ins) {

  // Cycles: 1
  case INS_ADC_IM: {
   Byte Value = FetchByte(Cycles, memory);
   ADC(Cycles);
  } break;

  // Cycles: 2
  case INS_ADC_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 3
  case INS_ADC_ZPX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 3
  case INS_ADC_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_ADC_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_ADC_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 5
  case INS_ADC_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_ADC_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ADC(Value);
  } break;

  // Cycles: 1
  case INS_AND_IM: {
   Byte Value = FetchByte(Cycles, memory);
   AND(Value);
  } break;

  // Cycles: 2
  case INS_AND_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 3
  case INS_AND_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 3
  case INS_AND_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_AND_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_AND_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 5
  case INS_AND_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   AND(Value);
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_AND_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
  } break;

  // Cycles: 1
  case INS_ASL_A: {
   ASL(Cycles, A);
  } break;

  // Cycles: 4
  case INS_ASL_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = FetchZeroPageAddress(Cycles, memory);
   ASL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5 (+1 if crossed page)
  case INS_ASL_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ASL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_ASL_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ASL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5 (+1 if crossed page)
  case INS_ASL_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ASL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BCC_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BCC(Cycles, Offset);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BCS_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BCS(Cycles, Offset);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BEQ_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BEQ(Cycles, Offset);
  } break;

  // Cycles: 2
  case INS_BIT_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   BIT(Value);
  } break;

  // Cycles: 3
  case INS_BIT_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BMI_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BMI(Cycles, Offset);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BNE_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BNE(Cycles, Offset);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BPL_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BPL(Cycles, Offset);
  } break;

  // Cycles: 6
  case INS_BRK_IMPL: {
   BRK(Cycles, memory);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BVC_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BVC(Cycles, Offset);
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BVS_REL: {
   Byte Offset = FetchByte(Cycles, memory);
   BVS(Cycles, Offset);
  } break;

   // Cycles: 1
  case INS_CLC_IMPL: {
   CLC(Cycles);
  } break;

   // Cycles: 1
  case INS_CLD_IMPL: {
   CLD(Cycles);
  } break;

   // Cycles: 1
  case INS_CLI_IMPL: {
   CLI(Cycles);
  } break;

  // Cycles: 1
  case INS_CLV_IMPL: {
   CLV(Cycles);
  } break;

  // Cycles: 1
  case INS_CMP_IM: {
   Byte Value = FetchByte(Cycles, memory);
   CMP(Value);
  } break;

  // Cycles: 2
  case INS_CMP_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 3
  case INS_CMP_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 3
  case INS_CMP_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_CMP_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_CMP_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 5
  case INS_CMP_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 4 (+1 on crossing page)
  case INS_CMP_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CMP(Value);
  } break;

  // Cycles: 1
  case INS_CPX_IM: {
   Byte Value = FetchByte(Cycles, memory);
   CPX(Value);
  } break;

  // Cycles: 2
  case INS_CPX_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CPX(Value);
  } break;

   // Cycles: 3
  case INS_CPX_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CPX(Value);
  } break;

   // Cycles: 1
  case INS_CPY_IM: {
   Byte Value = FetchByte(Cycles, memory);
   CPY(Value);
  } break;

   // Cycles: 2
  case INS_CPY_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CPY(Value);
  } break;

   // Cycles: 3
  case INS_CPY_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   CPY(Value);
  } break;

   // Cycles: 4
  case INS_DEC_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   DEC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_DEC_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   DEC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_DEC_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   DEC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 6
  case INS_DEC_ABX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   DEC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 1
  case INS_DEX_IMPL: {
   DEX(Cycles);
  } break;

  // Cycles: 1
  case INS_DEY_IMPL: {
   DEX(Cycles);
  } break;

  // Cycles: 1
  case INS_EOR_IM: {
   Byte Value = FetchByte(Cycles, memory);
   EOR(Value);
  } break;

  // Cycles: 2
  case INS_EOR_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 3
  case INS_EOR_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 3
  case INS_EOR_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 3
  case INS_EOR_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 3
  case INS_EOR_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 5
  case INS_EOR_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_EOR_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   EOR(Value);
  } break;

  // Cycles: 4
  case INS_INC_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   INC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_INC_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   INC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_INC_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   INC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_INC_ABX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   INC(Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 1
  case INS_INX_IMPL: {
   INX(Cycles);
  } break;

  // Cycles: 1
  case INS_INY_IMPL: {
   INY(Cycles);
  } break;

  // Cycles: 2
  case INS_JMP_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   JMP(Address);
  } break;

  // Cycles: 4
  case INS_JMP_IN: {
   Word Address          = FetchAbsoluteAddress(Cycles, memory);
   Word EffectiveAddress = ReadWord(Cycles, Address, memory);
   JMP(Address);
  } break;

  // Cycles: 5
  case INS_JSR_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   JSR(Cycles, memory, Address);
  } break;

  // Cycles: 1
  case INS_LDA_IM: {
   Byte Value = FetchByte(Cycles, memory);
   LDA(Value);
  } break;

   // Cycles: 2
  case INS_LDA_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 3
  case INS_LDA_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 3
  case INS_LDA_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 3
  case INS_LDA_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 3
  case INS_LDA_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 5
  case INS_LDA_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDA(Value);
  } break;

   // Cycles: 4 (+1 if crossed page)
  case INS_LDA_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDX(Value);
  } break;

   // Cycles: 1
  case INS_LDX_IM: {
   Byte Value = FetchByte(Cycles, memory);
   LDX(Value);
  } break;

   // Cycles: 2
  case INS_LDX_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDX(Value);
  } break;

   // Cycles: 3
  case INS_LDX_ZPY: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDX(Value);
  } break;

   // Cycles: 3
  case INS_LDX_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDX(Value);
  } break;

   // Cycles: 3
  case INS_LDX_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDX(Value);
  } break;

  // Cycles: 1
  case INS_LDY_IM: {
   Byte Value = FetchByte(Cycles, memory);
   LDY(Value);
  } break;

   // Cycles: 2
  case INS_LDY_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDY(Value);
  } break;

   // Cycles: 3
  case INS_LDY_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDY(Value);
  } break;

   // Cycles: 3
  case INS_LDY_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDY(Value);
  } break;

   // Cycles: 3
  case INS_LDY_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LDY(Value);
  } break;

  // Cycles: 1
  case INS_LSR_A: {
   LSR(Cycles, A);
  } break;

  // Cycles: 4
  case INS_LSR_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LSR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_LSR_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LSR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_LSR_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LSR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 6
  case INS_LSR_ABX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   LSR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  case INS_NOP_IMPL: {
   NOP(Cycles);
  } break;

  // Cycles: 1
  case INS_ORA_IM: {
   Byte Value = FetchByte(Cycles, memory);
   ORA(Value);
  } break;

  // Cycles: 2
  case INS_ORA_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 3
  case INS_ORA_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 3
  case INS_ORA_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 3
  case INS_ORA_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 3
  case INS_ORA_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 5
  case INS_ORA_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_ORA_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ORA(Value);
  } break;

  // Cycles: 1
  case INS_PHA_IMPL: {
   PHA(Cycles, memory);
  } break;
  
  // Cycles: 1
  case INS_PHP_IMPL: {
   PHP(Cycles, memory);
  } break;
  
  // Cycles: 1
  case INS_PLA_IMPL: {
   PLA(Cycles, memory);
  } break;
  
  // Cycles: 1
  case INS_PLP_IMPL: {
   PLP(Cycles, memory);
  } break;

  // Cycles: 1
  case INS_ROL_A: {
   ROL(Cycles, A);
  } break;
  
  // Cycles: 4
  case INS_ROL_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;
  // Cycles: 5
  case INS_ROL_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;
  // Cycles: 5
  case INS_ROL_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 6
  case INS_ROL_ABX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROL(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 1
  case INS_ROR_A: {
   ROR(Cycles, A);
  } break;
  
  // Cycles: 4
  case INS_ROR_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;
  // Cycles: 5
  case INS_ROR_ZPX: {
   Byte Address = FetchZeroPageAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  }break;
  // Cycles: 5
  case INS_ROR_AB: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 6
  case INS_ROR_ABX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   ROR(Cycles, Value);
   WriteValueToAddress(Cycles, memory, Address, Value);
  } break;

  // Cycles: 5
  case INS_RTI_IMPL: { 
    RTI(Cycles, memory);
  } break;
  
  // Cycles: 5
  case INS_RTS_IMPL: {
    RTS(Cycles, memory);
  } break;

  // Cycles: 1
  case INS_SBC_IM: {
   Byte Value = FetchByte(Cycles, memory);
   SBC(Cycles);
  } break;

  // Cycles: 2
  case INS_SBC_ZP: {
   Byte Address = FetchZeroPageAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 3
  case INS_SBC_ZPX: {
   Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 3
  case INS_SBC_AB: {
   Word Address = FetchAbsoluteAddress(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_SBC_ABX: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, X);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_SBC_ABY: {
   Word Address = FetchAbsoluteAddress(Cycles, memory, Y);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 5
  case INS_SBC_INX: {
   Word Address = FetchIndirectAddressX(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_SBC_INY: {
   Word Address = FetchIndirectAddressY(Cycles, memory);
   Byte Value   = ReadByte(Cycles, Address, memory);
   SBC(Value);
  } break;

  // Cycles: 1
  case INS_SED_IMPL: {
    SED(Cycles);
  } break;
  
  // Cycles: 1
  case INS_SEC_IMPL: {
    SEC(Cycles);
  } break;

  // Cycles: 1
  case INS_SEI_IMPL: {
    SEI(Cycles);
  } break;

  // Cycles: 2
  case INS_STA_ZP: {
    Byte Address = FetchZeroPageAddress(Cycles, memory);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STA_ZPX: {
    Byte Address = FetchZeroPageAddress(Cycles, memory, X);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STA_AB: {
    Byte Address = FetchAbsoluteAddress(Cycles, memory);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;
  
  // Cycles: 3 (+1 if crossed page)
  case INS_STA_ABX: {
    Byte Address = FetchAbsoluteAddress(Cycles, memory, X);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;

  // Cycles: 3 (+1 if crossed page)
  case INS_STA_ABY: {
    Byte Address = FetchAbsoluteAddress(Cycles, memory, Y);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;
 
  // Cycles: 5
  case INS_STA_INX: {
    Byte Address = FetchIndirectAddressX(Cycles, memory);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_STA_INY: {
    Byte Address = FetchIndirectAddressY(Cycles, memory);
    Byte Value = ReadByte(Cycles, Address, memory);

    STA(Cycles, memory, Address);
  } break;

  // Cycles: 2
  case INS_STX_ZP: {
    Byte Address = FetchZeroPageAddress(Cycles, memory);
    STX(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STX_ZPY: {
    Byte Address = FetchZeroPageAddress(Cycles, memory, Y);
    STX(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STX_AB: {
    Byte Address = FetchAbsoluteAddress(Cycles, memory);
    STX(Cycles, memory, Address);
  } break;

  // Cycles: 2
  case INS_STY_ZP: {
    Byte Address = FetchZeroPageAddress(Cycles, memory);
    STY(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STY_ZPX: {
    Byte Address = FetchZeroPageAddress(Cycles, memory, X);
    STY(Cycles, memory, Address);
  } break;

  // Cycles: 3
  case INS_STY_AB: {
    Byte Address = FetchAbsoluteAddress(Cycles, memory);
    STY(Cycles, memory, Address);
  } break;

  case INS_TAY_IMPL: {
    TAY(Cycles);
  } break;

  case INS_TXA_IMPL: {
    TXA(Cycles);
  } break;

  case INS_TAX_IMPL: {
    TAX(Cycles);
  } break;

  case INS_TSX_IMPL: {
    TSX(Cycles);
  } break;

  case INS_TXS_IMPL: {
    TXS(Cycles);
  } break;
  }
 }
}