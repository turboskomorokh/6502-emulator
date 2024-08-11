#include "common.h"
#include "cpu_65xx.h"
#include <stdint.h>

void CPU_65XX::ADC(Byte M) {
 Word Sum = (Word)A + (Word)M + (Word)SREG.C;

 SREG.C   = (Sum > 0xFF);
 A        = (Byte)Sum;
 SREG.Z   = (A == 0);
 SREG.N   = (A & 0x80) != 0;
 SREG.O   = (((A ^ M) & (A ^ Sum)) & 0x80) != 0;
}

void CPU_65XX::AND(Byte M) {
 A &= M;
 SREG.Z = (A == 0);
 SREG.N = (A & 0x80) != 0;
}

void CPU_65XX::ASL(Byte& Dest) {
 EatCycles(1);
 SREG.C = (Dest & 0x80) != 0;
 Dest <<= 1;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
}

void CPU_65XX::BCC(Byte Offset) {
 if (!SREG.C) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  PC = targetAddress;
  EatCycles(1);
 }
}

void CPU_65XX::BCS(Byte Offset) {
 if (SREG.C) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BEQ(Byte Offset) {
 if (SREG.Z) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BIT(Byte M) {
 Byte Mask = A & M;
 SREG.Z    = (Mask == 0);
 SREG.N    = (Mask & 0x80) != 0;
 SREG.O    = (Mask & 0x80) != 0;
}

void CPU_65XX::BMI(Byte Offset) {
 if (SREG.N) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BNE(Byte Offset) {
 if (!SREG.Z) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BPL(Byte Offset) {
 if (!SREG.N) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BRK(Memory& memory) {
 Word InterruptVector = 0xFFFE;
 SREG.B               = true;
 SREG.I               = false;

 StackPushWord(memory, PC + 1);

 StackPushByte(memory, SREG);
 PC = ReadWord(InterruptVector, memory);
}

void CPU_65XX::BVC(Byte Offset) {
 if (!SREG.O) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

void CPU_65XX::BVS(Byte Offset) {
 if (SREG.O) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(2);
  }
  EatCycles(1);
  PC = targetAddress;
 }
}

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

void CPU_65XX::CMP(Byte M) {
 Byte Result = A - M;
 SREG.C      = (A >= M);
 SREG.Z      = (Result == 0);
 SREG.N      = (Result & 0x80) != 0;
}

void CPU_65XX::CPX(Byte M) {
 Byte Result = X - M;
 SREG.C      = (X >= M);
 SREG.Z      = (Result == 0);
 SREG.N      = (Result & 0x80) != 0;
}

void CPU_65XX::CPY(Byte M) {
 Byte Result = Y - M;
 SREG.C      = (Y >= M);
 SREG.Z      = (Result == 0);
 SREG.N      = (Result & 0x80) != 0;
}

void CPU_65XX::DEC(Byte& Dest) {
 if (Dest)
  Dest--;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
}

void CPU_65XX::DEX() {
 if (X)
  X--;
 SREG.Z = (X == 0);
 SREG.N = (X & 0x80) != 0;
 EatCycles(1);
}

void CPU_65XX::DEY() {
 if (Y)
  Y--;
 SREG.Z = (Y == 0);
 SREG.N = (Y & 0x80) != 0;
 EatCycles(1);
}

void CPU_65XX::EOR(Byte M) {
 A ^= M;
 SREG.Z = (A == 0);
 SREG.N = (A & 0x80) != 0;
}

void CPU_65XX::INC(Byte& Dest) {
 Dest++;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
}

void CPU_65XX::INX() {
 X++;
 SREG.Z = (X == 0);
 SREG.N = (X & 0x80) != 0;
}

void CPU_65XX::INY() {
 Y++;
 SREG.Z = (Y == 0);
 SREG.N = (Y & 0x80) != 0;
}

void CPU_65XX::JMP(Word Address) { PC = Address; }

void CPU_65XX::JSR(Memory& memory, Word Address) {
 StackPushWord(memory, PC);
 PC = Address;
 EatCycles(1);
}

void CPU_65XX::LDA(Byte M) { A = M; }

void CPU_65XX::LDX(Byte M) { X = M; }

void CPU_65XX::LDY(Byte M) { Y = M; }

void CPU_65XX::LSR(Byte& Dest) {
 EatCycles(1);
 SREG.C = (Dest & 0x80) != 0;
 Dest >>= 1;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
}

void CPU_65XX::NOP() { EatCycles(1); }

void CPU_65XX::ORA(Byte M) {
 A      = A | M;
 SREG.Z = (A == 0);
 SREG.N = (A & 0x80) != 0;
}

void CPU_65XX::PHA(Memory& memory) {
 StackPushByte(memory, A);
 EatCycles(1);
}

void CPU_65XX::PHP(Memory& memory) { StackPushWord(memory, SREG); }

void CPU_65XX::PLA(Memory& memory) {
 A = StackPullByte(memory);
 EatCycles(2);
}

void CPU_65XX::PLP(Memory& memory) {
 SREG = StackPullByte(memory);
 EatCycles(2);
}

void CPU_65XX::ROL(Byte& Dest) {
 bool Carry = SREG.C;
 SREG.C     = (Dest & 0x80) != 0;
 Dest <<= 1;

 if (Carry)
  Dest |= 0x01;
 else
  Dest &= ~0x01;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
 EatCycles(1);
}

void CPU_65XX::ROR(Byte& Dest) {
 bool Carry = SREG.C;
 SREG.C     = (Dest & 0x80) != 0;
 Dest >>= 1;

 if (Carry)
  Dest |= 0x01;
 else
  Dest &= ~0x01;
 SREG.Z = (Dest == 0);
 SREG.N = (Dest & 0x80) != 0;
 EatCycles(1);
}

void CPU_65XX::RTI(Memory& memory) {
 Byte ProcessorStatus = StackPullByte(memory);
 Word StackPC         = StackPullWord(memory);

 SREG                 = ProcessorStatus;
 PC                   = StackPC;
 EatCycles(2);
}

void CPU_65XX::RTS(Memory& memory) {
 Word StackPC = StackPullWord(memory);
 PC           = StackPC;
 EatCycles(3);
}

void CPU_65XX::SBC(Byte M) {
 printf("A: 0x%02x, M: 0x%02x, C: 0x%02x", A, M, SREG.C);
 Word Sub = (Word)A - (Word)M - (Word)(1 - SREG.C);

 SREG.C   = (Sub >= 0x100);
 A        = (Byte)Sub;
 printf("Res: 0x%04x ", Sub);

 SREG.Z = (A == 0);
 SREG.N = (A & 0x80) != 0;
 SREG.O = (((A ^ M) & (A ^ (Byte)Sub)) & 0x80) != 0;
}

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
}

void CPU_65XX::TAY() {
 EatCycles(1);
 Y = A;
}

void CPU_65XX::TSX() {
 EatCycles(1);
 X = SP;
}

void CPU_65XX::TXA() {
 EatCycles(1);
 A = X;
}

void CPU_65XX::TXS() {
 EatCycles(1);
 SP = X;
}

void CPU_65XX::TYA() {
 EatCycles(1);
 A = Y;
}
