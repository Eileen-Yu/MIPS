#ifndef MEMWB_REG_H
#define MEMWB_REG_H

#include <ostream>

#include "control-signal.hpp"

struct MEMWBMeta {
  unsigned int PC; // The address for the current instruction.

  unsigned int ALUResult; // Compute result.
  unsigned int SWValue;
  unsigned int WriteRegNum; // The name of Write Register.

  unsigned int LWDataValue; // The value load from memory.

  SignalMeta signal; // Control signal.
};

class MEMWBReg {
private:
  MEMWBMeta writeVersion; // EX stage write to writeVersion
  MEMWBMeta readVersion;  // MEM stage read from readVersion

public:
  MEMWBReg();
  void setWriteVersion(MEMWBMeta meta);
  void copyFromWriteToRead();
  MEMWBMeta getWriteVersion();
  MEMWBMeta getReadVersion();
  friend std::ostream &operator<<(std::ostream &os, const MEMWBReg &reg);
};

#endif /*MEMWB_REG_H*/
