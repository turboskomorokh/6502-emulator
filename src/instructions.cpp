#include "common.h"
#include "cpu_6502.h"
#include <stdint.h>

void CPU_6502::ADC(Byte M) {
 Word Sum = (Word)A + (Word)M + (Word)Flag.C;

 Flag.C   = (Sum > 0xFF);
 A        = (Byte)Sum;
 Flag.Z   = (A == 0);
 Flag.N   = (A & 0x80);
 Flag.O   = (((A ^ M) & (A ^ Sum)) & 0x80);
}

void CPU_6502::AND(Byte M) {
 A &= M;
 Flag.Z = (A == 0);
 Flag.N = (A & 0x80);
}

void CPU_6502::ASL(uint32_t& Cycles, Byte& Dest) {
 EatCycles(Cycles, 1);
 Flag.C = (Dest & 0x80);
 Dest <<= 1;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
}

void CPU_6502::BCC(uint32_t& Cycles, Byte Offset) {
 if (!Flag.C) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  PC = targetAddress;
  EatCycles(Cycles, 1);
 }
}

void CPU_6502::BCS(uint32_t& Cycles, Byte Offset) {
 if (Flag.C) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BEQ(uint32_t& Cycles, Byte Offset) {
 if (Flag.Z) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BIT(Byte M) {
 Byte Mask = A & M;
 Flag.Z    = (Mask == 0);
 Flag.N    = (Mask & 0x80);
 Flag.O    = (Mask & 0x80);
}

void CPU_6502::BMI(uint32_t& Cycles, Byte Offset) {
 if (Flag.N) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BNE(uint32_t& Cycles, Byte Offset) {
 if (!Flag.Z) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BPL(uint32_t& Cycles, Byte Offset) {
 if (!Flag.N) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BRK(uint32_t& Cycles, Memory& memory) {
 Word InterruptVector = 0xFFFE;
 Flag.B               = true;
 Flag.I               = false;
 Byte Status          = GetProcessorStatus(Cycles);

 StackPushWord(Cycles, memory, PC + 1);

 StackPushByte(Cycles, memory, Status);
 PC = ReadWord(Cycles, InterruptVector, memory);
}

void CPU_6502::BVC(uint32_t& Cycles, Byte Offset) {
 if (!Flag.O) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::BVS(uint32_t& Cycles, Byte Offset) {
 if (Flag.O) {
  Word targetAddress = (PC + Offset) & 0xFFFF;
  if ((PC & 0xFF00) != (targetAddress & 0xFF00)) {
   EatCycles(Cycles, 2);
  }
  EatCycles(Cycles, 1);
  PC = targetAddress;
 }
}

void CPU_6502::CLC(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.C = false;
}

void CPU_6502::CLD(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.D = false;
}

void CPU_6502::CLI(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.I = false;
}

void CPU_6502::CLV(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.O = false;
}

void CPU_6502::CMP(Byte M) {
 Byte Result = A - M;
 Flag.C      = (A >= M);
 Flag.Z      = (Result == 0);
 Flag.N      = (Result & 0x80);
}

void CPU_6502::CPX(Byte M) {
 Byte Result = X - M;
 Flag.C      = (X >= M);
 Flag.Z      = (Result == 0);
 Flag.N      = (Result & 0x80);
}

void CPU_6502::CPY(Byte M) {
 Byte Result = Y - M;
 Flag.C      = (Y >= M);
 Flag.Z      = (Result == 0);
 Flag.N      = (Result & 0x80);
}

void CPU_6502::DEC(Byte& Dest) {
 if (Dest)
  Dest--;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
}

void CPU_6502::DEX(uint32_t& Cycles) {
 if (X)
  X--;
 Flag.Z = (X == 0);
 Flag.N = (X & 0x80);
 EatCycles(Cycles, 1);
}

void CPU_6502::DEY(uint32_t& Cycles) {
 if (Y)
  Y--;
 Flag.Z = (Y == 0);
 Flag.N = (Y & 0x80);
 EatCycles(Cycles, 1);
}

void CPU_6502::EOR(Byte M) {
 A ^= M;
 Flag.Z = (A == 0);
 Flag.N = (A & 0x80);
}

void CPU_6502::INC(Byte& Dest) {
 Dest++;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
}

void CPU_6502::INX(uint32_t& Cycles) {
 X++;
 Flag.Z = (X == 0);
 Flag.N = (X & 0x80);
}

void CPU_6502::INY(uint32_t& Cycles) {
 Y++;
 Flag.Z = (Y == 0);
 Flag.N = (Y & 0x80);
}

void CPU_6502::JMP(Word Address) { PC = Address; }

void CPU_6502::JSR(uint32_t& Cycles, Memory& memory, Word Address) {
 StackPushWord(Cycles, memory, PC);
 PC = Address;
 EatCycles(Cycles, 1);
}

void CPU_6502::LDA(Byte M) { A = M; }

void CPU_6502::LDX(Byte M) { X = M; }

void CPU_6502::LDY(Byte M) { Y = M; }

void CPU_6502::LSR(uint32_t& Cycles, Byte& Dest) {
 EatCycles(Cycles, 1);
 Flag.C = (Dest & 0x80);
 Dest >>= 1;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
}

void CPU_6502::NOP(uint32_t& Cycles) { EatCycles(Cycles, 1); }

void CPU_6502::ORA(Byte M) {
 A      = A | M;
 Flag.Z = (A == 0);
 Flag.N = (A & 0x80);
}

void CPU_6502::PHA(uint32_t& Cycles, Memory& memory) {
 StackPushByte(Cycles, memory, A);
 EatCycles(Cycles, 1);
}

void CPU_6502::PHP(uint32_t& Cycles, Memory& memory) {
 Byte ProcessorStatus = GetProcessorStatus();
 StackPushWord(Cycles, memory, ProcessorStatus);
}

void CPU_6502::PLA(uint32_t& Cycles, Memory& memory) {
 A = StackPullByte(Cycles, memory);
 EatCycles(Cycles, 2);
}

void CPU_6502::PLP(uint32_t& Cycles, Memory& memory) {
 Byte ProcessorStatus = StackPullByte(Cycles, memory);
 SetProcessorStatus(Cycles, ProcessorStatus & 0xEF);
 EatCycles(Cycles, 1);
}

void CPU_6502::ROL(uint32_t& Cycles, Byte& Dest) {
 bool Carry = Flag.C;
 Flag.C     = (Dest & 0x80) != 0;
 Dest <<= 1;

 if (Carry)
  Dest |= 0x01;
 else
  Dest &= ~0x01;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
 EatCycles(Cycles, 1);
}

void CPU_6502::ROR(uint32_t& Cycles, Byte& Dest) {
 bool Carry = Flag.C;
 Flag.C     = (Dest & 0x80) != 0;
 Dest >>= 1;

 if (Carry)
  Dest |= 0x01;
 else
  Dest &= ~0x01;
 Flag.Z = (Dest == 0);
 Flag.N = (Dest & 0x80);
 EatCycles(Cycles, 1);
}

void CPU_6502::RTI(uint32_t& Cycles, Memory& memory) {
 Byte ProcessorStatus = StackPullByte(Cycles, memory);
 Word StackPC         = StackPullWord(Cycles, memory);

 SetProcessorStatus(Cycles, ProcessorStatus);
 PC = StackPC;
 EatCycles(Cycles, 1);
}

void CPU_6502::RTS(uint32_t& Cycles, Memory& memory) {
 Word StackPC = StackPullWord(Cycles, memory);
 PC           = StackPC;
 EatCycles(Cycles, 3);
}

void CPU_6502::SBC(Byte M) {
 printf("A: 0x%02x, M: 0x%02x, C: 0x%02x", A, M, Flag.C);   
 Word Sub = (Word)A - (Word)M - (Word)(1 - Flag.C);

 Flag.C   = (Sub >= 0x100);
 A        = (Byte)Sub;
 printf("Res: 0x%04x ", Sub);

 Flag.Z   = (A == 0);
 Flag.N   = (A & 0x80);
 Flag.O   = (((A ^ M) & (A ^ (Byte)Sub)) & 0x80) != 0;
}

void CPU_6502::SEC(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.C = true;
}

void CPU_6502::SED(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.D = true;
}

void CPU_6502::SEI(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Flag.I = true;
}

void CPU_6502::STA(uint32_t& Cycles, Memory& memory, Word Address) { WriteValueToAddress(Cycles, memory, Address, A); }

void CPU_6502::STX(uint32_t& Cycles, Memory& memory, Word Address) { WriteValueToAddress(Cycles, memory, Address, X); }

void CPU_6502::STY(uint32_t& Cycles, Memory& memory, Word Address) { WriteValueToAddress(Cycles, memory, Address, Y); }

void CPU_6502::TAX(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 X = A;
}

void CPU_6502::TAY(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 Y = A;
}

void CPU_6502::TSX(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 X = SP;
}

void CPU_6502::TXA(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 A = X;
}

void CPU_6502::TXS(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 SP = X;
}

void CPU_6502::TYA(uint32_t& Cycles) {
 EatCycles(Cycles, 1);
 A = Y;
}