#ifndef EXMEM_REG_H
#define EXMEM_REG_H

#include <ostream>

#include "control-signal.hpp"

struct EXMEMMeta {
  unsigned int PC; // The address for the current instruction.

  unsigned int ALUResult; // Compute result.
  unsigned int SWValue;
  unsigned int WriteRegNum; // The name of Write Register.

  SignalMeta signal; // Control signal.
};

class EXMEMReg {
private:
  EXMEMMeta writeVersion; // EX stage write to writeVersion
  EXMEMMeta readVersion;  // MEM stage read from readVersion

public:
  EXMEMReg();
  void setWriteVersion(EXMEMMeta meta);
  void copyFromWriteToRead();
  EXMEMMeta getWriteVersion();
  EXMEMMeta getReadVersion();
  friend std::ostream &operator<<(std::ostream &os, const EXMEMReg &reg);
};

#endif /*EXMEM_REG_H*/
