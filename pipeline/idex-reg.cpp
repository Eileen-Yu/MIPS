#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "control-signal.hpp"
#include "idex-reg.hpp"
#include "utils.hpp"

IDEXReg::IDEXReg() {
  writeVersion = initIDEXMeta();
  readVersion = initIDEXMeta();
}

void IDEXReg::setWriteVersion(IDEXMeta meta) { writeVersion = meta; }

void IDEXReg::copyFromWriteToRead() { readVersion = writeVersion; }

IDEXMeta IDEXReg::getReadVersion() { return readVersion; }

IDEXMeta IDEXReg::getWriteVersion() { return writeVersion; }

std::ostream &operator<<(std::ostream &os, const IDEXReg &reg) {
  const char writeInfo[] =
      "Incr PC=%X, ReadReg1Value=%X, ReadReg2Value=%X,\nSEOffset=%d, "
      "WriteReg1Num=%d, WriteReg2Num=%d, Function=%X";

  std::vector<char> writeBuf(sizeof(writeInfo) + 128);

  std::snprintf(&writeBuf[0], writeBuf.size(), writeInfo, reg.writeVersion.PC,
                reg.writeVersion.readRegisterA, reg.writeVersion.readRegisterB,
                reg.writeVersion.signExtOffset, reg.writeVersion.writeReg1Num,
                reg.writeVersion.writeReg2Num, reg.writeVersion.funcCode);

  std::string writeMsg = &writeBuf[0];

  const char readInfo[] =
      "Incr PC=%X, ReadReg1Value=%X, ReadReg2Value=%X,\nSEOffset=%d, "
      "WriteReg1Num=%d, WriteReg2Num=%d, Function=%X";

  std::vector<char> readBuf(sizeof(readInfo) + 128);

  std::snprintf(&readBuf[0], readBuf.size(), readInfo, reg.readVersion.PC,
                reg.readVersion.readRegisterA, reg.readVersion.readRegisterB,
                reg.readVersion.signExtOffset, reg.readVersion.writeReg1Num,
                reg.readVersion.writeReg2Num, reg.readVersion.funcCode);

  std::string readMsg = &readBuf[0];

  return os << "ID/EX_Write (written to by the ID stage)\n"
            << "----------------------------------------\n"
            << reg.writeVersion.signal << "\n"
            << "----------------------------------------\n"
            << writeMsg << "\n"
            << "ID/EX_Read (read by the EX stage)\n"
            << "----------------------------------------\n"
            << reg.readVersion.signal << "\n"
            << "----------------------------------------\n"
            << readMsg << "\n";
}
