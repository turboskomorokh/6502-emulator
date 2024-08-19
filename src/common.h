#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <cstdint>

typedef char SignByte;
typedef unsigned char Byte;
typedef unsigned short Word;

extern uint32_t tickSpeed;
extern uint32_t startPC;
extern int32_t workCycles;
extern std::string binPath;

#endif