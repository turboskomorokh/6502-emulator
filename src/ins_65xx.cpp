#include "common.h"
#include "cpu_65xx.h"
#include "memory.h"
#include <cstdio>

void CPU_65XX::SetZeroNegativeFlags(Byte Value) {
 PS.Z = (Value == 0);
 PS.N = (Value & CPU_65XX_PS::NegativeBit) != 0;
}

void CPU_65XX::ADC(Byte Operand) {
 EatCycles(1);
 Word Sum = (Word)A + (Word)Operand + (Word)PS.C;
 PS.C     = (Sum > 0xFF);
 A        = (Byte)Sum;
 PS.V     = (((A ^ Operand) & (A ^ (Byte)Sum)) & 0x80) != 0;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::AND(Byte Operand) {
 EatCycles(1);
 A &= Operand;
 SetZeroNegativeFlags(A);
}

Byte CPU_65XX::ASL(Byte Value) {
 EatCycles(1);

 PS.C = (Value & CPU_65XX_PS::NegativeBit) != 0;
 Value <<= 1;
 SetZeroNegativeFlags(Value);
 return Value;
}

void CPU_65XX::ConditionalBranch(Memory& mem, bool Value, bool Needed) {
 SignByte Offset = (SignByte)FetchByte(mem);
 if (Value == Needed) {
  const Word PrevPC = PC;
  PC += Offset;
  EatCycles(1);

  if ((PC >> 8) != (PrevPC >> 8)) EatCycles(1);
 }
}

void CPU_65XX::BCS(Memory& mem) { ConditionalBranch(mem, PS.C, true); }

void CPU_65XX::BCC(Memory& mem) { ConditionalBranch(mem, PS.C, false); }

void CPU_65XX::BEQ(Memory& mem) { ConditionalBranch(mem, PS.Z, true); }

void CPU_65XX::BNE(Memory& mem) { ConditionalBranch(mem, PS.Z, false); }

void CPU_65XX::BMI(Memory& mem) { ConditionalBranch(mem, PS.N, true); }

void CPU_65XX::BPL(Memory& mem) { ConditionalBranch(mem, PS.N, false); }

void CPU_65XX::BVS(Memory& mem) { ConditionalBranch(mem, PS.V, true); }

void CPU_65XX::BVC(Memory& mem) { ConditionalBranch(mem, PS.V, false); }

void CPU_65XX::BIT(Memory& mem, Word Address) {
 Byte Value = ReadByte(mem, Address);
 Word Mask  = A & Value;

 PS.Z       = (Mask == 0);
 PS.V       = (Value & CPU_65XX_PS::OverflowBit) != 0;
 PS.N       = (Value & CPU_65XX_PS::NegativeBit) != 0;
}

void CPU_65XX::BRK(Memory& mem) {
 StackPushWord(mem, PC + 1);
 const Word InterruptVector = 0xFFFE;
 PS.B = true;
 PS.U = true;
 StackPushByte(mem, PS);
 PC     = ReadWord(mem, InterruptVector);
 PS.I = true;
}

void CPU_65XX::CLC() {
 EatCycles(1);
 PS.C = 0;
}

void CPU_65XX::CLD() {
 EatCycles(1);
 PS.D = 0;
}

void CPU_65XX::CLI() {
 EatCycles(1);
 PS.I = 0;
}

void CPU_65XX::CLV() {
 EatCycles(1);
 PS.V = 0;
}

void CPU_65XX::CMP(Byte Operand) {
 EatCycles(1);
 Byte Sub = A - Operand;
 printf("0x%02x - 0x%02x = 0x%02x ", A, Operand, Sub);

 PS.N = (Sub & CPU_65XX_PS::NegativeBit) > 0;
 PS.C = (A >= Operand);
 PS.Z = (A == Operand);
}

void CPU_65XX::CPX(Byte Operand) {
 EatCycles(1);
 Byte Sub = X - Operand;
 printf("0x%02x - 0x%02x = 0x%02x ", X, Operand, Sub);

 PS.C = (X >= Operand);
 PS.N = (Sub & CPU_65XX_PS::NegativeBit) != 0;
 PS.Z = (X == Operand);
}

void CPU_65XX::CPY(Byte Operand) {
 EatCycles(1);
 Byte Sub = Y - Operand;
 printf("0x%02x - 0x%02x = 0x%02x ", Y, Operand, Sub);

 PS.C = (Y >= Operand);
 PS.N = (Sub & CPU_65XX_PS::NegativeBit) != 0;
 PS.Z = (Y == Operand);
}

void CPU_65XX::DEC(Memory& mem, Word Address) {
 Byte Value = ReadByte(mem, Address);
 Value--;
 WriteByte(mem, Address, Value);
 SetZeroNegativeFlags(Value);
}

void CPU_65XX::DEX() {
 EatCycles(1);
 X--;
 SetZeroNegativeFlags(X);
}

void CPU_65XX::DEY() {
 EatCycles(1);
 Y--;
 SetZeroNegativeFlags(Y);
}

void CPU_65XX::EOR(Byte Value) {
 EatCycles(1);
 A ^= Value;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::INC(Memory& mem, Word Address) {
 Byte Value = ReadByte(mem, Address);
 Value++;
 WriteByte(mem, Address, Value);
 SetZeroNegativeFlags(Value);
}

void CPU_65XX::INX() {
 EatCycles(1);
 X++;
 SetZeroNegativeFlags(X);
 printf("X: %d ", X);
}

void CPU_65XX::INY() {
 EatCycles(1);
 Y++;
 SetZeroNegativeFlags(Y);
 printf("Y: %d ", Y);
}

void CPU_65XX::JMP(Word Address) {
 printf("0x%04x ", Address);
 //  if(Address == 0x371e)
 //   printf("N 0x%02x V 0x%02x Z 0x%02x C 0x%02x ", PS.N, PS.V, PS.Z, PS.C);

 PC = Address;
}

void CPU_65XX::JSR(Memory& mem) {
 Word EffectiveAddress = FetchWord(mem);
 StackPushWord(mem, PC - 1);
 PC = EffectiveAddress;
}

void CPU_65XX::LDA(Byte Value) {
 printf("LOAD 0x%02x ", Value);
 A = Value;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::LDX(Byte Value) {
 printf("LOAD 0x%02x ", Value);
 X = Value;
 SetZeroNegativeFlags(X);
}

void CPU_65XX::LDY(Byte Value) {
 printf("LOAD 0x%02x ", Value);
 Y = Value;
 SetZeroNegativeFlags(Y);
}

Byte CPU_65XX::LSR(Byte Value) {
 EatCycles(1);

 PS.C = (Value & CPU_65XX_PS::CarryBit) != 0;
 Value >>= 1;
 PS.N = (Value & CPU_65XX_PS::NegativeBit) != 0;
 PS.Z = (Value == 0);
 return Value;
}

void CPU_65XX::NOP() { EatCycles(1); }

void CPU_65XX::ORA(Byte Value) {
 EatCycles(1);
 A |= Value;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::PHA(Memory& mem) {
 EatCycles(1);
 StackPushByte(mem, A);
 printf("%02x ", A);
}

void CPU_65XX::PHP(Memory& mem) {
 EatCycles(1);
 PS.U = 1;
 PS.B = 1;
 printf("%02x ", PS.GetPS());
 StackPushByte(mem, PS);
}

void CPU_65XX::PLA(Memory& mem) {
 EatCycles(1);
 A = StackPopByte(mem);
 SetZeroNegativeFlags(A);
 printf("%02x ", A);
}

void CPU_65XX::PLP(Memory& mem) {
 EatCycles(2);
 PS   = StackPopByte(mem);
 PS.B = false;
 PS.U = false;
 printf("%02x ", PS.GetPS());
}

Byte CPU_65XX::ROL(Byte Value) {
 Byte Bit = PS.C;
 PS.C = (Value & CPU_65XX_PS::NegativeBit) != 0;

 Value <<= 1;
 Value |= Bit;

 SetZeroNegativeFlags(Value);
 EatCycles(1);
 return Value;
}

Byte CPU_65XX::ROR(Byte Value) {
 Byte Bit = PS.C;
 PS.C = (Value & CPU_65XX_PS::NegativeBit) != 0;

 Value <<= 1;
 Value |= Bit;

 SetZeroNegativeFlags(Value);
 EatCycles(1);
 return Value;
}

void CPU_65XX::RTI(Memory& mem) {
 EatCycles(2);
 PS   = StackPopByte(mem);
 PS.B = false;
 PS.U = false;
 PC   = StackPopWord(mem);
}

void CPU_65XX::RTS(Memory& mem) {
 EatCycles(3);
 Word EffectiveAddress = StackPopWord(mem);
 PC                    = EffectiveAddress + 1;
}

void CPU_65XX::SBC(Byte Operand) { ADC(~Operand); }

void CPU_65XX::SEC() {
 EatCycles(1);
 PS.C = true;
}

void CPU_65XX::SED() {
 EatCycles(1);
 PS.D = 1;
}

void CPU_65XX::SEI() {
 EatCycles(1);
 PS.I = 1;
}

void CPU_65XX::STA(Memory& mem, Word Address) { WriteByte(mem, Address, A); }

void CPU_65XX::STX(Memory& mem, Word Address) { WriteByte(mem, Address, X); }

void CPU_65XX::STY(Memory& mem, Word Address) { WriteByte(mem, Address, Y); }

void CPU_65XX::TAX() {
 EatCycles(1);
 X = A;
 SetZeroNegativeFlags(X);
}

void CPU_65XX::TXA() {
 EatCycles(1);
 A = X;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::TAY() {
 EatCycles(1);
 Y = A;
 SetZeroNegativeFlags(Y);
}

void CPU_65XX::TYA() {
 EatCycles(1);
 A = Y;
 SetZeroNegativeFlags(A);
}

void CPU_65XX::TSX() {
 EatCycles(1);
 X = SP;
 SetZeroNegativeFlags(X);
}

void CPU_65XX::TXS() {
 EatCycles(1);
 SP = X;
}
