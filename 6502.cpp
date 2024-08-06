#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <cstdint>

#define MAX_MEM 1024 * 64 - 1
#define PRINT_ROW_SIZE 8

typedef unsigned char Byte;
typedef unsigned short Word;

struct Memory {
 Byte Data[MAX_MEM];

 void Init() {
  for (Word i = 0; i < MAX_MEM; i++) {
   Data[i] = 0;
  }
 }

 void PrintRange(Word addr, Word end) {
  printf("\nMemory dump 0x%04x-0x%04x:\n", addr, end);
  while (addr != end || addr > MAX_MEM) {
   printf("0x%04x: ", addr);
   for (int i = 0; i < PRINT_ROW_SIZE && addr < end; i++) {
    printf("0x%02X ", Data[addr]);
    addr++;
   }
   printf("\n");
  }
  printf("\n");
 }
 void Print() {
  for (uint32_t addr = 0; addr < MAX_MEM;) {
   printf("0x%04x: ", addr);
   for (int i = 0; i < PRINT_ROW_SIZE; i++) {
    printf("0x%02X ", Data[addr]);
    addr++;
   }
   printf("\n");
  }
 }

 Byte operator[](Word Address) const { return Data[Address]; }

 Byte& operator[](Word Address) { return Data[Address]; }
};

struct CPU_6502 {
 Word PC; // Program Counter
 Word SP; // Stack pointer

 Byte A, X, Y; // registers, accumulator, x, y

 Byte C : 1; // Carry flag
 Byte Z : 1; // Zero flag
 Byte I : 1; // Interrupt Disable
 Byte D : 1; // Decimal mode
 Byte B : 1; // Break command
 Byte O : 1; // Overflow flag
 Byte N : 1; // Negative flag

 /*
 Instuction definitions
 ____________________________________________
  |             |         |       |        |
  | INSTRUCTION | OPCODE  | BYTES | CYCLES |
 _|_____________|_________|_______|________|_
 */
 enum INSTRUCTIONS {

  // LOAD INSTRUCTIONS

  // LDA

  INS_LDA_IM = 0xA9, //    2       2
  INS_LDA_ZP = 0xA5, //    2       3
  INS_LDA_ZPX = 0xB5, //    2       4
  INS_LDA_AB = 0xAD, //    3       4
  INS_LDA_ABX = 0xBD, //    3       4
  INS_LDA_ABY = 0xB9, //    3       4
  INS_LDA_INX = 0xA1, //    2       6
  INS_LDA_INY = 0xB1, //    2       6
  // LDX
  INS_LDX_IM = 0xA2, //    2       2
  INS_LDX_ZP = 0xA6, //    2       3
  INS_LDX_ZPY = 0xB6, //    2       4
  INS_LDX_AB = 0xAE, //    3       4
  INS_LDX_ABY = 0xBE, //    3       4
  // LDY
  INS_LDY_IM = 0xA0, //    2       2
  INS_LDY_ZP = 0xA4, //    2       3
  INS_LDY_ZPX = 0xB4, //    2       4
  INS_LDY_AB = 0xAC, //    3       4
  INS_LDY_ABX = 0xBC, //    3       4
  // STORE INSTRUCTIONS
  // STA
  INS_STA_ZP = 0x85, //    2       3
  INS_STA_ZPX = 0x95, //    2       4
  INS_STA_AB = 0x8D, //    3       4
  INS_STA_ABX = 0x9D, //    3       5
  INS_STA_ABY = 0x99, //    3       5
  INS_STA_INX = 0x81, //    2       6
  INS_STA_INY = 0x91, //    2       6
  // STX
  INS_STX_ZP = 0x86, //    2       3
  INS_STA_ZPY = 0x96, //    2       4
  INS_STX_AB = 0x8E, //    3       4
  // STY
  INS_STY_ZP = 0x84, //    2       3
  INS_STY_ZPX = 0x94, //    2       4
  INS_STY_AB = 0x8C, //    3       4
  // TRANSFER INSTRUCTIONS
  // X->A, A->X, TAX, XTA
  INS_TAX_IMPL = 0xAA, //    1       2
  INS_TXA_IMPL = 0x8A, //    1       2
  // Y->A, A->Y, TAY, TYA
  INS_TAY_IMPL = 0xA8, //    1       2
  INS_TYA_IMPL = 0x98, //    1       2
  // STACK OPERATIONS INSTRUCTIONS
  // SP->X, X->SP, TSX, TXS
  INS_TSX_IMPL = 0xBA, //    1       2
  INS_TXS_IMPL = 0x9A, //    1       2
  // PUSH
  INS_PHA_IMPL = 0x48, //    1       3
  INS_PHP_IMPL = 0x08, //    1       3
  // PULL
  INS_PLA_IMPL = 0x68, //    1       4
  INS_PLP_IMPL = 0x28, //    1       4
  // LOGICAL FUNCTIONS
  // AND
  INS_AND_IM = 0x29, //    2       2
  INS_AND_ZP = 0x25, //    2       3
  INS_AND_ZPX = 0x35, //    2       4
  INS_AND_AB = 0x2D, //    3       4
  INS_AND_ABX = 0x3D, //    3       4
  INS_AND_ABY = 0x39, //    3       4
  INS_AND_INX = 0x21, //    2       6
  INS_AND_INY = 0x31, //    2       6
  // EOR (Exclusive OR)
  INS_EOR_IM = 0x49, //    2       2
  INS_EOR_ZP = 0x45, //    2       3
  INS_EOR_ZPX = 0x55, //    2       4
  INS_EOR_AB = 0x4D, //    3       4
  INS_EOR_ABX = 0x5D, //    3       4
  INS_EOR_ABY = 0x59, //    3       4
  INS_EOR_INX = 0x41, //    2       6
  INS_EOR_INY = 0x51, //    2       6
  // ORA (Inclusive OR)
  INS_ORA_IM = 0x09, //    2       2
  INS_ORA_ZP = 0x05, //    2       3
  INS_ORA_ZPX = 0x15, //    2       4
  INS_ORA_AB = 0x0D, //    3       4
  INS_ORA_ABX = 0x1D, //    3       4
  INS_ORA_ABY = 0x19, //    3       4
  INS_ORA_INX = 0x01, //    2       6
  INS_ORA_INY = 0x11, //    2       6
  // BIT
  INS_BIT_ZP = 0x24, //    2       3
  INS_BIT_AB = 0x2C, //    3       4
  // ARITHMETIC INSTRUCTIONS
  // ADC
  INS_ADC_IM = 0x69, //    2       2
  INS_ADC_ZP = 0x65, //    2       3
  INS_ADC_ZPX = 0x75, //    2       4
  INS_ADC_AB = 0x6D, //    3       4
  INS_ADC_ABX = 0x7D, //    3       4
  INS_ADC_ABY = 0x79, //    3       4
  INS_ADC_INX = 0x61, //    2       6
  INS_ADC_INY = 0x71, //    2       5
  // SBC
  INS_SBC_IM = 0xE9, //    2       2
  INS_SBC_ZP = 0xE5, //    2       3
  INS_SBC_ZPX = 0xF5, //    2       4
  INS_SBC_AB = 0xED, //    3       4
  INS_SBC_ABX = 0xFD, //    3       4
  INS_SBC_ABY = 0xF9, //    3       4
  INS_SBC_INX = 0xE1, //    2       6
  INS_SBC_INY = 0xF1, //    2       6
  // CMP
  INS_CMP_IM = 0xC9, //    2       2
  INS_CMP_ZP = 0xC5, //    2       3
  INS_CMP_ZPX = 0xD5, //    2       4
  INS_CMP_AB = 0xCD, //    3       4
  INS_CMP_ABX = 0xDD, //    3       4
  INS_CMP_ABY = 0xD9, //    3       4
  INS_CMP_INX = 0xC1, //    2       6
  INS_CMP_INY = 0xD1, //    2       6
  // CPX
  INS_CPX_IM = 0xE0, //    2       2
  INS_CPX_ZP = 0xE4, //    2       3
  INS_CPX_AB = 0xEC, //    2       4
  // CPY (ахахаххаа)
  INS_CPY_IM = 0xC0, //    2       2
  INS_CPY_ZP = 0xC4, //    2       3
  INS_CPY_AB = 0xCC, //    2       4
  // INC AND DEC INSTRUCTIONS
  // INC
  INS_INC_ZP = 0xE6, //    2       5
  INS_INC_ZPX = 0xF6, //    2       6
  INS_INC_AB = 0xEE, //    3       6
  INS_INC_ABX = 0xFE, //    3       7
  // INX
  INS_INX_IMPL = 0xE8, //    1       2
  // INY
  INS_INY_IMPL = 0xC8, //    1       2
  // DEC
  INS_DEC_ZP = 0xC6, //    2       5
  INS_DEC_ZPX = 0xD6, //    2       6
  INS_DEC_AB = 0xCE, //    3       6
  INS_DEC_ABX = 0xDE, //    3       7
  // DEX
  INS_DEX_IMPL = 0xCA, //    1       2
  INS_DEY_IMPL = 0x88, //    1       2
  // SHIFT INSTRUCTIONS
  // ASL
  INS_ASL_A = 0x0A, //    1       2
  INS_ASL_ZP = 0x06, //    2       5
  INS_ASL_ZPX = 0x16, //    2       6
  INS_ASL_AB = 0x0E, //    3       6
  INS_ASL_ABX = 0x1E, //    3       7
  // LSR
  INS_LSR_A = 0x4A, //    1       2
  INS_LSR_ZP = 0x46, //    2       5
  INS_LSR_ZPX = 0x56, //    2       6
  INS_LSR_AB = 0x4E, //    3       6
  INS_LSR_ABX = 0x5E, //    3       7
  // ROL
  INS_ROL_A = 0x2A, //    1       2
  INS_ROL_ZP = 0x26, //    2       5
  INS_ROL_ZPX = 0x36, //    2       6
  INS_ROL_AB = 0x2E, //    3       6
  INS_ROL_ABX = 0x3E, //    3       7
  // ROR
  INS_ROR_A = 0x6A, //    1       2
  INS_ROR_ZP = 0x66, //    2       5
  INS_ROR_ZPX = 0x76, //    2       6
  INS_ROR_AB = 0x6E, //    3       6
  INS_ROR_ABX = 0x7E, //    3       7
  // JUMPS AND CALLS INSTRUCTIONS
  // JMP
  INS_JMP_AB = 0x4C, //    3       5
  INS_JSR_AB = 0x20, //    3       6
  INS_RTS_IMPL = 0x60, //    1       6
  // BRANCH INSTRUCTIONS
  INS_BCC_REL = 0x90, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BCS_REL = 0xB0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BEQ_REL = 0xF0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BMI_REL = 0x30, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BNE_REL = 0xD0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BPL_REL = 0x10, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BVC_REL = 0x50, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  INS_BVS_REL = 0x70, //    2       2 (+1 if branch succeeds. +2 if to a new page)
  // STATIS FLAG CHANGES INSTRUCTIONS
  // CLEAR
  INS_CLC_IMPL = 0x18, //    1       2
  INS_CLD_IMPL = 0xD8, //    1       2
  INS_CLI_IMPL = 0x58, //    1       2
  INS_CLV_IMPL = 0xB8, //    1       2
  // SET
  INS_SEC_IMPL = 0x38, //    1       2
  INS_SED_IMPL = 0xF8, //    1       2
  INS_SEI_IMPL = 0x78, //    1       2
  // SYSTEM INSTRUCTIONS
  INS_BRK_IMPL = 0x00, //    1       7
  INS_NOP_IMPL = 0xEA, //    1       2
  INS_RTI_IMPL = 0x40, //    1       6
 };

 void PrintFlags() {
  printf("CPU Flags:\n"
         "Carry     (C): 0x%02x\n"
         "Zero      (Z): 0x%02x\n"
         "Interrupt (I): 0x%02x\n"
         "Decimal   (D): 0x%02x\n"
         "Break     (B): 0x%02x\n"
         "Overflow  (O): 0x%02x\n"
         "Negative  (N): 0x%02x\n",
      C, Z, I, D, B, O, N);
 }

 void Reset(Memory& mem, Word ResetAddress = 0xFFFC /* Something about debug purposes */) {
  PC = ResetAddress;
  SP = 0x01FF;
  D = 0;
  C = Z = I = D = B = O = N = 0;
  A = X = Y = 0;
  mem.Init();
 }

 void EatCycles(uint32_t& Cycles, uint32_t amount /* amount of cycles to consume */) {
  if (Cycles >= amount)
   Cycles -= amount;
  else
   Cycles = 0;
 }

 // Does 1 cycle
 Byte FetchByte(uint32_t& Cycles, Memory& memory) {
  Byte Data = memory[PC];
  PC++;
  EatCycles(Cycles, 1);

  return Data;
 }

 // Does 1 cycle
 Byte ReadByte(uint32_t& Cycles, Byte Address, Memory& memory) {
  Byte Data = memory[Address];
  EatCycles(Cycles, 1);

  return Data;
 }

 // Does 2 cycles, useful for Absolute adressing
 Word FetchWord(uint32_t& Cycles, Memory& memory) {
  Byte lo = FetchByte(Cycles, memory);
  Byte hi = FetchByte(Cycles, memory);
  Word word = (Word)(hi << 8) | lo;

  return word;
 }

 // Does 2 cycles, useful for Indirect adressing
 Word ReadWord(uint32_t& Cycles, Byte Address, Memory& memory) {
  Byte lo = memory[Address];
  Byte hi = memory[Address + 1];
  Word word = (Word)(hi << 8) | lo;
  EatCycles(Cycles, 2);

  return word;
 }

 // Does 1 cycle, 2 on offset
 Byte GetZeroPageAddress(uint32_t Cycles, Memory& memory, Byte Offset = 0x00) {
  Byte ZeroPageAddress = FetchByte(Cycles, memory); // 1 cycle
  if (Offset) {
   ZeroPageAddress = (ZeroPageAddress + Offset) & 0xFF;
   EatCycles(Cycles, 1); // 1 cycle
  }
  return ZeroPageAddress;
 }

 // Does 2 cycles
 Word GetAbsoluteAddress(uint32_t Cycles, Memory& memory, Byte Offset = 0x00) {
  Word AbsoluteAddress = FetchWord(Cycles, memory); // 2 cycles
  AbsoluteAddress += Offset;
  return AbsoluteAddress;
 }

 // Does 2 cycles, 3 on offset
 Byte GetZeroPageAddressValue(uint32_t& Cycles, Memory& memory, Byte Offset = 0x00) {
  Byte ZeroPageAddress = GetZeroPageAddress(Cycles, memory);
  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory); // 1 cycle
  return Value;
 }

 // Does 3 cycles
 Byte GetAbsoluteAddressValue(uint32_t& Cycles, Memory& memory, Byte Offset = 0x00) {
  Word AbsoluteAddress = GetAbsoluteAddress(Cycles, memory);
  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory); // 1 cycle
  return Value;
 }

 // Does 4 cycles
 Byte GetIndirectAddressValueX(uint32_t& Cycles, Memory& memory) {
  Byte ZeroPageAddress = FetchByte(Cycles, memory); // 1 cycle
  Word IndirectAddress = ZeroPageAddress + X;

  Word EffectiveAddress = ReadWord(Cycles, IndirectAddress, memory); // 2 cycles
  Byte Value = ReadByte(Cycles, EffectiveAddress, memory); // 1 cycle
  return Value;
 }

 // Does 4 cycles
 Byte GetIndirectAddressValueY(uint32_t& Cycles, Memory& memory) {
  Byte ZeroPageAddress = FetchByte(Cycles, memory); // 1 cycle

  Word EffectiveAddress = ReadWord(Cycles, ZeroPageAddress, memory); // 2 cycles
  Byte Value = ReadByte(Cycles, EffectiveAddress + Y, memory); // 1 cycle
  return Value;
 }

 void WriteValueToAddress(uint32_t Cycles, Memory& memory, Word Address, Byte Value) {
  memory[Address] = Value;
  EatCycles(Cycles, 1);
 }

 // void LDA(uint32_t &Cycles, Byte Value) {
 //   A = Value;
 //   Z = (A == 0);
 //   N = (A & 0b10000000) != 0;
 // }

 void ADC(uint32_t& Cycles, Byte Value) {
  Word Sum = (Word)A + (Word)Value + (Word)C;

  A = (Byte)Sum;
  C = (Sum > 0xFF);
  Z = (A == 0);
  N = (A & 0b10000000) != 0;
  O = (((A ^ Value) & (A ^ Sum)) & 0x80) != 0;
  EatCycles(Cycles, 1);
 }

 void AND(uint32_t& Cycles, Byte Value) {
  A &= Value;
  Z = (A == 0);
  N = (A & 0b10000000) != 0;
  EatCycles(Cycles, 1);
 }

 void ASL(uint32_t& Cycles, Byte& Dest) {
  C = (Dest & 0b10000000) != 0;
  Dest <<= 1;
  Z = (Dest == 0);
  N = (Dest & 0b10000000) != 0;
  EatCycles(Cycles, 1);
 }

 void Execute(uint32_t Cycles, Memory& memory) {
  while (Cycles > 0) {
   printf("PC 0x%04x; ", PC);
   Byte Ins = FetchByte(Cycles, memory);
   printf("Instruction: 0x%02x ", Ins);
   switch (Ins) {
   /*
     ADC
   */

   // ADC Immediate
   case INS_ADC_IM: {
    Byte Value = FetchByte(Cycles, memory); // 1 cycle
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_IM\n");
   } break;

   // ADC Zero Page
   case INS_ADC_ZP: {
    Byte Value = GetZeroPageAddressValue(Cycles, memory); // 2 cycles, 3 if offset
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_ZP\n");
   } break;

   // ADC Zero Page X
   case INS_ADC_ZPX: {
    Byte Value = GetZeroPageAddressValue(Cycles, memory, X); // 2 cycles, 3 if offset
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_ZPX\n");
   } break;

   // ADC Absolute
   case INS_ADC_AB: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory); // 3 cycles
    EatCycles(Cycles, 1); // 1 cycle
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_AB\n");
   } break;

   // ADC Absolute + X
   case INS_ADC_ABX: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory, X); // 3 cycles
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_ABX\n");
   } break;

   // ADC Absolute + Y
   case INS_ADC_ABY: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory, Y); // 3 cycles
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_ABX\n");
   } break;

   // ADC (Indirect,X)
   case INS_ADC_INX: {
    Byte Value = GetIndirectAddressValueX(Cycles, memory); // 4 cycles
    EatCycles(Cycles, 1); // 1 cycle
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_INX\n");
   } break;

   // ADC (Indirect),Y
   case INS_ADC_INY: {
    Byte Value = GetIndirectAddressValueY(Cycles, memory); // 4 cycles
    ADC(Cycles, Value); // 1 cycle
    printf("Handled INS_ADC_INY\n");
   } break;

   /*
   AND
   */

   // AND Immediate
   case INS_AND_IM: {
    Byte Value = FetchByte(Cycles, memory); // 1 cycle
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_IM\n");
   } break;

   // AND Zero Page
   case INS_AND_ZP: {
    Byte Value = GetZeroPageAddressValue(Cycles, memory); // 2 cycles, 3 on offset
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_ZP\n");
   } break;

   // AND Zero Page X
   case INS_AND_ZPX: {
    Byte Value = GetZeroPageAddressValue(Cycles, memory, X); // 2 cycles, 3 on offset
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_ZPX\n");
   } break;

   // AND Absolute
   case INS_AND_AB: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory); // 3 cycles
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_AB\n");
   } break;

   // AND Absolute X
   case INS_AND_ABX: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory, X); // 3 cycles
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_AB\n");
   } break;

   // AND Absolute Y
   case INS_AND_ABY: {
    Byte Value = GetAbsoluteAddressValue(Cycles, memory, Y); // 3 cycles
    AND(Cycles, Value); // 1 cycle
    printf("Handled INS_AND_AB\n");
   } break;

   // AND (Indirect,X)
   case INS_AND_INX: {
    Byte Value = GetIndirectAddressValueX(Cycles, memory); // 4 cycles
    AND(Cycles, Value);
   } break;

   // AND (Indirect),Y
   case INS_AND_INY: {
    Byte Value = GetIndirectAddressValueY(Cycles, memory); // 4 cycles
    AND(Cycles, Value);
   } break;

   // ASL

   // ASL Accumulator
   case INS_ASL_A: {
    ASL(Cycles, A); // 1 cycle
    EatCycles(Cycles, 1);
   }

   // ASL Zero Page
   case INS_ASL_ZP: {
    Byte Address = GetZeroPageAddress(Cycles, memory); // 2 cycles
    Byte Value = ReadByte(Cycles, Address, memory); // 1 cycle
    ASL(Cycles, Value); // 1 cycle
    WriteValueToAddress(Cycles, memory, Address, Value); // 1 cycle
    break;
   }
   // ASL Zero Page X
   case INS_ASL_ZPX: {
    Byte Address = GetZeroPageAddress(Cycles, memory, X); // 3 cycles
    Byte Value = ReadByte(Cycles, Address, memory); // 1 cycle
    ASL(Cycles, Value); // 1 cycle
    WriteValueToAddress(Cycles, memory, Address, Value); // 1 cycle
    break;
   }

   // ASL Absolute
   case INS_ASL_AB: {
    Word Address = GetAbsoluteAddress(Cycles, memory); // 3 cycles
    Byte Value = ReadByte(Cycles, Address, memory); // 1 cycle
    ASL(Cycles, Value); // 1 cycle
    WriteValueToAddress(Cycles, memory, Address, Value); // 1 cycle
   }

   // ASL Absolute X
   case INS_ASL_ABX: {
    Word Address = GetAbsoluteAddress(Cycles, memory, X); // 3 cycles
    Byte Value = ReadByte(Cycles, Address, memory); // 1 cycle
    ASL(Cycles, Value); // 1 cycle
    WriteValueToAddress(Cycles, memory, Address, Value); // 1 cycle
    EatCycles(Cycles, 1); // 1 cycle
   }

   // JMP Absolute
   case INS_JMP_AB: {
    PC = FetchWord(Cycles, memory); // 2 cycles
    EatCycles(Cycles, 1); // 1 cycle
    printf("Handled INS_JMP_AB\n");
   } break;

   case INS_STA_AB: {
    Word Address = FetchWord(Cycles, memory);

    memory[Address] = A;

    EatCycles(Cycles, 2);

    printf("Handled INS_STX_AB\n");
   } break;

   default: {
    printf("Isn't handled.\n");
    return;
   } break;
   }
  }
 }
};

int main() {
 Memory mem;
 CPU_6502 cpu;
 cpu.Reset(mem, 0x0);
 mem[0x0] = CPU_6502::INS_ADC_IM;
 mem[0x0001] = 0x16;
 mem[0x0002] = CPU_6502::INS_ASL_ZP;
 mem[0x0003] = 0x04;
 mem[0x0004] = 0x04;

 cpu.Execute(5, mem);
 mem.PrintRange(0x0, 0x050);
 cpu.PrintFlags();
 return 0;
}