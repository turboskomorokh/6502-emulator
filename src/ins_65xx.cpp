#include "common.h"
#include "cpu_65xx.h"
#include "memory.h"
#include <stdint.h>

void CPU_65XX_SREG::setZeroNegativeFlags(Byte Reg) {
 Z = (Reg == 0);
 N = (Reg & 0x80) != 0;
}

void CPU_65XX::ADC(Byte Operand) {
 Word Sum = (Word)A + (Word)Operand + (Word)SREG.C;
 A        = (Sum & 0xFF);
 SREG.setZeroNegativeFlags(A);
 SREG.C = Sum > 0xFF;
}

void CPU_65XX::AND(Byte Operand) {
 A &= Operand;
 SREG.setZeroNegativeFlags(A);
}

void CPU_65XX::ASL(Byte& Dest) {
 EatCycles(1);
 SREG.C = (Dest & CPU_65XX_SREG::NegativeBit) != 0;
 Dest <<= 1;
 SREG.setZeroNegativeFlags(A);
}

void CPU_65XX::BranchIf(Memory& Memory, bool Value, bool Needed) {
 Byte Offset        = FetchByte(Memory);
 Word targetAddress = (PC + Offset) & 0xFFFF;
 if (Value == Needed) {
  PC = targetAddress;
  EatCycles(1);
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(1);
  }
 }
}

void CPU_65XX::BCC(Memory& Memory) { BranchIf(Memory, SREG.C, false); }

void CPU_65XX::BCS(Memory& Memory) { BranchIf(Memory, SREG.C, true); }

void CPU_65XX::BEQ(Memory& Memory) { BranchIf(Memory, SREG.Z, true); }

void CPU_65XX::BIT(Memory& Memory, Word Address) {
 Byte Operand    = ReadByte(Address, Memory);
 Byte Mask = A & Operand;
 SREG.Z = !Mask;
 SREG.Z = (Mask & CPU_65XX_SREG::NegativeBit) != 0;
 SREG.O = (Mask & CPU_65XX_SREG::OverflowBit) != 0;
}

void CPU_65XX::BMI(Memory& Memory) { BranchIf(Memory, SREG.N, true); }

void CPU_65XX::BNE(Memory& Memory) { BranchIf(Memory, SREG.N, false); }

void CPU_65XX::BPL(Memory& Memory) { BranchIf(Memory, SREG.N, false); }

void CPU_65XX::BRK(Memory& memory) {
 Word InterruptVector = 0xFFFE;
 StackPushWord(memory, PC + 1);
 StackPushByte(memory, SREG);

 PC     = ReadWord(InterruptVector, memory);

 SREG.B = true;
 SREG.I = true;
}

void CPU_65XX::BVC(Memory& Memory) { BranchIf(Memory, SREG.O, false); }

void CPU_65XX::BVS(Memory& Memory) { BranchIf(Memory, SREG.O, true); }

void CPU_65XX::CLC() {
 EatCycles(1);
 SREG.C = false;
}

void CPU_65XX::CLD() {
 EatCycles(1);
 SREG.D = false;
}

void CPU_65XX::CLI() {
 EatCycles(1);
 SREG.I = false;
}

void CPU_65XX::CLV() {
 EatCycles(1);
 SREG.O = false;
}

void CPU_65XX::CMP(Byte Operand) {
 Byte Result = A - Operand;
 SREG.C      = (A >= Operand);
 SREG.setZeroNegativeFlags(Result);
}

void CPU_65XX::CPX(Byte Operand) {
 Byte Result = X - Operand;
 SREG.C      = (X >= Operand);
 SREG.setZeroNegativeFlags(Result);
}

void CPU_65XX::CPY(Byte Operand) {
 Byte Result = Y - Operand;
 SREG.C      = (Y >= Operand);
 SREG.setZeroNegativeFlags(Result);
}

void CPU_65XX::DEC(Byte& Dest) {
 Dest--;
 SREG.setZeroNegativeFlags(Dest);
}

void CPU_65XX::DEX() {
 X--;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::DEY() {
 Y--;
 SREG.setZeroNegativeFlags(X);
 ;
}

void CPU_65XX::EOR(Memory& Memory, Word Address) {
 A ^= ReadByte(Address, Memory);
 SREG.setZeroNegativeFlags(A);
}

void CPU_65XX::INC(Byte& Dest) {
 Dest++;
 SREG.setZeroNegativeFlags(Dest);
}

void CPU_65XX::INX() {
 X++;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::INY() {
 Y++;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::JMP(Word Address) { PC = Address; }

void CPU_65XX::JSR(Memory& memory, Word Address) {
 StackPushWord(memory, PC - 1);
 PC = Address;
 EatCycles(1);
}

void CPU_65XX::LDA(Byte Operand) { A = Operand; }

void CPU_65XX::LDX(Byte Operand) { X = Operand; }

void CPU_65XX::LDY(Byte Operand) { Y = Operand; }

void CPU_65XX::LSR(Byte& Dest) {
 EatCycles(1);
 SREG.C = (Dest & CPU_65XX_SREG::NegativeBit) != 0;
 Dest >>= 1;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::NOP() { EatCycles(1); }

void CPU_65XX::ORA(Byte Operand) {
 A = A | Operand;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::PHA(Memory& memory) { StackPushByte(memory, A); }

void CPU_65XX::PHP(Memory& memory) { StackPushWord(memory, SREG); }

void CPU_65XX::PLA(Memory& memory) {
 EatCycles(1);
 A = StackPopByte(memory);
 SREG.setZeroNegativeFlags(A);
}

void CPU_65XX::PLP(Memory& memory) {
 EatCycles(1);
 SREG = StackPopByte(memory);
}

void CPU_65XX::ROL(Byte& Dest) {
 EatCycles(1);
 Byte Bit = SREG.C ? CPU_65XX_SREG::ZeroBit : 0;
 SREG.C   = (Dest & CPU_65XX_SREG::NegativeBit) > 0;
 Dest <<= 1;
 Dest |= Bit;
 SREG.setZeroNegativeFlags(Dest);
}

void CPU_65XX::ROR(Byte& Dest) {
 EatCycles(1);
 bool Bit = (Dest & CPU_65XX_SREG::ZeroBit) > 0;
 Dest >>= 1;
 if (SREG.C)
  Dest |= CPU_65XX_SREG::NegativeBit;

 SREG.C = Bit;
}

void CPU_65XX::RTI(Memory& memory) {
 SREG = StackPopByte(memory);
 PC   = StackPopWord(memory);
}

void CPU_65XX::RTS(Memory& memory) {
 Word StackPC = StackPopWord(memory);
 PC           = StackPC + 1;
 EatCycles(2);
}

void CPU_65XX::SBC(Byte Operand) { ADC(~Operand); }

void CPU_65XX::SEC() {
 EatCycles(1);
 SREG.C = true;
}

void CPU_65XX::SED() {
 EatCycles(1);
 SREG.D = true;
}

void CPU_65XX::SEI() {
 EatCycles(1);
 SREG.I = true;
}

void CPU_65XX::STA(Memory& memory, Word Address) { WriteByte(memory, Address, A); }

void CPU_65XX::STX(Memory& memory, Word Address) { WriteByte(memory, Address, X); }

void CPU_65XX::STY(Memory& memory, Word Address) { WriteByte(memory, Address, Y); }

void CPU_65XX::TAX() {
 EatCycles(1);
 X = A;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::TAY() {
 EatCycles(1);
 Y = A;
 SREG.setZeroNegativeFlags(Y);
}

void CPU_65XX::TSX() {
 EatCycles(1);
 X = SP;
 SREG.setZeroNegativeFlags(X);
}

void CPU_65XX::TXA() {
 EatCycles(1);
 A = X;
 SREG.setZeroNegativeFlags(A);
}

void CPU_65XX::TXS() {
 EatCycles(1);
 SP = X;
 SREG.setZeroNegativeFlags(SP);
}

void CPU_65XX::TYA() {
 EatCycles(1);
 A = Y;
 SREG.setZeroNegativeFlags(A);
}
