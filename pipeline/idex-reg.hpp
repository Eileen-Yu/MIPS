#ifndef IDEX_REG_H
#define IDEX_REG_H

#include <ostream>

#include "control-signal.hpp"

struct IDEXMeta {
  unsigned int PC;            // The address for the current instruction.
  unsigned int inst;          // The instruction.
  unsigned int readRegisterA; // The value of Read Register 1.
  unsigned int readRegisterB; // The value of Read Register 2. Invalid for Lw
  int signExtOffset;          // The offset.

  unsigned int writeReg1Num; // The index/name of Write Register.
  unsigned int writeReg2Num; // The index/name of Write Register.

  unsigned int funcCode; // The function code.(HEX)

  SignalMeta signal; // Control signal.
};

class IDEXReg {
private:
  IDEXMeta writeVersion; // ID stage write to writeVersion
  IDEXMeta readVersion;  // EX stage read from readVersion

public:
  IDEXReg();
  void setWriteVersion(IDEXMeta meta);
  void copyFromWriteToRead();
  IDEXMeta getWriteVersion();
  IDEXMeta getReadVersion();
  friend std::ostream &operator<<(std::ostream &os, const IDEXReg &reg);
};

#endif /*IDEX_REG_H*/
