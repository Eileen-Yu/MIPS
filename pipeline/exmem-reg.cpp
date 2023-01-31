#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "control-signal.hpp"
#include "exmem-reg.hpp"
#include "utils.hpp"

EXMEMReg::EXMEMReg() {
  writeVersion = initEXMEMMeta();
  readVersion = initEXMEMMeta();
}

void EXMEMReg::setWriteVersion(EXMEMMeta meta) { writeVersion = meta; }

void EXMEMReg::copyFromWriteToRead() { readVersion = writeVersion; }

EXMEMMeta EXMEMReg::getReadVersion() { return readVersion; }

EXMEMMeta EXMEMReg::getWriteVersion() { return writeVersion; }

std::ostream &operator<<(std::ostream &os, const EXMEMReg &reg) {
  const char writeInfo[] = "ALUResult=%X, SWValue=%X, WriteRegNum=%d";

  std::vector<char> writeBuf(sizeof(writeInfo) + 32);

  std::snprintf(&writeBuf[0], writeBuf.size(), writeInfo,
                reg.writeVersion.ALUResult, reg.writeVersion.SWValue,
                reg.writeVersion.WriteRegNum);

  std::string writeMsg = &writeBuf[0];

  const char readInfo[] = "ALUResult=%X, SWValue=%X, WriteRegNum=%d";

  std::vector<char> readBuf(sizeof(readInfo) + 32);

  std::snprintf(&readBuf[0], readBuf.size(), readInfo,
                reg.readVersion.ALUResult, reg.readVersion.SWValue,
                reg.readVersion.WriteRegNum);

  std::string readMsg = &readBuf[0];

  return os << "EX/MEM_Write (written to by the EX stage)\n"
            << "----------------------------------------\n"
            << reg.writeVersion.signal << "\n"
            << "----------------------------------------\n"
            << writeMsg << "\n"
            << "EX/MEM_Read (read by the MEM stage)\n"
            << "----------------------------------------\n"
            << reg.readVersion.signal << "\n"
            << "----------------------------------------\n"
            << readMsg << "\n";
}
