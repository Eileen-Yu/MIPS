#ifndef CONTROL_SIGNAL_H
#define CONTROL_SIGNAL_H

#include <ostream>
#include <string>

struct SignalMeta {
  char RegDst;       // 1 | 0 | X
  char RegWrite;     // 1 | 0
  char ALUSrc;       // 0 | 1
  std::string ALUOp; // 00 | 01 | 10
  char MemWrite;     // 0 | 1
  char MemRead;      // 0 | 1
  char MemToReg;     // 0 | 1 | X
  char Branch;       // 0 | 1
  friend std::ostream &operator<<(std::ostream &os, const SignalMeta &signal);
};

#endif /*CONTROL_SIGNAL_H*/
