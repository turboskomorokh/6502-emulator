#include <stdint.h>

#include "common.h"
#include "cpu_6502.h"
#include "memory.h"

void CPU_6502::Execute(Memory& memory) {
 Word Last_PC = PC;
 PC           = ReadWord(PC, memory);
 while (Cycles > 0) {
  printf("PC 0x%04x; ", PC);
  Byte Ins = FetchByte(memory);
  printf("Instruction: 0x%02x. ", Ins);

  if (PC == Last_PC) {
   printf("Program counter stuck at 0x%04x\n", Last_PC);
   break;
  }

  Last_PC = PC;

  switch (Ins) {

  // Cycles: 1
  case INS_ADC_IM: {
   Byte Value = FetchByte(memory);
   ADC(Value);
   printf("Handled INS_ADC_IM\n");
  } break;

  // Cycles: 2
  case INS_ADC_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_ZP\n");
  } break;

  // Cycles: 3
  case INS_ADC_ZPX: {
   Word Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_ZPX\n");
  } break;

  // Cycles: 3
  case INS_ADC_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_AB\n");
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_ADC_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_ABX\n");
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_ADC_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_ABY\n");
  } break;

  // Cycles: 5
  case INS_ADC_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_INX\n");
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_ADC_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   ADC(Value);
   printf("Handled INS_ADC_INY\n");
  } break;

  // Cycles: 1
  case INS_AND_IM: {
   Byte Value = FetchByte(memory);
   AND(Value);
   printf("Handled INS_AND_IM\n");
  } break;

  // Cycles: 2
  case INS_AND_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_ZP\n");
  } break;

  // Cycles: 3
  case INS_AND_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_ZPX\n");
  } break;

  // Cycles: 3
  case INS_AND_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_AB\n");
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_AND_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_ABX\n");
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_AND_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_ABY\n");
  } break;

  // Cycles: 5
  case INS_AND_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_INX\n");
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_AND_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   AND(Value);
   printf("Handled INS_AND_INY\n");
  } break;

  // Cycles: 1
  case INS_ASL_A: {
   ASL(A);
   printf("Handled INS_ASL_A\n");
  } break;

  // Cycles: 4
  case INS_ASL_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ASL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ASL_ZP\n");
  } break;

  // Cycles: 5 (+1 if crossed page)
  case INS_ASL_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ASL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ASL_ZP\n");
  } break;

  // Cycles: 5
  case INS_ASL_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ASL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ASL_AB\n");
  } break;

  // Cycles: 5 (+1 if crossed page)
  case INS_ASL_ABX: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ASL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ASL_ABX\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BCC_REL: {
   Byte Offset = FetchByte(memory);
   BCC(Offset);
   printf("Handled INS_BCC_REL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BCS_REL: {
   Byte Offset = FetchByte(memory);
   BCS(Offset);
   printf("Handled INS_BCS_REL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BEQ_REL: {
   Byte Offset = FetchByte(memory);
   BEQ(Offset);
   printf("Handled INS_BEQ_REL\n");
  } break;

  // Cycles: 2
  case INS_BIT_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   BIT(Value);
   printf("Handled INS_BIT_ZP\n");
  } break;

  // Cycles: 3
  case INS_BIT_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   printf("Handled INS_BIT_AB\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BMI_REL: {
   Byte Offset = FetchByte(memory);
   BMI(Offset);
   printf("Handled INS_BMI_REL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BNE_REL: {
   Byte Offset = FetchByte(memory);
   BNE(Offset);
   printf("Handled INS_BNE_REL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BPL_REL: {
   Byte Offset = FetchByte(memory);
   BPL(Offset);
   printf("Handled INS_BPL_REL\n");
  } break;

  // Cycles: 6
  case INS_BRK_IMPL: {
   BRK(memory);
   printf("Handled INS_BRK_IMPL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BVC_REL: {
   Byte Offset = FetchByte(memory);
   BVC(Offset);
   printf("Handled INS_BVC_REL\n");
  } break;

  // Cycles: 1 (+1 if succeed, + 2 if crossed page)
  case INS_BVS_REL: {
   Byte Offset = FetchByte(memory);
   BVS(Offset);
   printf("Handled INS_BVS_REL\n");
  } break;

   // Cycles: 1
  case INS_CLC_IMPL: {
   CLC();
   printf("Handled INS_CLC_IMPL\n");
  } break;

   // Cycles: 1
  case INS_CLD_IMPL: {
   CLD();
   printf("Handled INS_CLD_IMPL\n");
  } break;

   // Cycles: 1
  case INS_CLI_IMPL: {
   CLI();
   printf("Handled INS_CLI_IMPL\n");
  } break;

  // Cycles: 1
  case INS_CLV_IMPL: {
   CLV();
   printf("Handled INS_CLV_IMPL\n");
  } break;

  // Cycles: 1
  case INS_CMP_IM: {
   Byte Value = FetchByte(memory);
   CMP(Value);
   printf("Handled INS_CMP_IM\n");
  } break;

  // Cycles: 2
  case INS_CMP_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_ZP\n");
  } break;

  // Cycles: 3
  case INS_CMP_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_ZPX\n");
  } break;

  // Cycles: 3
  case INS_CMP_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_AB\n");
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_CMP_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_ABX\n");
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_CMP_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_ABY\n");
  } break;

  // Cycles: 5
  case INS_CMP_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_INX\n");
  } break;

  // Cycles: 4 (+1 on crossing page)
  case INS_CMP_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   CMP(Value);
   printf("Handled INS_CMP_INY\n");
  } break;

  // Cycles: 1
  case INS_CPX_IM: {
   Byte Value = FetchByte(memory);
   CPX(Value);
   printf("Handled INS_CPX_IM\n");
  } break;

  // Cycles: 2
  case INS_CPX_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CPX(Value);
   printf("Handled INS_CPX_ZP\n");
  } break;

   // Cycles: 3
  case INS_CPX_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CPX(Value);
   printf("Handled INS_CPX_AB\n");
  } break;

   // Cycles: 1
  case INS_CPY_IM: {
   Byte Value = FetchByte(memory);
   CPY(Value);
   printf("Handled INS_CPY_IM\n");
  } break;

   // Cycles: 2
  case INS_CPY_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CPY(Value);
   printf("Handled INS_CPY_ZP\n");
  } break;

   // Cycles: 3
  case INS_CPY_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   CPY(Value);
   printf("Handled INS_CPY_AB\n");
  } break;

   // Cycles: 4
  case INS_DEC_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   DEC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_DEC_ZP\n");
  } break;

  // Cycles: 5
  case INS_DEC_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   DEC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_DEC_ZPX\n");
  } break;

  // Cycles: 5
  case INS_DEC_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   DEC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_DEC_AB\n");
  } break;

  // Cycles: 6
  case INS_DEC_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   DEC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_DEC_ABX\n");
  } break;

  // Cycles: 1
  case INS_DEX_IMPL: {
   DEX();
   printf("Handled INS_DEX_IMPL\n");
  } break;

  // Cycles: 1
  case INS_DEY_IMPL: {
   DEX();
   printf("Handled INS_DEY_IMPL\n");
  } break;

  // Cycles: 1
  case INS_EOR_IM: {
   Byte Value = FetchByte(memory);
   EOR(Value);
   printf("Handled INS_EOR_IM\n");
  } break;

  // Cycles: 2
  case INS_EOR_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_ZP\n");
  } break;

  // Cycles: 3
  case INS_EOR_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_ZPX\n");
  } break;

  // Cycles: 3
  case INS_EOR_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_AB\n");
  } break;

  // Cycles: 3
  case INS_EOR_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_ABX\n");
  } break;

  // Cycles: 3
  case INS_EOR_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_ABY\n");
  } break;

  // Cycles: 5
  case INS_EOR_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_INX\n");
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_EOR_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   EOR(Value);
   printf("Handled INS_EOR_INY\n");
  } break;

  // Cycles: 4
  case INS_INC_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   INC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_INC_ZP\n");
  } break;

  // Cycles: 5
  case INS_INC_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   INC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_INC_ZPX\n");
  } break;

  // Cycles: 5
  case INS_INC_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   INC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_INC_AB\n");
  } break;

  // Cycles: 5
  case INS_INC_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   INC(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_INC_ABX\n");
  } break;

  // Cycles: 1
  case INS_INX_IMPL: {
   INX();
   printf("Handled INS_INX_IMPL\n");
  } break;

  // Cycles: 1
  case INS_INY_IMPL: {
   INY();
   printf("Handled INS_INY_IMPL\n");
  } break;

  // Cycles: 2
  case INS_JMP_AB: {
   Word Address = FetchABAddress(memory);
   JMP(Address);
   printf("Handled INS_JMP_AB\n");
  } break;

  // Cycles: 4
  case INS_JMP_IN: {
   Word Address          = FetchABAddress(memory);
   Word EffectiveAddress = ReadWord(Address, memory);
   JMP(Address);
   printf("Handled INS_JMP_IN\n");
  } break;

  // Cycles: 5
  case INS_JSR_AB: {
   Word Address = FetchABAddress(memory);
   JSR(memory, Address);
   printf("Handled INS_JSR_AB\n");
  } break;

  // Cycles: 1
  case INS_LDA_IM: {
   Byte Value = FetchByte(memory);
   LDA(Value);
   printf("Handled INS_LDA_IM\n");
  } break;

   // Cycles: 2
  case INS_LDA_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_ZP\n");
  } break;

   // Cycles: 3
  case INS_LDA_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_ZPX\n");
  } break;

   // Cycles: 3
  case INS_LDA_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_AB\n");
  } break;

   // Cycles: 3
  case INS_LDA_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_ABX\n");
  } break;

   // Cycles: 3
  case INS_LDA_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_ABY\n");
  } break;

   // Cycles: 5
  case INS_LDA_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   LDA(Value);
   printf("Handled INS_LDA_INX\n");
  } break;

   // Cycles: 4 (+1 if crossed page)
  case INS_LDA_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   LDX(Value);
   printf("Handled INS_LDA_INY\n");
  } break;

   // Cycles: 1
  case INS_LDX_IM: {
   Byte Value = FetchByte(memory);
   LDX(Value);
   printf("Handled INS_LDX_IM\n");
  } break;

   // Cycles: 2
  case INS_LDX_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDX(Value);
   printf("Handled INS_LDX_ZP\n");
  } break;

   // Cycles: 3
  case INS_LDX_ZPY: {
   Byte Address = FetchZPAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   LDX(Value);
   printf("Handled INS_LDX_ZPY\n");
  } break;

   // Cycles: 3
  case INS_LDX_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDX(Value);
   printf("Handled INS_LDX_AB\n");
  } break;

   // Cycles: 3
  case INS_LDX_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   LDX(Value);
   printf("Handled INS_LDX_ABY\n");
  } break;

  // Cycles: 1
  case INS_LDY_IM: {
   Byte Value = FetchByte(memory);
   LDY(Value);
   printf("Handled INS_LDY_IM\n");
  } break;

   // Cycles: 2
  case INS_LDY_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDY(Value);
   printf("Handled INS_LDY_ZP\n");
  } break;

   // Cycles: 3
  case INS_LDY_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LDY(Value);
   printf("Handled INS_LDY_ZPX\n");
  } break;

   // Cycles: 3
  case INS_LDY_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LDY(Value);
   printf("Handled INS_LDY_AB\n");
  } break;

   // Cycles: 3
  case INS_LDY_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LDY(Value);
   printf("Handled INS_LDY_ABX\n");
  } break;

  // Cycles: 1
  case INS_LSR_A: {
   LSR(A);
   printf("Handled INS_LSR_A\n");
  } break;

  // Cycles: 4
  case INS_LSR_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LSR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_LSR_ZP\n");
  } break;

  // Cycles: 5
  case INS_LSR_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LSR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_LSR_ZPX\n");
  } break;

  // Cycles: 5
  case INS_LSR_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   LSR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_LSR_AB\n");
  } break;

  // Cycles: 6
  case INS_LSR_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   LSR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_LSR_ABX\n");
  } break;

  case INS_NOP_IMPL: {
   NOP();
   printf("Handled INS_NOP_IMPL\n");
  } break;

  // Cycles: 1
  case INS_ORA_IM: {
   Byte Value = FetchByte(memory);
   ORA(Value);
   printf("Handled INS_ORA_IM\n");
  } break;

  // Cycles: 2
  case INS_ORA_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_ZP\n");
  } break;

  // Cycles: 3
  case INS_ORA_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_ZPX\n");
  } break;

  // Cycles: 3
  case INS_ORA_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_AB\n");
  } break;

  // Cycles: 3
  case INS_ORA_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_ABX\n");
  } break;

  // Cycles: 3
  case INS_ORA_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_ABY\n");
  } break;

  // Cycles: 5
  case INS_ORA_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_INX\n");
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_ORA_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   ORA(Value);
   printf("Handled INS_ORA_INY\n");
  } break;

  // Cycles: 1
  case INS_PHA_IMPL: {
   PHA(memory);
   printf("Handled INS_PHA_IMPL\n");
  } break;

  // Cycles: 1
  case INS_PHP_IMPL: {
   PHP(memory);
   printf("Handled INS_PHP_IMPL\n");
  } break;

  // Cycles: 1
  case INS_PLA_IMPL: {
   PLA(memory);
   printf("Handled INS_PLA_IMPL\n");
  } break;

  // Cycles: 1
  case INS_PLP_IMPL: {
   PLP(memory);
   printf("Handled INS_PLP_IMPL\n");
  } break;

  // Cycles: 1
  case INS_ROL_A: {
   ROL(A);
   printf("Handled INS_ROL_A\n");
  } break;

  // Cycles: 4
  case INS_ROL_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ROL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROL_ZP\n");
  } break;
  // Cycles: 5
  case INS_ROL_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ROL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROL_ZPX\n");
  } break;
  // Cycles: 5
  case INS_ROL_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ROL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROL_AB\n");
  } break;

  // Cycles: 6
  case INS_ROL_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ROL(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROL_ABX\n");
  } break;

  // Cycles: 1
  case INS_ROR_A: {
   ROR(A);
   printf("Handled INS_ROR_A\n");
  } break;

  // Cycles: 4
  case INS_ROR_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ROR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROR_ZP\n");
  } break;
  // Cycles: 5
  case INS_ROR_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ROR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROR_ZPX\n");
  } break;
  // Cycles: 5
  case INS_ROR_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   ROR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROR_AB\n");
  } break;

  // Cycles: 6
  case INS_ROR_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   ROR(Value);
   WriteByte(memory, Address, Value);
   printf("Handled INS_ROR_ABX\n");
  } break;

  // Cycles: 5
  case INS_RTI_IMPL: {
   RTI(memory);
   printf("Handled INS_RTI_IMPL\n");
  } break;

  // Cycles: 5
  case INS_RTS_IMPL: {
   RTS(memory);
   printf("Handled INS_RTS_IMPL\n");
  } break;

  // Cycles: 1
  case INS_SBC_IM: {
   Byte Value = FetchByte(memory);
   SBC(Value);
   printf("Handled INS_SBC_IM\n");
  } break;

  // Cycles: 2
  case INS_SBC_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_ZP\n");
  } break;

  // Cycles: 3
  case INS_SBC_ZPX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_ZPX\n");
  } break;

  // Cycles: 3
  case INS_SBC_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_AB\n");
  } break;

  // Cycles: 3 (+1 on crossing page)
  case INS_SBC_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_ABX\n");
  } break;

  // Cycles: 3 (+1 if crossing page)
  case INS_SBC_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_ABY\n");
  } break;

  // Cycles: 5
  case INS_SBC_INX: {
   Word Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_INX\n");
  } break;

  // Cycles: 4 (+1 if crossing page)
  case INS_SBC_INY: {
   Word Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);
   SBC(Value);
   printf("Handled INS_SBC_INY\n");
  } break;

  // Cycles: 1
  case INS_SED_IMPL: {
   SED();
   printf("Handled INS_SED_IMPL\n");
  } break;

  // Cycles: 1
  case INS_SEC_IMPL: {
   SEC();
   printf("Handled INS_SEC_IMPL\n");
  } break;

  // Cycles: 1
  case INS_SEI_IMPL: {
   SEI();
   printf("Handled INS_SEI_IMPL\n");
  } break;

  // Cycles: 2
  case INS_STA_ZP: {
   Byte Address = FetchZPAddress(memory);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_ZP\n");
  } break;

  // Cycles: 3
  case INS_STA_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_ZPX\n");
  } break;

  // Cycles: 3
  case INS_STA_AB: {
   Word Address = FetchABAddress(memory);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_AB\n");
  } break;

  // Cycles: 3 (+1 if crossed page)
  case INS_STA_ABX: {
   Word Address = FetchABAddress(memory, X);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_ABX\n");
  } break;

  // Cycles: 3 (+1 if crossed page)
  case INS_STA_ABY: {
   Word Address = FetchABAddress(memory, Y);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_ABY\n");
  } break;

  // Cycles: 5
  case INS_STA_INX: {
   Byte Address = FetchINAddressX(memory);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_INX\n");
  } break;

  // Cycles: 4 (+1 if crossed page)
  case INS_STA_INY: {
   Byte Address = FetchINAddressY(memory);
   Byte Value   = ReadByte(Address, memory);

   STA(memory, Address);
   printf("Handled INS_STA_INY\n");
  } break;

  // Cycles: 2
  case INS_STX_ZP: {
   Byte Address = FetchZPAddress(memory);
   STX(memory, Address);
   printf("Handled INS_STX_ZP\n");
  } break;

  // Cycles: 3
  case INS_STX_ZPY: {
   Byte Address = FetchZPAddress(memory, Y);
   STX(memory, Address);
   printf("Handled INS_STX_ZPY\n");
  } break;

  // Cycles: 3
  case INS_STX_AB: {
   Word Address = FetchABAddress(memory);
   STX(memory, Address);
   printf("Handled INS_STX_AB\n");
  } break;

  // Cycles: 2
  case INS_STY_ZP: {
   Byte Address = FetchZPAddress(memory);
   STY(memory, Address);
   printf("Handled INS_STY_ZP\n");
  } break;

  // Cycles: 3
  case INS_STY_ZPX: {
   Byte Address = FetchZPAddress(memory, X);
   STY(memory, Address);
   printf("Handled INS_STY_ZPX\n");
  } break;

  // Cycles: 3
  case INS_STY_AB: {
   Word Address = FetchABAddress(memory);
   STY(memory, Address);
   printf("Handled INS_STY_AB\n");
  } break;

  case INS_TYA_IMPL: {
   TYA();
   printf("Handled INS_TYA_IMPL\n");
  } break;

  case INS_TAY_IMPL: {
   TAY();
   printf("Handled INS_TAY_IMPL\n");
  } break;

  case INS_TXA_IMPL: {
   TXA();
   printf("Handled INS_TXA_IMPL\n");
  } break;

  case INS_TAX_IMPL: {
   TAX();
   printf("Handled INS_TAX_IMPL\n");
  } break;

  case INS_TSX_IMPL: {
   TSX();
   printf("Handled INS_TSX_IMPL\n");
  } break;

  case INS_TXS_IMPL: {
   TXS();
   printf("Handled INS_TXS_IMPL\n");
  } break;

  default: {
   printf("Isn't handled, falling to BRK\n");
   BRK(memory);
  } break;
  }
 }
}
