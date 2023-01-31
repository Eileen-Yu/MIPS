#ifndef INST_PARSER_H
#define INST_PARSER_H

#include <map>
#include <string>

unsigned int getOpcode(unsigned int code) { return (code & 0xFC000000) >> 26; }

unsigned int parseRS(unsigned int code) { return (code & 0x03E00000) >> 21; }

unsigned int parseRT(unsigned int code) { return (code & 0x001F0000) >> 16; }

unsigned int parseRD(unsigned int code) { return (code & 0x0000F800) >> 11; }

unsigned int getFuncValue(unsigned int code) { return code & 0x0000003F; }

std::map<int, std::string> rtypeOps{
    {0x20, "add"}, {0x22, "sub"}, {0x24, "and"}, {0x25, "or"}, {0x2A, "slt"},
};

// transfer RType funct code to name
std::string getRTypeOp(unsigned int funcValue) {
  return rtypeOps.at(funcValue);
}

short getSignExtImm(unsigned int code) {
  short signExtImm = code & 0x0000FFFF;
  return signExtImm;
}

struct LoadType {
  unsigned int op;
  unsigned int rt;
  unsigned int rs;
  unsigned int rd;
  unsigned int func;
  short signExtImm;
};

LoadType decodeRType(unsigned int code) {
  LoadType result;

  // int funcValue = getFuncValue(code);
  // std::string op = getRTypeOp(funcValue);

  result.rd = parseRD(code);
  result.rs = parseRS(code);
  result.rt = parseRT(code);
  result.op = 0;
  result.func = getFuncValue(code);

  return result;
}

LoadType decodeIType(unsigned int code, unsigned int addr) {
  LoadType result;

  result.op = getOpcode(code);
  result.rt = parseRT(code);
  result.rs = parseRS(code);
  result.signExtImm = getSignExtImm(code);
  result.func = 0;
  return result;
}

LoadType dissemble(unsigned int code, unsigned int addr) {
  unsigned int opcode = getOpcode(code);

  LoadType error;

  switch (opcode) {
  case 0:
    return decodeRType(code);
  case 0x20:
  case 0x28:
    return decodeIType(code, addr);
  default:
    return error;
  }
}

#endif /*INST_PARSER_H*/
