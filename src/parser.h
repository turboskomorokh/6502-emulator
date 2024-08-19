#ifndef _EMULATOR_PARSER_H_
#define _EMULATOR_PARSER_H_
#include <map>
#include <string>

enum {
 ARGUMENT_CYCLES,
 ARGUMENT_SPEED,
 ARGUMENT_PC,
};

extern std::string PossibleArgs[];
void parseArgs(char** argv);

#endif