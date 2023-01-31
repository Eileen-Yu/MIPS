#ifndef IFID_REG_H
#define IFID_REG_H

#include <ostream>

struct IFIDMeta {
  unsigned int address;
  unsigned int instruction;
};

class IFIDReg {
private:
  IFIDMeta writeVersion;
  IFIDMeta readVersion;

public:
  IFIDReg();
  void setWriteVersion(IFIDMeta meta);
  void copyFromWriteToRead();
  IFIDMeta getWriteVersion();
  IFIDMeta getReadVersion();
  friend std::ostream &operator<<(std::ostream &os, const IFIDReg &reg);
};

#endif /*IFID_REG_H*/
