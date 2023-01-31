#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "control-signal.hpp"

std::ostream &operator<<(std::ostream &os, const SignalMeta &signal) {
  SignalMeta ctrlSignal = signal;

  const char readInfo[] =
      "Control: RegDst=%c, ALUSrc=%c, ALUOp=%s, MemRead=%c, "
      "MemWrite=%c,\nBranch=%c, MemToReg=%c, RegWrite=%c";

  std::vector<char> readBuf(sizeof(readInfo) + 128);

  std::snprintf(&readBuf[0], readBuf.size(), readInfo, ctrlSignal.RegDst,
                ctrlSignal.ALUSrc, ctrlSignal.ALUOp.c_str(), ctrlSignal.MemRead,
                ctrlSignal.MemWrite, ctrlSignal.Branch, ctrlSignal.MemToReg,
                ctrlSignal.RegWrite);

  std::string readMsg = &readBuf[0];

  return os << readMsg;
}
