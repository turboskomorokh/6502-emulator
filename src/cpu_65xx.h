#ifndef _CPU_65XX_H_
#define _CPU_65XX_H_

#include <cstdint>

#include "common.h"

#include "memory.h"

struct CPU_65XX_SREG {
 Byte C : 1; // Carry
 Byte Z : 1; // Zero
 Byte I : 1; // Interrupt Disable
 Byte D : 1; // Decimal mode
 Byte B : 1; // Break command
 Byte O : 1; // Overflow
 Byte N : 1; // Negative

 Byte GetSREG() const;
 void SetSREG(Byte SREG);

 operator Byte() const { return GetSREG(); }
 CPU_65XX_SREG& operator=(Byte SREG) {
  SetSREG(SREG);
  return *this;
 }
};

struct CPU_65XX {
 Word PC; // Program Counter
 Byte SP; // Stack Pointer

 CPU_65XX_SREG SREG; // Status Register

 Byte A, X, Y; // Registers: Accumulator, X, Y

 uint32_t Cycles = 0; // Cycles

 // Verbose functions
 void ShowRegisters();
 void ShowFlags();

 // Main functions
 void Reset(Memory& mem);
 void Execute(Memory& memory);

 void EatCycles(uint32_t amount);

 // Stack operations

 void StackPushByte(Memory& Memory, Byte M);
 void StackPushWord(Memory& Memory, Word M);
 Byte StackPullByte(Memory& Memory);
 Word StackPullWord(Memory& Memory);

 // Memory operations

 Byte FetchByte(Memory& Memory);
 Word FetchWord(Memory& Memory);

 Byte ReadByte(Word Address, Memory& Memory);
 Word ReadWord(Word Address, Memory& Memory);

 void WriteByte(Memory& Memory, Word Address, Byte Value);
 void WriteWord(Memory& Memory, Word Address, Word Value);

 Byte FetchZPAddress(Memory& Memory, Byte offset = 0x00);
 Word FetchABAddress(Memory& Memory, Byte Offset = 0x00);

 Word FetchINAddressX(Memory& Memory);
 Word FetchINAddressY(Memory& Memory);

 static constexpr Byte
     // LOAD INSTRUCTIONS
     INS_LDA_IM
     = 0xA9, //    2       2
     INS_LDA_ZP   = 0xA5, //    2       3
     INS_LDA_ZPX  = 0xB5, //    2       4
     INS_LDA_AB   = 0xAD, //    3       4
     INS_LDA_ABX  = 0xBD, //    3       4
     INS_LDA_ABY  = 0xB9, //    3       4
     INS_LDA_INX  = 0xA1, //    2       6
     INS_LDA_INY  = 0xB1, //    2       6

     INS_LDX_IM   = 0xA2, //    2       2
     INS_LDX_ZP   = 0xA6, //    2       3
     INS_LDX_ZPY  = 0xB6, //    2       4
     INS_LDX_AB   = 0xAE, //    3       4
     INS_LDX_ABY  = 0xBE, //    3       4

     INS_LDY_IM   = 0xA0, //    2       2
     INS_LDY_ZP   = 0xA4, //    2       3
     INS_LDY_ZPX  = 0xB4, //    2       4
     INS_LDY_AB   = 0xAC, //    3       4
     INS_LDY_ABX  = 0xBC, //    3       4
     // STORE INSTRUCTIONS
     INS_STA_ZP   = 0x85, //    2       3
     INS_STA_ZPX  = 0x95, //    2       4
     INS_STA_AB   = 0x8D, //    3       4
     INS_STA_ABX  = 0x9D, //    3       5
     INS_STA_ABY  = 0x99, //    3       5
     INS_STA_INX  = 0x81, //    2       6
     INS_STA_INY  = 0x91, //    2       6

     INS_STX_ZP   = 0x86, //    2       3
     INS_STX_ZPY  = 0x96, //    2       4
     INS_STX_AB   = 0x8E, //    3       4

     INS_STY_ZP   = 0x84, //    2       3
     INS_STY_ZPX  = 0x94, //    2       4
     INS_STY_AB   = 0x8C, //    3       4
     // TRANSFER INSTRUCTIONS
     INS_TAX_IMPL = 0xAA, //    1       2
     INS_TXA_IMPL = 0x8A, //    1       2

     INS_TAY_IMPL = 0xA8, //    1       2
     INS_TYA_IMPL = 0x98, //    1       2
     // STACK OPERATIONS INSTRUCTIONS
     INS_TSX_IMPL = 0xBA, //    1       2
     INS_TXS_IMPL = 0x9A, //    1       2

     INS_PHA_IMPL = 0x48, //    1       3
     INS_PHP_IMPL = 0x08, //    1       3

     INS_PLA_IMPL = 0x68, //    1       4
     INS_PLP_IMPL = 0x28, //    1       4
     // LOGICAL FUNCTIONS
     INS_AND_IM   = 0x29, //    2       2
     INS_AND_ZP   = 0x25, //    2       3
     INS_AND_ZPX  = 0x35, //    2       4
     INS_AND_AB   = 0x2D, //    3       4
     INS_AND_ABX  = 0x3D, //    3       4
     INS_AND_ABY  = 0x39, //    3       4
     INS_AND_INX  = 0x21, //    2       6
     INS_AND_INY  = 0x31, //    2       6

     INS_EOR_IM   = 0x49, //    2       2
     INS_EOR_ZP   = 0x45, //    2       3
     INS_EOR_ZPX  = 0x55, //    2       4
     INS_EOR_AB   = 0x4D, //    3       4
     INS_EOR_ABX  = 0x5D, //    3       4
     INS_EOR_ABY  = 0x59, //    3       4
     INS_EOR_INX  = 0x41, //    2       6
     INS_EOR_INY  = 0x51, //    2       6

     INS_ORA_IM   = 0x09, //    2       2
     INS_ORA_ZP   = 0x05, //    2       3
     INS_ORA_ZPX  = 0x15, //    2       4
     INS_ORA_AB   = 0x0D, //    3       4
     INS_ORA_ABX  = 0x1D, //    3       4
     INS_ORA_ABY  = 0x19, //    3       4
     INS_ORA_INX  = 0x01, //    2       6
     INS_ORA_INY  = 0x11, //    2       6

     INS_BIT_ZP   = 0x24, //    2       3
     INS_BIT_AB   = 0x2C, //    3       4
     // ARITHMETIC INSTRUCTIONS
     INS_ADC_IM   = 0x69, //    2       2
     INS_ADC_ZP   = 0x65, //    2       3
     INS_ADC_ZPX  = 0x75, //    2       4
     INS_ADC_AB   = 0x6D, //    3       4
     INS_ADC_ABX  = 0x7D, //    3       4
     INS_ADC_ABY  = 0x79, //    3       4
     INS_ADC_INX  = 0x61, //    2       6
     INS_ADC_INY  = 0x71, //    2       5

     INS_SBC_IM   = 0xE9, //    2       2
     INS_SBC_ZP   = 0xE5, //    2       3
     INS_SBC_ZPX  = 0xF5, //    2       4
     INS_SBC_AB   = 0xED, //    3       4
     INS_SBC_ABX  = 0xFD, //    3       4
     INS_SBC_ABY  = 0xF9, //    3       4
     INS_SBC_INX  = 0xE1, //    2       6
     INS_SBC_INY  = 0xF1, //    2       6

     INS_CMP_IM   = 0xC9, //    2       2
     INS_CMP_ZP   = 0xC5, //    2       3
     INS_CMP_ZPX  = 0xD5, //    2       4
     INS_CMP_AB   = 0xCD, //    3       4
     INS_CMP_ABX  = 0xDD, //    3       4
     INS_CMP_ABY  = 0xD9, //    3       4
     INS_CMP_INX  = 0xC1, //    2       6
     INS_CMP_INY  = 0xD1, //    2       6

     INS_CPX_IM   = 0xE0, //    2       2
     INS_CPX_ZP   = 0xE4, //    2       3
     INS_CPX_AB   = 0xEC, //    2       4

     INS_CPY_IM   = 0xC0, //    2       2
     INS_CPY_ZP   = 0xC4, //    2       3
     INS_CPY_AB   = 0xCC, //    2       4
     // INC AND DEC INSTRUCTIONS
     INS_INC_ZP   = 0xE6, //    2       5
     INS_INC_ZPX  = 0xF6, //    2       6
     INS_INC_AB   = 0xEE, //    3       6
     INS_INC_ABX  = 0xFE, //    3       7

     INS_INX_IMPL = 0xE8, //    1       2
     INS_INY_IMPL = 0xC8, //    1       2

     INS_DEC_ZP   = 0xC6, //    2       5
     INS_DEC_ZPX  = 0xD6, //    2       6
     INS_DEC_AB   = 0xCE, //    3       6
     INS_DEC_ABX  = 0xDE, //    3       7

     INS_DEX_IMPL = 0xCA, //    1       2
     INS_DEY_IMPL = 0x88, //    1       2
     // SHIFT INSTRUCTIONS
     INS_ASL_A    = 0x0A, //    1       2
     INS_ASL_ZP   = 0x06, //    2       5
     INS_ASL_ZPX  = 0x16, //    2       6
     INS_ASL_AB   = 0x0E, //    3       6
     INS_ASL_ABX  = 0x1E, //    3       7

     INS_LSR_A    = 0x4A, //    1       2
     INS_LSR_ZP   = 0x46, //    2       5
     INS_LSR_ZPX  = 0x56, //    2       6
     INS_LSR_AB   = 0x4E, //    3       6
     INS_LSR_ABX  = 0x5E, //    3       7

     INS_ROL_A    = 0x2A, //    1       2
     INS_ROL_ZP   = 0x26, //    2       5
     INS_ROL_ZPX  = 0x36, //    2       6
     INS_ROL_AB   = 0x2E, //    3       6
     INS_ROL_ABX  = 0x3E, //    3       7

     INS_ROR_A    = 0x6A, //    1       2
     INS_ROR_ZP   = 0x66, //    2       5
     INS_ROR_ZPX  = 0x76, //    2       6
     INS_ROR_AB   = 0x6E, //    3       6
     INS_ROR_ABX  = 0x7E, //    3       7
     // JUMPS AND CALL INSTRUCTIONS
     INS_JMP_AB   = 0x4C, //    3       5
     // On the holy earth why
     INS_JMP_IN   = 0x6C, //    3       5
     INS_JSR_AB   = 0x20, //    3       6
     INS_RTS_IMPL = 0x60, //    1       6
     // BRANCH INSTRUCTIONS
     INS_BCC_REL  = 0x90, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BCS_REL  = 0xB0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BEQ_REL  = 0xF0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BMI_REL  = 0x30, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BNE_REL  = 0xD0, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BPL_REL  = 0x10, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BVC_REL  = 0x50, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     INS_BVS_REL  = 0x70, //    2       2 (+1 if branch succeeds. +2 if to a new page)
     // STATUS FLAG CHANGES INSTRUCTIONS
     INS_CLC_IMPL = 0x18, //    1       2
     INS_CLD_IMPL = 0xD8, //    1       2
     INS_CLI_IMPL = 0x58, //    1       2
     INS_CLV_IMPL = 0xB8, //    1       2

     INS_SEC_IMPL = 0x38, //    1       2
     INS_SED_IMPL = 0xF8, //    1       2
     INS_SEI_IMPL = 0x78, //    1       2
     // SYSTEM INSTRUCTIONS
     INS_BRK_IMPL = 0x00, //    1       7
     INS_NOP_IMPL = 0xEA, //    1       2
     INS_RTI_IMPL = 0x40; //    1       6

 // Internal cycles: 0
 void ADC(Byte M);
 // Internal cycles: 0
 void AND(Byte M);
 // Internal cycles: 1
 void ASL(Byte& Dest);
 // Internal cycles: 3
 void BCC(Byte Offset);
 // Internal cycles: 3
 void BCS(Byte Offset);
 // Internal cycles: 3
 void BEQ(Byte Offset);
 // Internal cycles: 0
 void BIT(Byte M);
 // Internal cycles: 3
 void BMI(Byte Offset);
 // Internal cycles: 3
 void BNE(Byte Offset);
 // Internal cycles: 3
 void BPL(Byte Offset);
 // Internal cycles: 5
 void BRK(Memory& memory);
 // Internal cycles: 3
 void BVC(Byte Offset);
 // Internal cycles: 3
 void BVS(Byte Offset);
 // Internal cycles: 1
 void CLC();
 // Internal cycles: 1
 void CLD();
 // Internal cycles: 1
 void CLI();
 // Internal cycles: 1
 void CLV();
 // Internal cycles: 0
 void CMP(Byte M);
 // Internal cycles: 0
 void CPX(Byte M);
 // Internal cycles: 0
 void CPY(Byte M);
 // Internal cycles: 0
 void DEC(Byte& Dest);
 // Internal cycles: 1
 void DEX();
 // Internal cycles: 1
 void DEY();
 // Internal cycles: 0
 void EOR(Byte M);
 // Internal cycles: 0
 void INC(Byte& Dest);
 // Internal cycles: 1
 void INX();
 // Internal cycles: 1
 void INY();
 // Internal cycles: 0
 void JMP(Word Address);
 // Internal cycles: 2
 void JSR(Memory& memory, Word Address);
 // Internal cycles: 0
 void LDA(Byte M);
 // Internal cycles: 0
 void LDX(Byte M);
 // Internal cycles: 0
 void LDY(Byte M);
 // Internal cycles: 1
 void LSR(Byte& Dest);
 // Internal cycles: 1
 void NOP();
 // Internal cycles: 0
 void ORA(Byte M);
 // Internal cycles: 2
 void PHA(Memory& memory);
 // Internal cycles: 2
 void PHP(Memory& memory);
 // Internal cycles: 3
 void PLA(Memory& memory);
 // Internal cycles: 3
 void PLP(Memory& memory);
 // Internal cycles: 1
 void ROL(Byte& Dest);
 // Internal cycles: 1
 void ROR(Byte& Dest);
 // Internal cycles: 5
 void RTI(Memory& memory);
 // Internal cycles: 5
 void RTS(Memory& memory);
 // Internal cycles: 0
 void SBC(Byte M);
 void SEC();
 void SED();
 void SEI();
 void STA(Memory& memory, Word Address);
 void STX(Memory& memory, Word Address);
 void STY(Memory& memory, Word Address);
 void TAX();
 void TAY();
 void TSX();
 void TXA();
 void TXS();
 void TYA();
};

#endif