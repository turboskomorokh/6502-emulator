#include "parser.h"
#include "common.h"
#include <string>

void parseArgs(char** argv) {
 while (*++argv) {
  if (*argv[0] != '-')
   continue;
  std::string Argument = *argv, Value = *++argv;
  printf("%s %s\n", Argument.c_str(), Value.c_str());
  switch (Argument[1]) {
  case 'p':
   startPC = std::stoi((std::string)Value, nullptr, 16);
   break;
  case 'c':
   workCycles = std::stoi((std::string)Value, nullptr, 16);
   break;
  case 's':
   tickSpeed = std::stoi((std::string)Value, nullptr, 16);
   break;
  case 'f':
   binPath = Value;
   break;
  }
 }
}
