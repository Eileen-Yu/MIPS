#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "ifid-reg.hpp"

IFIDReg::IFIDReg() {
  writeVersion.address = 0x00000000;
  writeVersion.instruction = 0x00000000;
}

void IFIDReg::setWriteVersion(IFIDMeta meta) { writeVersion = meta; }

void IFIDReg::copyFromWriteToRead() { readVersion = writeVersion; }

IFIDMeta IFIDReg::getReadVersion() { return readVersion; }

IFIDMeta IFIDReg::getWriteVersion() { return writeVersion; }

std::ostream &operator<<(std::ostream &os, const IFIDReg &reg) {
  const char writeInfo[] = "Inst = %X  IncrPC = %X";
  std::vector<char> writeBuf(sizeof(writeInfo) + 32);
  std::snprintf(&writeBuf[0], writeBuf.size(), writeInfo,
                reg.writeVersion.instruction, reg.writeVersion.address);
  std::string writeMsg = &writeBuf[0];

  const char readInfo[] = "Inst = %X  IncrPC = %X";
  std::vector<char> readBuf(sizeof(readInfo) + 32);
  std::snprintf(&readBuf[0], readBuf.size(), readInfo,
                reg.readVersion.instruction, reg.readVersion.address);
  std::string readMsg = &readBuf[0];

  return os << "IF/ID_Write (written to by the IF stage)\n"
            << "----------------------------------------\n"
            << writeMsg << "\n"
            << "IF/ID_Read (read by the ID stage)\n"
            << "----------------------------------------\n"
            << readMsg << "\n";
}
