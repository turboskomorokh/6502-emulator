#ifndef _CPU_65xx_H_
#define _CPU_65xx_H_

#include <cstdint>

#include "common.h"
#include "ins_65xx.h"
#include "memory.h"

// CPU PROGRAM COUNTER STUFF

struct CPU_65XX_PS {
 Byte C : 1;  // Carry
 Byte Z : 1;  // Zero
 Byte I : 1;  // Interrupt
 Byte D : 1;  // Decimal
 Byte B : 1;  // Break
 Byte U : 1;  // Unused
 Byte V : 1;  // oVerflow
 Byte N : 1;  // Negative

 CPU_65XX_PS() : C(0), Z(0), I(0), D(0), B(0), U(0), V(0), N(0) {}

 static const Byte CarryBit = 0x01, ZeroBit = 0x02, InterruptDisableBit = 0x04, DecimalBit = 0x08, BreakBit = 0x10, UnusedBit = 0x20, OverflowBit = 0x40, NegativeBit = 0x80;

 void SetPS(Byte NewPS) {
  C = (NewPS & CarryBit) ? 1 : 0;
  Z = (NewPS & ZeroBit) ? 1 : 0;
  I = (NewPS & InterruptDisableBit) ? 1 : 0;
  D = (NewPS & DecimalBit) ? 1 : 0;
  B = (NewPS & BreakBit) ? 1 : 0;
  U = (NewPS & UnusedBit) ? 1 : 0;
  V = (NewPS & OverflowBit) ? 1 : 0;
  N = (NewPS & NegativeBit) ? 1 : 0;
 }
 const Byte GetPS() {
  Byte PS = 0;
  if (C) PS |= CarryBit;
  if (Z) PS |= ZeroBit;
  if (I) PS |= InterruptDisableBit;
  if (D) PS |= DecimalBit;
  if (B) PS |= BreakBit;
  if (U) PS |= UnusedBit;
  if (V) PS |= OverflowBit;
  if (N) PS |= NegativeBit;

  return PS;
 }

 CPU_65XX_PS& operator=(const Byte PS) {
  SetPS(PS);
  return *this;
 }

 operator Byte() { return GetPS(); }
};

// CPU

struct CPU_65XX {
public:
 Word PC;  // Program counter
 Byte SP;  // Stack pointer (in range 0x00-0xFF)

 Byte A;  // Accumulator
 Byte X;  // X Register
 Byte Y;  // Y Register

 int32_t Cycles;

 struct CPU_65XX_PS PS;  // Processor status

 void Reset(Memory& mem);
 int32_t EatCycles(int32_t amount);

 Byte FetchByte(Memory& mem);
 Word FetchWord(Memory& mem);

 Byte ReadByte(Memory& mem, Word Address);
 Word ReadWord(Memory& mem, Word Address);

 void WriteByte(Memory& mem, Word Address, Byte Value);
 void WriteWord(Memory& mem, Word Address, Word Value);

 void StackPushByte(Memory& mem, Byte Value);
 void StackPushWord(Memory& mem, Word Value);

 Byte StackPopByte(Memory& mem);
 Word StackPopWord(Memory& mem);

 Byte FetchZPAddress(Memory& mem, Byte Offset = 0x00);
 Word FetchABAddress(Memory& mem, Byte Offset = 0x00);

 Word FetchINAddressX(Memory& Memory);
 Word FetchINAddressY(Memory& Memory);

 // private:
 void SetZeroNegativeFlags(Byte Value);

 void ConditionalBranch(Memory& mem, bool Value, bool Needed);

 void ADC(Byte Operand);
 void AND(Byte Operand);
 Byte ASL(Byte Value);
 void BCC(Memory& mem);
 void BCS(Memory& mem);
 void BEQ(Memory& mem);
 void BIT(Memory& mem, Word Address);
 void BMI(Memory& mem);
 void BNE(Memory& mem);
 void BPL(Memory& mem);
 void BRK(Memory& mem);
 void BVC(Memory& mem);
 void BVS(Memory& mem);
 void CLC();
 void CLD();
 void CLI();
 void CLV();
 void CMP(Byte Operand);
 void CPX(Byte Operand);
 void CPY(Byte Operand);
 void DEC(Memory& mem, Word Address);
 void DEX();
 void DEY();
 void EOR(Byte Value);
 void INC(Memory& mem, Word Address);
 void INX();
 void INY();
 void JMP(Word Address);
 void JSR(Memory& mem);
 void LDA(Byte Value);
 void LDX(Byte Value);
 void LDY(Byte Value);
 Byte LSR(Byte Value);
 void NOP();
 void ORA(Byte Value);
 void PHA(Memory& mem);
 void PHP(Memory& mem);
 void PLA(Memory& mem);
 void PLP(Memory& mem);
 Byte ROL(Byte Value);
 Byte ROR(Byte Value);
 void RTI(Memory& mem);
 void RTS(Memory& mem);
 void SBC(Byte Operand);
 void SEC();
 void SED();
 void SEI();
 void STA(Memory& mem, Word Address);
 void STX(Memory& mem, Word Address);
 void STY(Memory& mem, Word Address);
 void TAX();
 void TAY();
 void TSX();
 void TXA();
 void TXS();
 void TYA();
};

#endif