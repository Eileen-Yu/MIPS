#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "control-signal.hpp"
#include "memwb-reg.hpp"
#include "utils.hpp"

MEMWBReg::MEMWBReg() {
  writeVersion = initMEMWBMeta();
  readVersion = initMEMWBMeta();
}

void MEMWBReg::setWriteVersion(MEMWBMeta meta) { writeVersion = meta; }

void MEMWBReg::copyFromWriteToRead() { readVersion = writeVersion; }

MEMWBMeta MEMWBReg::getReadVersion() { return readVersion; }

MEMWBMeta MEMWBReg::getWriteVersion() { return writeVersion; }

std::ostream &operator<<(std::ostream &os, const MEMWBReg &reg) {
  const char writeInfo[] = "LWDataValue=%X, ALUResult=%X, WriteRegNum=%d";

  std::vector<char> writeBuf(sizeof(writeInfo) + 32);

  std::snprintf(&writeBuf[0], writeBuf.size(), writeInfo,
                reg.writeVersion.LWDataValue, reg.writeVersion.ALUResult,
                reg.writeVersion.WriteRegNum);

  std::string writeMsg = &writeBuf[0];

  const char readInfo[] = "LWDataValue=%X, ALUResult=%X, WriteRegNum=%d";

  std::vector<char> readBuf(sizeof(readInfo) + 32);

  std::snprintf(&readBuf[0], readBuf.size(), readInfo,
                reg.readVersion.LWDataValue, reg.readVersion.ALUResult,
                reg.readVersion.WriteRegNum);

  std::string readMsg = &readBuf[0];

  return os << "MEM/WB_Write (written to by the MEM stage)\n"
            << "----------------------------------------\n"
            << reg.writeVersion.signal << "\n"
            << "----------------------------------------\n"
            << writeMsg << "\n"
            << "MEM/WB_Read (read by the WB stage)\n"
            << "----------------------------------------\n"
            << reg.readVersion.signal << "\n"
            << "----------------------------------------\n"
            << readMsg << "\n";
}
